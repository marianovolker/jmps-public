/*
 * GDOMAttribute.h
 *
 *  Created on: Jun 24, 2011
 *      Author: pcantarini
 */

#ifndef GDOMAttribute_h
#define GDOMAttribute_h

#include <string>

#include "Common/Xml/GDOMEssentials.h"

class GDOMAttribute;

/*! @class GDOMPredicateForAttribute GDOMAttribute.h "Src/Common/Xml/GDOMAttribute.h"
 * @brief This class is an abstract base class for the user DOM attribute evaluation predicate.
 *
 */
class GDOMPredicateForAttribute
{
public:
	//! @brief Destructor.
	virtual ~GDOMPredicateForAttribute (void);

	/*! @brief Predicate which will tell to the searching algorithm whether the user accepts the node to be included in the outcome list or not.
	 *
	 * @param grAttr Attribute to be evaluated by the user.
	 * @return True if accepted or false if rejected.
	 */
	virtual bool operator () (GDOMAttribute const & grAttr) = 0;
};

/*! @class GDOMAttribute GDOMAttribute.h "Src/Common/Xml/GDOMAttribute.h"
 * @brief This class represents a DOM attribute with its corresponding name and value.
 *
 */
class GDOMAttribute
	:	public GDOMEssentials
{
public:
	//! @brief Constructor.
	GDOMAttribute (void);
	/*! @brief Constructor.
	 *
	 * @param rhs Internal reference to a DOM attribute.
	 */
	GDOMAttribute (GDOMAttributeRef const & rhs);
	//! @brief Destructor.
	virtual ~GDOMAttribute (void);
	/*! @brief Implicit getter to the reference of the internal handling of the user DOM attribute.
	 *
	 * @return Reference to the internal handling of the user DOM attribute.
	 */
	operator GDOMAttributeRef const & (void) const;
	/*! @brief Evaluate the attribute emptiness status.
	 *
	 * @return Attribute emptiness evaluation result.
	 */
	bool operator  ! (void) const;
	/*! @name ComparisonOperators Comparison Operators between attributes.
	 *
	 * @param rhs Attribute reference to be compared to.
	 * @return Comparison result.
	 */
	//!@{
	bool operator == (GDOMAttribute const & rhs) const;
	bool operator != (GDOMAttribute const & rhs) const;
	//!@}
	/*! @brief Evaluate the attribute emptiness status.
	 *
	 * @return Attribute emptiness evaluation result.
	 */
	bool Empty (void) const;
	/*! @brief Gets attribute name.
	 *
	 * @return Attribute name.
	 */
	std::string Name (void) const;
	/*! @brief Sets attribute name.
	 *
	 * @param rhs Attribute name.
	 */
	void Name (std::string const & rhs);
	/*! @brief Gets attribute value.
	 *
	 * @return Attribute value as string.
	 */
	std::string Value (void) const;
	/*! @name ValueSetters Attributes setters regarding possible attribute values.
	 *
	 * @param rhs New attribute value to be set.
	 */
	//!@{
	void Value (std::string const & rhs);
	void Value (bool const & rhs);
	void Value (int const & rhs);
	void Value (unsigned int const & rhs);
	void Value (double const & rhs);
	//!@}
	/*! @brief Obtain next attribute sibling.
	 *
	 * @return Next sibling attribute.
	 */
	GDOMAttribute Next (void) const;
	/*! @brief Obtain previous attribute sibling.
	 *
	 * @return Previous sibling attribute.
	 */
	GDOMAttribute Previous (void) const;
	/*! @brief Gets attribute value explicitly as string.
	 *
	 * @return Attribute value as string.
	 */
	std::string		AsString (void) const;
	/*! @brief Gets attribute value explicitly as bool.
	 * Get attribute value as bool (returns true if first character is in '1tTyY' set), or false if attribute is empty
	 *
	 * @return Attribute value as bool.
	 */
	bool			AsBool	 (void) const; //
	/*! @brief Gets attribute value explicitly as int.
	 * Get attribute value as a number, or 0 if conversion did not succeed or attribute is empty
	 *
	 * @return Attribute value as int.
	 */
	int				AsInt	 (void) const;
	/*! @brief Gets attribute value explicitly as unsigned int.
	 *
	 * @return Attribute value as unsigned int.
	 */
	unsigned int	AsUInt	 (void) const;
	/*! @brief Gets attribute value explicitly as float.
	 *
	 * @return Attribute value as float.
	 */
	float			AsFloat	 (void) const;
	/*! @brief Gets attribute value explicitly as double.
	 *
	 * @return Attribute value as double.
	 */
	double			AsDouble (void) const;
	/*! @brief Sets attribute value explicitly as string.
	 *
	 * @param rhs Attribute value as string.
	 */
	void AsString	(std::string const & rhs);
	/*! @brief Sets attribute value explicitly as bool.
	 *
	 * @param rhs Attribute value as bool.
	 */
	void AsBool		(bool const & rhs);
	/*! @brief Sets attribute value explicitly as int.
	 *
	 * @param rhs Attribute value as int.
	 */
	void AsInt		(int const & rhs);
	/*! @brief Sets attribute value explicitly as unsigned int.
	 *
	 * @param rhs Attribute value as unsigned int.
	 */
	void AsUInt		(unsigned int const & rhs);
	/*! @brief Sets attribute value explicitly as float.
	 *
	 * @param rhs Attribute value as float.
	 */
	void AsFloat	(float const & rhs);
	/*! @brief Sets attribute value explicitly as double.
	 *
	 * @param rhs Attribute value as double.
	 */
	void AsDouble	(double const & rhs);
	/*! @name AssignmentOperator Assignment Operators regarding possible attribute values.
	 *
	 * @param rhs New attribute value to be set.
	 * @return Current attribute.
	 */
	//!@{
	GDOMAttribute & operator = (std::string const & rhs);
	GDOMAttribute & operator = (bool rhs);
	GDOMAttribute & operator = (int rhs);
	GDOMAttribute & operator = (unsigned int rhs);
	GDOMAttribute & operator = (double rhs);
	//!@}

private:
	GDOMAttributeRef m_ref;	//!< Internal reference to the DOM attribute.
};

#endif
