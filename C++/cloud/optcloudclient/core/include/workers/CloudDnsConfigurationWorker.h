
/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudDnsConfigurationWorker.h
 * @author mgrieco
 * @date Wednesday, Jun 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CLOUDDNSCONFIGURATIONWORKER_H_
#define _CLOUDDNSCONFIGURATIONWORKER_H_

#include <workers/AbsCrlWorker.h>
#include <gvr/communication/spot/SpotSession.h>
#include <gvr/communication/ucc/IUccSessionListener.h>
#include <gvr/communication/ucc/session/UccSession.h>
#include <CloudStatus.h>

// Crl Messages...
#include <gvr/communication/crl/message/CrlMessageGetDnsConfiguration.h>
#include <gvr/communication/crl/message/CrlMessageGetDnsConfigurationResponse.h>
#include <gvr/communication/crl/message/CrlMessageSetDnsConfiguration.h>
#include <gvr/communication/crl/message/CrlMessageSetDnsConfigurationResponse.h>
#include <gvr/communication/crl/message/CrlMessageTestDns.h>
#include <gvr/communication/crl/message/CrlMessageTestDnsResponse.h>

// Ucc Messages...
#include <gvr/communication/ucc/message/UccMessageTestDns.h>
#include <gvr/communication/ucc/message/UccMessageTestDnsResponse.h>


using namespace gvr::communication::ucc;
using namespace gvr::communication::crl;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class CloudDnsConfigurationWorker : public gvr::cloud::AbsCrlWorker
      {
        public:

          /**
           * @brief default constructor.
           */
          CloudDnsConfigurationWorker( UccSession &, CrlSessionServer &, CloudStatus & );

          /**
           * @brief default destructor.
           */
          virtual ~CloudDnsConfigurationWorker(void);


        protected:

          /**
           * @brief Callback for the retrieved message by the session to which the listener is subscribed.
           *
           * @note  This method is called from the thread inside the session, so in this we need to enqueue something
           *        instead of process event received.
           *
           * @param buffer is the message buffer that occur.
           * @param typeId is the received message class type identification of the message received to be parsed.
           * @param key is the received message key identification, which in many protocols is the sequence number.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISessionListener.BufferType
           * @see gvr.communication.ISessionListener.TypeId
           * @see gvr.communication.ISessionListener.KeyType
           *
           * @since 0.0.0
           */
          virtual void sessionEvent (ICrlSessionListener::BufferType const &, ICrlSessionListener::TypeId const &, ICrlSessionListener::KeyType const &);

          /**
           * @brief Callback for the triggered status change by the session to which the listener is subscribed.
           *
           * @note  This method is called from the thread inside the session, so in this we need to enqueue something
           *        instead of process event status received.
           *
           * @param status is the current session status. Session state changes and errors are reported through this callback.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISessionListener.StatusType
           *
           * @since 0.0.0
           */
          virtual void sessionEvent (ICrlSessionListener::StatusType);

          /**
           * @brief This method is where the thread does the subscribe to events that will received from the UccSession.
           */
          virtual void doSubscriptions ();

          /**
           * @brief This method is where the thread does the unsubscribe to events that was previously subscribed.
           */
          virtual void doUnsubscriptions ();

          /**
           * @brief This method is where the thread does the process an event data notification received.
           */
          virtual void processNotification ( AbsOptCloudClientWorker::TypeId typeId, const AbsOptCloudClientWorker::BufferType& data );

          /**
           * @brief This method is where the thread does the process to each status event notification received.
           */
          virtual void processNotification ( StatusType statusType );

          /**
           * @brief UccSession to be used by this worker.
           */
          UccSession & m_uccSession;

          /**
           * @brief CrlSession to be used by this worker.
           */
          CrlSessionServer & m_crlSession;

          /**
           * @brief Cloud status member to be used by this worker.
           */
          CloudStatus & m_cloudStatus;

    private:
          /**
           * @brief This method is where to retrieve the dns configuration.
           * @param the event parsed message
           */
          void doGetDnsConfiguration(CrlMessageGetDnsConfiguration & msg);
          /**
           * @brief This method is where to set a new dns configuration.
           * @param the event parsed message
           */
          void doSetDnsConfiguration(CrlMessageSetDnsConfiguration & msg);
          /**
           * @brief This method is where to test the current dns configuration.
           * @param the event parsed message
           */
          void doTestDnsConfiguration(CrlMessageTestDns & msg);
    };
    }
  }

#endif
