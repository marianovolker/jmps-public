/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlManager.h
 * @author mvolker
 * @date Tuesday, April 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLMANAGER_H_
# define _CRLMANAGER_H_

# include <set>

# include <gvr/communication/StreamChannelServer.h>
# include <gvr/communication/IChannelListener.h>
# include <gvr/communication/IChannelServerListener.h>

# include <gvr/communication/spot/SpotSession.h>
# include <gvr/communication/ucc/session/UccSession.h>
# include <gvr/communication/crl/session/CrlSessionServer.h>

# include <workers/AbsCrlWorker.h>

# include <CloudStatus.h>
# include <CloudConfigDeviceChecker.h>

# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>
# include <Poco/Thread.h>
# include <Poco/Runnable.h>
# include <Poco/Event.h>
# include <Poco/NotificationQueue.h>
# include <Poco/SharedPtr.h>

using namespace gvr::communication::ucc;
using namespace gvr::communication::spot;
using namespace gvr::communication::crl;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace cloud
    {
        typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;

        /**
         * @brief This is the class for SPOT channel.
         * 
         * @see gvr.communication.IChannel
         * 
         * @since 0.0.0, 2014-12-16
         * @version 0.0.0, 2014-12-16
         * @author mvolker
         */
        class CrlManager: public gvr::communication::IChannelServerListener, public gvr::communication::IChannelListener
        {
          public:

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            CrlManager(const std::string& host, int port, UccSession &, SpotSession &, CloudStatus &, CloudConfigDeviceChecker &);

            /**
             * @brief Destructor.
             *
             * @since 0.0.0
             */
            virtual ~CrlManager (void);



          protected:

            /**
             * @brief Callback for the retrieved connected channel to which the listener is subscribed.
             *
             * @param the channel link where the event was occur.
             *
             * @exception Throws no exception.
             *
             * @since 0.0.0
             */
            virtual void channelStartedEvent (const Poco::Net::StreamSocket&);

            /**
             * @brief Callback for the retrieved buffer by the channel to which the listener is subscribed.
             *
             * @param buffer is the buffer retrieved from the channel link.
             *
             * @exception Throws no exception.
             *
             * @see gvr.communication.IChannelListener.BufferType
             *
             * @since 0.0.0
             */
            virtual void channelEvent (BufferType const &){};

            /**
             * @brief Callback for the new event has occurred reported by the channel to which the listener is subscribed.
             *
             * @param StatusType is the new event has occurred reported by the channel.
             *
             * @exception Throws no exception.
             *
             * @see gvr.communication.IChannelListener.StatusType
             *
             * @since 0.0.0
             */
            virtual void channelEvent (StatusType);

            /**
             * @brief Helper method to run admin task for the internal thread of CrlManger.
             *
             */
            virtual void checkNotifications();

            /**
             * @brief Helper method to run admin task for the internal thread of CrlManger.
             *
             */
            virtual void removeObjectsByStatus( StatusType status );

            /**
             * @brief Do the startup of the Admin Worker.
             *
             * @since 0.0.0
             */
            virtual bool startAdminWorker (void);

            /**
             * @brief Do the stop of the Admin Worker.
             *
             * @since 0.0.0
             */
            virtual bool stopAdminWorker (void);


            /**
             * @brief This subclass is a helper to create a CrlManager Worker
             * to obtain data from the socket.
             *
             * @see Poco::Net::StreamSocket
             * @see Poco::Runnable
             * @see Poco::Thread
             *
             * @since 0.0.0, 2014-12-16
             * @version 0.0.0, 2014-12-16
             * @author mvolker
             */
            class CrlManagerWorker: public Poco::Runnable
            {
              public:
                CrlManagerWorker();
                virtual ~CrlManagerWorker();

                bool start(CrlManager& crlManager);
                bool stop();

              private:
                bool isRunning();
                bool turnOn();
                bool turnOff();
                void run();

                // Protect the access from the outside, public interface ...
                Poco::FastMutex m_mxCrlManagerWorker;

                Poco::FastMutex m_mxCrlManager;
                CrlManager* m_pCrlManager;

                Poco::FastMutex m_mxRun;
                bool m_run;

                Poco::Thread m_thread;
                Poco::Event m_readyStarted;
                Poco::Event m_readyStopped;
            };


            /**
             * @brief This subclass is for internal communication between threads.
             */
            class WorkerNotification: public Poco::Notification
            {
              public:

                WorkerNotification(StatusType statusType):
                  m_status(statusType)
                {
                }

                virtual ~WorkerNotification()
                {
                }

                StatusType getStatus() const
                {
                  return m_status;
                }

              private:
                StatusType m_status;
            };


            /**
             * @brief This method is where the thread does the process to each notification received.
             *        Once notification is processed in this method is needed to release the pointer to notification.
             */
            void processNotification ( WorkerNotification* pNotification = 0 );


            /**
             * @brief Helper method to remove all elements created dynamically.
             */
            void terminateCleanly ( );


            /**
             * @brief Helper method to remove channel from the list.
             */
            bool eraseSessionByChannel ( const Poco::SharedPtr<CrlChannel>& );


            /**
             * @brief Helper method to remove session from the list.
             */
            bool eraseWorkersBySession ( const Poco::SharedPtr<CrlSessionServer>&  );



            /**
             * @brief List of Channels that this manager will handle.
             */
            Poco::FastMutex m_mxChannels;
            std::set< Poco::SharedPtr<CrlChannel> > m_channels;

            /**
             * @brief List of Sessions that this manager will handle.
             */
            Poco::FastMutex m_mxSessions;
            std::set< Poco::SharedPtr<CrlSessionServer> > m_sessions;

            /**
             * @brief List of Workers that this manager will handle.
             */
            Poco::FastMutex m_mxWorkers;
            std::set< Poco::SharedPtr<AbsCrlWorker> > m_workers;

            /**
             * @brief Internal Worker to do some administrator tasks.
             */
            Poco::FastMutex m_mxCrlManagerWorker;
            CrlManagerWorker m_crlManagerWorker;

            /**
             * @brief Channel Server where this manager will receive new connections.
             */
            gvr::communication::StreamChannelServer m_channelServer;

            /**
             * @brief Internal queue that communicate internal threads.
             */
            Poco::NotificationQueue m_notificationQueue;

            /**
             * @brief UccSession to be used by this crlManager.
             */
            UccSession  * m_pUccSession;

            /**
             * @brief SpotSession to be used by this crlManager.
             */
            SpotSession * m_pSpotSession;

            /**
             * @brief CloudStatus to be used by this crlManager.
             */
            CloudStatus * m_pCloudStatus;

            CloudConfigDeviceChecker * m_pConfigChecker;
        };
    }
  }

#endif
