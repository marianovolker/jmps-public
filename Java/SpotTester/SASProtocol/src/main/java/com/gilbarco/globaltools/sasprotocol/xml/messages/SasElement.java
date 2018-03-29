/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/


package com.gilbarco.globaltools.sasprotocol.xml.messages;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;

public class SasElement
{
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private   String value;
	
	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************	
	protected SasElementTags 	tagName		= null;
	protected SasElementType 	tagType		= null;
	protected Boolean 			printable	= null;
	
	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	/* If the SasElement is a leaf element */
	public SasElement(SasElementTags tagName, SasElementType tagType, String value)
	{
		this.printable  = Boolean.valueOf(false);
		this.tagName 	= tagName;
		this.tagType 	= tagType;
		this.value   	= value;
	}

	/* If the SasElement is a container */
	public SasElement(SasElementTags tagName, SasElementType tagType)
	{
		this.printable  = Boolean.valueOf(false);
		this.tagName = tagName;
		this.tagType = tagType;
	}	

	public SasElementTags getTagName()
	{
		return tagName;
	}

	public SasElementType getTagType()
	{
		return tagType;
	}

	public String getValue()
	{
		return value;
	}
	
	public void setPrintable(Boolean printable) 
	{
		this.printable = printable;
	}
	
	public boolean isPrintable()
	{
		return (printable.equals(true));
	}	

	public void toXML(XMLStreamWriter writer) throws XMLStreamException
	{
		writer.writeStartElement( getTagName( ).getName( ) );
		writer.writeCharacters( getValue( ) );
		writer.writeEndElement( );
	}
	
	public enum SasElementType
	{
		CONTAINER 				("container"),		
		INTEGER 				("integer"),
		STRING  				("string"),
		DATE					("date"),
		TIME					("time");
		
		private String name;
		
		private SasElementType (String name)
		{
			this.name = name;
		}
		
		public String getTypeName()
		{
			return this.name;
		}
	}
	
	public enum SasElementTags
	{
		VANGUARD_TOKEN_PROTOCOL		("VanguardTokenProtocol"),
		COMMAND						("Command"),		
		BODY						("Body"),		
		TECHNICIANID				("Technician-ID"),
		START_TIME					("StartTime"),
		DATE						("Date"),
		TIME						("Time"),
		TERMINAL					("Terminal"),
		TARGET_UNIT					("TargetUnit"),		
		TOKEN						("Token"),
		CODE						("Code"),
		DESCRIPTION					("Description"),
		CERTIFICATE					("Certificate"),
		TOKEN_CODE					("TokenCode"),
		SESSION						("Session"),
		EXPIRATION					("Expiration"),
		VERSION						("Version"),
		MAX_DURATION				("MaxDuration"),
		VALIDATION_CODE				("ValidationCode"),
		X509_CERTIFICATE			("X.509-Certificate"),
		SIGNATURE					("Signature"),
		XML_VERSION					("1.0"), 
		REMOTE_MAINTENACE			("RemoteMaintenance"), 
		PERMISSIONS					("Permissions"), 
		TECHNICIAN_DEVICE			("Technician-Device"), 
		OPERATORID					("Operator-ID"),
		TIME_WINDOW					("TimeWindow"),
		TOKEN_LIST					("TokenList"),
		END_TIME					("EndTime"),
		TIMES						("Times");
		

		private String	name;

		private SasElementTags(String name)
		{
			this.name = name;
		}

		public String getName()
		{
			return this.name;
		}
	}
}
