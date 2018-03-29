#ifndef GXMLWriter_h
#define GXMLWriter_h

#include "GConfigItem.h"
#include "GConfigTypes.h"
#include <list>
#include <string>

/*!
 * \class [GXMLWriter]
 *
 * \brief [Writes XML Documents]
 *
 * [Write XML Document containing data passed in as GConfigItems]
 *
 * \author [Shawn Barber]
 * \date 9Jul09
 */


class GXMLWriter
{
public:
   GXMLWriter (std::string sFile);
   ~GXMLWriter (void);

   /*!
    * \brief [Adds a config item to the file for output]
    *
    * \param[in] [ConfigItem] [The config item to be added]
    */
   void AddConfigItem (GConfigItem ConfigItem);

   /*!
    * \brief [Writes the XML file]
    * \return [Config Status]
    */
   GConfigTypes::eConfigStatusType Save (void);

private:
   // Prevent copy constructor and assignment usage.
   GXMLWriter (const GXMLWriter & rhs);
   const GXMLWriter& operator= (const GXMLWriter & rhs);

   typedef std::multimap<std::string, GConfigItem> DocMapType;

   std::string                             m_File;
   std::multimap<std::string, GConfigItem> m_DocMap;
};

#endif
