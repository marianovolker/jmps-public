//! GURLMsgData
/*!
  URL Message Data
*/

#ifndef GURL_MSG_DATA_H
#define GURL_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"
#include "control/src/displaymanagermsgs/utils/GURLException.h"

class GURLMsgData : public GAbsMsgData
{
public:
	GURLMsgData();
	virtual ~GURLMsgData();

	const QString & GetURL() const;
	void SetURL(const QString &strURL);

	void Validate() const throw(GURLException);

private:
	QString m_strURL;
};

#endif
