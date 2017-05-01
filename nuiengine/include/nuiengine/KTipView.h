#ifndef KTipView_DEFINED
#define KTipView_DEFINED

#include "KViewGroup.h"
#include "KTextDrawable.h"
#include "KImageDrawable.h"
#include "KColorDrawable.h"
#include "KTextMultiLineDrawable.h"
#include "sigslot.h"

#define NO_HIDE_MENU 400

class KTipBase : public KViewGroup
{
public:
	KTipBase();
	virtual ~KTipBase();

	virtual void init(KScreen*, RERect rect, kn_string str=_T("") ); //传入的是tip针对的view的范围，screen全局坐标
	kn_bool isAutoHide();
	void setAutoHide(kn_bool b);
//	sigslot::signal1<int> m_sign_menu_click;
	void  setBKDrawable( KDrawable_PTR p );
	void  setTipText(kn_string );
	//显示后自动隐藏的时间
	void setShowTime(int);
	int	 getTipID();
protected:
	//触发menu的view
	KTextMultiLineDrawable_PTR	m_txt_drawable;
	KDrawable_PTR	m_bk_drawable;

	//自动隐藏，只要菜单收到点击消息就隐藏
	int	m_show_time;
	int m_max_w; //最大宽度，超过这个宽度换行
	kn_bool m_b_auto_hide;

	static int m_s_id;
	int m_tip_id;
};
//Menu 类使用智能指针管理
typedef boost::shared_ptr<KTipBase> KTipBase_PTR;

#endif

