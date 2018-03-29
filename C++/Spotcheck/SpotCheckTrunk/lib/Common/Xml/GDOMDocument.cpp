/*
 * GDOMDocument.cpp
 *
 *  Created on: Jun 29, 2011
 *      Author: pcantarini
 */

#include <pugixml.hpp>

#include "Common/Xml/GDOMDocument.h"

class GDOMDocumentXMLWriterPrivate
	:	public GDOMXMLWriter
{
public:
	virtual ~GDOMDocumentXMLWriterPrivate (void);
	virtual void SerializedChunk (void const * vkpContents, unsigned int uiSize);
	std::vector<unsigned char> & SerializedBuffer(void);
	std::vector<unsigned char> const & SerializedBuffer(void) const;
private:
	std::vector<unsigned char> m_vSerializedBuffer;
};

GDOMDocumentXMLWriterPrivate::~GDOMDocumentXMLWriterPrivate (void)
{
}
void GDOMDocumentXMLWriterPrivate::SerializedChunk (void const * vkpContents, unsigned int uiSize)
{
	m_vSerializedBuffer.insert(m_vSerializedBuffer.end(),static_cast<unsigned char const *>(vkpContents),(static_cast<unsigned char const *>(vkpContents)+uiSize));
}
std::vector<unsigned char> & GDOMDocumentXMLWriterPrivate::SerializedBuffer(void)
{
	return m_vSerializedBuffer;
}
std::vector<unsigned char> const & GDOMDocumentXMLWriterPrivate::SerializedBuffer(void) const
{
	return m_vSerializedBuffer;
}

GDOMDocument::ParseResult::ParseResult (void)
	:	GDOMEssentials(),
	 	m_ref()
{
}
GDOMDocument::ParseResult::ParseResult (GDOMDocument::ParseResult::GDOMDocumentParseResultRef const & rhs)
	:	GDOMEssentials(),
	 	m_ref(rhs)
{
}
GDOMDocument::ParseResult::~ParseResult (void)
{
}
GDOMDocument::ParseResult::operator GDOMDocument::ParseResult::GDOMDocumentParseResultRef const & (void) const
{
	return m_ref;
}
GDOMDocument::ParseResult::etStatus GDOMDocument::ParseResult::Status (void) const
{
	GDOMDocument::ParseResult::etStatus res(GDOMDocument::ParseResult::eStatusUnknownError);

	switch (m_ref.Referenced().status)
	{
	case ::pugi::status_ok:
		res = GDOMDocument::ParseResult::eStatusOk;
		break;

	case ::pugi::status_file_not_found:
		res = GDOMDocument::ParseResult::eStatusFileNotFound;
		break;
	case ::pugi::status_io_error:
		res = GDOMDocument::ParseResult::eStatusIOError;
		break;
	case ::pugi::status_out_of_memory:
		res = GDOMDocument::ParseResult::eStatusOutOfMemory;
		break;
	case ::pugi::status_internal_error:
		res = GDOMDocument::ParseResult::eStatusInternalError;
		break;

	case ::pugi::status_unrecognized_tag:
		res = GDOMDocument::ParseResult::eStatusUnrecognizedTag;
		break;

	case ::pugi::status_bad_pi:
		res = GDOMDocument::ParseResult::eStatusBadPI;
		break;
	case ::pugi::status_bad_comment:
		res = GDOMDocument::ParseResult::eStatusBadComment;
		break;
	case ::pugi::status_bad_cdata:
		res = GDOMDocument::ParseResult::eStatusBadCData;
		break;
	case ::pugi::status_bad_doctype:
		res = GDOMDocument::ParseResult::eStatusBadDoctype;
		break;
	case ::pugi::status_bad_pcdata:
		res = GDOMDocument::ParseResult::eStatusBadPCData;
		break;
	case ::pugi::status_bad_start_element:
		res = GDOMDocument::ParseResult::eStatusBadStartElement;
		break;
	case ::pugi::status_bad_attribute:
		res = GDOMDocument::ParseResult::eStatusBadAttribute;
		break;
	case ::pugi::status_bad_end_element:
		res = GDOMDocument::ParseResult::eStatusBadEndElement;
		break;
	case ::pugi::status_end_element_mismatch:
		res = GDOMDocument::ParseResult::eStatusEndElementMismatch;
		break;
	default:
		break;
	}
	return res;
}
signed int GDOMDocument::ParseResult::Offset (void) const
{
	return static_cast<signed int>(m_ref.Referenced().offset);
}
GDOMDocument::etEncoding GDOMDocument::ParseResult::Encoding (void) const
{
	return GDOMEssentials::FromEncodingImplToGDOM(m_ref.Referenced().encoding);
}
std::string GDOMDocument::ParseResult::Description (void) const
{
	return static_cast<char const *>(m_ref.Referenced().description());
}
GDOMDocument::ParseResult::operator bool (void) const
{
	return static_cast<bool>(m_ref.Referenced());
}

GDOMDocument::GDOMDocument (void)
	:	GDOMEssentials(),
		m_ref()
{
}
GDOMDocument::GDOMDocument (GDOMDocument const & rhs)
	:	GDOMEssentials(),
		m_ref()

{
	m_ref.reset(rhs.m_ref);
}
GDOMDocument::~GDOMDocument (void)
{
}
GDOMDocument & GDOMDocument::operator = (GDOMDocument const & rhs)
{
	m_ref.reset(rhs.m_ref);
	return (*this);
}
GDOMElement GDOMDocument::DocumentElement (void) const
{
	return GDOMEssentials::GDOMElementRef(m_ref.document_element());
}
GDOMElement GDOMDocument::AsRootElement (void) const
{
	return GDOMEssentials::GDOMElementRef(m_ref);
}
void GDOMDocument::Reset (void)
{
	m_ref.reset();
}
void GDOMDocument::Reset (GDOMDocument const & rhs)
{
	m_ref.reset(rhs.m_ref);
}
GDOMDocument::ParseResult GDOMDocument::Load (std::string sContents, unsigned int uiParse)
{
	return GDOMDocument::GDOMDocumentParseResultRef(m_ref.load(sContents.c_str(),GDOMEssentials::FromParseFlagGDOMToImpl(uiParse)));
}
GDOMDocument::ParseResult GDOMDocument::Load (std::vector<unsigned char> vContents, unsigned int uiParse, GDOMDocument::etEncoding evEncoding)
{
	unsigned char * ucpContents(NULL);
	unsigned int uiSize(0);
	if (vContents.empty() == false)
	{
		ucpContents = static_cast<unsigned char *>(&vContents[0]);
		uiSize = vContents.size();
	}
	return GDOMDocument::GDOMDocumentParseResultRef(m_ref.load_buffer(static_cast<void const *>(ucpContents),uiSize,GDOMEssentials::FromParseFlagGDOMToImpl(uiParse),GDOMEssentials::FromEncodingGDOMToImpl(evEncoding)));
}
GDOMDocument::ParseResult GDOMDocument::LoadFromRawBuffer (void const * vpContents, unsigned int uiSize, unsigned int uiParse, GDOMDocument::etEncoding evEncoding)
{
	return GDOMDocument::GDOMDocumentParseResultRef(m_ref.load_buffer(vpContents,uiSize,GDOMEssentials::FromParseFlagGDOMToImpl(uiParse),GDOMEssentials::FromEncodingGDOMToImpl(evEncoding)));
}
void GDOMDocument::Serialize (std::string & sContents, std::string sIndent, unsigned int uiFormat) const
{
	GDOMDocumentXMLWriterPrivate oWriter;
	GDOMEssentials::GDOMXMLWriterPrivateImplProxy oWriterProxy(&oWriter);
	m_ref.save(oWriterProxy,sIndent.c_str(),GDOMEssentials::FromFormatFlagGDOMToImpl(uiFormat),GDOMEssentials::FromEncodingGDOMToImpl(GDOMEssentials::eEncodingUTF8));
	if (oWriter.SerializedBuffer().empty() == false)
	{
		sContents.assign(static_cast<char const *>(static_cast<void const *>(&(oWriter.SerializedBuffer()[0]))),oWriter.SerializedBuffer().size());
	}
}
void GDOMDocument::Serialize (std::vector<unsigned char> & vContents, std::string sIndent, unsigned int uiFormat, GDOMDocument::etEncoding evEncoding) const
{
	GDOMDocumentXMLWriterPrivate oWriter;
	GDOMEssentials::GDOMXMLWriterPrivateImplProxy oWriterProxy(&oWriter);
	m_ref.save(oWriterProxy,sIndent.c_str(),GDOMEssentials::FromFormatFlagGDOMToImpl(uiFormat),GDOMEssentials::FromEncodingGDOMToImpl(evEncoding));
	vContents = oWriter.SerializedBuffer();
}
void GDOMDocument::SerializeToObserver (GDOMXMLWriter & oWriter, std::string sIndent, unsigned int uiFormat, GDOMDocument::etEncoding evEncoding) const
{
	GDOMEssentials::GDOMXMLWriterPrivateImplProxy oWriterProxy(&oWriter);
	m_ref.save(oWriterProxy,sIndent.c_str(),GDOMEssentials::FromFormatFlagGDOMToImpl(uiFormat),GDOMEssentials::FromEncodingGDOMToImpl(evEncoding));
}

#include <GFile.h>

GDOMDocument::ParseResult GDOMDocument::LoadFromFile (std::string sFileName, unsigned int uiParse, GDOMDocument::etEncoding evEncoding)
{
	GDOMDocument::ParseResult gResult;
	GFile gFile(sFileName);
	if (gFile.Open(eOpenReadOnly))
	{
		bool bOk(false);
		std::vector<unsigned char> vContents;
		{
			static int const CReadBufferSize(512);
			unsigned char ucaBuffer[CReadBufferSize];
			int iBytesRead(0);
			do
			{
				vContents.insert(vContents.end(),ucaBuffer,(ucaBuffer+iBytesRead));
				bOk = gFile.Read(static_cast<void *>(ucaBuffer),CReadBufferSize,iBytesRead);
			}
			while ((bOk) && (iBytesRead > 0));
		}
		if (bOk)
		{
			unsigned char * ucpContents(NULL);
			unsigned int uiSize(0);
			if (vContents.empty() == false)
			{
				ucpContents = static_cast<unsigned char *>(&vContents[0]);
				uiSize = vContents.size();
			}
			gResult = GDOMDocument::GDOMDocumentParseResultRef(m_ref.load_buffer(static_cast<void const *>(ucpContents),uiSize,GDOMEssentials::FromParseFlagGDOMToImpl(uiParse),GDOMEssentials::FromEncodingGDOMToImpl(evEncoding)));
		}
	}
	return gResult;
}
bool GDOMDocument::SerializeToFile (std::string sFileName, eFileOpenMode evOpenMode, std::string sIndent, unsigned int uiFormat, GDOMDocument::etEncoding evEncoding) const
{
	bool bOk(false);
	GFile gFile(sFileName);
	if (gFile.Open(evOpenMode))
	{
		std::vector<unsigned char> vContents;
		{
			GDOMDocumentXMLWriterPrivate oWriter;
			GDOMEssentials::GDOMXMLWriterPrivateImplProxy oWriterProxy(&oWriter);
			m_ref.save(oWriterProxy,sIndent.c_str(),GDOMEssentials::FromFormatFlagGDOMToImpl(uiFormat),GDOMEssentials::FromEncodingGDOMToImpl(evEncoding));
			vContents = oWriter.SerializedBuffer();
		}
		{
			unsigned char * ucpContents(NULL);
			unsigned int uiSize(0);
			if (vContents.empty() == false)
			{
				ucpContents = static_cast<unsigned char *>(&vContents[0]);
				uiSize = vContents.size();
			}
			unsigned int uiBytesWritten(0);
			bOk = true;
			while ((bOk) && (uiBytesWritten < uiSize))
			{
				int iBytesWrittenNext(0);
				bOk = gFile.Write(static_cast<void *>(ucpContents),(uiSize - uiBytesWritten),iBytesWrittenNext);
				if (iBytesWrittenNext > 0)
				{
					uiBytesWritten += static_cast<unsigned int>(iBytesWrittenNext);
				}
			}
		}
	}
	return bOk;
}
