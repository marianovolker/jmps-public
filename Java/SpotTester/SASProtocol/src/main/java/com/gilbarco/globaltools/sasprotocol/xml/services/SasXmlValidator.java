/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.services;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collection;

import javax.xml.XMLConstants;
import javax.xml.transform.sax.SAXSource;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;

import org.xml.sax.ErrorHandler;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;

public class SasXmlValidator implements SasXmlValidable
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
	private EdtXmlErrorHandler	handler		= null;
	private Validator			validator	= null;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SasXmlValidator(File schema) throws SAXException
	{
		this.handler = new EdtXmlErrorHandler( );
		this.validator = SchemaFactory.newInstance( XMLConstants.W3C_XML_SCHEMA_NS_URI )
						.newSchema( schema ).newValidator( );
		this.validator.setErrorHandler( this.handler );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public boolean validate(InputStream input) throws SAXException, IOException
	{
		SAXSource source = new SAXSource( new InputSource( input ) );
		this.validator.validate( source );
		return this.handler.isValid( );
	}

	public Collection<String> getDetails()
	{
		return this.handler.getErrors( );
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	class EdtXmlErrorHandler implements ErrorHandler
	{
		private Collection<String>	errors	= new ArrayList<String>( );

		@Override
		public void warning(SAXParseException exception) throws SAXException
		{
			System.out.println( exception.toString( ) );
		}

		@Override
		public void error(SAXParseException exception) throws SAXException
		{
			System.out.println( exception.toString( ) );
			this.errors.add( exception.toString( ) );
		}

		@Override
		public void fatalError(SAXParseException exception) throws SAXException
		{
			System.out.println( exception.toString( ) );
			this.errors.add( exception.toString( ) );
		}

		public boolean isValid()
		{
			return this.errors.isEmpty( );
		}

		public Collection<String> getErrors()
		{
			return this.errors;
		}
	}
	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
