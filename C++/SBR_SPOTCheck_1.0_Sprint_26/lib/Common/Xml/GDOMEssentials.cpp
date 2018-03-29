/*
 * GDOMEssentials.cpp
 *
 *  Created on: Jun 29, 2011
 *      Author: pcantarini
 */

#include "Common/Xml/GDOMEssentials.h"
#include "Common/Xml/GDOMXMLWriter.h"

unsigned int const GDOMEssentials::CMaxSearchDeph = static_cast<unsigned int>(~static_cast<unsigned int>(0));

unsigned int const GDOMEssentials::CParseMinimal = ::pugi::parse_minimal;
unsigned int const GDOMEssentials::CParsePI = ::pugi::parse_pi;
unsigned int const GDOMEssentials::CParseComments = ::pugi::parse_comments;
unsigned int const GDOMEssentials::CParseCData = ::pugi::parse_cdata;
unsigned int const GDOMEssentials::CParseWSPCData = ::pugi::parse_ws_pcdata;
unsigned int const GDOMEssentials::CParseEscapes = ::pugi::parse_escapes;
unsigned int const GDOMEssentials::CParseEOL = ::pugi::parse_eol;
unsigned int const GDOMEssentials::CParseWConvAttribute = ::pugi::parse_wconv_attribute;
unsigned int const GDOMEssentials::CParseWNormAttribute = ::pugi::parse_wnorm_attribute;
unsigned int const GDOMEssentials::CParseDeclaration = ::pugi::parse_declaration;
unsigned int const GDOMEssentials::CParseDoctype = ::pugi::parse_doctype;
unsigned int const GDOMEssentials::CParseDefault = ::pugi::parse_default;
unsigned int const GDOMEssentials::CParseFull = ::pugi::parse_full;

unsigned int const GDOMEssentials::CFormatIndent = ::pugi::format_indent;
unsigned int const GDOMEssentials::CFormatWriteBOM = ::pugi::format_write_bom;
unsigned int const GDOMEssentials::CFormatRaw = ::pugi::format_raw;
unsigned int const GDOMEssentials::CFormatNoDeclaration = ::pugi::format_no_declaration;
unsigned int const GDOMEssentials::CFormatDefault = ::pugi::format_default;

GDOMEssentials::etNodeType GDOMEssentials::FromNodeTypeImplToGDOM (::pugi::xml_node_type rhs)
{
	GDOMEssentials::etNodeType res(GDOMEssentials::eNodeNull);
	switch (rhs)
	{
	case ::pugi::node_null:
		res = GDOMEssentials::eNodeNull;
		break;
	case ::pugi::node_document:
		res = GDOMEssentials::eNodeDocument;
		break;
	case ::pugi::node_element:
		res = GDOMEssentials::eNodeElement;
		break;
	case ::pugi::node_pcdata:
		res = GDOMEssentials::eNodePCData;
		break;
	case ::pugi::node_cdata:
		res = GDOMEssentials::eNodeCData;
		break;
	case ::pugi::node_comment:
		res = GDOMEssentials::eNodeComment;
		break;
	case ::pugi::node_pi:
		res = GDOMEssentials::eNodePI;
		break;
	case ::pugi::node_declaration:
		res = GDOMEssentials::eNodeDeclaration;
		break;
	case ::pugi::node_doctype:
		res = GDOMEssentials::eNodeDoctype;
		break;
	default:
		break;
	}
	return res;
}
::pugi::xml_node_type GDOMEssentials::FromNodeTypeGDOMToImpl (GDOMEssentials::etNodeType rhs)
{
	::pugi::xml_node_type res(::pugi::node_null);
	switch (rhs)
	{
	case GDOMEssentials::eNodeNull:
		res = ::pugi::node_null;
		break;
	case GDOMEssentials::eNodeDocument:
		res = ::pugi::node_document;
		break;
	case GDOMEssentials::eNodeElement:
		res = ::pugi::node_element;
		break;
	case GDOMEssentials::eNodePCData:
		res = ::pugi::node_pcdata;
		break;
	case GDOMEssentials::eNodeCData:
		res = ::pugi::node_cdata;
		break;
	case GDOMEssentials::eNodeComment:
		res = ::pugi::node_comment;
		break;
	case GDOMEssentials::eNodePI:
		res = ::pugi::node_pi;
		break;
	case GDOMEssentials::eNodeDeclaration:
		res = ::pugi::node_declaration;
		break;
	case GDOMEssentials::eNodeDoctype:
		res = ::pugi::node_doctype;
		break;
	default:
		break;
	}
	return res;
}
GDOMEssentials::etEncoding GDOMEssentials::FromEncodingImplToGDOM (::pugi::xml_encoding rhs)
{
	GDOMEssentials::etEncoding res(GDOMEssentials::eEncodingAuto);
	switch (rhs)
	{
	case ::pugi::encoding_auto:
		res = GDOMEssentials::eEncodingAuto;
		break;
	case ::pugi::encoding_utf8:
		res = GDOMEssentials::eEncodingUTF8;
		break;
	case ::pugi::encoding_utf16_le:
		res = GDOMEssentials::eEncodingUTF16_le;
		break;
	case ::pugi::encoding_utf16_be:
		res = GDOMEssentials::eEncodingUTF16_be;
		break;
	case ::pugi::encoding_utf16:
		res = GDOMEssentials::eEncodingUTF16;
		break;
	case ::pugi::encoding_utf32_le:
		res = GDOMEssentials::eEncodingUTF32_le;
		break;
	case ::pugi::encoding_utf32_be:
		res = GDOMEssentials::eEncodingUTF32_be;
		break;
	case ::pugi::encoding_utf32:
		res = GDOMEssentials::eEncodingUTF32;
		break;
	case ::pugi::encoding_wchar:
		res = GDOMEssentials::eEncodingWChar;
		break;
	default:
		break;
	}
	return res;
}
::pugi::xml_encoding GDOMEssentials::FromEncodingGDOMToImpl (GDOMEssentials::etEncoding rhs)
{
	::pugi::xml_encoding res(::pugi::encoding_auto);
	switch (rhs)
	{
	case GDOMEssentials::eEncodingAuto:
		res = ::pugi::encoding_auto;
		break;
	case GDOMEssentials::eEncodingUTF8:
		res = ::pugi::encoding_utf8;
		break;
	case GDOMEssentials::eEncodingUTF16_le:
		res = ::pugi::encoding_utf16_le;
		break;
	case GDOMEssentials::eEncodingUTF16_be:
		res = ::pugi::encoding_utf16_be;
		break;
	case GDOMEssentials::eEncodingUTF16:
		res = ::pugi::encoding_utf16;
		break;
	case GDOMEssentials::eEncodingUTF32_le:
		res = ::pugi::encoding_utf32_le;
		break;
	case GDOMEssentials::eEncodingUTF32_be:
		res = ::pugi::encoding_utf32_be;
		break;
	case GDOMEssentials::eEncodingUTF32:
		res = ::pugi::encoding_utf32;
		break;
	case GDOMEssentials::eEncodingWChar:
		res = ::pugi::encoding_wchar;
		break;
	default:
		break;
	}
	return res;
}
unsigned int GDOMEssentials::FromParseFlagImplToGDOM (unsigned int rhs)
{
	return rhs;
}
unsigned int GDOMEssentials::FromParseFlagGDOMToImpl (unsigned int rhs)
{
	return rhs;
}
unsigned int GDOMEssentials::FromFormatFlagImplToGDOM (unsigned int rhs)
{
	return rhs;
}
unsigned int GDOMEssentials::FromFormatFlagGDOMToImpl (unsigned int rhs)
{
	return rhs;
}

GDOMEssentials::GDOMAttributeRef::GDOMAttributeRef (void)
	:	m_ref(),
	 	m_pnt()
{
}
GDOMEssentials::GDOMAttributeRef::GDOMAttributeRef (::pugi::xml_attribute const & ref, ::pugi::xml_node  const & pnt)
	:	m_ref(ref),
		m_pnt(pnt)
{
}
GDOMEssentials::GDOMAttributeRef::~GDOMAttributeRef (void)
{
}
::pugi::xml_attribute GDOMEssentials::GDOMAttributeRef::Referenced (void) const
{
	return m_ref;
}
::pugi::xml_node GDOMEssentials::GDOMAttributeRef::Parent (void) const
{
	return m_pnt;
}

GDOMEssentials::GDOMElementRef::GDOMElementRef (void)
	:	m_ref()
{
}
GDOMEssentials::GDOMElementRef::GDOMElementRef (::pugi::xml_node const & ref)
	:	m_ref(ref)
{
}
GDOMEssentials::GDOMElementRef::~GDOMElementRef (void)
{
}
::pugi::xml_node GDOMEssentials::GDOMElementRef::Referenced (void) const
{
	return m_ref;
}

GDOMEssentials::GDOMDocumentParseResultRef::GDOMDocumentParseResultRef (void)
	:	m_ref()
{
}
GDOMEssentials::GDOMDocumentParseResultRef::GDOMDocumentParseResultRef (::pugi::xml_parse_result const & ref)
	:	m_ref(ref)
{
}
GDOMEssentials::GDOMDocumentParseResultRef::~GDOMDocumentParseResultRef (void)
{
}
::pugi::xml_parse_result GDOMEssentials::GDOMDocumentParseResultRef::Referenced (void) const
{
	return m_ref;
}

GDOMEssentials::GDOMXMLWriterPrivateImplProxy::GDOMXMLWriterPrivateImplProxy (GDOMXMLWriter * pRef)
	:	::pugi::xml_writer(),
		m_pRef(pRef)
{
}
GDOMEssentials::GDOMXMLWriterPrivateImplProxy::~GDOMXMLWriterPrivateImplProxy (void)
{
}
void GDOMEssentials::GDOMXMLWriterPrivateImplProxy::write(const void * data, size_t size)
{
	m_pRef->SerializedChunk(data,size);
}

GDOMEssentials::GDOMEssentials (void)
{
}
GDOMEssentials::~GDOMEssentials (void)
{
}
