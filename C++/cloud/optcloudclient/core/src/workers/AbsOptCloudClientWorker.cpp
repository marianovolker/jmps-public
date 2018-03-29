/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsOptCloudClientWorker.cpp
 * @author mvolker
 * @date Monday, March 30, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <workers/AbsOptCloudClientWorker.h>

#include <gvrlog/log.h>

DECLARE_LOG_MODULE(core);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    AbsOptCloudClientWorker::AbsOptCloudClientWorker( const std::string& threadName ):
        m_thread(threadName),
        m_readyStarted(), m_readyStopped(),
        m_mxRun(),
        m_notificationQueue(),
        m_run(false)
    {
    }

    AbsOptCloudClientWorker::~AbsOptCloudClientWorker()
    {
    }

    void AbsOptCloudClientWorker::enqueueNotification ( TypeId typeId, const BufferType& data )
    {
      LOG_VDebug(core, "Enqueue a new message notification inside internal [%s] notification queue...\n", m_thread.getName().c_str());

      m_notificationQueue.enqueueNotification(new WorkerNotification(typeId, data));
    }

    void AbsOptCloudClientWorker::enqueueNotification ( StatusType status )
    {
      LOG_VDebug(core, "Enqueue a new status notification inside internal [%s] notification queue...\n", m_thread.getName().c_str());

      m_notificationQueue.enqueueNotification(new WorkerNotification(status));
    }


    void AbsOptCloudClientWorker::run()
    {
      // Wait for the signal to begin the thread ...
      m_readyStarted.wait();

      LOG_VDebug(core, "The internal worker thread [%s] is running now...\n", m_thread.getName().c_str());

      doSubscriptions();

      while( isRunning() )
      {
        WorkerNotification * pNotification(dynamic_cast<WorkerNotification *>( m_notificationQueue.waitDequeueNotification() ));

        if( pNotification != 0 )
        {
          processNotification(pNotification);
        }
      }

      doUnsubscriptions();

      // Signal that the thread has finished...
      m_readyStopped.set();

      LOG_VDebug(core, "The internal worker thread [%s] is stopping now...\n", m_thread.getName().c_str());
    }

    bool AbsOptCloudClientWorker::start()
    {
      if( !isRunning() )
      {
        try
        {
          turnOn();

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

    bool AbsOptCloudClientWorker::stop()
    {
      if( isRunning() )
      {
        try
        {
          turnOff();

          // Wake up the main thread of this worker...
          m_notificationQueue.wakeUpAll();

          // Wait for the signal to terminate the thread ...
          m_readyStopped.wait();

          m_thread.join();
        }
        catch(...)
        {
          return false;
        }
      }

      return true;
    }

    bool AbsOptCloudClientWorker::isRunning()
    {
      ScopedLock lock(m_mxRun);
      return m_run;
    }

    bool AbsOptCloudClientWorker::turnOff()
    {
      ScopedLock lock(m_mxRun);
      return (m_run=false);
    }

    bool AbsOptCloudClientWorker::turnOn()
    {
      ScopedLock lock(m_mxRun);
      return (m_run=true);
    }

    void AbsOptCloudClientWorker::processNotification ( WorkerNotification* pNotification )
    {
      if( pNotification != 0 && pNotification->isDataEvent() )
      {
        processNotification(pNotification->getTypeId(), pNotification->getBufferData());
      }

      if( pNotification != 0 && pNotification->isStatusEvent() )
      {
        processNotification(pNotification->getStatus());
      }

      if( pNotification != 0 )
      {
        delete pNotification;
      }
    }
  }
}
