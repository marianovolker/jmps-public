/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file OptCloudClientWorkerFileActivation.h
 * @author mvolker
 * @date Monday, March 30, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _OPTCLOUDCLIENTWORKERFILEACTIVATION_H_
# define _OPTCLOUDCLIENTWORKERFILEACTIVATION_H_

#include <workers/AbsOptCloudClientWorker.h>

#include <gvr/communication/spot/SpotSession.h>
#include <gvr/communication/ucc/session/UccSession.h>
#include <gvr/communication/spot/ISpotSessionListener.h>
#include <gvr/communication/ucc/IUccSessionListener.h>

using namespace gvr::communication::ucc;
using namespace gvr::communication::spot;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {


      class OptCloudClientWorkerFileActivation : public gvr::cloud::AbsOptCloudClientWorker, public IUccSessionListener
      {
        public:
        /**
         * @brief Mark to signal rebooting after package activation.
         */
        static const char * CActivationMarkTempFilename;


          /**
           * @brief default constructor.
           */
          OptCloudClientWorkerFileActivation( UccSession &, SpotSession & );

          /**
           * @brief default destructor.
           */
          virtual ~OptCloudClientWorkerFileActivation(void);

          /**
           * @brief Checks if this starting up of this application has been a consequence of an immediately
           *        recent package activation operation triggered by the cloud.
           */
          static bool hasBeenPackageActivationPerformed();

          /**
           * @brief Notifies the cloud with an event that the last installation was finished OK.
           */
          void notifyCloudInstallationFinished () const;

          /**
           * @brief Notifies the cloud with an event that the last installation was finished OK.
           *
           * @param uccSession is the session to use to send the installation status.
           *
           * @return True if successful. False otherwise.
           */
          static bool notifyCloudInstallationFinished (UccSession &);

        protected:

          virtual void sessionEvent (IUccSessionListener::BufferType const &, IUccSessionListener::TypeId const &, IUccSessionListener::KeyType const &);

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
           * @brief This method is where the thread does the process of the specific software install event received from uccsession.
           */
          void doActivation ( const AbsOptCloudClientWorker::BufferType& );


          /**
           * @brief Timeout for sending package activation and receiving the corresponding response.
           */
          static const unsigned int CSendActivationTimeout = 3000; // 3 seconds

          /**
           * @brief To skip binary header when printing messages in case of debugging.
           */
          static const unsigned int CBinaryHeaderSize = 5;


          UccSession  * m_pUccSession;
          SpotSession * m_pSpotSession;
      };


    }
  }

#endif // _OPTCLOUDCLIENTWORKERFILEACTIVATION_H_

