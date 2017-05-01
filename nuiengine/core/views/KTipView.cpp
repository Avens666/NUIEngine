#include "KTipView.h"
#include "DataSync.h"
#include "KShapeDrawable.h"
#include "KScreen.h"
/////////////////// KTipBase //////////////////////////
int KTipBase::m_s_id = 0;
KTipBase::KTipBase()
{
	m_show_time = 5000;
	m_max_w = 200;
	m_b_auto_hide = TRUE;

	m_tip_id = ++m_s_id;
}

KTipBase::~KTipBase()
{
}

int KTipBase::getTipID()
{
	return m_tip_id;
}

void KTipBase::init(KScreen* screen, RERect rect, kn_string str)
{
	if (!screen)
	{
		return;
	}
	int r = 5;
	int step = 5;
	KRectShape* p_rect_shape = new KRoundRectShape(rect,r,r);
	REColor clr = ColorSetARGB(255, 255,255,225) ;
	p_rect_shape->setFillColor(clr);
	clr = ColorSetARGB(255, 0,0,0);
	p_rect_shape->setStrokeColor(clr );
	p_rect_shape->setStrokeWidth(1);
	p_rect_shape->initPaint();
	m_bk_drawable  = KShapeDrawable_PTR(new KShapeDrawable(p_rect_shape));
	m_bk_drawable->setRect(0,0,m_rect.width(),m_rect.height() );
	m_bk_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_bk_drawable->setAutoSize(TRUE);
	addDrawable(m_bk_drawable);

	m_txt_drawable = KTextMultiLineDrawable_PTR(new KTextMultiLineDrawable(ColorSetARGB(255,20,20,20),12, REPaint::kLeft_Align) );
	m_txt_drawable->setRect(r,r, 190, 30);
	m_txt_drawable->SetText(str);
	addDrawable(m_txt_drawable);

	int w = m_txt_drawable->getMaxWidth()+r*2;
	int h = m_txt_drawable->getLine()*12 + r*2;
	
	//计算tip显示位置，优先顺序为 下  上  右 左 
	int x , y;
	if (rect.bottom() + h + step + 40> screen->GetHeight() )
	{//显示在rect上面
		y = rect.top() - h - step;
	}
	else
	{//显示在rect下面
		y = rect.bottom() + step;
	}

	x = rect.centerX() - w/2;
	if (x  + 40 > screen->GetWidth() )
	{//显示在rect左面
		x = screen->GetWidth() - w;
	}
	else if(x < step)
	{//显示在rect右面
		x = step;
	}

	setRect(x,y,w,h);
	m_bk_drawable->setRect(0,0, w, h );
	m_txt_drawable->setRect(r,r, w - r*2, h - r*2);
}

kn_bool KTipBase::isAutoHide()
{
	return m_b_auto_hide;
}

void KTipBase::setAutoHide(kn_bool b)
{
	m_b_auto_hide = b;
}

void  KTipBase::setBKDrawable( KDrawable_PTR p )
{
	writeLock lock(m_lst_drawable_mutex);
	m_bk_drawable = p;
	m_lst_drawable[0] = p;

	m_bk_drawable->SetShow(TRUE);

	m_bk_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
}