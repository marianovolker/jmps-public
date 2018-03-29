/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file ReleaseBundleCopyEngine.java
 * @author mgrieco
 * @date 11/09/2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */
package com.gilbarco.globaltools.flexpay.engines;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.util.Observable;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.util.EdtFileUtil;


/**
 * @author mgrieco
 *
 * @brief This class provides the logic to generate a release bundle to be installed on an Unit for example by USB
 * 
 */
public class ReleaseBundleCopyEngine extends Observable
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log		logger						= LogFactory.getLog( ReleaseBundleCopyEngine.class );

	private static final String				VGD_DEFAULT_APP_EXTENSION	= ".vgd";
	private static final String				VGD_NEW_EXTENSION			= "new";
	private static final String				VGD_TYPE_PARAMETER_KEY		= "TYPE=";
	private static final String				VGD_APPLICATION_TYPE		= "APP";
	private static final String				VGD_FIRMWARE_TYPE			= "FWR";
	private static final int				VGD_TYPE_PARAMETER_LENGTH	= 3;

	private static ReleaseBundleCopyEngine	instance;

	private static synchronized ReleaseBundleCopyEngine createInstance() throws EdtServiceException
	{
		if (instance == null)
		{
			instance = new ReleaseBundleCopyEngine( );
		}

		return instance;
	}

	// ******************************************************************
	// PRIVATE METHODS
	// ******************************************************************

	private static boolean isFirmWaReFile(File sourceFile) throws FileNotFoundException, IOException
	{
		return getVanguardFileType( sourceFile.getAbsolutePath( ) ).equals( VGD_FIRMWARE_TYPE );
	}

	private static boolean isApplicationFile(File sourceFile) throws FileNotFoundException, IOException
	{
		return getVanguardFileType( sourceFile.getAbsolutePath( ) ).equals( VGD_APPLICATION_TYPE );
	}

	private static String getVanguardFileType(String filePath) throws FileNotFoundException, IOException
	{
		String filedata = EdtFileUtil.loadFile( filePath, false );
		int position = filedata.lastIndexOf( VGD_TYPE_PARAMETER_KEY ) + VGD_TYPE_PARAMETER_KEY.length( );
		String fileType = filedata.substring( position, position + VGD_TYPE_PARAMETER_LENGTH );
		return fileType;
	}

	/**
	 * Collect release bundle files.
	 * 
	 * @param path
	 *            the path of the release
	 * @return the vector of the collected files
	 * @throws EdtServiceException
	 *             the edt service exception
	 * @throws IOException
	 *             Signals that an I/O exception has occurred.
	 */
	private static Vector<File> collectReleaseBundleFiles(String path) throws EdtServiceException, IOException
	{
		Vector<File> filesToStore;
		filesToStore = new Vector<File>( );

		File currentFolder = new File( path );
		if (!currentFolder.exists( ) || !currentFolder.isDirectory( ))
		{
			logger.error( "Version folder: " + path + "  doesn't exist. Update finished.\n" );
			return null;
		}

		File[] fileVector = currentFolder.listFiles( );
		for (File file : fileVector)
		{
			if (file.isDirectory( ))
			{
				filesToStore.addAll( collectReleaseBundleFiles( file.getCanonicalPath( ) ) );
			}
			else
			{
				if (!file.exists( ))
				{
					return null;
				}
				filesToStore.addElement( file );
			}
		}
		return filesToStore;
	}

	// ******************************************************************
	// PUBLIC METHODS
	// ******************************************************************

	/**
	 * @return
	 * @throws EdtServiceException 
	 */
	public static ReleaseBundleCopyEngine getInstance() throws EdtServiceException
	{
		if (instance != null)
		{
			return instance;
		}

		return createInstance( );
	}
	
	/**
	 * Store release bundle with the required directory structure.
	 * 
	 * @param strSourceVersions
	 *            the string path of source versions
	 * @param destFirmwareStr
	 *            the destination firmware path
	 * @param destApplicationStr
	 *            the destination application path
	 * @param destSecureLogsStr
	 *            the destination secure logs path
	 * @param destStandardLogsStr
	 *            the destination standard logs path
	 * @return true, if successful
	 */
	public boolean storeReleaseBundle(String strSourceVersions, String destFirmwareStr, String destApplicationStr)
	{
		try
		{
			Vector<File> filesToStore = collectReleaseBundleFiles( strSourceVersions );

			EdtFileUtil.delete( new File(destFirmwareStr), true);
			EdtFileUtil.delete( new File(destApplicationStr), true);

			if (!EdtFileUtil.checkOrCreateDirectory( FileSystems.getDefault( ).getPath( destFirmwareStr ) )
				|| !EdtFileUtil.checkOrCreateDirectory( FileSystems.getDefault( ).getPath( destApplicationStr ) ))
			{
				logger.error( "storeBundlesToUSB - Error creating directory to store releas" );
				return false;
			}
			
			// send the files to the destination
			for (int i = 0; i < filesToStore.size( ); ++i)
			{
				File destFile;
				File sourceFile = filesToStore.elementAt( i );

				String destFilename = sourceFile.getName( ) + "." + VGD_NEW_EXTENSION;
				destFile = isApplicationFile( sourceFile ) ? new File( destApplicationStr + "\\" + destFilename ) : isFirmWaReFile( sourceFile ) ? new File( destFirmwareStr + "\\" + destFilename )
																																				: null;
				if (destFile != null)
				{
					EdtFileUtil.copyFile( destFile, sourceFile );
					logger.info( sourceFile.getName( ) + " - file copied"  );
				}
				else
				{
					logger.info( sourceFile.getName( ) + " - file skiped"  );
				}
				setChanged();
				notifyObservers( sourceFile.getName( ) );
			}
		}
		catch (EdtServiceException | IOException e)
		{
			logger.error( "Error collecting files to store " + e.getMessage( ) );
			return false;
		}

		return true;
	}

	/**
	 * Checks if is a release bundle is a vanguard one.
	 * 
	 * @param path
	 *            the path of the release
	 * @return true, if is a vanguard release bundle
	 */
	public static boolean isAVanguardReleaseBundle(String path)
	{
		File currentFolder = new File( path );
		if (!currentFolder.exists( ) || !currentFolder.isDirectory( ))
		{
			return false;
		}

		File[] fileVector = currentFolder.listFiles( );
		for (File file : fileVector)
		{
			if (file.isDirectory( ))
			{
				try
				{
					if (isAVanguardReleaseBundle( file.getCanonicalPath( ) ))
																				return true;
				}
				catch (IOException e)
				{
					return false; // error
				}
			}
			else
			{
				if (!file.exists( )) // error
				{
					return false;
				}

				if (file.getName( ).endsWith( VGD_DEFAULT_APP_EXTENSION ))
				{
					return true;
				}
			}
		}
		return false;
	}

}
