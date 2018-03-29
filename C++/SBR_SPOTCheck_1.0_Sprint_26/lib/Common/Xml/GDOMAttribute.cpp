/*
 * GDOMAttribute.cpp
 *
 *  Created on: Jun 29, 2011
 *      Author: pcantarini
 */

#include "Common/Xml/GDOMAttribute.h"

GDOMPredicateForAttribute::~GDOMPredicateForAttribute (void)
{
}

GDOMAttribute::GDOMAttribute (void)
	:	GDOMEssentials(),
	 	m_ref()
{
}
GDOMAttribute::GDOMAttribute (GDOMEssentials::GDOMAttributeRef const & rhs)
	:	GDOMEssentials(),
	 	m_ref(rhs)
{
}
GDOMAttribute::~GDOMAttribute (void)
{
}
GDOMAttribute::operator GDOMEssentials::GDOMAttributeRef  const & (void) const
{
	return m_ref;
}
bool GDOMAttribute::operator  ! (void) const
{
	return !(m_ref.Referenced());
}
bool GDOMAttribute::operator == (GDOMAttribute const & rhs) const
{
	return (m_ref.Referenced() == static_cast<GDOMEssentials::GDOMAttributeRef const &>(rhs).Referenced());
}
bool GDOMAttribute::operator != (GDOMAttribute const & rhs) const
{
	return (m_ref.Referenced() != static_cast<GDOMEssentials::GDOMAttributeRef const &>(rhs).Referenced());
}
bool GDOMAttribute::Empty (void) const
{
	return m_ref.Referenced().empty();
}
std::string GDOMAttribute::Name (void) const
{
	return static_cast<char const *>(m_ref.Referenced().name());
}
void GDOMAttribute::Name (std::string const & rhs)
{
	m_ref.Referenced().set_name(rhs.c_str());
}
std::string GDOMAttribute::Value (void) const
{
	return static_cast<char const *>(m_ref.Referenced().value());
}
void GDOMAttribute::Value (std::string const & rhs)
{
	m_ref.Referenced().set_value(rhs.c_str());
}
void GDOMAttribute::Value (bool const & rhs)
{
	m_ref.Referenced().set_value(rhs);
}
void GDOMAttribute::Value (int const & rhs)
{
	m_ref.Referenced().set_value(rhs);
}
void GDOMAttribute::Value (unsigned int const & rhs)
{
	m_ref.Referenced().set_value(rhs);
}
void GDOMAttribute::Value (double const & rhs)
{
	m_ref.Referenced().set_value(rhs);
}
GDOMAttribute GDOMAttribute::Next (void) const
{
	return GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().next_attribute(),m_ref.Parent());
}
GDOMAttribute GDOMAttribute::Previous (void) const
{
	return GDOMEssentials::GDOMAttributeRef(m_ref.Referenced().previous_attribute(),m_ref.Parent());
}
std::string GDOMAttribute::AsString (void) const
{
	return static_cast<char const *>(m_ref.Referenced().value());
}
bool GDOMAttribute::AsBool (void) const
{
	return m_ref.Referenced().as_bool();
}
int GDOMAttribute::AsInt (void) const
{
	return m_ref.Referenced().as_int();
}
unsigned int GDOMAttribute::AsUInt (void) const
{
	return m_ref.Referenced().as_uint();
}
float GDOMAttribute::AsFloat (void) const
{
	return m_ref.Referenced().as_float();
}
double GDOMAttribute::AsDouble(void) const
{
	return m_ref.Referenced().as_double();
}
void GDOMAttribute::AsString (std::string const & rhs)
{
	m_ref.Referenced().set_value(rhs.c_str());
}
void GDOMAttribute::AsBool (bool const & rhs)
{
	m_ref.Referenced().set_value(rhs);
}
void GDOMAttribute::AsInt (int const & rhs)
{
	m_ref.Referenced().set_value(rhs);
}
void GDOMAttribute::AsUInt (unsigned int const & rhs)
{
	m_ref.Referenced().set_value(rhs);
}
void GDOMAttribute::AsFloat (float const & rhs)
{
	m_ref.Referenced().set_value(rhs);
}
void GDOMAttribute::AsDouble (double const & rhs)
{
	m_ref.Referenced().set_value(rhs);
}
GDOMAttribute & GDOMAttribute::operator = (std::string const & rhs)
{
	m_ref.Referenced().set_value(rhs.c_str());
	return (*this);
}
GDOMAttribute & GDOMAttribute::operator = (bool rhs)
{
	m_ref.Referenced().set_value(rhs);
	return (*this);
}
GDOMAttribute & GDOMAttribute::operator = (int rhs)
{
	m_ref.Referenced().set_value(rhs);
	return (*this);
}
GDOMAttribute & GDOMAttribute::operator = (unsigned int rhs)
{
	m_ref.Referenced().set_value(rhs);
	return (*this);
}
GDOMAttribute & GDOMAttribute::operator = (double rhs)
{
	m_ref.Referenced().set_value(rhs);
	return (*this);
}
