#include "model/src/urlmodel/GURLModel.h"

#include "utils/logger/src/GLogger.h"
#include "view/src/urlview/GURLView.h"

GURLModel::GURLModel()
{
	LOG_LEVEL4("GURLModel()");
}

GURLModel::~GURLModel()
{
	LOG_LEVEL4("~GURLModel()");
}

void GURLModel::UpdateView(bool bSynchronize)
{
	LOG_LEVEL4(QString("UpdateView() [")+((bSynchronize)?("true"):("false"))+"]");
}

void GURLModel::SetUrl( QString const & sUrl )
{
	m_sUrl = sUrl;
}

QString const & GURLModel::GetUrl() const
{
	return m_sUrl;
}

QMutex *GURLModel::GetMutex()
{
    return &m_mutex;
}
