/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SystemStatusNotificationWorker.cpp
 * @author mvolker
 * @date Tuesday, May 19, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <map>
# include <workers/SystemStatusNotificationWorker.h>
# include <gvr/communication/spot/SpotMessageSystemStatus.h>
# include <gvr/communication/spot/SpotMessageSystemStatusResponse.h>

# include <gvrlog/log.h>
# include <gvr/communication/spot/Utils.h>

DECLARE_LOG_MODULE(core);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {


    SystemStatusNotificationWorker::SystemStatusNotificationWorker( SpotSession & spotSession ):
        AbsOptCloudClientWorker("SystemStatusNotificationWorker"),
        m_spotSession(spotSession)
    {
      start();

      LOG_Notice(core, "Starting SystemStatusNotification Worker...");
      SpotMessageSystemStatus sysStatusRequest;
      sysStatusRequest.setSubSystemStatusType();
      spotSession.send(sysStatusRequest);
    }

    SystemStatusNotificationWorker::~SystemStatusNotificationWorker()
    {
      stop();

      LOG_Notice(core, "Stopping SystemStatusNotification Worker...");
    }

    void SystemStatusNotificationWorker::sessionEvent( ISpotSessionListener::BufferType const & buffer, ISpotSessionListener::TypeId const & typeId, ISpotSessionListener::KeyType const & keyType)
    {
      LOG_Trace(core, "Enqueue a new SPOT message notification.");

      enqueueNotification ( typeId, buffer );
    }

    void SystemStatusNotificationWorker::sessionEvent( ISpotSessionListener::StatusType status )
    {
      LOG_Trace(core, "Enqueue a new SPOT session status notification.");

      enqueueNotification ( status );
    }

    void SystemStatusNotificationWorker::doSubscriptions( )
    {
      LOG_Debug(core, "Subscribing to SystemStatusNotification reply messages.");

      m_spotSession.subscribe(*this, SpotMessageSystemStatusResponse::GetTypeId());
    }

    void SystemStatusNotificationWorker::doUnsubscriptions( )
    {
      // Here we need to un-subscribe to all events.
      LOG_Debug(core, "Un-subscribing to SystemStatusNotification Info reply messages.");

      m_spotSession.unsubscribe(*this);
    }

    void SystemStatusNotificationWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      if( SpotMessageSystemStatusResponse::GetTypeId() != tId )
      {
        LOG_Warn(core, "Message received has unexpected type ID in this worker. Discarded.");
        return;
      }

      LOG_Debug(core, "A message SpotMessageSystemStatusResponse has been received.");

      // TODO: remove these lines to avoid the dump of the buffer to log...
      char pcBuffer[(data.size()*3)+2];
      LOG_VDebug(core, "System status notification [%s].\n", Utils::dumpBuffer(pcBuffer, &data.front(), data.size()));

      SpotMessageSystemStatusResponse sysStatusReply;
      if( ! sysStatusReply.parse(data) )
      {
        LOG_Notice(core, "Could not parse incoming system status notification.");
        return;
      }

      std::string sDeviceName;
      DeviceStatusMap  map, attribMap;

      if( sysStatusReply.getDisplayInfo(sDeviceName, map, attribMap) )
      {
        logDeviceStatusInfo(sDeviceName, map, attribMap);
      }

      map.clear();
      attribMap.clear();

      if( sysStatusReply.getKeypadInfo(sDeviceName, map, attribMap) )
      {
        logDeviceStatusInfo(sDeviceName, map, attribMap);
      }

      map.clear();
      attribMap.clear();

      if( sysStatusReply.getCardReaderInfo(sDeviceName, map, attribMap) )
      {
        logDeviceStatusInfo(sDeviceName, map, attribMap);
      }

      map.clear();
      attribMap.clear();

      if( sysStatusReply.getPrinterInfo(sDeviceName, map, attribMap) )
      {
        logDeviceStatusInfo(sDeviceName, map, attribMap);
      }
    }

    void SystemStatusNotificationWorker::processNotification( StatusType status )
    {
      LOG_VNotice(core, "New status event has been received from SpotSession [%s].\n", m_spotSession.getStatusDescription().c_str());
    }

    void SystemStatusNotificationWorker::logDeviceStatusInfo
    (
      const std::string & sDeviceName,
      const DeviceStatusMap & map,
      const DeviceStatusMap & attribMap ) const
    {
      DeviceStatusMapConstIterator it, itend;

      it  = map.begin();
      itend = map.end();
      LOG_VDebug(core, "\n\nDevice: %s\n", sDeviceName.c_str());
      while(it != itend)
      {
        LOG_VDebug(core, "\"%s\" : \"%s\"\n", it->first.c_str(), it->second.c_str());
        ++it;
      }

      LOG_Debug(core, "Attributes:");

      it  = attribMap.begin();
      itend = attribMap.end();
      while(it != itend)
      {
        LOG_VDebug(core, "\"%s\" : \"%s\"", it->first.c_str(), it->second.c_str());
        ++it;
      }
    }


  }
}
