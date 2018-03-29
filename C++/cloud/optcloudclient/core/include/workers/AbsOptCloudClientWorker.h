/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsOptCloudClientWorker.h
 * @author mvolker
 * @date Monday, March 30, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSOPTCLOUDCLIENTWORKER_H_
# define _ABSOPTCLOUDCLIENTWORKER_H_

# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>
# include <Poco/Thread.h>
# include <Poco/Runnable.h>
# include <Poco/Event.h>
# include <Poco/NotificationQueue.h>

# include <gvr/communication/IChannel.h>

using namespace gvr::communication;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class AbsOptCloudClientWorker : public Poco::Runnable
      {
        public:

          typedef IChannel::SizeType    SizeType, TypeId;
          typedef IChannel::BufferType  BufferType;


          /**
           * @brief default constructor.
           */
          AbsOptCloudClientWorker( const std::string & = "OptCloudClientWorker" );

          /**
           * @brief default destructor.
           */
          virtual ~AbsOptCloudClientWorker(void);

          /**
           * @brief This method is where the thread does the enqueue to each event data notification received.
           */
          virtual void enqueueNotification ( TypeId typeId, const BufferType& data );

          /**
           * @brief This method is where the thread does the enqueue to each notification received.
           */
          virtual void enqueueNotification ( StatusType status );

          /**
            * @brief This method is to start the worker thread.
            */
          bool start();

          /**
            * @brief This method is to stop the worker thread.
            */
          bool stop();


        private:

          typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;

          /**
            * @brief This method is to check if the thread is still running.
            */
          bool isRunning();

          /**
            * @brief This method is to turn on the internal flag that indicate the worker thread is on.
            */
          bool turnOn();

          /**
            * @brief This method is to turn off the internal flag that indicate the worker thread is off.
            */
          bool turnOff();

          /**
            * @brief This method is the main of the worker thread.
            */
          void run();

          Poco::FastMutex m_mxRun;
          bool m_run;

          Poco::Thread m_thread;
          Poco::Event m_readyStarted;
          Poco::Event m_readyStopped;

          Poco::NotificationQueue m_notificationQueue;

          /**
           * @brief This subclass is for internal communication between threads.
           */
          class WorkerNotification: public Poco::Notification
          {
            public:

              WorkerNotification(TypeId typeId, const BufferType& data):
                  m_typeId(typeId), m_data(data), m_status(0),
                  m_isStatusEvent(false), m_isDataEvent(true)
              {
              }

              WorkerNotification(StatusType statusType):
                  m_typeId(0), m_data(), m_status(statusType),
                  m_isStatusEvent(true), m_isDataEvent(false)
              {
              }

              virtual ~WorkerNotification()
              {
              }

              const BufferType& getBufferData() const
              {
                return m_data;
              }

              TypeId getTypeId() const
              {
                return m_typeId;
              }

              StatusType getStatus() const
              {
                return m_status;
              }

              bool isStatusEvent() const
              {
                return m_isStatusEvent;
              }

              bool isDataEvent() const
              {
                return m_isDataEvent;
              }

            private:
              BufferType m_data;
              TypeId m_typeId;
              StatusType m_status;
              bool m_isStatusEvent;
              bool m_isDataEvent;
          };

          /**
           * @brief This method is where the thread does the process to each notification received.
           *        Once notification is processed in this method is needed to release the pointer to notification.
           */
          void processNotification ( WorkerNotification* pNotification = 0 );



        protected:

          ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          // The following methods should be overwritten in the specific worker's class in where is needed to decide what can be
          // done with a new notification and to what subscription the workers need to be subscribed.
          ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

          /**
           * @brief This method is where the thread does the subscribe to events that will received then.
           */
          virtual void doSubscriptions () = 0;

          /**
           * @brief This method is where the thread does the unsubscribe to events that was previously subscribed.
           */
          virtual void doUnsubscriptions () = 0;

          /**
           * @brief This method is where the thread does the process an event data notification received.
           */
          virtual void processNotification ( TypeId typeId, const BufferType& data ) = 0;

          /**
           * @brief This method is where the thread does the process to each status event notification received.
           */
          virtual void processNotification ( StatusType statusType ) = 0;

          ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      };
    }
  }

#endif
