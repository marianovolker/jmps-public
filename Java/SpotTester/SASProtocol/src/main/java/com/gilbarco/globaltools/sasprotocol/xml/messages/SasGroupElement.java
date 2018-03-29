/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.messages;

import java.util.ArrayList;
import java.util.List;

import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamWriter;

/**
 * @author mvolker
 * 
 */
public class SasGroupElement extends SasElement
{
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected List<SasElement>	listOfElements;

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private int					iListIndex;


	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public SasGroupElement(SasElement.SasElementTags tagName)
	{
		super( tagName, SasElement.SasElementType.CONTAINER );
		this.tagName = tagName;
		this.listOfElements = new ArrayList<>( );
		this.iListIndex = 0;
	}

	public void addNew(SasElement element)
	{
		element.setPrintable( this.isPrintable( ) );
		this.listOfElements.add( element );
	}

	public SasElement getNextElement()
	{
		if (!this.listOfElements.isEmpty( ) && iListIndex < this.listOfElements.size( ))
		{
			SasElement sasElement = this.listOfElements.get( this.iListIndex );
			sasElement.setPrintable( this.isPrintable( ) );
			this.iListIndex++;
			return sasElement;
		}

		return null;
	}

	public SasElement getElement(int nOrder)
	{
		if (!this.listOfElements.isEmpty( ) && nOrder < this.listOfElements.size( ))
		{
			SasElement sasElement = this.listOfElements.get( nOrder );
			sasElement.setPrintable( this.isPrintable( ) );
			return sasElement;
		}

		return null;
	}

	public SasElement getElement(String key)
	{
		if (this.listOfElements != null && !this.listOfElements.isEmpty( ))
		{
			int index = 0;
			while( index < this.listOfElements.size( ) )
			{
				SasElement sasElement = this.listOfElements.get( index );
				if( sasElement instanceof SasGroupElement )
				{
					SasElement sasChildElement = ((SasGroupElement)sasElement).getElement( key );
					if( sasChildElement != null )
					{
						return sasChildElement;
					}
				}
				else
				{
					if( sasElement.getTagName( ).getName( ).equals( key ) )
					{
						return sasElement;
					}
				}
				index++;
			}
		}

		return null;
	}

	public int getQuantityOfSubnodes()
	{
		if (this.listOfElements != null && !this.listOfElements.isEmpty( ))
		{
			return this.listOfElements.size( );
		}

		return 0;
	}

	@Override
	public void toXML(XMLStreamWriter writer) throws XMLStreamException
	{
		SasElement sasElement = null;

		if (isPrintable( )) writer.writeCharacters( "\n" );

		writer.writeStartElement( getTagName( ).getName( ) );

		if (isPrintable( )) writer.writeCharacters( "\n\t" );

		while ((sasElement = getNextElement( )) != null)
		{
			sasElement.toXML( writer );

			if (isPrintable( ))
			{
				if (iListIndex < this.listOfElements.size( ))
				{
					writer.writeCharacters( "\n\t" );
				}
				else
				{
					writer.writeCharacters( "\n" );
				}
			}
		}
		writer.writeEndElement( );
		if (isPrintable( )) writer.writeCharacters( "\n" );
		this.iListIndex = 0;
	}
}
