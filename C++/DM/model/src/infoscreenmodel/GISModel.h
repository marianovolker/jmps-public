#if !defined(GISModel_h)
#define GISModel_h

#include <QMap>
#include <QList>
#include <QMutex>

#include "GDisplay.h"
#include "GScreen.h"
#include "GTextField.h"
#include "model/src/base/GAbsModel.h"

class GISModel : public GAbsModel
{

public:
	GISModel();
	virtual ~GISModel();
	
	void AddDisplay(GDisplay* oDisplay);	
	GDisplay* GetDisplay(int iNumber) const;
	QList<GDisplay *> GetAllDisplays() const;
	void RemoveDisplay(int iNumber);
	void RemoveAllDisplays();
	void ClearView(bool bSynchronize = false);
	void UpdateView(bool bSynchronize = false);
	void UpdateViewSection(GTextField * pTextField, bool bSynchronize = false);
	void UpdateViewSection(GSegment * pSegment, bool bSynchronize = false);

	QMutex *GetMutex();

public:
	static const QString CXMLTag;

private:
	QMap<int, GDisplay *> *m_oDisplays;
	QMutex m_mutex;
};

#endif
