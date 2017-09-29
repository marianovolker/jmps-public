/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IChannelServer.cpp
 * @author mvolker
 * @date Wednesday, April 15, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <gvr/communication/IChannelServer.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;

    // Initial values for states
    /////////////////////////////////////////////////////////////////////////////////////
    StatusType const IChannelServer::IChannelServerStatus::CStatusInitial       = 0;
    StatusType const IChannelServer::IChannelServerStatus::CStatusStarted       = 1;
    StatusType const IChannelServer::IChannelServerStatus::CStatusStopped       = 2;
    StatusType const IChannelServer::IChannelServerStatus::CLastStatus          = 10;
    /////////////////////////////////////////////////////////////////////////////////////

    // Initial values for errors
    /////////////////////////////////////////////////////////////////////////////////////
    ErrorType const IChannelServer::IChannelServerError::CNoError               = 0;
    ErrorType const IChannelServer::IChannelServerError::CErrorListening        = 1;
    ErrorType const IChannelServer::IChannelServerError::CErrorAccepting        = 2;
    ErrorType const IChannelServer::IChannelServerError::CErrorAlreadyStarted   = 3;
    ErrorType const IChannelServer::IChannelServerError::CLastError             = 10;
    /////////////////////////////////////////////////////////////////////////////////////




    IChannelServer::IChannelServer (void):
        m_status(IChannelServer::IChannelServerStatus::CStatusInitial),
        m_error(IChannelServer::IChannelServerError::CNoError),
        m_descriptionStatusList(), m_descriptionErrorList(),
        m_mxStatus(), m_mxError()
    {
      m_descriptionStatusList [ IChannelServer::IChannelServerStatus::CStatusInitial      ] = "Channel Server is at initial state";
      m_descriptionStatusList [ IChannelServer::IChannelServerStatus::CStatusStarted      ] = "Channel Server is started state";
      m_descriptionStatusList [ IChannelServer::IChannelServerStatus::CStatusStopped      ] = "Channel Server is stopped state";
      m_descriptionStatusList [ IChannelServer::IChannelServerStatus::CLastStatus         ] = "Channel Server is at an unknown status";


      m_descriptionErrorList  [ IChannelServer::IChannelServerError::CNoError             ] = "NO ERROR";
      m_descriptionErrorList  [ IChannelServer::IChannelServerError::CErrorListening      ] = "Channel Server had an error performing a listening";
      m_descriptionErrorList  [ IChannelServer::IChannelServerError::CErrorAccepting      ] = "Channel Server had an error accepting a new connection";
      m_descriptionErrorList  [ IChannelServer::IChannelServerError::CErrorAlreadyStarted ] = "Channel Server has been started, you need to stop it first";
      m_descriptionErrorList  [ IChannelServer::IChannelServerError::CLastError           ] = "Channel Server has an unknown error";
    }

    IChannelServer::~IChannelServer (void)
    {
    }

    StatusType IChannelServer::getStatus()
    {
      ScopedLock lock(m_mxStatus);

      return m_status;
    }

    ErrorType IChannelServer::getError()
    {
      ScopedLock lock(m_mxError);

      return m_error;
    }

    StatusType IChannelServer::setStatus(StatusType status)
    {
      ScopedLock lock(m_mxStatus);

      return (m_status=status);
    }

    ErrorType IChannelServer::setError(ErrorType error)
    {
      ScopedLock lock(m_mxError);

      return (m_error=error);
    }

    std::string IChannelServer::getStatusDescription()
    {
      ScopedLock lock(m_mxStatus);

      descriptionStatusType::const_iterator it = m_descriptionStatusList.find(m_status);
      return (it == m_descriptionStatusList.end())?(m_descriptionStatusList[IChannelServer::IChannelServerStatus::CLastStatus]):(it->second);
    }

    std::string IChannelServer::getStatusDescription(StatusType status)
    {
      ScopedLock lock(m_mxStatus);

      descriptionStatusType::const_iterator it = m_descriptionStatusList.find(status);
      return (it == m_descriptionStatusList.end())?(m_descriptionStatusList[IChannelServer::IChannelServerStatus::CLastStatus]):(it->second);
    }

    std::string IChannelServer::getErrorDescription()
    {
      ScopedLock lock(m_mxError);

      descriptionErrorType::const_iterator it = m_descriptionErrorList.find(m_error);
      return (it == m_descriptionErrorList.end())?(m_descriptionErrorList[IChannelServer::IChannelServerError::CLastError]):(it->second);
    }

    std::string IChannelServer::getErrorDescription(ErrorType error)
    {
      ScopedLock lock(m_mxError);

      descriptionErrorType::const_iterator it = m_descriptionErrorList.find(error);
      return (it == m_descriptionErrorList.end())?(m_descriptionErrorList[IChannelServer::IChannelServerError::CLastError]):(it->second);
    }

  };
}
