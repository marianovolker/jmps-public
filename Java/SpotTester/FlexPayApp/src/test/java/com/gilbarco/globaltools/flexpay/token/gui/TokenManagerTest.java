/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.token.gui;

import java.awt.Dialog;

import javax.swing.JButton;

import org.junit.Assert;
import org.junit.Test;

import com.gilbarco.globaltools.flexpay.token.controller.UnitsPerTokenController;
import com.gilbarco.globaltools.flexpay.token.dto.DataTransferForTokenUnitTableRow;

public class TokenManagerTest
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	static boolean compareStringArrays(String[] as1, String[] as2)
	{
		if (as1 == null && as2 == null)
			return true;
		else if (as1 == null || as2 == null) return false;

		if (as1.length != as2.length) return false;

		for (int i = 0; i < as1.length; i++)
			if (!as1[i].equals( as2[i] )) return false;

		return true;
	}

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	@Test
	public void AdvancedOptionsDialogDataTest()
	{
		try
		{
			DataTransferForTokenUnitTableRow dttu = new DataTransferForTokenUnitTableRow( );

			dttu.setTokenCode( "12345678901234567890" );
			dttu.setTargetUnit( "1621241" );
			dttu.setStartTime( "2/29/2012" );

			UnitsPerTokenController.addUnitAndToken( dttu, null );
			AdvancedOptionsDialog dlg = new AdvancedOptionsDialog( );

			// Setting modeless only for unit test. Just to be able to kill the dialog.
			dlg.setModalityType( Dialog.ModalityType.MODELESS );
			dlg.setVisible( true );

			synchronized (dlg)
			{
				while (!dlg.isDataLoaded( ))
					// waits for the data in dialog
					dlg.wait( ); // to finish loading.
			}

			Assert.assertTrue( TokenManagerModel.getTokenServerIp1( ).equals( dlg.getServerIP1String( ) ) );
			Assert.assertTrue( TokenManagerModel.getTokenServerIp2( ).equals( dlg.getServerIP2String( ) ) );
			Assert.assertTrue( Integer.toString( TokenManagerModel.getTokenServerPort( ) ).equals( dlg.getServerPortString( ) ) );
			Assert.assertTrue( Integer.toString( TokenManagerModel.getTerminalPort( ) ).equals( dlg.getTerminalPortString( ) ) );

			dlg.dispose( );
		}
		catch (Exception e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void requestTokenDialogTest()
	{
		try
		{
			DataTransferForTokenUnitTableRow dttu = new DataTransferForTokenUnitTableRow( );
			TokenRequestDialog dlg = new TokenRequestDialog( );
			String code = dlg.getTokenCode( );
			JButton button = dlg.getRequestButton( );
			Assert.assertFalse( button.isEnabled( ) );
			dlg.dispose( );

			dttu.setTokenCode( code );
			dttu.setTargetUnit( "1621241" );
			dttu.setStartTime( "2/29/2012" );
			UnitsPerTokenController.addUnitAndToken( dttu, null );

			dlg = new TokenRequestDialog( );
			button = dlg.getRequestButton( );
			Assert.assertFalse( button.isEnabled( ) );

			dlg.dispose( );
		}
		catch (Exception e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void TokenSingletonControllerTest()
	{
		TokenManagerView view = new TokenManagerView( );
		TokenManagerController controller = TokenManagerController.getInstance( view );
		Assert.assertTrue( view.getAppController( ).equals( controller ) );

	}

	// test the dual authentication properties for flexpayapp
	@Test
	public void TokenControllerTLSSocketServiceTest()
	{
		TokenManagerView view = new TokenManagerView( );
		TokenManagerController controller = TokenManagerController.getInstance( view );
		
		/*
		Assert.assertFalse( controller.getTLSSocketTransportLayer( ).getTLSSocketServiceDefinition( ).getClientAuthentication( ) );
		Assert.assertFalse( controller.getTLSSecondarySocketTransportLayer( ).getTLSSocketServiceDefinition( ).getClientAuthentication( ) );*/
	}
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************
}
