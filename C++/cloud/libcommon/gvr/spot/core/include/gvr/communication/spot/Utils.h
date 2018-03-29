/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file Utils.h
 * @author Guillermo Paris
 * @date Monday, March 03, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <gvr/communication/spot/ISpotMessage.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


/**
 * @brief This class is a STATIC one that encompasses all the miscellaneous stand alone functions in spot module.
 *        The aim of this class is stand alone function grouping.
 * @note  Everyone is invited to populate miscellaneous stand alone functions here, provided that these are used inside
 *        the spot module..
 *
 * @since 0.0.0, 2015-03-03
 * @version 0.0.0, 2015-03-03
 * @author Guillermo Paris
 */
class Utils
{
public:

    static const char * CUp1RightNNThenEraseToEOL;///< @brief To erase an invalid blank input line;
    /**
     * @brief Says if the given ascii char is a "blank character" or not.
     */
	static bool isBlankChar( char c );

    /**
     * @brief Says if the given null terminated C string has all of its characters "blank" or not.
     */
    static bool isBlankString( const char* pc);

    /**
     * @brief Says if the given C++ string has all of its characters "blank" or not.
     */
    static bool isBlankString( const std::string & s );

    /**
     * @brief Gets a complete line string from the standard input.
     * @note The console blocks the calling thread until the complete line is entered. An "enter"
     *       or new line character finishes the input operation.
     *       A blank input string is discarded and the operation begins again until a valid non blank
     *       string is entered.
     *
     * @param [In]  sLabelPrompt is a prompt on the left to guide the user input.
     * @param [Out] sOut  The non blank string entered is returned via this parameter.
     */
    static void getStringFromConsole( const std::string & sLabelPrompt, std::string & sOut );


    /**
     * @brief Says if the full pathname to a file, given by a null terminated C string, exists or not.
     */
    static bool canAccess(const char* pc);

    /**
     * @brief Says if the full pathname to a file, given by a C++ string, exists or not.
     */
    static bool canAccess(const std::string & s);

   /**
    * @brief dumpBuffer dumps a binary pbInputBuffer to the supplied output character buffer.
    * @note The use of this static method is only meant for DEBUGGING/TRACING purpose.
    *       This method DOES NOT CHECK THE SIZE of the output buffer. Use this carefully.
    *
    * @param [In|Out] @b pcOutputBuffer is the output char buffer, provided with enough capacity given by (3 * length + 2).
    * @param [In]     @b pbInputBuffer is the byte array you want to convert in a readable sequence of hex characters.
    * @param [In]     @b length is the length of the byte array.
    *
    * @return The same char buffer passed as first parameter.
    *
    * @since 0.0.0
    */
    static const char* dumpBuffer(char* pcOutputBuffer,
                                  unsigned const char* pbInputBuffer, ISpotMessage::SizeType length);

   /**
    * @brief dumpBuffer dumps a binary pbInputBuffer to the supplied output character buffer.
    * @note The use of this static method is only meant for DEBUGGING/TRACING purpose.
    *       This method DOES NOT CHECK THE SIZE of the output buffer. Use this carefully.
    *
    * @param [In|Out] @b pcOutputBuffer is the output char buffer, provided with enough capacity given by (3 * length + 2).
    * @param [In]     @b inputBuffer is the byte vector you want to convert in a readable sequence of hex characters.
    * @param [In]     @b length is the length of the byte array.
    *
    * @return The same char buffer passed as first parameter.
    *
    * @since 0.0.0
    */
    static const char* dumpBuffer(char* pcOutputBuffer, const ISpotMessage::BufferType& inputBuffer);


};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* UTILS_H_ */
