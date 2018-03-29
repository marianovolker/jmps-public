#ifndef GConfigUtils_h
#define GConfigUtils_h

#include <string>
#include <list>

/*!
 * \class [GConfigUtils]
 *
 * \brief [Utilities used in the ConfigSys]
 *
 * [Common utility methods used in the ConfigSys]
 *
 * \author [Shawn Barber]
 * \date 9Jul09
 */



class GConfigUtils
{
public:

   /*!
    * \brief [Splits a string delimted by s8Delim and returns a list of it's substrings]
    *
    * \param[in] [Value] [String value to be split]
    * \param[in] [s8Delim] [Delimiting character]
    * \param[out] [Results] [A List of strings separated by the delimiting character]
    */
   static void StringSplit(std::string Value, char s8Delim, std::list<std::string>& Results);

   /*!
    * \brief [Joins a list of strings inserting a delimiting character between them]
    *
    * \param[in] [Parts] [A list of strings to be joined]
    * \param[in] [s8Delim] [Delimiting character]
    * \return [The joined string]
    */
   static std::string CombinedString(std::list<std::string> Parts, char s8Delim);

   /*!
    * \brief [Trims whitespace from the front and back of a string (Value)]
    *
    * \param[in] [Value] [String value to be trimmed]
    * \return [The trimmed string]
    */
   static std::string Trim (std::string Value);
   
   /*!
    * \brief [Trims whitespace from the front of a string (Value)]
    *
    * \param[in] [Value] [String value to be trimmed]
    * \return [The trimmed string]
    */
   static std::string LTrim(std::string Value);
   
   /*!
    * \brief [Trims whitespace from the back of a string (Value)]
    *
    * \param[in] [Value] [String value to be trimmed]
    * \return [The trimmed string]
    */
   static std::string RTrim(std::string Value);
   
   /*!
    * \brief [Retrieves the uppercase of a character]
    *
    * \param[in] [Char] [Char to set to convert to its uppercase]
    * \return [Char turned to uppercase]
    */
   static int char2upper(int c);

   /*!
    * \brief [Replaces a string (Old) with another string (New) in the target string (TheString), the replace is case sensitive forward searching.]
    *
    * \param[inout] [TheString] [Target string]
    * \param[in] [Old] [String value being replaced]
    * \param[in] [New] [String replacing Old]
    */
   static void SFReplace(std::string& TheString, std::string Old, std::string New);
   
   /*!
    * \brief [Replaces a string (Old) with another string (New) in the target string (TheString), the replace is case insensitive forward searching.]
    *
    * \param[inout] [TheString] [Target string]
    * \param[in] [Old] [String value being replaced]
    * \param[in] [New] [String replacing Old]
    */
   static void IFReplace(std::string& TheString, std::string Old, std::string New);
   
   /*!
    * \brief [Replaces a string (Old) with another string (New) in the target string (TheString), the replace is case sensitive reverse searching.]
    *
    * \param[inout] [TheString] [Target string]
    * \param[in] [Old] [String value being replaced]
    * \param[in] [New] [String replacing Old]
    */
   static void SRReplace(std::string& TheString, std::string Old, std::string New);
   
   /*!
    * \brief [Replaces a string (Old) with another string (New) in the target string (TheString), the replace is case insensitive reverse searching.]
    *
    * \param[inout] [TheString] [Target string]
    * \param[in] [Old] [String value being replaced]
    * \param[in] [New] [String replacing Old]
    */
   static void IRReplace(std::string& TheString, std::string Old, std::string New);

};

#endif
