/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlManager.cpp
 * @author mvolker
 * @date Tuesday, April 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <CrlManager.h>

# include <Poco/Timespan.h>

# include <gvr/communication/crl/session/CrlSessionServer.h>
# include <workers/CloudRegistrationWorker.h>
# include <workers/CloudRegistrationStatusWorker.h>
# include <workers/CloudDnsConfigurationWorker.h>

# include <gvrlog/log.h>

using namespace gvr::communication::ucc;
using namespace gvr::communication::spot;
using namespace gvr::communication::crl;

DECLARE_LOG_MODULE(core);


/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Cloud Domain
  namespace cloud
  {
      /* Public Interface ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      CrlManager::CrlManager(const std::string& host, int port, UccSession &uccSession, SpotSession &spotSession, CloudStatus & cloudStatus, CloudConfigDeviceChecker & configChecker):
        m_channelServer(host, port),
        m_channels(),
        m_sessions(),
        m_workers(),
        m_crlManagerWorker(),
        m_mxChannels(),
        m_mxSessions(),
        m_mxWorkers(),
        m_mxCrlManagerWorker(),
        m_pUccSession(&uccSession),
        m_pSpotSession(&spotSession),
        m_pCloudStatus(&cloudStatus),
        m_pConfigChecker(&configChecker)
      {
        m_channelServer.subscribe(*this);

        startAdminWorker();

        LOG_Debug(core, "Starting CrlManager...");
      }

      CrlManager::~CrlManager (void)
      {
        terminateCleanly();

        stopAdminWorker();

        LOG_Debug(core, "Stopping CrlManager...");
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /* End Public Interface ********/




      /* Protected Interface ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      bool CrlManager::startAdminWorker ()
      {
        ScopedLock lock(m_mxCrlManagerWorker);

        if( !m_crlManagerWorker.start(*this) )
        {
          return false;
        }

        return true;
      }

      bool CrlManager::stopAdminWorker ()
      {
        ScopedLock lock(m_mxCrlManagerWorker);

        if( !m_crlManagerWorker.stop() )
        {
          return false;
        }

        return true;
      }

      void CrlManager::channelEvent( StatusType status )
      {
        if( status == CrlChannel::IChannelStatus::CStatusDisconnected )
        {
          LOG_Debug(core, "CrlManager - Has been received a disconnection from a channel...");

          m_notificationQueue.enqueueNotification(new WorkerNotification(status));
        }
        else
        {
          LOG_VInfo(core, "CrlManager - received a change of status of channel [%d] ...\n", status);
        }
      }

      void CrlManager::channelStartedEvent (const Poco::Net::StreamSocket& socket)
      {
        static const int CMaxConnectionsPermitted = 1;
        int nConnections = 0;

        {
          ScopedLock lockChannels(m_mxChannels);
          nConnections = (m_channels.size()+1);
        }

        LOG_VDebug(core, "CrlManager - Has been received a new connection in channel server, connection number [%d] of [%d] permitted ...", nConnections, CMaxConnectionsPermitted);

        if( nConnections <= CMaxConnectionsPermitted )
        {
          LOG_Debug(core, "CrlManager - New connection is received...");

          CrlChannel* pChannel(new CrlChannel(socket));

          pChannel->subscribe(*this);

          {
            ScopedLock lockChannels(m_mxChannels);
            Poco::SharedPtr<CrlChannel> qtrChannel(pChannel);
            m_channels.insert(qtrChannel);

            ScopedLock lockSessions(m_mxSessions);
            Poco::SharedPtr<CrlSessionServer> qtrSession(new CrlSessionServer());
            m_sessions.insert(qtrSession);

            ScopedLock lockWorkers(m_mxWorkers);
            {
              Poco::SharedPtr<gvr::cloud::AbsCrlWorker> qtrWorker(new gvr::cloud::CloudRegistrationWorker( (*m_pUccSession), (*m_pSpotSession), (*qtrSession), (*m_pCloudStatus), (*m_pConfigChecker) ));
              m_workers.insert(qtrWorker);
            }
            {
              Poco::SharedPtr<gvr::cloud::AbsCrlWorker> qtrWorker(new gvr::cloud::CloudRegistrationStatusWorker( (*qtrSession), (*m_pCloudStatus) ));
              m_workers.insert(qtrWorker);
            }
            {
              Poco::SharedPtr<gvr::cloud::AbsCrlWorker> qtrWorker(new gvr::cloud::CloudDnsConfigurationWorker( (*m_pUccSession), (*qtrSession), (*m_pCloudStatus) ));
              m_workers.insert(qtrWorker);
            }

            qtrSession->start( *pChannel );
          }

          LOG_Debug(core, "CrlManager - Session is started...");
        }
        else
        {
          LOG_Debug(core, "CrlManager - The connections limits has been reached...");
        }
      }

      void CrlManager::checkNotifications()
      {
        WorkerNotification * pNotification(dynamic_cast<WorkerNotification *>( m_notificationQueue.waitDequeueNotification(1000) ));

        if( pNotification != 0 )
        {
          processNotification(pNotification);
        }
      }

      void CrlManager::processNotification ( WorkerNotification* pNotification )
      {
        if( pNotification != 0 )
        {
          if( pNotification->getStatus() == CrlChannel::IChannelStatus::CStatusDisconnected )
          {
            removeObjectsByStatus(pNotification->getStatus());
          }

          delete pNotification;
        }
      }

      void CrlManager::terminateCleanly ( )
      {
        {
          ScopedLock lockChannels(m_mxChannels);

          Poco::SharedPtr<CrlChannel> qChannel;
          std::set< Poco::SharedPtr<CrlChannel> >::iterator it;
          for( it = m_channels.begin(); it != m_channels.end(); ++it )
          {
            qChannel = (*it);
            qChannel->unsubscribe(*this);
          }
        }

        {
          ScopedLock lockWorkers(m_mxWorkers);
          if( !m_workers.empty() )
          {
            m_workers.clear();
          }

          ScopedLock lockSessions(m_mxSessions);
          if( !m_sessions.empty() )
          {
            m_sessions.clear();
          }

          ScopedLock lockChannels(m_mxChannels);
          if( !m_channels.empty() )
          {
            m_channels.clear();
          }
        }
      }

      bool CrlManager::eraseWorkersBySession ( const Poco::SharedPtr<CrlSessionServer>& qSession )
      {
        ScopedLock lock(m_mxWorkers);

        bool found(false);

        Poco::SharedPtr<gvr::cloud::AbsCrlWorker> qWorker;

        std::set< Poco::SharedPtr<gvr::cloud::AbsCrlWorker> >::iterator itWorker;
        for( itWorker = m_workers.begin(); itWorker != m_workers.end(); )
        {
          qWorker = (*itWorker);

          if( qWorker->getSession() == qSession.get() )
          {
            LOG_Debug(core, "CrlManager - removing worker by session...");

            m_workers.erase(itWorker++);

            found=true;
          }
          else
          {
            ++itWorker;
          }
        }

        return found;
      }

      bool CrlManager::eraseSessionByChannel ( const Poco::SharedPtr<CrlChannel>& qChannel )
      {
        ScopedLock lock(m_mxSessions);

        Poco::SharedPtr<CrlSessionServer> qSession;

        std::set< Poco::SharedPtr<CrlSessionServer> >::iterator itSession;
        for( itSession = m_sessions.begin(); itSession != m_sessions.end(); )
        {
          qSession = (*itSession);

          if( (&qSession->getChannel()) == qChannel.get() )
          {
            eraseWorkersBySession(qSession);

            LOG_Debug(core, "CrlManager - removing Session...");

            qSession->stop();

            m_sessions.erase(itSession++);

            return true;
          }
          else
          {
            ++itSession;
          }
        }

        return false;
      }

      void CrlManager::removeObjectsByStatus( StatusType status )
      {
        ScopedLock lock(m_mxChannels);

        Poco::SharedPtr<CrlChannel> qChannel;

        std::set< Poco::SharedPtr<CrlChannel> >::iterator it;
        for( it = m_channels.begin(); it != m_channels.end(); )
        {
          qChannel = (*it);

          if( qChannel->getStatus() == status )
          {
            qChannel->unsubscribe(*this);

            eraseSessionByChannel(qChannel);

            LOG_Debug(core, "CrlManager - removing Channel...");

            m_channels.erase(it++);

            LOG_VDebug(core, "CrlManager - removing objects associated to this channel. Channels[%d], Sessions[%d], Workers[%d]...", m_channels.size(), m_sessions.size(), m_workers.size());
          }
          else
          {
            ++it;
          }
        }
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////




      /* Section of Thread CrlManager ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      CrlManager::CrlManagerWorker::CrlManagerWorker()
        : m_pCrlManager(NULL), m_run(false), m_thread("CrlManagerWorker"),
          m_readyStarted(), m_readyStopped(),
          m_mxCrlManagerWorker(), m_mxCrlManager(), m_mxRun()
      {
      }


      CrlManager::CrlManagerWorker::~CrlManagerWorker()
      {
        stop();
      }


      bool CrlManager::CrlManagerWorker::start(CrlManager& crlManager)
      {
        ScopedLock lock(m_mxCrlManagerWorker);

        if( !isRunning() )
        {
          try
          {
            turnOn();

            {
              ScopedLock lock(m_mxCrlManager);
              m_pCrlManager = &crlManager;
            }

            m_thread.start(*this);

            // Signal that the thread could begin...
            m_readyStarted.set();
          }
          catch(...)
          {
            return false;
          }
        }

        return true;
      }


      bool CrlManager::CrlManagerWorker::stop()
      {
        ScopedLock lock(m_mxCrlManagerWorker);

        if( isRunning() )
        {
          try
          {
            turnOff();

            {
              ScopedLock lock(m_mxCrlManager);
              m_pCrlManager = 0;
            }

            // Wait for the signal to terminate the thread ...
            m_readyStopped.wait();

            m_thread.join();
          }
          catch (...)
          {
            return false;
          }
        }

        return true;
      }


      void CrlManager::CrlManagerWorker::run()
      {
        // Wait for the signal to begin the thread ...
        m_readyStarted.wait();

        while( isRunning() )
        {
          {
            ScopedLock lock(m_mxCrlManager);
            if( m_pCrlManager )
            {
              m_pCrlManager->checkNotifications();
            }
          }
        }

        // Signal that the thread has finished...
        m_readyStopped.set();
      }


      bool CrlManager::CrlManagerWorker::isRunning()
      {
        ScopedLock lock(m_mxRun);
        return m_run;
      }


      bool CrlManager::CrlManagerWorker::turnOff()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=false);
      }


      bool CrlManager::CrlManagerWorker::turnOn()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=true);
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /* End Section of Thread CrlManager********/

  }
}
