/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.sasprotocol.xml.messages;


public class SasVanguardTokenElement extends SasGroupElement
{
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************	
	protected SasCommand		command;
	protected SasGroupElement	body;

	
	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************	
	public SasVanguardTokenElement(SasCommand command, SasGroupElement body)
	{
		super( SasElement.SasElementTags.VANGUARD_TOKEN_PROTOCOL );
		
		this.command = command;
		this.body = body;
		
		this.addNew( this.command );
		this.addNew( this.body );
	}
	
	/**
	 * @return the command
	 */
	public SasCommand getCommand()
	{
		return command;
	}

	/**
	 * @return the body
	 */
	public SasGroupElement getBody()
	{
		return body;
	}
	
	@Override
	public void setPrintable(Boolean printable)
	{
		this.printable = printable;
		command.setPrintable(printable);
		body.setPrintable(printable);
	}
}
