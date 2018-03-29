/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.messages;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;


public abstract class SasXmlResponseMsg extends SasMessage
{
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected SasResponseType sasResponseType;


	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************	
	public SasXmlResponseMsg ()
	{
		super( new SasCommand(SasCommand.SasCommandTypes.RESPONSE) );
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
			writer.writeCharacters( sasResponseType.getName( ) );
			writer.writeEndElement( );
		
			this.content.getBody( ).toXML( writer );
			
		writer.writeEndElement( ); 
		
		if(isPrintable()) writer.writeCharacters( "\n" );
	}

	/**
	 * @return the sasResponseType
	 */
	public SasResponseType getSasResponseType()
	{
		return sasResponseType;
	}

	public enum SasResponseType
	{
		TOKEN_CODE_RESPONSE					("TokenCodeResponse"),
		TOKEN_RESPONSE						("TokenResponse"),		
		PUBLISH_CERTIFICATE_RESPONSE		("PublishCertificateResponse"),
		CERTIFICATE_INFORMATION_RESPONSE	("CertificateInformationResponse"),
		TOKEN_QUERY_RESPONSE				("TokenQueryResponse"),
		UNKNOW_RESPONSE_TYPE				("Unknow request type");

		private String	name;
		
		SasResponseType( String name )
		{
			this.name = name ;
		}

		public String getName()
		{
			return name;
		}
		
		public static SasResponseType fromString(String code)
		{
			for ( SasResponseType value : SasResponseType.values( ) ) 
			{
				if ( value.getName( ).equals( code ) )
				{
					return value;
				}
			}
			
			return UNKNOW_RESPONSE_TYPE;
		}		
	}
}
