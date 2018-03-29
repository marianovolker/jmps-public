/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.services;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Collection;

import javax.xml.stream.XMLStreamException;

import com.gilbarco.globaltools.sasprotocol.xml.messages.SasMessage;

/**
 * This class serves as template for marshalling, unmarshalling and validating processes of messages using Stax
 * 
 * @author ecampolo
 * 
 */
public abstract class SasStaxXmlService implements SasXmlService
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public final InputStream marshall(SasMessage message, File schema) throws XMLStreamException,
																		IOException
	{
		InputStream xml = new ByteArrayInputStream( writeXML( message ) );
		this.checkMarkAndValidate( schema, xml );
		return xml;
	}

	@Override
	public final SasMessage unmarshall(File schema, InputStream xml) throws XMLStreamException,
																	IOException
	{
		this.checkMarkAndValidate( schema, xml );
		return parse( schema, xml );
	}

	public abstract Collection<String> validate(File schema, InputStream xml) throws XMLStreamException,
																				IOException;

	public abstract SasMessage parse(File schema, InputStream xml) throws XMLStreamException,
																	IOException;

	public abstract byte[] writeXML(SasMessage msg) throws XMLStreamException, IOException;

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	private void checkMarkAndValidate(File schema, InputStream xml) throws IOException,
																	XMLStreamException
	{
		return;
		/*
		 * if (!xml.markSupported( )) { throw new IOException( "Mark functionality is not supported by the InputStream: " + xml.getClass( ).getName( ) ); }
		 * 
		 * xml.mark( 0 ); Collection<String> details = validate( schema, xml ); if (!details.isEmpty( )) { throw new XMLStreamException( details.toString( ) ); } else { xml.reset( ); }
		 */
	}
	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
