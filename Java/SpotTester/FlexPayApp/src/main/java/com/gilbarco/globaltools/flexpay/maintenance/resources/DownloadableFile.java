package com.gilbarco.globaltools.flexpay.maintenance.resources;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * It is the base class for files to be downloaded to SPOT
 * It contains a File reference inside (not by inheritance).
 * 
 * @author gparis
 *
 */
public class DownloadableFile
{
	// ******************************************************************
	// INNER TYPES 
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS 
	// ******************************************************************
	protected static final int	MAX_FILEBLOCK_LENGTH = 		8192;
	private static final transient Log	logger	= LogFactory.getLog( DownloadableFile.class );

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected File			file = null;
	protected boolean		blError = false;
	protected boolean		blEOF = false;
	protected byte[]		abFileData = null;
//	protected byte[]		abCertificateData = null;
	
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************

	/**
	 * @param pathname
	 */
	public DownloadableFile(String pathname)
	{
		file = new File(pathname);
	}

	/**
	 * @param parent
	 * @param child
	 */
	public DownloadableFile(String parent, String child)
	{
		file = new File(parent, child);
	}

	/**
	 * @param parent
	 * @param child
	 */
	public DownloadableFile(File parent, String child)
	{
		file = new File(parent, child);
	}

	/**
	 * @param file : a pre-existent file object
	 */
	public DownloadableFile(File file)
	{
		this.file = file;
	}

	// ******************************************************************
	// OVERRIDDEN METHODS  (invoked from polymorphic interface).
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS	  (general, getter, setter, interface imp)
	// ******************************************************************

	public boolean hasError()
	{
		return blError;
	}

	public boolean eof()
	{
		return blEOF;
	}

	public long length()
	{
		if(file == null)
			return -1;

		return file.length();
	}

	public String getName()
	{
		return file.getName();
	}

	public File getFile()
	{
		return file;
	}

/*
	public byte[] getCertificateData()
	{
		return abCertificateData;
	}
*/

	public byte[] getDataBlock(int iOffset, int iBlockLength)
	{
		if( iOffset >= this.length() )
		{
			blEOF = true;
			return null; // no more data
		}

		int nread = iBlockLength;

		if( this.length() < iOffset + iBlockLength ) // This is last block to read
			nread = (int) this.length() - iOffset;   // so nread is adjusted.

		if(abFileData == null)
			if(getData() == null)
				return null;

		return Arrays.copyOfRange(abFileData, iOffset, iOffset + nread);
	}

	public byte[] getData()
	{
		if(abFileData != null)
			return abFileData;

		synchronized(this)
		{
			if(abFileData != null)
				return abFileData;

			int nread = 0;
			FileInputStream fis = null;

			try
			{
				fis = new FileInputStream(file);
				abFileData = new byte[(int) this.length()];
				nread = fis.read(abFileData);
				if(nread < this.length())
				{
					String sMsg = String.format("Only %d bytes were read from resource having length %d.",
													nread, this.length());
					Arrays.fill(abFileData, nread, (int) (this.length()), (byte) 0);
					logger.error(sMsg);
					blError = true;
				}
			}
			catch (FileNotFoundException e)
			{
				logger.error(e.getMessage(), e);
				abFileData = null;
				blError = true;
				return null;
			}
			catch (IOException e)
			{
				logger.error(e.getMessage(), e);
				abFileData = null;
				blError = true;
			}

			try
			{
				fis.close();
			}
			catch (IOException e)
			{
				logger.error(e.getMessage(), e);
			}

			return (blError ? null : abFileData);
		}
	}


	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// STATIC METHODS 
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
