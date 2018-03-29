/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SystemStatusNotificationWorker.h
 * @author mvolker
 * @date Tuesday, May 19, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SYSTEMSTATUSNOTIFICATIONWORKER_H_
#define _SYSTEMSTATUSNOTIFICATIONWORKER_H_

#include <gvr/communication/spot/ISpotSessionListener.h>
#include <gvr/communication/spot/SpotSession.h>
#include <workers/AbsOptCloudClientWorker.h>


using namespace gvr::communication::spot;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class SystemStatusNotificationWorker : public gvr::cloud::AbsOptCloudClientWorker, public ISpotSessionListener
      {
        public:

          /**
           * @brief default constructor.
           */
          SystemStatusNotificationWorker( SpotSession & );

          /**
           * @brief default destructor.
           */
          virtual ~SystemStatusNotificationWorker(void);


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


        private:

          /**
           * @type   DeviceStatusMap
           * @brief  Return type when requesting all the status information about a specific device.
           * @see    SystemStatusNotificationWorker::logDeviceStatusInfo()
           */
          typedef std::map<std::string, std::string>                   DeviceStatusMap;
          typedef std::map<std::string, std::string>::const_iterator   DeviceStatusMapConstIterator;

          void logDeviceStatusInfo ( const std::string & sDeviceName,
                                     const DeviceStatusMap & map, const DeviceStatusMap & attrMap) const;

          SpotSession&  m_spotSession;
      };
    }
  }

#endif
