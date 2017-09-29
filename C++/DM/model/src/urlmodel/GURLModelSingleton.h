//! GURLModelSingleton
/*!
  Implementation of the Class GURLModelSingleton
*/

#if !defined(GURLModelSingleton_h)
#define GURLModelSingleton_h

#include "utils/base/src/GException.h"
#include "GURLModel.h"

#include <QMutex>
#include <QSharedPointer>

class GURLModelSingleton
{
public:
	virtual ~GURLModelSingleton();
	static QSharedPointer<GURLModelSingleton> GetInstance() throw (GException);
	
	QSharedPointer<GURLModel> GetModel();
	void SetModel( QSharedPointer<GURLModel>  pModel);
	
protected:
	GURLModelSingleton();

private:
	static QSharedPointer<GURLModelSingleton> m_pGURLModelSingleton;
	static QMutex m_mutex;
	
	QSharedPointer<GURLModel> m_pModel;
};
#endif
