#include"GImageConverter.h"

#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"

#include <QImage>
#include <QColor>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <memory>


QSharedPointer<GImageConverter>  GImageConverter::st_poGImageConverter	(NULL);
QMutex GImageConverter::st_oMutex;
const QByteArray GImageConverter::st_CPNGImageFormat	("png");
const QByteArray GImageConverter::st_CBMPImageFormat	("bmp");


GImageConverter::GImageConverter():
	m_fScaleXFactor(0),
	m_fScaleYFactor(0),
	m_iMaxRetries(3)
{
	LOG_LEVEL4("GImageConverter()");

	moveToThread(this);
}

GImageConverter::~GImageConverter()
{
	LOG_LEVEL4("~GImageConverter()");
}

QSharedPointer<GImageConverter> GImageConverter::GetInstance()
{
	LOG_LEVEL4("GetInstance()");

	st_oMutex.lock();

	if(st_poGImageConverter == NULL)
	{
		st_poGImageConverter = QSharedPointer<GImageConverter>(new GImageConverter());
	}

	st_oMutex.unlock();

	return st_poGImageConverter;
}

void GImageConverter::run()
{
	LOG_LEVEL3("run()");

	if( !connect(this, SIGNAL(AddJob(GImageJob *)), SLOT(ConvertImage(GImageJob *)), Qt::QueuedConnection) )
	{
		LOG_LEVEL1("Can't connect to GImageJob object.");
	}

	exec();

	LOG_LEVEL4("The event loop of this thread has been finished");
}

void GImageConverter::Stop()
{
	LOG_LEVEL3("Stop()");

	if( !disconnect(this, SIGNAL(AddJob(GImageJob *)), this, SLOT(ConvertImage(GImageJob *))) )
	{
		LOG_LEVEL1("Can't disconnect to GImageJob object.");
	}

	quit();
}

bool GImageConverter::ConvertImagesIfNotConverted(const QString &strRawImagesDir, const QString &strConvertedImagesDir) const
{
	LOG_LEVEL4("ConvertImagesIfNotConverted()");

    QString strConvertedImageFullName;
	bool anyConversionDone=false;
	QSharedPointer<GImageConverter> oImageConverter = GImageConverter::GetInstance();

    QDir oRawImagesDir( strRawImagesDir );

    QStringList oFilters;

    oFilters << RawImagesFilter();

    oRawImagesDir.setNameFilters(oFilters);

    QFileInfoList oRawImagesFiles(oRawImagesDir.entryInfoList());

    QFileInfoList::const_iterator itRawImagesFiles;


    for(itRawImagesFiles = oRawImagesFiles.constBegin() ; itRawImagesFiles < oRawImagesFiles.constEnd() ; ++itRawImagesFiles)
    {
        strConvertedImageFullName = strConvertedImagesDir + itRawImagesFiles->baseName() + ConvertedImagesExtension();

        if (!QFile::exists( strConvertedImageFullName ))
        {
            std::auto_ptr<GImageJob>  poImageJob(new GImageJob(itRawImagesFiles->baseName(), itRawImagesFiles->absoluteFilePath(), GImageConverter::st_CBMPImageFormat));

			poImageJob->SetOutputFile(strConvertedImageFullName);

			oImageConverter->AddImageJob(poImageJob.release());

			anyConversionDone = anyConversionDone || true;
		}
    }

	return anyConversionDone;
}

void GImageConverter::ConvertImage(GImageJob *poImageJob)
{
	LOG_LEVEL4("ConvertImage()");

	try
	{
		if (!QFile::exists(poImageJob->GetInputFile()))
		{
			throw GEXCEPTION("Non-existent Image File.");
		}

		QString strNonConvertedImagesPath;

		if (poImageJob->GetBackupFileName()!="") //If no backup directory setted, no copy is made
		{
			LOG_LEVEL3("Copying image for a secure conversion...");
			{
				QDir oOutputDir(QFileInfo(poImageJob->GetBackupFileName()).absoluteDir());

				if (!oOutputDir.exists())
				{
					LOG_LEVEL3("Directory for copying images doesn't exist. Creating it.");
					if (!oOutputDir.mkpath(oOutputDir.absolutePath()))
					{
						throw GEXCEPTION("Couldn't create directory: " + oOutputDir.absolutePath() + ".");
					}
				}
			}

			if (!QFile::copy(poImageJob->GetInputFile(), poImageJob->GetBackupFileName()))
			{
				throw GEXCEPTION("Couldn't copy Image: " + poImageJob->GetInputFile() + " to " + poImageJob->GetBackupFileName() + ".");
			}

			strNonConvertedImagesPath = poImageJob->GetBackupFileName();
		}
		else
			strNonConvertedImagesPath = poImageJob->GetInputFile();


		QImage oIncomeImage(strNonConvertedImagesPath, (poImageJob->GetFileFormat()).constData());

		if (oIncomeImage.isNull())
		{
			throw GEXCEPTION("Wrong Image File Format.");
		}

		
		LOG_LEVEL3("Scaling image: X Scale Factor: " + QString("%1").arg(GetScaleXFactor()) + "; Y Scale Factor: " + QString("%1").arg(GetScaleYFactor()) );

		LOG_LEVEL4("Current Image Size: (" + QString("%1").arg(oIncomeImage.width()) + ";" + QString("%1").arg(oIncomeImage.height()) + ")");

		QImage oScaledImage(oIncomeImage.scaled(oIncomeImage.width()*GetScaleXFactor(), oIncomeImage.height()*GetScaleYFactor()));

		LOG_LEVEL4("New Image Size: (" + QString("%1").arg(oScaledImage.width()) + ";" + QString("%1").arg(oScaledImage.height()) + ")");

		if (oScaledImage.format() == QImage::Format_Mono)
		{
			LOG_LEVEL4("Setting New Image Background Transparency...");
			unsigned int uiI = 0;
			static QRgb const qRgbaWhite(QColor(Qt::white).rgba());
			static QRgb const qRgbaTransparent(QColor(Qt::transparent).rgba());
			while ((uiI < static_cast<unsigned int>(oScaledImage.numColors())) && (oScaledImage.color(uiI) != qRgbaWhite))
			{
				++uiI;
			}
			if ((uiI < static_cast<unsigned int>(oScaledImage.numColors())) && (oScaledImage.color(uiI) == qRgbaWhite))
			{
				oScaledImage.setColor(uiI,qRgbaTransparent);
				LOG_LEVEL4("New Image Background Transparency set.");
			}
			else
			{
				LOG_LEVEL4("New Image Background Transparency cannot be set.");
			}
		}

		{
				QDir oOutputDir(QFileInfo(poImageJob->GetOutputFile()).absoluteDir());

				if (!oOutputDir.exists())
				{
					LOG_LEVEL3("Non-existent output directory: " + QFileInfo(poImageJob->GetOutputFile()).dir().path() + ". Creating it.");
					if (!oOutputDir.mkpath(oOutputDir.absolutePath())) throw GEXCEPTION("Couldn't create directory: " + oOutputDir.absolutePath() + ".");
				}
		}
		
			
		LOG_LEVEL3("Saving image as png...");

		{
			QImage oOutcomeImage(oScaledImage.convertToFormat(QImage::Format_ARGB32, Qt::AutoColor));
			oOutcomeImage.save(poImageJob->GetOutputFile(), QByteArray(ConvertedImagesFormat()).constData());
		}

		LOG_LEVEL3("Image: " + poImageJob->GetInputFile() + " converted.");

		delete poImageJob;

		poImageJob = NULL;

	}
	catch(GException e)
	{

		int iRetries = poImageJob->GetRetries()+1;

		if (iRetries > GetMaxRetries())
		{
			QString strErrMsg;

			strErrMsg = "Error Converting Image: " + poImageJob->GetInputFile() + ". "  + e.GetMessage();

			LOG_LEVEL1(strErrMsg);

			delete poImageJob;

			poImageJob = NULL;
		}
		else
		{
			QString strInfoMsg;

			strInfoMsg = "Couldn't convert image: " + poImageJob->GetInputFile() + ". " + e.GetMessage() + "Sending again ImageJob for retrying...";

			LOG_LEVEL2(strInfoMsg);

			poImageJob->SetRetries(iRetries);

			AddImageJob(poImageJob);
		}
	}
}

void GImageConverter::AddImageJob(GImageJob *poImageJob)
{
	LOG_LEVEL4("AddImageJob()");

	emit AddJob(poImageJob);
}

float GImageConverter::GetScaleXFactor() const
{
	LOG_LEVEL4("GetScaleXFactor()");

	return m_fScaleXFactor;
}

float GImageConverter::GetScaleYFactor() const
{
	LOG_LEVEL4("GetScaleYFactor()");

	return m_fScaleYFactor;
}

int GImageConverter::GetMaxRetries() const
{
	LOG_LEVEL4("GetMaxRetries()");

	return m_iMaxRetries;
}

void GImageConverter::SetScaleXFactor(const float fScaleXFactor)
{
	LOG_LEVEL4("SetScaleXFactor()");

	m_fScaleXFactor = fScaleXFactor;
}

void GImageConverter::SetScaleYFactor(const float fScaleYFactor)
{
	LOG_LEVEL4("SetScaleYFactor()");

	m_fScaleYFactor = fScaleYFactor;
}

