/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IFileResource.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api;

public interface IFileResource
{
	public class FileResourceImp implements IFileResource
	{
		private String                   name        = null;
		private byte                     fileType    = -1;
		private byte                     fileId      = -1;
		private IFileResourceContentType contentType = null;

		public FileResourceImp(String name, int fileType, int fileId, IFileResourceContentType fileResourceContentType)
		{
			this.name        = name;
			this.fileType    = (byte) fileType;
			this.fileId      = (byte) fileId;
			this.contentType = fileResourceContentType;
		}

		@Override
		public byte getFileType()
		{
			return fileType;

		}

		@Override
		public byte getFileId()
		{
			return fileId;
		}

		@Override
		public String getName()
		{
			return name;
		}

		@Override
		public IFileResourceContentType getContentType()
		{
			return contentType;
		}
	}

	public byte getFileType();

	public byte getFileId();

	public String getName();

	public IFileResourceContentType getContentType();
}