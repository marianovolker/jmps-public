/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file SelectSoftwareVersionDialog.java
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

import javax.swing.AbstractAction;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.KeyStroke;
import javax.swing.SwingConstants;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * The Class SelectSoftwareVersionDialog.
 */
public class SelectSoftwareVersionDialog extends JDialog
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	/** The Constant serialVersionUID. */
	private static final long			serialVersionUID		= 8755710539831593576L;

	/** The Constant logger. */
	private static final transient Log	logger					= LogFactory.getLog( SelectSoftwareVersionDialog.class );

	/** The Constant DEFAULT_BUTTON_COLOR. */
	private static final Color			DEFAULT_BUTTON_COLOR	= new Color( 205, 217, 233 );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	/** The button select version. */
	private JButton						btnSelectVersion;

	/** The button cancel. */
	private JButton						btnCancel;

	/** The label for available versions. */
	private JLabel						lblAvailableVersions;

	/** The combo box for versions to select. */
	private JComboBox<String>			comboBoxVersions;

	/** The boolean value of version selected. */
	protected boolean					bVersionSelected;

	/** The versions folder. */
	private String						versionsFolder;

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
	 * Instantiates a new select software version dialog.
	 * 
	 * @param folder
	 *            the default folder name
	 */
	public SelectSoftwareVersionDialog(String folder)
	{
		super( );

		this.versionsFolder = folder;
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
		buildVersionList( getVersionsFolder( ) );
		addSelectButton( );
		addCancelButton( );

		pack( );
		getContentPane( ).setLayout( null );
	}

	/**
	 * Gets the versions folder.
	 * 
	 * @return the versions folder
	 */
	private String getVersionsFolder()
	{
		return versionsFolder;
	}

	/**
	 * Adds the cancel button to quit the dialog.
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
		btnCancel.setBounds( 338, 109, 112, 30 );
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
	 * Adds the select button to choose the selected version.
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
		btnSelectVersion.setBounds( 172, 109, 133, 30 );
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

		setPreferredSize( new Dimension( 480, 190 ) );
		setResizable( false );
	}

	/**
	 * Close dialog.
	 */
	protected void closeDialog()
	{
		this.setVisible( false );
	}

	/**
	 * Enable request to select a version.
	 */
	public void enableRequest()
	{
		boolean enable = this.comboBoxVersions.getSelectedIndex( ) >= 0;
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
	 * Sets Versions Tool Tip.
	 * 
	 * @param tooltip
	 *            string with the tool tip leyend for available versions
	 */
	public void setVersionsToolTip(String tooltip)
	{
		comboBoxVersions.setToolTipText( tooltip );
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
			comboBoxVersions.setEnabled( false );
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
		lblAvailableVersions = new JLabel( "Available Versions" );
		lblAvailableVersions.setHorizontalAlignment( SwingConstants.CENTER );
		lblAvailableVersions.setBounds( 26, 30, 130, 30 );
		lblAvailableVersions.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );

		getContentPane( ).add( lblAvailableVersions );

		comboBoxVersions = new JComboBox<String>( );
		comboBoxVersions.setBounds( 170, 30, 250, 25 );
		comboBoxVersions.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		comboBoxVersions.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		comboBoxVersions.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				enableRequest( );
			}
		} );
		getContentPane( ).add( comboBoxVersions );
	}

	/**
	 * Version item selected.
	 * 
	 * @return the string
	 */
	public String versionItemSelected()
	{
		if (!bVersionSelected)
		{
			return "";
		}
		return this.comboBoxVersions.getSelectedItem( ).toString( );
	}
}
