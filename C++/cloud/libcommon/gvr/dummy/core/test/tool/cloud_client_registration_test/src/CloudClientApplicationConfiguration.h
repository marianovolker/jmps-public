/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudClientApplicationConfiguration.h
 * @author gparis
 * @date Tuesday, September 16, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CLOUD_CLIENT_APPLICATION_CONFIGURATION_H_
#define _CLOUD_CLIENT_APPLICATION_CONFIGURATION_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

/**
 * Global Immutable definitions
 * ============================
 *
 * @brief This section is intended for global constants.
 *
 */

const int   DEFAULT_FOO_VALUE = 0;
extern const char* DEFAULT_FOO_NAME;

/**
 * @class CloudClientApplicationConfiguration
 *
 * @brief  This class is designed to fully contain all the configuration
 *         variables and its default values.
 *
 * @author Guillermo Paris
 * @date   2014-09-16
 * @since  0.0.0
 */
class CloudClientApplicationConfiguration
{
  public:

   /** @brief Default Constructor */
    CloudClientApplicationConfiguration();

   /**
    * @brief Constructor taking command line parameters.
    *
    * @param int nParams  Total number of parameters (parameter array length).
    * @param char*[] apcParameters  Array of parameters (char* type each one).
    */
    CloudClientApplicationConfiguration(int nParams, const char* *apcParameters);

    /**
     * @brief Gets all the command line parameters as a char* array, and also the number of them.
     * @param [Out] nParams  The total number of command line parameters (array dimension).
     * @return Array of parameters (array of char*).
     */
    const char*  *getCommandLineParameters(int& nParams);

    /**
     * @brief Gets one command line parameters determined by its numerical position.
     *
     * @param nPos  The numerical position in command line of the parameter to be retrieved.
     * @return      The selected parameter as a char*
     */
    const char*   getCommandLineParameter(int nPos);

    /**
     * @brief Gets the foo!  A parameter combination of a number and a legend (char *)
     *
     * @param iFoo [Out] The foo number.
     * @return The foo legend as a char*
     */
    const char*   getFoo(int& iFoo);

    /**
     * @brief Gets the default values for foo!
     *
     * Gets a constant combination of a number and its legend (char *)
     *
     * @param iFoo [Out] The default value for the foo number.
     * @return The default foo legend as a const char*
     */
    static const char*  getDefaultFoo(int& iFoo);

    /**
     * @brief Sets all the command line parameters as a char* array, and also the number of them.
     *
     * @param nParams       The total number of command line parameters (array dimension).
     * @param apcParameters Array of parameters (array of char*).
     */
    void          setCommandLineParameters(int nParams, const char* *apcParameters);

    /**
     * @brief Sets one command line parameters determined by its numerical position.
     *
     * @param nPos        The numerical position in command line of the parameter to be retrieved.
     * @param pcParameter The selected parameter as a char*
     */
    void          setCommandLineParameter(int nPos, const char* pcParameter);

    /**
     * @brief Sets the foo!
     * Sets a parameter combination of a number and its legend (char *)
     *
     * @param iFoo      The foo number.
     * @param pcFooName The foo legend as a char*
     */
    void          setFoo(int iFoo, const char* pcFooName);

    /**
     * @brief Loads the site registration request configuration
     * @param filename, the configuration file name
     * @return true if the loading was successful, false in other case.
     */
    bool          loadSiteRegistrationConfig(const char* filename);

    /**The constant value for Device Registration Request configuration key*/
    static const std::string CDeviceRegistrationRequestConfigKey;
    /**The constant value for the Register Device configuration key*/
    static const std::string CRegisterDeviceConfigKey;
    /**The constant value for the Registration Code configuration key*/
    static const std::string CRegistrationCodeConfigKey;
    /**The constant value for the Cert. Format configuration key*/
    static const std::string CCertFormatConfigKey;
    /**The constant value for the Dns Supported configuration key*/
    static const std::string CDnsSupportedConfigKey;
    /**The constant value for the Device configuration key*/
    static const std::string CDeviceConfigConfigKey;
    /**The constant value for the Device Type configuration key*/
    static const std::string CDeviceTypeConfigKey;
    /**The constant value for the Local Id configuration key*/
    static const std::string CLocalIdConfigKey;
    /**The constant value for the Asset Info configuration key*/
    static const std::string CAssetInfoConfigKey;
    /**The constant value for the Manufacturer configuration key*/
    static const std::string CManufacturerConfigKey;
    /**The constant value for the Model configuration key*/
    static const std::string CModelConfigKey;
    /**The constant value for the Serial Nbr configuration key*/
    static const std::string CSerialNbrConfigKey;
    /**The constant value for the Firmware configuration key*/
    static const std::string CFirmwareConfigKey;
    /**The constant value for the Commponents configuration key*/
    static const std::string CComponentsConfigKey;
    /**The constant value for the Events configuration key*/
    static const std::string CEventsKey;
    /**The constant value for the Event Name configuration key*/
    static const std::string CEventNameConfigKey;
    /**The constant value for the Payload configuration key*/
    static const std::string CPayloadConfigKey;
    /**The constant value for the Commands configuration key*/
    static const std::string CMSDSConfigKey;
    /**The constant value for the Command configuration key*/
    static const std::string CCmdConfigKey;
    /**The constant value for the Command Name configuration key*/
    static const std::string CCmdNameConfigKey;
    /**The constant value for the Command Request configuration key*/
    static const std::string CCmdRequestConfigKey;
    /**The constant value for the Command Response configuration key*/
    static const std::string CCmdResponseConfigKey;
    /**The constant value for the Associated configuration key*/
    static const std::string CAssociatedConfigKey;

  private:

    static const int    mst_CiFoo = 0;   ///< default foo number.
    static const char*  mst_CpcFooName;  ///< default foo legend.

    /** The tree containing all the site registration data*/
    boost::property_tree::ptree m_siteRegistrationDataTree;

    int           m_iFoo;               ///< foo number.
    const char*   m_pcFooName;          ///< foo legend.
    int           m_iLineParamQty;      ///< number of command line parameters set.
    const char*  *m_apcLineParameters;  ///< Array of command line parameters (array of char*).


};

#endif

