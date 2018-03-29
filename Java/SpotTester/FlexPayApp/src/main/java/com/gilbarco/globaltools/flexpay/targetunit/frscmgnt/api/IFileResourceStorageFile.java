/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IFileResourceStorageFile.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api;

import java.nio.file.Paths;

public interface IFileResourceStorageFile extends IFileResourceStorage
{
	public abstract class AFileResourceStorageFile implements IFileResourceStorageFile
	{
		IFileResourceLocation fileResourceLocation;
		
		public AFileResourceStorageFile ()
		{
			this.fileResourceLocation = new IFileResourceLocation() {
				private String path;

				public IFileResourceLocation initialize ()
				{
					path = "";
					return this;
				}
				
				@Override
				public String getPath()
				{
					return path;
				}

				@Override
				public void setPath(String path)
				{
					this.path = path;
				}
				
			}.initialize();
		}

		public AFileResourceStorageFile (IFileResourceLocation fileResourceLocation)
		{
			this.fileResourceLocation = fileResourceLocation;
		}
		
		@Override
		public IFileResourceLocation getFileLocation()
		{
			return fileResourceLocation;
		}

		@Override
		public void setFileLocation(IFileResourceLocation fileResourceLocation)
		{
			this.fileResourceLocation = fileResourceLocation;
		}
		
		@Override
		public String getFileName()
		{
			return getName() + getFileExtension();
		}

		@Override
		public String getFileExtension()
		{
			return getContentType().getFileExtension();
		}

		@Override
		public String getFilePath()
		{
			return Paths.get(getFileLocation().getPath(),getFileName()).toString();
		}
	}

	public IFileResourceLocation getFileLocation();
	public void setFileLocation(IFileResourceLocation fileResourceLocation);

	public String getFileName();
	public String getFileExtension();
	public String getFilePath();
}