/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.sasprotocol.xml.sasprotocol;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;
import javax.xml.stream.XMLStreamWriter;

import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement.SasElementTags;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasGroupElement;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlResponseMsg;

/**
 * @author mvolker
 * @version 1.0
 * @created 04-Jun-2012 05:55:26 p.m.
 */
public class SasTokenResponse extends SasXmlResponseMsg
{
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private SasElement		version;
	private SasElement		tokenCode;
	private SasElement		startTime;
	private SasElement		endTime;
	private SasElement		maxDuration;
	private SasElement		times;
	private SasElement		targetUnit;
	private SasElement		remoteMaintenance;
	private SasElement		permissions;
	private SasElement		technicianID;
	private SasElement		technicianDevice;
	private SasElement		operatorID;
	private SasGroupElement	timeWindow;
	private SasGroupElement	session;
	private SasGroupElement	terminal;

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasTokenResponse()
	{
		this.sasResponseType = SasResponseType.TOKEN_RESPONSE;
	}

	public void setVersion(String version)
	{
		this.version = new SasElement( SasElement.SasElementTags.VERSION, SasElement.SasElementType.STRING, version );
		this.getContent( ).getBody( ).addNew( this.version );
	}

	public void setTokenCode(String tokenCode)
	{
		this.tokenCode = new SasElement( SasElement.SasElementTags.TOKEN_CODE, SasElement.SasElementType.STRING, tokenCode );
		this.getContent( ).getBody( ).addNew( this.tokenCode );
	}

	public void setStartTime(String startTime)
	{
		this.startTime = new SasElement( SasElement.SasElementTags.START_TIME, SasElement.SasElementType.STRING, startTime );
		this.timeWindow.addNew( this.startTime );
	}

	public void setEndTime(String endTime)
	{
		this.endTime = new SasElement( SasElement.SasElementTags.END_TIME, SasElement.SasElementType.STRING, endTime );
		this.timeWindow.addNew( this.endTime );
	}

	public void createSessionGroup()
	{
		this.session = new SasGroupElement( SasElement.SasElementTags.SESSION );
		this.getContent( ).getBody( ).addNew( this.session );
	}

	public void setTimes(String times)
	{
		this.times = new SasElement( SasElement.SasElementTags.TIMES, SasElement.SasElementType.STRING, times );
		this.session.addNew( this.times );
	}

	public void setMaxDuration(String maxDuration)
	{
		this.maxDuration = new SasElement( SasElement.SasElementTags.MAX_DURATION, SasElement.SasElementType.INTEGER, maxDuration );
		this.session.addNew( this.maxDuration );
	}

	public void createTerminalGroup()
	{
		this.terminal = new SasGroupElement( SasElement.SasElementTags.TERMINAL );
		this.getContent( ).getBody( ).addNew( this.terminal );
	}

	public void setTargetUnit(String targetunit)
	{
		targetUnit = new SasElement( SasElement.SasElementTags.TARGET_UNIT, SasElement.SasElementType.STRING, targetunit );
		this.terminal.addNew( targetUnit );
	}

	public void createTimeWindowGroup()
	{
		this.timeWindow = new SasGroupElement( SasElement.SasElementTags.TIME_WINDOW );
		this.getContent( ).getBody( ).addNew( this.timeWindow );
	}

	public void setRemoteMaintenace(String remotemaintenance)
	{
		remoteMaintenance = new SasElement( SasElement.SasElementTags.REMOTE_MAINTENACE, SasElement.SasElementType.STRING, remotemaintenance );
		this.getContent( ).getBody( ).addNew( this.remoteMaintenance );
	}

	private void setPermissions(String permissions)
	{
		this.permissions = new SasElement( SasElement.SasElementTags.PERMISSIONS, SasElement.SasElementType.STRING, permissions );
		this.getContent( ).getBody( ).addNew( this.permissions );
	}

	private void setTechnicianID(String technician)
	{
		this.technicianID = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, technician );
		this.getContent( ).getBody( ).addNew( this.technicianID );
	}

	private void setTechnicianDevice(String device)
	{
		this.technicianDevice = new SasElement( SasElement.SasElementTags.TECHNICIAN_DEVICE, SasElement.SasElementType.STRING, device );
		this.getContent( ).getBody( ).addNew( this.technicianDevice );
	}

	private void setOperatorID(String operator)
	{
		this.operatorID = new SasElement( SasElement.SasElementTags.OPERATORID, SasElement.SasElementType.STRING, operator );
		this.getContent( ).getBody( ).addNew( this.operatorID );
	}

	public SasElement getVersion()
	{
		return version;
	}

	public SasElement getTokenCode()
	{
		return tokenCode;
	}

	public SasElement getTimes()
	{
		return times;
	}

	public SasElement getMaxDuration()
	{
		return maxDuration;
	}

	public String getTerminal(int nOrder)
	{
		if (this.terminal != null)
		{
			return this.terminal.getElement( nOrder ).getValue( );
		}

		return null;
	}

	public String getTargetUnit()
	{
		return getTerminal( 0 );
	}

	public SasElement getRemoteMaintenance()
	{
		return this.remoteMaintenance;
	}

	public SasElement getPermissions()
	{
		return this.permissions;
	}

	public SasElement getTechnicianID()
	{
		return this.technicianID;
	}

	public SasElement getTechnicianDevice()
	{
		return this.technicianDevice;
	}

	public SasElement getOperatorID()
	{
		return this.operatorID;
	}

	public SasElement getStartTime()
	{
		return startTime;
	}

	public SasElement getEndTime()
	{
		return endTime;
	}

	public SasGroupElement getTimeWindow()
	{
		return timeWindow;
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

		if (elementName.equals( SasElement.SasElementTags.TOKEN_CODE.getName( ) ))
		{
			setTokenCode( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.VERSION.getName( ) ))
		{
			setVersion( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.TIME_WINDOW.getName( ) ))
		{
			createTimeWindowGroup( );
		}
		else if (elementName.equals( SasElement.SasElementTags.START_TIME.getName( ) ))
		{
			if (this.timeWindow != null && this.startTime == null)
			{
				setStartTime( xmlReader.getElementText( ) );
			}
		}
		else if (elementName.equals( SasElement.SasElementTags.END_TIME.getName( ) ))
		{
			if (this.timeWindow != null && this.endTime == null)
			{
				setEndTime( xmlReader.getElementText( ) );
			}
		}
		else if (elementName.equals( SasElement.SasElementTags.SESSION.getName( ) ))
		{
			createSessionGroup( );
		}
		else if (elementName.equals( SasElement.SasElementTags.TIMES.getName( ) ))
		{
			setTimes( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.MAX_DURATION.getName( ) ))
		{
			setMaxDuration( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.TERMINAL.getName( ) ))
		{
			createTerminalGroup( );
		}
		else if (elementName.equals( SasElement.SasElementTags.TARGET_UNIT.getName( ) ))
		{
			setTargetUnit( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.REMOTE_MAINTENACE.getName( ) ))
		{
			setRemoteMaintenace( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.PERMISSIONS.getName( ) ))
		{
			setPermissions( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.TECHNICIANID.getName( ) ))
		{
			setTechnicianID( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.TECHNICIAN_DEVICE.getName( ) ))
		{
			setTechnicianDevice( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.OPERATORID.getName( ) ))
		{
			setOperatorID( xmlReader.getElementText( ) );
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
		writer.writeStartElement( SasElementTags.TOKEN.getName( ) );
			this.content.getBody( ).toXML( writer );
			writer.writeStartElement( SasElement.SasElementTags.X509_CERTIFICATE.getName( ) );
				writer.writeStartElement( SasElement.SasElementTags.SIGNATURE.getName( ) );
					writer.writeCharacters( signature );
				writer.writeEndElement( );
			writer.writeEndElement( );
		writer.writeEndElement( ); 
		writer.writeCharacters( "\n" );
	}
}
