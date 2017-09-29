//! GISModelSingleton 
/*!
  Implementation of the Class GISModelSingleton
*/

#if !defined(GISModelSingleton_h)
#define GISModelSingleton_h

#include "utils/base/src/GException.h"
#include "GISModel.h"

#include <QMutex>
#include <QSharedPointer>

class GISModelSingleton
{
public:
	virtual ~GISModelSingleton();
	static QSharedPointer<GISModelSingleton> GetInstance() throw (GException);
	
	QSharedPointer<GISModel> GetModel();
	void SetModel(QSharedPointer<GISModel>pModel);
	
protected:
	GISModelSingleton();

private:
	static QSharedPointer<GISModelSingleton> m_pGISModelSingleton;
	static QMutex m_mutex;
	
	QSharedPointer<GISModel> m_pModel;
};
#endif
