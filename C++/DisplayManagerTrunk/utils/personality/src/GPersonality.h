#ifndef GPersonality_H_
#define GPersonality_H_

#include <QString>
#include <QChar>
#include <QVector>

#include "GPersonalityException.h"
#include "utils/configreader/src/GConfigReader.h"

class GPersonality
{
public:
	GPersonality() throw (GPersonalityException);
	virtual ~GPersonality();

	typedef enum
	{
		eLeftArrow,
		eRightArrow
	} eArrowDirection;

	typedef enum
	{
		eWidth,
		eHeight
	} eDimension;

	void Init(QString const & strDefaultResolution="320x240", QString const & strDefaultPersonalityName="") throw (GPersonalityException);

	static QStringList GetPersonalityList( ) throw (GPersonalityException);
	static bool IsAValidPersonality( QString const & strPersonality );

	QString GetStrUrlPrefix() const throw (GPersonalityException);
	QString GetStrClearScreen() const throw (GPersonalityException);
	QString GetStrGraphicsPrefix( QString const & strPrefix ) const throw (GPersonalityException);
	QString GetStrGraphicClockPrefix( QString const & strPrefix ) const throw (GPersonalityException);
	QString GetPersonalityDir() const;
	QString GetCustomGraphicProperty(QString const & strSuffix) const;
	QString GetCustomGraphicProperty(int iSegment) const;
	QString GetClockGraphicProperty(QString const & strSuffix) const;
	QString GetClockGraphicProperty(int iPhase) const;
	QString GetCustomScreenProperty(QString const & strSuffix) const;
	QString GetCustomScreenProperty(int iScreen) const;
	QString GetArrowGraphicProperty(GPersonality::eArrowDirection const iArrow) const;
	QVector<QChar> GetPersonalityLanguageMapping( ) const;
	int GetCustomGraphicPropertyDimension(GPersonality::eDimension const iDimension, QString const & strSuffix) const;
	int GetClockGraphicPropertyDimension(GPersonality::eDimension const iDimension, int iPhase) const;
	int GetArrowGraphicPropertyDimension(GPersonality::eDimension const iDimension, GPersonality::eArrowDirection const iArrow) const;
	int GetOverlapsErrorMarginPixels() const;
	bool IsRemoveOverlaps() const;
	bool GetAValidPath(QString & strPath);

private:
	QString m_strPersonalityDir;
	QString m_strResolution;
	GConfigReader* m_pConfig;

	static const QString CSTRConfigFileName;
	static const QString CSTRURLPrefixParameter;
	static const QString CSTRClearScreenParameter;
	static const QString CSTRCustomGraphicPropertyNamePrefix;
	static const QString CSTRClockGraphicPropertyNamePrefix;
	static const QString CSTRCustomScreenPropertyNamePrefix;
	static const QString CSTRArrowGraphicPropertyNameFrame;
	static const QString CSTRLeftArrowName;
	static const QString CSTRRightArrowName;
	static const QString CSTRCharMappingFileName;
	static const QString CSTRCustomGraphicPropertyDimensionNameFrame;
	static const QString CSTRClockGraphicPropertyDimensionNameFrame;
	static const QString CSTRArrowGraphicPropertyDimensionNameFrame;
	static const QString CSTRDimensionWidthName;
	static const QString CSTRDimensionHeightName;
	static const QString CSTROverlapsErrorMarginPixelsName;
	static const QString CSTRRemoveOverlapsConditionName;
};

#endif
