/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file SoftwarePackageTypes.java
 * @author mgrieco
 * @date 25/09/2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */
package com.gilbarco.globaltools.flexpay.engines;

/**
 * The Enum SoftwarePackageTypes.
 * Recovers the info about all possible application files to update the unit. 
 */
public enum SoftwarePackageTypes
{

	/** The spot recovery version. */
	SPOT_RECOVERY_VERSION(0x01, "RECOVERY", "Recovery."),

	/** The spot os version. */
	SPOT_OS_VERSION(0x02, "OS", "Operating system."),

	/** The SPOT software versio n1. */
	SPOT_SWR_VERSION1(0x03, "PINPAD", "Interface and pinpad applications and related devices drivers."),

	/** The SPOT software versio n2. */
	SPOT_SWR_VERSION2(0x04, "EMV", "EMV application"),

	/** The SPOT software versio n3. */
	SPOT_SWR_VERSION3(0x05, "OPT", "OPT application  and others reserved applications for special SPOTterminal configurations."),

	/** The SPOT software versio n4. */
	SPOT_SWR_VERSION4(0x06, "SETUP", "SWdownload application."),

	/** The SPOT software versio n5. */
	SPOT_SWR_VERSION5(0x07, "GVROPTEMV", "Acquirer Custom Application1"),

	/** The SPOT software versio n6. */
	SPOT_SWR_VERSION6(0x08, "RESERVED", "RESERVED"),

	/** The SPOT software versio n7. */
	SPOT_SWR_VERSION7(0x09, "RESERVED", "RESERVED"),

	/** The SPOT software versio n8. */
	SPOT_SWR_VERSION8(0x0a, "CRINDBIOS", "CRINDBIOS"),

	/** The SPOT firmware versio n1. */
	SPOT_FWR_VERSION1(0x0b, "RD012-100204_1", "Firmware RD012-100204_1"),

	/** The SPOT firmware versio n2. */
	SPOT_FWR_VERSION2(0x0c, "KB0172", "Firmware KB0172"),

	/** The SPOT firmware versio n3. */
	SPOT_FWR_VERSION3(0x0d, "DY0178", "Firmware DY0178"),

	/** The spot recovery data. */
	SPOT_RECOVERY_DATA(0x0f, "RECOVERY_DATA", "Recovery data."),

	/** The spot boot recovery. */
	SPOT_BOOT_RECOVERY(0x1a, "BOOT_RECOVERY", "Boot recovery."),

	/** The spot boot version. */
	SPOT_BOOT_VERSION(0x1b, "BOOT", "Boot version."),

	/** The SPOT tools software. */
	SPOT_SWR_TOOLS(0x1c, "TOOLS", "Boot version."), //TODO
	
	/** The SPOT resources firmware. */
	SPOT_FWR_RESOURCES(0x1d, "RESOURCES", "Boot version."), //TODO
			
	/** The spot software  unknown. */
	SPOT_SWR_UNKNOWN(0x00, "UNKNOWN", "UNKNOWN");

	/** The software  number. */
	private int		swrNum;

	/** The software  name. */
	private String	swrName;

	/** The software  description. */
	private String	swrDescription;

	/**
	 * Gets the software number identification.
	 * 
	 * @return the software number
	 */
	public int getSwrNum()
	{
		return swrNum;
	}

	/**
	 * Gets the software name.
	 * 
	 * @return the software name
	 */
	public String getSwrName()
	{
		return swrName;
	}

	/**
	 * Gets the software description.
	 * 
	 * @return the software description
	 */
	public String getSwrDescription()
	{
		return swrDescription;
	}

	/**
	 * Instantiates a new spot software module.
	 * @param swrNum the software  number
	 * @param swrName the software  name
	 * @param swrDescription the software  description
	 */
	private SoftwarePackageTypes(int swrNum, String swrName, String swrDescription)
	{
		this.swrNum = swrNum;
		this.swrName = swrName;
		this.swrDescription = swrDescription;
	}

	/**
	 * From string.
	 *
	 * @param swrName the software  name
	 * @return the spot software module
	 */
	public static SoftwarePackageTypes fromString(String swrName)
	{
		if (swrName.equals( SPOT_RECOVERY_VERSION.getSwrName( ) ))
		{
			return SPOT_RECOVERY_VERSION;
		}
		else if (swrName.equals( SPOT_OS_VERSION.getSwrName( ) ))
		{
			return SPOT_OS_VERSION;
		}
		else if (swrName.equals( SPOT_SWR_VERSION1.getSwrName( ) ))
		{
			return SPOT_SWR_VERSION1;
		}
		else if (swrName.equals( SPOT_SWR_VERSION2.getSwrName( ) ))
		{
			return SPOT_SWR_VERSION2;
		}
		else if (swrName.equals( SPOT_SWR_VERSION3.getSwrName( ) ))
		{
			return SPOT_SWR_VERSION3;
		}
		else if (swrName.equals( SPOT_SWR_VERSION4.getSwrName( ) ))
		{
			return SPOT_SWR_VERSION4;
		}
		else if (swrName.equals( SPOT_SWR_VERSION5.getSwrName( ) ))
		{
			return SPOT_SWR_VERSION5;
		}
		else if (swrName.equals( SPOT_SWR_VERSION6.getSwrName( ) ))
		{
			return SPOT_SWR_VERSION6;
		}
		else if (swrName.equals( SPOT_SWR_VERSION7.getSwrName( ) ))
		{
			return SPOT_SWR_VERSION7;
		}
		else if (swrName.equals( SPOT_SWR_VERSION8.getSwrName( ) ))
		{
			return SPOT_SWR_VERSION8;
		}
		else if (swrName.equals( SPOT_FWR_VERSION1.getSwrName( ) ))
		{
			return SPOT_FWR_VERSION1;
		}
		else if (swrName.equals( SPOT_FWR_VERSION2.getSwrName( ) ))
		{
			return SPOT_FWR_VERSION2;
		}
		else if (swrName.equals( SPOT_FWR_VERSION3.getSwrName( ) ))
		{
			return SPOT_FWR_VERSION3;
		}
		else if (swrName.equals( SPOT_RECOVERY_DATA.getSwrName( ) ))
		{
			return SPOT_RECOVERY_DATA;
		}
		else if (swrName.equals( SPOT_BOOT_RECOVERY.getSwrName( ) ))
		{
			return SPOT_BOOT_RECOVERY;
		}
		else if (swrName.equals( SPOT_BOOT_VERSION.getSwrName( ) ))
		{
			return SPOT_BOOT_VERSION;
		}
		else if (swrName.equals( SPOT_SWR_TOOLS.getSwrName( ) ))
		{
			return SPOT_SWR_TOOLS;
		}
		else if (swrName.equals( SPOT_FWR_RESOURCES.getSwrName( ) ))
		{
			return SPOT_FWR_RESOURCES;
		}
		else
			return SPOT_SWR_UNKNOWN;
	}

	/**
	 * From int.
	 *
	 * @param swrNum the software number
	 * @return the spot software module according to the software number
	 */
	public static SoftwarePackageTypes fromInt(int swrNum)
	{
		if (swrNum == SPOT_RECOVERY_VERSION.getSwrNum( ))
		{
			return SPOT_RECOVERY_VERSION;
		}
		else if (swrNum == SPOT_OS_VERSION.getSwrNum( ))
		{
			return SPOT_OS_VERSION;
		}
		else if (swrNum == SPOT_SWR_VERSION1.getSwrNum( ))
		{
			return SPOT_SWR_VERSION1;
		}
		else if (swrNum == SPOT_SWR_VERSION2.getSwrNum( ))
		{
			return SPOT_SWR_VERSION2;
		}
		else if (swrNum == SPOT_SWR_VERSION3.getSwrNum( ))
		{
			return SPOT_SWR_VERSION3;
		}
		else if (swrNum == SPOT_SWR_VERSION4.getSwrNum( ))
		{
			return SPOT_SWR_VERSION4;
		}
		else if (swrNum == SPOT_SWR_VERSION5.getSwrNum( ))
		{
			return SPOT_SWR_VERSION5;
		}
		else if (swrNum == SPOT_SWR_VERSION6.getSwrNum( ))
		{
			return SPOT_SWR_VERSION6;
		}
		else if (swrNum == SPOT_SWR_VERSION7.getSwrNum( ))
		{
			return SPOT_SWR_VERSION7;
		}
		else if (swrNum == SPOT_SWR_VERSION8.getSwrNum( ))
		{
			return SPOT_SWR_VERSION8;
		}
		else if (swrNum == SPOT_FWR_VERSION1.getSwrNum( ))
		{
			return SPOT_FWR_VERSION1;
		}
		else if (swrNum == SPOT_FWR_VERSION2.getSwrNum( ))
		{
			return SPOT_FWR_VERSION2;
		}
		else if (swrNum == SPOT_FWR_VERSION3.getSwrNum( ))
		{
			return SPOT_FWR_VERSION3;
		}
		else if (swrNum == SPOT_RECOVERY_DATA.getSwrNum( ))
		{
			return SPOT_RECOVERY_DATA;
		}
		else if (swrNum == SPOT_BOOT_RECOVERY.getSwrNum( ))
		{
			return SPOT_BOOT_RECOVERY;
		}
		else if (swrNum == SPOT_BOOT_VERSION.getSwrNum( ))
		{
			return SPOT_BOOT_VERSION;
		}
		else if (swrNum ==  SPOT_SWR_TOOLS.getSwrNum( ) )
		{
			return SPOT_SWR_TOOLS;
		}
		else if (swrNum ==  SPOT_FWR_RESOURCES.getSwrNum( ) )
		{
			return SPOT_FWR_RESOURCES;
		}
		else
			return SPOT_SWR_UNKNOWN;
	}

	/**
	 * @param appId
	 * @return
	 */
	public static boolean isVanguardComparable(SoftwarePackageTypes app)
	{
		return (!app.equals( SoftwarePackageTypes.SPOT_BOOT_VERSION )) // VGD don't compare TOOLS and BOOT files.
				&& (!app.equals( SoftwarePackageTypes.SPOT_SWR_TOOLS ));
	}
}