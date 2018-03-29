/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * Additional comments for the file . . .
 */

package com.gilbarco.globaltools.sasprotocol.xml.sasprotocol;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;

import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlResponseMsg;

/**
 * @author mvolker
 * @version 1.0
 * @created 04-Jun-2012 05:55:29 p.m.
 */
public class SasPublishCertificateResponse extends SasXmlResponseMsg
{
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private SasElement	technician;
	private SasElement	validationCode;

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasPublishCertificateResponse()
	{
		this.sasResponseType = SasResponseType.PUBLISH_CERTIFICATE_RESPONSE;
	}

	public void setTechnician(String technicianID)
	{
		this.technician = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, technicianID );
		this.getContent( ).getBody( ).addNew( this.technician );
	}

	public void setValidationCode(String validationCode)
	{
		this.validationCode = new SasElement( SasElement.SasElementTags.VALIDATION_CODE, SasElement.SasElementType.STRING, validationCode );
		this.getContent( ).getBody( ).addNew( this.validationCode );
	}
	
	public String getValidationCode()
	{
		return validationCode.getValue( );
	}

	public String getTechnicianID()
	{
		return this.technician.getValue( );
	}

	/**
	 * @return decode each part of the message
	 * @throws XMLStreamException
	 * @throws NumberFormatException
	 */
	@Override
	public boolean decode(XMLStreamReader xmlReader) throws NumberFormatException,
													XMLStreamException
	{
		// To parse the signature of the message and another common things.
		if (super.decode( xmlReader ))
		{
			return true;
		}

		String elementName = xmlReader.getLocalName( );

		if (elementName.equals( SasElement.SasElementTags.TECHNICIANID.getName( ) ))
		{
			setTechnician( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.VALIDATION_CODE.getName( ) ))
		{
			setValidationCode( xmlReader.getElementText( ) );
		}
		else
		{
			return false;
		}

		return true;
	}
}
