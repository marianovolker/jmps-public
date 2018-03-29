/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file ICompareStrategy.java
 * @author mgrieco
 * @date 23/09/2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */
package com.gilbarco.globaltools.flexpay.engines.strategies;

import java.io.File;

import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate;

/**
 * The Interface CompareStrategy.
 * Provides an interface for the compare unit model strategies.
 */
public interface ICompareStrategy
{
	/**
	 * Gets the package info request message.
	 *
	 * @return the package info request message
	 */
	public byte[] getPackageInfoRequest();

	/**
	 * Gets the package info name.
	 *
	 * @return the package info name
	 */
	public String getPackageInfoName();

	/**
	 * New package certificate according with the strategy.
	 * creates the appropriate certificate given a package file.
	 * 
	 * @param file the certificate file
	 * @return the ROMfsCertificate package certificate
	 */
	public ROMfsCertificate newPackageCertificate(File file);

	/**
	 * New package certificate according with the strategy.
	 * creates the appropriate certificate given a certificate data.
	 * 
	 * @param data the certificate data
	 * @return the ROMfsCertificate package certificate
	 */
	public ROMfsCertificate newPackageCertificate(byte[] data);
}
