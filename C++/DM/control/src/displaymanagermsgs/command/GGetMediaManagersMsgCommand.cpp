#include "GGetMediaManagersMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GGetMediaManagersMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/media/src/GMediaManagerFactory.h"
#include "utils/logger/src/GLogger.h"

#include <QStringList>
#include <memory>

GGetMediaManagersMsgCommand::GGetMediaManagersMsgCommand(GAbsData * pData)
: GAbsMsgCommand(pData) {
    LOG_LEVEL4("GGetMediaManagersMsgCommand()");
}

GGetMediaManagersMsgCommand::~GGetMediaManagersMsgCommand() {
    LOG_LEVEL4("~GGetMediaManagersMsgCommand()");
}

void GGetMediaManagersMsgCommand::Execute() throw (GException) {
    LOG_LEVEL4("Execute()");

    std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GGetMediaManagersMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

    try {
        GGetMediaManagersMsgData * pRequestData(static_cast<GGetMediaManagersMsgData *> (m_pData));
        if (pRequestData == NULL) {
            throw GEXCEPTION("Null pointer for GGetMediaManagersMsgData *pRequestData");
        }
        pRequestData->Validate();

        QByteArray strResult;
        QChar const aUnicodeAsNullChar(0x0000);
        bool bFirst(true);

        // Set up our data to return over the XML message back to the CRIND
        // It's expecting this format: currentMediaManager\0mediaManagerList
        //   For example: daemon\0gstlaunch,daemon
        // First we add the current media manager that's being used (and the null)...
        strResult += GETCONF_STR_FROM_FILESYSTEM(eDisplayManagerMediaManagerType);
        strResult.append(aUnicodeAsNullChar);

        // then we add the complete list of possible choices so they can be
        // displayed in the Diagnostics menu
        foreach(QString str, GMediaManagerFactory::GetMediaManagerList()) {
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
        LOG_LEVEL1(QString("Excepcion: ") + e.GetMessage());

        poResponse->SetResponseCode(GResponseMsgData::eCantGetMediaManagerList);
    }

    SetResponseNamespace(poResponse);
    m_pResponse = poResponse.release();
}
