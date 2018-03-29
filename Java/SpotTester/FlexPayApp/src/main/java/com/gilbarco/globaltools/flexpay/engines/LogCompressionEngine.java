/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file LogCompressionEngine.java
 * @author mgrieco
 * @date 19/09/2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */
package com.gilbarco.globaltools.flexpay.engines;

import java.io.File;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Observable;
import java.util.Vector;
import org.apache.commons.compress.archivers.ArchiveException;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import com.gilbarco.globaltools.edt.util.EdtCompressUtil;
import com.gilbarco.globaltools.edt.util.EdtFileUtil;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.FileResourceContentType;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmission.FileResourceTransmissionStatus;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log.IFileResourceTransmissionStorageFileLog;

/**
 * @author mgrieco
 * 
 * @brief This class provides the logic to compress files as Log Collection requires.
 */
public class LogCompressionEngine extends Observable
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log						logger			= LogFactory.getLog( LogCompressionEngine.class );

	// ******************************************************************
	// PRIVATE FIELDS
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS
	// ******************************************************************
	
	/**
	 * Execute The Uncompress/Compress of all requested files.
	 * @param filesToCompress 
	 * @param ppn 
	 * @param logTypeDirectoryName [In] Is the leaf name of the destination directory to use for the log compression. Expected values are "standard_log" and "secure_log"  
	 *
	 * @return true, if compression successful 
	 */
	public boolean compress(Vector<IFileResourceTransmissionStorageFileLog> filesToCompress, String ppn, String logTypeDirectoryName)
	{
		if (filesToCompress.isEmpty( ))
		{
			return true;
		}

		String timestampFolderName = filesToCompress.elementAt( 0 ).getFileLocation( ).getPath( );
		File timestampFolder = new File( timestampFolderName );
		File ipFolder = timestampFolder.getParentFile( );
		String temporaryFolder = ipFolder.getParentFile( ).getPath( ) + "\\temp_"+timestampFolder.getName( );
		File destinationFolder = new File( temporaryFolder + "\\"+ logTypeDirectoryName + "\\" + ppn + "_" + ipFolder.getName( ) + "\\" + timestampFolder.getName( ) + "\\" );
		
		EdtFileUtil.checkOrCreateDirectory( destinationFolder.toPath( ) );
		Iterator<IFileResourceTransmissionStorageFileLog> iter = filesToCompress.iterator( );
		
		List<File> untaredFiles = new LinkedList<File>( );
		while (iter.hasNext( ))
		{
			IFileResourceTransmissionStorageFileLog file = iter.next( );
			if (file.getTransferStatus( ).equals( FileResourceTransmissionStatus.DONE ))
			{
				File currFile = new File( file.getFilePath( ) );
				try
				{
					logger.info( "processing: " + currFile.getCanonicalPath( ) + " to " + destinationFolder.getPath( ) );
					
					if( file.getContentType( ).equals( FileResourceContentType.TAR_GZ ) )
					{
						File tarfile = EdtCompressUtil.unGzip( currFile, destinationFolder );
						untaredFiles.addAll( EdtCompressUtil.unTar(tarfile,destinationFolder) );
						EdtFileUtil.delete( tarfile, true );
					}
					else if (file.getContentType( ).equals( FileResourceContentType.TAR ))
					{
						untaredFiles.addAll( EdtCompressUtil.unTar( currFile, destinationFolder ) );
					}
					else
					{
						logger.info( destinationFolder.getCanonicalPath( ) + "\\" + currFile.getName( ) );
						EdtFileUtil.copyFile( new File( destinationFolder.getCanonicalPath( ) + "\\" + currFile.getName( ) ), currFile );
					}
				}
				catch (IOException | ArchiveException e)
				{
					logger.error( file.getName( ) + " - " + e.getMessage( ) );
					EdtFileUtil.delete( currFile, true );
					return false;
				}
				System.gc();
				EdtFileUtil.delete( currFile, true );
				setChanged( );
				notifyObservers( file.getName( ) );
			}
		}

		try
		{
			System.gc();
			String zipFileName = ipFolder.getParentFile( ).getAbsolutePath( ) + "\\" + ppn + "_" + ipFolder.getName( ) + "_" + timestampFolder.getName( ) + ".zip";
			EdtCompressUtil.createZipFile( temporaryFolder + "\\" + logTypeDirectoryName + "\\", zipFileName );
			EdtFileUtil.delete( new File(temporaryFolder), true );
			EdtFileUtil.delete( timestampFolder, true );
			if (ipFolder.list( ).length <= 0)
			{
				EdtFileUtil.delete( ipFolder, true );
			}
		}
		catch (Exception e)
		{
			logger.error( e.getMessage( ) );
			return false;
		}
		return true;
	}
}
