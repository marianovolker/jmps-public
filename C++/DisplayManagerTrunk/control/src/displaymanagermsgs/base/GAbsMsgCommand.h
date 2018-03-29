//! GAbsMsgCommand
/*!
  Message Command BaseClass
*/

#ifndef GAbsMsgCommand_H_
#define GAbsMsgCommand_H_

#include "utils/base/src/GAbsCommand.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"

#include <QByteArray>
#include <memory>

class GAbsMsgCommand : public GAbsCommand
{
public:
	GAbsMsgCommand(GAbsData * pData = NULL);	
	virtual ~GAbsMsgCommand();

	virtual void Execute() throw(GException)=0;

	std::auto_ptr<GResponseMsgData>	CreateInitializedResponse(const QString &strXMLTag, const QString &strInterfaceVersion) throw(GException);
	void SetResponseNamespace(std::auto_ptr<GResponseMsgData> &poResponse) throw(GException);
};


inline std::auto_ptr<GResponseMsgData> GAbsMsgCommand::CreateInitializedResponse(const QString &strXMLTag, const QString &strInterfaceVersion) throw(GException)
{
	std::auto_ptr<GResponseMsgData> poResponse(new GResponseMsgData());
	if( !poResponse.get() )
	{
		QString strExMsg("Error creating response message for " + strXMLTag + " message.");
		throw GEXCEPTION(strExMsg);
	}

	poResponse->SetInterfaceVersion(strInterfaceVersion);
	poResponse->SetCommand(strXMLTag);
	
	return poResponse;
}

inline void GAbsMsgCommand::SetResponseNamespace(std::auto_ptr<GResponseMsgData> &poResponse) throw(GException)
{
	GAbsMsgData * pRequestData(static_cast<GAbsMsgData *>(m_pData));
	if( pRequestData )
	{
		poResponse->SetNamespacePrefix(pRequestData->GetNamespacePrefix());
		poResponse->SetNamespaceURI(pRequestData->GetNamespaceURI());
	}
	else
	{
		throw GEXCEPTION("GAbsMsgData is NULL.");
	}
}

#endif
