/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.engines;

import com.gilbarco.globaltools.edt.services.EdtServiceException;

/**
 * @interface DataTransportLayer
 * 
 * @description
 * 
 * 
 * @version 1.0.0
 * 
 * @author ajadzinsky Feb 23, 2011
 */
public interface DataTransportLayer
{
	public void send(byte[] data) throws EdtServiceException ;
	
	public void send(byte[] data, long timeout) throws EdtServiceException ;
}
