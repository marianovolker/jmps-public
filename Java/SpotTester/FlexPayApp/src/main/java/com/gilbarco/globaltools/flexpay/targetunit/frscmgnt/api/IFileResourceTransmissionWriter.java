/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IFileResourceTransmissionWriter.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api;

public interface IFileResourceTransmissionWriter extends IFileResourceTransmission, IFileResourceWriter
{
	public IFileResourceTransmissionStorage getFileResourceTransmited();
}