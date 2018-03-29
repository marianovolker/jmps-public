/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceLogSecure.java
 * @author pcantarini
 * @date Wednesday, October 26, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.FileResourceContentType;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResource;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceContentType;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResource.FileResourceImp;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log.IFileResourceLog.FileResourceLogType;

public enum FileResourceLogSecure implements IFileResourceLog
{
	// filename, file type, file id, log type
	PCI(
					"PCI", //"CURR_0x17_0x00",
					0x17,
					0x00,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	PCI_HIST(
					"PCI_HIST", //"HIST_0x18_0x00,
					0x18,
					0x00,
					FileResourceContentType.TAR_GZ,
					FileResourceLogType.HIST);
	
	private IFileResource fileResourceInfo = null;
	private FileResourceLogType logFileResourceType = null;

	private FileResourceLogSecure (String fileName, int fileType, int fileId, IFileResourceContentType fileResourceContentType, FileResourceLogType logFileResourceType)
	{
		this.fileResourceInfo    = new FileResourceImp(fileName,fileType,fileId,fileResourceContentType);
		this.logFileResourceType = logFileResourceType;
	}

	@Override
	public byte getFileType()
	{
		return this.fileResourceInfo.getFileType();
	}

	@Override
	public byte getFileId()
	{
		return this.fileResourceInfo.getFileId();
	}

	@Override
	public String getName()
	{
		return this.fileResourceInfo.getName();
	}

	@Override
	public IFileResourceContentType getContentType()
	{
		return this.fileResourceInfo.getContentType();
	}

	@Override
	public FileResourceLogType getLogType()
	{
		return logFileResourceType;
	}

	@Override
	public boolean isSensitive()
	{
		return true;
	}
}
