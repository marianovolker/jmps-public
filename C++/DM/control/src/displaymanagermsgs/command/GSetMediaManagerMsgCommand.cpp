#include "GSetMediaManagerMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GSetMediaManagerMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/media/src/GMediaManagerFactory.h"
#include "utils/logger/src/GLogger.h"

#include <memory>

GSetMediaManagerMsgCommand::GSetMediaManagerMsgCommand(GAbsData * pData)
: GAbsMsgCommand(pData) {
    LOG_LEVEL4("GSetMediaManagerMsgCommand()");
}

GSetMediaManagerMsgCommand::~GSetMediaManagerMsgCommand() {
    LOG_LEVEL4("~GSetMediaManagerMsgCommand()");
}

void GSetMediaManagerMsgCommand::Execute() throw (GException) {
    LOG_LEVEL4("Execute()");

    std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GSetMediaManagerMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

    try {
        GSetMediaManagerMsgData * pRequestData(static_cast<GSetMediaManagerMsgData *> (m_pData));
        if (pRequestData == NULL) {
            throw GEXCEPTION("Null pointer for GSetMediaManagerMsgData *pRequestData");
        }
        pRequestData->Validate();

        // Set up our data to return over the XML message back to the CRIND
        // In this case it's a simple success or failure that the MediaManager type
        // sent was valid and that we were able to store it in the config file.
        if (GMediaManagerFactory::IsAValidMediaManagerType(pRequestData->GetType()) && SETCONF_TO_FILESYSTEM(eDisplayManagerMediaManagerType, pRequestData->GetType())) {
            poResponse->SetResponseCode(GResponseMsgData::eOk);
        } else {
            poResponse->SetResponseCode(GResponseMsgData::eInvalidMediaManagerType);
        }
    } catch (GException& e) {
        LOG_LEVEL1(QString("Exception: ") + e.GetMessage());

        poResponse->SetResponseCode(GResponseMsgData::eInvalidMediaManagerType);
    }

    SetResponseNamespace(poResponse);
    m_pResponse = poResponse.release();
}
