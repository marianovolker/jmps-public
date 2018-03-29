/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file DebianInfoCollectorWorker.h
 * @author gparis
 * @date Tuesday, April 07, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _DEBIANINFOCOLLECTORWORKER_H_
#define _DEBIANINFOCOLLECTORWORKER_H_

# include <gvr/communication/spot/ISpotSessionListener.h>
# include <gvr/communication/spot/SpotSession.h>
# include <workers/AbsOptCloudClientWorker.h>

# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>


using namespace gvr::communication::spot;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class DebianInfoCollectorWorker : public gvr::cloud::AbsOptCloudClientWorker, public ISpotSessionListener
      {
        public:

          typedef std::map<std::string, std::string>  TextInfoMap;

          /**
           * @brief default constructor.
           */
          DebianInfoCollectorWorker( SpotSession & );

          /**
           * @brief default destructor.
           */
          virtual ~DebianInfoCollectorWorker(void);


          /**
           * @brief Collect debian information using the spot session.
           *
           *
           * @param label to use when the message is building.
           * @param map to use when the message is building.
           *
           * @exception Throws no exception.
           */
          bool collectInformation( std::string& , TextInfoMap& );



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
          virtual void sessionEvent (ISpotSessionListener::BufferType const &, ISpotSessionListener::TypeId const &, ISpotSessionListener::KeyType const &);

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
          virtual void sessionEvent (ISpotSessionListener::StatusType);

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
          void DoDebianPkgInfoCollection ( const AbsOptCloudClientWorker::BufferType& );

          /**
           * @brief This method is where the thread does the process of the specific event reset received from uccsession.
           */
          bool fillFieldsWithInformationCollected( std::string&, TextInfoMap& );

          /**
           * @brief Check if the task was completed.
           */
          bool isWorkDone() const;

          /**
           * @brief Check if the task has errors.
           */
          bool hasError() const;

          /**
           * @brief Pattern to find "line 0", the first line of the debian list obtained from the command dpkg -l.
           */
          static const char * CdpkgLine0;

          typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;

          Poco::FastMutex     m_mtxblWorkDone;
          bool                m_blWorkDone;

          Poco::FastMutex     m_mtxblError;
          bool                m_blError;

          Poco::FastMutex     m_mtxdebianInfoStream;
          std::stringstream   m_debianInfoStream;

          SpotSession&        m_spotSession;
      };
    }
  }

#endif
