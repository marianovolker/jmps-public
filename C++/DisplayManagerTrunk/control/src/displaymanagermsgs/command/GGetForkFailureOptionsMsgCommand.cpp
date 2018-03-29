#include "GGetForkFailureOptionsMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GGetForkFailureOptionsMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/media/src/GMediaManagerFactory.h"
#include "utils/logger/src/GLogger.h"

#include <QStringList>
#include <memory>

GGetForkFailureOptionsMsgCommand::GGetForkFailureOptionsMsgCommand(GAbsData * pData)
: GAbsMsgCommand(pData) {
    LOG_LEVEL4("GGetForkFailureOptionsMsgCommand()");
}

GGetForkFailureOptionsMsgCommand::~GGetForkFailureOptionsMsgCommand() {
    LOG_LEVEL4("~GGetForkFailureOptionsMsgCommand()");
}

void GGetForkFailureOptionsMsgCommand::Execute() throw (GException) {
    LOG_LEVEL4("Execute()");

    std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GGetForkFailureOptionsMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

    try {
        GGetForkFailureOptionsMsgData * pRequestData(static_cast<GGetForkFailureOptionsMsgData *> (m_pData));
        if (pRequestData == NULL) {
            throw GEXCEPTION("Null pointer for GGetForkFailureOptionsMsgData *pRequestData");
        }
        pRequestData->Validate();

        QByteArray strResult;
        QChar const aUnicodeAsNullChar(0x0000);
        bool bFirst(true);

        // Set up our data to return over the XML message back to the CRIND
        // It's expecting this format: currentSetting\0forkFailureOptionsList
        //   For example: Enabled\0Enabled,Disabled
        // First we add the current setting that's being used (and the null)...
        strResult += GETCONF_STR_FROM_FILESYSTEM(eDisplayManagerMediaManagerForkFailedRestart);
        strResult.append(aUnicodeAsNullChar);

        // then we add the complete list of possible choices so they can be
        // displayed in the Diagnostics menu
        foreach(QString str, GMediaManagerFactory::GetForkFailureOptionList()) {
            if (bFirst) {
                strResult += str;
                bFirst = false;
            } else {
                strResult += "," + str;
            }
        }
        strResult.append(aUnicodeAsNullChar);

        LOG_LEVEL4(QString("Data: ") + strResult);
        poResponse->SetData(strResult.toHex());
        poResponse->SetResponseCode(GResponseMsgData::eOk);
    } catch (GException& e) {
        LOG_LEVEL1(QString("Exception: ") + e.GetMessage());

        poResponse->SetResponseCode(GResponseMsgData::eCantGetForkFailureOptionsList);
    }

    SetResponseNamespace(poResponse);
    m_pResponse = poResponse.release();
}
