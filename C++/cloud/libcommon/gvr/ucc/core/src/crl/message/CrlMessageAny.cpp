/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageAny.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageAny.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      CrlMessageAny::~CrlMessageAny (void)
      {
      }

      CrlMessageAny::CrlMessageAny(void)
       : AbsCrlMessage(CInteralFlagInternalMessage)
      {
      }

      void CrlMessageAny::setInternalFlag (unsigned char flag)
      {
        private_setInternalFlag(flag);
      }

      CrlMessageAny::TypeId CrlMessageAny::GetTypeId (void)
      {
        return _getTypeId<CrlMessageAny>();
      };

      CrlMessageAny::TypeId CrlMessageAny::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
