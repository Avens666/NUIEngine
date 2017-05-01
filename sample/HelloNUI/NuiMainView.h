#ifndef K_KMainView_VIEW_H___
#define K_KMainView_VIEW_H___
#include "KViewGroup.h"
#include "KTextDrawable.h"
#include "KImageDrawable.h"
#include "KShapeDrawable.h"



class KMainView : public KViewGroup
{
public:
	KMainView(void);
	virtual ~KMainView(void);
	void init();
protected:
	CAnimationThread m_ani_thread;
};
typedef boost::shared_ptr<KMainView> KMainView_PTR;


#endif

