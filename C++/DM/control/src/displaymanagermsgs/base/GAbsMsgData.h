//! GAbsMsgData 
/*!
  Requests base class
*/

#ifndef GAbsMsgData_H_
#define GAbsMsgData_H_

#include <QString>

#include "utils/base/src/GAbsData.h"

class GAbsMsgData : public GAbsData
{
public:
	GAbsMsgData();
	virtual ~GAbsMsgData();

	QString GetInterfaceVersion();
	void SetInterfaceVersion(QString strInterfaceVersion);

	QString GetNamespacePrefix();
	void SetNamespacePrefix(QString strNamespacePrefix);

	QString GetNamespaceURI();
	void SetNamespaceURI(QString strNamespaceURI);

	bool GetSynchronousRequest();
	void SetSynchronousRequest(bool bSynchronousRequest);

	virtual void Validate() throw(GException);	

protected:
	QString m_strInterfaceVersion;
	QString m_strNamespacePrefix;
	QString m_strNamespaceURI;
	bool    m_bSynchronousRequest;
};

#endif
