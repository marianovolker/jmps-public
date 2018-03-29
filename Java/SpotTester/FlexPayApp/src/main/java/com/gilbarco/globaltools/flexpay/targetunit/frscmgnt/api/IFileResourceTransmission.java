/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IFileResourceTransmission.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api;

public interface IFileResourceTransmission extends IFileResource
{
	public enum FileResourceTransmissionStatus
	{
		NOT_AVAILABLE,
		IDLE,
		IN_PROGRESS,
		DONE,
		ERROR;
	}
	
	public abstract class AFileResourceTransmission implements IFileResourceTransmission
	{
		private IFileResourceTransmission.FileResourceTransmissionStatus status;

		public AFileResourceTransmission ()
		{
			this.status = FileResourceTransmissionStatus.IDLE;
		}
		
		@Override
		public IFileResourceTransmission.FileResourceTransmissionStatus getTransferStatus()
		{
			return status;
		}

		@Override
		public void setTransferStatus(IFileResourceTransmission.FileResourceTransmissionStatus status)
		{
			this.status = status;
		}
	}

	public IFileResourceTransmission.FileResourceTransmissionStatus getTransferStatus();

	public void setTransferStatus(IFileResourceTransmission.FileResourceTransmissionStatus status);
}