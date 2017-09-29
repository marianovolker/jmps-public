#ifndef GRequestClassifier_H
#define GRequestClassifier_H

#include <QString>
#include <QByteArray>
#include <QRegExp>
#include <QMap>
#include <QMutex>


class GRequestClassifier
{
public:
	typedef enum
	{
		eClearScreenRequest,
		eDownloadGraphicRequest,
		eFlyTextRequest,
		eGdiClearScreenRequest,
		eGdiDrawLineRequest,
		eGdiDrawTextRequest,
		eGdiFillRectRequest,
		eGdiGetScreenSizeRequest,
		eGdiRefreshScreenRequest,
		eInfoScreenRequest,
		eKeyEventRequest,
		eKioskIdRequest,
		eResponse,
		eGetPersonalitiesRequest,
		eSetPersonalityRequest,
		eSetDefaultPersonalityRequest,
		eURLRequest,
		eClockRequest,
		eSetMediaManagerRequest,
		eGetMediaManagersRequest,
		eSetForkFailureOptionRequest,
		eGetForkFailureOptionsRequest,
		eNotFound

	} eRequestType;
	
private:
	typedef QMap<QString, eRequestType> GRequestClassifierMap;
	GRequestClassifierMap m_oKeys;
	QRegExp m_oReg;
	static GRequestClassifier* m_poClassifier;
	static QMutex m_oMutex;
	
public:
	static GRequestClassifier* GetInstance();
	eRequestType GetRequestType(QByteArray & oXML);
		
private:
	GRequestClassifier();
	virtual ~GRequestClassifier();
};

#endif
