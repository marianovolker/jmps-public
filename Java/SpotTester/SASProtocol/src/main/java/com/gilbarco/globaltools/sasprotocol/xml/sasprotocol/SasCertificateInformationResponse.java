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
 * @author mgrieco
 * @version 1.0
 * @created 29-Sep-2012 05:55:29 p.m.
 */
public class SasCertificateInformationResponse extends SasXmlResponseMsg
{
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private SasElement	technicianId;
	private SasElement	certificateInfo;

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasCertificateInformationResponse()
	{
		this.sasResponseType = SasResponseType.CERTIFICATE_INFORMATION_RESPONSE;
	}

	public void setTechnician(String technicianId)
	{
		this.technicianId = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, technicianId );
		this.getContent( ).getBody( ).addNew( this.technicianId );
	}

	public void setCertificate(String certificate)
	{
		this.certificateInfo = new SasElement( SasElement.SasElementTags.CERTIFICATE, SasElement.SasElementType.STRING, certificate );
		this.getContent( ).getBody( ).addNew( this.certificateInfo );
	}

	public String getCertificate()
	{
		return certificateInfo.getValue( );
	}

	public String getTechnician()
	{
		return technicianId.getValue( );
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
		else if (elementName.equals( SasElement.SasElementTags.CERTIFICATE.getName( ) ))
		{
			setCertificate( xmlReader.getElementText( ) );
		}
		else
		{
			return false;
		}

		return true;
	}
}
