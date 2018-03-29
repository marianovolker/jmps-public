
package com.gilbarco.globaltools.flexpay.maintenance.gui;

import org.junit.Assert;
import org.junit.Test;

/**
 * The Class SelectUSBOptionsDialogTest.
 */
public class SelectUSBOptionsDialogTest
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

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
	public void SelectUSBOptionsDialogTestInitialStateTest()
	{
		SelectUSBOptionsDialog dialog = new SelectUSBOptionsDialog( );
		dialog.setLocationRelativeTo( null );
		dialog.setAlwaysOnTop( true );
		dialog.setModal( true );

		dialog.setAlwaysOnTop( true );
		dialog.setModal( true );

		dialog.setTitle( "Select USB Storage Options" );
		dialog.buildVersionList( "" );
		dialog.clearVersionSelected( );
		dialog.loadDrivesComboBoxData( );
		dialog.enableRequest( );

		Assert.assertTrue( dialog.getComboBoxVersions( ).getItemCount( ) > 0 );
		Assert.assertTrue( dialog.versionSelected( ) == false );
		dialog.dispose( );
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
