#include "GISSaveApplicationMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GISModelExporter.h"
#include "model/src/infoscreenmodel/GISModel.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/logger/src/GLogger.h"
#include "utils/image/src/GImageMappings.h"
#include "utils/image/src/GImageMappingsExporter.h"
#include "utils/image/src/GImageHelper.h"

#include <QString>
#include <QMutexLocker>
#include <QSharedPointer>

GISSaveApplicationMsgCommand::GISSaveApplicationMsgCommand()
{
	LOG_LEVEL4("GISSaveApplicationMsgCommand()");
}

GISSaveApplicationMsgCommand::~GISSaveApplicationMsgCommand()
{
	LOG_LEVEL4("~GISSaveApplicationMsgCommand()");
}

void GISSaveApplicationMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();
	{
		QMutexLocker mtxScope(pModel->GetMutex());

		QString strXmlFile = GETCONF_STR(eDisplayManagerModelFileName);

		LOG_LEVEL4( QString("File xml: [") + strXmlFile  + QString("]") );

        if ( !GISModelExporter::Export(pModel.data(), strXmlFile ) )
			 throw GIS_EXCEPTION(GISCommandStatusEnum::eFlashRAMWriteReadError);
	}

    QSharedPointer<GImageMappings> poImageMappings(GImageMappings::GetInstance());
	{
		QMutexLocker mtxImageMappingsScope(poImageMappings->GetMutex());

        if ( !GImageMappingsExporter::Export(poImageMappings, GImageHelper::ImagesMappingsFile() ))
			throw GIS_EXCEPTION(GISCommandStatusEnum::eFlashRAMWriteReadError);
	}

	throw GIS_EXCEPTION(GISCommandStatusEnum::eApplicationSaveComplete);
}

