//  **************************************
//  File:        KTextMultiLineDrawable.cpp
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
#include "KTextMultiLineDrawable.h"


KTextMultiLineDrawable::KTextMultiLineDrawable( REColor color,  kn_int iFontSize, REPaint::Align align):KTextDrawable(NULL, color,  iFontSize, align)
{
	m_type = KDrawable::KDRAWABLE_TEXTMULTILINE;
	m_aligntype = ALIGN_TOP;
	//m_txt_line = 0;
	m_txt_row_interval = 0;
	m_multi_line_w = 0;
	m_max_line = -1;
	m_b_overroad = false;
}

void KTextMultiLineDrawable::setRowInterval(int v)
{
	m_txt_row_interval = v;
}

void KTextMultiLineDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	readLock lock(m_drawable_draw_mutex); 
	if (! BeginDraw(pDstSurface, pMatrix, iDstX, iDstY ) )
	{
		return;
	}

	kn_int txt_line = m_v_draw_string.size();


	int iTarX = m_rect.left() + iDstX;
	int iTarY = m_rect.top() + iDstY;


	REPaint::Align align = m_paint.getTextAlign();
	if(align == REPaint::kCenter_Align )
	{
		iTarX += m_rect.width() / 2;
	}
	else if(align == REPaint::kRight_Align)
	{
		iTarX += m_rect.width();
	}

	// 设置文字垂直居中
	// 获取文字高度, 文字从baseline 开始画起,不是从最底部画起
	REPaint::FontMetrics fm; 
	m_paint.getFontMetrics(&fm);

	//文字总的高度
//	int iTextHeight = ceil (fm.fDescent - fm.fAscent);
	int iTextHeight = m_paint.getTextSize();

	// 文字在baseline以上的高度
	int iBaseline = abs((int)(fm.fAscent+0.5));

	if(m_aligntype == ALIGN_TOP)
	{
		iTarY = iBaseline + iTarY;
	}
	else if(m_aligntype == ALIGN_CENTER)
	{
		iTarY =  (m_rect.height() - iTextHeight * txt_line) / 2 + iBaseline + iTarY;
	}
	else // ALIGN_BOTTOM
	{
		iTarY =  (m_rect.height() - iTextHeight * txt_line) + iBaseline + iTarY;
	}

	try
	{
		//pDstSurface->DrawText(m_strText.c_str(), m_strText.length() * sizeof(kn_char), iTarX, iTarY, m_paint);

		if ( isTextFrame())
		{//小外框
			REColor clr = m_paint.getColor();
			//暂时外框色取补色
			REColor frameclr = ColorSetARGB(ColorGetA(clr), 255 - ColorGetR(clr), 255 - ColorGetG(clr),255 - ColorGetB(clr) );
			m_paint.setColor(frameclr);
			for(int i = 0; i < txt_line; i++)
			{
				pDstSurface->DrawText(m_v_draw_string[i].c_str(), m_v_draw_string[i].length() * sizeof(kn_char), iTarX-1, iTarY+ (iTextHeight+m_txt_row_interval)*i, m_paint);
				pDstSurface->DrawText(m_v_draw_string[i].c_str(), m_v_draw_string[i].length() * sizeof(kn_char), iTarX+1, iTarY+ (iTextHeight+m_txt_row_interval)*i, m_paint);
				pDstSurface->DrawText(m_v_draw_string[i].c_str(), m_v_draw_string[i].length() * sizeof(kn_char), iTarX,   iTarY+ (iTextHeight+m_txt_row_interval)*i-1, m_paint);
				pDstSurface->DrawText(m_v_draw_string[i].c_str(), m_v_draw_string[i].length() * sizeof(kn_char), iTarX,	  iTarY+ (iTextHeight+m_txt_row_interval)*i+1, m_paint);
			}
			m_paint.setColor(clr);
		}

		for(int i = 0; i < txt_line; i++)
		{
			kn_string temp = m_v_draw_string[i];
			if (m_max_line != -1 && txt_line > m_max_line)
			{
				if (m_max_line - 1 == i)
				{
					m_b_overroad = false; 
					//最后一行用"..."
					const kn_string str_omit = _T("...");
					kn_int i_omit_w = m_paint.measureText(str_omit.c_str() ,str_omit.size() * sizeof(kn_char));
					while(1)
					{
						kn_int i_text_w = m_paint.measureText(temp.c_str() ,temp.size() * sizeof(kn_char));
						if (i_text_w + i_omit_w < m_rect.width())
						{
							temp+=str_omit; 
							m_b_overroad = true;
							break;
						}
						temp.erase(temp.end() - 1);
					}
				}
				if (i >= m_max_line)
				{
					continue;
				}
			}
			else
			{
				m_b_overroad = false;
			}
			pDstSurface->DrawText(temp.c_str(), temp.length() * sizeof(kn_char), iTarX, iTarY + (iTextHeight+m_txt_row_interval)*i, m_paint);
//			iTarY += iTextHeight;
		}
	}
	catch (...)
	{
		iTarX = iTarX;
	}

	EndDraw(pDstSurface, pMatrix);
}

void KTextMultiLineDrawable::SetText(const kn_char* szText)
{
	writeLock lock(m_drawable_draw_mutex); 
	m_strText = szText;
	autoMLine();
	//m_txt_line = GetTextLine(m_v_draw_string);
}

void KTextMultiLineDrawable::SetText(const kn_string& szText)
{
	SetText(szText.c_str() );
}

int KTextMultiLineDrawable::GetTextLine(std::vector<kn_string>& vStr)
{
	int cnt = 0;
	int i = 0;
	kn_string str = _T("");
	for(; i < m_strText.length(); i++)
	{
		if(m_strText[i] == '\n')
		{
			cnt++;
			vStr.push_back(str);
			str =_T("");
		}
		else
		{
			str += m_strText[i];
		}
	}
	return cnt;
}


void KTextMultiLineDrawable::autoMLine()
{
	int tw = m_paint.getTextSize();
	int w =  m_rect.width();
	int tsize = m_strText.size();

    writeLock lock(m_draw_string_mutex);
	m_v_draw_string.clear();
	kn_string strText = m_strText;
	//去换行
	kn_string strtmp, strtmp_last;
	int i ,j, len;
	int pos = 0;
	for (i = 0; i< tsize; i++)
	{//计算一行的字符分割
		if (m_strText[i] == '\n')
		{
			m_v_draw_string.push_back( strtmp );
			pos =i+1;
			strtmp.clear();
			continue;
		}

		strtmp = strText.substr(pos, i-pos+1);
		len = m_paint.measureText(strtmp.c_str() ,strtmp.size() * sizeof(kn_char));
		if ( len > w )
		{
			if (m_multi_line_w < len)
			{
				m_multi_line_w = len;
			}
			m_v_draw_string.push_back( strtmp_last );
			strtmp.clear();
			pos =i;

		}
		strtmp_last = strtmp;
	}

	if (pos < tsize)
	{
		strtmp = strText.substr(pos, tsize-pos);
		m_v_draw_string.push_back( strtmp );
	}

	//m_strText +=  _T("\n");
	//m_txt_line = m_v_draw_string.size();

	m_b_overroad = m_max_line < m_v_draw_string.size();
}


///这个不支持换行
//void KTextMultiLineDrawable::autoMLine()
//{
//	int tw = m_paint.getTextSize();
//	int w =  m_rect.width();
//	int tsize = m_strText.size();
//
//	m_v_draw_string.clear();
//	kn_string strText = m_strText;
//	//去换行
//	for (kn_string::iterator s = strText.begin(); s != strText.end(); )
//	{//计算一行的字符分割
//		if ( *s == '\n')
//		{
//			s = strText.erase(s);
//			continue;
//		}
//		else
//		{
//			s++;
//		}
//	}
//
//	if(getTextWidth() > w)
//	{
//		int count = w /  tw;
//		if (count < tsize)
//		{
//			//需要显示的行数
//			int n_line = getTextWidth() / w + 1;
//
//			kn_string strtmp;
//			int i ,j;
//			int pos = 0;
//			strtmp = strText.substr(0, count);
//			for (i = 0; i < n_line;  i++)
//			{
//				for (j = count; pos + j<= tsize; j++)
//				{//计算一行的字符分割
//					strtmp = strText.substr(pos, pos+j);
//					 if ( m_paint.measureText(strtmp.c_str() ,strtmp.size() * sizeof(kn_char)) > w )
//					 {
//						 break;
//					 }					 
//				}
//
//				if (pos +j > tsize  )
//				{ // 已经到字符结尾
//					//str += strtmp;
//					m_v_draw_string.push_back( strtmp );
//					break;
//				}
//			//	str += strtmp.substr(0, j-1) + _T("\n");
//				m_v_draw_string.push_back( strtmp.substr(0, j-1) );
//				pos += j-1;
//
//				if (pos + count >= tsize )
//				{//剩余字符不足count个
//					m_v_draw_string.push_back( strText.substr(pos, tsize-pos) );
//					break;
//				}
//				
//			}
////			m_strText = str;
//		}
//
//	}
//	else
//	{
//		m_v_draw_string.push_back( strText );
//	}
//	//m_strText +=  _T("\n");
//	m_txt_line = m_v_draw_string.size();
//}

int  KTextMultiLineDrawable::getLine()
{
	return m_v_draw_string.size();
}

int KTextMultiLineDrawable::getMaxWidth()
{
	kn_int w = 0;
	for (int i = 0; i < m_v_draw_string.size() ; i++)
	{
		kn_int row_w = m_paint.measureText(m_v_draw_string[i].c_str() ,m_v_draw_string[i].size() * sizeof(kn_char));
		w = (w < row_w) ? row_w : w;
	}
	return w;
}

void KTextMultiLineDrawable::setMaxLine( int i)
{
	m_max_line = i;
	m_b_overroad = m_max_line < m_v_draw_string.size();
}

bool KTextMultiLineDrawable::isOverroad()
{
	return m_b_overroad;
}
