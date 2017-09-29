#ifndef GGDIView_H_
#define GGDIView_H_

#include <QMutex>
#include <QSharedPointer>

#include "view/src/base/GAbsView.h"
#include "utils/base/src/GException.h"

class GGDILine;
class GGDIRect;
class GGDIText;

class GGDIView : public GAbsView
{
public:
	static QSharedPointer<GGDIView> GetInstance() throw(GException);
	virtual ~GGDIView();

protected:
	GGDIView();
	void Init();

public:
	void ClearView(bool bSynchronize = false);
	void DrawLine (GGDILine&, bool bSynchronize = false);
	void DrawText (GGDIText&, bool bSynchronize = false);
	void DrawRect (GGDIRect&, bool bSynchronize = false);
	void Update	  (bool bSynchronize = false);

private:
	static QSharedPointer<GGDIView> m_pGGDIView;
	static QMutex m_mutex;
	QStringList m_rStrListJSCalls;
};

#endif
