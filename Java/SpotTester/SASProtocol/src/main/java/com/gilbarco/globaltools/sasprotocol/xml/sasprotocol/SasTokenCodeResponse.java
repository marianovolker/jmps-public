/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
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
public class SasTokenCodeResponse extends SasXmlResponseMsg
{
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private SasElement technician;
	private SasElement tokenCode;

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************	
	public SasTokenCodeResponse()
	{
		this.sasResponseType = SasResponseType.TOKEN_CODE_RESPONSE;
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

	/**
	 * @return the technician
	 */
	public SasElement getTechnician()
	{
		return technician;
	}

	/**
	 * @return the tokenCode
	 */
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
	public boolean decode(XMLStreamReader xmlReader) throws NumberFormatException, XMLStreamException
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
		else if (elementName.equals( SasElement.SasElementTags.TOKEN_CODE.getName( ) ))
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
