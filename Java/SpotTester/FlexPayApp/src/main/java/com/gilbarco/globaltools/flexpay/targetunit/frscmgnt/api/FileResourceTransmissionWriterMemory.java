/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceTransmissionWriterMemory.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmission.FileResourceTransmissionStatus;

public class FileResourceTransmissionWriterMemory implements IFileResourceTransmissionWriter
{
	static final transient Log logger = LogFactory.getLog( FileResourceTransmissionWriterMemory.class );

	private IFileResourceTransmissionStorageMemory fileResource;
	private ByteArrayOutputStream                  outputStream;
	
	public FileResourceTransmissionWriterMemory (IFileResourceTransmissionStorageMemory fileResource)
	{
		this.fileResource = fileResource;
	}
	
	@Override
	public byte getFileType()
	{
		return fileResource.getFileType();
	}

	@Override
	public byte getFileId()
	{
		return fileResource.getFileId();
	}

	@Override
	public String getName()
	{
		return fileResource.getName();
	}

	@Override
	public IFileResourceContentType getContentType()
	{
		return fileResource.getContentType();
	}

	@Override
	public FileResourceTransmissionStatus getTransferStatus()
	{
		return fileResource.getTransferStatus();
	}

	@Override
	public void setTransferStatus(FileResourceTransmissionStatus status)
	{
		fileResource.setTransferStatus(status);
	}

	@Override
	public boolean open()
	{
		boolean bResult = false;
		if (outputStream != null)
		{
			try
			{
				outputStream.close();
				fileResource.setData(null);
			}
			catch (IOException e)
			{
				logger.error( "Error while closing unexpected old outputStream. " + e.getMessage( ), e );
			}
			outputStream = null;
		}
		outputStream = new ByteArrayOutputStream();
		bResult = true;
		return bResult;
	}

	@Override
	public boolean write(byte[] data)
	{
		boolean bResult = false;
		try
		{
			outputStream.write(data);
			bResult = true;
		}
		catch (IOException e)
		{
			outputStream = null;
			logger.error("Buffer could not be wrote. " + e.getMessage( ), e );
		}
		return bResult;
	}

	@Override
	public boolean close()
	{
		boolean bResult = false;
		if (outputStream != null)
		{
			try
			{
				outputStream.close();
				fileResource.setData(outputStream.toByteArray());
				outputStream = null;
				bResult = true;
			}
			catch (IOException e)
			{
				outputStream = null;
				logger.error("Buffer could not be closed. " + e.getMessage( ), e );
			}
		}
		return bResult;
	}

	@Override
	public IFileResourceTransmissionStorage getFileResourceTransmited()
	{
		return fileResource;
	}
}