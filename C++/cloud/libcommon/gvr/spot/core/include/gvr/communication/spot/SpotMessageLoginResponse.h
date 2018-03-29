/*
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageLoginResponse.h
 * @author mgrieco
 * @date Friday, December 29, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGELOGINRESPONSE_H_
# define _SPOTMESSAGELOGINRESPONSE_H_

# include <gvr/communication/spot/ISpotMessage.h>
# include <vector>
# include <string>
# include <iostream>


/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      /**
       * @brief This class is the implementation for Login message response on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageLoginResponse : public ISpotMessage
      {
        public:
          /**@brief Spot Message Identification.*/
          static const unsigned char CSpotMessageId = 0x01;

          /**@brief no variable part size. (no tables elements, ack code no checked)*/
          static const int CMessageCommonSize = 6 ;

          /**@brief no variable part size. (no tables elements, ack code with no error)*/
          static const int CMessageOkCommonSize = 20 ;

          /**@brief default size value for date-time field*/
          static const int CDefaultDateTimeSize = 14;

          /**@brief default size value for application description field*/
          static const int CDefaultAppDescriptionSize = 40;

          /**@brief default size value for application sign field*/
          static const int CDefaultAppSignSize = 4;

          /**
           * @brief Following methods Retrieves the current instance Message class type identification.
           *
           * @return The current Message class type identification.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.TypeId
           *
           * @since 0.0.0
           */
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageLoginResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief This class represents each application entry on the login message response applications table.
           **/
          class AppEntry
          {
            private:
              /** @brief ClientId of available SPOT local application.*/
              unsigned char appId;

              /** @brief CRC32. It allows to verify the release of the local application on SPOT. For EMV application this CRC32 is submitted to EMVco during level2 certification.*/
              BufferType sign;

              /** @brief Application version in text format.*/
              std::string appAscii;

            public:

              /** @brief constant size for each entry.*/
              static const int CEntrySize = 45;

              /** @brief Default constructor **/
              AppEntry(void)
              {
                appId = 0;
              }

              /** @brief Default constructor with all fields values
               * @param appId the application id
               * @param sign the application sign
               * @param appAscii the application ASCII description**/
              AppEntry(unsigned char appId, BufferType sign, BufferType appAscii);

              /**
               * Retrieves the application ascii field
               * @return the application ascii field value
               *  */
              const std::string& getAppAscii() const
              {
                return appAscii;
              }

              /**
               * @brief Sets the application ascii field
               * @param appAscii the application ascii field value
               **/
              void setAppAscii(const std::string& appAscii)
              {
                this->appAscii = appAscii;
              }

              /**
               * @brief Retrieves the application id field
               * @return the application id field value
               **/
              unsigned char getAppId() const
              {
                return appId;
              }

              /**
               * @brief Sets the application id field
               * @param appAscii the application id field value
               **/
              void setAppId(const unsigned char appid)
              {
                this->appId = appid;
              }

              /**
               * @brief Retrieves the application sign field
               * @return the application sign field value
               **/
              BufferType getSign() const
              {
                return sign;
              }

              /**
               * @brief Sets the application sign field
               * @param appAscii the application sign field value
               **/
              void setSign(const BufferType& sign)
              {
                this->sign = sign;
              }
          };

          /**
           * @brief default constructor
           */
          SpotMessageLoginResponse(void);

          /**
           * Constructor giving the acknowledge value.
           * @param the acknowledge value for the Login Response.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          SpotMessageLoginResponse(unsigned char);

          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageLoginResponse (void);

          /**
           * @brief Implements the inherited method. getKey.
           * @see IMessage::getKey()
           */
          KeyType getKey (void) const;

          /**
           * @brief Implements the inherited method. size.
           * @see IMessage::size()
           */
          SizeType size (void) const;

          /**
           * @brief Implements the inherited method. serialize.
           * @see IMessage::serialize()
           */
          BufferType serialize (void) const;

          /**
           * @brief Implements the inherited method. parse.
           * @see IMessage::parse()
           */
          bool parse (BufferType const &);

          /**
           * @brief Retrieves the spot application id.
           * @return The spot application id.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getSpotAppId (void) const;

          /**
           * @brief Retrieves the ssk message field.
           * @return The ssk message field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getSsk (void) const;

          /**
           * @brief Retrieves the command message field.
           * @return The command message field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getCommand(void) const;

          /**
           * @brief Retrieves the acknowledge response code.
           * @return The acknowledge response code.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getAckCode( void ) const;

          /**
           * @brief Sets the acknowledge response code.
           * @param ackcode. The acknowledge response code.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setAckCode( const unsigned char ackcode );

          /**
            * @brief Retrieves the date-time response field. Date and time of last login hhmmssddmmyyyy.
            * @return the date-time response field
            * @exception Throws no exception.
            * @since 0.0.0
            *  */
          BufferType getDateTime() const;       /// @brief

          /**
            * @brief Sets the date-time response field
            * @param datetime, the date-time response field
            * @exception Throws no exception.
            * @since 0.0.0
            *  */
          void setDateTime(const BufferType& datetime);

          /**
            * @brief Retrieves the application entries response field
            * @return the application entries vector
            * @exception Throws no exception.
            * @since 0.0.0
            *  */
          std::vector<AppEntry> getAppEntries() const;

          /**
            * @brief Retrieves the spot application entries response field
            * @param entries the application entries vector
            * @see AppEntry.
            * @exception Throws no exception.
            * @since 0.0.0
            *  */
          void setAppEntries(const std::vector<AppEntry> entries);

          /**
            * @brief Retrieves the rsa key response field.
            * @return the rsa key response field.
            * @exception Throws no exception.
            * @since 0.0.0
            *  */
          BufferType getRSAKey() const; ///public key modulo len

          /**
           * @brief Sets the rsa key response code.
           * @param key. The rsa key buffer.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setRSAKey(const BufferType& key); ///public key modulo len

        private:

          /** @brief acknowledge response code*/
          unsigned char ackCode;

          /** @brief Date and time of last login hh:mm:ss, dd/mm/yyyy*/
          BufferType dateTime;

          /** @brief SPOT local applications data*/
          std::vector<AppEntry> entries;

          /** @brief RSAKey[] If public key (PMK) was required.*/
          BufferType rsaKey;
      };
    }
  }
}

#endif
