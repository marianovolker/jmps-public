/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.sasprotocol.xml.messages;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SignatureException;

import javax.xml.stream.XMLOutputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;
import javax.xml.stream.XMLStreamWriter;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.util.EdtSignData;

/**
 * Base class used by implementations of SasXmlService for exchanging xml messages from and to the SAS Token Server
 * 
 * @author mvolker
 * 
 */
public abstract class SasMessage
{
	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected SasVanguardTokenElement	content		= null;
	protected Boolean					printable	= null;
	protected String					signature	= null;	
	
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( SasMessage.class );

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public SasMessage(SasCommand command)
	{
		this.printable = Boolean.valueOf( false );
		this.content = new SasVanguardTokenElement( command, new SasGroupElement( SasElement.SasElementTags.BODY ) );
	}

	/**
	 * @return decode each part of the message
	 * @throws XMLStreamException
	 * @throws NumberFormatException
	 */
	public boolean decode(XMLStreamReader xmlReader) throws NumberFormatException, XMLStreamException
	{
		String elementName = xmlReader.getLocalName( );

		if (elementName.equals( SasElement.SasElementTags.SIGNATURE.getName( ) ))
		{
			this.signature = xmlReader.getElementText( );
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
	public abstract void encode(XMLStreamWriter writer) throws XMLStreamException;

	/**
	 * @return the content
	 */
	public SasVanguardTokenElement getContent()
	{
		return content;
	}

	/**
	 * This method is used to verify if the signed data received is valid.
	 * 
	 * @return none.
	 * @throws XMLStreamException
	 */
	public void verifySignedData() throws XMLStreamException
	{
		boolean result = false;
		try
		{
			result = EdtSignData.getInstance( ).verifySignedData( getBody( ), signature );
		}
		catch (InvalidKeyException | NoSuchAlgorithmException | SignatureException e)
		{
			logger.error("verifySignedData error - " + e.getMessage( ) );
			result = false;
		}

		if (!result)
		{
			throw new XMLStreamException( "The signature or message format is not valid." );
		}
	}

	@Override
	public String toString()
	{
		XMLOutputFactory outputFactory = null;
		outputFactory = XMLOutputFactory.newFactory( );
		ByteArrayOutputStream out = new ByteArrayOutputStream( );
		XMLStreamWriter writer;

		try
		{
			writer = outputFactory.createXMLStreamWriter( out );
			writer.writeStartDocument( SasElement.SasElementTags.XML_VERSION.getName( ) );
			writer.writeCharacters( "\n" );

			setPrintable( true );
			encode( writer );

			writer.flush( );
			writer.close( );
		}
		catch (XMLStreamException e)
		{
			logger.error( e.getMessage( ) );
		}

		return new String( out.toByteArray( ) );
	}

	/**
	 * @return the signature
	 */
	public String getSignature()
	{
		return signature;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	/**
	 * This method is to set the message as printable for debug purpose.
	 * 
	 * @return none.
	 */
	protected void setPrintable(Boolean printable)
	{
		this.printable = printable;
		content.setPrintable( printable );
	}

	/**
	 * This method is to verify if the message is set as printable, just for debug purpose.
	 * 
	 * @return true if is printable.
	 */
	protected boolean isPrintable()
	{
		return (printable.equals( true ));
	}

	/**
	 * This method is used to write the signed data as XML rounded by corresponding tags.
	 * 
	 * @return none
	 * @throws XMLStreamException
	 */
	protected void writeSignedDataTag(SasGroupElement body, XMLStreamWriter writer)
																					throws XMLStreamException
	{
		writer.writeStartElement( SasElement.SasElementTags.X509_CERTIFICATE.getName( ) );
		writer.writeStartElement( SasElement.SasElementTags.SIGNATURE.getName( ) );

		try
		{
			XMLOutputFactory outputFactory = null;
			outputFactory = XMLOutputFactory.newFactory( );
			ByteArrayOutputStream data = new ByteArrayOutputStream( );
			XMLStreamWriter temporaryWriter = outputFactory.createXMLStreamWriter( data );
			body.toXML( temporaryWriter );
			temporaryWriter.flush( );
			temporaryWriter.close( );

			signature = EdtSignData.getInstance( ).signDataSHA256withRSA( data.toByteArray( ) );
			writer.writeCharacters( signature );
		}
		catch (XMLStreamException | InvalidKeyException | NoSuchAlgorithmException | SignatureException | IOException e)
		{
			logger.error( e.getMessage( ) );
			throw new XMLStreamException(e);
		}

		writer.writeEndElement( );
		writer.writeEndElement( );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	/**
	 * This method is used to obtain the body of the message without any space character inside tags.
	 * 
	 * @return the current body as a byte array.
	 */
	private byte[] getBody()
	{
		try
		{
			XMLOutputFactory outputFactory = null;
			outputFactory = XMLOutputFactory.newFactory( );
			ByteArrayOutputStream data = new ByteArrayOutputStream( );
			XMLStreamWriter temporaryWriter = outputFactory.createXMLStreamWriter( data );
			this.content.getBody( ).toXML( temporaryWriter );
			temporaryWriter.flush( );
			temporaryWriter.close( );

			return data.toByteArray( );
		}
		catch (XMLStreamException e)
		{
			logger.error( e.getMessage( ) );
		}

		return null;
	}
}
