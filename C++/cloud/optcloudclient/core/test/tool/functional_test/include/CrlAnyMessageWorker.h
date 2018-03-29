/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlAnyMessageWorker.h
 * @author gparis
 * @date Friday, May 8, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#ifndef CRLANYMESSAGEWORKER_H_
#define CRLANYMESSAGEWORKER_H_


#include <Poco/SharedPtr.h>

#include <gvr/communication/crl/session/CrlSessionClient.h>
#include <gvr/communication/crl/ICrlSessionListener.h>

#include <workers/AbsOptCloudClientWorker.h>


  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {


      using namespace gvr::communication::crl;

      class CrlAnyMessageWorker : public gvr::cloud::AbsOptCloudClientWorker, public ICrlSessionListener
      {
        public:

          /**
           * @brief default constructor.
           */
          CrlAnyMessageWorker(CrlSessionClient * pSession);

          /**
           * @brief default destructor.
           */
          virtual ~CrlAnyMessageWorker(void);

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

          virtual void sessionEvent (ICrlSessionListener::BufferType const &, ICrlSessionListener::TypeId const &, ICrlSessionListener::KeyType const &);

          virtual void sessionEvent (ICrlSessionListener::StatusType);

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

          Poco::FastMutex               m_mutex;             ///< @brief Synchronization mutex.
          const char  *                 m_pcReplyMessageDir; ///< @brief Directory for reply Json files.
          CrlSessionClient  *           m_pSession;          ///< @brief Server session link
      };


    } // Cloud Domain
  }  //  Gilbarco Domain





#endif /* CRLANYMESSAGEWORKER_H_ */
