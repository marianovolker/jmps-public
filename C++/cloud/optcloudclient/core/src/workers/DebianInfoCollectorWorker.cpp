/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file DebianInfoCollectorWorker.cpp
 * @author gparis
 * @date Tuesday, April 07, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <workers/DebianInfoCollectorWorker.h>
#include <gvr/communication/spot/SpotMessageDebianInfoResponse.h>
#include <gvrlog/log.h>

DEFINE_LOG_MODULE(worker);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    // Begin public methods and data members ...
    // -----------------------------------------------------------------------------------------------------------------

    DebianInfoCollectorWorker::DebianInfoCollectorWorker( SpotSession & spotSession ):
        AbsOptCloudClientWorker("DebianInfoCollectorWorker"),
        m_mtxblWorkDone(), m_blWorkDone(false),
        m_mtxblError(), m_blError(false),
        m_mtxdebianInfoStream(), m_debianInfoStream(),
        m_spotSession(spotSession)
    {
      start();

      LOG_Notice(worker, "Starting Debian Info Collector Worker...");
    }

    DebianInfoCollectorWorker::~DebianInfoCollectorWorker()
    {
      stop();

      LOG_Notice(worker, "Stopping Debian Info Collector Worker...");
    }

    bool DebianInfoCollectorWorker::collectInformation( std::string& sDeblabel, DebianInfoCollectorWorker::TextInfoMap& mapDebInfo )
    {
      if( m_spotSession.getDevicePlatform() == SpotSession::eM5 )
      {
        {
          ScopedLock lck(m_mtxdebianInfoStream);
          m_debianInfoStream.clear();
        }

        SpotMessageDebianInfo msgDebRequest;

        m_spotSession.send(msgDebRequest);
        if( m_spotSession.getError() != SpotSession::SpotSessionError::CNoError )
        {
          LOG_VCritical(worker, "SpotSession Error: [%s], sending a message.", m_spotSession.getErrorDescription().c_str());
          return false;
        }

        // Wait until the debian info worker finish its work ...
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        const SpotSession::TimeMillisecondsType CDefaultTimeOut = 20000; // twenty seconds ...
        Poco::Timestamp initialTime;
        while( (!isWorkDone()) && (Poco::Timestamp().epochTime() - initialTime.epochTime()) < (CDefaultTimeOut/1000) )
        {
          Poco::Thread::sleep(100);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////

        if( isWorkDone() )
        {
          fillFieldsWithInformationCollected(sDeblabel, mapDebInfo);
          return true;
        }
        else
        {
          LOG_Error(worker, "Error waiting debian packages info list.");
          sDeblabel = "firmware-debians";
        }
      }
      else // not M5 platform.
      {
        LOG_Notice(worker, "Not M5 platform.\n");
        sDeblabel = "firmware-debians";

        return true;
      }

      return false;
    }
    // -----------------------------------------------------------------------------------------------------------------






    // Begin protected methods and data members ...
    // -----------------------------------------------------------------------------------------------------------------

    /**
     * @brief Pattern to find "line 0", the first line of the debian list obtained from the command dpkg -l.
     */
    const char * DebianInfoCollectorWorker::CdpkgLine0 = "+++-===";

    /**
     * @brief This is to filter non-printable chars from a string.
     */
    struct InvalidChar
    {
        bool operator()(char c) const
        {
            return !isprint((unsigned)c);
        }
    };



    bool DebianInfoCollectorWorker::isWorkDone() const
    {
      ScopedLock lck(const_cast<DebianInfoCollectorWorker*>(this)->m_mtxblWorkDone);
      return m_blWorkDone;
    }

    bool DebianInfoCollectorWorker::hasError() const
    {
      ScopedLock lck(const_cast<DebianInfoCollectorWorker*>(this)->m_mtxblError);
      return m_blError;
    }

    bool DebianInfoCollectorWorker::fillFieldsWithInformationCollected(std::string& sLabel, TextInfoMap& map)
    {
      ScopedLock lck(m_mtxdebianInfoStream);

      bool blret = false;
      std::string sLine, sField1, sField2, sField3;

      // Here we find the first valid line that corresponding with the list of packages obtained with
      // "dpkg -l" command ...
      // ------
      do
      {
        std::getline(m_debianInfoStream, sLine);
      }
      while( sLine.find(CdpkgLine0) == std::string::npos );
      // ------

      while( std::getline(m_debianInfoStream, sLine) )
      {
        std::stringstream iss(sLine);

        // Must read at least the first 3 text columns.
        iss >> sField1 >> sField2 >> sField3;

        if( ! iss.fail() )
        {
          sField2.erase( std::remove_if( sField2.begin(), sField2.end(), InvalidChar() ), sField2.end() );
          sField3.erase( std::remove_if( sField3.begin(), sField3.end(), InvalidChar() ), sField3.end() );

          map.insert(make_pair(sField2 , sField3));

          blret = true;
        }
        else
        {
          LOG_VError(worker, "Discarding line from debian packages list: [ [field1=[%s]], [field2=[%s]], [field3=[%s]] ]\n", sField1.c_str(), sField2.c_str(), sField3.c_str());
        }
      }

      if( blret ) sLabel = "firmware-debians";

      return blret;
    }

    void DebianInfoCollectorWorker::sessionEvent( ISpotSessionListener::BufferType const & buffer, ISpotSessionListener::TypeId const & typeId, ISpotSessionListener::KeyType const & keyType)
    {
      LOG_Trace(worker, "Enqueue a new SPOT message notification.");

      enqueueNotification ( typeId, buffer );
    }

    void DebianInfoCollectorWorker::sessionEvent( ISpotSessionListener::StatusType status )
    {
      LOG_Trace(worker, "Enqueue a new SPOT session status notification.");

      enqueueNotification ( status );
    }

    void DebianInfoCollectorWorker::doSubscriptions( )
    {
      // Here we need to subscribe to a specific debian info reply event reset that is received from the SPOT session.
      LOG_Debug(worker, "Subscribing to Debian Info reply messages.");

      m_spotSession.subscribe(*this, SpotMessageDebianInfoResponse::GetTypeId());
    }

    void DebianInfoCollectorWorker::doUnsubscriptions( )
    {
      // Here we need to un-subscribe to all events.
      LOG_Debug(worker, "Un-subscribing to Debian Info reply messages.");

      m_spotSession.unsubscribe(*this);
    }

    void DebianInfoCollectorWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(worker, "Processing event message type notification.");

      if( SpotMessageDebianInfoResponse::GetTypeId() == tId )
      {
        DoDebianPkgInfoCollection(data);
      }
      else
      {
        LOG_Warn(worker, "Message received has unexpected type ID. Discarded.");
      }
    }

    void DebianInfoCollectorWorker::processNotification( StatusType status )
    {
      LOG_VNotice(worker, "New status event has been received from SpotSession [%s].\n", m_spotSession.getStatusDescription().c_str());
    }

    void DebianInfoCollectorWorker::DoDebianPkgInfoCollection ( const AbsOptCloudClientWorker::BufferType& msgData )
    {
      SpotMessageDebianInfoResponse msgDebInfoReply;

      if( ! msgDebInfoReply.parse(msgData) )
      {
        ScopedLock lck1(m_mtxblError);
        ScopedLock lck2(m_mtxblWorkDone);

        LOG_Error(worker, "A debian info message could not be parsed. Data discarded.");
        m_blError = true;
        m_blWorkDone = true;
        return;
      }

      {
        ScopedLock lck(m_mtxdebianInfoStream);

        std::string sData = msgDebInfoReply.getInfoResponseData();
        m_debianInfoStream << sData;
        LOG_VDebug( worker, "DebianInfoCollectorWorker: ACK Code: [0x%x]\n", msgDebInfoReply.getAckCode() );
      }

      if( msgDebInfoReply.getAckCode() == 0x00 || msgDebInfoReply.getAckCode() == 0x02 )
      {
        ScopedLock lck(m_mtxblWorkDone);
        m_blWorkDone = true;
      }
    }
    // -----------------------------------------------------------------------------------------------------------------

  }
}
