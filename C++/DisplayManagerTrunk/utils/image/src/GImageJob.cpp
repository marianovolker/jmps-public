#include"GImageJob.h"

#include "utils/logger/src/GLogger.h"


GImageJob::GImageJob()
:
	m_iId(0),
	m_strName(""),
	m_strInputFile(""),
	m_strFileFormat(""),
	m_strOutPutFile(""),
	m_strBackupFileName(""),
	m_iRetries(0)
{}


GImageJob::GImageJob(const QString &strName, const QString &strInputFile, const int iId, const QByteArray &strFileFormat=""):
	m_iId(iId),
	m_strName(strName),
	m_strInputFile(strInputFile),
	m_strFileFormat(strFileFormat),
	m_strOutPutFile(""),
	m_strBackupFileName(""),
	m_iRetries(0)
{}


GImageJob::GImageJob(const QString &strName, const QString &strInputFile, const QByteArray &strFileFormat=""):
	m_iId(0),
	m_strName(strName),
	m_strInputFile(strInputFile),
	m_strFileFormat(strFileFormat),
	m_strOutPutFile(""),
	m_strBackupFileName(""),
	m_iRetries(0)
{}


GImageJob::GImageJob(const GImageJob& oImageJob):
	m_iId(oImageJob.GetId()),
	m_strName(oImageJob.GetName()),
	m_strInputFile(oImageJob.GetInputFile()),
	m_strFileFormat(oImageJob.GetFileFormat()),
	m_strOutPutFile(oImageJob.GetOutputFile()),
	m_strBackupFileName(oImageJob.GetBackupFileName()),
	m_iRetries(oImageJob.GetRetries())
{


}

GImageJob::~GImageJob()
{}


int GImageJob::GetId() const
{
	return m_iId;
}


const QString GImageJob::GetName() const
{
	LOG_LEVEL4("GetName()");
	return m_strName;
}


const QString GImageJob::GetInputFile() const
{
	LOG_LEVEL4("GetInputFile()");
	return m_strInputFile;
}


const QByteArray GImageJob::GetFileFormat() const
{
	LOG_LEVEL4("GetFileFormat()");
	return m_strFileFormat;
}


const QString GImageJob::GetOutputFile() const
{
	LOG_LEVEL4("GetOutputFile()");
	return m_strOutPutFile;
}


const QString GImageJob::GetBackupFileName() const
{
	LOG_LEVEL4("GetBackupFileName()");
	return m_strBackupFileName;
}


int GImageJob::GetRetries() const
{
	LOG_LEVEL4("GetRetries()");
	return m_iRetries;
}


void GImageJob::SetId(const int iId)
{
	LOG_LEVEL4("SetId()");
	m_iId = iId;
}


void GImageJob::SetName(const QString &strName)
{
	LOG_LEVEL4("SetName()");
	m_strName = strName;
}


void GImageJob::SetInputFile(const QString &strInputFile)
{
	LOG_LEVEL4("SetInputFile()");
	m_strInputFile = strInputFile;
}


void GImageJob::SetFileFormat(const QByteArray &strFileFormat)
{
	LOG_LEVEL4("SetFileFormat()");
	m_strFileFormat = strFileFormat;
}


void GImageJob::SetOutputFile(const QString &strOutputFile)
{
	LOG_LEVEL4("SetOutputFile()");
	m_strOutPutFile = strOutputFile;
}


void GImageJob::SetBackupFileName(const QString &strBackupFileName)
{
	LOG_LEVEL4("SetBackupFileName()");
	m_strBackupFileName = strBackupFileName;
}


void GImageJob::SetRetries(const int iRetries)
{
	LOG_LEVEL4("SetRetries()");
	m_iRetries = iRetries;
}

