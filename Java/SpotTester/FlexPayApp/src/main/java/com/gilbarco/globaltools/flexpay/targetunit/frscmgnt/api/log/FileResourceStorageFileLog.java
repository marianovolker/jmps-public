/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceStorageFileLog.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceContentType;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceLocation;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceStorageFile;

public class FileResourceStorageFileLog extends IFileResourceStorageFile.AFileResourceStorageFile implements IFileResourceStorageFileLog
{
	private IFileResourceLog fileResource;
	
	public FileResourceStorageFileLog(IFileResourceLog fileResource)
	{
		super();
		this.fileResource = fileResource;
	}

	public FileResourceStorageFileLog(IFileResourceLog fileResource, IFileResourceLocation fileResourceLocation)
	{
		super(fileResourceLocation);
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