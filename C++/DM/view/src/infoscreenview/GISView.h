#ifndef GISView_H_
#define GISView_H_

#include <QMutex>
#include <QString>
#include <QSharedPointer>

#include "view/src/base/GAbsView.h"
#include "view/src/utils/GPersonalityView.h"
#include "utils/base/src/GException.h"

#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GTextField.h"

class GISView :	public GAbsView
{
public:
	static QSharedPointer<GISView> GetInstance() throw(GException);
	virtual ~GISView();

protected:
	GISView();
	void Init();

public:
	void Clear(bool bSynchronize = false);
	void Update(bool bSynchronize = false);
	void UpdateSection(GSegment * pSegment, bool bSynchronize = false);
	void UpdateSection(GTextField * pTextField, bool bSynchronize = false);

private:
	void UpdateDom(QStringList& rStrListJSCalls, QString &strPersonalityURL);
	void UpdateScreen(GScreen* pScreen, QStringList& rStrListJSCalls, QString &strPersonalityURL);
	void UpdateSegment(GSegment* pSegment, QString const & sParentName, QStringList& rStrListJSCalls);
	void UpdateTextField(GTextField* pField, QString const & sParentName, QStringList& rStrListJSCalls, bool bFreeText);

private:
	bool ObtainImagePath(QString & strImagePath, int iSegmentNumber) const;

private:
  static QSharedPointer<GISView> m_pGISView;
  static QMutex m_mutex;

};

#endif
