/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.services;

import java.io.IOException;
import java.io.InputStream;

import org.xml.sax.SAXException;

public interface SasXmlValidable
{
	public boolean validate(InputStream input) throws SAXException, IOException;
}
