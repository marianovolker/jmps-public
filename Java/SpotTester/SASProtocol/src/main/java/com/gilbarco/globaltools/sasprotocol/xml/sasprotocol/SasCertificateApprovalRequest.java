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
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlRequestMsg;

public class SasCertificateApprovalRequest extends SasXmlRequestMsg
{
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private SasElement 		technicianId 	= null;
	private SasElement 		validationCode  = null;

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasCertificateApprovalRequest()
	{
		this.sasRequestType = SasRequestType.CERTIFICATE_APPROVAL_REQUEST;
	}

	public void setTechnicianId(String technicianId)
	{
		this.technicianId = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, technicianId );
		this.getContent( ).getBody( ).addNew( this.technicianId );
	}

	public void setValidationCode(String validationcode)
	{
		this.validationCode = new SasElement( SasElement.SasElementTags.VALIDATION_CODE, SasElement.SasElementType.STRING, validationcode );
		this.getContent( ).getBody( ).addNew( this.validationCode );
	}

	public String getTechnicianId()
	{
		return technicianId.getValue( );
	}

	public String getValidationCode()
	{
		return validationCode.getValue( );
	}
	
	/**
	 * @return decode each part of the message
	 * @throws XMLStreamException
	 * @throws NumberFormatException
	 */
	@Override
	public boolean decode(XMLStreamReader xmlReader) throws NumberFormatException,XMLStreamException
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
		else if (elementName.equals( SasElement.SasElementTags.VALIDATION_CODE.getName( ) ))
		{
			this.setValidationCode( xmlReader.getElementText( ) );
		}
		else
		{
			return false;
		}

		return true;
	}	
}
