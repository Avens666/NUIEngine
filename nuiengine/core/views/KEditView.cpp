//  **************************************
//  File:        KEditView.cpp
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

#include "KEditView.h"
#include "mainforwindows.h"
#include "KTextMultiLineDrawable.h"
#include "CharSet.h"

KEditView::KEditView()
{
	m_e_viewtype = KVIEW_EDIT;
	m_Index = 0;   // 游标位置
	m_bReadOnly = false;
	m_bAllSelect = false;
	m_bMultiLine = false;

   // 左右各留出一定的空间
	m_bk_border_w = 5;
	m_bk_border_h = 5;


	m_i_x_offset = 0;
	m_i_y_offset = 0;
	m_i_cross_offset = 5;
    m_input_max_length = 256;
    /// @brief 默认不是backspace操作 add by weix at 2015-12-01
    m_b_is_backspace_oper = false;
    // backspace操作删除的文字长度 add by weix at 2015-12-01
    m_backspace_char_length = 0;
}

KEditView::~KEditView()
{
}

void KEditView::shared_ptr_inited()
{
	KTextView::shared_ptr_inited();
	m_sign_key_down.connect(this, &KEditView::onKeydown);
	m_sign_focus.connect(this,&KEditView::onFocus);
	m_sign_unfocus.connect(this,&KEditView::onUnFocus);
	m_sign_keyboard_input.connect(this, &KEditView::onKeyInput);
}

kn_bool KEditView::Create(kn_int iX, kn_int iY, kn_int iWidth, kn_int iHeight)
{
	bool  ret = KTextView::Create(iX, iY, iWidth, iHeight);

	return ret;
}

kn_bool KEditView::Create(RERect& rect)
{
	bool  ret = KTextView::Create(rect);

	return ret;
}

void KEditView::Init(int b_w, int b_h,REColor cross_clr )
{
	//文字drawable
	m_bk_border_w = b_w;
	m_bk_border_h = b_h;

	int l = m_bk_border_w;
	int w = m_rect.width() - m_bk_border_w*2;
	SetTextBound(RERect::MakeXYWH(l, 0, w, m_rect.height()));
	SetTextClip(RERect::MakeXYWH(l, 0, w, m_rect.height()));

	//背景drawable
	KDrawable_PTR pDrawable (new KColorDrawable(RE_ColorWHITE));
	pDrawable->SetRect(RERect::MakeXYWH(0, 0, m_rect.width(), m_rect.height()));
	setBKDrawable( pDrawable );

	//游标drawable
	KDrawable_PTR pDrawableCross (new KColorDrawable(cross_clr));
	pDrawableCross->SetRect(RERect::MakeXYWH(0, 0, 1, m_rect.height()));
	pDrawableCross->SetShow(false);
//	pDrawableCross->SetClip(RERect::MakeXYWH(m_bk_border_w,m_bk_border_h, m_rect.width() - m_bk_border_w*2, m_rect.height() - m_bk_border_h*2));
	addDrawable(pDrawableCross);
	m_Cross_drawable = pDrawableCross;

	//全选的背景drawable
	KDrawable_PTR pAllSelect_drawable (new KColorDrawable(0x55000000 ));
//	pAllSelect_drawable->SetClip(RERect::MakeXYWH(m_bk_border_w,m_bk_border_h, m_rect.width() - m_bk_border_w*2, m_rect.height() - m_bk_border_h*2));
	pAllSelect_drawable->SetShow(false);
	addDrawable(pAllSelect_drawable);
	m_AllSelect_drawable = pAllSelect_drawable;
}

void KEditView::setMultiLine(bool bMultiLine)
{
	if (m_bMultiLine == bMultiLine)
	{
		return;
	}
	m_bMultiLine = bMultiLine;
	kn_string text = m_text_drawable->GetText();
	if(bMultiLine)
	{
		KTextDrawable_PTR   ptext_drawable (new KTextMultiLineDrawable(RE_ColorBLACK, GetFontSize(), REPaint::kLeft_Align));
		ptext_drawable->setFont( m_text_drawable->GetFont() );
		setTextDrawable( ptext_drawable );
	}
	else
	{
		KTextDrawable_PTR   ptext_drawable (new KTextDrawable(_T(""), RE_ColorBLACK, GetFontSize(), REPaint::kLeft_Align));
		ptext_drawable->setFont( m_text_drawable->GetFont() );
		setTextDrawable( ptext_drawable );
	}

	SetTextBound(RERect::MakeXYWH(m_bk_border_w, m_bk_border_h, m_rect.width() - m_bk_border_w*2, m_rect.height() - m_bk_border_h*2));
	SetTextClip(RERect::MakeXYWH(m_bk_border_w-1, m_bk_border_h-1, m_rect.width() - m_bk_border_w*2, m_rect.height() - m_bk_border_h*2));

	RemoveAllChar();
	SetText(text);
}

void KEditView::setRowInterval(int v)
{
	if (m_bMultiLine)
	{
		VIEW_PTR(KTextMultiLineDrawable)(m_text_drawable)->setRowInterval(v);
	}
}

void KEditView::SetText( kn_string text )
{
	//REPaint* pPaint = m_text_drawable->GetPaint();

	RemoveAllChar();

	//设置文字
	InsertChar(0,text );

//	UpdateTextDrawable();
}

void KEditView::OnDClick(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(m_bReadOnly)
	{
		return;
	}
	KTextView::OnDClick(iScreenX, iScreenY, pMsg);
	//双击，全选
	SetAllSelect(true);
}

void KEditView::onUpDirect( kn_int iScreenX, kn_int iScreenY , KMessageMouse* pMsg)
{
	//kn_bool b_in = isPointInView(iScreenX, iScreenY) ;
	if (getState() == BS_PRESSED || getState() == BS_FOCUS)
	{
		setState(BS_ACTIVE, false);
		//zhic 重载就为了注释这句
		//unSetViewFocus();

	}
	else
	{//没down的时候，up不归自己处理，让别的控件继续处理
		pMsg->setIdle(KMSG_RETURN_CONTINUE);
	}
}

void KEditView::onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(m_bReadOnly)
	{
		return;
	}
	KTextView::onDownDirect(iScreenX, iScreenY, pMsg);

	SetAllSelect(false);
	int iWinX = iScreenX - m_rect.left();  // 转换成窗口内相对坐标
	int iWinY = iScreenY - m_rect.top();  // 转换成窗口内相对坐标
	RERect TextRect = GetTextBound();
	int x = iWinX - TextRect.fLeft;
	int y = iWinY - TextRect.fTop;


	//单击，计算游标的位置
	int i = 0;
	if(m_bMultiLine)
	{
		// 根据Y算出i
		int linecnt = GetLinecount();
		int fs = m_text_drawable->GetFontSize() * linecnt;
		int t;
		if(getAlignType() == ALIGN_TOP)
		{
			t = m_bk_border_h;
		}
		else if(getAlignType() == ALIGN_CENTER)
		{
			t = (m_rect.height() - fs)/2;
		}
		else  // ALIGN_BOTTOM
		{
			t = m_rect.height() - m_bk_border_h - fs;
		}
		int row = (y - t) / m_text_drawable->GetFontSize() + 1;
		row = row < 1 ? 1 : row;
		row = row > linecnt ? linecnt : row;
		int j = 0;
		int l = TextRect.left();
		for(; j < m_Text.size(); j++)
		{
			if(row == 1)
			{
				break;
			}
			if(m_Text[j].m_TextAtom == _T("\n")) 
			{
				row--;
				l = TextRect.left();
			}
			else
			{
				l += m_Text[j].m_Length;
			}
		

			if(l >= TextRect.right())
			{
				l = TextRect.left();
				j--;
				row--;
			}
		}
		i = j;

		int len = 0;
		for(; i < m_Text.size(); i++)
		{
			if(m_Text[i].m_TextAtom == _T("\n")) 
			{
				break;
			}
			REScalar l = m_Text[i].m_Length;

			len += l;
			if(x - len <= -l/2)
			{
				break;
			}
		}
	}
	else
	{
		int len = 0;
		for(; i < m_Text.size(); i++)
		{
			REScalar l = m_Text[i].m_Length;

            len += l;
            if(x - len <= -l/2)
            {
                break;
            }
        }
    }
    // 不是backspace操作 add by weix at 2015-12-01
    m_b_is_backspace_oper = false;
    //设置游标位置
    SetCrossIndex(i);
    //显示游标
    ShowCross(true);
    showIME(true);

}


void KEditView::onKeydown(KMessageKey* pMsg)
{
	if(!m_bFocus)
	{
		return;
	}
	if (!isShow())
	{
		return;
	}
	//键盘消息
	pMsg->setIdle(KMSG_RETURN_DILE);
	if(m_bReadOnly)
	{
		return;
	}

	//if(pMsg->m_msg_type == KMSG_TYPE_INPUT_ING) //输入法输入中
	//{

	//}
	//else if(pMsg->m_msg_type == KMSG_TYPE_INPUT_END)  //输入法输入结束消息
	//{
	//	KMessageInput* pMsgInput = (KMessageInput*)pMsg;
	//	InsertChar(m_Index,pMsgInput->m_InputStr);
	//}
	//else if(pMsg->m_msg_type == KMSG_TYPE_CHAR) //普通字符消息
	//{
	//	int key = pMsg->m_key_id;
	//	if(key >= _T(' ') && key <= 126)
	//	{
	//		kn_string str;
	//		str = pMsg->m_key_id;
	//		InsertChar(m_Index,str);
	//	}
	//}
	//else  //特殊键消息
	{
		int key = pMsg->m_key_id;
		if(pMsg->m_key_flag != 0) //Ctrl Alt Shift
		{
			bool bCtrlKey = false;
			bool bAltKey = false;
			bool bShift = false;
			if(pMsg->m_key_flag == KMSG_KEYFLAG_CONTROL)
			{
				bCtrlKey = true;
			}
			if(pMsg->m_key_flag == KMSG_KEYFLAG_ALT)
			{
				bAltKey = true;
			}
			if(pMsg->m_key_flag == KMSG_KEYFLAG_SHIFT)
			{
				bShift = true;
			}
			if(bCtrlKey && (key == _T('C') || key == _T('c')))
			{
				Copy();
			}
			if(bCtrlKey && (key == _T('V') || key == _T('v')))
			{
				Paste();
			}
			if(bCtrlKey && (key == _T('X') || key == _T('x')))
			{
				Cut();
			}
			if(bCtrlKey && (key == _T('A') || key == _T('a')))
			{
				SetAllSelect(true);
			}
		}
		else
		{
			if(key == Key_backspaceKey)
			{
				if(m_bAllSelect)
				{
					RemoveAllChar();
				}
				{
                    // 是backspace操作 add by weix at 2015-12-01
                    m_b_is_backspace_oper = true;
					RemoveChar(m_Index-1);
				}
			}
			else if(key == Key_deleteKey)
			{
				if(m_bAllSelect)
				{
					RemoveAllChar();
				}
				else
				{
                    // 不是backspace操作 add by weix at 2015-12-01
                    m_b_is_backspace_oper = false;
					RemoveChar(m_Index);
				}
			}
			else if(key == Key_leftKey)
			{
				if (m_bAllSelect)
				{
					SetAllSelect(false);
					ShowCross(true);
				}
                // 不是backspace操作 add by weix at 2015-12-01
                m_b_is_backspace_oper = false;
                SetCrossIndex(m_Index-1);
            }
            else if(key == Key_rightKey)
            {
				if (m_bAllSelect)
				{
					SetAllSelect(false);
					ShowCross(true);
				}
                // 不是backspace操作 add by weix at 2015-12-01
                m_b_is_backspace_oper = false;
                SetCrossIndex(m_Index+1);
            }
            else if(key == Key_homeKey)
            {
				if (m_bAllSelect)
				{
					SetAllSelect(false);
					ShowCross(true);
				}
				SetCrossIndex(0);
			}
			else if(key == Key_endKey)
			{
				if (m_bAllSelect)
				{
					SetAllSelect(false);
					ShowCross(true);
				}
				SetCrossIndex(m_Text.size());
			}
			else if(key == Key_returnKey)
			{
				if(m_bMultiLine)
				{
					kn_string str = _T("");
					str += '\n';
					InsertChar(m_Index,str);
				}
			}
		}
	}
}

void  KEditView::onKeyInput(KMessageInput* pMsg)
{
	if(!m_bFocus)
	{
		return;
	}
	//键盘消息
	pMsg->setIdle(KMSG_RETURN_DILE);
	if(m_bReadOnly)
	{
		return;
	}

	if(pMsg->m_msg_type == KMSG_TYPE_INPUT_ING) //输入法输入中
	{

	}
	else if(pMsg->m_msg_type == KMSG_TYPE_INPUT_END)  //输入法输入结束消息
	{
		InsertChar(m_Index,pMsg->m_InputStr);
	}
	else if(pMsg->m_msg_type == KMSG_TYPE_CHAR) //普通字符消息
	{
		int key = pMsg->m_key_id;
		if((key >= _T(' ') && key <= 126)  // ASCII
			|| (key > 128)		// unicode
			)
		{
			kn_string str;
			str = pMsg->m_key_id;
			InsertChar(m_Index,str);
		}
	}
	else if(pMsg->m_msg_type == KMSG_TYPE_IME_UPDATE)
	{	// android edittext change 获取的输入
		KMessageEditTextIME* pMsgIME = (KMessageEditTextIME*)pMsg;
		string strText = KnChars2String(pMsgIME->m_InputStr.c_str(), CP_UTF8);
		LOGI ("KEditView::onKeyInput strText = %s, start(%d), count(%d)", strText.c_str(), pMsgIME->m_i_start, pMsgIME->m_i_count);
		SetText(pMsgIME->m_InputStr);
		SetCrossIndex(pMsgIME->m_i_start + pMsgIME->m_i_count);
		ShowCross(true);
	}
}

void KEditView::InsertChar(int index,kn_string& str )
{
	if(m_bAllSelect)
	{
		RemoveAllChar();
		index = 0;
	}

	for(int i = 0; i < str.size(); i++)
	{
		REPaint* pPaint = m_text_drawable->GetPaint();
		if(str[i] == '\r')
		{
			continue;
		}
		if(!m_bMultiLine && str[i] == '\n')
		{
			continue;
		}
        // 文字长度截断判断错误修正 modify by weix 2015-12-1
		if(m_input_max_length <= m_Text.size())
		{
			 break;
		}

		if(str[i] == '\t')
		{
			EditWordAtom wordAtom;
			wordAtom.m_TextAtom = _T("   ");
			wordAtom.m_Length = pPaint->measureText(wordAtom.m_TextAtom.c_str(), wordAtom.m_TextAtom.length() * sizeof(kn_char));
			m_Text.insert(m_Text.begin() + index,wordAtom);
			index++;
		}
		else
		{
			EditWordAtom wordAtom;
			wordAtom.m_TextAtom = str[i];
			wordAtom.m_Length = pPaint->measureText(wordAtom.m_TextAtom.c_str(), wordAtom.m_TextAtom.length() * sizeof(kn_char));
			m_Text.insert(m_Text.begin() + index,wordAtom);
			index++;
		}
	}
    // 不是backspace操作 add by weix at 2015-12-01
    m_b_is_backspace_oper = false;
	SetCrossIndex(index);
	UpdateTextDrawable();
}

void KEditView::RemoveChar(int index)
{
	if(m_Index >= 0 && index < m_Text.size())
	{
        // 删除的文字长度 add by weix at 2015-12-01
        m_backspace_char_length = m_Text[index].m_Length;
        m_Text.erase(m_Text.begin() + index);
        SetCrossIndex(index);
        UpdateTextDrawable();
	}
}

void KEditView::RemoveAllChar()
{
	m_Text.clear();
	SetCrossIndex(0);
	UpdateTextDrawable();
	SetAllSelect(false);
}


void KEditView::UpdateTextDrawable()
{
	//改变textdrawable的值
	kn_string text = _T("");
	if(!m_bMultiLine)
	{
		for(int i = 0; i < m_Text.size(); i++)
		{
			text += m_Text[i].m_TextAtom;
		}
	}
	else
	{
		RERect TextBound = GetTextBound();
		int w = TextBound.width();
		int len = 0;
		for(int i = 0; i < m_Text.size(); i++)
		{
			if(m_Text[i].m_TextAtom == _T("\n"))
			{
				len = 0;
				text += m_Text[i].m_TextAtom;
				continue;
			}
			if(m_Text[i].m_TextAtom == _T("\r"))
			{
				continue;
			}

			if(len + m_Text[i].m_Length >= w)
			{
				len = 0;
				text += '\n';
			}
			text += m_Text[i].m_TextAtom;
			len += m_Text[i].m_Length;
		}
//		text += '\n';
	}
	KTextView::SetText(text);
	UpdateUI();
}

void KEditView::SetCrossIndex(int index)
{
	//设置游标的index
	if(index < 0)
	{
		index = 0;
	}
	else if(index > m_Text.size())
	{
		index = m_Text.size();
	}
	if (m_Index != index)
	{
		m_sign_change_input.emit();
	}
	m_Index = index;

	int fs = m_text_drawable->GetFontSize() * GetLinecount();
	int t = (m_rect.height() - fs)/2;
	int h =  m_text_drawable->GetFontSize();
	int w = m_Cross_drawable->GetRect().width();

	//计算游标的像素位置，循环加上每个字符的像素宽
	RERect TextBound = GetTextBound();
	int l = TextBound.left();

    kn_int text_length = 0;
	if(!m_bMultiLine)
	{
		for(int i = 0; i < m_Index; i++)
		{
			l += m_Text[i].m_Length;
            text_length += m_Text[i].m_Length;
		}

		//如果游标超过了最右边
		if(l >= TextBound.right())
		{
			int dx = l - TextBound.right();
			SetTextBound(RERect::MakeXYWH(TextBound.left() - dx, 0, TextBound.width() + dx, m_rect.height()));
			l = TextBound.right() - 1;
		}

		//如果游标超过了最左边
		if(l < m_bk_border_w)	
		{
			int dx = m_bk_border_w - l;
			SetTextBound(RERect::MakeXYWH(TextBound.left() + dx, 0, TextBound.width() - dx, m_rect.height()));
			l = m_bk_border_w;
		}
    
        // backspace时，屏幕外文字往里填充
        if (m_b_is_backspace_oper)
        {
            if (TextBound.left() < m_bk_border_w && l != m_bk_border_w)
            {
                if (text_length - l +  m_bk_border_w >= m_backspace_char_length)
                {
                    SetTextBound(RERect::MakeXYWH(TextBound.left() + m_backspace_char_length, 0, TextBound.width() - m_backspace_char_length, m_rect.height()));
                    l += m_backspace_char_length;
                }
                else
                {
                    int dx = text_length - l +  m_bk_border_w;
                    SetTextBound(RERect::MakeXYWH(TextBound.left() + dx, 0, TextBound.width() - dx, m_rect.height()));
                    l += dx;
                }
            }
        }
	}
	else
	{
		if(getAlignType() == ALIGN_TOP)
		{
			t = m_bk_border_h;
		}
		else if(getAlignType() == ALIGN_CENTER)
		{
			t = (m_rect.height() - fs)/2;
		}
		else  // ALIGN_BOTTOM
		{
			t = m_rect.height() - m_bk_border_h - fs;
		}
		int TextHeight = m_text_drawable->GetFontSize();
		for(int i = 0; i < m_Index; i++)
		{
			if(m_Text[i].m_TextAtom[0] == '\n') 
			{
				t += TextHeight;
				l = TextBound.left();
			}
			else
			{
				l += m_Text[i].m_Length;
			}

			if(l >= TextBound.right())
			{
				t += TextHeight;
				l = TextBound.left() + m_Text[i].m_Length;
			}
		}
	}

	//设置游标的位置
	m_Cross_drawable->SetRect(RERect::MakeXYWH(l, t - m_i_cross_offset + m_i_y_offset, w, h + m_i_cross_offset + m_i_y_offset));


	UpdateUI();
}

void KEditView::ShowCross(bool bshow)
{
	m_Cross_drawable->SetShow(bshow);
	if (m_focus_drawable)
	{
		if (bshow)
		{
			m_lst_drawable[0] = m_focus_drawable;
		}
		else
		{
			m_lst_drawable[0] = m_bk_drawable;
		}
	}
	UpdateUI();
}


void KEditView::SetAllSelect(bool bSel)
{
	m_bAllSelect = bSel;
	if(m_bAllSelect)
	{
		//计算所有字符的宽度，作为全选背景的宽
		RERect TextBound = GetTextBound();
		int len = 0;
		for(int i = 0; i < m_Text.size(); i++)
		{
			len += m_Text[i].m_Length;
		}

		int fs = m_text_drawable->GetFontSize() * GetLinecount();
		int t;
		if(!m_bMultiLine)
		{
			t = (m_rect.height() - fs)/2;
		}
		else
		{
			if(getAlignType() == ALIGN_TOP)
			{
				t = m_bk_border_h;
			}
			else if(getAlignType() == ALIGN_CENTER)
			{
				t = (m_rect.height() - fs)/2;
			}
			else  // ALIGN_BOTTOM
			{
				t = m_rect.height() - m_bk_border_h - fs;
			}
		}
		
		int h = fs;
		m_AllSelect_drawable->SetRect(RERect::MakeXYWH(m_bk_border_w + m_i_x_offset, t - m_i_cross_offset  + m_i_y_offset,
			len > m_rect.width() ? m_rect.width() - (m_bk_border_w+m_i_x_offset): len, h + m_i_cross_offset  + m_i_y_offset));
		m_AllSelect_drawable->SetShow(true);
		SetCrossIndex(m_Text.size());
		ShowCross(false);
		UpdateUI();
	}
	else
	{
		m_AllSelect_drawable->SetShow(false);
		ShowCross(false);
		UpdateUI();
	}
}

void KEditView::setState( ViewState state, kn_bool bRefresh )
{
	if (m_state != state)
	{
		m_state = state;
		switch(state)
		{
		case BS_FOCUS:
			//KStaticView::SetTextColor(m_text_focus_color);
			if(m_b_enable_cursor_change)
			{
				SetMouseCursorStyle(CURSOR_CROSS);
			}

			break;
		case BS_NORMAL:
			//KStaticView::SetTextColor(m_text_normal_color);
			if(m_b_enable_cursor_change)
			{
				SetMouseCursorStyle(CURSOR_ARROW);
			}

			break;
		case BS_PRESSED:
			//KStaticView::SetTextColor(m_text_pressed_color);
			break;
		case BS_DISABLED:
			//KStaticView::SetTextColor(m_text_disable_color);
			break;
		default:
			break;
		}
		if (bRefresh)
		{
			InvalidateView();
		}
	}
}

void  KEditView::onFocus()
{
	m_bFocus = true;
}

void  KEditView::onUnFocus()
{
	m_bFocus = false;
	SetAllSelect(false);
	ShowCross(false);
	m_sign_end_input.emit();
    showIME(false);
}

void KEditView::Copy()
{
	// 拷贝到剪贴板
	if(m_bAllSelect)
	{
		const kn_string& str = m_text_drawable->GetText();
		SystemClipboard::copyTextToClipboard(str);
	}
}



void KEditView::Paste()
{
	// 从剪贴板粘贴
	kn_string str = SystemClipboard::getTextFromClipboard();
	if(m_bAllSelect)
	{
		//删除所有字符
		RemoveAllChar();
	}

	//从游标所在的位置开始粘贴
	for(int i = 0; i < str.size(); i++)
	{
		kn_string ch;
		ch = str[i];
		InsertChar(m_Index,ch );
	}
	SetAllSelect(false);
}



void KEditView::Cut()
{
	// 剪切到剪贴板
	if(m_bAllSelect)
	{
		const kn_string& str = m_text_drawable->GetText();
		SystemClipboard::copyTextToClipboard(str);

		//删除所有字符
		RemoveAllChar();
	}

}


int KEditView::GetLinecount()
{
	if(!m_bMultiLine)
	{
		return 1;
	}
	else
	{
		const kn_string& text = m_text_drawable->GetText();
		int cnt = 1;
		int i = 0;
		for(; i < text.length(); i++)
		{
			if(text[i] == '\n')
			{
				cnt++;
			}
		}
		return cnt;
	}
}

void KEditView::setAlignType(int type)
{
	if(m_bMultiLine)
	{
		 KTextMultiLineDrawable_PTR ptext_drawable;
		 ptext_drawable = boost::dynamic_pointer_cast<KTextMultiLineDrawable>(m_text_drawable); 
		 ptext_drawable->setAlignType(type);		
	}
}

int KEditView::getAlignType()
{
	if(m_bMultiLine)
	{
		KTextMultiLineDrawable_PTR ptext_drawable;
		ptext_drawable = boost::dynamic_pointer_cast<KTextMultiLineDrawable>(m_text_drawable); 
		return ptext_drawable->getAlignType();		
	}
	return ALIGN_CENTER;
}
void KEditView::setTextOff(kn_int ix,kn_int iy)
{
	RERect rect_text = m_text_drawable->GetRect();
	rect_text.offset(ix,iy);
	m_text_drawable->SetRect(rect_text);
	RERect rect_cross = m_Cross_drawable->GetRect();
	rect_cross.offset(ix,iy);
	m_Cross_drawable->SetRect(rect_cross);
	RERect rect_all = m_AllSelect_drawable->GetRect();
	rect_all.offset(ix,iy);
	m_AllSelect_drawable->SetRect(rect_all);
	m_i_x_offset = ix;
	m_i_y_offset = iy;
}
void KEditView::setCrossOff(kn_int i)
{
	m_i_cross_offset = i;
}


void  KEditView::setFocusDrawable( KDrawable_PTR p )
{
	writeLock lock(m_lst_drawable_mutex);
	m_focus_drawable = p;
	m_focus_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
}

kn_int KEditView::GetCrossIndex()
{
	return m_Index;
}

kn_bool KEditView::GetAllSelect()
{
	return m_bAllSelect;
}

void KEditView::setInputMaxLength( kn_int max_length )
{
    m_input_max_length = max_length;
}

