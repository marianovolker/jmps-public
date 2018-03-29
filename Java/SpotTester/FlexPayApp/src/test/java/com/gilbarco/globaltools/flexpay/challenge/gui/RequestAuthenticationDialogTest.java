/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file RequestAuthenticationDialog.java
 * @author mgrieco
 * @date Tuesday, August 27, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.challenge.gui;

import org.junit.Assert;
import org.junit.Test;

import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.challenge.gui.RequestAuthenticationDialog;

/** @class RequestAuthenticationDialogTest RequestAuthenticationDialogTest.java "com.gilbarco.globaltools.flexpay.challenge.gui"
 * @brief This class provides a unit test for the dialog to request a password for a challenge authentication.
 *
 * @since 1.0.0
 */
public class RequestAuthenticationDialogTest
{
	/** @brief RequestAuthenticationDialog Test for initial state.
	 *
	 * @since 1.0.0
	 */
	@Test
	public void RequestAuthenticationDialogInitialStateTest ()
	{
		RequestAuthenticationDialog dialog = new RequestAuthenticationDialog();

		Assert.assertTrue(dialog.getPassword().length() == 0);
		Assert.assertFalse(dialog.isReadyForAuthentication());
		dialog.dispose();
	}
}
