/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotDeviceEventFilter.h
 * @author Guillermo Paris
 * @date Friday, Jul 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTDEVICEEVENTFILTER_H_
#define SPOTDEVICEEVENTFILTER_H_


#include <map>
#include <vector>
#include <string>

#include <gvr/communication/spot/AbsColumnSeparatedValuesConfigurationFile.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


  class SpotDeviceEventFilter : public AbsColumnSeparatedValuesConfigurationFile
  {
    static const char *        CpcConfigurationFile;
    static const unsigned int  CMainSytemCode          =  1;
    static const unsigned int  CMinDeviceNameLength    =  3; // 3 or more chars define file type name.
    static const unsigned int  CMinCharsInLine         = 12; // Minimum line length.

  public:

    /**
     * @enum EStatusCategory
     * @brief All of the possible categories of information yielded by a system status notification.
     */
    enum EStatusCategory
    {
      eInvalid, eStatus, eStatusEx, eError, eJobStatus, eTotalCategories = eJobStatus
    };

    /**
     * @struct DeviceEvent
     * @brief Information about a particular component event.
     */
    struct DeviceEvent
    {
      DeviceEvent() : b_AppId(0), b_CompCode(0), b_CategoryId(eStatus) {}

      DeviceEvent( unsigned char appId, unsigned char dc, unsigned char catId,
                   std::vector<unsigned char> & v )
        : b_AppId(appId), b_CompCode(dc), b_CategoryId(catId), v_AllowedValues(v) {}

      unsigned char b_AppId;      ///< @brief SPOT Application Id, reporter of the event.
      unsigned char b_CompCode;   ///< @brief SPOT device or component , reporter of the event.
      unsigned char b_CategoryId; ///< @brief Event category.

      std::vector<unsigned char>  v_AllowedValues; ///< @brief Set of all allowed values for the event value.
    };

    typedef std::map<std::string, DeviceEvent>  SupportedEventsMap;


    /**
     * @brief The constructor taking 2 parameters.
     *
     * @param @b sConfFilename is the full pathname to the configuration file having the list of all
     *                         possible events supported and enabled to be reported.
     * @param @b pcPlatform is the 2 char C string indicating the platform ("m5" is the default if it is NULL).
     */
    SpotDeviceEventFilter(std::string sConfFilename, const char* pcPlatform = NULL);

    /**
     * @brief Indicates if the supplied component event byte is supported by this device for cloud notification.
     *
     * @param sFileType is the text file type name to find out.
     *
     * @return @b true If this component event is supported and thus can be notified to cloud.
     *         @b false If it is not supported.
     */
    bool isSupportedEventByte( unsigned char uAppId, unsigned char uComponentId, EStatusCategory eCategory,
                               std::string & sComponentName, std::string & sCategoryName);

    /**
     * @brief Retrieves the list of all the supported components included in this OPT device.
     *
     * @param [Out] @b vout The output component name vector.
     *
     * @return @b true  If there was no errors retrieving the list from file system.
     *         @b false Otherwise.
     */
    bool getSupportedComponentNames( std::vector<std::string> & vout );

    static bool isDeviceEvent(unsigned char uDeviceId) { return (uDeviceId == CMainSytemCode); }

    static bool isComponentEvent(unsigned char uComponentId) { return (uComponentId != CMainSytemCode); }


  private:

    static const char *   CapcCategories[eTotalCategories];

    static   void         getComponentName( const std::string & sKeyname, std::string & sName );
    static   const char * getCategoryName( EStatusCategory eCategory );
    virtual bool          parseLine( const std::string & sLine );
    bool                  validateFirstField( const std::string & sFirstField, std::string & sKey );
    EStatusCategory       intToCategoryEnum( int i );

    const char *          m_pcPlatform;
    SupportedEventsMap    m_mapSupportedEvents;
  };





} // namespace spot

} // namespace communication

} // namespace gvr


#endif // SPOTDEVICEEVENTFILTER_H_

