/* getCommand when returns 3 values, these command, subcommand, and applicationID 
   All types need to have 3 values for getCommand() except the messages for applicationID equal zero (0)
  
*/

package com.gilbarco.globaltools.spotprotocol.api;

import com.gilbarco.globaltools.mgp.api.MessageType;

public enum SpotMessageType implements MessageType
{
	PING
	{
		@Override
		public String getName()
		{
			return "Ping";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x03 };
		}
	},
	LOGIN
	{
		@Override
		public String getName()
		{
			return "Login";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x01 };
		}
	},
	LOGOUT
	{
		@Override
		public String getName()
		{
			return "Logout";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x02 };
		}
	},
	RESET
	{
		@Override
		public String getName()
		{
			return "Reset";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x06 };
		}
	},
	
	/**
	 * Command used for Vanguard Units to access secure menu options.
	 * This message will be handle only if Dual Control is ongoing and if the “switch operation” is allowed by the token/authentication sent.
	 */
	SERVICE_MENU_SWITCH
	{
		@Override
		public String getName()
		{
			return "ServiceMenuSwitch";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x0D };
		}
	},
	
	/**
	 * Command used for Vanguard Units to get the serial number and the unit challenge.
	 * This message will be handle only if Dual Control is ongoing.
	 */
	GET_CHALLENGE
	{
		@Override
		public String getName()
		{
			return "GetChallenge";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x0E };
		}
	},	
	CREATE_WINDOW
	{
		@Override
		public String getName()
		{
			return "CreateWindow";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x30, 0x03, 0x01 };
		}
	},
	SHOW_WINDOW
	{
		@Override
		public String getName()
		{
			return "ShowWindow";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x30, 0x07, 0x01 };
		}
	},
	HIDE_WINDOW
	{
		@Override
		public String getName()
		{
			return "HideWindow";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x30, 0x08, 0x01};
		}

	},
	FILE_BROWSE
	{
		@Override
		public String getName()
		{
			return "FileBrowse";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x00, 0x03, 0x01 };
		}
	},
	FILE_DELETE
	{
		@Override
		public String getName()
		{
			return "FileDelete";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x00, 0x04, 0x01};
		}
	},
	M3M5_ROMfs_PACKAGE_ACTIVATION
	{
		@Override
		public String getName()
		{
			return "M3M5ROMfsPackageActivation";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x10, 0x04, 0x03};
		}
	},
	VANGUARD_ROMfs_PACKAGE_ACTIVATION
	{
		@Override
		public String getName()
		{
			return "VanguardROMfsPackageActivation";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x60, 0x04, 0x01};
		}
	},
	M3M5_ROMfs_PACKAGE_INFO
	{
		@Override
		public String getName()
		{
			return "M3M5ROMfsPackageInfo";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x10, 0x03, 0x03};
		}
	},
	VANGUARD_ROMfs_PACKAGE_INFO
	{
		@Override
		public String getName()
		{
			return "VanguardROMfsPackageInfo";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x60, 0x03, 0x01};
		}
	},
	M3M5_ROMfs_FILE_DOWNLOAD
	{
		@Override
		public String getName()
		{
			return "M3M5ROMfsFileDownload";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x10, 0x01, 0x03};
		}
	},
	VANGUARD_ROMfs_FILE_DOWNLOAD
	{
		@Override
		public String getName()
		{
			return "VanguardROMfsFileDownload";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x60, 0x01, 0x01};
		}
	},
	FILE_DOWNLOAD_REQUEST
	{
		@Override
		public String getName()
		{
			return "FileDownloadRequest";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x00, 0x01, 0x01};
		}
	},
	FILE_BLOCK_DOWNLOAD
	{
		@Override
		public String getName()
		{
			return "FileBlockDownload";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x00, 0x02, 0x01};
		}
	},
	M3M5_ROMfs_BLOCK_DOWNLOAD
	{
		@Override
		public String getName()
		{
			return "M3M5ROMfsBlockDownload";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x10, 0x02, 0x03 };
		}
	},
	VANGUARD_ROMfs_BLOCK_DOWNLOAD
	{
		@Override
		public String getName()
		{
			return "VanguardROMfsBlockDownload";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x60, 0x02, 0x01 };
		}
	},
	VANGUARD_MAINTENANCE
	{
		@Override
		public String getName()
		{
			return "VanguardMaintenance";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x0C };
		}
	},
	SYSTEM_STATUS
	{
		@Override
		public String getName()
		{
			return "SystemStatus";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x10, 0x01, 0x01 };
		}
	},
	HARDWARE_INFO
	{
		@Override
		public String getName()
		{
			return "HardwareInfo";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x10, 0x02, 0x01 };
		}
	},
	/*
	 * SOFTWARE_INFO {
	 * 
	 * @Override public String getName() { return "SoftwareInfo"; }
	 * 
	 * @Override public byte[] getCommand() { return new byte[] { 0x10, 0x03 }; } },
	 */
	DIAGNOSTIC
	{
		@Override
		public String getName()
		{
			return "Diagnostic";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x0F };
		}
	},
	OTI_SET
	{
		@Override
		public String getName()
		{
			return "OTISet";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x50, 0x06 };
		}
	},
	OTI_GET
	{
		@Override
		public String getName()
		{
			return "OTIGet";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x50, 0x07 };
		}
	},
	OTI_DO
	{
		@Override
		public String getName()
		{
			return "OTIDo";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x50, 0x08 };
		}
	},
	ENABLE_CL_READER
	{
		@Override
		public String getName()
		{
			return "EnableCardReader";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x50, 0x01 };
		}
	},
	DISABLE_CL_READER
	{
		@Override
		public String getName()
		{
			return "DisableCardReader";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x50, 0x05 };
		}
	},
	CL_DATA
	{
		@Override
		public String getName()
		{
			return "ContactlessData";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x50, 0x04 };
		}
	},
	FILE_UPLOAD
	{
		@Override
		public String getName()
		{
			return "FileUpload";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x00, 0x05, 0x01 };
		}
	},
	FILE_BLOCK_UPLOAD
	{
		@Override
		public String getName()
		{
			return "FileBlockUpload";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x00, 0x06, 0x01 };
		}
	},
	KEY_STATUS
	{ 
		@Override
		public String getName()
		{
			return "KeyStatus";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x40, 0x06, 0x01 };
		}
	},
	APPLICATION_KEY_DOWNLOAD
	{ 
		@Override
		public String getName()
		{
			return "ApplicationKeyDownload";
		}

		@Override
		public byte[] getCommand()
		{
			return new byte[]
			{ 0x40, 0x04, 0x01 };
		}
	};

	/**
	 * VGD_INTERFACE_APPLICATION constant for Vanguard application response id.
	 */
	private static final byte	VGD_INTERFACE_APPLICATION	= (byte) 0x80;

	public static SpotMessageType getByName(String name)
	{
		for (SpotMessageType msg : SpotMessageType.values( ))
			if (msg.getName( ).equals( name )) return msg;

		return null;
	}
	
	/**
	 * Gets the vanguard app response id.
	 *
	 * @return the vanguard app response id byte value.
	 */
	public static byte getVanguardAppResponseId()
	{
		return VGD_INTERFACE_APPLICATION;
	}
}
