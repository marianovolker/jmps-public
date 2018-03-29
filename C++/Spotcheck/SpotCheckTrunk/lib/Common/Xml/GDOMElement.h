/*
 * GDOMElement.h
 *
 *  Created on: Jun 24, 2011
 *      Author: pcantarini
 */

#ifndef GDOMElement_h
#define GDOMElement_h

#include <vector>
#include <string>
#include <list>

#include "Common/Xml/GDOMEssentials.h"
#include "Common/Xml/GDOMXMLWriter.h"
#include "Common/Xml/GDOMAttribute.h"

class GDOMElement;

/*! @class GDOMPredicateForElement GDOMElement.h "Src/Common/Xml/GDOMElement.h"
 * @brief This class is an abstract base class for the user DOM element evaluation predicate.
 *
 */
class GDOMPredicateForElement
{
public:
	//! @brief Destructor.
	virtual ~GDOMPredicateForElement (void);

	/*! @brief Predicate which will tell to the searching algorithm whether the user accepts the node to be included in the outcome list or not.
	 *
	 * @param grElem Element to be evaluated by the user.
	 * @return True if accepted or false if rejected.
	 */
	virtual bool operator () (GDOMElement const & grElem) = 0;
};

/*! @class GDOMElement GDOMElement.h "Src/Common/Xml/GDOMElement.h"
 * @brief This class represents a DOM element with its corresponding name, type and value.
 *
 */
class GDOMElement
	:	public GDOMEssentials
{
public:
	//! @brief Constructor.
	GDOMElement (void);
	/*! @brief Constructor.
	 *
	 * @param rhs Internal reference to a DOM element.
	 */
	GDOMElement (GDOMElementRef const & rhs);
	//! @brief Destructor.
	virtual ~GDOMElement (void);
	/*! @brief Implicit getter to the reference of the internal handling of the user DOM element.
	 *
	 * @return Reference to the internal handling of the user DOM element.
	 */
	operator GDOMElementRef const & (void) const;
	/*! @brief Evaluate the element emptiness status.
	 *
	 * @return Element emptiness evaluation result.
	 */
	bool operator  ! (void) const;
	/*! @name ComparisonOperators Comparison Operators.
	 *
	 * @param rhs DOM element reference to be compared to
	 * @return Comparison result
	 */
	//!@{
	bool operator == (GDOMElement const & rhs) const;
	bool operator != (GDOMElement const & rhs) const;
	//!@}
	/*! @brief Evaluate the element emptiness status.
	 *
	 * @return Element emptiness evaluation result.
	 */
	bool Empty (void) const;
	/*! @brief Gets element name.
	 *
	 * @return Element name.
	 */
	std::string Name (void) const;
	/*! @brief Sets element name.
	 * The DOM elements which can have their name set are:
	 * - eNodeElement: Element tag, i.e. '<node/>'.
	 * - eNodePI: Processing instruction, i.e. '<?name?>'.
	 * - eNodeDeclaration: Document declaration, i.e. '<?xml version="1.0"?>'.
	 * .
	 *
	 * @param rhs element name.
	 * @return True if value was able to be set.
	 */
	bool Name (std::string const & rhs);
	/*! @brief Gets element value.
	 *
	 * @return Element value as string.
	 */
	std::string Value (void) const;
	/*! @brief Element value setter.
	 * The DOM elements which can have their values set are:
	 * - eNodePCData: Plain character data, i.e. 'text'.
	 * - eNodeCData: Character data, i.e. '<![CDATA[text]]>'.
	 * - eNodeComment: Comment tag, i.e. '<!-- text -->'.
	 * - eNodePI: Processing instruction, i.e. '<?name?>'.
	 * - eNodeDoctype: Document type declaration, i.e. '<!DOCTYPE doc>'.
	 * .
	 *
	 * @param rhs New element value to be set.
	 * @return True if value was able to be set.
	 */
	bool Value (std::string const & rhs);
	/*! @brief Obtains element node type.
	 *
	 * @return Element node type.
	 */
	etNodeType Type (void) const;
	/*! @brief Retrieves current element Root node from the tree to which it belongs.
	 *
	 * @return Root element.
	 */
	GDOMElement Root (void) const;
	/*! @brief Retrieves current element Parent node from the tree to which it belongs.
	 *
	 * @return Parent element.
	 */
	GDOMElement Parent (void) const;
	/*! @brief Obtain next sibling element.
	 *
	 * @return Next sibling element. Empty element if there is none.
	 */
	GDOMElement Next (void) const;
	/*! @brief Obtain previous sibling element.
	 *
	 * @return Previous sibling element. Empty element if there is none.
	 */
	GDOMElement Previous (void) const;
	/*! @brief Obtain next sibling element which name corresponds to the one given.
	 *
	 * @param rhs Next sibling element name to be found.
	 * @return Next sibling element. Empty element if not found.
	 */
	GDOMElement Next (std::string const & rhs) const;
	/*! @brief Obtain previous element sibling which name corresponds to the one given.
	 *
	 * @param rhs Previous sibling element name to be found.
	 * @return Previous sibling element. Empty element if not found.
	 */
	GDOMElement Previous (std::string const & rhs) const;
	/*! @brief Gets element first child value.
	 * The child elements evaluated to retrieve the information have to be the first one found on any of these types:
	 * - eNodePCData: Plain character data, i.e. 'text'.
	 * - eNodeCData: Character data, i.e. '<![CDATA[text]]>'.
	 * .
	 *
	 * @return Element fist child value as string.
	 */
	std::string FirstChildValue (void) const;
	/*! @brief Sets element first child value.
	 * The child elements evaluated to set new information have to be the first one found on any of these types:
	 * - eNodePCData: Plain character data, i.e. 'text'.
	 * - eNodeCData: Character data, i.e. '<![CDATA[text]]>'.
	 * .
	 * Or one will be added if not found. The element to be added in such case will be by default of type eNodePCData
	 * unless the bAsPCData is set to false in which case the added element will be of eNodeCData type.
	 *
	 * @param rhs New element first child value to be set.
	 * @param bAsPCData Boolean to determine whether eNodePCData or eNodeCData child node value should be set.
	 */
	void FirstChildValue (std::string const & rhs, bool bAsPCData = true);
	/*! @brief Gets element compound child value between all child nodes of first level.
	 * The child elements evaluated to retrieve the information have to be the first one found on any of these types:
	 * - eNodePCData: Plain character data, i.e. 'text'.
	 * - eNodeCData: Character data, i.e. '<![CDATA[text]]>'.
	 * .
	 *
	 * @return Element compound child value as string list.
	 */
	std::list<std::string> ChildValue (void) const;
	/*! @brief Obtain first child element.
	 *
	 * @return First child element. Empty element if there are no child element nodes for this element.
	 */
	GDOMElement FirstChild (void) const;
	/*! @brief Obtain last child element.
	 *
	 * @return Last child element. Empty element if there are no child element nodes for this element.
	 */
	GDOMElement LastChild (void) const;
	/*! @brief Obtain first level child element list which name corresponds to the one given..
	 *
	 * @param rhs Child element name to be found.
	 * @return List of first level child elements with the corresponding searched name. Empty list if not found.
	 */
	std::list<GDOMElement> Child (std::string const & rhs) const;
	/*! @brief Obtain Nth level child element list which the given user predicate tells to the searching algorithm whether the he accepts the node to be included in the outcome list or not.
	 *
	 * @param User predicate which indicates the acceptance of the current node in the outcome list.
	 * @param depth Nth level ov elements to be included in the outcome list starting from the current level.
	 * @return List of Nth level child elements found. Empty list if not found.
	 */
	std::list<GDOMElement> Child (GDOMPredicateForElement & pred, unsigned int const & depth = 1) const;
	/*! @brief Create a new child element with the given node type and place it at the front of the first level child list.
	 * One have to set its name, value and attributes if that is the case after the element node insertion with the element received as result.
	 *
	 * @param rhs New child element node type to be created.
	 * @return New child element created. Empty child if the call fails.
	 */
	GDOMElement InsertChild (etNodeType const & rhs);
	/*! @brief Create a new child element with the given node type and place it at the back of the first level child list.
	 * One have to set its name, value and attributes if that is the case after the element node insertion with the element received as result.
	 *
	 * @param rhs New child element node type to be created.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement AppendChild (etNodeType const & rhs);
	/*! @brief Create a new child element with the given node type and place it at the front of the given element child position.
	 * One have to set its name, value and attributes if that is the case after the element node insertion with the element received as result.
	 * The child element given as position has to be a direct child of the current element or the call will fail.
	 *
	 * @param rhs New child element node type to be created.
	 * @param pos Child element given as position which belongs to the current element child list.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement InsertChild (etNodeType const & rhs, GDOMElement const & pos);
	/*! @brief Create a new child element with the given node type and place it at the back of the given element child position.
	 * One have to set its name, value and attributes if that is the case after the element node insertion with the element received as result.
	 * The child element given as position has to be a direct child of the current element or the call will fail.
	 *
	 * @param rhs New child element node type to be created.
	 * @param pos Child element given as position which belongs to the current element child list.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement AppendChild (etNodeType const & rhs, GDOMElement const & pos);
	/*! @brief Create a new child element with the given name and place it at the front of the first level child list.
	 * One have to set its value and attributes if that is the case after the element node insertion with the element received as result.
	 * The inserted element is of node type eNodeElement.
	 *
	 * @param rhs New child element name to be created.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement InsertChild (std::string const & rhs);
	/*! @brief Create a new child element with the given name and place it at the back of the first level child list.
	 * One have to set its value and attributes if that is the case after the element node insertion with the element received as result.
	 * The inserted element is of node type eNodeElement.
	 *
	 * @param rhs New child element name to be created.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement AppendChild (std::string const & rhs);
	/*! @brief Create a new child element with the given name and place it at the front of the given element child position.
	 * One have to set its value and attributes if that is the case after the element node insertion with the element received as result.
	 * The inserted element is of node type eNodeElement.
	 * The child element given as position has to be a direct child of the current element or the call will fail.
	 *
	 * @param rhs New child element name to be created.
	 * @param pos Child element given as position which belongs to the current element child list.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement InsertChild (std::string const & rhs, GDOMElement const & pos);
	/*! @brief Create a new child element with the given name and place it at the back of the given element child position.
	 * One have to set its value and attributes if that is the case after the element node insertion with the element received as result.
	 * The inserted element is of node type eNodeElement.
	 * The child element given as position has to be a direct child of the current element or the call will fail.
	 *
	 * @param rhs New child element name to be created.
	 * @param pos Child element given as position which belongs to the current element child list.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement AppendChild (std::string const & rhs, GDOMElement const & pos);
	/*! @brief Create a new child element as a content copy of the one given and place it at the front of the first level child list.
	 * One can set its name, value and attributes if that is the case after the element node insertion with the element received as result.
	 *
	 * @param rhs Element which content has to be copied to the new child element.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement InsertChild (GDOMElement const & rhs);
	/*! @brief Create a new child element as a content copy of the one given and place it at the back of the first level child list.
	 * One can set its name, value and attributes if that is the case after the element node insertion with the element received as result.
	 *
	 * @param rhs Element which content has to be copied to the new child element.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement AppendChild (GDOMElement const & rhs);
	/*! @brief Create a new child element as a content copy of the one given and place it at the front of the given element child position.
	 * One can set its name, value and attributes if that is the case after the element node insertion with the element received as result.
	 * The child element given as position has to be a direct child of the current element or the call will fail.
	 *
	 * @param rhs Element which content has to be copied to the new child element.
	 * @param pos Child element given as position which belongs to the current element child list.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement InsertChild (GDOMElement const & rhs, GDOMElement const & pos);
	/*! @brief Create a new child element as a content copy of the one given and place it at the back of the given element child position.
	 * One can set its name, value and attributes if that is the case after the element node insertion with the element received as result.
	 * The child element given as position has to be a direct child of the current element or the call will fail.
	 *
	 * @param rhs Element which content has to be copied to the new child element.
	 * @param pos Child element given as position which belongs to the current element child list.
	 * @return New child element created. Empty element if the call fails.
	 */
	GDOMElement AppendChild (GDOMElement const & rhs, GDOMElement const & pos);
	/*! @brief Remove all direct child elements and its corresponding sub trees which name corresponds to the one given.
	 *
	 * @param rhs Child element name which has to be removed.
	 * @return	True if at least one element was found and removed.
	 */
	bool RemoveChild (std::string const & rhs);
	/*! @brief Remove direct child element and its corresponding sub tree.
	 * The element has to be a direct child of the current node.
	 *
	 * @param rhs Child element which has to be removed.
	 * @return	True if child element was found and removed.
	 */
	bool RemoveChild (GDOMElement const & rhs);
	/*! @brief Obtain first element attribute.
	 *
	 * @return First element attribute. Empty attribute if there are no attributes for this element.
	 */
	GDOMAttribute FirstAttribute (void) const;
	/*! @brief Obtain last element attribute.
	 *
	 * @return Last element attribute. Empty attribute if there are no attributes for this element.
	 */
	GDOMAttribute LastAttribute (void) const;
	/*! @brief Obtain attribute list which name corresponds to the one given..
	 *
	 * @param rhs Attribute name to be found.
	 * @return List of attributes with the corresponding searched name. Empty list if not found.
	 */
	std::list<GDOMAttribute> Attribute (std::string const & rhs) const;
	/*! @brief Obtain attribute list which the given user predicate tells to the searching algorithm whether the he accepts the node to be included in the outcome list or not.
	 *
	 * @param User predicate which indicates the acceptance of the current node in the outcome list.
	 * @return List of attributes found. Empty list if not found.
	 */
	std::list<GDOMAttribute> Attribute (GDOMPredicateForAttribute & pred) const;
	/*! @brief Create a new attribute with the given name and place it at the front of the attribute list.
	 * One have to set its value after the node insertion with the attribute received as result.
	 *
	 * @param rhs New attribute name to be created.
	 * @return New attribute created. Empty attribute if the call fails.
	 */
	GDOMAttribute InsertAttribute (std::string const & rhs);
	/*! @brief Create a new attribute with the given name and place it at the back of the attribute list.
	 * One have to set its value after the attribute node insertion with the attribute received as result.
	 *
	 * @param rhs New attribute name to be created.
	 * @return New attribute created. Empty attribute if the call fails.
	 */
	GDOMAttribute AppendAttribute (std::string const & rhs);
	/*! @brief Create a new attribute with the given name and place it at the front of the given attribute position.
	 * One have to set its value after the attribute node insertion with the attribute received as result.
	 * The attribute given as position has to be a direct attribute of the current element or the call will fail.
	 *
	 * @param rhs New attribute name to be created.
	 * @param pos Attribute given as position which belongs to the current element attribute list.
	 * @return New attribute created. Empty attribute if the call fails.
	 */
	GDOMAttribute InsertAttribute (std::string const & rhs, GDOMAttribute const & pos);
	/*! @brief Create a new attribute with the given name and place it at the back of the given attribute position.
	 * One have to set its value after the attribute node insertion with the attribute received as result.
	 * The attribute given as position has to be a direct attribute of the current element or the call will fail.
	 *
	 * @param rhs New attribute name to be created.
	 * @param pos Attribute given as position which belongs to the current element attribute list.
	 * @return New attribute created. Empty attribute if the call fails.
	 */
	GDOMAttribute AppendAttribute (std::string const & rhs, GDOMAttribute const & pos);
	/*! @brief Create a new attribute as a content copy of the one given and place it at the front of the attribute list.
	 * One can set its value if that is the case after the attribute node insertion with the attribute received as result.
	 *
	 * @param rhs Attribute which content has to be copied to the new attribute.
	 * @return New attribute created. Empty attribute if the call fails.
	 */
	GDOMAttribute InsertAttribute (GDOMAttribute const & rhs);
	/*! @brief Create a new attribute as a content copy of the one given and place it at the back of the attribute list.
	 * One can set its value if that is the case after the attribute node insertion with the attribute received as result.
	 *
	 * @param rhs Attribute which content has to be copied to the new attribute.
	 * @return New attribute created. Empty attribute if the call fails.
	 */
	GDOMAttribute AppendAttribute (GDOMAttribute const & rhs);
	/*! @brief Create a new attribute as a content copy of the one given and place it at the front of the given attribute position.
	 * One cans set its value if that is the case after the attribute node insertion with the attribute received as result.
	 * The attribute given as position has to be a direct attribute of the current element or the call will fail.
	 *
	 * @param rhs Attribute which content has to be copied to the new attribute element.
	 * @param pos Attribute given as position which belongs to the current element attribute list.
	 * @return New attribute created. Empty attribute if the call fails.
	 */
	GDOMAttribute InsertAttribute (GDOMAttribute const & rhs, GDOMAttribute const & pos);
	/*! @brief Create a new attribute as a content copy of the one given and place it at the back of the given attribute position.
	 * One cans set its value if that is the case after the attribute node insertion with the attribute received as result.
	 * The attribute given as position has to be a direct attribute of the current element or the call will fail.
	 *
	 * @param rhs Attribute which content has to be copied to the new attribute element.
	 * @param pos Attribute given as position which belongs to the current element attribute list.
	 * @return New attribute created. Empty attribute if the call fails.
	 */
	GDOMAttribute AppendAttribute (GDOMAttribute const & rhs, GDOMAttribute const & pos);
	/*! @brief Remove all element attributes which name corresponds to the one given.
	 *
	 * @param rhs Attribute name which has to be removed.
	 * @return	True if at least one attribute was found and removed.
	 */
	bool RemoveAttribute (std::string const & rhs);
	/*! @brief Remove element attribute.
	 * The attribute has to belong to the current node.
	 *
	 * @param rhs Element which has to be removed.
	 * @return	True if attribute was found and removed.
	 */
	bool RemoveAttribute (GDOMAttribute const & rhs);
	/*! @brief Serialize node subtree starting from the current node element as root.
	 *
	 * @param sContents	String repository which will held the serialized node subtree.
	 * @param sIndent Indent string used to give hierarchy levels to the output.
	 * @param uiFormat Format flags used to perform the serialization.
	 */
	void Serialize (std::string & sContents, std::string sIndent = "\t", unsigned int uiFormat = GDOMElement::CFormatDefault) const;
	/*! @brief Serialize node subtree starting from the current node element as root.
	 *
	 * @param vContents	Array repository which will held the serialized node subtree.
	 * @param sIndent Indent string used to give hierarchy levels to the output.
	 * @param uiFormat Format flags used to perform the serialization.
	 * @param evEncoding Encoding used to perform the serialization.
	 */
	void Serialize (std::vector<unsigned char> & vContents, std::string sIndent = "\t", unsigned int uiFormat = GDOMElement::CFormatDefault, etEncoding evEncoding = GDOMElement::eEncodingAuto) const;
	/*! @brief Serialize node subtree starting from the current node element as root.
	 *
	 * @param oWriter User defined serialization observer which will receive the chunk of serialized buffers to be stored in any way the user seems best to fit it needs.
	 * @param sIndent Indent string used to give hierarchy levels to the output.
	 * @param uiFormat Format flags used to perform the serialization.
	 * @param evEncoding Encoding used to perform the serialization.
	 */
	void Serialize (GDOMXMLWriter & oWriter, std::string sIndent = "\t", unsigned int uiFormat = GDOMElement::CFormatDefault, etEncoding evEncoding = GDOMElement::eEncodingAuto) const;

private:
	GDOMElementRef m_ref;	//!< Internal reference to the DOM element.
};

#endif
