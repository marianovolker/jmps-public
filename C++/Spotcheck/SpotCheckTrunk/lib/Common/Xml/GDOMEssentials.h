/*
 * GDOMEssentials.h
 *
 *  Created on: Jun 24, 2011
 *      Author: pcantarini
 */

#ifndef GDOMEssentials_h
#define GDOMEssentials_h

#include <pugixml.hpp>

class GDOMXMLWriter;

/*! @class GDOMEssentials GDOMEssentials.h "Src/Common/Xml/GDOMEssentials.h"
 *  @brief This class holds all essential information shared between all GDOM XML classes.
 *
 * Is for internal usage. Includes information regarding parsing, formatting, encoding and the resulting node types
 * for the parsing process.
 */
class GDOMEssentials
{
public:
	static unsigned int const CMaxSearchDeph;	//!< Maximum tree search depth for some algorithms that require it.

	//! @enum etNodeType
	enum etNodeType
	{
		eNodeNull,			//!< Empty (null) node handle.
		eNodeDocument,		//!< A document tree's absolute root.
		eNodeElement,		//!< Element tag, i.e. '<node/>'.
		eNodePCData,		//!< Plain character data, i.e. 'text'.
		eNodeCData,			//!< Character data, i.e. '<![CDATA[text]]>'.
		eNodeComment,		//!< Comment tag, i.e. '<!-- text -->'.
		eNodePI,			//!< Processing instruction, i.e. '<?name?>'.
		eNodeDeclaration,	//!< Document declaration, i.e. '<?xml version="1.0"?>'.
		eNodeDoctype		//!< Document type declaration, i.e. '<!DOCTYPE doc>'.
	};

	//! @enum etEncoding
	enum etEncoding
	{
		eEncodingAuto,      //!< Auto-detect input encoding using BOM or < / <? detection; use UTF8 if BOM is not found.
		eEncodingUTF8,      //!< UTF8 encoding.
		eEncodingUTF16_le,  //!< Little-endian UTF16.
		eEncodingUTF16_be,  //!< Big-endian UTF16.
		eEncodingUTF16,     //!< UTF16 with native endianness.
		eEncodingUTF32_le,  //!< Little-endian UTF32.
		eEncodingUTF32_be,  //!< Big-endian UTF32.
		eEncodingUTF32,     //!< UTF32 with native endianness.
		eEncodingWChar      //!< The same encoding wchar_t has (either UTF16 or UTF32).
	};

	//! @name ParsingFlags Parsing Flags.
	//!@{
	static unsigned int const CParseMinimal;		//!< Minimal parsing mode (equivalent to turning all other flags off).
													//!< Only elements and PCDATA sections are added to the DOM tree, no text conversions are performed.

	static unsigned int const CParsePI;				//!< This flag determines if processing instructions (node_pi) are added to the DOM tree. This flag is off by default.

	static unsigned int const CParseComments;		//!< This flag determines if comments (node_comment) are added to the DOM tree. This flag is off by default.

	static unsigned int const CParseCData;			//!< This flag determines if CDATA sections (node_cdata) are added to the DOM tree. This flag is on by default.

	static unsigned int const CParseWSPCData;		//!< This flag determines if plain character data (node_pcdata) that consist only of whitespace are added to the DOM tree.
													//!< This flag is off by default; turning it on usually results in slower parsing and more memory consumption.

	static unsigned int const CParseEscapes;		//!< This flag determines if character and entity references are expanded during parsing. This flag is on by default.

	static unsigned int const CParseEOL;			//!< This flag determines if EOL characters are normalized (converted to #xA) during parsing. This flag is on by default.

	static unsigned int const CParseWConvAttribute;	//!< This flag determines if attribute values are normalized using CDATA normalization rules during parsing. This flag is on by default.

	static unsigned int const CParseWNormAttribute;	//!< This flag determines if attribute values are normalized using NMTOKENS normalization rules during parsing. This flag is off by default.

	static unsigned int const CParseDeclaration;	//!< This flag determines if document declaration (node_declaration) is added to the DOM tree. This flag is off by default.

	static unsigned int const CParseDoctype;		//!< This flag determines if document type declaration (node_doctype) is added to the DOM tree. This flag is off by default.

	static unsigned int const CParseDefault;		//!< The default parsing mode.
													//!< Elements, PCDATA and CDATA sections are added to the DOM tree, character/reference entities are expanded,
													//!< End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules.

	static unsigned int const CParseFull;			//!< The full parsing mode.
													//!< Nodes of all types are added to the DOM tree, character/reference entities are expanded,
													//!< End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules.
	//!@}

	//! @name FormattingFlags Formatting Flags.
	//!@{
	static unsigned int const CFormatIndent;		//!< Indent the nodes that are written to output stream with as many indentation strings as deep the node is in DOM tree. This flag is on by default.

	static unsigned int const CFormatWriteBOM;		//!< Write encoding-specific BOM to the output stream. This flag is off by default.

	static unsigned int const CFormatRaw;			//!< Use raw output mode (no indentation and no line breaks are written). This flag is off by default.

	static unsigned int const CFormatNoDeclaration;	//!< Omit default XML declaration even if there is no declaration in the document. This flag is off by default.

	static unsigned int const CFormatDefault;		//!< The default set of formatting flags.
													//!< Nodes are indented depending on their depth in DOM tree, a default declaration is output if document has none.
	//!@}

protected:
	/*! @brief Translates the Underneath Implementation DOM node type to the one that is used in the Gilbarco Framework.
	 *
	 * @param rhs Underneath Implementation DOM node type to be translated.
	 * @return Gilbarco Framework DOM node type.
	 */
	static etNodeType FromNodeTypeImplToGDOM (::pugi::xml_node_type rhs);
	/*! @brief Translates the Gilbarco Framework DOM node type to the one that is used in the Underneath Implementation.
	 *
	 * @param rhs Gilbarco Framework DOM node type to be translated.
	 * @return Underneath Implementation DOM node type.
	 */
	static ::pugi::xml_node_type FromNodeTypeGDOMToImpl (etNodeType rhs);
	/*! @brief Translates the Underneath Implementation DOM encoding type to the one that is used in the Gilbarco Framework.
	 *
	 * @param rhs Underneath Implementation DOM encoding type to be translated.
	 * @return Gilbarco Framework DOM encoding type.
	 */
	static etEncoding FromEncodingImplToGDOM (::pugi::xml_encoding rhs);
	/*! @brief Translates the Gilbarco Framework DOM encoding type to the one that is used in the Underneath Implementation.
	 *
	 * @param rhs Gilbarco Framework DOM encoding type to be translated.
	 * @return Underneath Implementation DOM encoding type.
	 */
	static ::pugi::xml_encoding FromEncodingGDOMToImpl (etEncoding rhs);
	/*! @brief Translates the Underneath Implementation DOM parsing flag to the one that is used in the Gilbarco Framework.
	 *
	 * @param rhs Underneath Implementation DOM parsing flag to be translated.
	 * @return Gilbarco Framework DOM parsing flag.
	 */
	static unsigned int FromParseFlagImplToGDOM (unsigned int rhs);
	/*! @brief Translates the Gilbarco Framework DOM parsing flag to the one that is used in the Underneath Implementation.
	 *
	 * @param rhs Gilbarco FrameworkDOM parsing flag to be translated.
	 * @return Underneath Implementation DOM parsing flag.
	 */
	static unsigned int FromParseFlagGDOMToImpl (unsigned int rhs);
	/*! @brief Translates the Underneath Implementation DOM formatting flag to the one that is used in the Gilbarco Framework.
	 *
	 * @param rhs Underneath Implementation DOM formatting flag to be translated.
	 * @return Gilbarco Framework DOM formatting flag.
	 */
	static unsigned int FromFormatFlagImplToGDOM (unsigned int rhs);
	/*! @brief Translates the Gilbarco Framework DOM formatting flag to the one that is used in the Underneath Implementation.
	 *
	 * @param rhs Gilbarco FrameworkDOM formatting flag to be translated.
	 * @return Underneath Implementation DOM formatting flag.
	 */
	static unsigned int FromFormatFlagGDOMToImpl (unsigned int rhs);

protected:
	/*! @class GDOMAttributeRef GDOMEssentials.h "Src/Common/Xml/GDOMEssentials.h"
	 * @brief This class holds reference information to the Underneath Implementation DOM attribute element.
	 *
	 * For internal usage.
	 */
	class GDOMAttributeRef
	{
	public:
		//! @brief Constructor.
		GDOMAttributeRef (void);
		/*! @brief Constructor.
		 *
		 * @param ref Reference to the Underneath Implementation DOM attribute element.
		 * @param pnt Reference to the Underneath Implementation DOM node element.
		 */
		GDOMAttributeRef (::pugi::xml_attribute const & ref, ::pugi::xml_node  const & pnt);
		//! @brief Destructor.
		~GDOMAttributeRef (void);
		/*! @brief Reference getter to the Underneath Implementation DOM attribute element.
		 *
		 * @return Reference to the Underneath Implementation DOM attribute element.
		 */
		::pugi::xml_attribute Referenced (void) const;
		/*! @brief Parent Reference getter to the Underneath Implementation DOM node element.
		 *
		 * @return Reference to the Parent Underneath Implementation DOM node element.
		 */
		::pugi::xml_node Parent (void) const;
	protected:
		::pugi::xml_attribute m_ref;	//!< Reference to the Underneath Implementation DOM attribute element.
		::pugi::xml_node m_pnt;			//!< Reference to the Underneath Implementation DOM node element.
	};
	/*! @class GDOMElementRef GDOMEssentials.h "Src/Common/Xml/GDOMEssentials.h"
	 * @brief This class holds reference information to the Underneath Implementation DOM node element.
	 *
	 * For internal usage.
	 */
	class GDOMElementRef
	{
	public:
		//! @brief Constructor.
		GDOMElementRef (void);
		/*! @brief Constructor.
		 *
		 * @param ref Reference to the Underneath Implementation DOM node element.
		 */
		GDOMElementRef (::pugi::xml_node const & ref);
		//! @brief Destructor.
		~GDOMElementRef (void);
		/*! @brief Reference getter to the Underneath Implementation DOM node element.
		 *
		 * @return Reference to the Underneath Implementation DOM node element.
		 */
		::pugi::xml_node Referenced (void) const;
	protected:
		::pugi::xml_node m_ref;	//!< Reference to the Underneath Implementation DOM node element.
	};
	/*! @class GDOMDocumentParseResultRef GDOMEssentials.h "Src/Common/Xml/GDOMEssentials.h"
	 * @brief This class holds reference information to the Underneath Implementation DOM parsing result.
	 *
	 * For internal usage.
	 */
	class GDOMDocumentParseResultRef
	{
	public:
		//! @brief Constructor.
		GDOMDocumentParseResultRef (void);
		/*! @brief Constructor.
		 *
		 * @param ref Reference to the Underneath Implementation DOM parsing result.
		 */
		GDOMDocumentParseResultRef (::pugi::xml_parse_result const & ref);
		//! @brief Destructor.
		~GDOMDocumentParseResultRef (void);
		/*! @brief Reference getter to the Underneath Implementation DOM parsing result.
		 *
		 * @return Reference to the Underneath Implementation DOM parsing result.
		 */
		::pugi::xml_parse_result Referenced (void) const;
	protected:
		::pugi::xml_parse_result m_ref;	//!< Reference to the Underneath Implementation DOM parsing result.
	};
	/*! @class GDOMXMLWriterPrivateImplProxy GDOMEssentials.h "Src/Common/Xml/GDOMEssentials.h"
	 * @brief This class holds reference information to the Underneath Implementation DOM xml writer observer.
	 *
	 * For internal usage.
	 */
	class GDOMXMLWriterPrivateImplProxy
		:	public ::pugi::xml_writer
	{
	public:
		//! @brief Constructor.
		GDOMXMLWriterPrivateImplProxy (GDOMXMLWriter * pRef);
		//! @brief Destructor.
		virtual ~GDOMXMLWriterPrivateImplProxy (void);
		/*! @brief DOM serializing observer callback method which works as a proxy between the user implementation and the Underneath Implementation.
		 *
		 * @param data Pointer to the buffer chunk to be dumped.
		 * @param size Size of the buffer chunk to be dumped.
		 */
		virtual void write(const void * data, size_t size);
	private:
		GDOMXMLWriter * m_pRef;	//!< Reference to the Underneath Implementation DOM xml writer observer.
	};

protected:
	//! @brief Constructor.
	GDOMEssentials (void);
	//! @brief Destructor.
	virtual ~GDOMEssentials (void);
};

#endif
