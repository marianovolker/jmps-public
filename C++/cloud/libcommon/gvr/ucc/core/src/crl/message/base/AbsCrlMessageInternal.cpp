/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsCrlMessageInternal.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/base/AbsCrlMessageInternal.h>

#include <Poco/UUIDGenerator.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      char const * AbsCrlMessageInternal::COperationName = "operation";
      char const * AbsCrlMessageInternal::CPayloadName = "payload";

      AbsCrlMessageInternal::~AbsCrlMessageInternal (void)
      {
      }

      bool AbsCrlMessageInternal::validate (void)
      {
        bool result = AbsCrlMessage::validate();
        if (result)
        {
          result = result && (m_flag == CInteralFlagInternalMessage);
          result = result && (m_json.get(COperationName).isString());
          result = result && (m_kOperation.compare(m_json.get(COperationName).asCString()) == 0);
        }
        return result;
      }

      std::string AbsCrlMessageInternal::getOperationExpected (void) const
      {
        return m_kOperation;
      }

      std::string AbsCrlMessageInternal::getOperation (void) const
      {
        return m_json.get(COperationName).asCString();
      }

      void AbsCrlMessageInternal::resetHeader (bool force = false)
      {
        m_json.set(COperationName) = m_kOperation;
      }

      AbsCrlMessageInternal::JsonType const AbsCrlMessageInternal::getPayload (void) const
      {
        return m_json.get(CPayloadName);
      }

      void AbsCrlMessageInternal::setPayload (AbsJsonType const & json)
      {
        m_json.set(CPayloadName) = json;
      }

      void AbsCrlMessageInternal::removePayload (void)
      {
        m_json.remove(CPayloadName);
      }

      AbsCrlMessageInternal::AbsCrlMessageInternal (char const * operation)
       : AbsCrlMessage(CInteralFlagInternalMessage), m_kOperation(operation)
      {
        // Ensure these fields exists in the message, if not they will be created.
        m_json.set(COperationName) = m_kOperation;
      }
    }
  }
}
