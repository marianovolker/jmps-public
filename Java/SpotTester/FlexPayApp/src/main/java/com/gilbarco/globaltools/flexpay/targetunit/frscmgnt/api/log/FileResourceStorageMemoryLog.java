/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceStorageMemoryLog.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceContentType;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceStorageMemory;

public class FileResourceStorageMemoryLog extends IFileResourceStorageMemory.AFileResourceStorageMemory implements IFileResourceStorageMemoryLog
{
	private IFileResourceLog fileResource;
	
	public FileResourceStorageMemoryLog (IFileResourceLog fileResource)
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
	public IFileResourceLog.FileResourceLogType getLogType()
	{
		return fileResource.getLogType();
	}

	@Override
	public boolean isSensitive()
	{
		return fileResource.isSensitive();
	}
}