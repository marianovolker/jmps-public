/*
 * GDOMXMLWriter.h
 *
 *  Created on: Jun 24, 2011
 *      Author: pcantarini
 */

#ifndef GDOMXMLWriter_h
#define GDOMXMLWriter_h

/*! @class GDOMXMLWriter GDOMXMLWriter.h "Src/Common/Xml/GDOMXMLWriter.h"
 * @brief This class is an abstract base class for the user DOM serializing writer observer.
 *
 */
class GDOMXMLWriter
{
public:
	//! @brief Destructor.
	virtual ~GDOMXMLWriter (void);
	/*! @brief DOM serializing observer callback method to be implemented by the user.
	 *
	 * @param vkpData Pointer to the buffer chunk to be dumped.
	 * @param uiSize Size of the buffer chunk to be dumped.
	 */
	virtual void SerializedChunk (void const * vkpData, unsigned int uiSize) = 0;
};

#endif
