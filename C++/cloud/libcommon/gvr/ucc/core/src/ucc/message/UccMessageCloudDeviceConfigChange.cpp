/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceConfigChange.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudDeviceConfigChange.h>

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
      char const * UccMessageCloudDeviceConfigChange::CMessageOperation = "cloud-device-config-change-event";

      char const * UccMessageCloudDeviceConfigChange::CDeviceTypeName = "deviceType";
      char const * UccMessageCloudDeviceConfigChange::CLocalIdName = "localId";
      char const * UccMessageCloudDeviceConfigChange::CAssetInfoName = "assetInfo";
      char const * UccMessageCloudDeviceConfigChange::CManufacturerName = "manufacturer";
      char const * UccMessageCloudDeviceConfigChange::CModelName = "model";
      char const * UccMessageCloudDeviceConfigChange::CSerialNbrName = "serialNbr";
      char const * UccMessageCloudDeviceConfigChange::CFirmwareName = "firmware";
      char const * UccMessageCloudDeviceConfigChange::CComponentName = "component";
      char const * UccMessageCloudDeviceConfigChange::CNameName = "name";
      char const * UccMessageCloudDeviceConfigChange::CRemoveName = "remove";
      char const * UccMessageCloudDeviceConfigChange::CAtributesName = "attributes";
      char const * UccMessageCloudDeviceConfigChange::CEventsName = "events";
      char const * UccMessageCloudDeviceConfigChange::COperationName = "operation";
      char const * UccMessageCloudDeviceConfigChange::CPayloadName = "payload";
      char const * UccMessageCloudDeviceConfigChange::CCmdsName = "cmds";
      char const * UccMessageCloudDeviceConfigChange::CRequestName = "request";
      char const * UccMessageCloudDeviceConfigChange::CResponseName = "response";
      char const * UccMessageCloudDeviceConfigChange::CAssociatedName = "associated";
      // TODO: Check with Business Analyst!!!
      //char const * UccMessageCloudDeviceConfigChange::CProxiedName = "proxied";

      UccMessageCloudDeviceConfigChange::~UccMessageCloudDeviceConfigChange (void)
      {
      }

      UccMessageCloudDeviceConfigChange::UccMessageCloudDeviceConfigChange(void)
       : AbsUccMessageCloudEventRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudDeviceConfigChange::validate (void)
      {
        static std::string const empty = "";
        bool result = AbsUccMessageCloudEventRequest::validate();
        result = result && m_json.get(CMessageIdName).isString();
        result = result && (!m_json.get(CMessageIdName).asString().empty());

        result = result && m_json.get(CDeviceIdName).isString();
        result = result && (!m_json.get(CDeviceIdName).asString().empty());

        result = result && m_json.get(CPayloadName).isObject();
        if (result)
        {
          // TODO: Check with Business Analyst!!!
          // result = result && m_json.get(CPayloadName).get(CDeviceTypeName).isString();
          // result = result && (!m_json.get(CPayloadName).get(CDeviceTypeName).asString().empty());

          if (result && (!m_json.get(CPayloadName).get(CLocalIdName).isNull()))
          {
            result = result && m_json.get(CPayloadName).get(CLocalIdName).isString(); //Number
            // TODO: Check with Business Analyst!!!
            // result = result && (!m_json.get(CPayloadName).get(CLocalIdName).asString().empty()); //Number
          }

          if (result && (!m_json.get(CPayloadName).get(CAssetInfoName).isNull()))
          {
            result = result && m_json.get(CPayloadName).get(CAssetInfoName).isObject();
            if (result)
            {
              if (result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CManufacturerName).isNull()))
              {
                result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CManufacturerName).isString();
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CManufacturerName).asString().empty());
              }

              if (result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CModelName).isNull()))
              {
                result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CModelName).isString();
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CModelName).asString().empty());
              }

              if (result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CSerialNbrName).isNull()))
              {
                result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CSerialNbrName).isString(); //Number
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CSerialNbrName).asString().empty()); //Number
              }

              if (result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CFirmwareName).isNull()))
              {
                result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CFirmwareName).isObject();
                if (result)
                {
                  int lengthI = m_json.get(CPayloadName).get(CAssetInfoName).get(CFirmwareName).length();
                  for (int i = 0; (result) && (i < lengthI); ++i)
                  {
                    result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CFirmwareName)[i].isString();
                    // TODO: Check with Business Analyst!!!
                    // result = result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CFirmwareName)[i].asString().empty());

                    result = result && (empty.compare(m_json.get(CPayloadName).get(CAssetInfoName).get(CFirmwareName)[i].name()) != 0);
                  }
                }
              }

              if (result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName).isNull()))
              {
                result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName).isArray();
                if (result)
                {
                  int lengthI = m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName).length();
                  for (int i = 0; (result) && (i < lengthI); ++i)
                  {
                    result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].isObject();
                    if (result)
                    {
                      if (result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CRemoveName).isNull()))
                      {
                        result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CRemoveName).isBoolean();
                      }

                      result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CNameName).isString();
                      // TODO: Check with Business Analyst!!!
                      // result = result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CNameName).asString().empty());

                      if (result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).isNull()))
                      {
                        result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).isObject();
                        if (result)
                        {
                          int lengthJ = m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).length();
                          for (int j = 0; (result) && (j < lengthJ); ++j)
                          {
                            result = result && m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].isString();
                            // TODO: Check with Business Analyst!!!
                            // result = result && (!m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].asString().empty());

                            result = result && (empty.compare(m_json.get(CPayloadName).get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].name()) != 0);
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }

          if (result && (!m_json.get(CPayloadName).get(CEventsName).isNull()))
          {
            result = result && m_json.get(CPayloadName).get(CEventsName).isArray();
            if (result)
            {
              int lengthI = m_json.get(CPayloadName).get(CEventsName).length();
              for (int i = 0; (result) && (i < lengthI); ++i)
              {
                result = result && m_json.get(CPayloadName).get(CEventsName)[i].isObject();
                if (result)
                {
                  if (result && (!m_json.get(CPayloadName).get(CEventsName)[i].get(CRemoveName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CEventsName)[i].get(CRemoveName).isBoolean();
                  }

                  result = result && m_json.get(CPayloadName).get(CEventsName)[i].get(COperationName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CPayloadName).get(CEventsName)[i].get(COperationName).asString().empty());

                  if (result && (!m_json.get(CPayloadName).get(CEventsName)[i].get(CPayloadName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CEventsName)[i].get(CPayloadName).isString(); //URL
                    // TODO: Check with Business Analyst!!!
                    // result = result && (!m_json.get(CPayloadName).get(CEventsName)[i].get(CPayloadName).asString().empty()); //URL
                  }
                }
              }
            }
          }

          if (result && (!m_json.get(CPayloadName).get(CCmdsName).isNull()))
          {
            result = result && m_json.get(CPayloadName).get(CCmdsName).isArray();
            if (result)
            {
              int lengthI = m_json.get(CPayloadName).get(CCmdsName).length();
              for (int i = 0; (result) && (i < lengthI); ++i)
              {
                result = result && m_json.get(CPayloadName).get(CCmdsName)[i].isObject();
                if (result)
                {
                  if (result && (!m_json.get(CPayloadName).get(CCmdsName)[i].get(CRemoveName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CCmdsName)[i].get(CRemoveName).isBoolean();
                  }

                  result = result && m_json.get(CPayloadName).get(CCmdsName)[i].get(COperationName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CPayloadName).get(CCmdsName)[i].get(COperationName).asString().empty());

                  if (result && (!m_json.get(CPayloadName).get(CCmdsName)[i].get(CRequestName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CCmdsName)[i].get(CRequestName).isString(); //URL
                    // TODO: Check with Business Analyst!!!
                    // result = result && (!m_json.get(CPayloadName).get(CCmdsName)[i].get(CRequestName).asString().empty()); //URL
                  }

                  if (result && (!m_json.get(CPayloadName).get(CCmdsName)[i].get(CResponseName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CCmdsName)[i].get(CResponseName).isString(); //URL and can be empty
                  }
                }
              }
            }
          }

          if (result && (!m_json.get(CPayloadName).get(CAssociatedName).isNull()))
          {
            result = result && m_json.get(CPayloadName).get(CAssociatedName).isArray();
            if (result)
            {
              int lengthH = m_json.get(CPayloadName).get(CAssociatedName).length();
              for (int h = 0; (result) && (h < lengthH); ++h)
              {
                result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].isObject();
                if (result)
                {
                  if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CRemoveName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CRemoveName).isBoolean();
                  }

                  if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CDeviceIdName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CDeviceIdName).isString();
                  }

                  // TODO: Check with Business Analyst!!!
                  //result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CProxiedName).isBoolean();
////////////////////////////////////////////////////////////////////////////////
                  if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CDeviceTypeName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CDeviceTypeName).isString();
                    // TODO: Check with Business Analyst!!!
                    // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CDeviceTypeName).asString().empty());
                  }

                  if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CLocalIdName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CLocalIdName).isString(); //Number
                    // TODO: Check with Business Analyst!!!
                    // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CLocalIdName).asString().empty()); //Number
                  }

                  if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).isObject();
                    if (result)
                    {
                      if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CManufacturerName).isNull()))
                      {
                        result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CManufacturerName).isString();
                        // TODO: Check with Business Analyst!!!
                        // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CManufacturerName).asString().empty());
                      }

                      if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CModelName).isNull()))
                      {
                        result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CModelName).isString();
                        // TODO: Check with Business Analyst!!!
                        // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CModelName).asString().empty());
                      }

                      if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CSerialNbrName).isNull()))
                      {
                        result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CSerialNbrName).isString(); //Number
                        // TODO: Check with Business Analyst!!!
                        // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CSerialNbrName).asString().empty()); //Number
                      }

                      if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName).isNull()))
                      {
                        result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName).isObject();
                        if (result)
                        {
                          int lengthI = m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName).length();
                          for (int i = 0; (result) && (i < lengthI); ++i)
                          {
                            result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName)[i].isString();
                            // TODO: Check with Business Analyst!!!
                            // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName)[i].asString().empty());

                            result = result && (empty.compare(m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CFirmwareName)[i].name()) != 0);
                          }
                        }
                      }

                      if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName).isNull()))
                      {
                        result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName).isArray();
                        if (result)
                        {
                          int lengthI = m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName).length();
                          for (int i = 0; (result) && (i < lengthI); ++i)
                          {
                            result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].isObject();
                            if (result)
                            {
                              if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CRemoveName).isNull()))
                              {
                                result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CRemoveName).isBoolean();
                              }

                              result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CNameName).isString();
                              // TODO: Check with Business Analyst!!!
                              // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CNameName).asString().empty());

                              if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).isNull()))
                              {
                                result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).isObject();
                                if (result)
                                {
                                  int lengthJ = m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName).length();
                                  for (int j = 0; (result) && (j < lengthJ); ++j)
                                  {
                                    result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].isString();
                                    // TODO: Check with Business Analyst!!!
                                    // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].asString().empty());

                                    result = result && (empty.compare(m_json.get(CPayloadName).get(CAssociatedName)[h].get(CAssetInfoName).get(CComponentName)[i].get(CAtributesName)[j].name()) != 0);
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }

                  if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName).isArray();
                    if (result)
                    {
                      int lengthI = m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName).length();
                      for (int i = 0; (result) && (i < lengthI); ++i)
                      {
                        result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName)[i].isObject();
                        if (result)
                        {
                          if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName)[i].get(CRemoveName).isNull()))
                          {
                            result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName)[i].get(CRemoveName).isBoolean();
                          }

                          result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName)[i].get(COperationName).isString();
                          // TODO: Check with Business Analyst!!!
                          // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName)[i].get(COperationName).asString().empty());

                          if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName)[i].get(CPayloadName).isNull()))
                          {
                            result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName)[i].get(CPayloadName).isString(); //URL
                            // TODO: Check with Business Analyst!!!
                            // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CEventsName)[i].get(CPayloadName).asString().empty()); //URL
                          }
                        }
                      }
                    }
                  }

                  if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName).isNull()))
                  {
                    result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName).isArray();
                    if (result)
                    {
                      int lengthI = m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName).length();
                      for (int i = 0; (result) && (i < lengthI); ++i)
                      {
                        result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].isObject();
                        if (result)
                        {
                          if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CRemoveName).isNull()))
                          {
                            result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CRemoveName).isBoolean();
                          }

                          result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].get(COperationName).isString();
                          // TODO: Check with Business Analyst!!!
                          // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].get(COperationName).asString().empty());

                          if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CRequestName).isNull()))
                          {
                            result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CRequestName).isString(); //URL
                            // TODO: Check with Business Analyst!!!
                            // result = result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CRequestName).asString().empty()); //URL
                          }

                          if (result && (!m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CResponseName).isNull()))
                          {
                            result = result && m_json.get(CPayloadName).get(CAssociatedName)[h].get(CCmdsName)[i].get(CResponseName).isString(); //URL and can be empty
                          }
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

      UccMessageCloudDeviceConfigChange::TypeId UccMessageCloudDeviceConfigChange::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
