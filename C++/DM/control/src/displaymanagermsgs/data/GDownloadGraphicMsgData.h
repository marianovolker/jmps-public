//! GDownloadGraphicMsgData 
/*!
  Download Graphics Message Data
*/

#ifndef GDOWNLOAD_GRAPHIC_MSG_DATA_H
#define GDOWNLOAD_GRAPHIC_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include <QString>

class GDownloadGraphicMsgData : public GAbsMsgData
{
public:
	GDownloadGraphicMsgData();
	virtual ~GDownloadGraphicMsgData();

	QString GetScreen();
	void SetScreen(QString strScreen);
	
	QString GetType();
	void SetType(QString strType);
	
	QString GetCRC();
	void SetCRC(QString strCRC);
	
	QString  GetDisplay();
	void SetDisplay(QString strDisplay);
	
	QString GetFilename();
	void SetFilename(QString strFilename);
	
	QString GetSegment();
	void SetSegment(QString strSegment);

	static const QString st_CRequestMsgImageType;

private:
	QString m_strScreen;
	QString m_strType;
	QString m_strCRC;
	QString m_strDisplay;
	QString m_strFilename;
	QString m_strSegment;
};

#endif
