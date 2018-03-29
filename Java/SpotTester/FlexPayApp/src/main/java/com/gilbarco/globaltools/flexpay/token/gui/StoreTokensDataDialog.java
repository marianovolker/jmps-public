
package com.gilbarco.globaltools.flexpay.token.gui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.util.EdtDiskUtil;
import com.gilbarco.globaltools.flexpay.maintenance.gui.FlexPayMaintenanceController;
import com.gilbarco.globaltools.flexpay.maintenance.gui.FlexPayMaintenanceView;
import com.gilbarco.globaltools.flexpay.widgets.JFlexPayDialog;

/**
 * @author mgrieco
 * 
 */
public class StoreTokensDataDialog extends JFlexPayDialog
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final long			serialVersionUID	= 1L;
	protected static final int			MAX_PORT_LEN		= 10;
	protected static final String		SHOULDNT_BE_HERE	= "Shouldn't be here !";
	private static final transient Log	logger				= LogFactory.getLog( StoreTokensDataDialog.class );

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

	private JPanel						pnlUsbDrive;
	private JLabel						lblUsbDrive;
	private JComboBox<String>			comboUSBDrives;
	private boolean						bStoreToken;
	private JPanel						pnlReleaseVersions;
	private JLabel						lblReleaseVersion;
	private JComboBox<String>			comboReleaseVersions;
	private JButton						btnSave;
	private JButton						btnClose;
	private boolean						bNeedReleaseBundle;

	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************

	public StoreTokensDataDialog()
	{
		super( );
		setLocation( -223, -44 );
		commonConstructor( );
		bStoreToken = false;
	}

	private void commonConstructor()
	{
		setLayoutSettings( );

		addReleaseVersionsPanel( );
		addUsbDrivePanel( );
		addFlashDrivesCombo( );

		addSaveButton( );
		addCloseButton( );

		formatDialog( );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	@Override
	protected boolean loadDialogData()
	{
		comboUSBDrives.removeAllItems( );
		loadDrivesComboBoxData( );

		comboReleaseVersions.removeAllItems( );
		loadVersionsComboBoxData( );

		return true;
	}

	@Override
	protected boolean clearDialogData()
	{
		return true;
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	public String getUSBPathSelected()
	{
		if (comboUSBDrives.getSelectedItem( ) != null)
		{
			return comboUSBDrives.getSelectedItem( ).toString( );
		}
		return null;
	}

	public String getReleaseVersionSelected()
	{
		if (comboReleaseVersions.getSelectedItem( ) != null)
		{
			return comboReleaseVersions.getSelectedItem( ).toString( );
		}
		return null;
	}

	public boolean dataSelected()
	{
		return this.saveDialogData( );
	}

	// ******************************************************************
	// PROTECTED METHODS
	// ******************************************************************
	protected void closeDialog()
	{
		this.dispose( );
	}

	protected boolean isTokenStored()
	{
		return bStoreToken;
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void setLayoutSettings()
	{
		setDefaultCloseOperation( JDialog.HIDE_ON_CLOSE );
		getContentPane( ).setLayout( null );

		setPreferredSize( new Dimension( 300, 185 ) );
		setTitle( "Select Store Token Options" );
		setResizable( false );
	}

	private void addUsbDrivePanel()
	{
		pnlUsbDrive = new JPanel( );
		pnlUsbDrive.setBorder( new LineBorder( new Color( 0, 0, 0 ) ) );
		pnlUsbDrive.setBounds( 10, 63, 271, 41 );
		pnlUsbDrive.setBorder( null );
		pnlUsbDrive.setLayout( null );

		getContentPane( ).add( pnlUsbDrive );
	}

	private void addFlashDrivesCombo()
	{
		lblUsbDrive = new JLabel( "Flash Drive:" );
		lblUsbDrive.setBounds( 10, 11, 80, 20 );
		pnlUsbDrive.add( lblUsbDrive );

		comboUSBDrives = new JComboBox<String>( );
		comboUSBDrives.setBounds( 119, 11, 130, 20 );
		comboUSBDrives.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				enableSave( );
			}
		} );

		pnlUsbDrive.add( comboUSBDrives );
	}

	private void addReleaseVersionsPanel()
	{
		pnlReleaseVersions = new JPanel( );
		pnlReleaseVersions.setBorder( new LineBorder( new Color( 0, 0, 0 ) ) );
		pnlReleaseVersions.setBounds( 10, 11, 271, 41 );
		pnlReleaseVersions.setBorder( null );
		pnlReleaseVersions.setLayout( null );

		getContentPane( ).add( pnlReleaseVersions );

		lblReleaseVersion = new JLabel( "Release Version:" );
		lblReleaseVersion.setBounds( 10, 10, 100, 20 );
		pnlReleaseVersions.add( lblReleaseVersion );

		comboReleaseVersions = new JComboBox<String>( );
		comboReleaseVersions.setBounds( 112, 10, 149, 20 );
		comboReleaseVersions.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				enableSave( );
			}
		} );

		pnlReleaseVersions.add( comboReleaseVersions );
	}

	private void addSaveButton()
	{
		btnSave = new JButton( "Save" );
		btnSave.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				markDataForSaving( );
				TokenManagerController.setUSBdrive( getUSBPathSelected( ) );

				closeDialog( );
			}
		} );
		btnSave.setBounds( 20, 115, 90, 30 );
		btnSave.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		enableSave( );
		getContentPane( ).add( btnSave );
	}

	private void enableSave()
	{
		boolean enable = ((this.comboReleaseVersions.getSelectedIndex( ) >= 0 && this.comboReleaseVersions.getSelectedItem( ) != null
								&& !this.comboReleaseVersions.getSelectedItem( ).toString( ).isEmpty( )) 
								|| !this.needReleaseBundle( ))
							&& this.comboUSBDrives.getSelectedIndex( ) >= 0 
							&& this.comboUSBDrives.getSelectedItem( ) != null 
							&& !this.comboUSBDrives.getSelectedItem( ).toString( ).isEmpty( );

		btnSave.setEnabled( enable );
	}

	private void addCloseButton()
	{
		btnClose = new JButton( "Cancel" );
		btnClose.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				closeDialog( );
			}
		} );
		btnClose.setBounds( 185, 115, 90, 30 );
		btnClose.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		getContentPane( ).add( btnClose );
	}

	private void loadDrivesComboBoxData()
	{
		String[] astr = EdtDiskUtil.getRemovableDisks( );
		if (comboUSBDrives != null)
		{
			for (int i = 0; i < EdtDiskUtil.getRemovableDisks( ).length; i++)
			{
				comboUSBDrives.addItem( astr[i] );
				if (astr[i] == TokenManagerController.getUSBdrive( ))
				{
					comboUSBDrives.setSelectedIndex( i );
				}
			}
		}
	}

	private void loadVersionsComboBoxData()
	{
		if (!needReleaseBundle( ))
		{
			comboReleaseVersions.setEnabled( false );
			return;
		}
		
		String directory = TokenManagerController.getVersionsFolder( );
		File dir;
		try
		{
			dir = new File( new java.io.File( directory ).getCanonicalPath( ) );
			if (dir.isDirectory( ))
			{
				String[] children = dir.list( );
				if (children != null)
				{
					for (int i = 0, j = 0; i < children.length; i++)
					{
						File entry;
						String filename = dir.getCanonicalPath( ) + "/" + children[i];
						entry = new File( new java.io.File( filename ).getCanonicalPath( ) );
						if (entry.isDirectory( ))
						{
							comboReleaseVersions.insertItemAt( children[i], j );
							j++;
						}
					}
				}
			}
			comboReleaseVersions.setEnabled( true );
		}
		catch (IOException e)
		{
			comboReleaseVersions.setEnabled( false );
			logger.error( "loadVersionsComboBoxData - " + e.getMessage( ) );
			return;
		}
	}

	public boolean needReleaseBundle()
	{
		return bNeedReleaseBundle;
	}

	public void setNeedReleaseBundle(boolean bNeedReleaseBundle)
	{
		this.bNeedReleaseBundle = bNeedReleaseBundle;
	}
}
