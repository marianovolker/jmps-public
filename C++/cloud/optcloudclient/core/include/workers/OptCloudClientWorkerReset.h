/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file OptCloudClientWorkerReset.h
 * @author mvolker
 * @date Monday, March 30, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _OPTCLOUDCLIENTWORKERRESET_H_
# define _OPTCLOUDCLIENTWORKERRESET_H_

#include <workers/AbsOptCloudClientWorker.h>

# include <gvr/communication/ucc/IUccSessionListener.h>
# include <gvr/communication/spot/SpotSession.h>
# include <gvr/communication/ucc/session/UccSession.h>

using namespace gvr::communication::ucc;
using namespace gvr::communication::spot;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class OptCloudClientWorkerReset : public gvr::cloud::AbsOptCloudClientWorker, public IUccSessionListener
      {
        public:

          /**
           * @brief default constructor.
           */
          OptCloudClientWorkerReset( UccSession &, SpotSession & );

          /**
           * @brief default destructor.
           */
          virtual ~OptCloudClientWorkerReset(void);


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

          /**
           * @brief This method is where the thread does the process of the specific event reset received from uccsession.
           */
          void doReset ( const AbsOptCloudClientWorker::BufferType& );


          UccSession  * m_pUccSession;
          SpotSession * m_pSpotSession;
      };
    }
  }

#endif
