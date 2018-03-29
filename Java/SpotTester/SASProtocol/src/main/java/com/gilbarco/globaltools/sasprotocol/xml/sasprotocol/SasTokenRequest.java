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

public class SasTokenRequest extends SasXmlRequestMsg
{
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private SasElement technician;
	private SasElement tokenCode;	
	
	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************	
	public SasTokenRequest()
	{
		this.sasRequestType = SasRequestType.TOKEN_REQUEST;
	}

	public void setTechnician(String technicianID)
	{
		this.technician = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, technicianID );
		this.getContent().getBody().addNew( this.technician );		
	}

	public void setTokenCode(String tokenCode)
	{
		this.tokenCode = new SasElement( SasElement.SasElementTags.TOKEN_CODE, SasElement.SasElementType.STRING, tokenCode );
		this.getContent().getBody().addNew( this.tokenCode );		
	}
	
	public SasElement getTechnician() 
	{
		return technician;
	}

	public SasElement getTokenCode() 
	{
		return tokenCode;
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
		
		if( elementName.equals( SasElement.SasElementTags.TECHNICIANID.getName( ) ) )
		{
			setTechnician(xmlReader.getElementText( ));
		}
		else if( elementName.equals( SasElement.SasElementTags.TOKEN_CODE.getName( ) ) )
		{
			setTokenCode(xmlReader.getElementText( ));
		}
		else
		{
			return false;
		}

		return true;
	}
}
