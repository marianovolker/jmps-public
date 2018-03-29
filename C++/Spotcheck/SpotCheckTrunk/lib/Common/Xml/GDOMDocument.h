/*
 * GDOMDocument.h
 *
 *  Created on: Jun 24, 2011
 *      Author: pcantarini
 */

#ifndef GDOMDocument_h
#define GDOMDocument_h

#include <vector>
#include <string>
#include <GFileTypes.h>

#include "Common/Xml/GDOMEssentials.h"
#include "Common/Xml/GDOMXMLWriter.h"
#include "Common/Xml/GDOMElement.h"

/*! @class GDOMDocument GDOMDocument.h "Src/Common/Xml/GDOMDocument.h"
 * @brief This class represents a DOM document with its sub tree node.
 *
 */
class GDOMDocument
	:	public GDOMEssentials
{
public:
	/*! @class ParseResult GDOMDocument.h "Src/Common/Xml/GDOMDocument.h"
	 * @brief This class represents a DOM document parsing result status.
	 *
	 */
	class ParseResult
		:	public GDOMEssentials
	{
	public:
		//! @enum etNodeType
		enum etStatus
		{
			eStatusOk = 0,				//!< No error.
			eStatusFileNotFound,		//!< File was not found during Load().
			eStatusIOError,				//!< Error reading from file/stream.
			eStatusOutOfMemory,			//!< Could not allocate memory.
			eStatusInternalError,		//!< Internal error occurred.
			eStatusUnrecognizedTag,		//!< Parser could not determine tag type.
			eStatusBadPI,				//!< Parsing error occurred while parsing document declaration/processing instruction.
			eStatusBadComment,			//!< Parsing error occurred while parsing comment.
			eStatusBadCData,			//!< Parsing error occurred while parsing CDATA section.
			eStatusBadDoctype,			//!< Parsing error occurred while parsing document type declaration.
			eStatusBadPCData,			//!< Parsing error occurred while parsing PCDATA section.
			eStatusBadStartElement,		//!< Parsing error occurred while parsing start element tag.
			eStatusBadAttribute,		//!< Parsing error occurred while parsing element attribute.
			eStatusBadEndElement,		//!< Parsing error occurred while parsing end element tag.
			eStatusEndElementMismatch,	//!< There was a mismatch of start-end tags (closing tag had incorrect name, some tag was not closed or there was an excessive closing tag).
			eStatusUnknownError			//!< Unknown error.
		};
	public:
		//! @brief Constructor.
		ParseResult (void);
		/*! @brief Constructor.
		 *
		 * @param rhs Internal reference to a DOM document parse result.
		 */
		ParseResult (GDOMDocumentParseResultRef const & rhs);
		//! @brief Destructor.
		~ParseResult (void);
		/*! @brief Implicit getter to the reference of the internal handling of the user DOM document parse result.
		 *
		 * @return Reference to the internal handling of the user DOM document parse result.
		 */
		operator GDOMDocumentParseResultRef const & (void) const;
		/*! @brief Obtain current XML parsing status result.
		 *
		 * @return Parsing status.
		 */
		etStatus Status (void) const;
		/*! @brief Obtain current XML offset where parsing has stopped.
		 *
		 * @return Parsing offset.
		 */
		signed int Offset (void) const;
		/*! @brief Obtain current XML encoding used while parsing.
		 *
		 * @return Parsing encoding.
		 */
		etEncoding Encoding (void) const;
		/*! @brief Obtain current XML parsing status description.
		 *
		 * @return Parsing status description.
		 */
		std::string Description(void) const;

		/*! @brief Obtain current XML parsing success.
		 *
		 * @return True if parsing status result is eStatusOk.
		 */
		operator bool (void) const;

	private:
		GDOMDocumentParseResultRef m_ref;	//!< Internal reference to the DOM document parse result.
	};

public:
	//! @brief Constructor.
	GDOMDocument (void);
	/*! @brief Copy Constructor.
	 *
	 * @param rhs reference to a DOM document.
	 */
	GDOMDocument (GDOMDocument const & rhs);
	//! @brief Destructor.
	virtual ~GDOMDocument (void);
	/*! @brief Assigment operator.
	 *
	 * @param rhs Document to be copied from.
	 * @return Current document.
	 */
	GDOMDocument & operator = (GDOMDocument const & rhs);
	/*! @brief Obtain first child node element which node type is eNodeElement.
	 *
	 * @return First element node found. Empty element if there are no child element nodes for this document.
	 */
	GDOMElement DocumentElement (void) const;
	/*! @brief Obtain current document representation as node element.
	 *
	 * @return Current document representation as node element.
	 */
	GDOMElement AsRootElement (void) const;
	//! @brief Clear document erasing all its content.
    void Reset (void);
    /*! @brief Clear document erasing all its content and replacing it with a copy of the content of the given document.
     *
     * @param rhs Document to be copied from.
     */
	void Reset (GDOMDocument const & rhs);
	/*! @brief Parse the given XML buffer generating the corresponding DOM tree if applies.
	 *
	 * @param sContents String repository which contains the XML document to be parsed.
	 * @param uiParse Parse flags used to perform the parsing.
	 * @return Document parse status result.
	 */
	ParseResult Load (std::string sContents, unsigned int uiParse = GDOMDocument::CParseDefault);
	/*! @brief Parse the given XML buffer generating the corresponding DOM tree if applies.
	 *
	 * @param vContents Array repository which contains the XML document to be parsed.
	 * @param uiParse Parse flags used to perform the parsing.
	 * @param evEncoding Encoding used to perform the parsing.
	 * @return Document parse status result.
	 */
	ParseResult Load (std::vector<unsigned char> vContents, unsigned int uiParse = GDOMDocument::CParseDefault, etEncoding evEncoding = GDOMDocument::eEncodingAuto);
	/*! @brief Parse the given XML buffer generating the corresponding DOM tree if applies.
	 *
	 * @param pkContents Buffer pointer to the repository address which contains the XML document to be parsed.
	 * @param uiSize Size of the buffer to be parsed.
	 * @param uiParse Parse flags used to perform the parsing.
	 * @param evEncoding Encoding used to perform the parsing.
	 * @return Document parse status result.
	 */
	ParseResult LoadFromRawBuffer (void const * pkContents, unsigned int uiSize, unsigned int uiParse = GDOMDocument::CParseDefault, etEncoding evEncoding = GDOMDocument::eEncodingAuto);
	/*! @brief Serialize DOM tree starting from the root document node.
	 *
	 * @param sContents	String repository which will held the serialized node subtree.
	 * @param sIndent Indent string used to give hierarchy levels to the output.
	 * @param uiFormat Format flags used to perform the serialization.
	 */
	void Serialize (std::string & sContents, std::string sIndent = "\t", unsigned int uiFormat = GDOMDocument::CFormatDefault) const;
	/*! @brief Serialize DOM tree starting from the root document node.
	 *
	 * @param vContents	Array repository which will held the serialized node subtree.
	 * @param sIndent Indent string used to give hierarchy levels to the output.
	 * @param uiFormat Format flags used to perform the serialization.
	 * @param evEncoding Encoding used to perform the serialization.
	 */
	void Serialize (std::vector<unsigned char> & vContents, std::string sIndent = "\t", unsigned int uiFormat = GDOMDocument::CFormatDefault, etEncoding evEncoding = GDOMDocument::eEncodingAuto) const;
	/*! @brief Serialize DOM tree starting from the root document node.
	 *
	 * @param oWriter User defined serialization observer which will receive the chunk of serialized buffers to be stored in any way the user seems best to fit it needs.
	 * @param sIndent Indent string used to give hierarchy levels to the output.
	 * @param uiFormat Format flags used to perform the serialization.
	 * @param evEncoding Encoding used to perform the serialization.
	 */
	void SerializeToObserver (GDOMXMLWriter & oWriter, std::string sIndent = "\t", unsigned int uiFormat = GDOMDocument::CFormatDefault, etEncoding evEncoding = GDOMDocument::eEncodingAuto) const;
	/*! @brief Load the given XML file name generating the corresponding DOM tree if applies.
	 *
	 * @param sFileName File name of the XML document to be parsed.
	 * @param uiParse Parse flags used to perform the parsing.
	 * @param evEncoding Encoding used to perform the parsing.
	 * @return Document parse status result.
	 */
	ParseResult LoadFromFile (std::string sFileName, unsigned int uiParse = GDOMDocument::CParseDefault, etEncoding evEncoding = GDOMDocument::eEncodingAuto);
	/*! @brief Save DOM tree the given XML file name.
	 *
	 * @param sFileName File name of the XML document to be serialized.
	 * @param evOpenMode File open mode to use.
	 * @param sIndent Indent string used to give hierarchy levels to the output.
	 * @param uiFormat Format flags used to perform the serialization.
	 * @param evEncoding Encoding used to perform the serialization.
	 * @return True if was able to save the file with its XML content.
	 */
	bool SerializeToFile(std::string sFileName, eFileOpenMode evOpenMode, std::string sIndent = "\t", unsigned int uiFormat = GDOMDocument::CFormatDefault, etEncoding evEncoding = GDOMDocument::eEncodingAuto) const;

private:
	::pugi::xml_document m_ref;	//!< Internal reference to the DOM document.
};

#endif
