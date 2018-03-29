#ifndef GIMAGEMAP_H_
#define GIMAGEMAP_H_

#include <QString>

class GImageMap
{
public:
	GImageMap(const QString &strName, const QString &strFileName, int iSegment);
	GImageMap(const QString &strName, const QString &strFileName, int iSegment, const QString &strFileFormat);
	
	int GetSegment() const;
	const QString &GetName() const;
	
	const QString &GetFileName() const;
	void SetFileName(const QString &stName);
	
	const QString &GetFileFormat() const;
	void SetFileFormat(const QString &stFormat);
	
public:
	static const QString CXMLTag;
	static const QString CXMLSegment;
	static const QString CXMLName;
	static const QString CXMLFileName;
	static const QString CXMLFileFormat;
	
private:
	int m_iSegment;
	QString m_strName;
	QString m_strFileName;
	QString m_strFileFormat;
};

#endif
