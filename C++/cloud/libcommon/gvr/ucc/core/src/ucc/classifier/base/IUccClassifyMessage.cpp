/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IUccClassifyMessage.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/classifier/base/IUccClassifyMessage.h>
#include <gvr/utils/Json.h>
#include <arpa/inet.h>

#include <iostream>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief ucc Domain
    namespace ucc
    {
      unsigned int const IUccClassifyMessage::CSizeFiledSize = 4 * sizeof(unsigned char);
      unsigned int const IUccClassifyMessage::CFlagFiledSize = 1 * sizeof(unsigned char);
      unsigned int const IUccClassifyMessage::CBinaryFiledsSize = IUccClassifyMessage::CSizeFiledSize + IUccClassifyMessage::CFlagFiledSize;

      IUccClassifyMessage::IntegrityType IUccClassifyMessage::parseToStructure (BufferType const & buffer, SizeType & parsedSize, SizeType & structureSize, unsigned char & flag, JsonType & json)
      {
        IntegrityType integrity = Integrity::CIncomplete;
        std::size_t receivedSize = buffer.size();
        parsedSize = receivedSize;
        structureSize = 0;
        flag = 0;
        json.clear();
        if (receivedSize >= CBinaryFiledsSize)
        {
          union
          {
            unsigned int lengh;
            unsigned char byte[4];
          } network;
          network.lengh = 0;
          network.byte[0] = buffer[0];
          network.byte[1] = buffer[1];
          network.byte[2] = buffer[2];
          network.byte[3] = buffer[3];
          std::size_t const messageSize = ntohl(network.lengh);
          if (messageSize >= CBinaryFiledsSize)
          {
            structureSize = messageSize - CBinaryFiledsSize;
            flag = buffer[4];
            if (receivedSize >= messageSize)
            {
              parsedSize = messageSize;
              if (structureSize)
              {
                std::string jsonMsg(&buffer[CBinaryFiledsSize],&buffer[messageSize]);
                if (jsonMsg.size() == structureSize)
                {
                  if (json.parse(jsonMsg))
                  {
                    integrity = Integrity::CComplete;
                  }
                  else
                  {
                    integrity = Integrity::CCorrupt;
                  }
                }
                else
                {
                  integrity = Integrity::CCorrupt;
                }
              }
              else
              {
                integrity = Integrity::CComplete;
              }
            }
          }
          else
          {
            integrity = Integrity::CCorrupt;
          }
        }
        return integrity;
      }

      IUccClassifyMessage::~IUccClassifyMessage (void)
      {
      }
    }
  }
}
