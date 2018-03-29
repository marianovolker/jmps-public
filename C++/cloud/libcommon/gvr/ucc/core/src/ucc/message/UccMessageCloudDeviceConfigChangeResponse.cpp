/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceConfigChangeResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudDeviceConfigChangeResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudDeviceConfigChangeResponse::CSuccessName = "success";
      char const * UccMessageCloudDeviceConfigChangeResponse::CAssociatedIdName = "associatedId";
      char const * UccMessageCloudDeviceConfigChangeResponse::CDeviceIdName = "deviceId";
      char const * UccMessageCloudDeviceConfigChangeResponse::CDeviceTypeName = "deviceType";
      char const * UccMessageCloudDeviceConfigChangeResponse::CLocalIdName = "localId";
      char const * UccMessageCloudDeviceConfigChangeResponse::CCloudStateName = "cloudState";
      // TODO: Check with Business Analyst!!!
      //char const * UccMessageCloudDeviceConfigChangeResponse::CFileUploadUrlName = "fileUploadURL";
      char const * UccMessageCloudDeviceConfigChangeResponse::CCodeName = "code";
      char const * UccMessageCloudDeviceConfigChangeResponse::CMessageName = "message";

      UccMessageCloudDeviceConfigChangeResponse::~UccMessageCloudDeviceConfigChangeResponse (void)
      {
      }

      UccMessageCloudDeviceConfigChangeResponse::UccMessageCloudDeviceConfigChangeResponse(void)
       : AbsUccMessageCloudEventResponse()
      {
      }

      bool UccMessageCloudDeviceConfigChangeResponse::validate (void)
      {
        bool result = AbsUccMessageCloudEventResponse::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CMessageIdName).isString();
          result = result && (!m_json.get(CMessageIdName).asString().empty());

          result = result && m_json.get(CSuccessName).isBoolean();
          if (result)
          {
            if (m_json.get(CSuccessName).asBoolean())
            {
              if (result && (!m_json.get(CAssociatedIdName).isNull()))
              {
                result = result && m_json.get(CAssociatedIdName).isArray();
                if (result)
                {
                  int length = m_json.get(CAssociatedIdName).length();
                  for (int i = 0; (result) && (i < length); ++i)
                  {
                    result = result && m_json.get(CAssociatedIdName)[i].isObject();
                    if (result)
                    {
                      result = result && m_json.get(CAssociatedIdName)[i].get(CDeviceTypeName).isString();
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CAssociatedIdName)[i].get(CDeviceTypeName).asString().empty());

                      result = result && m_json.get(CAssociatedIdName)[i].get(CDeviceIdName).isString(); //Number
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CAssociatedIdName)[i].get(CDeviceIdName).asString().empty()); //Number

                      result = result && m_json.get(CAssociatedIdName)[i].get(CLocalIdName).isString(); //Number
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CAssociatedIdName)[i].get(CLocalIdName).asString().empty()); //Number

                      result = result && m_json.get(CAssociatedIdName)[i].get(CCloudStateName).isString();
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CAssociatedIdName)[i].get(CCloudStateName).asString().empty());

                      // TODO: Check with Business Analyst!!!
                      // result = result && m_json.get(CAssociatedIdName)[i].get(CFileUploadUrlName).isString(); //URL
                      // result = result && (!m_json.get(CAssociatedIdName)[i].get(CFileUploadUrlName).asString().empty()); //URL
                    }
                  }
                }
              }
            }
            else
            {
              if (result && (!m_json.get(CCodeName).isNull()))
              {
                result = result && m_json.get(CCodeName).isString();
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CCodeName).asString().empty());
              }

              if (result && (!m_json.get(CMessageName).isNull()))
              {
                result = result && m_json.get(CMessageName).isString();
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CMessageName).asString().empty());
              }
            }
          }
        }
        return result;
      }

      UccMessageCloudDeviceConfigChangeResponse::TypeId UccMessageCloudDeviceConfigChangeResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
