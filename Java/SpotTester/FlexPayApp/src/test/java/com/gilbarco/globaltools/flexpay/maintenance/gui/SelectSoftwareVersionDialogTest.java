
package com.gilbarco.globaltools.flexpay.maintenance.gui;

import org.junit.Assert;
import org.junit.Test;

/**
 * The Class SelectSoftwareVersionDialogTest.
 */
public class SelectSoftwareVersionDialogTest
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

	/**
	 * Select software version dialog initial state test.
	 */
	@Test
	public void SelectSoftwareVersionDialogInitialStateTest()
	{
		String vers = "folder";
		SelectSoftwareVersionDialog dialog = new SelectSoftwareVersionDialog( vers );
		dialog.setLocationRelativeTo( null );
		dialog.setAlwaysOnTop( true );
		dialog.setModal( true );

		dialog.setTitle( "Select Software Version - compare" );
		dialog.setVersionsToolTip( "compare version" );
		dialog.buildVersionList( vers );
		dialog.clearVersionSelected( );
		dialog.enableRequest( );

		Assert.assertTrue( dialog.getComboBoxVersions( ).getItemCount( ) == 0 );
		Assert.assertTrue( dialog.versionSelected( ) == false);
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
