/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file SelectUSBOptionsDialog.java
 * @author mgrieco
 * @date 27 Ago 2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */
package com.gilbarco.globaltools.flexpay.maintenance.gui;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.util.Vector;

import javax.swing.AbstractAction;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.KeyStroke;
import javax.swing.SwingConstants;
import javax.swing.filechooser.FileSystemView;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.util.EdtDiskUtil;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerController;

/**
 * The Class SelectUSBOptionsDialog.
 */
public class SelectUSBOptionsDialog extends JDialog
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	/** The Constant serialVersionUID. */
	private static final long	serialVersionUID		= 8755710539831593576L;
	
	/** The Constant logger. */
	private static final transient Log	logger					= LogFactory.getLog( SelectUSBOptionsDialog.class );

	/** The Constant DEFAULT_BUTTON_COLOR. */
	private static final Color	DEFAULT_BUTTON_COLOR	= new Color( 205, 217, 233 );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	/** The button select version. */
	private JButton				btnSelectVersion;
	
	/** The button cancel. */
	private JButton				btnCancel;

	/** The label available versions. */
	private JLabel				lblAvailableVersions;
	
	/** The combo box versions. */
	private JComboBox<String>	comboBoxVersions;

	/** The boolean value for version selected. */
	protected boolean			bVersionSelected;

	/** The label usb drive. */
	private JLabel				lblUsbDrive;
	
	/** The combo for usb drives. */
	private JComboBox<String>	comboUSBDrives;
	
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
	// CONSTRUCTORS.
	// ******************************************************************

	/**
	 * Instantiates a new select usb options dialog.
	 */
	public SelectUSBOptionsDialog()
	{
		super( );
		this.getContentPane( ).setBackground( new Color( 255, 255, 255 ) );
		
		setLocation( -223, -44 );
		commonConstructor( );
	}


	/**
	 * Common constructor.
	 */
	private void commonConstructor()
	{
		setLayoutSettings( );

		createVersionComboBox( );
		addFlashDrivesCombo( );
		addSelectButton( );
		addCancelButton( );
		pack( );
		getContentPane( ).setLayout( null );
	}


	/**
	 * Adds the flash drives combo.
	 */
	private void addFlashDrivesCombo()
	{
		lblUsbDrive = new JLabel( "Flash Drive:" );
		lblUsbDrive.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		lblUsbDrive.setBounds( 38, 66, 80, 20 );
		getContentPane( ).add( lblUsbDrive );

		comboUSBDrives = new JComboBox<String>( );
		comboUSBDrives.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		comboUSBDrives.setBounds( 160, 65, 200, 26 );
		comboUSBDrives.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		comboUSBDrives.setToolTipText( "Select the USB flash drive" );
		comboUSBDrives.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				enableRequest( );
			}
		} );
		getContentPane( ).add( comboUSBDrives );
	}

	/**
	 * Load drives combo box data.
	 */
	public void loadDrivesComboBoxData()
	{
		comboUSBDrives.removeAllItems( );
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

	/**
	 * Gets the removable disks.
	 *
	 * @return the removable disks
	 */
	public static String[] getRemovableDisks()
	{
		try
		{
			FileSystemView fsv = FileSystemView.getFileSystemView( );
			File[] froots = File.listRoots( );
			Vector<String> rootname = new Vector<String>( );
			for (File f : froots)
			{
				if (fsv.getSystemTypeDescription( f ).contains( "Removable Disk" ))
				{
					rootname.add( f.toString( ) );
				}
			}
			String[] srootname = new String[rootname.size( )];
			rootname.toArray( srootname );
			return srootname;
		}
		catch (Exception e)
		{
			return new String[] {};
		}
	}

	/**
	 * Adds the cancel button.
	 */
	private void addCancelButton()
	{
		btnCancel = new JButton( "Cancel" );
		btnCancel.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				closeDialog( );
			}
		} );	
		btnCancel.setBounds( 309, 130, 112, 30 );
		btnCancel.setBorderPainted( false );
		btnCancel.setBackground( DEFAULT_BUTTON_COLOR );
		btnCancel.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnCancel.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		getContentPane( ).add( btnCancel );
		
		this.getRootPane( ).getActionMap( ).put( "ON_EXIT", new AbstractAction( )
		{
			private static final long	serialVersionUID	= 1L;

			@Override
			public void actionPerformed(ActionEvent e)
			{
				closeDialog( );
			}
		} );
		this.getRootPane( ).getInputMap( JComponent.WHEN_IN_FOCUSED_WINDOW ).put( KeyStroke.getKeyStroke( "ESCAPE" ), "ON_EXIT" );

	}

	/**
	 * Adds the select button.
	 */
	private void addSelectButton()
	{
		btnSelectVersion = new JButton( "Select Version" );
		btnSelectVersion.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				bVersionSelected = true;
				closeDialog( );
			}
		} );
		btnSelectVersion.setBounds( 107, 130, 133, 30 );
		btnSelectVersion.setBorderPainted( false );
		btnSelectVersion.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnSelectVersion.setBackground( DEFAULT_BUTTON_COLOR );
		btnSelectVersion.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );

		enableRequest( );
		getContentPane( ).add( btnSelectVersion );
	}

	/**
	 * Sets the layout settings.
	 */
	private void setLayoutSettings()
	{
		setDefaultCloseOperation( JDialog.HIDE_ON_CLOSE );
		getContentPane( ).setLayout( null );

		setPreferredSize( new Dimension( 500, 210 ) );
		setResizable( false );
	}

	/**
	 * Close dialog.
	 */
	protected void closeDialog()
	{
		this.setVisible( false );// dispose( );
	}

	/**
	 * Enable the usb option request.
	 */
	public void enableRequest()
	{
		boolean enable = comboBoxVersions.getSelectedIndex( ) >= 0
							&& comboBoxVersions.getItemCount( ) > 0
							&& comboUSBDrives.getItemCount( ) > 0
							&& comboUSBDrives.getSelectedIndex( ) >= 0
							&& comboUSBDrives.getSelectedItem( ) != null
							&& !comboBoxVersions.getSelectedItem( ).toString( ).isEmpty( )
							&& !comboUSBDrives.getSelectedItem( ).toString( ).isEmpty( );
		btnSelectVersion.setEnabled( enable );
	}

	/**
	 * Version selected.
	 *
	 * @return true, if successful
	 */
	public boolean versionSelected()
	{
		return this.bVersionSelected;
	}

	/**
	 * Clear version selected.
	 */
	public void clearVersionSelected()
	{
		bVersionSelected = false;
		if (comboBoxVersions.getItemCount( ) > 0)
		{
			comboBoxVersions.setSelectedIndex( 0 );
		}
	}

	/**
	 * Gets the select version button.
	 *
	 * @return the select version button
	 */
	public JButton getSelectVersionButton()
	{
		return btnSelectVersion;
	}

	/**
	 * Builds the version list from the base directory.
	 * 
	 * @param directory
	 *            the directory
	 */
	public void buildVersionList(String directory)
	{
		comboBoxVersions.removeAllItems( );
		File dir;
		try
		{
			dir = new File( new java.io.File( directory ).getCanonicalPath( ) );
		}
		catch (IOException e1)
		{
			logger.error( e1.getMessage( ) );
			return;
		}

		if (dir.isDirectory( ))
		{
			String[] children = dir.list( );
			if (children != null)
			{
				for (int i = 0, j = 0; i < children.length; i++)
				{
					File entry;

					try
					{
						String filename = dir.getCanonicalPath( ) + "/" + children[i];
						entry = new File( new java.io.File( filename ).getCanonicalPath( ) );
					}
					catch (IOException e)
					{
						continue;
					}

					if (entry.isDirectory( ))
					{
						comboBoxVersions.insertItemAt( children[i], j );
						j++;
					}
				}
			}
		}
	}


	/**
	 * Gets the combo box versions.
	 *
	 * @return the combo box versions
	 */
	public JComboBox<String> getComboBoxVersions()
	{
		return comboBoxVersions;
	}

	/**
	 * Creates the version combo box.
	 */
	private void createVersionComboBox()
	{
		lblAvailableVersions = new JLabel( "Available Versions:" );
		lblAvailableVersions.setHorizontalAlignment( SwingConstants.LEFT );
		lblAvailableVersions.setBounds( 38, 25, 112, 30 );
		lblAvailableVersions.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );

		getContentPane( ).add( lblAvailableVersions );

		comboBoxVersions = new JComboBox<String>( );
		comboBoxVersions.setBounds( 160, 25, 255, 26 );
		comboBoxVersions.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		comboBoxVersions.setToolTipText( "Select software package to transfer to USB flash drive" );
		comboBoxVersions.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		comboBoxVersions.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				enableRequest();
			}
		} );

		getContentPane( ).add( comboBoxVersions );
	}
	
	/**
	 * Return the version item selected.
	 * 
	 * @return the string
	 */
	public String getVersionItemSelected()
	{
		if (!bVersionSelected)
		{
			return "";
		}
		return this.comboBoxVersions.getSelectedItem( ).toString( );
	}



	/**
	 * Gets the USB drive selected.
	 *
	 * @return the USB drive selected
	 */
	public String getUSBSelected()
	{
		if (!bVersionSelected)
		{
			return "";
		}
		return this.comboUSBDrives.getSelectedItem( ).toString( );	}
}
