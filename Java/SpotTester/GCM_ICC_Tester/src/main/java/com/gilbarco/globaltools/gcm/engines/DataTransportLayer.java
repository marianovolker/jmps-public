package com.gilbarco.globaltools.gcm.engines;


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
public interface DataTransportLayer {
	public void send(byte[] data);
}
