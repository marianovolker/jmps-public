package com.gilbarco.globaltools.gcm.state;

/**
 * @author jrubstein
 * 
 * @Description Possible states for SpotTester application.
 * 
 * @see GCMContext
 * 
 * @version 1.0.0
 */

public enum GCMState {
	IDLE,
	INIT,
	ENABLE_READER,
	WAIT_DATA,
	LOG_RESULT, 
	;
}
