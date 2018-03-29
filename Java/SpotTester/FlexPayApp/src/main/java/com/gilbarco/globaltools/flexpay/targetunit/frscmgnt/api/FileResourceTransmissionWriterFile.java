/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceTransmissionWriterFile.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.DirectoryNotEmptyException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmission.FileResourceTransmissionStatus;

public class FileResourceTransmissionWriterFile implements IFileResourceTransmissionWriter
{
	static final transient Log logger = LogFactory.getLog( FileResourceTransmissionWriterFile.class );

	private IFileResourceTransmissionStorageFile fileResource;
	private OutputStream                         outputStream;
	
	public FileResourceTransmissionWriterFile (IFileResourceTransmissionStorageFile fileResource)
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
			}
			catch (IOException e)
			{
				logger.error("Error while closing unexpected old outputStream. " + e.getMessage( ), e);
			}
			outputStream = null;
		}
		try
		{
			outputStream = new BufferedOutputStream(Files.newOutputStream(Paths.get(fileResource.getFilePath()), StandardOpenOption.CREATE, StandardOpenOption.WRITE, StandardOpenOption.TRUNCATE_EXISTING), 524288); // Buffer size to hold before flushing is 512kb.
			bResult = true;
		}
		catch (IOException e)
		{
			logger.error("File " + Paths.get(fileResource.getFilePath()).toString() + " could not be created. " + e.getMessage( ), e);
		}
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
			logger.error("Buffer could not be wrote. " + e.getMessage( ), e);
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
				outputStream = null;
			}
			catch (IOException e)
			{
				logger.error("File " + Paths.get(fileResource.getFilePath()).toString() + " could not be properly closed. " + e.getMessage( ), e);
				outputStream = null;
			}

			Path closedFileToCheck = Paths.get(fileResource.getFilePath());
			try
			{
				if (Files.size(closedFileToCheck) > 0)
				{
					logger.info("File " + closedFileToCheck.toString() + " has been saved.");								
					bResult = true;
				}
				else
				{
				    Files.delete(closedFileToCheck);
					logger.warn("File " + closedFileToCheck.toString() + " has been reported but no content is stored. File removed.");								
				}
			}
			catch (NoSuchFileException e)
			{
				logger.error("File " + closedFileToCheck.toString() + " could not be properly checked. File not exists. " + e.getMessage( ), e);
			}
			catch (DirectoryNotEmptyException e)
			{
				logger.error("File " + closedFileToCheck.toString() + " could not be properly checked. Directory conflict. " + e.getMessage( ), e);
			}
			catch (IOException e)
			{
				logger.error("File " + closedFileToCheck.toString() + " could not be properly checked. " + e.getMessage( ), e);
			}
			catch (SecurityException e)
			{
				logger.error("File " + closedFileToCheck.toString() + " could not be properly accessed for check. " + e.getMessage( ), e);
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