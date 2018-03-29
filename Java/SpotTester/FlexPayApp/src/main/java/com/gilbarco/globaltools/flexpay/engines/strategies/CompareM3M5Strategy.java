/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file CompareM3M5Strategy.java
 * @author mgrieco 
 * @date 23/09/2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

package com.gilbarco.globaltools.flexpay.engines.strategies;

import java.io.File;

import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/**
 * The Class CompareM3M5Strategy.
 * Implements the strategy to compare software information for M3 and M5 unit model.
 */
public class CompareM3M5Strategy implements ICompareStrategy
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	// ******************************************************************
	// OVERRIDEN METHODS
	// ******************************************************************
	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy#getPackageInfoRequest()
	 */
	@Override
	public byte[] getPackageInfoRequest()
	{
		return SpotMessages.getInstance( ).getM3M5PackageInfoRequest( );
	}

	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy#getPackageInfoName()
	 */
	@Override
	public String getPackageInfoName()
	{
		return SpotMessageType.M3M5_ROMfs_PACKAGE_INFO.getName( );
	}

	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy#newPackageCertificate(java.io.File)
	 */
	@Override
	public ROMfsCertificate newPackageCertificate(File file)
	{
		return new ROMfsCertificate(file);
	}

	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy#newPackageCertificate(java.io.File)
	 */
	@Override
	public ROMfsCertificate newPackageCertificate(byte[] data)
	{
		return new ROMfsCertificate(data);
	}
}
