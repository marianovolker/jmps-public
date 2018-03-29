#include "GGDIModelSingleton.h"

#include <QMutexLocker>

#include "utils/logger/src/GLogger.h"

QSharedPointer<GGDIModelSingleton> GGDIModelSingleton::m_pGGDIModelSingleton;
QMutex GGDIModelSingleton::m_mutex;

GGDIModelSingleton::GGDIModelSingleton()
{
	LOG_LEVEL3( "GGDIModelSingleton()" );

	m_pModel = QSharedPointer<GGDIModel>(new GGDIModel());
}

GGDIModelSingleton::~GGDIModelSingleton()
{
}

QSharedPointer<GGDIModelSingleton> GGDIModelSingleton::GetInstance() throw(GException)
{
	LOG_LEVEL4( "GetInstance()" );

	QMutexLocker oLocker(&GGDIModelSingleton::m_mutex);

	if( m_pGGDIModelSingleton.isNull() )
	{
		m_pGGDIModelSingleton = QSharedPointer<GGDIModelSingleton>(new GGDIModelSingleton());
	}

	if( m_pGGDIModelSingleton.isNull() )
	{
		 throw( GException("GGDIModelSingleton::GetInstance() made a NULL pointer.") );
	}

	return m_pGGDIModelSingleton;
}

QSharedPointer<GGDIModel> GGDIModelSingleton::GetModel()
{
	LOG_LEVEL4( "GetModel()" );

	return m_pModel;
}

void GGDIModelSingleton::SetModel(QSharedPointer<GGDIModel> pModel)
{
	LOG_LEVEL4( "SetModel()" );

	m_pModel = QSharedPointer<GGDIModel>(pModel);
}
