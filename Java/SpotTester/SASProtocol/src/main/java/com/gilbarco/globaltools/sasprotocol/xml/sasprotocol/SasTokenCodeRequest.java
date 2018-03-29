/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.sasprotocol.xml.sasprotocol;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;

import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasGroupElement;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlRequestMsg;

public class SasTokenCodeRequest extends SasXmlRequestMsg
{
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private SasElement		startTime;
	private SasElement		endTime;
	private SasElement		times;
	private SasElement		targetUnit;
	private SasElement		remoteMaintenance;
	private SasElement		permissions;
	private SasElement		technicianID;
	private SasElement		operatorID;
	private SasGroupElement	timeWindow;
	private SasGroupElement	session;
	private SasGroupElement	terminal;

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasTokenCodeRequest()
	{
		this.sasRequestType = SasRequestType.TOKEN_CODE_REQUEST;
	}

	public void createTimeWindowGroup()
	{
		this.timeWindow = new SasGroupElement( SasElement.SasElementTags.TIME_WINDOW );
		this.getContent( ).getBody( ).addNew( this.timeWindow );
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

	public void setRemoteMaintenance(String remotemaintenance)
	{
		remoteMaintenance = new SasElement( SasElement.SasElementTags.REMOTE_MAINTENACE, SasElement.SasElementType.STRING, remotemaintenance );
		this.getContent( ).getBody( ).addNew( this.remoteMaintenance );
	}

	public void setPermissions(String permissions)
	{
		this.permissions = new SasElement( SasElement.SasElementTags.PERMISSIONS, SasElement.SasElementType.STRING, permissions );
		this.getContent( ).getBody( ).addNew( this.permissions );
	}

	public void setTechnicianID(String technician)
	{
		this.technicianID = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, technician );
		this.getContent( ).getBody( ).addNew( this.technicianID );
	}

	public void setOperatorID(String operator)
	{
		this.operatorID = new SasElement( SasElement.SasElementTags.OPERATORID, SasElement.SasElementType.STRING, operator );
		this.getContent( ).getBody( ).addNew( this.operatorID );
	}

	public SasElement getTimes()
	{
		return times;
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

		if (elementName.equals( SasElement.SasElementTags.TIME_WINDOW.getName( ) ))
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
			setRemoteMaintenance( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.PERMISSIONS.getName( ) ))
		{
			setPermissions( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.TECHNICIANID.getName( ) ))
		{
			setTechnicianID( xmlReader.getElementText( ) );
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
}