/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.messages;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;
import javax.xml.stream.XMLStreamWriter;



public class SasXmlErrorCodeMsg extends SasMessage
{
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************	
	private SasElement errorCode;
	private SasElement description;


	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************	
	public SasXmlErrorCodeMsg( )
	{
		super( new SasCommand(SasCommand.SasCommandTypes.ERRORCODE) );
	}

	public void setErrorCode( String errorCode )
	{
		this.errorCode = new SasElement( SasElement.SasElementTags.CODE, SasElement.SasElementType.INTEGER, errorCode);
		this.getContent().getBody().addNew( this.errorCode );		
	}
	
	public void setDescription ( String description )
	{
		this.description = new SasElement( SasElement.SasElementTags.DESCRIPTION, SasElement.SasElementType.STRING, description );
		this.getContent().getBody().addNew( this.description );		
	}
	
	public SasElement getErrorCode()
	{
		return errorCode;
	}

	public SasElement getDescription()
	{
		return description;
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
		if( super.decode(xmlReader) )
		{
			return true;
		}

		String elementName = xmlReader.getLocalName( );
		
		if (elementName.equals( SasElement.SasElementTags.CODE.getName( ) ))
		{
			setErrorCode( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.DESCRIPTION.getName( ) ))
		{
			setDescription( xmlReader.getElementText( ) );
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
		
		this.content.getCommand( ).toXML( writer );
		this.content.getBody( ).toXML( writer );

		writer.writeEndElement( );
		
		if(isPrintable()) writer.writeCharacters( "\n" );
	}

	public enum SasErrorCodeType
	{
		/* Extracted from specification.
		 * If SAS server side has been changed these values, this list need to be update too.
		0000		No error.
		0001		Technician’s certificate could not be validated by the certificate authority.
		0002		Technician’s signature could not be validated.
		0003		Invalid technician identification number (Technician-ID field).
		0004		Server’s signature could not be validated.
		0005		Invalid request or invalid xml message.
		0006		TAC’s certificate could not be validated.
		0007		TAC’s signature could not be validated.
		0008		Wrong validation code.
		0009		Wrong token code.
		0010		Wrong token time window.
		0011		There is no certificate.
		0012		Invalid unit PPN.
		0013		Secure Authentication Server Internal Error
		0014		Secure Authentication Server is busy
		0015		No Token found for given TechnitianId-TargetUnit*/
		
		NO_ERROR											(0,	"NO ERROR"),
		BAD_CERTIFICATE										(1,	"TECHNICIAN'S CERTIFICATE COULD NOT BE VALIDATED BY THE CERTIFICATE AUTHORITY"),
		BAD_TECHNICIAN_SIGNATURE							(2,	"TECHNICIAN'S SIGNATURE COULD NOT BE VALIDATED"),
		INVALID_TECHNICIAN									(3,	"INVALID TECHNICIAN IDENTIFICATION NUMBER (TECHNICIAN-ID FIELD)"),
		BAD_SERVER_SIGNATURE								(4,	"SERVER'S SIGNATURE COULD NOT BE VALIDATED"),
		INVALID_REQUEST										(5,	"INVALID REQUEST OR INVALID XML MESSAGE"),
		BAD_TAC_CERTIFICATE									(6,	"TAC'S CERTIFICATE COULD NOT BE VALIDATED"),
		BAD_TAC_SIGNATURE									(7,	"TAC'S SIGNATURE COULD NOT BE VALIDATED"),
		WRONG_VALIDATION_CODE								(8,	"WRONG VALIDATION CODE"),
		WRONG_TOKEN_CODE									(9,	"WRONG TOKEN CODE"),
		WRONG_TOKEN_TIME_WINDOW								(10,"WRONG TOKEN TIME WINDOW"),
		THERE_IS_NO_CERTIFICATE								(11,"THERE IS NO CERTIFICATE"),
		INVALID_UNIT_PPN									(12,"INVALID UNIT PPN"),
		SECURE_AUTH_SERVER_INTERNAL_ERROR					(13,"SECURE AUTH SERVER INTERNAL ERROR"),
		SECURE_AUTH_SERVER_IS_BUSY							(14,"SECURE AUTH SERVER IS BUSY"),
		NO_TOKEN_FOUND_FOR_GIVEN_TECHNITIAN_ID_TARGET_UNIT	(15,"NO TOKEN FOUND FOR GIVEN TECHNITIAN-ID - TARGET UNIT"),
		UNKNOWN_ERROR										(20,"UNKNOWN ERROR");

		private Integer		errorCodeNum;
		private String		errorDescription;

		private SasErrorCodeType(Integer errorCodeNum, String errorCodeName)
		{
			this.errorCodeNum		= errorCodeNum;
			this.errorDescription	= errorCodeName;
		}

		/**
		 * @return the description
		 */
		public String getDescription()
		{
			return errorDescription;
		}

		/**
		 * @return the error as number
		 */
		public Integer getErrorNum()
		{
			return errorCodeNum;
		}
		
		public static SasErrorCodeType fromInt(Integer errorCode)
		{
			for ( SasErrorCodeType value : SasErrorCodeType.values( ) ) 
			{
				if ( value.getErrorNum( ).equals( errorCode ) )
				{
					return value;
				}
			}
				
			return UNKNOWN_ERROR;
		}
	}
}
