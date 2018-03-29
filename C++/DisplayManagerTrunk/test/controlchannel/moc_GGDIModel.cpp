#include "model/src/gdimodel/GGDIModel.h"

#include "utils/logger/src/GLogger.h"
#include "model/src/gdimodel/GGDILine.h"
#include "model/src/gdimodel/GGDIRect.h"
#include "model/src/gdimodel/GGDIText.h"

GGDIModel::GGDIModel()
{
	LOG_LEVEL4("GGDIModel()");
}

GGDIModel::~GGDIModel()
{
	LOG_LEVEL4("~GGDIModel()");
}

void GGDIModel::UpdateView(bool bSynchronize)
{
	LOG_LEVEL4(QString("UpdateView() [")+((bSynchronize)?("true"):("false"))+"]");
}

void GGDIModel::ClearView(bool bSynchronize)
{
	LOG_LEVEL4(QString("ClearView() [")+((bSynchronize)?("true"):("false"))+"]");
}

void GGDIModel::DrawLine (GGDILine& oLine, bool bSynchronize)
{
	LOG_LEVEL4(QString("DrawLine() [")+((bSynchronize)?("true"):("false"))+"], ["+oLine.GetColor()+"]");
}

void GGDIModel::DrawText (GGDIText& oText, bool bSynchronize)
{
	LOG_LEVEL4(QString("DrawText() [")+((bSynchronize)?("true"):("false"))+"], ["+oText.GetFGColor()+"]");
}

void GGDIModel::DrawRect (GGDIRect& oRect, bool bSynchronize)
{
	LOG_LEVEL4(QString("DrawRect() [")+((bSynchronize)?("true"):("false"))+"], ["+oRect.GetColor()+"]");
}

QMutex *GGDIModel::GetMutex()
{
    return &m_mutex;
}
