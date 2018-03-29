/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.messages;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;


/**
 * @author mvolker
 * 
 */
public class SasCommand extends SasElement
{
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************	
	private SasCommandTypes	type;	
	
	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************	
	public SasCommand( SasCommandTypes type )
	{
		super(SasElement.SasElementTags.COMMAND, SasElement.SasElementType.STRING, "");
		this.type = type;
	}
	
	@Override
	public void toXML(XMLStreamWriter writer) throws XMLStreamException
	{
		writer.writeStartElement( getTagName( ).getName( ) );
		writer.writeCharacters( type.getName( ) );
		writer.writeEndElement( );
	}	

	/**
	 * @return the type
	 */
	public SasCommandTypes getType()
	{
		return type;
	}
	
	public enum SasCommandTypes
	{
		REQUEST		("Request"),
		RESPONSE	("Response"),
		ERRORCODE	("ErrorCode"),
		UNKNOW		("Unknow");

		private String	name;

		private SasCommandTypes(String name)
		{
			this.name = name;
		}

		public String getName()
		{
			return this.name;
		}
		
		public static SasCommandTypes fromString(String code)
		{
			for ( SasCommandTypes value : SasCommandTypes.values( ) ) 
			{
				if ( value.getName( ).equals( code ) )
				{
					return value;
				}
			}
			
			return UNKNOW;
		}		
	}
}
