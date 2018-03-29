/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageRegister.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/base/AbsUccMessageRegister.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      unsigned char const AbsUccMessageRegister::CInteralFlagRegisterMessage = 0x02;

      AbsUccMessageRegister::~AbsUccMessageRegister (void)
      {
      }

      bool AbsUccMessageRegister::validate (void)
      {
        return AbsUccMessage::validate() && (m_flag == CInteralFlagRegisterMessage);
      }

      AbsUccMessageRegister::TypeId AbsUccMessageRegister::GetTypeId (void)
      {
        return _getTypeId<AbsUccMessageRegister>();
      };

      AbsUccMessageRegister::AbsUccMessageRegister (void)
       : AbsUccMessage(CInteralFlagRegisterMessage)
      {
      }
    }
  }
}
