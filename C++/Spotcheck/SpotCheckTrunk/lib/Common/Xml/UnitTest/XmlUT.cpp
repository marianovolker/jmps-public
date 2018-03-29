/*
 * XmlUT.cpp
 *
 *  Created on: Jun 22, 2011
 *      Author: pcantarini
 */

#include "XmlUT.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Common/Xml/GDOMDocument.h"
#include <GFile.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "XMLTestSet.h"

XmlUT::XmlUT (void)
{
}

XmlUT::~XmlUT (void)
{
}

void XmlUT::SetUp (void)
{
}

void XmlUT::TearDown(void)
{
}

class MyDOMXMLWriter
	:	public GDOMXMLWriter
{
public:
		MyDOMXMLWriter()
		:	GDOMXMLWriter(),
			m_sXMLBuffer()
	{}
	virtual ~MyDOMXMLWriter()
	{}

	// Write memory chunk into stream/file/whatever
	virtual void SerializedChunk (void const * pvData, unsigned int uiSize)
	{
		m_sXMLBuffer.append(static_cast<char const *>(pvData),uiSize);
	}

	std::string & getXMLBuffer (void)
	{
		return m_sXMLBuffer;
	}
protected:
	std::string m_sXMLBuffer;
};

class MyDOMPredicate
	:	public GDOMPredicateForElement
{
public:
	virtual ~MyDOMPredicate (void)
	{
	}

	bool operator () (GDOMElement const & r)
	{
		return true;
	}
};

class MyDOMPredicateVie
	:	public GDOMPredicateForElement
{
public:
	virtual ~MyDOMPredicateVie (void)
	{
	}

	bool operator () (GDOMElement const & r)
	{
		return r.Name() == "vie";
	}
};

static char const CXMLTestFileName[] = "XMLTestFile.xml";
static char const CXMLTestFileNameOutcome[] = "XMLTestFileOutcome.xml";
static char const CTextIamGraterThanYou[] = "I > U";

TEST_F(XmlUT, TestXMLBufferProcessing)
{
	GDOMDocument gDoc;
	{
		char const * ckpContents(acXMLTest1);
		GDOMDocument::ParseResult gResult(gDoc.Load(ckpContents, GDOMDocument::CParseFull));
		EXPECT_EQ(GDOMDocument::ParseResult::eStatusOk,gResult.Status());
	}
	{
		std::list<GDOMElement> elemList (gDoc.AsRootElement().Child("root"));
		EXPECT_FALSE(elemList.empty());
		if (!elemList.empty())
		{
			std::string sAttrValue;
			std::list<GDOMAttribute> attrList(elemList.front().Attribute("name"));
			EXPECT_FALSE(attrList.empty());
			if (!attrList.empty())
			{
				sAttrValue = attrList.front().AsString();
			}
			EXPECT_STREQ("RootNode",sAttrValue.c_str());
		}
	}
	{
		MyDOMXMLWriter myWriter;
		gDoc.SerializeToObserver(myWriter, "\t", GDOMDocument::CFormatDefault|GDOMDocument::CFormatRaw, GDOMDocument::eEncodingUTF8);
		EXPECT_FALSE(myWriter.getXMLBuffer().empty());
	}
}

TEST_F(XmlUT, TestXMLFileProcessing)
{
	unlink(CXMLTestFileName);
	unlink(CXMLTestFileNameOutcome);
	{
		bool bRet(false);
		GFile gFile(CXMLTestFileName);

		bRet = gFile.Open(eOpenWriteNew);
		EXPECT_TRUE(bRet);

		{
			bool bOk(true);
			char const * ckpContents(acXMLTest2);
			unsigned int uiSize(strlen(acXMLTest2));
			unsigned int uiBytesWritten(0);
			while ((bOk) && (uiBytesWritten < uiSize))
			{
				int iBytesWrittenNext(0);
				bOk = gFile.Write(static_cast<void const *>(ckpContents),(uiSize - uiBytesWritten),iBytesWrittenNext);
				if (iBytesWrittenNext > 0)
				{
					uiBytesWritten += static_cast<unsigned int>(iBytesWrittenNext);
				}
			}
			bRet = bOk;
		}
		EXPECT_TRUE(bRet);

		bRet = gFile.Close();
		EXPECT_TRUE(bRet);
	}

	GDOMDocument gDoc;
	{
		GDOMDocument::ParseResult gResult(gDoc.LoadFromFile(CXMLTestFileName, GDOMDocument::CParseFull, GDOMDocument::eEncodingUTF8));
		EXPECT_EQ(GDOMDocument::ParseResult::eStatusOk,gResult.Status());
	}
	{
		std::list<GDOMElement> elemList (gDoc.AsRootElement().Child("root"));
		EXPECT_FALSE(elemList.empty());
		if (!elemList.empty())
		{
			std::string sAttrValue;
			std::list<GDOMAttribute> attrList(elemList.front().Attribute("xmlns"));
			EXPECT_FALSE(attrList.empty());
			if (!attrList.empty())
			{
				sAttrValue = attrList.front().AsString();
			}
			EXPECT_STREQ("http://www.mynamespace.com/cracatoa/",sAttrValue.c_str());
		}
	}
	{
		std::list<GDOMElement> elemList (gDoc.AsRootElement().Child("root"));
		EXPECT_FALSE(elemList.empty());
		if (!elemList.empty())
		{
			elemList.front().FirstChildValue("xoxo><><xoxo>>xo&&xo<<xoxo><><xoxo",false);
			elemList.front().FirstChildValue(CTextIamGraterThanYou,true);
			elemList.front().InsertChild("Dummy");
			EXPECT_STREQ(CTextIamGraterThanYou,elemList.front().FirstChildValue().c_str());
		}
	}
	{
		std::list<GDOMElement> elemList (gDoc.AsRootElement().Child("root"));
		EXPECT_FALSE(elemList.empty());
		if (!elemList.empty())
		{
			GDOMElement gElement(elemList.front());
			gElement.AppendChild("Cest").InsertChild("la").AppendChild("vie");
			elemList.clear();
			{
				MyDOMPredicateVie gMyDOMPredicateVie;
				elemList = gElement.Child(gMyDOMPredicateVie,GDOMElement::CMaxSearchDeph);
				EXPECT_FALSE(elemList.empty());
			}
		}
	}
	{
		std::list<GDOMElement> elemList (gDoc.AsRootElement().Child("root"));
		EXPECT_FALSE(elemList.empty());
		if (!elemList.empty())
		{
			GDOMElement gElement(elemList.front());
			gElement.AppendChild("Dumber");
			elemList.clear();
			{
				elemList = gElement.Child("Dumber");
				EXPECT_FALSE(elemList.empty());
				if (!elemList.empty())
				{
					gElement.RemoveChild("Dumber");
					elemList.clear();
					{
						elemList = gElement.Child("Dumber");
						EXPECT_TRUE(elemList.empty());
					}
				}
			}
		}
	}
	{
		std::list<GDOMElement> elemList (gDoc.AsRootElement().Child("root"));
		EXPECT_FALSE(elemList.empty());
		if (!elemList.empty())
		{
			GDOMElement gParent(elemList.front());
			gParent.InsertChild("Dumb");
			elemList.clear();
			{
				elemList = gParent.Child("Dumb");
				EXPECT_FALSE(elemList.empty());
				if (!elemList.empty())
				{
					GDOMElement gElement(elemList.front());
					EXPECT_TRUE(gElement!=gParent);
					EXPECT_TRUE(gElement.Parent()==gParent);
				}
			}
		}
	}
	{
		std::list<GDOMElement> elemList (gDoc.AsRootElement().Child("root"));
		EXPECT_FALSE(elemList.empty());
		if (!elemList.empty())
		{
			elemList.front().InsertAttribute("MyNewAttributeInt").Value(2000);
			elemList.front().AppendAttribute("MyNewAttributeDouble").Value(20.25);
			std::string sAttrValue;
			std::list<GDOMAttribute> attrList(elemList.front().Attribute("MyNewAttributeDouble"));
			EXPECT_FALSE(attrList.empty());
			if (!attrList.empty())
			{
				sAttrValue = attrList.front().AsString();
			}
			EXPECT_STREQ("20.25",sAttrValue.c_str());
		}
	}
	{
		bool bSaved(gDoc.SerializeToFile(CXMLTestFileNameOutcome,eOpenWriteNew,"\t",GDOMDocument::CFormatDefault|GDOMDocument::CFormatRaw,GDOMDocument::eEncodingUTF8));
		EXPECT_TRUE(bSaved);
	}
	GDOMDocument gDocSaved;
	{
		{
			GDOMDocument::ParseResult gResult(gDocSaved.LoadFromFile(CXMLTestFileNameOutcome, GDOMDocument::CParseFull, GDOMDocument::eEncodingUTF8));
			EXPECT_EQ(GDOMDocument::ParseResult::eStatusOk,gResult.Status());
		}
		std::string sOriginalContentSaved;
		std::string sContentSaved;
		gDoc.Serialize(sOriginalContentSaved,"\t",GDOMDocument::CFormatDefault|GDOMDocument::CFormatRaw);
		gDocSaved.Serialize(sContentSaved,"\t",GDOMDocument::CFormatDefault|GDOMDocument::CFormatRaw);
		EXPECT_STREQ(sContentSaved.c_str(),sOriginalContentSaved.c_str());
	}
	{
		std::list<GDOMElement> lList;
		MyDOMPredicate myPredicate;
		lList = gDoc.AsRootElement().Child(myPredicate,GDOMDocument::CMaxSearchDeph);
		EXPECT_FALSE(lList.empty());
	}
	{
		std::list<GDOMElement> lList;
		MyDOMPredicate myPredicate;
		lList = gDocSaved.AsRootElement().Child(myPredicate,GDOMDocument::CMaxSearchDeph);
		EXPECT_FALSE(lList.empty());
	}

	// Remove test file.
	unlink(CXMLTestFileNameOutcome);
	unlink(CXMLTestFileName);
}
