#include "GXMLParser.h"

#include <vector>
#include <map>

#include <Common/Xml/GDOMDocument.h>
#include "GConfigUtils.h"
#include "Common/Util/DEBUGOUT.h"

using namespace GConfigTypes;


class GDOMPredicateAllChildren
	:	public GDOMPredicateForElement
{
public:
	virtual ~GDOMPredicateAllChildren (void);
	virtual bool operator () (GDOMElement const & grElem);
};

GDOMPredicateAllChildren::~GDOMPredicateAllChildren (void)
{
}
bool GDOMPredicateAllChildren::operator () (GDOMElement const & grElem)
{
	return (grElem.Type() == GDOMEssentials::eNodeElement);
}

class GDOMPredicateAllAttributes
	:	public GDOMPredicateForAttribute
{
public:
	virtual ~GDOMPredicateAllAttributes (void);
	virtual bool operator () (GDOMAttribute const & grAttr);
};

GDOMPredicateAllAttributes::~GDOMPredicateAllAttributes (void)
{
}
bool GDOMPredicateAllAttributes::operator () (GDOMAttribute const & grAttr)
{
	return true;
}

std::string CreateXMLPath (std::vector<std::string> & vXMLPath)
{
	std::string sRes;
	std::vector<std::string>::iterator itr = vXMLPath.begin();
	std::vector<std::string>::iterator itre = vXMLPath.end();
	for (; itr != itre; ++itr)
	{
		sRes += "/" + (*itr);
	}
	return sRes;
}

void WalkTree (GDOMElement Elem, std::vector<std::string> & vXMLPath, std::list<GConfigItem> & oConfigItemList)
{
	GDOMPredicateAllChildren oDOMPredicateAllChildren;
	std::list<GDOMElement> lElem(Elem.Child(oDOMPredicateAllChildren,1));
	if (lElem.empty() == false)
	{
		vXMLPath.push_back(Elem.Name());
		std::list<GDOMElement>::iterator itr = lElem.begin();
		std::list<GDOMElement>::iterator itre = lElem.end();
		for (; itr != itre; ++itr)
		{
			WalkTree((*itr),vXMLPath,oConfigItemList);
		}
		vXMLPath.pop_back();
	}
	else
	{
		std::string sValue(Elem.FirstChildValue());
		if (sValue.empty() == false)
		{
			std::map<std::string, std::string> mAttributes;

			GDOMPredicateAllAttributes oDOMPredicateAllAttributes;
			std::list<GDOMAttribute> lAttr(Elem.Attribute(oDOMPredicateAllAttributes));
			if (lAttr.empty() == false)
			{
				std::list<GDOMAttribute>::iterator itr = lAttr.begin();
				std::list<GDOMAttribute>::iterator itre = lAttr.end();
				for (; itr != itre; ++itr)
				{
					mAttributes.insert(std::pair<std::string, std::string>(itr->Name(),itr->Value()));
				}
			}
			GConfigItem NewConfigItem(Elem.Name(), sValue, CreateXMLPath(vXMLPath), mAttributes);
			oConfigItemList.push_back(NewConfigItem);
		}
	}
}

void InspectTree (GDOMDocument & oDoc, std::list<GConfigItem> & oConfigItemList)
{
	GDOMPredicateAllChildren oDOMPredicateAllChildren;
	std::list<GDOMElement> lElem(oDoc.AsRootElement().Child(oDOMPredicateAllChildren,1));
	if (lElem.empty() == false)
	{
		std::vector<std::string> vXMLPath;
		std::list<GDOMElement>::iterator itr = lElem.begin();
		std::list<GDOMElement>::iterator itre = lElem.end();
		for (; itr != itre; ++itr)
		{
			WalkTree((*itr),vXMLPath,oConfigItemList);
		}
	}
}

GXMLParser::GXMLParser(std::string File, std::list<GConfigItem>& ConfigItemList)
	:	m_pConfigItemList(&ConfigItemList),
	 	m_cFile(File)
{
}

GXMLParser::~GXMLParser()
{
}

eConfigStatusType GXMLParser::Load (void)
{
	eConfigStatusType res(eConfigOk);
	GDOMDocument oDoc;
	GDOMDocument::ParseResult ParseRes(oDoc.LoadFromFile(m_cFile,GDOMEssentials::CParseFull,GDOMEssentials::eEncodingUTF8));
	if (ParseRes.Status() == GDOMDocument::ParseResult::eStatusOk)
	{
		InspectTree(oDoc, *m_pConfigItemList);
	}
	else
	{
		res = eConfigError;
	}
	return res;
}
