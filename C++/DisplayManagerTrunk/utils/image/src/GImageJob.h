#ifndef GImageJob_H_
#define GImageJob_H_

#include<QString>
#include<QByteArray>
//#include <QObject>

class GImageJob// : public QObject
{
//	Q_OBJECT

public:


	GImageJob();


	GImageJob(const QString &name, const QString &inputFile, const int id, const QByteArray &fileFormat);
        
	GImageJob(const QString &strName, const QString &strInputFile, const QByteArray &strFileFormat);

	GImageJob(const GImageJob&);


	~GImageJob();

	int GetId() const;
	const QString GetName() const;
	const QString GetInputFile() const;
	const QByteArray GetFileFormat() const;
	const QString GetOutputFile() const;
	const QString GetBackupFileName() const;
	int GetRetries() const;

	void SetId(const int iId);
	void SetName(const QString &strName);
	void SetInputFile(const QString &strInputFile);
	void SetFileFormat(const QByteArray &strFileFormat);
	void SetOutputFile(const QString &strOutputFile);
	void SetBackupFileName(const QString &strBackupFileName);
	void SetRetries(const int iRetries);


private:

	int m_iId;
	QString m_strName;
	QString m_strInputFile;
	QByteArray m_strFileFormat;
	QString m_strOutPutFile;
	QString m_strBackupFileName;
	int m_iRetries;

};

#endif
