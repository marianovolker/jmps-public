#include "GURLModel.h"

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

void GURLModel::SetUrl( QString const & sUrl )
{
	m_sUrl = sUrl;
}

QString const & GURLModel::GetUrl() const
{
	return m_sUrl;
}

void GURLModel::UpdateView(bool bSynchronize)
{
	LOG_LEVEL4("UpdateView()");

	QSharedPointer<GURLView> poURLView = GURLView::GetInstance();
	poURLView->Update(bSynchronize);
}

QMutex *GURLModel::GetMutex()
{
    return &m_mutex;
}
