//! GResponseMsgData
/*!
  Response Message Data
*/

#ifndef GRESPONSE_MSG_DATA_H
#define GRESPONSE_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>
#include <QByteArray>

class GResponseMsgData : public GAbsMsgData
{
public:
	typedef enum
	{
		eOk,
		eParseError,
		eInvalidPersonality,
		eUnknownError,
		eError,
		eInfoScreenError,
		eURLError,
		eGDIError,
		eCantGetPersonalityList,
		eInvalidMediaManagerType,
		eCantGetMediaManagerList,
		eInvalidForkFailureOption,
		eCantGetForkFailureOptionsList

	} eResponseCode;
	
	static const QString CResponseOk;
	static const QString CResponseParseError;
	static const QString CResponseInvalidPersonality;
	static const QString CResponseUnknownError;
	static const QString CResponseError;
	static const QString CResponseInfoScreenError;
	static const QString CResponseURLError;
	static const QString CResponseGDIError;
	static const QString CResponseCantGetPersonalityList;
	static const QString CResponseInvalidMediaManagerType;
	static const QString CResponseCantGetMediaManagerList;
	static const QString CResponseInvalidForkFailureOption;
	static const QString CResponseCantGetForkFailureOptionsList;

public:
	GResponseMsgData();
	virtual ~GResponseMsgData();

	const QString & GetCommand() const;
	void SetCommand( const QString & strCommand);

	const QByteArray & GetData();
	void SetData(const QByteArray & strData);

	eResponseCode GetResponseCode();
	void SetResponseCode(eResponseCode eCode);

private:
	QString m_strCommand;
	QByteArray m_oData;
	eResponseCode m_eCode;
};

#endif
