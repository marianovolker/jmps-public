/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.sasprotocol.xml.sasprotocol;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;

import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlRequestMsg;

public class SasRemoveCertificateRequest extends SasXmlRequestMsg
{
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private SasElement	technicianId = null;

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasRemoveCertificateRequest()
	{
		this.sasRequestType = SasRequestType.REMOVE_CERTIFICATE_REQUEST;
	}

	public void setTechnicianId(String laptopid)
	{
		this.technicianId = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, laptopid );
		this.getContent( ).getBody( ).addNew( this.technicianId );
	}

	public String getTechnicianId()
	{
		return technicianId.getValue( );
	}

	/**
	 * @return decode each part of the message
	 * @throws XMLStreamException
	 * @throws NumberFormatException
	 */
	@Override
	public boolean decode(XMLStreamReader xmlReader) throws NumberFormatException, XMLStreamException
	{
		// To parse the signature of the message and another common things.
		if (super.decode( xmlReader ))
		{
			return true;
		}

		String elementName = xmlReader.getLocalName( );
		if (elementName.equals( SasElement.SasElementTags.TECHNICIANID.getName( ) ))
		{
			this.setTechnicianId( xmlReader.getElementText( ) );
		}
		else
		{
			return false;
		}
		return true;
	}
}
