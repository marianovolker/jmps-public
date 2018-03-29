#ifndef GXMLParser_h
#define GXMLParser_h

#include <list>
#include <string>
#include "GConfigTypes.h"
#include "GConfigItem.h"

/*!
 * \class [GXMLParser]
 *
 * \brief [Parses XML Documents]
 *
 * [Parses XML Documents, creating GConfigItems as it encounters values]
 *
 * \author [Shawn Barber]
 * \date 9Jul09
 */


class GXMLParser
{
public:
   GXMLParser (std::string File, std::list<GConfigItem>& ConfigItemList);
   ~GXMLParser (void);

   /*!
    * \brief [Loads a file, returning the parsed data]
    *
    * \return [Config Status]
    * \note [Name is case insensitive and if the name doesn't exist an error code "eConfigError" will be returned in the status]
    */
   GConfigTypes::eConfigStatusType Load (void);

private:
   // Prevent copy constructor and assignment usage.
   GXMLParser(const GXMLParser & rhs);
   const GXMLParser& operator=(const GXMLParser & rhs);

   std::list<GConfigItem>*            m_pConfigItemList;
   std::string                        m_cFile;
};

#endif
