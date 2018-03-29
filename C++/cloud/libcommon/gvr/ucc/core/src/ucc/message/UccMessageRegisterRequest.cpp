/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageRegisterRequest.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageRegisterRequest.h>

#include <iostream>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageRegisterRequest::CRegisterDeviceName = "registerDevice";
      char const * UccMessageRegisterRequest::CRegistrationCodeName = "registrationCode";
      char const * UccMessageRegisterRequest::CCertFormatName = "certFormat";
      char const * UccMessageRegisterRequest::CDnsSupportedName = "dnsSupported";
      char const * UccMessageRegisterRequest::CDeviceConfigName = "deviceConfig";
      char const * UccMessageRegisterRequest::CDeviceTypeName = "deviceType";
      char const * UccMessageRegisterRequest::CLocalIdName = "localId";
      char const * UccMessageRegisterRequest::CAssetInfoName = "assetInfo";
      char const * UccMessageRegisterRequest::CManufacturerName = "manufacturer";
      char const * UccMessageRegisterRequest::CModelName = "model";
      char const * UccMessageRegisterRequest::CSerialNbrName = "serialNbr";
      char const * UccMessageRegisterRequest::CFirmwareName = "firmware";
      char const * UccMessageRegisterRequest::CComponentName = "component";
      char const * UccMessageRegisterRequest::CNameName = "name";
      char const * UccMessageRegisterRequest::CRemoveName = "remove";
      char const * UccMessageRegisterRequest::CAtributesName = "attributes";
      char const * UccMessageRegisterRequest::CEventsName = "events";
      char const * UccMessageRegisterRequest::COperationName = "operation";
      char const * UccMessageRegisterRequest::CPayloadName = "payload";
      char const * UccMessageRegisterRequest::CCmdsName = "cmds";
      char const * UccMessageRegisterRequest::CRequestName = "request";
      char const * UccMessageRegisterRequest::CResponseName = "response";
      char const * UccMessageRegisterRequest::CAssociatedName = "associated";
      char const * UccMessageRegisterRequest::CProxiedName = "proxied";

      UccMessageRegisterRequest::~UccMessageRegisterRequest (void)
      {
      }

      UccMessageRegisterRequest::UccMessageRegisterRequest(void)
       : AbsUccMessageRegister()
      {
      }

      bool UccMessageRegisterRequest::validate (void)
      {
        static std::string const empty = "";
        bool result = AbsUccMessageRegister::validate();
        // Validate required fields.
        result = result && m_json.get(CRegisterDeviceName).isObject();
        if (result)
        {
          result = result && m_json.get(CRegisterDeviceName).get(CRegistrationCodeName).isString();
          // TODO: Check with Business Analyst!!!
          // result = result && (!m_json.get(CRegisterDeviceName).get(CRegistrationCodeName).asString().empty());

          result = result && m_json.get(CRegisterDeviceName).get(CCertFormatName).isString();
          // TODO: Check with Business Analyst!!!
          // result = result && (!m_json.get(CRegisterDeviceName).get(CCertFormatName).asString().empty());

          result = result && m_json.get(CRegisterDeviceName).get(CDnsSupportedName).isBoolean();
        }

        result = result && m_json.get(CDeviceConfigName).isObject();
        if (result)
        {
          result = result && m_json.get(CDeviceConfigName).get(CDeviceTypeName).isString();
          // TODO: Check with Business Analyst!!!
          // result = result && (!m_json.get(CDeviceConfigName).get(CDeviceTypeName).asString().empty());

          result = result && m_json.get(CDeviceConfigName).get(CLocalIdName).isString(); //Number
          // TODO: Check with Business Analyst!!!
          // result = result && (!m_json.get(CDeviceConfigName).get(CLocalIdName).asString().empty()); //Number

          if (result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).isNull()))
          {
            result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).isObject();
            if (result)
            {
              result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CManufacturerName).isString();
              // TODO: Check with Business Analyst!!!
              // result = result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CManufacturerName).asString().empty());

              if (result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CModelName).isNull()))
              {
                result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CModelName).isString();
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CModelName).asString().empty());
              }

              if (result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CSerialNbrName).isNull()))
              {
                result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CSerialNbrName).isString(); //Number
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CSerialNbrName).asString().empty()); //Number
              }

              if (result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CFirmwareName).isNull()))
              {
                result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CFirmwareName).isObject();
                if (result)
                {
                  int lengthI = m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CFirmwareName).length();
                  for (int i = 0; (result) && (i < lengthI); ++i)
                  {
                    result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CFirmwareName)[i].isString();
                    // TODO: Check with Business Analyst!!!
                    // result = result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CFirmwareName)[i].asString().empty());

                    result = result && (empty.compare(m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CFirmwareName)[i].name()) != 0);
                  }
                }
              }

              if (result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName).isNull()))
              {
                result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName).isArray();
                if (result)
                {
                  int lengthI = m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName).length();
                  for (int i = 0; (result) && (i < lengthI); ++i)
                  {
                    result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].isObject();
                    if (result)
                    {
                      if (result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CRemoveName).isNull()))
                      {
                        result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CRemoveName).isBoolean();
                      }

                      result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CNameName).isString();
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CNameName).asString().empty());

                      if (result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).isNull()))
                      {
                        result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).isObject();
                        if (result)
                        {
                          int lengthJ = m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).length();
                          for (int j = 0; (result) && (j < lengthJ); ++j)
                          {
                            result = result && m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].isString();
                            // TODO: Check with Business Analyst!!!
                            // result = result && (!m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].asString().empty());

                            result = result && (empty.compare(m_json.get(CDeviceConfigName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].name()) != 0);
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }

          if (result && (!m_json.get(CDeviceConfigName).get(CEventsName).isNull()))
          {
            result = result && m_json.get(CDeviceConfigName).get(CEventsName).isArray();
            if (result)
            {
              int lengthI = m_json.get(CDeviceConfigName).get(CEventsName).length();
              for (int i = 0; (result) && (i < lengthI); ++i)
              {
                result = result && m_json.get(CDeviceConfigName).get(CEventsName)[i].isObject();
                if (result)
                {
                  result = result && m_json.get(CDeviceConfigName).get(CEventsName)[i].get(COperationName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CDeviceConfigName).get(CEventsName)[i].get(COperationName).asString().empty());

                  result = result && m_json.get(CDeviceConfigName).get(CEventsName)[i].get(CPayloadName).isString(); //URL
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CDeviceConfigName).get(CEventsName)[i].get(CPayloadName).asString().empty()); //URL
                }
              }
            }
          }

          if (result && (!m_json.get(CDeviceConfigName).get(CCmdsName).isNull()))
          {
            result = result && m_json.get(CDeviceConfigName).get(CCmdsName).isArray();
            if (result)
            {
              int lengthI = m_json.get(CDeviceConfigName).get(CCmdsName).length();
              for (int i = 0; (result) && (i < lengthI); ++i)
              {
                result = result && m_json.get(CDeviceConfigName).get(CCmdsName)[i].isObject();
                if (result)
                {
                  result = result && m_json.get(CDeviceConfigName).get(CCmdsName)[i].get(COperationName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CDeviceConfigName).get(CCmdsName)[i].get(COperationName).asString().empty());

                  result = result && m_json.get(CDeviceConfigName).get(CCmdsName)[i].get(CRequestName).isString(); //URL
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CDeviceConfigName).get(CCmdsName)[i].get(CRequestName).asString().empty()); //URL

                  result = result && m_json.get(CDeviceConfigName).get(CCmdsName)[i].get(CResponseName).isString(); //URL and can be empty
                }
              }
            }
          }

          if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName).isNull()))
          {
            result = result && m_json.get(CDeviceConfigName).get(CAssociatedName).isArray();
            if (result)
            {
              int lengthH = m_json.get(CDeviceConfigName).get(CAssociatedName).length();
              for (int h = 0; (result) && (h < lengthH); ++h)
              {
                result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].isObject();
                if (result)
                {
                  result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CProxiedName).isBoolean();
////////////////////////////////////////////////////////////////////////////////
                  result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CDeviceTypeName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CDeviceTypeName).asString().empty());

                  result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CLocalIdName).isString(); //Number
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CLocalIdName).asString().empty()); //Number

                  if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).isNull()))
                  {
                    result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).isObject();
                    if (result)
                    {
                      result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CManufacturerName).isString();
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CManufacturerName).asString().empty());

                      if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CModelName).isNull()))
                      {
                        result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CModelName).isString();
                        // TODO: Check with Business Analyst!!!
                        // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CModelName).asString().empty());
                      }

                      if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CSerialNbrName).isNull()))
                      {
                        result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CSerialNbrName).isString(); //Number
                        // TODO: Check with Business Analyst!!!
                        // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CSerialNbrName).asString().empty()); //Number
                      }

                      if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName).isNull()))
                      {
                        result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName).isObject();
                        if (result)
                        {
                          int lengthI = m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName).length();
                          for (int i = 0; (result) && (i < lengthI); ++i)
                          {
                            result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName)[i].isString();
                            // TODO: Check with Business Analyst!!!
                            // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName)[i].asString().empty());

                            result = result && (empty.compare(m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName)[i].name()) != 0);
                          }
                        }
                      }

                      if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName).isNull()))
                      {
                        result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName).isArray();
                        if (result)
                        {
                          int lengthI = m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName).length();
                          for (int i = 0; (result) && (i < lengthI); ++i)
                          {
                            result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].isObject();
                            if (result)
                            {
                              if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CRemoveName).isNull()))
                              {
                                result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CRemoveName).isBoolean();
                              }

                              result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CNameName).isString();
                              // TODO: Check with Business Analyst!!!
                              // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CNameName).asString().empty());

                              if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).isNull()))
                              {
                                result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).isObject();
                                if (result)
                                {
                                  int lengthJ = m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).length();
                                  for (int j = 0; (result) && (j < lengthJ); ++j)
                                  {
                                    result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].isString();
                                    // TODO: Check with Business Analyst!!!
                                    // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].asString().empty());

                                    result = result && (empty.compare(m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].name()) != 0);
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }

                  if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CEventsName).isNull()))
                  {
                    result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CEventsName).isArray();
                    if (result)
                    {
                      int lengthI = m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CEventsName).length();
                      for (int i = 0; (result) && (i < lengthI); ++i)
                      {
                        result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CEventsName)[i].isObject();
                        if (result)
                        {
                          result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CEventsName)[i].get(COperationName).isString();
                          // TODO: Check with Business Analyst!!!
                          // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CEventsName)[i].get(COperationName).asString().empty());

                          result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CEventsName)[i].get(CPayloadName).isString(); //URL
                          // TODO: Check with Business Analyst!!!
                          // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CEventsName)[i].get(CPayloadName).asString().empty()); //URL
                        }
                      }
                    }
                  }

                  if (result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CCmdsName).isNull()))
                  {
                    result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CCmdsName).isArray();
                    if (result)
                    {
                      int lengthI = m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CCmdsName).length();
                      for (int i = 0; (result) && (i < lengthI); ++i)
                      {
                        result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CCmdsName)[i].isObject();
                        if (result)
                        {
                          result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CCmdsName)[i].get(COperationName).isString();
                          // TODO: Check with Business Analyst!!!
                          // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CCmdsName)[i].get(COperationName).asString().empty());

                          result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CRequestName).isString(); //URL
                          // TODO: Check with Business Analyst!!!
                          // result = result && (!m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CRequestName).asString().empty()); //URL

                          result = result && m_json.get(CDeviceConfigName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CResponseName).isString(); //URL and can be empty
                        }
                      }
                    }
                  }
////////////////////////////////////////////////////////////////////////////////
                }
              }
            }
          }
        }
        return result;
      }

      UccMessageRegisterRequest::TypeId UccMessageRegisterRequest::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
