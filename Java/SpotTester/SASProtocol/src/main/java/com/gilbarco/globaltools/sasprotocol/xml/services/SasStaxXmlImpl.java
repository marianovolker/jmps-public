/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.services;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.InputStream;
import java.util.Collection;

import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLOutputFactory;
import javax.xml.stream.XMLStreamConstants;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;
import javax.xml.stream.XMLStreamWriter;

import com.gilbarco.globaltools.sasprotocol.xml.messages.SasCommand;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasMessage;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlErrorCodeMsg;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlRequestMsg;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlResponseMsg;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasCertificateApprovalRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasCertificateInformationRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasCertificateInformationResponse;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasPublishCertificateRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasPublishCertificateResponse;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasRemoveCertificateRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenCodeRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenCodeResponse;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenQueryRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenQueryResponse;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenResponse;

public class SasStaxXmlImpl extends SasStaxXmlService
{
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private XMLStreamReader		xmlReader		= null;
	private XMLInputFactory		inputFactory	= null;
	private XMLOutputFactory	outputFactory	= null;
	private SasXmlValidable		validator		= null;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public SasMessage parse(File schema, InputStream xml) throws XMLStreamException
	{
		SasMessage message = null;
		this.inputFactory = XMLInputFactory.newFactory( );
		this.xmlReader = inputFactory.createXMLStreamReader( xml );
		int eventType;

		while (this.xmlReader.hasNext( ))
		{
			String elementName = null;
			eventType = this.xmlReader.getEventType( );

			if (eventType == XMLStreamConstants.START_ELEMENT)
			{
				elementName = this.xmlReader.getLocalName( );

				if (elementName.equals( SasElement.SasElementTags.COMMAND.getName( ) ))
				{
					String strAsTagContent = this.xmlReader.getElementText( );
					
					// Message responses ...
					// ---------------------------------------------------------------------------------------------
					if (SasCommand.SasCommandTypes.fromString( strAsTagContent ) == SasCommand.SasCommandTypes.ERRORCODE)
					{
						message = new SasXmlErrorCodeMsg( );
					}
					else if (SasXmlResponseMsg.SasResponseType.fromString( strAsTagContent ) == SasXmlResponseMsg.SasResponseType.TOKEN_RESPONSE)
					{
						message = new SasTokenResponse( );
					}
					else if (SasXmlResponseMsg.SasResponseType.fromString( strAsTagContent ) == SasXmlResponseMsg.SasResponseType.TOKEN_CODE_RESPONSE)
					{
						message = new SasTokenCodeResponse( );
					}
					else if (SasXmlResponseMsg.SasResponseType.fromString( strAsTagContent ) == SasXmlResponseMsg.SasResponseType.PUBLISH_CERTIFICATE_RESPONSE)
					{
						message = new SasPublishCertificateResponse( );
					}
					else if (SasXmlResponseMsg.SasResponseType.fromString( strAsTagContent ) == SasXmlResponseMsg.SasResponseType.CERTIFICATE_INFORMATION_RESPONSE )
					{
						message = new SasCertificateInformationResponse( );
					}
					else if (SasXmlResponseMsg.SasResponseType.fromString( strAsTagContent ) == SasXmlResponseMsg.SasResponseType.TOKEN_QUERY_RESPONSE )
					{
						message = new SasTokenQueryResponse( );
					}
					// ---------------------------------------------------------------------------------------------
					
					// Message requests ...					
					// ---------------------------------------------------------------------------------------------
					else if (SasXmlRequestMsg.SasRequestType.fromString( strAsTagContent ) == SasXmlRequestMsg.SasRequestType.TOKEN_CODE_REQUEST)
					{
						message = new SasTokenCodeRequest( );
					}
					else if (SasXmlRequestMsg.SasRequestType.fromString( strAsTagContent ) == SasXmlRequestMsg.SasRequestType.TOKEN_REQUEST)
					{
						message = new SasTokenRequest( );
					}
					else if (SasXmlRequestMsg.SasRequestType.fromString( strAsTagContent ) == SasXmlRequestMsg.SasRequestType.PUBLISH_CERTIFICATE_REQUEST)
					{
						message = new SasPublishCertificateRequest( );
					}
					else if (SasXmlRequestMsg.SasRequestType.fromString( strAsTagContent ) == SasXmlRequestMsg.SasRequestType.CERTIFICATE_INFORMATION_REQUEST )
					{
						message = new SasCertificateInformationRequest( );
					}				
					else if (SasXmlRequestMsg.SasRequestType.fromString( strAsTagContent ) == SasXmlRequestMsg.SasRequestType.CERTIFICATE_APPROVAL_REQUEST )
					{
						message = new SasCertificateApprovalRequest( );
					}
					else if (SasXmlRequestMsg.SasRequestType.fromString( strAsTagContent ) == SasXmlRequestMsg.SasRequestType.REMOVE_CERTIFICATE_REQUEST )
					{
						message = new SasRemoveCertificateRequest( );
					}
					else if (SasXmlRequestMsg.SasRequestType.fromString( strAsTagContent ) == SasXmlRequestMsg.SasRequestType.TOKEN_QUERY_REQUEST )
					{
						message = new SasTokenQueryRequest( );
					}
					// ---------------------------------------------------------------------------------------------

					// ---------------------------------------------------------------------------------------------
					else
					{
						throw new XMLStreamException( "Invalid Message." );
					}
					// ---------------------------------------------------------------------------------------------
				}

				else if ( message != null )
				{
					message.decode(xmlReader);
				}
			}
			else if (eventType == XMLStreamConstants.END_ELEMENT)
			{
				elementName = this.xmlReader.getLocalName( );

				if (elementName.equals( SasElement.SasElementTags.VANGUARD_TOKEN_PROTOCOL.getName( ) ))
				{
					message.verifySignedData();
					
					return message;
				}
			}

			eventType = this.xmlReader.next( );
		}

		return message;
	}

	@Override
	public Collection<String> validate(File schema, InputStream xml) throws XMLStreamException
	{
		try
		{
			SasXmlValidator xmlValidator = new SasXmlValidator( schema );
			xmlValidator.validate( xml );
			return xmlValidator.getDetails( );
		}
		catch (Exception e)
		{
			throw new XMLStreamException( e.getMessage( ) );
		}

	}

	@Override
	public byte[] writeXML(SasMessage message) throws XMLStreamException
	{
		this.outputFactory = XMLOutputFactory.newFactory( );
		ByteArrayOutputStream out = new ByteArrayOutputStream( );
		XMLStreamWriter writer = this.outputFactory.createXMLStreamWriter( out );

		writer.writeStartDocument( SasElement.SasElementTags.XML_VERSION.getName() );
		
		if( message != null )
		{
			message.encode(writer);
		}

		writer.flush( );
		writer.close( );
		return out.toByteArray( );

	}

	public SasXmlValidable getValidator()
	{
		return validator;
	}

	public void setValidator(SasXmlValidable validator)
	{
		this.validator = validator;
	}
}
