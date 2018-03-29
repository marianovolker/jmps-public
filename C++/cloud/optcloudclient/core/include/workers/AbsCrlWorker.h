/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsCrlWorker.h
 * @author mvolker
 * @date Monday, April 20, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSCRLWORKER_H_
#define _ABSCRLWORKER_H_

# include <workers/AbsOptCloudClientWorker.h>

# include <gvr/communication/crl/ICrlSessionListener.h>
# include <gvr/communication/crl/session/CrlSessionServer.h>

using namespace gvr::communication::crl;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class AbsCrlWorker : public gvr::cloud::AbsOptCloudClientWorker, public gvr::communication::crl::ICrlSessionListener
      {
        public:

          /**
           * @brief default constructor.
           */
          AbsCrlWorker( const std::string & , CrlSessionServer & );

          /**
           * @brief default destructor.
           */
          virtual ~AbsCrlWorker(void);

          /**
           * @brief Obtain internal crl session.
           */
          virtual CrlSessionServer * getSession(void) { return m_pCrlSession; }


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
          virtual void sessionEvent (ICrlSessionListener::BufferType const &, ICrlSessionListener::TypeId const &, ICrlSessionListener::KeyType const &) = 0;

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
          virtual void sessionEvent (ICrlSessionListener::StatusType) = 0;

          /**
           * @brief This method is where the thread does the subscribe to events that will received from the UccSession.
           */
          virtual void doSubscriptions () = 0;

          /**
           * @brief This method is where the thread does the unsubscribe to events that was previously subscribed.
           */
          virtual void doUnsubscriptions () = 0;

          /**
           * @brief This method is where the thread does the process an event data notification received.
           */
          virtual void processNotification ( AbsOptCloudClientWorker::TypeId typeId, const AbsOptCloudClientWorker::BufferType& data ) = 0;

          /**
           * @brief This method is where the thread does the process to each status event notification received.
           */
          virtual void processNotification ( StatusType statusType ) = 0;




          /**
           * @brief CrlSession to be used by this crlManager.
           */
          CrlSessionServer * m_pCrlSession;
      };
    }
  }

#endif
