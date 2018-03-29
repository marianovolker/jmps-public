/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IAuthenticationBlock.java
 * @author pcantarini
 * @date Tuesday, October 29, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api;

import com.gilbarco.globaltools.flexpay.targetunit.utilities.IObservable;

/** @class IAuthenticationBlock IAuthenticationBlock.java "com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api"
 * @brief This is Authentication wrapper to be implemented by the user.
 *
 * @since 1.0.0
 */
public interface IAuthenticationBlock extends IObservable
{
	/** @brief Perform authentication when commanded.
	 * 
	 * @param uri [In] Uri address to be matched for authentication.
	 * 
	 * @return         AuthenticationResult.AUTHENTICATION_GRANTED if successfully authenticated. AuthenticationResult.AUTHENTICATION_DENIED if authentication failed. AuthenticationResult.AUTHENTICATION_CANCELED if authentication was not even attempted.
	 *
	 * @since 1.0.0
	 */ 
	public AuthenticationResult authenticate (String uri);

	/** @brief Hard reset authentication flag without performing any kind of authentication.
	 * 
	 * @param uri           [In] Uri address to be matched for authentication.
	 * @param authenticated [In] flag value to set for authentication.
	 * 
	 * @since 1.0.0
	 */ 
	public void setAuthenticated (String uri, boolean authenticated);

	/** @brief Check if authentication was commanded at least once and keep track of it.
	 * 
	 * @param uri [In] Uri address to be matched for authentication.
	 * 
	 * @return         True if was authenticated. False otherwise.
	 *
	 * @since 1.0.0
	 */ 
	public boolean isAuthenticated (String uri);

	/** @brief Perform an explicit call to forget authentication.
	 * 
	 * @param uri [In] Uri address to be matched for authentication.
	 *
	 * @since 1.0.0
	 */ 
	public void forgetAuthentication (String uri);

	/** @brief Check if authentication is in progress.
	 * 
	 * @return True if is currently authenticating. False otherwise.
	 *
	 * @since 1.0.0
	 */ 
    public boolean isAuthenticating ();
}