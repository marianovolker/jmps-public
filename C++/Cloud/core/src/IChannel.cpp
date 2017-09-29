/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IChannel.cpp
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <gvr/communication/IChannel.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;

    // Initial values for states
    /////////////////////////////////////////////////////////////////////////////////////
    StatusType const IChannel::IChannelStatus::CStatusInitial       = 0;
    StatusType const IChannel::IChannelStatus::CStatusDisconnected  = 1;
    StatusType const IChannel::IChannelStatus::CStatusConnected     = 2;
    StatusType const IChannel::IChannelStatus::CLastStatus          = 10;
    /////////////////////////////////////////////////////////////////////////////////////

    // Initial values for errors
    /////////////////////////////////////////////////////////////////////////////////////
    ErrorType const IChannel::IChannelError::CNoError               = 0;
    ErrorType const IChannel::IChannelError::CErrorConnecting       = 1;
    ErrorType const IChannel::IChannelError::CErrorDisconnecting    = 2;
    ErrorType const IChannel::IChannelError::CErrorAlreadyConnected = 3;
    ErrorType const IChannel::IChannelError::CErrorSending          = 4;
    ErrorType const IChannel::IChannelError::CErrorReceiving        = 5;
    ErrorType const IChannel::IChannelError::CLastError             = 10;
    /////////////////////////////////////////////////////////////////////////////////////




    IChannel::IChannel (void):
        m_status(IChannel::IChannelStatus::CStatusInitial),
        m_error(IChannel::IChannelError::CNoError),
        m_descriptionStatusList(), m_descriptionErrorList(),
        m_bufferSize(1024), m_mxStatus(), m_mxError(), m_mxBufferSize()
    {
      m_descriptionStatusList [ IChannel::IChannelStatus::CStatusInitial        ] = "Channel is at initial state";
      m_descriptionStatusList [ IChannel::IChannelStatus::CStatusDisconnected   ] = "Channel is disconnected";
      m_descriptionStatusList [ IChannel::IChannelStatus::CStatusConnected      ] = "Channel is connected";
      m_descriptionStatusList [ IChannel::IChannelStatus::CLastStatus           ] = "Channel is at an unknown status";


      m_descriptionErrorList  [ IChannel::IChannelError::CNoError               ] = "NO ERROR";
      m_descriptionErrorList  [ IChannel::IChannelError::CErrorConnecting       ] = "Channel had an error performing a connection";
      m_descriptionErrorList  [ IChannel::IChannelError::CErrorDisconnecting    ] = "Channel had an error performing a disconnection";
      m_descriptionErrorList  [ IChannel::IChannelError::CErrorAlreadyConnected ] = "Channel connection request on an already connected link";
      m_descriptionErrorList  [ IChannel::IChannelError::CErrorSending          ] = "Channel had an error writing to the link";
      m_descriptionErrorList  [ IChannel::IChannelError::CErrorReceiving        ] = "Channel had an error reading from the link";
      m_descriptionErrorList  [ IChannel::IChannelError::CLastError             ] = "Channel has an unknown error";
    }

    IChannel::~IChannel (void)
    {
    }

    StatusType IChannel::getStatus()
    {
      ScopedLock lock(m_mxStatus);

      return m_status;
    }

    ErrorType IChannel::getError()
    {
      ScopedLock lock(m_mxError);

      return m_error;
    }

    StatusType IChannel::setStatus(StatusType status)
    {
      ScopedLock lock(m_mxStatus);

      return (m_status=status);
    }

    ErrorType IChannel::setError(ErrorType error)
    {
      ScopedLock lock(m_mxError);

      return (m_error=error);
    }

    std::string IChannel::getStatusDescription()
    {
      ScopedLock lock(m_mxStatus);

      descriptionStatusType::const_iterator it = m_descriptionStatusList.find(m_status);
      return (it == m_descriptionStatusList.end())?(m_descriptionStatusList[IChannel::IChannelStatus::CLastStatus]):(it->second);
    }

    std::string IChannel::getStatusDescription(StatusType status)
    {
      ScopedLock lock(m_mxStatus);

      descriptionStatusType::const_iterator it = m_descriptionStatusList.find(status);
      return (it == m_descriptionStatusList.end())?(m_descriptionStatusList[IChannel::IChannelStatus::CLastStatus]):(it->second);
    }

    std::string IChannel::getErrorDescription()
    {
      ScopedLock lock(m_mxError);

      descriptionErrorType::const_iterator it = m_descriptionErrorList.find(m_error);
      return (it == m_descriptionErrorList.end())?(m_descriptionErrorList[IChannel::IChannelError::CLastError]):(it->second);
    }

    std::string IChannel::getErrorDescription(ErrorType error)
    {
      ScopedLock lock(m_mxError);

      descriptionErrorType::const_iterator it = m_descriptionErrorList.find(error);
      return (it == m_descriptionErrorList.end())?(m_descriptionErrorList[IChannel::IChannelError::CLastError]):(it->second);
    }

    unsigned short IChannel::getBufferSize()
    {
      ScopedLock lock(m_mxBufferSize);

      return m_bufferSize;
    }

    void IChannel::setBufferSize(unsigned short bufferSize)
    {
      ScopedLock lock(m_mxBufferSize);

      m_bufferSize = bufferSize;
    }

  };
}
