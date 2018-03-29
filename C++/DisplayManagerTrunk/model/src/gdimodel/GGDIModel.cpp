#include "GGDIModel.h"

#include "utils/logger/src/GLogger.h"
#include "view/src/gdiview/GGDIView.h"
#include "GGDILine.h"
#include "GGDIRect.h"
#include "GGDIText.h"

#include <QMutexLocker>

GGDIModel::GGDIModel()
{
	LOG_LEVEL4("GGDIModel()");
}

GGDIModel::~GGDIModel()
{
	LOG_LEVEL4("~GGDIModel()");
}

void GGDIModel::ClearView(bool bSynchronize)
{
	LOG_LEVEL4("ClearView()");

	QSharedPointer<GGDIView> poGDIView = GGDIView::GetInstance();
	poGDIView->ClearView(bSynchronize);
}

void GGDIModel::DrawLine (GGDILine& oLine, bool bSynchronize)
{
	LOG_LEVEL4("DrawLine()");

	QSharedPointer<GGDIView> poGDIView = GGDIView::GetInstance();
	poGDIView->DrawLine(oLine, bSynchronize);
}

void GGDIModel::DrawText (GGDIText& oText, bool bSynchronize)
{
	LOG_LEVEL4("DrawText()");

	QSharedPointer<GGDIView> poGDIView = GGDIView::GetInstance();
	poGDIView->DrawText(oText, bSynchronize);
}

void GGDIModel::DrawRect (GGDIRect& oRect, bool bSynchronize)
{
	LOG_LEVEL4("DrawRect()");

	QSharedPointer<GGDIView> poGDIView = GGDIView::GetInstance();
	poGDIView->DrawRect(oRect, bSynchronize);
}

void GGDIModel::UpdateView(bool bSynchronize)
{
	LOG_LEVEL4("UpdateView()");

	QSharedPointer<GGDIView> poGDIView = GGDIView::GetInstance();
	poGDIView->Update(bSynchronize);
}

QMutex *GGDIModel::GetMutex()
{
    return &m_mutex;
}
