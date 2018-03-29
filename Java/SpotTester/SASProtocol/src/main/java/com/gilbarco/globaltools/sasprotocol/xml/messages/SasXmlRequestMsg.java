/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
* Additional comments for the file . . .
*/

package com.gilbarco.globaltools.sasprotocol.xml.messages;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;


public abstract class SasXmlRequestMsg extends SasMessage
{
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected SasRequestType sasRequestType;

	
	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasXmlRequestMsg ()
	{
		super( new SasCommand(SasCommand.SasCommandTypes.REQUEST) );
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
			
			if(!isPrintable()) writeSignedDataTag(this.content.getBody( ), writer);
			
		writer.writeEndElement( );
		
		if(isPrintable()) writer.writeCharacters( "\n" );
	}
	
	/**
	 * This method is used to verify if the signature of the message received is valid.
	 * This verification is not needed for these type of messages.
	 * @return none.
	 * @throws XMLStreamException
	 */
	@Override
	public void verifySignedData() throws XMLStreamException {};


	/**
	 * @return the sasRequestType
	 */
	public SasRequestType getSasRequestType()
	{
		return sasRequestType;
	}

	public enum SasRequestType
	{
		TOKEN_CODE_REQUEST				("TokenCodeRequest"),
		TOKEN_REQUEST					("TokenRequest"),		
		PUBLISH_CERTIFICATE_REQUEST		("PublishCertificateRequest"),
		CERTIFICATE_INFORMATION_REQUEST	("CertificateInformationRequest"),
		CERTIFICATE_APPROVAL_REQUEST	("CertificateApprovalRequest"),
		REMOVE_CERTIFICATE_REQUEST		("RemoveCertificateRequest"),
		TOKEN_QUERY_REQUEST				("TokenQueryRequest"),
		UNKNOWN_REQUEST_TYPE			("Unknown request type");


		private String	name;
		
		SasRequestType( String name )
		{
			this.name = name ;
		}

		/**
		 * @return the name
		 */
		public String getName()
		{
			return name;
		}
		
		public static SasRequestType fromString(String code)
		{
			for ( SasRequestType value : SasRequestType.values( ) ) 
			{
				if ( value.getName( ).equals( code ) )
				{
					return value;
				}
			}
			
			return UNKNOWN_REQUEST_TYPE;
		}		
	}
}
