/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageDebianInfo.h
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEDEBIANINFO_H_
# define _SPOTMESSAGEDEBIANINFO_H_

#include <gvr/communication/spot/AbsSpotMessageWithAppCatCmdID.h>
# include <gvr/communication/spot/ISpotMessage.h>
# include <vector>

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
       * @brief This class is the implementation for Debian Info message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageDebianInfo : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CDebianInfoCommandId = 0x10;

          /**@brief Spot Message Identification.*/
          static const unsigned char CDebianInfoSubCommandId = 0x05;

          /**@brief fix message header size*/
          static const int CMessageHeaderSize = 6 ;
          /**@brief Default debian info size */
          static const int CDebiansInfoSize = 248;
          /**@brief fix message size*/
          static const int CMessageSize = CDebiansInfoSize + CMessageHeaderSize;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageDebianInfo>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageDebianInfo(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageDebianInfo (void);

          /**
           * @brief Constructor by all fields.
           * @param package, debian package name field
           * @since 0.0.0
           * */
          SpotMessageDebianInfo(std::string package);

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
           * @brief Sets the debian package list field.
           * @param package, debian package list field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setPackageListData( std::string list );


          /**
           * @brief Retrieves the debian package lista data field (separated by 0x20).
           * @return The debian package name field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          std::string getPackageListData( void ) const;

        private:
          /** @brief debian package name field*/
          std::string packageList;

      };
    }
  }
}

#endif
