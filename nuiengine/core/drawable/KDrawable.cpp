//  **************************************
//  File:        KDrawable.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description: This code is part of NUI Engine (NUI Graphics Lib)
//  Comments:
//  Rev:         2
//  Created:     2017/4/11
//  Last edit:   2017/4/28
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
//  ***************************************
#include "KDrawable.h"
#include "KFontManager.h"
#include "FilePath.h"

int g_da_count = 0;

KShowObject::KShowObject()
{
	m_b_clip = false;
	m_rect_clip.setEmpty();

	m_rect.setXYWH(0,0,100,100);
	m_rect_clip.setEmpty();
	m_rect_origin.setEmpty();
	m_rect_clip_origin .setEmpty();
	m_b_show = TRUE;
	m_i_opacity = 255;

	m_LRMethod_left = KVIEW_LRMethod_Parent_Left;
	m_LRMethod_right = KVIEW_LRMethod_Parent_Left;
	m_BTMethod_top = KVIEW_BTMethod_Parent_Top;
	m_BTMethod_bottom = KVIEW_BTMethod_Parent_Top;
}

KShowObject::~KShowObject()
{
}

RERect KShowObject::GetRect()
{
	return m_rect;
}

kn_bool KShowObject::isShow()
{
	return m_b_show;
}

void KShowObject::SetShow(kn_bool bShow)
{
	m_b_show = bShow;
}

RERect KShowObject::getRectOrigin()
{
	return m_rect_origin;
}

void KShowObject::SetClip(RERect rect)
{
	m_rect_clip = rect;
	m_b_clip = true;
}

RERect KShowObject::GetClip ()
{
	return m_rect_clip;

}

void KShowObject::SetClipEnable(kn_bool b)
{
//	writeLock lock(m_lst_drawable_mutex);
	m_b_clip = b;
}

void KShowObject::SetViewChangeType(IN LRMethod LeftType,IN LRMethod RightType,IN BTMethod TopType,IN BTMethod BottomType)
{

	m_LRMethod_left = LeftType;
	m_LRMethod_right = RightType;
	m_BTMethod_top = TopType;
	m_BTMethod_bottom = BottomType;
}

RERect  KShowObject::calcSizeChange(RERect& src_rect,  RERect& parent_rect, RERect& parent_rect_origin)
{
	RERect rect_change;
	SkScalar fWPercent = parent_rect.width() / parent_rect_origin.width();
	SkScalar fHPercent = parent_rect.height() / parent_rect_origin.height();

	switch(m_LRMethod_left)
	{
	case KVIEW_LRMethod_Parent_Left:
		rect_change.fLeft = src_rect.fLeft;
		break;
	case KVIEW_LRMethod_Parent_Right:
		rect_change.fLeft = parent_rect.fRight - (parent_rect_origin.fRight - src_rect.fRight) - src_rect.width();
		break;
	case KVIEW_LRMethod_Parent_LRMiddle:
		if (parent_rect_origin.centerX() >= src_rect.fLeft)
		{
			rect_change.fLeft = parent_rect.centerX() - (parent_rect_origin.centerX() - src_rect.fLeft);
		}
		else
		{
			rect_change.fLeft = parent_rect.centerX() + (src_rect.fLeft - parent_rect_origin.centerX());
		}
		//if (rect_change.fLeft < 0)
		//{
		//	rect_change.fLeft = 0;
		//}
		break;
	case KVIEW_LRMethod_Parent_LRNone:
		rect_change.fLeft = src_rect.fLeft * fWPercent;
	default:
		break;
	}

	switch(m_LRMethod_right)
	{
	case KVIEW_LRMethod_Parent_Left:
		rect_change.fRight = src_rect.fRight;
		break;
	case KVIEW_LRMethod_Parent_Right:
		rect_change.fRight = parent_rect.width() - (parent_rect_origin.width()- src_rect.fRight);
		break;
	case KVIEW_LRMethod_Parent_LRMiddle:
		if (parent_rect_origin.centerX() >= src_rect.fRight)
		{
			rect_change.fRight = parent_rect.centerX() - (parent_rect_origin.centerX() - src_rect.fRight);
		}
		else
		{
			rect_change.fRight = parent_rect.centerX() + (src_rect.fRight - parent_rect_origin.centerX());
		}
		//if (rect_change.fRight > parent_rect.width())
		//{
		//	rect_change.fRight = parent_rect.width();
		//}
		break;
	case KVIEW_LRMethod_Parent_LRNone:
		rect_change.fRight = src_rect.fRight * fWPercent;
	default:
		break;
	}

	switch(m_BTMethod_top)
	{
	case KVIEW_BTMethod_Parent_Top:
		rect_change.fTop = src_rect.fTop;
		break;
	case KVIEW_BTMethod_Parent_Bottom:
		//	rect_change.fTop = parent_rect.fBottom - (parent_rect_origin.fBottom - src_rect.fBottom) - src_rect.height();
		rect_change.fTop = parent_rect.height() - (parent_rect_origin.height() - src_rect.fTop);
		break;
	case KVIEW_BTMethod_Parent_BTMiddle:
		if (parent_rect_origin.centerY() >= src_rect.fTop)
		{
			rect_change.fTop = parent_rect.centerY() - (parent_rect_origin.centerY() - src_rect.fTop);
		} 
		else
		{
			rect_change.fTop = parent_rect.centerY() + (src_rect.fTop - parent_rect_origin.centerY());
		}
		//if (rect_change.fTop < 0)
		//{
		//	rect_change.fTop = 0;
		//}
		break;
	case KVIEW_BTMethod_Parent_BTNone:
		rect_change.fTop = src_rect.fTop * fHPercent;
	default:
		break;
	}

	switch(m_BTMethod_bottom)
	{
	case KVIEW_BTMethod_Parent_Top:
		rect_change.fBottom = src_rect.fBottom;
		break;
	case KVIEW_BTMethod_Parent_Bottom:
		rect_change.fBottom = parent_rect.height() - (parent_rect_origin.height() - src_rect.fBottom);
		break;
	case KVIEW_BTMethod_Parent_BTMiddle:
		if (parent_rect_origin.centerY() >= src_rect.fBottom)
		{
			rect_change.fBottom = parent_rect.centerY() - (parent_rect_origin.centerY() - src_rect.fBottom);
		} 
		else
		{
			rect_change.fBottom = parent_rect.centerY() + (src_rect.fBottom - parent_rect_origin.centerY());
		}
		//if (rect_change.fBottom > parent_rect.height())
		//{
		//	rect_change.fBottom = parent_rect.height();
		//}
		break;
	case KVIEW_BTMethod_Parent_BTNone:
		rect_change.fBottom = src_rect.fBottom * fHPercent;
	default:
		break;
	}

	if (rect_change.fRight < rect_change.fLeft + 2)
	{
		rect_change.fRight = rect_change.fLeft + 1;
	}

	if (rect_change.fBottom < rect_change.fTop + 2)
	{
		rect_change.fBottom = rect_change.fTop + 1;
	}

	return rect_change;
}

void KShowObject::sizeChange(RERect& parent_rect, RERect& parent_rect_origin)
{
	SetRect( calcSizeChange(m_rect_origin, parent_rect, parent_rect_origin ) );
	if (!m_rect_clip_origin.isEmpty())
	{
		SetClip( calcSizeChange(m_rect_clip_origin, parent_rect, parent_rect_origin ) );
	}
}

void KShowObject::SaveOriginalViewRect()
{
	m_rect_origin = m_rect;
	m_rect_clip_origin = m_rect_clip;
}



void KShowObject::setOpacity(kn_byte value)
{
	if(m_i_opacity != value)
	{
//		InvalidateForParent(GetBoundRect());
		m_i_opacity = value;
	}
}

kn_byte KShowObject::getOpacity()
{
	return m_i_opacity;
}


void KShowObject::setRect(int l, int t, int w, int h)
{
	SetRect(RERect::MakeXYWH(l,t,w,h) );
}

void KShowObject::SetRect(const RERect& rect)
{
	m_rect = rect;
}

// 根据不同平台自动设置绘制文字的编码 UTF16/ UTF32 / UTF8
void SetPaintEncodingAuto(REPaint& paint)
{
#ifdef UNICODE
	if(sizeof(wchar_t) == 2)
	{
		paint.setTextEncoding(REPaint::kUTF16_TextEncoding);
	}
	else if(sizeof(wchar_t) == 4)
	{
		paint.setTextEncoding(REPaint::kUTF32_TextEncoding);
	}
	else
	{
		ASSERT(0);
	}

#else
	paint.setTextEncoding(REPaint::kUTF8_TextEncoding);
#endif

}


///////////////////////////////////////////
KDrawable::KDrawable()
{
	m_type = KDRAWABLE_BASE;
	m_b_auto_size = FALSE;
	g_da_count++;
	m_b_clip_rect = TRUE;
	// 设置统一的文字字符集和字体 [2014-4-30]
	SetPaintEncodingAuto(m_paint);
#ifndef WIN32

	m_paint.setTypeface(GetFontManagerSingleton()->GetFontFromFile(_T("./resource/msyh.ttf")));
#endif
}

KDrawable::~KDrawable()
{
	m_paint.setTypeface(NULL);
	g_da_count--;
	m_b_show = FALSE;
}

bool KDrawable::BeginDraw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	// 设置绘制区域
	if(pDstSurface == NULL) return false;
	if (m_b_show == FALSE)  return false;


	pDstSurface->Save();
	if (pMatrix)
	{
		pDstSurface->Concat(*pMatrix);
	}

	if (m_b_clip_rect)
	{
		RERect rect = RERect::MakeXYWH(m_rect.left() + iDstX-1, m_rect.top() + iDstY-1, m_rect.width()+1, m_rect.height()+1);
		pDstSurface->ClipRect(rect);
	}

	if(m_b_clip)
	{
		RERect rectClip = RERect::MakeXYWH(iDstX+m_rect_clip.left(), iDstY+m_rect_clip.top(), m_rect_clip.width(), m_rect_clip.height());
		pDstSurface->ClipRect(rectClip);
	}

	return true;
}

void KDrawable::EndDraw(IRESurface* pDstSurface, const REMatrix* pMatrix )
{
	pDstSurface->Restore();
}

void KDrawable::Draw(IRESurface* pSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
}
 
kn_bool KDrawable::checkPoint(int x, int y)
{
	return TRUE;
}

void KDrawable::CreateFromResource()
{
}

void KDrawable::SetViewChangeType(IN LRMethod LeftType,IN LRMethod RightType,IN BTMethod TopType,IN BTMethod BottomType)
{
	KShowObject::SetViewChangeType(  LeftType,RightType, TopType,BottomType );
	setAutoSize(TRUE);
}


REPaint* KDrawable::GetPaint()
{
	return &m_paint;
}

void KDrawable::SetPaint(REPaint* pPaint)
{
	if(pPaint != NULL)
	{
		m_paint = *pPaint;
	}

}

KDrawable::KDRAWABLETYPE KDrawable::getType()
{
	return m_type;
}

kn_bool KDrawable::getAutoSize()
{
	return m_b_auto_size;
}

void KDrawable::setAutoSize(kn_bool b)
{
	m_b_auto_size = b;
}
