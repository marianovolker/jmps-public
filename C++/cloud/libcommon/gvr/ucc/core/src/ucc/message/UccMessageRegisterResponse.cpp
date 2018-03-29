/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageRegisterResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageRegisterResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageRegisterResponse::CSuccessName = "success";
      char const * UccMessageRegisterResponse::CConfigName = "config";
      char const * UccMessageRegisterResponse::CGvrSiteIdName = "gvrSiteId";
      char const * UccMessageRegisterResponse::CDeviceIdName = "deviceId";
      char const * UccMessageRegisterResponse::CConnectName = "connect";
      char const * UccMessageRegisterResponse::CUriName = "uri";
      char const * UccMessageRegisterResponse::CCertFormatName = "certFormat";
      char const * UccMessageRegisterResponse::CCertName = "cert";
      char const * UccMessageRegisterResponse::CUserIdName = "userid";
      char const * UccMessageRegisterResponse::CPwdName = "pwd";
      char const * UccMessageRegisterResponse::CDeviceListName = "deviceList";
      char const * UccMessageRegisterResponse::CDeviceTypeName = "deviceType";
      char const * UccMessageRegisterResponse::CLocalIdName = "localId";
      char const * UccMessageRegisterResponse::CCloudStateName = "cloudState";
      char const * UccMessageRegisterResponse::CFileUploadUrlName = "fileUploadURL";
      char const * UccMessageRegisterResponse::CCodeName = "code";
      char const * UccMessageRegisterResponse::CMessageName = "message";
      char const * UccMessageRegisterResponse::CErrorNumberName = "errorNumber";
      char const * UccMessageRegisterResponse::CUccCodeName = "ucc_code";
      char const * UccMessageRegisterResponse::CUccMessageName = "ucc_message";

      UccMessageRegisterResponse::~UccMessageRegisterResponse (void)
      {
      }

      UccMessageRegisterResponse::UccMessageRegisterResponse(void)
       : AbsUccMessageRegister()
      {
      }

      bool UccMessageRegisterResponse::validate (void)
      {
        bool result = AbsUccMessageRegister::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CSuccessName).isBoolean();
          if (result)
          {
            if (m_json.get(CSuccessName).asBoolean())
            {
              result = result && m_json.get(CConfigName).isObject();
              if (result)
              {
                result = result && m_json.get(CConfigName).get(CGvrSiteIdName).isNumber();

                result = result && m_json.get(CConfigName).get(CDeviceIdName).isString(); //Number
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CConfigName).get(CDeviceIdName).asString().empty()); //Number

                result = result && m_json.get(CConfigName).get(CConnectName).isObject();
                if (result)
                {
                  result = result && m_json.get(CConfigName).get(CConnectName).get(CUriName).isString(); //URL
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CConfigName).get(CConnectName).get(CUriName).asString().empty()); //URL

                  result = result && m_json.get(CConfigName).get(CConnectName).get(CCertFormatName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CConfigName).get(CConnectName).get(CCertFormatName).asString().empty());

                  result = result && m_json.get(CConfigName).get(CConnectName).get(CCertName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CConfigName).get(CConnectName).get(CCertName).asString().empty());

                  result = result && m_json.get(CConfigName).get(CConnectName).get(CUserIdName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CConfigName).get(CConnectName).get(CUserIdName).asString().empty());

                  result = result && m_json.get(CConfigName).get(CConnectName).get(CPwdName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CConfigName).get(CConnectName).get(CPwdName).asString().empty());
                }

                result = result && m_json.get(CConfigName).get(CDeviceListName).isArray();
                if (result)
                {
                  int length = m_json.get(CConfigName).get(CDeviceListName).length();
                  for (int i = 0; (result) && (i < length); ++i)
                  {
                    result = result && m_json.get(CConfigName).get(CDeviceListName)[i].isObject();
                    if (result)
                    {
                      result = result && m_json.get(CConfigName).get(CDeviceListName)[i].get(CDeviceTypeName).isString();
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CConfigName).get(CDeviceListName)[i].get(CDeviceTypeName).asString().empty());

                      result = result && m_json.get(CConfigName).get(CDeviceListName)[i].get(CDeviceIdName).isString(); //Number
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CConfigName).get(CDeviceListName)[i].get(CDeviceIdName).asString().empty()); //Number

                      result = result && m_json.get(CConfigName).get(CDeviceListName)[i].get(CLocalIdName).isString(); //Number
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CConfigName).get(CDeviceListName)[i].get(CLocalIdName).asString().empty()); //Number

                      result = result && m_json.get(CConfigName).get(CDeviceListName)[i].get(CCloudStateName).isString();
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CConfigName).get(CDeviceListName)[i].get(CCloudStateName).asString().empty());

                      result = result && m_json.get(CConfigName).get(CDeviceListName)[i].get(CFileUploadUrlName).isString(); //URL
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CConfigName).get(CDeviceListName)[i].get(CFileUploadUrlName).asString().empty()); //URL
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

              if (result && (!m_json.get(CErrorNumberName).isNull()))
              {
                result = result && m_json.get(CErrorNumberName).isNumber();
              }

              if (result && (!m_json.get(CUccCodeName).isNull()))
              {
                result = result && m_json.get(CUccCodeName).isNumber();
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CUccCodeName).asString().empty());
              }

              if (result && (!m_json.get(CUccMessageName).isNull()))
              {
                result = result && m_json.get(CUccMessageName).isString();
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CUccMessageName).asString().empty());
              }
            }
          }
        }
        return result;
      }

      UccMessageRegisterResponse::TypeId UccMessageRegisterResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
