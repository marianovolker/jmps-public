/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * Additional comments for the file . . .
 */

package com.gilbarco.globaltools.sasprotocol.xml.sasprotocol;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;
import javax.xml.stream.XMLStreamWriter;

import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlRequestMsg;

/**
 * @author mvolker
 * @version 1.0
 * @created 04-Jun-2012 05:55:09 p.m.
 */
public class SasPublishCertificateRequest extends SasXmlRequestMsg
{
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************	
	private SasElement technician;
	private SasElement certificate;

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasPublishCertificateRequest()
	{
		this.sasRequestType = SasRequestType.PUBLISH_CERTIFICATE_REQUEST;
	}
	
	public void setTechnician( String technicianID )
	{
		this.technician = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, technicianID );
		this.getContent().getBody().addNew( this.technician );		
	}
	
	public void setCertificate( String cert )
	{
		this.certificate = new SasElement( SasElement.SasElementTags.CERTIFICATE, SasElement.SasElementType.STRING, cert );
		this.getContent().getBody().addNew( this.certificate );		
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
		if( super.decode(xmlReader) )
		{
			return true;
		}
		
		String elementName = xmlReader.getLocalName( );
		
		if (elementName.equals( SasElement.SasElementTags.TECHNICIANID.getName( ) ))
		{
			setTechnician(xmlReader.getElementText( ));
		}
		else if (elementName.equals( SasElement.SasElementTags.CERTIFICATE.getName( ) ))
		{
			setCertificate(xmlReader.getElementText( ));
		}
		else
		{
			return false;
		}

		return true;
	}
	
	/**
	 * @return encode the message
	 * @throws XMLStreamException
	 */
	@Override
	public void encode(XMLStreamWriter writer) throws XMLStreamException
	{
		writer.writeStartElement( this.content.getTagName( ).getName( ) );
		
		if(isPrintable()) writer.writeCharacters( "\n" );
		
			writer.writeStartElement( this.content.getCommand( ).getTagName( ).getName( ) );
			writer.writeCharacters( sasRequestType.getName( ) );
			writer.writeEndElement( );
		
			this.content.getBody( ).toXML( writer );
			
		writer.writeEndElement( );
		
		if(isPrintable()) writer.writeCharacters( "\n" );
	}	
}
