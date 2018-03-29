#include "GConfigItem.h"
#include <Common/Util/DEBUGOUT.h>
#include <algorithm>
#include <cctype>

using namespace GConfigTypes;


GConfigItem::GConfigItem() {}



GConfigItem::GConfigItem(std::string Name, std::string Value, std::string XMLPath, std::map<std::string, std::string> Attributes) :
   m_Name(Name), m_Value(Value), m_XMLPath(XMLPath), m_Attributes(Attributes) {}



GConfigItem::~GConfigItem() {}



eConfigStatusType GConfigItem::GetAttribute(std::string Name, std::string & Value)
{
   eConfigStatusType ConfigStatus(eConfigOk);

   std::map<std::string, std::string>::const_iterator Iterator = m_Attributes.find(Name);

   if(Iterator == m_Attributes.end())
   {
      ConfigStatus = eConfigError;
      LOG(eConfig, eError, "Attribute [%s] does not exist\n",Name.c_str());
   }
   else
   {
      Value = Iterator->second;
   }

   return ConfigStatus;
}



std::string  GConfigItem::GetName()
{
   return m_Name;
}



std::string  GConfigItem::GetValue()
{
   return m_Value;
}



std::string  GConfigItem::GetXMLPath()
{
   return m_XMLPath;
}



void GConfigItem::SetValue(std::string Value)
{
   m_Value = Value;
}



bool GConfigItem::operator == (const GConfigItem& Right)
{
   return ((m_Name       == Right.m_Name)    &&
           (m_Value      == Right.m_Value)   &&
           (m_XMLPath    == Right.m_XMLPath) &&
           (m_Attributes == Right.m_Attributes));
}



void GConfigItem::GetAttributesIndex(std::list<std::string>& AttribList)
{
   std::map<std::string, std::string>::const_iterator Iterator;
   if(!m_Attributes.empty())
   {
      for(Iterator=m_Attributes.begin();Iterator != m_Attributes.end();++Iterator)
      {
         AttribList.push_back(Iterator->first);
      }
   }
}

