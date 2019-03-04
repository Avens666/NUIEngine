package com.nui;

import java.io.File;
import java.nio.ByteBuffer;
import java.util.Timer;
import java.util.TimerTask;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.View;
import android.view.View.OnLongClickListener;
import android.view.View.OnTouchListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.Toast;

@SuppressLint("NewApi")
public class MainActivity extends Activity
{
    protected static SkiaDrawView m_SkiaView = null;
    protected GestureDetector mGestureDetector;
    protected ScaleGestureDetector mScaleGestureDetector;
    
    protected boolean m_bGuestDone = false;
    // HMI KMessageType.h enum KMESSAGETYPE 必须和C++一致
    public final int KMSG_KEYPRESS 		= 1;
    public final int KMSG_KEYUP 		= 3;

    public final int KMSG_LBBUTTONDOWN 	= 5;
	public final int KMSG_LBBUTTONUP 	= 6;
	public final int KMSG_MOUSEMOVE 	= 11;
	public final int KMSG_LBUTTONDBLCLK = 12;
	public final int KMSG_TIMER 		= 28;
	
	public final int KMSG_TOUCH_UP 		= 17;		//	多点抬起
	public final int KMSG_TOUCH_MOVE 	= 18;		//	多点移动
	public final int KMSG_TOUCH_DOWN 	= 19;		//	多点按下 (2或2点以上)
	
	public static Handler m_Handler = null;
	
	// m_Handler message
	public static final int JMSG_TIMER = 0;
	public static final int JMSG_REQUEST_RENDER = 1;
	public static final int JMSG_IME_ON = 100;
	public static final int JMSG_IME_OFF = 101;
	public static final int TMSG_SYSTEM_EXIT = 102;
	public static final int TMSG_SYSTEM_INIT_OK = 103;	
	
	public static final String TAG = "nui";
	
	EditText m_edittext = null;
	
	//zhic 针对电阻屏 鼠标消息过滤
	public boolean m_b_mouse_filter = true;		//false表示电容屏 ，true表示电阻屏，电阻屏需要过滤屏幕抖动
	public boolean m_b_mouse_up_filter = true;
	public final int MOUSE_MOV_FILTER_STEP 	= 5; //过滤抖动消息的阈值	
	protected  int m_mouse_pos_bak_x = 0; //记录上一次有效鼠标位置值
	protected  int m_mouse_pos_bak_y = 0; //记录上一次有效鼠标位置值
	protected  int m_mouse_up_pos_x = 0; //
	protected  int m_mouse_up_pos_y = 0; //
	public boolean m_b_mouse_up_flag = true;
	public static final int JMSG_MOUSE_FILTER_TIMER = 2000;
	///////////////////////
	// nui 路径
	String m_strDataPath = "";
    
    //控制onSizeChanged()调用
    boolean m_bFirstSizeChanged = false;
	
    /* this is used to load the library on application
     * startup. 
     */
    static {   	   
        try {
            // Load Skia and then the app shared object in this order
            System.loadLibrary("skia_android");
            System.loadLibrary("nuiengine");
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, "Link Error: " + e);
        }
    }
				
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);//去掉标题栏  
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                                  WindowManager.LayoutParams.FLAG_FULLSCREEN);//去掉信息栏
    	// FrameLayout
        ViewGroup.LayoutParams framelayout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                       ViewGroup.LayoutParams.FILL_PARENT);
        FrameLayout framelayout = new FrameLayout(this);
        framelayout.setLayoutParams(framelayout_params);    
        ViewGroup.LayoutParams edittext_layout_params =
                new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                           ViewGroup.LayoutParams.WRAP_CONTENT);
        // 添加一个EditView 来调用输入法
        m_edittext = new EditText(this);
        m_edittext.setLayoutParams(edittext_layout_params);
        m_edittext.setImeOptions(EditorInfo.IME_FLAG_NO_EXTRACT_UI);
    		
    	// ...add to FrameLayout
        framelayout.addView(m_edittext);
        m_edittext.clearFocus();
        // Makes and sets a SkiaDrawView as the only thing seen in this activity
        m_SkiaView = new SkiaDrawView(this);
        framelayout.addView(this.m_SkiaView);
        setContentView(framelayout);
        
        // 保持屏幕唤醒
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        
    	// 消息响应
    	m_Handler = new Handler() {
    		public void handleMessage(Message msg) 
    		{
    			switch (msg.what) 
    			{
    			case JMSG_TIMER:
    				NUICore.sendNUIMessage(KMSG_TIMER, 0, 0);
    				break;
    			case JMSG_REQUEST_RENDER:
    				m_SkiaView.postInvalidate();
    				break;
    			case TMSG_SYSTEM_EXIT:
					finish();
					System.exit(0);
					break;
    			case JMSG_MOUSE_FILTER_TIMER:
    				if(m_b_mouse_up_flag)
    				{
    					NUICore.sendNUIMessage(KMSG_LBBUTTONUP, m_mouse_up_pos_x, m_mouse_up_pos_y);
    				}    				
    				break;
    			default:
    				break;
    			}
    		}
    	};	
        
        mGestureDetector = new GestureDetector(this, new MyOnGestureListener());
        m_bGuestDone = false;

        // Set a timer that will periodically request an update of the SkiaDrawView
        Timer fAnimationTimer = new Timer();
        fAnimationTimer.schedule(new TimerTask() {
            public void run()
            {
                // This will request an update of the SkiaDrawView, even from other threads
                //m_SkiaView.postInvalidate();
            	m_Handler.sendEmptyMessage(JMSG_TIMER);
            }
        }, 1000, 20); // 0 means no delay before the timer starts; 5 means repeat every 5 milliseconds
        
		// 设置触摸监听
        m_SkiaView.setOnTouchListener(touchEvent);
    	
    	View decorView = getWindow().getDecorView();
    	decorView.setOnSystemUiVisibilityChangeListener
    	        (new View.OnSystemUiVisibilityChangeListener() {
    	    @Override
    	    public void onSystemUiVisibilityChange(int visibility) {
    	        // Note that system bars will only be "visible" if none of the
    	        // LOW_PROFILE, HIDE_NAVIGATION, or FULLSCREEN flags are set.
    	        if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
    	            // TODO: The system bars are visible.
    	        	hideSystemUI();
    	        } else {
    	            // TODO: The system bars are NOT visible. 
    	        }
    	    }
    	});	
		Log.d(TAG, "---------onCreate");
    }

    protected int registerCallback()
    {
    	int iRegister = NUICore.registerStaticCallback("com/nui/MainActivity", "requestRenderFromNUI", "()V", NUICore.CB_FLIP);
    	Log.d(TAG, "MainActivity CB_FLIP = " + iRegister);
    	iRegister = NUICore.registerStaticCallback("com/nui/MainActivity", "sendJavaMessage", "(IIILjava/lang/String;)I", NUICore.CB_SEND_JAVAMSG);
    	Log.d(TAG, "MainActivity CB_SEND_JAVAMSG = " + iRegister);
    	return iRegister;
    }
    
    protected int unregisterCallback()
    {
    	int iRegister = 0;
    	for(int i = 0; i < NUICore.CB_SIZE; i++)
    	{
    		iRegister = NUICore.unregisterCallback(i);
    	} 	
    	return iRegister;
    }
    
    
	@Override
	protected void onResume() 
	{
		super.onResume();
		registerCallback();	
		//Immersive Mode 沉浸式
		hideSystemUI();	
	}
    
	@Override
	protected void onPause() 
	{
		super.onPause();
		unregisterCallback();
	}
	
	protected void onStop() 
	{
		super.onStop();
	}
	
	//开启全屏模式,进入Immersive Mode沉浸模式
	private void hideSystemUI() {
		 if( android.os.Build.VERSION.SDK_INT >= 19 ){ 
			 int uiFlags = View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
					   View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
					   View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
					   View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
					   View.SYSTEM_UI_FLAG_FULLSCREEN |
					   View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;	 
			 getWindow().getDecorView().setSystemUiVisibility(uiFlags);
		 }
	}
	
	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		 if (hasFocus) {
			 hideSystemUI();
		 }
	}
	
    private String getActionName(int action) {
        String name = "";
        switch (action) {
            case MotionEvent.ACTION_DOWN: {
                name = "ACTION_DOWN";
                break;
            }
            case MotionEvent.ACTION_MOVE: {
                name = "ACTION_MOVE";
                break;
            }
            case MotionEvent.ACTION_UP: {
                name = "ACTION_UP";
                break;
            }
            default:
            break;
        }
        return name;
    } 
 
    class MyOnGestureListener extends SimpleOnGestureListener {
        @Override
        public boolean onSingleTapUp(MotionEvent e) {
            return false;
        }

        @Override
        public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {         
            return false;
        }

        @Override
        public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
            return false;
        }

        @Override
        public void onShowPress(MotionEvent e) {
        }

        @Override
        public void onLongPress(MotionEvent e) {
        }
        
        @Override
        public boolean onDown(MotionEvent e) {
            return false;
        }

        @Override
        public boolean onDoubleTap(MotionEvent e) {
            return false;
        }

        @Override
        public boolean onDoubleTapEvent(MotionEvent e) {
            Log.i(TAG, "onDoubleTapEvent-----" + getActionName(e.getAction()));   
            // 确认双击
            NUICore.sendNUIMessage(KMSG_LBUTTONDBLCLK, (int)e.getX(), (int)e.getY());
            return false;
        }

        @Override
        public boolean onSingleTapConfirmed(MotionEvent e) {
            return false;
        }
    }
    
    DialogInterface.OnClickListener listener = new DialogInterface.OnClickListener() {
		
		@Override
		public void onClick(DialogInterface arg0, int arg1) {
			// TODO Auto-generated method stub
			switch(arg1)
			{
			case AlertDialog.BUTTON_POSITIVE:
				{
					finish();
					System.exit(0);
				}
				break;
			case AlertDialog.BUTTON_NEGATIVE:
				hideSystemUI();
				break;
			default:
				break;
			}
		}
	};
    
    OnLongClickListener longClickEvent = new View.OnLongClickListener() {  
        @Override
        public boolean onLongClick(View v) 
        {
        	Log.d (TAG, "onLongClick");
            return false;
        }
    };
    
    // zhic 用于电阻屏不稳定的mouse up消息过滤
    Timer m_mouse_up_delay_timer = new Timer();   
	OnTouchListener touchEvent = new OnTouchListener() {
		@SuppressWarnings("deprecation")
		@Override
		public boolean onTouch(View v, MotionEvent event) 
		{
			Log.i(getClass().getName(), "onTouch-----" + getActionName(event.getAction()));
			m_bGuestDone = false; 
			// 获取触摸点坐标
			int iScreenX = (int) event.getX();
			int iScreenY = (int) event.getY();
			// 计算Action的真实值
			int action = event.getAction();
			int iPointCount = event.getPointerCount();
			// 添加触摸事件处理
			switch (action & MotionEvent.ACTION_MASK) {
			case MotionEvent.ACTION_DOWN:
				NUICore.sendNUIMessage(KMSG_LBBUTTONDOWN, iScreenX, iScreenY);
				if( m_b_mouse_filter )
				{				
					m_b_mouse_up_flag = false;
					m_mouse_pos_bak_x = iScreenX;
					m_mouse_pos_bak_y = iScreenY;
				}
				Log.i(getClass().getName(), "onTouch-----" + getActionName(event.getAction()) + "iScreenX = " + iScreenX +"iScreenY = " + iScreenY);
				break;
			case MotionEvent.ACTION_MOVE:
				//zhic 过滤电阻屏抖动消息
				if( m_b_mouse_filter )
				{
					if( Math.abs(iScreenX - m_mouse_pos_bak_x) <= MOUSE_MOV_FILTER_STEP && 
						Math.abs(iScreenY - m_mouse_pos_bak_y) <= MOUSE_MOV_FILTER_STEP )
					{
						break;
					}
				}
				NUICore.sendNUIMessage(KMSG_MOUSEMOVE, iScreenX, iScreenY);
				m_mouse_pos_bak_x = iScreenX;
				m_mouse_pos_bak_y = iScreenY;
				if(iPointCount >= 2)
				{ 
					ByteBuffer buff = ByteBuffer.allocate(256);					
					for(int i = 0; i < iPointCount; i++)
					{
						buff.putInt((int)event.getX(i));
						buff.putInt((int)event.getY(i));
					}	
					NUICore.sendNUIMessageBuf(KMSG_TOUCH_MOVE, iPointCount, 0, buff.position(), buff.array());
					Log.i("OnScaleGestureListener", "ACTION_POINTER_MOVE");
				}
				break;
			case MotionEvent.ACTION_UP:
				if( m_b_mouse_filter )
				{
					m_b_mouse_up_flag = true;
					m_mouse_up_pos_x = iScreenX;
					m_mouse_up_pos_y = iScreenY;
					m_Handler.sendEmptyMessage(JMSG_MOUSE_FILTER_TIMER);
					//up之后 备份坐标置为无效值
					m_mouse_pos_bak_x = 9999;
					m_mouse_pos_bak_y = 9999;
				}
				else
				{
					NUICore.sendNUIMessage(KMSG_LBBUTTONUP, iScreenX, iScreenY);
				}
				break;
			case MotionEvent.ACTION_POINTER_DOWN:
				if(iPointCount >= 2)
				{
					ByteBuffer buff = ByteBuffer.allocate(256);					
					for(int i = 0; i < iPointCount; i++)
					{
						buff.putInt((int)event.getX(i));
						buff.putInt((int)event.getY(i));
					}	 			
					NUICore.sendNUIMessageBuf(KMSG_TOUCH_DOWN, iPointCount, 0, buff.position(), buff.array());	
				}
				break;
			case MotionEvent.ACTION_POINTER_UP:  
				{
					ByteBuffer buff = ByteBuffer.allocate(256);					
					for(int i = 0; i < iPointCount; i++)
					{
						buff.putInt((int)event.getX(i));
						buff.putInt((int)event.getY(i));
					}		
					NUICore.sendNUIMessageBuf(KMSG_TOUCH_UP, iPointCount, 0, buff.position(), buff.array());
				}
				break;
			}
			mGestureDetector.onTouchEvent(event);		
			return true;
		}
	};

    static long s_lRequestTicks = 0;
    
	// call by NUI thread
	public static void requestRenderFromNUI() {
		if (m_Handler != null) {			
			s_lRequestTicks = System.currentTimeMillis();
			m_SkiaView.postInvalidate();
		}
	}
	
	public static int sendJavaMessage(int iMsgID, int iArg1, int iArg2, String strArg)
	{
		Log.d(TAG, String.format("sendJavaMessage %d, %d, %d, %s", iMsgID, iArg1, iArg2, strArg));
		if (m_Handler != null) 
		{
			final Message msg = new Message();
			msg.what = iMsgID;
			msg.arg1 = iArg1;
			msg.arg2 = iArg2;
			msg.obj = strArg;
  			m_Handler.sendMessage(msg);
		}
		return 0;
	}
	
    @Override
 	protected void onDestroy() {
 		super.onDestroy();
 	}
    
    private class SkiaDrawView extends  View {
        Bitmap m_SkiaBitmap = null;    
        int m_width = 0;
        int m_height = 0;   
        // 真实的View大小
        int m_iDeviceWidth = 0;
        int m_iDeviceHeight = 0;      
        // HMI Engine 的surface大小
        int m_iHMIEngineWidth = 1028;
        int m_iHMIEngineHeight = 720;  
        public SkiaDrawView(Context ctx) {
            super(ctx);
        }
        
        @Override
        protected void onSizeChanged(int w, int h, int oldw, int oldh)
        {
        	Log.d(TAG, String.format("onSizeChanged w(%d)h(%d), old w(%d) h(%d)", w, h, oldw, oldh));
        	// onSizeChanged 只允许调用一次
        	if(m_bFirstSizeChanged)
        	{
        		return;
        	}
        	m_bFirstSizeChanged = true;
        	m_width = w;
        	m_height = h;
        	
        	m_iDeviceWidth = w;
        	m_iDeviceHeight = h;
        	
        	m_iHMIEngineWidth = w;
        	m_iHMIEngineHeight = h;
        	//初始化加载本地数据
			DataOperator.initialize(getContext());
        	// Create a bitmap for skia to draw into
        	String strDataPath = DataOperator.getFileDir();
        	String savPath = Environment.getExternalStorageDirectory()+"/nuiengine/sav/";
        	File file = new File(savPath);
        	if(!file.exists()){
        		file.mkdirs();
        	}
        	if(strDataPath.isEmpty())
        	{
        		Toast.makeText(getApplicationContext(), "NO MAP DATA, please check /sdcard/nui",
        			     Toast.LENGTH_SHORT).show();
        	}
        	else
        	{
        		m_strDataPath = strDataPath;
        		//使用固定大小的bitmap
	        	m_SkiaBitmap = Bitmap.createBitmap(m_iHMIEngineWidth, m_iHMIEngineHeight, Bitmap.Config.RGB_565);
	            // 只传递bitmap 参数
				NUICore.initializeEngine(m_strDataPath, savPath, m_SkiaBitmap, w, h);
	        }
        }

        @Override
        protected void onDraw(Canvas canvas) {
        	if(m_SkiaBitmap == null)
        	{
        		return;
        	}
        	NUICore.lockBitmap();     
            Rect src = new Rect (0, 0, m_iHMIEngineWidth, m_iHMIEngineHeight);
            Rect dst = new Rect (0, 0, m_width, m_height);
         	Paint paint = new Paint();
         	paint.setFilterBitmap(true);
         	//Log.d(TAG, String.format("m_iHMIEngineWidth w(%d)h(%d), m_width w(%d) h(%d)", m_iHMIEngineWidth, m_iHMIEngineHeight, m_width, m_height));
           	NUICore.drawIntoBitmap(m_SkiaBitmap, m_width, m_height, 0);
         	if(m_iHMIEngineWidth == m_width && m_iHMIEngineHeight == m_height)
         	{
         		canvas.drawBitmap(m_SkiaBitmap, 0, 0, null);	
         	}
         	else
         	{
         		canvas.drawBitmap(m_SkiaBitmap, src,dst, paint);
         	}
            NUICore.unlockBitmap();
        }
    }	
}
