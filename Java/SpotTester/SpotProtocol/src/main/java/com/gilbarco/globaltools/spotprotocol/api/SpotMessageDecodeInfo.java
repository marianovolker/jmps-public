/**
 * 
 */

package com.gilbarco.globaltools.spotprotocol.api;

import com.gilbarco.globaltools.mgp.domain.ContainerType;

/**
 * @author jrubstein Sep 15, 2010
 * 
 */
public class SpotMessageDecodeInfo
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final int	APP_ID_OFFSET		= 2;
	private static final int	COMMAND_OFFSET		= 4;
	private static final int	SUB_COMMAND_OFFSET	= 5;
	private static final byte	RESPONSE_MASCK		= (byte) 0x80;

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
	/**
	 * @return the appId
	 */
	public static byte getAppId(byte[] data)
	{
		return (byte) (data[APP_ID_OFFSET] & 0xFF);
	}

	/**
	 * @return the command
	 */
	public static byte[] getCommand(byte[] data)
	{
		byte appId = getAppId( data );

		// The purpose of this constant (0x7F) is for making 0x81 equal to 0x01 for the appid.
		if ((appId & 0x7F) == 0)
		{
			return (new byte[] { data[COMMAND_OFFSET] });
		}
		else
		{
			// The purpose of this constant (0x7F) is for making 0x81 equal to 0x01 for the appid.
			return (new byte[] { data[COMMAND_OFFSET], data[SUB_COMMAND_OFFSET], (byte) (data[APP_ID_OFFSET] & 0x7F) });
		}
	}

	public static ContainerType getContainertype(byte[] data)
	{
		return getContainertype( getAppId( data ) );
	}

	public static ContainerType getContainertype(byte appId)
	{
		if ((appId & RESPONSE_MASCK) == RESPONSE_MASCK)
		{
			return ContainerType.RESPONSE;
		}

		return ContainerType.REQUEST;
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
