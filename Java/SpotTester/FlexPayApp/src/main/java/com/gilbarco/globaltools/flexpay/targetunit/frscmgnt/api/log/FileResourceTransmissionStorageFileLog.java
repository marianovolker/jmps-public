/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceTransmissionStorageFileLog.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceContentType;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceLocation;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmission;

public class FileResourceTransmissionStorageFileLog extends IFileResourceTransmission.AFileResourceTransmission implements IFileResourceTransmissionStorageFileLog
{
	private FileResourceStorageFileLog fileResourceStorage; 
	
	public FileResourceTransmissionStorageFileLog(IFileResourceLog fileResource)
	{
		super();
		this.fileResourceStorage = new FileResourceStorageFileLog(fileResource);
	}

	public FileResourceTransmissionStorageFileLog(IFileResourceLog fileResource, IFileResourceLocation fileResourceLocation)
	{
		super();
		this.fileResourceStorage = new FileResourceStorageFileLog(fileResource,fileResourceLocation);
	}

	@Override
	public byte getFileType()
	{
		return this.fileResourceStorage.getFileType();
	}

	@Override
	public byte getFileId()
	{
		return this.fileResourceStorage.getFileId();
	}

	@Override
	public String getName()
	{
		return this.fileResourceStorage.getName();
	}

	@Override
	public IFileResourceContentType getContentType()
	{
		return this.fileResourceStorage.getContentType();
	}

	@Override
	public IFileResourceLog.FileResourceLogType getLogType()
	{
		return this.fileResourceStorage.getLogType();
	}

	@Override
	public boolean isSensitive()
	{
		return this.fileResourceStorage.isSensitive();
	}

	@Override
	public IFileResourceLocation getFileLocation()
	{
		return this.fileResourceStorage.getFileLocation();
	}

	@Override
	public void setFileLocation(IFileResourceLocation fileResourceLocation)
	{
		this.fileResourceStorage.setFileLocation(fileResourceLocation);
	}

	@Override
	public String getFileName()
	{
		return this.fileResourceStorage.getFileName();
	}

	@Override
	public String getFileExtension()
	{
		return this.fileResourceStorage.getFileExtension();
	}

	@Override
	public String getFilePath()
	{
		return this.fileResourceStorage.getFilePath();
	}
}