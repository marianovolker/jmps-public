/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AuthenticationResult.java
 * @author pcantarini
 * @date Tuesday, October 29, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api;

/** @class AuthenticationResult AuthenticationResult.java "com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api"
 * @brief This is the Authentication process result status.
 *
 * @since 1.0.0
 */
public enum AuthenticationResult
{
	/** @brief Authentication was successful */
	AUTHENTICATION_GRANTED,
	/** @brief Authentication failed */
	AUTHENTICATION_DENIED,
	/** @brief Authentication canceled */
	AUTHENTICATION_CANCELED,
	/** @brief Authentication interrupted by a lost connection */
	AUTHENTICATION_INTERRUPTED;
}