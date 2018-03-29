/**
 * 
 */
package com.gilbarco.globaltools.flexpay.maintenance.resources;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * It extends the File functionality to a signed resource file, "having" a
 * File reference inside (not by inheritance).
 * 
 * @author gparis
 */
public class SignedResource extends DownloadableFile
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	protected static final int MARKER_LENGTH = 8;
	protected static final int SIGNATURE_LENGTH = 26;
	protected static final byte abMarker[] = {'/', 'r', 'e', 's', 'M', 'a', 'r', 'k'};
	private static final transient Log	logger	= LogFactory.getLog( SignedResource.class );


	// ******************************************************************
	// PUBLIC FIELDS
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS
	// ******************************************************************
	private long		iSignatureOffset = 0;


	// ******************************************************************
	// CONSTRUCTORS
	// ******************************************************************

	/**
	 * @param pathname
	 */
	public SignedResource(String pathname)
	{
		super(pathname);
		commonConstructor();
	}

	/**
	 * @param parent
	 * @param child
	 */
	public SignedResource(String parent, String child)
	{
		super(parent, child);
		commonConstructor();
	}

	/**
	 * @param parent
	 * @param child
	 */
	public SignedResource(File parent, String child)
	{
		super(parent, child);
		commonConstructor();
	}

	/**
	 * @param file : a pre-existent file object
	 */
	public SignedResource(File file)
	{
		super(file);
		commonConstructor();
	}

	private void commonConstructor()
	{
		try
		{
			iSignatureOffset = searchForSignature();
		}
		catch (FileNotFoundException e)
		{
			logger.error(e.getMessage(), e);
			blError = true;
		}
		catch (IOException e)
		{
			logger.error(e.getMessage(), e);
			blError = true;
		}

		if(iSignatureOffset == -1) // no signature found
			iSignatureOffset = file.length();
	}


	// ******************************************************************
	// OVERRIDDEN METHODS  (invoked from polymorphic interface).
	// ******************************************************************

	@Override
	public long length()
	{
		return iSignatureOffset;
	}


	// ******************************************************************
	// PUBLIC METHODS	  (general, getter, setter, interface imp)
	// ******************************************************************

	private long searchForSignature() throws FileNotFoundException, IOException
	{
		int nblock = -1 ;
		int nread = 0;
		int iPartialMatchPos = 0;
		FileInputStream fis = new FileInputStream(file);
		byte buffer[] = new byte[MAX_FILEBLOCK_LENGTH];

		while(nread >= 0) // this iterates for each data block of the resource file
		{
			nblock++;
			nread = fis.read(buffer);
			if(nread < 0)
				break;

			// This also verify a possible partial match pending from the former data block
			// That is, a valid signature laid between two consecutive file blocks.
			iPartialMatchPos = searchIntoBlock(iPartialMatchPos, buffer);
			if(iPartialMatchPos == MARKER_LENGTH) // this means signature totally found
				break;

			if(nread < MAX_FILEBLOCK_LENGTH) // all resource file has been read
				break;
		}

		long lret = -1;

		if(iPartialMatchPos == MARKER_LENGTH) // search complete and successful
		{
			// full offset into the file
			lret = nblock * MAX_FILEBLOCK_LENGTH + iSignatureOffset - MARKER_LENGTH;
		}

		try
		{
			fis.close();
		}
		catch (IOException e)
		{
			// message is logged (very weird) but no error is set (deferred to a later open)
			logger.error(e.getMessage(), e);
		}

		return lret;
	}

	private int searchIntoBlock(int iPartialMatchPos, byte[] buffer)
	{
		int i = 0;
		while(i < MAX_FILEBLOCK_LENGTH && iPartialMatchPos < MARKER_LENGTH)
		{
			if( buffer[i++] == abMarker[iPartialMatchPos] )
			{
				iPartialMatchPos++;
			}
			else
			{
				iPartialMatchPos = 0; // not a complete ("fake") signature found
			}
		}

		if(iPartialMatchPos == MARKER_LENGTH) // search complete and successful
		{
			this.iSignatureOffset = i; // offset within the block by now
		}

		return iPartialMatchPos;
	}
}
