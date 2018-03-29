/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceTransmissionStorageMemoryLog.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceContentType;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmission;

public class FileResourceTransmissionStorageMemoryLog extends IFileResourceTransmission.AFileResourceTransmission implements IFileResourceTransmissionStorageMemoryLog
{
	private FileResourceStorageMemoryLog fileResourceStorage; 
	
	public FileResourceTransmissionStorageMemoryLog (IFileResourceLog fileResource)
	{
		super();
		this.fileResourceStorage = new FileResourceStorageMemoryLog(fileResource);
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
	public byte[] getData()
	{
		return this.fileResourceStorage.getData();
	}

	@Override
	public void setData(byte[] data)
	{
		this.fileResourceStorage.setData(data);
	}
}