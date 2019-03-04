#include "NuiMainView.h"
#include "DataSync.h"
#include "KScreen.h"
#include "KEasingCurve.h"
#include "KColorTextDrawable.h"
#include "KSurfaceManager.h"
#include "KMoveableView.h"
#include "KWaveEffectView.h"
KMainView::KMainView()
{

}


KMainView::~KMainView()
{
}

void KMainView::init()
{
	//设置纯色背景
	KColorDrawable_PTR bk_drawable; 
	//bk_drawable = KColorDrawable_PTR(new KColorDrawable(0xFF262f35) );
	//bk_drawable->SetRect(RERect::MakeXYWH(0, 0, m_rect.width(), m_rect.height()) );
	//addDrawable(bk_drawable);
	createColorDrawableHelper(&bk_drawable, 0, 0, m_rect.width(), m_rect.height(), 0xFF262f35 );

	KTextDrawable_PTR pDrawText = KTextDrawable_PTR (new KTextDrawable(_T("Hello NUI"), RE_ColorGREEN, 32, REPaint::kCenter_Align) );
	pDrawText->SetRect(RERect::MakeXYWH(0, 20, m_rect.width(), 50)); //文本范围
	addDrawable(pDrawText);

	KImageDrawable_PTR pDrawable = KImageDrawable_PTR(new KImageDrawable(getSurfaceManager()->GetSurface(_T("logo.png")), TRUE  ) );
	pDrawable->SizeToImage();
	pDrawable->SetRect(pDrawable->GetRect());

	int TIME_MAX = 4000;
	int VIEW_SIZE = 48; //动画控件个数
	int iDaleyTime = (TIME_MAX - 750) / VIEW_SIZE;
	REPoint *pStartState = new REPoint[VIEW_SIZE];	//矩阵布局
	REPoint *pEndState = new REPoint[VIEW_SIZE];		//环形布局
	int png_size = pDrawable->GetRect().width();
	int w = 600, h = 600, offset_x = m_rect.width()-w, offset_y = m_rect.height() - h; //定义动画范围

	int iStartX = (w - png_size) / 2;
	int iStartY = (h- png_size) / 2;

	// 
	int ROW_SIZE = 8;  
	// 生成矩阵布局坐标组
	for(int i = 0; i < VIEW_SIZE; i++)
	{
		int row = i / ROW_SIZE;
		int col = i - row * ROW_SIZE;
		int x = col * png_size + offset_x;
		int y = row * png_size + offset_y;
		pStartState[i].set(x, y);
	}

	// 生成环形布局坐标组
	int r = (h - png_size*2) / 2;
	float delta = 2 * 3.1415926 / VIEW_SIZE;
	float sita = 0;
	for(int i = 0; i < VIEW_SIZE; i++)
	{
		int x = r * cos(sita) + iStartX + offset_x;
		int y = r * sin(sita) + iStartY + offset_y;
		pEndState[i].set(x, y);
		sita += delta;
	}

	//创建控件组
	KMoveableView_PTR pViewList[50];
	for (kn_int i = 0; i < VIEW_SIZE; i++)
	{
		KMoveableView_PTR  pView = KMoveableView_PTR (new KMoveableView());
		RERect rect = RERect::MakeXYWH(pStartState[i].x(), pStartState[i].y(), png_size, png_size);
		pView->Create(rect);
		pView->addDrawable(pDrawable);
		pViewList[i] = pView;
		AddView(pView);
	}

	//定义动画
	for(int i = 0; i < VIEW_SIZE; i++)
	{
		CNUIAnimation* pNUIAnimation = new CNUIAnimation (TIME_MAX - 4000*i/VIEW_SIZE, 0, 4000*i/VIEW_SIZE); //iDaleyTime * i);
		CPropertyPos* pPosDst = new CPropertyPos(pViewList[i], pEndState[i].x(),pEndState[i].y());
		CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPosDst);
		pNUIAnimation->addAnimationData(pAnimationData);
		pNUIAnimation->setLoopType(CNUIAnimation::LOOP_PINGPONG);
		pNUIAnimation->setLoopTimes(LOOP_ALWAYS);
		m_ani_thread.addAnimation(pNUIAnimation);
	}

	//加一个涟漪效果控件
	KWaveEffectView_PTR wave_view = KWaveEffectView_PTR(new KWaveEffectView() );
	wave_view->Create(10,200,350,95);
	wave_view->init(_T("nuilogo.png") );
	AddView( wave_view );

	delete[] pStartState;
	delete[] pEndState;

	//启动动画
	wave_view->play(1000);
	m_ani_thread.Start();
}
