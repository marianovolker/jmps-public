/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageOptSwInfoResponse.h
 * @author mgrieco
 * @date Monday, May 11, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEOPTSWINFORESPONSE_H_
# define _SPOTMESSAGEOPTSWINFORESPONSE_H_

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
       * @brief This class is the implementation for OptSwInfo message response on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageOptSwInfoResponse : public ISpotMessage
      {
        public:
          /**@brief Spot Message Identification.*/
          static const unsigned char COptSwInfoMessageResponseId = 0x10;

          /**@brief Spot Message Identification.*/
          static const unsigned char COptSwInfoMessageResponseSubCmd = 0x03;

          /**@brief no variable part size. (no tables elements, ack code no checked)*/
          static const int CMessageCommonSize = 7 ;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageOptSwInfoResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };


          /**
           * @type   AllInfoMap
           * @brief  Return type when requesting all software/firmware information.
           * @see    SpotMessageOptSwInfoResponse::getAllOptSoftwareDescription()
           */
           typedef std::map<std::string, std::string> OptSoftwareInfoMap;



          /**
           * @brief default constructor
           */
          SpotMessageOptSwInfoResponse(void);

          /**
           * Constructor giving the acknowledge value.
           * @param the acknowledge value for the OptSwInfo Response.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          SpotMessageOptSwInfoResponse(unsigned char);

          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageOptSwInfoResponse (void);

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
           * @brief Retrieves the sub command message field.
           * @return The command message field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getSubCommand(void) const;

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

          /*
           * @brief Gets all the software info as a map.
           *
           * @param [Out] @b sLabel is the map label legend to be used in reports.
           * @param [Out] @b map is the returned map<string(field_name),string(field_value)>.
           * @return @b true if everything went OK collecting items from the response.
           *         @b false if an error blocked to collect all the info.
           */
           void getAllOptSoftwareDescription (std::string & sLabel, OptSoftwareInfoMap & map ) const ;


        private:

          /**
           * @brief This class represents each application entry on the OptSwInfo message response applications table.
           **/
          class AppEntry
          {
            private:
              /**@brief spot message entry fields*/
              unsigned char tagId;
              unsigned char tagLen;
              BufferType tagData;

            public:

              /** @brief Default constructor **/
              AppEntry(void)
              {
                tagId = 0;
                tagLen = 0;
              }

              /** @brief Default constructor with all fields values
               **/
              AppEntry(unsigned char const tagId, unsigned char const tagLen, BufferType const tagData)
              {
                this->tagId = tagId;
                this->tagLen = tagLen;
                this->tagData = tagData;
              }


              /**
               * @brief Retrieves the application id field
               * @return the application id field value
               **/
              unsigned char getTagId() const
              {
                return tagId;
              }

              /**
               * @brief Sets the application id field
               * @param appAscii the application id field value
               **/
              void setTagId(const unsigned char appid)
              {
                this->tagId = appid;
              }

              /**
               * @brief Retrieves the tag len field
               * @return the tag len field value
               **/
              unsigned char getTagLen() const
              {
                return tagLen;
              }

              /**
               * @brief Sets the tag len field
               * @param appAscii the tag len field value
               **/
              void setTagLen(const unsigned char len)
              {
                this->tagLen = len;
              }

              /**
               * @brief Retrieves the tag data field
               * @return the tag data field value
               **/
              BufferType getTagData() const
              {
                return tagData;
              }

              /**
               * @brief Sets the tag data field
               * @param appAscii the tag data field value
               **/
              void setTagData(const BufferType& tag)
              {
                this->tagData = tag;
              }
          };


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
           * @brief Gets the item description/details as an output string.
           *
           * @param iTagID is the "sub-item unique code" or hardware info tag ID.
           * @param [Out] @b sOut The sub-item description/details to be returned.
           * @param maxLength (optional) If supplied (not null), clips the output text to be no larger than this length.
           *
           */
           bool getSubItemDescription(int iTagID, std::string & sOut) const ;

          /**
           * @brief Gets the item label as an output string.
           *
           * @param iTagID is the "sub-item unique code" or hardware info tag ID.
           * @param [Out] @b sOut The sub-item label to be returned.
           *
           */
          bool getSubItemLabel(int iTagID, std::string & sOut) const ;


          /** @brief Item identification codes */
          enum ItemID
          {
            eFirstID              =  0x01, ///< @brief Just a marker for the first ID.
            ePR_FWR_VERSION       =  0x01, ///< @brief Printer firmware version.
            eIO_FWR_VERSION       =  0x02, ///< @brief IO module firmware version.
            eCL_FWR_VERSION       =  0x03, ///< @brief Main Firmware Version of the OTI reader - Available only on M3 EMEA and M4.
            eCL_PROTOCOL_VERSION  =  0x04, ///< @brief Protocol Version used to communicate with the OTI reader - Available only on M3 EMEA and M4.
            eTRIND_FWR_VERSION    =  0x06, ///< @brief Trind module firmware version
            eLastID               =  0x06  ///< @brief Just a marker for the last ID.
          };

          struct ItemIDLabel
          {
            int           itemID;
            const char *  itemLabel;
          };


          /** @brief acknowledge response code*/
          unsigned char ackCode;

          /** @brief SPOT local applications data*/
          std::vector<AppEntry> entries;

          static const char *                   CpcOptSwInfo;             ///< @brief Label "opt software info".

          static const ISpotMessage::SizeType   CMaxItems = 12;           ///< @brief Max. number of requested items.
          static const ItemIDLabel              aCItemLabels[CMaxItems];  ///< @brief Items IDs and its labels.
      };
    }
  }
}

#endif
