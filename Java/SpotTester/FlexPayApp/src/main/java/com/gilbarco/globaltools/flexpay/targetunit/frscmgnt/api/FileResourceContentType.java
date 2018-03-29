/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceContentType.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api;

public enum FileResourceContentType implements IFileResourceContentType
{
	NULL(""),
	PLAIN_TEXT(".txt"),
	PLAIN_TEXT_LOG(".log"),
	TAR(".tar"),
	TAR_GZ(".tar.gz");

	private String fileExtension;
	
	private FileResourceContentType (String fileExtension)
	{
		this.fileExtension = fileExtension;
	}
	
	@Override
	public String getFileExtension() {
		return fileExtension;
	}
}