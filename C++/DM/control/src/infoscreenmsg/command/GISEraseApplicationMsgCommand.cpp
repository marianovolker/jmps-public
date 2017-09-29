#include "GISEraseApplicationMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GISModel.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/logger/src/GLogger.h"
#include "utils/image/src/GImageHelper.h"
#include "utils/image/src/GImageMappings.h"

#include <QMutexLocker>
#include <QString>
#include <QFile>
#include <QDir>


GISEraseApplicationMsgCommand::GISEraseApplicationMsgCommand()
{
	LOG_LEVEL4("GISEraseApplicationMsgCommand()");
}

GISEraseApplicationMsgCommand::~GISEraseApplicationMsgCommand()
{
	LOG_LEVEL4("~GISEraseApplicationMsgCommand()");
}

void GISEraseApplicationMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	{
		QFile oFile;

		QString strXmlFile = GETCONF_STR(eDisplayManagerModelFileName);

		LOG_LEVEL4( QString("File xml: [") + strXmlFile  + QString("]") );

		if( oFile.exists(strXmlFile) )
		{
			oFile.remove(strXmlFile);
		}
	}

	GISAbsData* pGISAbsData = GetData();

	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());

        pModel->RemoveDisplay(pGISAbsData->GetDisplay());

        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());
        if ( poDisplay == NULL )
        {
            pModel->AddDisplay(new GDisplay(pGISAbsData->GetDisplay()));
        }

        DeleteImageFiles();
    }
}

void GISEraseApplicationMsgCommand::DeleteImageFiles() const
{
	LOG_LEVEL4("DeleteImageFiles()");

	LOG_LEVEL3("Deleting Raw Images...");
	DeleteFilesInDir( GImageHelper::RawImagesPath() );

	LOG_LEVEL3("Deleting Scales Monochrome Images...");
	DeleteFilesInDir( GImageHelper::CurrentResolutionMonochromeImagesPath() );

	LOG_LEVEL3("Deleting Image Mappings File...");

	{
	    QString strFileToDelete( GImageHelper::ImagesMappingsFile() );

	    if( QFile::remove(strFileToDelete) )
	    {
	    	LOG_LEVEL3("File: " + strFileToDelete + " deleted.");
	    }
	    else
	    {
	    	LOG_LEVEL3("File: " + strFileToDelete + " coudn't be deleted.");
	    }
	}

    QSharedPointer<GImageMappings> poImageMappings(GImageMappings::GetInstance());

    {
        QMutexLocker mtxImageMappingsScope(poImageMappings->GetMutex());


        if (poImageMappings)
        {
              poImageMappings->RemoveAllMappings();
        }
    }
}

void GISEraseApplicationMsgCommand::DeleteFilesInDir( QString const &strImagesPath ) const
{
	LOG_LEVEL4("DeleteFilesInDir()");

	QString strFileToDelete;

	QDir oFilesDir( strImagesPath );

	QStringList oFiles(oFilesDir.entryList());
	
	QStringList::const_iterator itFiles;

	for(itFiles = oFiles.constBegin() ; itFiles < oFiles.constEnd() ; ++itFiles)
	{
	    strFileToDelete = strImagesPath + *itFiles;

	    if ( QFile::remove(strFileToDelete) )
	    {
	    	LOG_LEVEL3("File: " + strFileToDelete + " deleted.");
	    }
	    else
	    {
	    	LOG_LEVEL3("File: " + strFileToDelete + " coudn't be deleted.");
	    }
	}
}
