#include "GISModelSingleton.h"

#include "GISModelImporter.h"
#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"

#include <QString>
#include <QFile>

QSharedPointer<GISModelSingleton> GISModelSingleton::m_pGISModelSingleton;
QMutex GISModelSingleton::m_mutex;

GISModelSingleton::GISModelSingleton()
{
	LOG_LEVEL3( "GISModelSingleton()" );

	QFile oFile;

	QString strXmlFile = GETCONF_STR(eDisplayManagerModelFileName);

	if( oFile.exists(strXmlFile) )
	{
		LOG_LEVEL3( QString("Inporting Model from XML:[") + strXmlFile  + QString("]") );

		m_pModel = QSharedPointer<GISModel>(GISModelImporter::Import(strXmlFile));

		if( m_pModel.isNull() )
		{
			LOG_LEVEL1( QString("Error inporting Model from XML:[") + strXmlFile  + QString("]") );
		}
	}

	if( m_pModel.isNull() )
	{
		m_pModel = QSharedPointer<GISModel>(new GISModel());
	}

	GDisplay *poDisplay = m_pModel->GetDisplay(1);
	if ( poDisplay == NULL )
	{
		m_pModel->AddDisplay(new GDisplay(1));
	}
}

GISModelSingleton::~GISModelSingleton()
{
}

QSharedPointer<GISModelSingleton> GISModelSingleton::GetInstance() throw(GException)
{
    LOG_LEVEL4( "GetInstance()" );

    QMutexLocker oLocker(&GISModelSingleton::m_mutex);

	if( m_pGISModelSingleton.isNull() )
	{
		m_pGISModelSingleton = QSharedPointer<GISModelSingleton>(new GISModelSingleton());
	}

	if( m_pGISModelSingleton.isNull() )
	{
		 throw( GException("GISModelSingleton::GetInstance() made a NULL pointer.") );
	}

	return m_pGISModelSingleton;
}

QSharedPointer<GISModel> GISModelSingleton::GetModel()
{
	LOG_LEVEL4( "GetModel()" );

	return m_pModel;
}

void GISModelSingleton::SetModel(QSharedPointer<GISModel> pModel)
{
	LOG_LEVEL4( "SetModel()" );

	m_pModel = pModel;
}
