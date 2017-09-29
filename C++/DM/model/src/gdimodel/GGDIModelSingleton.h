//! GGDIModelSingleton
/*!
  Implementation of the Class GGDIModelSingleton
*/

#if !defined(GGDIModelSingleton_h)
#define GGDIModelSingleton_h

#include "utils/base/src/GException.h"
#include "GGDIModel.h"

#include <QMutex>
#include <QSharedPointer>

class GGDIModelSingleton
{
public:
	virtual ~GGDIModelSingleton();
	static QSharedPointer<GGDIModelSingleton> GetInstance() throw (GException);
	
	QSharedPointer<GGDIModel> GetModel();
	void SetModel(QSharedPointer<GGDIModel> pModel);
	
protected:
	GGDIModelSingleton();

private:
	static QSharedPointer<GGDIModelSingleton> m_pGGDIModelSingleton;
	static QMutex m_mutex;
	
	QSharedPointer<GGDIModel> m_pModel;
};
#endif
