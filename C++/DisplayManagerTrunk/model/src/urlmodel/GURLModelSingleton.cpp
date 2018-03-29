#include "GURLModelSingleton.h"
#include "utils/logger/src/GLogger.h"

QSharedPointer<GURLModelSingleton> GURLModelSingleton::m_pGURLModelSingleton;
QMutex GURLModelSingleton::m_mutex;

GURLModelSingleton::GURLModelSingleton()
{
	LOG_LEVEL3( "GURLModelSingleton()" );

	m_pModel = QSharedPointer<GURLModel>(new GURLModel());
}

GURLModelSingleton::~GURLModelSingleton()
{
}

QSharedPointer<GURLModelSingleton> GURLModelSingleton::GetInstance() throw(GException)
{
	LOG_LEVEL3( "GetInstance()" );

    QMutexLocker oLocker(&GURLModelSingleton::m_mutex);

	if( m_pGURLModelSingleton.isNull() )
	{
		m_pGURLModelSingleton = QSharedPointer<GURLModelSingleton>(new GURLModelSingleton());
	}

	if( m_pGURLModelSingleton.isNull() )
	{
		 throw( GException("GURLModelSingleton::GetInstance() made a NULL pointer.") );
	}

	return m_pGURLModelSingleton;
}

QSharedPointer<GURLModel> GURLModelSingleton::GetModel()
{
	LOG_LEVEL4( "GetModel()" );

	return m_pModel;
}

void GURLModelSingleton::SetModel( QSharedPointer<GURLModel> pModel )
{
	LOG_LEVEL4( "SetModel()" );

	m_pModel = pModel;
}
