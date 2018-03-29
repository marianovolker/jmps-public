/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudStatus.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CLOUDSTATUS_H_
# define _CLOUDSTATUS_H_

# include <string>
# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>
# include <Poco/Util/IniFileConfiguration.h>
# include <Poco/AutoPtr.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      /**
       * @brief This class is the holder for the cloud connection and registration status.
       * @since 0.0.0, 2015-03-02
       * @version 0.0.0, 2015-03-02
       * @author pcantarini
       */
      class CloudStatus
      {
      public:
        static char const * CDefaultDeviceId;               /** @brief default device id which is the unregistered device id "0". */
        static char const * CDefaultLocalId;                /** @brief default local id which is the unregistered local id "0". */
        static char const * CUnregisteredDeviceId;        /** @brief unregistered device id which is "0". */
        static char const * CUnregisteredLocalId;         /** @brief unregistered local id which is "0". */
        static char const * CRegistrationErrorCodeNoError; /** @brief default registration error code when there is no error, which is "0". */
        static char const * CConnectionErrorCodeNoError;   /** @brief default connection error code when there is no error, which is "0". */

        /**
         * @brief default destructor.
         */
        virtual ~CloudStatus (void);

        /**
         * @brief Constructor.
         */
        CloudStatus (Poco::AutoPtr<Poco::Util::IniFileConfiguration>);

        /**
         * @brief Get the current device id.
         *
         * @return the current device id if it is registered. If not registered returns default device id, which is "0".
         */
        std::string getDeviceId  (void) const;

        /**
         * @brief Get the current local id.
         *
         * @return the current local id if it is registered. If not registered returns default local id, which is "0".
         */
        std::string getLocalId  (void) const;

        /**
         * @brief Check if registered to cloud.
         *
         * @return true if device is registered, false otherwise.
         */
        bool        isRegistered (void) const;

        /**
         * @brief Check if connected to cloud.
         *
         * @return true if device is connected to the cloud, false otherwise.
         */
        bool        isConnected  (void) const;

        /**
         * @brief Check if cloud is operative.
         *
         * @return true if the cloud is operative, false otherwise.
         */
        bool        isOperative  (void) const;

        /**
         * @brief Gets latest registration error information.
         *
         * @param errorCode is the latest registration error code.
         * @param errorMessage is the latest registration error message.
         */
        void getRegistrationError (std::string &, std::string &) const;

        /**
         * @brief Gets latest connection error information.
         *
         * @param errorCode is the latest connection error code.
         * @param errorMessage is the latest connection error message.
         */
        void getConnectionError (std::string &, std::string &) const;


        /**
         * @brief Gets the ini configuration file for PathRegistrationServerData.
         */
        void getPathRegistrationServerDataFile (std::string &) const;

        /**
         * @brief Gets the ini configuration file for PathCloudServerData.
         */
        void getPathCloudServerDataFile (std::string &) const;

        /**
         * @brief Gets the ini configuration file for PathNewCloudServerData.
         */
        void getPathNewCloudServerDataFile (std::string & pathNewCloudServerDataFile) const;

        /**
         * @brief Gets the ini dns configuration file for PathDeviceDnsConfiguration
         */
        void getPathDeviceDnsConfiguration (std::string & pathDeviceDnsConfigurationFile) const;

        /**
         * @brief Sets the current registered device id.
         *
         * @param new registered device id.
         */
        void setRegisteredDeviceId (std::string const &);

        /**
         * @brief Sets the current registered device id.
         *
         * @param new registered device id.
         */
        void setRegisteredDeviceId (std::string const &, std::string const &);

        /**
         * @brief Unsets the current registered device id.
         */
        void setUnregistered (void);

        /**
         * @brief Unsets the current registered device id.
         *
         * @param errorCode is the registration error code for the failed registration.
         * @param errorMessage is the registration error message for the failed registration.
         */
        void setUnregistered (std::string const &, std::string const &);

        /**
         * @brief Sets the connection status to connected.
         */
        void setConnected (void);

        /**
         * @brief Sets the connection status to disconnected.
         */
        void setDisconnected (void);

        /**
         * @brief Sets the connection status to disconnected.
         *
         * @param errorCode is the connection error code for the failed connection.
         * @param errorMessage is the connection error message for the failed connection.
         */
        void setDisconnected (std::string const &, std::string const &);


        /**
         * @brief Sets the connection status to connected or disconnected and returns the previous state atomically.
         *
         * @param connected is the connection new state. Default is true.
         *
         * @return The previous connection state.
         */
        bool testAndSetConnected (bool const &);

        /**
         * @brief Sets the connection status to disconnected and returns the previous state atomically.
         *
         * @param errorCode is the connection error code for the failed connection.
         * @param errorMessage is the connection error message for the failed connection.
         *
         * @return The previous connection state.
         */
        bool testAndSetDisconnected (std::string const &, std::string const &);

        /**
         * @brief Sets the cloud operative status.
         */
        void setOperative (bool);

        /**
         * @brief Gets Dns configuration local data.
         * @param dns name server1 .
         * @param dns name server2.
         */
        void getDnsLocalConfiguration(std::string & dnsServerName1, std::string & dnsServerName2) const;

        /**
         * @brief Gets the reconnection delay value.
         * @param the obtained value .
         * */
        void getReconnectionDelayValue (int & reConnectionDelayValue) const;

      protected:
        static char const * CEmptyString;                             /** @brief default empty string which is "". */
        static char const * CPathDeviceRegisterInfoFieldName;         /** @brief path for device register info field name. */
        static char const * CPathDeviceRegisterInfoFieldValueDefault; /** @brief path for device register info default value. */
        static char const * CDeviceIdFieldName;                       /** @brief device id field name. */
        static char const * CLocalIdFieldName;                       /** @brief device id field name. */

        /**@brief the default reconnection delay field value */
        static char const * CDefaultReconnectionDelayValue;

        /**
         * @brief Saves the registered device id.
         *
         * @param deviceId is the variable where the information is going to be stored.
         */
        void private_unsafeSaveDeviceId (std::string const &, std::string const &) const;

        /**
         * @brief Loads the registered device id.
         *
         * @param deviceId is the variable from where the information is being taken.
         */
        void private_unsafeLoadDeviceId (std::string &, std::string &) const;

        std::string m_deviceId;                 /** @brief Current registered device id. "0" if unregistered. */
        std::string m_localId;                  /** @brief Current registered local id. "0" if unregistered. */
        std::string m_RegistrationErrorCode;    /** @brief Current registration error code. "0" if no error. */
        std::string m_RegistrationErrorMessage; /** @brief Current registration error message. "0" if no relevant. */

        bool        m_connected;              /** @brief Current connection status. False if disconnected. */
        std::string m_ConnectionErrorCode;    /** @brief Current connection error code. "0" if no error. */
        std::string m_ConnectionErrorMessage; /** @brief Current connection error message. "0" if no relevant. */

        bool        m_operative;              /** @brief Current cloud operative status. False if is not operative. */

        typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;
        Poco::FastMutex m_mtx; /** @brief Current instance synchronization mutex. */

        Poco::AutoPtr<Poco::Util::IniFileConfiguration> m_initConfigurationFile;
      };
    }
  }

#endif
