/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceLocation.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api;

import java.nio.file.Path;
import java.nio.file.Paths;

public class FileResourceLocation implements IFileResourceLocation
{
	private Path path;
	
	public FileResourceLocation()
	{
		this.path = Paths.get("");
	}

	public FileResourceLocation(String path)
	{
		this.path = Paths.get(path);
	}

	@Override
	public String getPath()
	{
		return this.path.toString();
	}

	@Override
	public void setPath(String path)
	{
		this.path = Paths.get(path);
	}
}
