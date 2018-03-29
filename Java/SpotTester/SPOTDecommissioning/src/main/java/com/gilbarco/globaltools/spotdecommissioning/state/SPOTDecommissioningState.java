
package com.gilbarco.globaltools.spotdecommissioning.state;

/**
 * @author mgrieco
 * 
 * @Description Possible states.
 * 
 * @version 1.0.0
 */

public enum SPOTDecommissioningState
{
	IDLE,
	CONNECT_AND_LOGIN,
	GET_LOGIN_RESPONSE,
	SEND_PPN_INFO_REQUEST,
	GET_PPN_INFO_RESPONSE,
	SETUP_PROCESS,
	SEND_KEY_STATUS_TABLE_REQUEST,
	GET_KEY_STATUS_TABLE_RESPONSE,
	GET_KEY_TO_CLEAR,
	SEND_CLEAR_KEY_REQUEST,
	GET_CLEAR_KEY_RESPONSE,
	SHOW_ERROR,
	SHOW_SUCCESS,
	CLEANUP_PROCESS,
	DISCONNECT,
	SPOT_ERROR;
}
