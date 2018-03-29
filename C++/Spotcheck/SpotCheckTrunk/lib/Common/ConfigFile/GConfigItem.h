#ifndef GConfigItem_h
#define GConfigItem_h

#include "GConfigTypes.h"
#include <list>
#include <map>
#include <string>

/*!
 * \class [GConfigItem]
 *
 * \brief [Single Block of Data]
 *
 * [A class containing a single XML Node's data including any attributes]
 *
 * \author [Shawn Barber]
 * \date 9Jul09
 */



class GConfigItem
{
public:
   GConfigItem(std::string Name, std::string Value, std::string XMLPath, std::map<std::string, std::string> Attributes);
   ~GConfigItem();

   /*!
    * \brief [Returns the value of an attribute]
    *
    * \param[in] [Name] [Name of the attribute to retrieve]
    * \param[out] [Value] [Value of the attribute]
    * \return [Config Status]
    * \note [Name is case insensitive and if the name doesn't exist an error code "eConfigError" will be returned in the status]
    */
   GConfigTypes::eConfigStatusType  GetAttribute(std::string Name, std::string & Value);

   /*!
    * \brief [Returns the name of this config item]
    *
    * \return [Name of the config item]
    */
   std::string  GetName();

   /*!
    * \brief [Returns the value of this config item]
    *
    * \return [Value of the config item as a string]
    */
   std::string  GetValue();

   /*!
    * \brief [Returns the XML path of this config item]
    *
    * \return [XML path of this config item]
    */
   std::string  GetXMLPath();

   /*!
    * \brief [Sets a new value for this config item]
    *
    * \param[in] [Value] [New value for this config item]
    */
   void SetValue(std::string Value);

   /*!
    * \brief [equality operator]
    *
    * \param[in] [Right] [Other config item to compare against]
    * \return [Boolean: True - Identical, False - Not Identical]
    */
   bool operator == (const GConfigItem& Right);

   /*!
    * \brief [Retrieves a list of attribute names]
    *
    * \param[out] [AttribList] [List of Attribute names]
    * \note [If this config item has no attributes, the list will be empty (size = 0)]
    */
   void GetAttributesIndex(std::list<std::string>& AttribList);

private:
   GConfigItem();

   std::string m_Name;
   std::string m_Value;
   std::string m_XMLPath;

   std::map<std::string, std::string> m_Attributes;
};

#endif
