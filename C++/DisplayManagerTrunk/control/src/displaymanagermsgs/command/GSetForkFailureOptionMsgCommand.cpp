#include "GSetForkFailureOptionMsgCommand.h"

#include "control/src/displaymanagermsgs/xml/GSetForkFailureOptionMsgDataXML.h"
#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/media/src/GMediaManagerFactory.h"
#include "utils/logger/src/GLogger.h"

#include <memory>

GSetForkFailureOptionMsgCommand::GSetForkFailureOptionMsgCommand(GAbsData * pData)
: GAbsMsgCommand(pData) {
    LOG_LEVEL4("GSetForkFailureOptionMsgCommand()");
}

GSetForkFailureOptionMsgCommand::~GSetForkFailureOptionMsgCommand() {
    LOG_LEVEL4("~GSetForkFailureOptionMsgCommand()");
}

void GSetForkFailureOptionMsgCommand::Execute() throw (GException) {
    LOG_LEVEL4("Execute()");

    std::auto_ptr<GResponseMsgData> poResponse(CreateInitializedResponse(GSetForkFailureOptionMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

    try {
        GSetForkFailureOptionMsgData * pRequestData(static_cast<GSetForkFailureOptionMsgData *> (m_pData));
        if (pRequestData == NULL) {
            throw GEXCEPTION("Null pointer for GSetForkFailureOptionMsgData *pRequestData");
        }
        pRequestData->Validate();

        // Set up our data to return over the XML message back to the CRIND
        // In this case it's a simple success or failure that the Fork Failure Option
        // sent was valid and that we were able to store it in the config file.
        if (GMediaManagerFactory::IsAValidForkFailureOption(pRequestData->GetType()) && SETCONF_TO_FILESYSTEM(eDisplayManagerMediaManagerForkFailedRestart, pRequestData->GetType())) {
            poResponse->SetResponseCode(GResponseMsgData::eOk);
        } else {
            poResponse->SetResponseCode(GResponseMsgData::eInvalidForkFailureOption);
        }
    } catch (GException& e) {
        LOG_LEVEL1(QString("Exception: ") + e.GetMessage());

        poResponse->SetResponseCode(GResponseMsgData::eInvalidForkFailureOption);
    }

    SetResponseNamespace(poResponse);
    m_pResponse = poResponse.release();
}
