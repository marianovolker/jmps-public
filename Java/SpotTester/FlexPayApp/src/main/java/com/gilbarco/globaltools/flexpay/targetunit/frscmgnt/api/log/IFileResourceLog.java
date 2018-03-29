/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IFileResourceLog.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResource;

public interface IFileResourceLog extends IFileResource
{
	public enum FileResourceLogType
	{
		NULL,
		LOG,
		HIST,
		LINUX_HIST,
		EXTAPP_HIST;
	}

	public IFileResourceLog.FileResourceLogType getLogType();
	public boolean isSensitive();
}