/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file SelectDirectoryDialog.java
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
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.io.File;
import java.nio.file.Path;
import java.nio.file.Paths;

import javax.swing.AbstractAction;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.SwingConstants;

/**
 * The Class SelectDirectoryDialog.
 */
public class SelectDirectoryDialog extends JDialog
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	/** The Constant serialVersionUID. */
	private static final long	serialVersionUID		= 8755710539831593576L;
	
	/** The Constant DEFAULT_BUTTON_COLOR. */
	private static final Color	DEFAULT_BUTTON_COLOR	= new Color( 205, 217, 233 );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	/** The button choose directory. */
	private JButton				btnChooseDirectory;
	
	/** The button cancel. */
	private JButton				btnCancel;

	/** The directory selected boolean value. */
	protected boolean			bDirectorySelected;

	/** The file chooser. */
	private JFileChooser		fileChooser;
	
	/** The selected file. */
	private File				selectedFile;

	/** The label directory. */
	private JLabel				lblDirectory;
	
	/** The directory text field. */
	private JTextField			txtDirectory;
	
	/** The button browse. */
	private JButton				btnBrowse;

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
	 * Instantiates a new select directory dialog.
	 */
	public SelectDirectoryDialog()
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

		addDirectoryText( );
		addBrowseButton( );
		addSelectButton( );
		addCancelButton( );
		pack( );
		getContentPane( ).setLayout( null );
	}

	/**
	 * Sets the layout settings.
	 */
	private void setLayoutSettings()
	{
		setDefaultCloseOperation( JDialog.HIDE_ON_CLOSE );
		getContentPane( ).setLayout( null );

		setPreferredSize( new Dimension( 600, 180 ) );
		setResizable( false );
	}

	/**
	 * Adds the directory text field.
	 */
	private void addDirectoryText()
	{
		lblDirectory = new JLabel( "Directory:" );
		lblDirectory.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		lblDirectory.setHorizontalAlignment( SwingConstants.LEFT );
		lblDirectory.setBounds( 39, 32, 62, 36 );
		getContentPane( ).add( lblDirectory );

		txtDirectory = new JTextField( "" );
		txtDirectory.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		txtDirectory.setHorizontalAlignment( SwingConstants.LEFT );
		txtDirectory.setEditable( true );
		txtDirectory.setBounds( 100, 36, 365, 28 );
		txtDirectory.setBackground( Color.WHITE );
		txtDirectory.setToolTipText("Current selected directory");
		txtDirectory.addKeyListener(new KeyAdapter()
		{
			@Override
			public void keyReleased (KeyEvent e)
			{
				String text = ((JTextField)e.getSource()).getText();
				if ((text.trim().isEmpty()) && (btnChooseDirectory != null))
				{
					btnChooseDirectory.setEnabled(false);
				}
				else if (btnChooseDirectory != null)
				{
					btnChooseDirectory.setEnabled(true);
				}
			}
		});
		getContentPane().add(txtDirectory);
	}

	/**
	 * Adds the browse button.
	 */
	private void addBrowseButton()
	{
		btnBrowse = new JButton( "Browse" );
		btnBrowse.setFont( new Font( "Tahoma", Font.PLAIN, 12 ) );
		btnBrowse.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent arg0)
			{
				chooseDirectory( );
			}
		} );
		btnBrowse.setBounds( 475, 35, 76, 30 );
		btnBrowse.setBorderPainted( false );
		btnBrowse.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnBrowse.setBackground( DEFAULT_BUTTON_COLOR );
		btnBrowse.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		getContentPane( ).add( btnBrowse );

	}

	/**
	 * Adds the select button.
	 */
	private void addSelectButton()
	{
		btnChooseDirectory = new JButton( "Choose Directory" );
		btnChooseDirectory.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				bDirectorySelected = true;
				selectedFile = new File(txtDirectory.getText());
				closeDialog( );
			}
		} );
		btnChooseDirectory.setBounds( 280, 96, 133, 30 );
		btnChooseDirectory.setBorderPainted( false );
		btnChooseDirectory.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnChooseDirectory.setBackground( DEFAULT_BUTTON_COLOR );
		btnChooseDirectory.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );

		enableRequest( );
		getContentPane( ).add( btnChooseDirectory );
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
		btnCancel.setBounds( 446, 96, 112, 30 );
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
	 * Choose directory.
	 *
	 * @return true, if a directory was choose
	 */
	protected boolean chooseDirectory()
	{
		if (fileChooser == null)
		{
			fileChooser = new JFileChooser( );
			fileChooser.setPreferredSize(new Dimension(600, 400));
			fileChooser.setFileSelectionMode( JFileChooser.DIRECTORIES_ONLY );
			fileChooser.setDialogTitle( "Choose Logs Destination:" );
		}

		File selectedFileTmp = new File(txtDirectory.getText());
		
		while((selectedFileTmp != null) && ((!selectedFileTmp.exists())|| (!selectedFileTmp.isDirectory())))
		{
			selectedFileTmp = selectedFileTmp.getParentFile();
		}

		fileChooser.setCurrentDirectory( selectedFileTmp );
		if (fileChooser.showDialog( this, "Choose" ) == JFileChooser.APPROVE_OPTION)
		{
			selectedFile = fileChooser.getSelectedFile();
			txtDirectory.setText(selectedFile.getAbsolutePath());
			return true;
		}
		else
		{
			return false;
		}
	}
	
	/**
	 * Close dialog.
	 */
	protected void closeDialog()
	{
		this.setVisible( false );// dispose( );
	}

	/**
	 * Enable request.
	 */
	public void enableRequest()
	{
		btnChooseDirectory.setEnabled( this.txtDirectory.getText( ).length( ) > 3 );
	}

	/**
	 * Directory selected.
	 *
	 * @return true, if successful
	 */
	public boolean directorySelected()
	{
		return bDirectorySelected;
	}

	/**
	 * Clear directory selected.
	 */
	public void clearDirectorySelected()
	{
		bDirectorySelected = false;
		selectedFile = null;
	}

	/**
	 * Sets the default directory path.
	 *
	 * @param path the new default directory
	 */
	public void setDefaultDirectory(String path)
	{
		selectedFile = new File(path);
		txtDirectory.setText(selectedFile.getAbsolutePath());
	}

	/**
	 * @return the selected directory
	 */
	public Path getSelectedDirectory()
	{
		Path selectedPath = null;
		if (selectedFile != null)
		{
			selectedPath = Paths.get(selectedFile.getPath());			
		}
		else
		{
			selectedPath = Paths.get("");
		}
		return selectedPath;
	}
}
