/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudReconnectWorker.h
 * @author mgrieco
 * @date Monday, Jun 08, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CLOUDRECONNECTWORKER_H_
# define _CLOUDRECONNECTWORKER_H_

#include <workers/AbsOptCloudClientWorker.h>

#include <gvr/communication/spot/SpotSession.h>
#include <gvr/communication/ucc/IUccSessionListener.h>
#include <gvr/communication/ucc/session/UccSession.h>

# include <CloudStatus.h>

using namespace gvr::communication::ucc;
using namespace gvr::communication::spot;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class CloudReconnectWorker : public gvr::cloud::AbsOptCloudClientWorker, public IUccSessionListener
      {
        public:

          /**
           * @brief default constructor.
           * @param the current OPTCloudClient ucc session
           */
          CloudReconnectWorker( UccSession &, SpotSession &, CloudStatus & );

          /**
           * @brief default destructor.
           */
          virtual ~CloudReconnectWorker(void);


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
          virtual void sessionEvent (IUccSessionListener::BufferType const &, IUccSessionListener::TypeId const &, IUccSessionListener::KeyType const &);

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
          virtual void sessionEvent (IUccSessionListener::StatusType);

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

          /**@brief the current OPTCloudClient ucc session*/
          UccSession  * m_pUccSession;
          /**@brief the current OPTCloudClient spot session*/
          SpotSession * m_pSpotSession;
          /** @brief the current cloud status.*/
          CloudStatus & m_cloudStatus;

    private:

          /**
           * @brief This method is where the thread does the process of the specific event reconnect received from uccsession.
           * @param the event data received to start the reconnect
           */
          void doReconnect ( const AbsOptCloudClientWorker::BufferType& );

          /**
           * @brief This method is where the thread try a new ucc connection.
           * @param The decrypte data to use in a connection.
           */
          void doConnect ( const std::string& strDecryptedData );

          /**
           * @brief This method is where the thread try a new ucc connection.
           * @param The reconnect response success value
           */
          void sendReconnectResponse(const std::string& messgaeId, const bool reconnectStartOK);

          /**
           * @brief This method decrypted  the connection encrypted data.
           * @param The decrypted data
           */
          bool getDecryptedConnectionData(std::string& decryptedData);

          /**
           * @brief This method replace the old encrypted file with the new one.
           * @param The old encrypted file
           * @param The new encryptedfile
           */
          void replaceOldEncryptedConnectionData(const std::string& oldEncryptedDataFile, const std::string& newEncryptedDataFile);

      };
    }
  }

#endif
