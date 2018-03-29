/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageAny.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageAny.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      UccMessageAny::~UccMessageAny (void)
      {
      }

      UccMessageAny::UccMessageAny(void)
       : AbsUccMessage(CInteralFlagInternalMessage)
      {
      }

      void UccMessageAny::setInternalFlag (unsigned char flag)
      {
        private_setInternalFlag(flag);
      }

      UccMessageAny::TypeId UccMessageAny::GetTypeId (void)
      {
        return _getTypeId<UccMessageAny>();
      };

      UccMessageAny::TypeId UccMessageAny::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
