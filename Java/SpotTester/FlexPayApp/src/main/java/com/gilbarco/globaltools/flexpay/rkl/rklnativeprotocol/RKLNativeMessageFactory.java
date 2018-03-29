
package com.gilbarco.globaltools.flexpay.rkl.rklnativeprotocol;

import java.util.Arrays;
import java.util.List;

import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.mgp.api.MessageFactory;
import com.gilbarco.globaltools.mgp.api.MessageType;
import com.gilbarco.globaltools.mgp.persistence.ProtocolProvider;
import com.gilbarco.globaltools.rklnativeprotocol.api.RklNativeMessageType;

;

public class RKLNativeMessageFactory extends MessageFactory
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final int	COMMAND_SOH		= 1;
	private static final int	COMMAND_CTRL	= 1;
	private static final int	COMMAND_START	= 1;
	private static final int	COMMAND_PREFIX	= 2;
	private static final int	COMMAND_INIT	= COMMAND_SOH + COMMAND_CTRL + COMMAND_START + COMMAND_PREFIX;
	private static final int	COMMAND_NAME	= 4;

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
	public RKLNativeMessageFactory(ProtocolProvider provider)
	{
		super( provider );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	@Override
	public byte[] getCommand(byte[] arg0)
	{
		byte[] data = Arrays.copyOfRange( arg0, COMMAND_INIT, COMMAND_INIT + COMMAND_NAME );
		// erase 0x8000 from the response command.
		// data[0] &= 0x0F;
		return data;
	}

	@Override
	public List<? extends MessageType> getMessageTypes()
	{
		return Arrays.asList( RklNativeMessageType.values( ) );
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

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
