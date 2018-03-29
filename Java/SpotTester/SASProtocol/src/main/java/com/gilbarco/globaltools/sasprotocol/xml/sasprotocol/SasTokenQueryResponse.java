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
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlResponseMsg;

/**
 * @author mvolker
 * @version 1.0
 * @created 14-Nov-2012 05:24:34 p.m.
 */
public class SasTokenQueryResponse extends SasXmlResponseMsg
{
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private SasElement		technicianID;
	private SasGroupElement	terminal;
	private SasGroupElement	tokenList;
	
	private SasGroupElement	token;
	private SasElement		tokenCode;
	
	private SasGroupElement	timeWindow;	
	private SasElement		startTime;	
	private SasElement		endTime;
	
	private SasGroupElement	session;
	private SasElement		times;
	private SasElement		maxDuration;
	
	private SasElement		remoteMaintenance;	
	private SasElement		permissions;
	
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private String getFieldValueOfToken( int nToken, String key )
	{
		if( this.tokenList != null && ( nToken >= 0 && nToken < this.tokenList.getQuantityOfSubnodes( ) ) )
		{
			SasGroupElement tokenRequested = (SasGroupElement) this.tokenList.getElement( nToken );
			SasElement value  = tokenRequested.getElement( key );
			
			if( value != null )
			{
				return value.getValue( );
			}
		}
		
		return "";
	}	
	
	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasTokenQueryResponse()
	{
		this.sasResponseType = SasResponseType.TOKEN_QUERY_RESPONSE;
	}


	// Body Group Entities ...
	// ---------------------------------------------------------------------------------------------
	private void setTechnicianID(String technician)
	{
		this.technicianID = new SasElement( SasElement.SasElementTags.TECHNICIANID, SasElement.SasElementType.STRING, technician );
		this.getContent( ).getBody( ).addNew( this.technicianID );
	}
	
	public void createTerminalGroup()
	{
		this.terminal = new SasGroupElement( SasElement.SasElementTags.TERMINAL );
		this.getContent( ).getBody( ).addNew( this.terminal );
	}
	
	public void createTokenListGroup()
	{
		this.tokenList = new SasGroupElement( SasElement.SasElementTags.TOKEN_LIST );
		this.getContent( ).getBody( ).addNew( this.tokenList );
	}
	// ---------------------------------------------------------------------------------------------	
	
	// Terminal Group Entities ...
	// ---------------------------------------------------------------------------------------------
	public void setTargetUnit(String targetUnit)
	{
		SasElement targetUnitElement = new SasElement( SasElement.SasElementTags.TARGET_UNIT, SasElement.SasElementType.STRING, targetUnit );
		
		if( terminal == null )
		{
			createTerminalGroup();
		}
		
		this.terminal.addNew( targetUnitElement );
	}
	// ---------------------------------------------------------------------------------------------
	
	// Token Group Entities ...
	// ---------------------------------------------------------------------------------------------
	public void createTokenGroup()
	{
		this.token = new SasGroupElement( SasElement.SasElementTags.TOKEN );
		
		if( tokenList == null )
		{
			createTokenListGroup();
		}
		
		this.tokenList.addNew( this.token );
	}

	public void setTokenCode(String tokenCode)
	{
		this.tokenCode = new SasElement( SasElement.SasElementTags.TOKEN_CODE, SasElement.SasElementType.STRING, tokenCode );
		
		if( token == null )
		{
			createTokenGroup();
		}
		
		this.token.addNew( this.tokenCode );		
	}
	// ---------------------------------------------------------------------------------------------
	
	
	// TimeWindow Group Entities ...
	// ---------------------------------------------------------------------------------------------
	public void createTimeWindowGroup()
	{
		this.timeWindow = new SasGroupElement( SasElement.SasElementTags.TIME_WINDOW );
		
		if( token == null )
		{
			createTokenGroup();
		}
		
		this.token.addNew( this.timeWindow );		
	}
	
	public void setStartTime(String startTime)
	{
		this.startTime = new SasElement( SasElement.SasElementTags.START_TIME, SasElement.SasElementType.STRING, startTime );
		
		if( timeWindow == null )
		{
			createTimeWindowGroup();
		}		
		
		this.timeWindow.addNew( this.startTime );
	}	
	
	public void setEndTime(String endTime)
	{
		this.endTime = new SasElement( SasElement.SasElementTags.END_TIME, SasElement.SasElementType.STRING, endTime );
		
		if( timeWindow == null )
		{
			createTimeWindowGroup();
		}		
		
		this.timeWindow.addNew( this.endTime );
	}	
	// ---------------------------------------------------------------------------------------------	
	
	
	// Session Group Entities ...
	// ---------------------------------------------------------------------------------------------	
	public void createSessionGroup()
	{
		this.session = new SasGroupElement( SasElement.SasElementTags.SESSION );
		
		if( token == null )
		{
			createTokenGroup();
		}
		
		this.token.addNew( this.session );
	}
	
	public void setMaxDuration(String maxDuration)
	{
		this.maxDuration = new SasElement( SasElement.SasElementTags.MAX_DURATION, SasElement.SasElementType.INTEGER, maxDuration );
		
		if( session == null )
		{
			createSessionGroup();
		}
		
		this.session.addNew( this.maxDuration );
	}
	
	public void setTimes(String times)
	{
		this.times = new SasElement( SasElement.SasElementTags.TIMES, SasElement.SasElementType.INTEGER, times );
		
		if( session == null )
		{
			createSessionGroup();
		}
		
		this.session.addNew( this.times );
	}	
	// ---------------------------------------------------------------------------------------------

	// Token Group Entities ...
	// ---------------------------------------------------------------------------------------------
	public void setRemoteMaintenace(String remotemaintenance)
	{
		remoteMaintenance = new SasElement( SasElement.SasElementTags.REMOTE_MAINTENACE, SasElement.SasElementType.STRING, remotemaintenance );
		
		if( token == null )
		{
			createTokenGroup();
		}
		
		this.token.addNew( this.remoteMaintenance );		
	}

	private void setPermissions(String permissions)
	{
		this.permissions = new SasElement( SasElement.SasElementTags.PERMISSIONS, SasElement.SasElementType.STRING, permissions );
		
		if( token == null )
		{
			createTokenGroup();
		}
		
		this.token.addNew( this.permissions );		
	}
	// ---------------------------------------------------------------------------------------------

	// Getters ...
	// ---------------------------------------------------------------------------------------------
	public String getTechnicianID()
	{
		if( this.technicianID == null )
		{
			return null;
		}

		return technicianID.getValue( );
	}

	public String getTargetUnit()
	{
		SasElement value  = this.terminal.getElement( 0 );
		
		if( value == null )
		{
			return null;
		}

		return value.getValue( );
	}
	
	public int getQuantityOfTokens()
	{
		if( this.tokenList == null )
		{
			return 0;
		}
		
		return this.tokenList.getQuantityOfSubnodes( );
	}
	

	/**
	 * @param nToken this value should be between 0 and n-1.
	 * @return the tokenCode of the token selected
	 */
	public String getTokenCode(int nToken)
	{
		return getFieldValueOfToken( nToken, SasElement.SasElementTags.TOKEN_CODE.getName( ) );
	}

	/**
	 * @param nToken this value should be between 0 and n-1.
	 * @return the startTime of the token selected
	 */
	public String getStartTime(int nToken)
	{
		return getFieldValueOfToken( nToken, SasElement.SasElementTags.START_TIME.getName( ) );
	}

	/**
	 * @param nToken this value should be between 0 and n-1.
	 * @return the endTime of the token selected
	 */
	public String getEndTime(int nToken)
	{
		return getFieldValueOfToken( nToken, SasElement.SasElementTags.END_TIME.getName( ) );
	}

	/**
	 * @param nToken this value should be between 0 and n-1.
	 * @return the times of the token selected
	 */
	public String getTimes(int nToken)
	{
		return getFieldValueOfToken( nToken, SasElement.SasElementTags.TIMES.getName( ) );
	}

	/**
	 * @param nToken this value should be between 0 and n-1.
	 * @return the maxDuration of the token selected
	 */
	public String getMaxDuration(int nToken)
	{
		return getFieldValueOfToken( nToken, SasElement.SasElementTags.MAX_DURATION.getName( ) );
	}

	/**
	 * @param nToken this value should be between 0 and n-1.
	 * @return the remoteMaintenance of the token selected
	 */
	public String getRemoteMaintenance(int nToken)
	{
		return getFieldValueOfToken( nToken, SasElement.SasElementTags.REMOTE_MAINTENACE.getName( ) );
	}

	/**
	 * @param nToken this value should be between 0 and n-1.
	 * @return the permissions of the token selected
	 */
	public String getPermissions(int nToken)
	{
		return getFieldValueOfToken( nToken, SasElement.SasElementTags.PERMISSIONS.getName( ) );
	}	
	// ---------------------------------------------------------------------------------------------


	/**
	 * @return decode each part of the message
	 * @throws XMLStreamException
	 * @throws NumberFormatException
	 */
	@Override
	public boolean decode(XMLStreamReader xmlReader) throws NumberFormatException, XMLStreamException
	{
		// To parse the signature of the message and another common things.
		if (super.decode( xmlReader ))
		{
			return true;
		}

		String elementName = xmlReader.getLocalName( );

		if (elementName.equals( SasElement.SasElementTags.TECHNICIANID.getName( ) ))
		{
			setTechnicianID( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.TERMINAL.getName( ) ))
		{
			createTerminalGroup( );
		}
		else if (elementName.equals( SasElement.SasElementTags.TARGET_UNIT.getName( ) ))
		{
			setTargetUnit( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.TOKEN_LIST.getName( ) ))
		{
			createTokenListGroup( );
		}
		else if (elementName.equals( SasElement.SasElementTags.TOKEN.getName( ) ))
		{
			createTokenGroup( );
		}		
		else if (elementName.equals( SasElement.SasElementTags.TOKEN_CODE.getName( ) ))
		{
			setTokenCode( xmlReader.getElementText( ) );
		}		
		else if (elementName.equals( SasElement.SasElementTags.TIME_WINDOW.getName( ) ))
		{
			createTimeWindowGroup();
		}		
		else if (elementName.equals( SasElement.SasElementTags.START_TIME.getName( ) ))
		{
			setStartTime( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.END_TIME.getName( ) ))
		{
			setEndTime( xmlReader.getElementText( ) );
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
		else if (elementName.equals( SasElement.SasElementTags.REMOTE_MAINTENACE.getName( ) ))
		{
			setRemoteMaintenace( xmlReader.getElementText( ) );
		}
		else if (elementName.equals( SasElement.SasElementTags.PERMISSIONS.getName( ) ))
		{
			setPermissions( xmlReader.getElementText( ) );
		}
		else
		{
			return false;
		}

		return true;
	}
}
