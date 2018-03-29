
package com.gilbarco.globaltools.spotlogcollector.state;

/**
 * @author mgrieco
 * 
 * @Description Possible log files.
 * 
 * @version 1.0.2
 */

public enum SPOTLogCollectorLogFiles
{
	// filename, file type, file id, log type
	PINPAD(
					"CURR_0x13_0x00.log", //"PINPAD.log",
					0x13,
					0x00,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	PINPAD_HIST(
					"HIST_0x13_0x80.tar", //"PINPAD_HIST.tar",
					0x13,
					0x80,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	SYSMANSERV(
					"CURR_0x13_0x01.log", //"sysmanserv.log",
					0x13,
					0x01,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	SYSMANSERV_HIST(
					"HIST_0x13_0x81.tar", //"sysmanserv_HIST.tar",
					0x13,
					0x81,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	DISPCLIB(
					"CURR_0x13_0x02.log", //"dispclib.log",
					0x13,
					0x02,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	DISPCLIB_HIST(
					"HIST_0x13_0x82.tar", //"dispclib_HIST.tar",
					0x13,
					0x82,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	JOURNAL(
					"CURR_0x13_0x03.log", //"journal.log",
					0x13,
					0x03,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	JOURNAL_HIST(
					"HIST_0x13_0x83.tar", //"journal_HIST.tar",
					0x13,
					0x83,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	ABI(
					"CURR_0x13_0x04.log", //"ABI.log",
					0x13,
					0x04,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	ABI_HIST(
					"HIST_0x13_0x84.tar", //"ABI_HIST.tar",
					0x13,
					0x84,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	EMV(
					"CURR_0x23_0x00.log", //"EMV.log",
					0x23,
					0x00,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	EMV_HIST(
					"HIST_0x23_0x80.tar", //"EMV_HIST.tar",
					0x23,
					0x80,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	OPT(
					"CURR_0x40_0x00.log", //"OPT.log",
					0x40,
					0x00,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	OPT_HIST(
					"HIST_0x40_0x80.tar", //"OPT_HIST.tar",
					0x40,
					0x80,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	OPTJOURNAL(
					"CURR_0x40_0x01.log", //"optjournal.log",
					0x40,
					0x01,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	OPTJOURNAL_HIST(
					"HIST_0x40_0x81.tar", //"optjournal_HIST.tar",
					0x40,
					0x81,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	SETUP(
					"CURR_0x30_0x00.log", //"SETUP.log",
					0x30,
					0x00,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	SETUP_HIST(
					"HIST_0x30_0x80.tar", //"SETUP_HIST.tar",
					0x30,
					0x80,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	GVROPTEMV(
					"CURR_0x50_0x00.log", //"GVROPTEMV.log", "AS2805.log",
					0x50,
					0x00,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	GVROPTEMV_HIST(
					"HIST_0x50_0x80.tar", //"GVROPTEMV_HIST.tar", "AS2805_HIST.tar",
					0x50,
					0x80,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	AS2805(
					"CURR_0x5E_0x00.log", // "AS2805.log"
					0x5E,
					0x00,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	AS2805_HIST(
					"HIST_0x5E_0x80.tar", // "AS2805_HIST.tar",
					0x5E,
					0x80,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	CVXADAPTER(
					"CURR_0x7F_0x00.log", //"CVXADAPTER.log",
					0x7F,
					0x00,
					SPOTLogCollectorLogType.TYPELOG_LOG),

	CVXADAPTER_HIST(
					"HIST_0x7F_0x80.tar", //"CVXADAPTER_HIST.tar",
					0x7F,
					0x80,
					SPOTLogCollectorLogType.TYPELOG_TAR),

	LINUXKERNEL(
					"HIST_0x13_0x05.tar.gz", //"LINUXKERNEL.tar.gz",
					0x13,
					0x05,
					SPOTLogCollectorLogType.TYPELOG_LINUX),
					
	EXTAPP_EXT0(
					"HIST_0x71_0x80.tar", //"80.CrindBios.tar",
					0x71,
					0x80,
					SPOTLogCollectorLogType.TYPELOG_EXTAPP),

	EXTAPP_EXT1(
					"HIST_0x72_0x01.tar.gz", //"01.CrindBios.tar.gz",
					0x72,
					0x01,
					SPOTLogCollectorLogType.TYPELOG_EXTAPP),

	EXTAPP_EXT2(
					"HIST_0x72_0x02.tar.gz", //"02.DispMan.tar.gz",
					0x72,
					0x02,
					SPOTLogCollectorLogType.TYPELOG_EXTAPP), ;

	// ******************************************************************
	// PRIVATE FIELDS
	// ******************************************************************

	private String							fileName	= null;
	private byte							fileType	= -1;
	private byte							fileId		= -1;
	private SPOTLogCollectorLogType			spotLog		= SPOTLogCollectorLogType.TYPELOG_NULL;
	private SPOTLogCollectorLogFilesStatus	fileStatus	= null;

	// ******************************************************************
	// CONSTRUCTOR
	// ******************************************************************
	private SPOTLogCollectorLogFiles(String fileName, int fileType, int fileId, SPOTLogCollectorLogType spotLog)
	{
		this.fileName = fileName;
		this.fileType = (byte) fileType;
		this.fileId = (byte) fileId;
		this.spotLog = spotLog;
		this.fileStatus = SPOTLogCollectorLogFilesStatus.NO_SENT;
	}

	// ******************************************************************
	// GETTERS AND SETTERS
	// ******************************************************************
	public String getFileName()
	{
		return fileName;
	}

	public byte getFileType()
	{
		return fileType;

	}

	public byte getFileId()
	{
		return fileId;
	}

	public SPOTLogCollectorLogType getLogType()
	{
		return spotLog;
	}

	public SPOTLogCollectorLogFilesStatus getFileStatus()
	{
		return fileStatus;
	}

	public void setFileStatus(SPOTLogCollectorLogFilesStatus status)
	{
		fileStatus = status;
	}
}
