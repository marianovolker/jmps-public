#include "GConfigUtils.h"
#include "GXMLWriter.h"
#include "Common/Util/DEBUGOUT.h"
#include <Common/Xml/GDOMDocument.h>

using namespace GConfigTypes;


GXMLWriter::GXMLWriter (std::string sFile)
	:	m_File(sFile),
		m_DocMap()
{
}

GXMLWriter::~GXMLWriter (void)
{
}

void GXMLWriter::AddConfigItem (GConfigItem ConfigItem)
{
   m_DocMap.insert(std::pair<std::string, GConfigItem>(ConfigItem.GetXMLPath(), ConfigItem));
}

eConfigStatusType GXMLWriter::Save (void)
{
	eConfigStatusType res(eConfigOk);
	GDOMDocument oDoc;
	{
		DocMapType::iterator iter = m_DocMap.begin();
		DocMapType::iterator eiter = m_DocMap.end();
		while (iter != eiter)
		{
			GDOMElement oElem(oDoc.AsRootElement());
			{
				std::list<std::string> lXMLPath;
				GConfigUtils::StringSplit(iter->first, '/', lXMLPath);
				std::list<std::string>::iterator PathIter = lXMLPath.begin();
				std::list<std::string>::iterator PathEIter = lXMLPath.end();
				for (; PathIter!= PathEIter; ++PathIter)
				{
					std::list<GDOMElement> lChild(oElem.Child((*PathIter)));
					if (lChild.empty() == true)
					{
						oElem = oElem.AppendChild((*PathIter));
					}
					else
					{
						oElem = lChild.front();
					}
				}
			}

			std::pair<DocMapType::iterator, DocMapType::iterator> DocKeyRange = m_DocMap.equal_range(iter->first);
			iter = DocKeyRange.second;

			{
				DocMapType::iterator RangeIter = DocKeyRange.first;
				DocMapType::iterator RangeEIter = DocKeyRange.second;

				for (; RangeIter != RangeEIter; ++RangeIter)
				{
					GDOMElement oChild(oElem.AppendChild(RangeIter->second.GetName()));
					oChild.FirstChildValue(RangeIter->second.GetValue(),true);
					{
						std::list<std::string> lAttr;
						RangeIter->second.GetAttributesIndex(lAttr);
						std::list<std::string>::iterator AttrIter(lAttr.begin());
						std::list<std::string>::iterator AttrEIter(lAttr.end());
						for (; AttrIter != AttrEIter; ++AttrIter)
						{
							std::string sAttrValue;
							RangeIter->second.GetAttribute((*AttrIter),sAttrValue);
							oChild.AppendAttribute((*AttrIter)).Value(sAttrValue);
						}
					}
				}
			}
		}
	}

	if (oDoc.SerializeToFile(m_File,eOpenWriteNew,"\t",GDOMDocument::CFormatDefault|GDOMDocument::CFormatRaw,GDOMDocument::eEncodingUTF8) == false)
	{
		res = eConfigFileWriteError;
	}
	return res;
}
