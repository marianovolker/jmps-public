#if !defined(GGDIModel_h)
#define GGDIModel_h

#include <QMutex>

#include "model/src/base/GAbsModel.h"

class GGDILine;
class GGDIRect;
class GGDIText;

class GGDIModel : public GAbsModel
{

public:
	GGDIModel();
	virtual ~GGDIModel();

	void ClearView (bool bSynchronize = false);
	void DrawLine  (GGDILine&, bool bSynchronize = false);
	void DrawText  (GGDIText&, bool bSynchronize = false);
	void DrawRect  (GGDIRect&, bool bSynchronize = false);
	void UpdateView(bool bSynchronize = false);

	QMutex *GetMutex();

private:
	QMutex m_mutex;
};

#endif
