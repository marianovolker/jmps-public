/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IFileResourceStorageMemory.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api;

public interface IFileResourceStorageMemory extends IFileResourceStorage
{
	public abstract class AFileResourceStorageMemory implements IFileResourceStorageMemory
	{
		byte []       data;
		
		public AFileResourceStorageMemory ()
		{
			this.data = null;
		}

		@Override
		public byte [] getData()
		{
			return data;
		}

		@Override
		public void setData(byte [] data)
		{
			this.data = data;
		}
	}

	public byte [] getData();
	public void setData(byte [] data);
}