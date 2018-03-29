/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file CompareVanguardStrategy.java
 * @author mgrieco
 * @date 23/09/2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

package com.gilbarco.globaltools.flexpay.engines.strategies;

import java.io.File;

import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.VanguardROMfsCertificate;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/**
 * The Class CompareVanguardStrategy.
 * Implements the strategy to compare software information forvanguard unit model.
 */
public class CompareVanguardStrategy implements ICompareStrategy
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	// ******************************************************************
	// OVERRIDEN METHODS
	// ******************************************************************
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy#getPackageInfoRequest()
	 */
	@Override
	public byte[] getPackageInfoRequest()
	{
		return SpotMessages.getInstance( ).getVanguardPackageInfoRequest( ); // TODO
	}

	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy#getPackageInfoName()
	 */
	@Override
	public String getPackageInfoName()
	{
		return SpotMessageType.VANGUARD_ROMfs_PACKAGE_INFO.getName( );
	}

	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy#newPackageCertificate(java.io.File)
	 */
	@Override
	public ROMfsCertificate newPackageCertificate(File file)
	{
		return new VanguardROMfsCertificate(file);
	}

	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy#newPackageCertificate(java.io.File)
	 */
	@Override
	public ROMfsCertificate newPackageCertificate(byte[] data)
	{
		return new VanguardROMfsCertificate(data);
	}
}
