#ifndef GPersonalityView_H_
#define GPersonalityView_H_

#include <QObject>
#include <QString>
#include <QVector>
#include <QPoint>
#include <QChar>
#include <QRect>
#include <QSize>
#include <QMap>
#include <QMutex>
#include <QSharedPointer>

#include "utils/base/src/GException.h"
#include "utils/personality/src/GPersonality.h"

class GSetPreloadedPagesRequest;

class GPersonalityView
	:	public QObject
{
	Q_OBJECT
public:
	static QSharedPointer<GPersonalityView> GetInstance (void) throw(GException);
	virtual ~GPersonalityView (void);

protected:
	GPersonalityView (void);
	void Init (void);
	void PresetView (bool bSynchronize = false);

public:
	enum {eLeftArrowDirection = 0, eRightArrowDirection = 1};
	enum {eClockPhaseMin = 1, eClockPhaseMax = 8};

public:
	void RefreshPersonality (bool bSynchronize = false);
	bool MayRemoveOverlaps (void);
	unsigned int ObtainOverlapsErrorMarginPixels (void);
	bool ObtainImageFromPersonality (QString& strImagePath, int iImageId);
	bool ObtainClockCharacterImageFromPersonality (QString& strImagePath, int iClockPhase);
	bool ObtainArrowCharacterImageFromPersonality (QString& strImagePath, int iArrowDirection);
	bool ObtainDefaultScreenURL (QString & strScreenURL);
	bool ObtainCustomScreenURL (QString & strScreenURL, int iScreenNumber);
	bool ObtainScreenURL (QString & strScreenURL, int iScreenNumber = 0);
	QString ConvertExtendedASCIIToUTF8 (QString const& sInfoScreenText);
	unsigned int ObtainTextFieldAttrFlag (unsigned int uiTxtAttr);
	QString GenerateSegmentScript (QString const & sContent, QString const & sSegmentName, QString const & sSegmentParent, QRect const & qPosition, bool bIsVisible, bool bFloatingSegment = false);
	QString GenerateTextFieldScript (QString const & sText, QString const & sFieldName, QString const & sFieldParent, QPoint const & qPosition, unsigned int uiTxtAttrFlag, QString const & sFontName, QString const & sBackgroundColour, QString const & sForegroundColour, bool bFloatingText, bool bRemoveOverlaps, unsigned int uiOverlapMargin);

signals:
	void SignalSendRequest (GSetPreloadedPagesRequest*);

private:
	static QSharedPointer<GPersonalityView> m_pGPersonalityView;
	static QMutex m_mutex;
	QMutex m_mutex_instance;
	GPersonality m_oPersonality;
	QVector<QChar> m_qvInfoscreenLanguageMapping;
	QMap<unsigned int,QSize> m_qzClockDimension;
	QMap<unsigned int,QSize> m_qzArrowDimension;
	unsigned int m_uiSessionId;
};

#endif
