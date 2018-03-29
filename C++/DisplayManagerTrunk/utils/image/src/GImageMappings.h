#ifndef GImageMappings_H_
#define GImageMappings_H_

#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <memory>
#include <QSharedPointer>

#include "GImageMap.h"

class GImageMappings
{

public:

	GImageMappings();
	virtual ~GImageMappings();

	

	static QSharedPointer<GImageMappings> GetInstance();

	void AddImageMap(std::auto_ptr<GImageMap> oImage);
	GImageMap* GetImageMap(int iSegment) const;
	QList<GImageMap *> GetAllImageMaps() const;

	void RemoveAllMappings();

	QMutex *GetMutex();

private:
	void PurgeImageMaps();
	
public:
	static const QString CXMLTag;

private:
	QMap<int, GImageMap *> m_oImages;
	static QMutex m_oMux;
	static QSharedPointer<GImageMappings> st_poGImageMappings;
	QMutex m_mutex;
};

#endif
