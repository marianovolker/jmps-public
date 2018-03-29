
package com.gilbarco.globaltools.flexpay.token.state;

/**
 * @author stomasini
 * 
 * @Description Possible states for SpotTester application.
 * 
 * @see GCMContext
 * 
 * @version 1.0.0
 */

public enum TokenAppState
{
	IDLE_STATE,
	CERTIFICATE_GENERATED_STATE,
	PUBLISH_CERTIFICATE_REQUEST_STATE,
	PUBLISH_CERTIFICATE_RESPONSE_STATE,	
	TOKEN_CODE_REQUEST_STATE, // application state to send message RequestTokenCode
	TOKEN_CODE_RESPONSE_STATE, // wait the response
	TOKEN_CODE_ASSIGNED_STATE, // successful response, token code assigned
	TOKEN_REQUEST_STATE, //send message
	TOKEN_RESPONSE_STATE, //wait response
	TOKEN_AUTHORIZED_STATE,
	TOKEN_PRESENT_STATE, //send message to terminal unit
	TOKEN_PRESENT_RESPONSE_STATE,// wait response from terminal
	TOKEN_QUERY_REQUEST_STATE,
	TOKEN_QUERY_RESPONSE_STATE,
	ERROR_STATE,	
	TOKEN_STORED_STATE,
	UNKNOW_STATE;
}
