
package com.gilbarco.globaltools.spotlogcollector.state;

/**
 * @author mgrieco
 * 
 * @Description Possible states.
 * 
 * @version 1.0.0
 */

public enum SPOTLogCollectorState
{
	IDLE,
	WAIT_DATA,
	CONNECT_AND_LOGIN,
	SETUP_PROCESS,
	GET_FILE_TO_REQUEST,
	SEND_FILE_UPLOAD_REQUEST,
	GET_FILE_UPLOAD_RESPONSE,
	SEND_FILE_BLOCK_UPLOAD_REQUEST,
	GET_FILE_BLOCK_UPLOAD_RESPONSE,
	SHOW_ERROR,
	SHOW_SUCCESS,
	CLEANUP_PROCESS,
	DISCONNECT,
	SPOT_ERROR;
}
