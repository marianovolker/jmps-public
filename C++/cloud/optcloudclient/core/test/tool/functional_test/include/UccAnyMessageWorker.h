/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccAnyMessageWorker.h
 * @author gparis
 * @date Wednesday, April 29, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef UCCANYMESSAGEWORKER_H_
#define UCCANYMESSAGEWORKER_H_

#include <Poco/SharedPtr.h>

#include <gvr/communication/ucc/session/UccSessionServer.h>
#include <gvr/communication/ucc/IUccSessionListener.h>

#include <workers/AbsOptCloudClientWorker.h>


  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {


      using namespace gvr::communication::ucc;

      class UccAnyMessageWorker : public gvr::cloud::AbsOptCloudClientWorker, public IUccSessionListener
      {
        public:

          /**
           * @brief Protocol names used by UCC session.
           */
          static const char * CProtocols[3];

          /**
           * @brief default constructor.
           */
          UccAnyMessageWorker(UccSessionServer * pSession);

          /**
           * @brief default destructor.
           */
          virtual ~UccAnyMessageWorker(void);

          void setReplyMessageDir (const char* pcReplyDir);

          /**
           * @brief This method is where the thread does the subscribe to events that will received from the UccSession.
           */
          virtual void doSubscriptions ();

          /**
           * @brief This method is where the thread does the unsubscribe to events that was previously subscribed.
           */
          virtual void doUnsubscriptions ();


        protected:

          virtual void sessionEvent (IUccSessionListener::BufferType const &, IUccSessionListener::TypeId const &, IUccSessionListener::KeyType const &);

          virtual void sessionEvent (IUccSessionListener::StatusType);

          /**
           * @brief This method is where the thread does the process an event data notification received.
           */
          virtual void processNotification ( AbsOptCloudClientWorker::TypeId typeId, const AbsOptCloudClientWorker::BufferType& data );

          /**
           * @brief This method is where the thread does the process to each status event notification received.
           */
          virtual void processNotification ( StatusType statusType );

          /**
           * @brief This method is where the thread does the process the specific event received from uccsession.
           */
          void doTheWork ( const AbsOptCloudClientWorker::BufferType& );


          /**
           * @brief Generic integer timeout value.
           */
          static const unsigned int CTimeout = 3000; // 3 seconds

          /**
           * @brief To skip binary header when printing messages in case of debugging.
           */
          static const unsigned int CBinaryHeaderSize = 5;

          /**
           * @brief The default path to the reply Json files.
           */
          static const char * CDefaultMessageReplyDir;


        private:

          Poco::FastMutex               m_mutex;            ///< @brief Synchronization mutex.
          const char  *                 m_pcReplyMessageDir;///< @brief Directory for reply Json files.
          UccSessionServer  *           m_pServerSession;   ///< @brief Server session link
      };


    } // Cloud Domain
  }  //  Gilbarco Domain


#endif /* UCCANYMESSAGEWORKER_H_ */
