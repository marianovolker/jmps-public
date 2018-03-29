/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageSystemStatus.cpp
 * @author Mariano Volker
 * @date Thursday, March 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessageSystemStatus.h>

#include <iostream>
#include <gvr/communication/spot/Utils.h>


/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageSystemStatus::SpotMessageSystemStatus ( unsigned char bAppId /* = CMessageResponsePinpadAppId */ )
        :  AbsSpotMessageWithAppCatCmdID (false, bAppId, CSystemStatusCommandId, CSystemStatusSubCommandId),
           m_subSystemStatus(eSPOT_SYS_MAIN)
      {
        // By default this command is set ready to request all available items in SPOT.
        setSerializedSize( CMinMessageSize );
      }

      SpotMessageSystemStatus::~SpotMessageSystemStatus (void)
      {
      }

      ISpotMessage::BufferType SpotMessageSystemStatus::serialize (void) const
      {
        // Serialize the header.
        ISpotMessage::BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

        retBuffer.push_back( static_cast<unsigned char>(m_subSystemStatus) );

        char pcBuffer[(retBuffer.size()*3)+2];
        std::cout << "SpotMessageSystemStatus Serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), retBuffer.size()) << std::endl;

        return retBuffer;
      }

      bool SpotMessageSystemStatus::parse (BufferType const & buff)
      {
        if(buff.size() <= CHeaderSize)  return false; // message too short

        if( buff[CMessageAppIdOffset] == ISpotMessageWithAppCatCmdID::CMessageRequestPinpadAppId ||
            buff[CMessageAppIdOffset] == ISpotMessageWithAppCatCmdID::CMessageRequestOPTAppId )
        {
          setAppId( buff[CMessageAppIdOffset] );
        }
        else
        {
          return false; // not an Application ID having system status notification (Pinpad or OPT).
        }

        bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff);

        if( !blRet ) return false; // malformed header

        m_subSystemStatus = buff[CMessageItemDataOffset];

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessageSystemStatus Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return true;
      }

    }
  }
}
