/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceLogStandard.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.FileResourceContentType;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResource;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceContentType;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResource.FileResourceImp;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log.IFileResourceLog.FileResourceLogType;

public enum FileResourceLogStandard implements IFileResourceLog
{
	// filename, file type, file id, log type
	PINPAD(
					"PINPAD", //"CURR_0x13_0x00",
					0x13,
					0x00,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	PINPAD_HIST(
					"PINPAD_HIST", //"HIST_0x13_0x80",
					0x13,
					0x80,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	SYSMANSERV(
					"SYSMANSERV", //"CURR_0x13_0x01",
					0x13,
					0x01,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	SYSMANSERV_HIST(
					"SYSMANSERV_HIST", //"HIST_0x13_0x81",
					0x13,
					0x81,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	DISPCLIB(
					"DISPCLIB", //"CURR_0x13_0x02",
					0x13,
					0x02,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	DISPCLIB_HIST(
					"DISPCLIB_HIST", //"HIST_0x13_0x82",
					0x13,
					0x82,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	JOURNAL(
					"JOURNAL", //"CURR_0x13_0x03",
					0x13,
					0x03,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	JOURNAL_HIST(
					"JOURNAL_HIST", //"HIST_0x13_0x83",
					0x13,
					0x83,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	ABI(
					"ABI", //"CURR_0x13_0x04",
					0x13,
					0x04,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	ABI_HIST(
					"ABI_HIST", //"HIST_0x13_0x84",
					0x13,
					0x84,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	EMV(
					"EMV", //"CURR_0x23_0x00",
					0x23,
					0x00,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	EMV_HIST(
					"EMV_HIST", //"HIST_0x23_0x80",
					0x23,
					0x80,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	OPT(
					"OPT", //"CURR_0x40_0x00",
					0x40,
					0x00,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	OPT_HIST(
					"OPT_HIST", //"HIST_0x40_0x80",
					0x40,
					0x80,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	OPTJOURNAL(
					"OPTJOURNAL", //"CURR_0x40_0x01",
					0x40,
					0x01,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	OPTJOURNAL_HIST(
					"OPTJOURNAL_HIST", //"HIST_0x40_0x81",
					0x40,
					0x81,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	SETUP(
					"SETUP", //"CURR_0x30_0x00",
					0x30,
					0x00,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	SETUP_HIST(
					"SETUP_HIST", //"HIST_0x30_0x80",
					0x30,
					0x80,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	GVROPTEMV(
					"GVROPTEMV", //"CURR_0x50_0x00"
					0x50,
					0x00,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	GVROPTEMV_HIST(
					"GVROPTEMV_HIST", //"HIST_0x50_0x80"
					0x50,
					0x80,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	AS2805(
					"AS2805", //"CURR_0x5E_0x00"
					0x5E,
					0x00,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	AS2805_HIST(
					"AS2805_HIST", //"HIST_0x5E_0x80",
					0x5E,
					0x80,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	CVXADAPTER(
					"CVXADAPTER", //"CURR_0x7F_0x00",
					0x7F,
					0x00,
					FileResourceContentType.PLAIN_TEXT_LOG,
					FileResourceLogType.LOG),

	CVXADAPTER_HIST(
					"CVXADAPTER_HIST", //"HIST_0x7F_0x80",
					0x7F,
					0x80,
					FileResourceContentType.TAR,
					FileResourceLogType.HIST),

	LINUXKERNEL(
					"LINUXKERNEL_ALL", //"HIST_0x13_0x05",
					0x13,
					0x05,
					FileResourceContentType.TAR_GZ,
					FileResourceLogType.LINUX_HIST),
					
	EXTAPP_EXT0(
					"80.CrindBios_ALL", //"HIST_0x71_0x80",
					0x71,
					0x80,
					FileResourceContentType.TAR,
					FileResourceLogType.EXTAPP_HIST),

	EXTAPP_EXT1(
					"01.CrindBios_ALL", //"HIST_0x72_0x01",
					0x72,
					0x01,
					FileResourceContentType.TAR_GZ,
					FileResourceLogType.EXTAPP_HIST),

	EXTAPP_EXT2(
					"02.DispMan_ALL", //"HIST_0x72_0x02",
					0x72,
					0x02,
					FileResourceContentType.TAR_GZ,
					FileResourceLogType.EXTAPP_HIST);
	
	private IFileResource fileResourceInfo = null;
	private FileResourceLogType logFileResourceType = null;

	private FileResourceLogStandard (String fileName, int fileType, int fileId, IFileResourceContentType fileResourceContentType, FileResourceLogType logFileResourceType)
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
		return false;
	}
}