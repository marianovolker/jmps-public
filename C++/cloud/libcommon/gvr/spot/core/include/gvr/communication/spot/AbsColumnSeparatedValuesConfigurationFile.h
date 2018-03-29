/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsColumnSeparatedValuesConfigurationFile.h
 * @author Guillermo Paris
 * @date Friday, Jul 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef ABSCOLUMNSEPARATEDVALUESCONFIGURATIONFILE_H_
#define ABSCOLUMNSEPARATEDVALUESCONFIGURATIONFILE_H_

#include <string>


/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


  class AbsColumnSeparatedValuesConfigurationFile
  {
  public:

    /**
     * @brief The constructor taking 1 parameter.
     *
     * @param @b sConfFilename is the full pathname to the configuration file.
     */
    AbsColumnSeparatedValuesConfigurationFile();

    /**
     * @brief Return if this object is in error or not.
     */
    bool         hasErrors() { return m_blErrors; }

    /**
     * @brief Return the total number of records loaded from configuration file.
     */
    std::size_t  getTotalOfRecords() { return m_nRecords; }


  protected:

    static const unsigned int CMaxPlatformChars =  2; // Only 1 or 2 chars define platform ("m3" , "m5", "m7")

    static int   getFieldValue(std::string s);
    static int   getDigitValue(bool blHex, char c);
    bool         loadList( const std::string & sFilename, const char* pcFileUsage = NULL);
    virtual bool parseLine( const std::string & sLine ) = 0; // derived class responsibility

    bool         m_blErrors;
    std::size_t  m_nRecords;
  };





} // namespace spot

} // namespace communication

} // namespace gvr




#endif // ABSCOLUMNSEPARATEDVALUESCONFIGURATIONFILE_H_

