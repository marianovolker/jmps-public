/*
 * GDOMElement.cpp
 *
 *  Created on: Jun 29, 2011
 *      Author: pcantarini
 */
#include <pugixml.hpp>

#include "Common/Xml/GDOMElement.h"

class GDOMElementXMLWriterPrivate
	:	public GDOMXMLWriter
{
public:
	virtual ~GDOMElementXMLWriterPrivate (void);
	virtual void SerializedChunk (void const * vkpContents, unsigned int uiSize);
	std::vector<unsigned char> & SerializedBuffer(void);
	std::vector<unsigned char> const & SerializedBuffer(void) const;
private:
	std::vector<unsigned char> m_vSerializedBuffer;
};

GDOMElementXMLWriterPrivate::~GDOMElementXMLWriterPrivate (void)
{
}
void GDOMElementXMLWriterPrivate::SerializedChunk (void const * vkpContents, unsigned int uiSize)
{
	m_vSerializedBuffer.insert(m_vSerializedBuffer.end(),static_cast<unsigned char const *>(vkpContents),(static_cast<unsigned char const *>(vkpContents)+uiSize));
}
std::vector<unsigned char> & GDOMElementXMLWriterPrivate::SerializedBuffer(void)
{
	return m_vSerializedBuffer;
}
std::vector<unsigned char> const & GDOMElementXMLWriterPrivate::SerializedBuffer(void) const
{
	return m_vSerializedBuffer;
}

GDOMPredicateForElement::~GDOMPredicateForElement (void)
{
}

GDOMElement::GDOMElement (void)
	:	GDOMEssentials(),
	 	m_ref()

{
}
GDOMElement::GDOMElement (GDOMEssentials::GDOMElementRef const & rhs)
	:	GDOMEssentials(),
	 	m_ref(rhs)
{
}
GDOMElement::~GDOMElement (void)
{
}
GDOMElement::operator GDOMEssentials::GDOMElementRef  const & (void) const
{
	return m_ref;
}
bool GDOMElement::operator  ! (void) const
{
	return !(m_ref.Referenced());
}
bool GDOMElement::operator == (GDOMElement const & rhs) const
{
	return (m_ref.Referenced() == static_cast<GDOMEssentials::GDOMElementRef const &>(rhs).Referenced());
}
bool GDOMElement::operator != (GDOMElement const & rhs) const
{
	return (m_ref.Referenced() != static_cast<GDOMEssentials::GDOMElementRef const &>(rhs).Referenced());
}
bool GDOMElement::Empty (void) const
{
	return m_ref.Referenced().empty();
}
std::string GDOMElement::Name (void) const
{
	return static_cast<char const *>(m_ref.Referenced().name());
}
bool GDOMElement::Name (std::string const & rhs)
{
	return m_ref.Referenced().set_name(rhs.c_str());
}
std::string GDOMElement::Value (void) const
{
	return static_cast<char const *>(m_ref.Referenced().value());
}
bool GDOMElement::Value (std::string const & rhs)
{
	return m_ref.Referenced().set_value(rhs.c_str());
}
GDOMElement::etNodeType GDOMElement::Type (void) const
{
	return GDOMEssentials::FromNodeTypeImplToGDOM(m_ref.Referenced().type());
}
GDOMElement GDOMElement::Root (void) const
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().root());
}
GDOMElement GDOMElement::Parent (void) const
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().parent());
}
GDOMElement GDOMElement::Next (void) const
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().next_sibling());
}
GDOMElement GDOMElement::Previous (void) const
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().previous_sibling());
}
GDOMElement GDOMElement::Next (std::string const & rhs) const
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().next_sibling(rhs.c_str()));
}
GDOMElement GDOMElement::Previous (std::string const & rhs) const
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().previous_sibling(rhs.c_str()));
}
std::string GDOMElement::FirstChildValue (void) const
{
	std::string sResult;
	{
		::pugi::xml_node i(m_ref.Referenced().first_child());
		while ((i.empty() == false) && (i.type() != ::pugi::node_pcdata) && (i.type() != ::pugi::node_cdata))
		{
			i = i.next_sibling();
		}
		if (i.empty() == false)
		{
			sResult = static_cast<char const *>(i.value());
		}
	}
	return sResult;
}
void GDOMElement::FirstChildValue (std::string const & rhs, bool bAsPCData)
{
	::pugi::xml_node i(m_ref.Referenced().first_child());
	::pugi::xml_node_type evNodeDataType(((bAsPCData == true)?(::pugi::node_pcdata):(::pugi::node_cdata)));
	while ((i.empty() == false) && (i.type() != evNodeDataType))
	{
		i = i.next_sibling();
	}
	if (i.empty() == true)
	{
		i = m_ref.Referenced().prepend_child(evNodeDataType);
	}
	if (i.empty() == false)
	{
		i.set_value(rhs.c_str());
	}
}
std::list<std::string> GDOMElement::ChildValue (void) const
{
	std::list<std::string> lResult;
	for (::pugi::xml_node i(m_ref.Referenced().first_child()); i.empty() == false; i = i.next_sibling())
	{
		if ((i.type() == ::pugi::node_pcdata) || (i.type() == ::pugi::node_cdata))
		{
			lResult.push_back(static_cast<char const *>(i.value()));
		}
	}
	return lResult;
}
GDOMElement GDOMElement::FirstChild (void) const
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().first_child());
}
GDOMElement GDOMElement::LastChild (void) const
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().last_child());
}
std::list<GDOMElement> GDOMElement::Child (std::string const & rhs) const
{
	std::list<GDOMElement> lResult;
	for (::pugi::xml_node i(m_ref.Referenced().first_child()); i.empty() == false; i = i.next_sibling())
	{
		std::string sName(static_cast<char const *>(i.name()));
		if (sName == rhs)
		{
			lResult.push_back(static_cast<GDOMEssentials::GDOMElementRef const &>(GDOMEssentials::GDOMElementRef(i)));
		}
	}
	return lResult;
}
std::list<GDOMElement> GDOMElement::Child (GDOMPredicateForElement & pred, unsigned int const & depth) const
{
	std::list<GDOMElement> lResult;
	if (depth > 0)
	{
		for ( ::pugi::xml_node i( m_ref.Referenced().first_child() ); i.empty() == false; i = i.next_sibling() )
		{
			GDOMElement curr(static_cast<GDOMEssentials::GDOMElementRef const &>(GDOMEssentials::GDOMElementRef(i)));
			if (pred(curr))
			{
				lResult.push_back(curr);
			}
			std::list<GDOMElement> lResultAux(curr.Child(pred,(depth-1)));
			lResult.splice(lResult.end(),lResultAux);
		}
	}
	return lResult;
}
GDOMElement GDOMElement::InsertChild (GDOMElement::etNodeType const & rhs)
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().prepend_child(GDOMEssentials::FromNodeTypeGDOMToImpl(rhs)));
}
GDOMElement GDOMElement::AppendChild (GDOMElement::etNodeType const & rhs)
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().append_child(GDOMEssentials::FromNodeTypeGDOMToImpl(rhs)));
}
GDOMElement GDOMElement::InsertChild (GDOMElement::etNodeType const & rhs, GDOMElement const & pos)
{
	GDOMEssentials::GDOMElementRef rResult;
	GDOMEssentials::GDOMElementRef rPos(pos);
	if (rPos.Referenced().parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMElementRef(m_ref.Referenced().insert_child_before(GDOMEssentials::FromNodeTypeGDOMToImpl(rhs),rPos.Referenced()));
	}
	return rResult;
}
GDOMElement GDOMElement::AppendChild (GDOMElement::etNodeType const & rhs, GDOMElement const & pos)
{
	GDOMEssentials::GDOMElementRef rResult;
	GDOMEssentials::GDOMElementRef rPos(pos);
	if (rPos.Referenced().parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMElementRef(m_ref.Referenced().insert_child_after(GDOMEssentials::FromNodeTypeGDOMToImpl(rhs),rPos.Referenced()));
	}
	return rResult;
}
GDOMElement GDOMElement::InsertChild (std::string const & rhs)
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().prepend_child(rhs.c_str()));
}
GDOMElement GDOMElement::AppendChild (std::string const & rhs)
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().append_child(rhs.c_str()));
}
GDOMElement GDOMElement::InsertChild (std::string const & rhs, GDOMElement const & pos)
{
	GDOMEssentials::GDOMElementRef rResult;
	GDOMEssentials::GDOMElementRef rPos(pos);
	if (rPos.Referenced().parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMElementRef(m_ref.Referenced().insert_child_before(rhs.c_str(),rPos.Referenced()));
	}
	return rResult;
}
GDOMElement GDOMElement::AppendChild (std::string const & rhs, GDOMElement const & pos)
{
	GDOMEssentials::GDOMElementRef rResult;
	GDOMEssentials::GDOMElementRef rPos(pos);
	if (rPos.Referenced().parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMElementRef(m_ref.Referenced().insert_child_after(rhs.c_str(),rPos.Referenced()));
	}
	return rResult;
}
GDOMElement GDOMElement::InsertChild (GDOMElement const & rhs)
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().prepend_copy(static_cast<GDOMEssentials::GDOMElementRef const &>(rhs).Referenced()));
}
GDOMElement GDOMElement::AppendChild (GDOMElement const & rhs)
{
	return GDOMEssentials::GDOMElementRef(m_ref.Referenced().append_copy(static_cast<GDOMEssentials::GDOMElementRef const &>(rhs).Referenced()));
}
GDOMElement GDOMElement::InsertChild (GDOMElement const & rhs, GDOMElement const & pos)
{
	GDOMEssentials::GDOMElementRef rResult;
	GDOMEssentials::GDOMElementRef rPos(pos);
	if (rPos.Referenced().parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMElementRef(m_ref.Referenced().insert_copy_before(static_cast<GDOMEssentials::GDOMElementRef const &>(rhs).Referenced(),rPos.Referenced()));
	}
	return rResult;
}
GDOMElement GDOMElement::AppendChild (GDOMElement const & rhs, GDOMElement const & pos)
{
	GDOMEssentials::GDOMElementRef rResult;
	GDOMEssentials::GDOMElementRef rPos(pos);
	if (rPos.Referenced().parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMElementRef(m_ref.Referenced().insert_copy_after(static_cast<GDOMEssentials::GDOMElementRef const &>(rhs).Referenced(),rPos.Referenced()));
	}
	return rResult;
}
bool GDOMElement::RemoveChild (std::string const & rhs)
{
	return m_ref.Referenced().remove_child(rhs.c_str());
}
bool GDOMElement::RemoveChild (GDOMElement const & rhs)
{
	return m_ref.Referenced().remove_child(static_cast<GDOMEssentials::GDOMElementRef const &>(rhs).Referenced());
}
GDOMAttribute GDOMElement::FirstAttribute (void) const
{
	return GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().first_attribute(),m_ref.Referenced());
}
GDOMAttribute GDOMElement::LastAttribute (void) const
{
	return GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().last_attribute(),m_ref.Referenced());
}
std::list<GDOMAttribute> GDOMElement::Attribute (std::string const & rhs) const
{
	std::list<GDOMAttribute> lResult;
	for (::pugi::xml_attribute i(m_ref.Referenced().first_attribute()); i.empty() == false; i = i.next_attribute())
	{
		std::string sName(static_cast<char const *>(i.name()));
		if (sName == rhs)
		{
			lResult.push_back(static_cast<GDOMEssentials::GDOMAttributeRef const &>(GDOMEssentials::GDOMAttributeRef(i,m_ref.Referenced())));
		}
	}
	return lResult;
}
std::list<GDOMAttribute> GDOMElement::Attribute (GDOMPredicateForAttribute & pred) const
{
	std::list<GDOMAttribute> lResult;
	for (::pugi::xml_attribute i(m_ref.Referenced().first_attribute()); i.empty() == false; i = i.next_attribute())
	{
		GDOMAttribute curr(static_cast<GDOMEssentials::GDOMAttributeRef const &>(GDOMEssentials::GDOMAttributeRef(i,m_ref.Referenced())));
		if (pred(curr))
		{
			lResult.push_back(curr);
		}
	}
	return lResult;
}
GDOMAttribute GDOMElement::InsertAttribute (std::string const & rhs)
{
	return GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().prepend_attribute(rhs.c_str()),m_ref.Referenced());
}
GDOMAttribute GDOMElement::AppendAttribute (std::string const & rhs)
{
	return GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().append_attribute(rhs.c_str()),m_ref.Referenced());
}
GDOMAttribute GDOMElement::InsertAttribute (std::string const & rhs, GDOMAttribute const & pos)
{
	GDOMEssentials::GDOMAttributeRef rResult;
	GDOMEssentials::GDOMAttributeRef rPos(pos);
	if (rPos.Parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().insert_attribute_before(rhs.c_str(),rPos.Referenced()),m_ref.Referenced());
	}
	return rResult;
}
GDOMAttribute GDOMElement::AppendAttribute (std::string const & rhs, GDOMAttribute const & pos)
{
	GDOMEssentials::GDOMAttributeRef rResult;
	GDOMEssentials::GDOMAttributeRef rPos(pos);
	if (rPos.Parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().insert_attribute_after(rhs.c_str(),rPos.Referenced()),m_ref.Referenced());
	}
	return rResult;
}
GDOMAttribute GDOMElement::InsertAttribute (GDOMAttribute const & rhs)
{
	return GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().prepend_copy(static_cast<GDOMEssentials::GDOMAttributeRef const &>(rhs).Referenced()),m_ref.Referenced());
}
GDOMAttribute GDOMElement::AppendAttribute (GDOMAttribute const & rhs)
{
	return GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().append_copy(static_cast<GDOMEssentials::GDOMAttributeRef const &>(rhs).Referenced()),m_ref.Referenced());
}
GDOMAttribute GDOMElement::InsertAttribute (GDOMAttribute const & rhs, GDOMAttribute const & pos)
{
	GDOMEssentials::GDOMAttributeRef rResult;
	GDOMEssentials::GDOMAttributeRef rPos(pos);
	if (rPos.Parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().insert_copy_before(static_cast<GDOMEssentials::GDOMAttributeRef const &>(rhs).Referenced(),rPos.Referenced()),m_ref.Referenced());
	}
	return rResult;
}
GDOMAttribute GDOMElement::AppendAttribute (GDOMAttribute const & rhs, GDOMAttribute const & pos)
{
	GDOMEssentials::GDOMAttributeRef rResult;
	GDOMEssentials::GDOMAttributeRef rPos(pos);
	if (rPos.Parent() == m_ref.Referenced())
	{
		rResult = GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().insert_copy_after(static_cast<GDOMEssentials::GDOMAttributeRef const &>(rhs).Referenced(),rPos.Referenced()),m_ref.Referenced());
	}
	return rResult;
}
bool GDOMElement::RemoveAttribute (std::string const & rhs)
{
	return m_ref.Referenced().remove_attribute(rhs.c_str());
}
bool GDOMElement::RemoveAttribute (GDOMAttribute const & rhs)
{
	return m_ref.Referenced().remove_attribute(static_cast<GDOMEssentials::GDOMAttributeRef const &>(rhs).Referenced());
}
void GDOMElement::Serialize (std::string & sContents, std::string sIndent, unsigned int uiFormat) const
{
	GDOMElementXMLWriterPrivate oWriter;
	GDOMEssentials::GDOMXMLWriterPrivateImplProxy oWriterProxy(&oWriter);
	m_ref.Referenced().print(oWriterProxy,sIndent.c_str(),GDOMEssentials::FromFormatFlagGDOMToImpl(uiFormat),GDOMEssentials::FromEncodingGDOMToImpl(GDOMEssentials::eEncodingUTF8),0);
	if (oWriter.SerializedBuffer().empty() == false)
	{
		sContents.assign(static_cast<char const *>(static_cast<void const *>(&(oWriter.SerializedBuffer()[0]))),oWriter.SerializedBuffer().size());
	}
}
void GDOMElement::Serialize (std::vector<unsigned char> & vContents, std::string sIndent, unsigned int uiFormat, GDOMElement::etEncoding evEncoding) const
{
	GDOMElementXMLWriterPrivate oWriter;
	GDOMEssentials::GDOMXMLWriterPrivateImplProxy oWriterProxy(&oWriter);
	m_ref.Referenced().print(oWriterProxy,sIndent.c_str(),GDOMEssentials::FromFormatFlagGDOMToImpl(uiFormat),GDOMEssentials::FromEncodingGDOMToImpl(evEncoding),0);
	vContents = oWriter.SerializedBuffer();
}
void GDOMElement::Serialize (GDOMXMLWriter & oWriter, std::string sIndent, unsigned int uiFormat, GDOMElement::etEncoding evEncoding) const
{
	GDOMEssentials::GDOMXMLWriterPrivateImplProxy oWriterProxy(&oWriter);
	m_ref.Referenced().print(oWriterProxy,sIndent.c_str(),GDOMEssentials::FromFormatFlagGDOMToImpl(uiFormat),GDOMEssentials::FromEncodingGDOMToImpl(evEncoding),0);
}
