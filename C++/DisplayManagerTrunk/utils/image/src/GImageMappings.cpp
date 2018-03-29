#include "GImageMappings.h"

#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"

const QString GImageMappings::CXMLTag								("ImageMappings");
QSharedPointer<GImageMappings> GImageMappings::st_poGImageMappings	(NULL);
QMutex GImageMappings::m_oMux;


GImageMappings::GImageMappings()
{
	LOG_LEVEL4("GImageMappings()");

	m_oImages.clear();
}


GImageMappings::~GImageMappings()
{
	LOG_LEVEL4("~GImageMappings()");

	PurgeImageMaps();
}

void GImageMappings::RemoveAllMappings()
{
	PurgeImageMaps();
	m_oImages.clear();
}


void GImageMappings::PurgeImageMaps()
{
	QMap<int, GImageMap *>::iterator itImages;

	for(itImages = m_oImages.begin(); itImages != m_oImages.end(); ++itImages)
	{
		delete *itImages;
		*itImages = NULL;
	}
}

QSharedPointer<GImageMappings> GImageMappings::GetInstance()
{
	QMutexLocker oLock(&m_oMux);
	LOG_LEVEL4("GetInstance()");

	if( st_poGImageMappings == NULL )
	{
		st_poGImageMappings = QSharedPointer<GImageMappings> (new GImageMappings());

		if( st_poGImageMappings == NULL )
		{
			 throw( GException("GImageMappings::GetInstance() made a NULL pointer.") );
		}
	}

	return st_poGImageMappings;
}


void GImageMappings::AddImageMap(std::auto_ptr<GImageMap> oImage)
{
	LOG_LEVEL4("AddImageMap()");
	QMutexLocker oLock(&m_oMux);

	int iSegment = oImage->GetSegment();

	if (m_oImages.contains(iSegment))
	{
		LOG_LEVEL3("Image for that Segment already exists. Erasing previous one.");

		std::auto_ptr<GImageMap> poImageMapToDelete(m_oImages[iSegment]);

		poImageMapToDelete.reset(0);

		m_oImages.remove(iSegment);
	}

	m_oImages.insert(iSegment, oImage.release());
}


GImageMap* GImageMappings::GetImageMap(int iSegment) const
{
	LOG_LEVEL4("GetImageMap()");

	QMutexLocker oLock(&m_oMux);
	QMap<int, GImageMap *>::const_iterator i = m_oImages.find(iSegment);
	return (i != m_oImages.end())? i.value() : NULL;
}


QList<GImageMap *> GImageMappings::GetAllImageMaps() const
{
	QMutexLocker oLock(&m_oMux);
	LOG_LEVEL4("GetAllImageMaps()");

	return m_oImages.values();
}


QMutex *GImageMappings::GetMutex()
{
    return &m_mutex;
}
