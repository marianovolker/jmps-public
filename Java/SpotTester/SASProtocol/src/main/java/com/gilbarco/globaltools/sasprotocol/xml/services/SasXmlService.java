/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.services;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

import javax.xml.stream.XMLStreamException;

import com.gilbarco.globaltools.sasprotocol.xml.messages.SasMessage;

public interface SasXmlService
{

	public SasMessage unmarshall(File schema, InputStream xml) throws XMLStreamException,
																IOException;

	public InputStream marshall(SasMessage message, File schema) throws XMLStreamException,
																IOException;
}
