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
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasGroupElement;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlRequestMsg;

public class SasTokenQueryRequest extends SasXmlRequestMsg
{
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private SasElement 		technician 	= null;
	private SasGroupElement terminal	= null;	

	
	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************	
	public SasTokenQueryRequest( )
	{
		this.sasRequestType = SasRequestType.TOKEN_QUERY_REQUEST; 
	}
	
	
	// Body Group Entities ...
	// ---------------------------------------------------------------------------------------------	
	public void setTechnician( String technicianID )
	{
		this.technician = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, technicianID );
		
		this.getContent().getBody().addNew( this.technician );		
	}
	
	public void createTerminalGroup( )
	{
		this.terminal = new SasGroupElement( SasElement.SasElementTags.TERMINAL );
		
		this.getContent().getBody().addNew( this.terminal );		
	}
	// ---------------------------------------------------------------------------------------------

	// Terminal Group Entities ...
	// ---------------------------------------------------------------------------------------------
	public void setTargetUnit( String targetUnit )
	{
		SasElement ppnElement = new SasElement( SasElement.SasElementTags.TARGET_UNIT, SasElement.SasElementType.STRING, targetUnit );
		
		if( this.terminal == null )
		{
			createTerminalGroup();
		}
		
		this.terminal.addNew( ppnElement );
	}
	// ---------------------------------------------------------------------------------------------

	// Getters ...
	// ---------------------------------------------------------------------------------------------
	public String getTechnician( )
	{
		if( this.technician == null )
		{
			return null;
		}

		return technician.getValue( );
	}
	
	public String getTerminal( )
	{
		if( this.terminal != null )
		{
			return this.terminal.getElement(0).getValue();
		}
		
		return null;
	}
	// ---------------------------------------------------------------------------------------------	
	
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
		else if (elementName.equals( SasElement.SasElementTags.TERMINAL.getName( ) ))
		{
			createTerminalGroup();
		}
		else if (elementName.equals( SasElement.SasElementTags.TARGET_UNIT.getName( ) ))
		{
			setTargetUnit(xmlReader.getElementText( ));
		}		
		else
		{
			return false;
		}

		return true;
	}
}
