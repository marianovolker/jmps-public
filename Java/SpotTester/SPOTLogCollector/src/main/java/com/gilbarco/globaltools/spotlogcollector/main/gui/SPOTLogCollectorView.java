
package com.gilbarco.globaltools.spotlogcollector.main.gui;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;
import javax.swing.SwingConstants;
import javax.swing.UIManager;
import javax.swing.WindowConstants;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.edt.util.IpValidator;
import com.gilbarco.globaltools.spotlogcollector.config.SPOTLogCollectorAppConfig;
import com.gilbarco.globaltools.spotlogcollector.model.SPOTLogCollectorContext;
import com.gilbarco.globaltools.spotlogcollector.widgets.JSPOTLogCollectorDialog;
import java.io.File;

public class SPOTLogCollectorView extends EdtAppView<SPOTLogCollectorController>
{
	/** The Constant MAX_DOT_LINE. */
	private static final int					MAX_DOT_LINE			= 20;

	/** The Constant DEFAULT_BUTTON_COLOR. */
	private static final Color					DEFAULT_BUTTON_COLOR	= new Color( 205, 217, 233 );

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final long					serialVersionUID	= 1L;
	private static final transient Log			logger				= LogFactory.getLog( SPOTLogCollectorView.class );

	@SuppressWarnings("unused")
	private static SPOTLogCollectorAppConfig	_appconfig;
	private JFileChooser						fileChooser;

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
	private JLabel								lblGilbarcoImg;
	private JButton								btnExit;

	private JLabel								lblFuelingPositionIp;
	private JTextField							txtFieldIp1;
	private JTextField							txtFieldIp2;
	private JTextField							txtFieldIp3;
	private JTextField							txtFieldIp4;

	private JButton								btnCollectLogs;

	private JLabel								lblDirectory;
	private JTextField							textDirectory;
	private JButton								btnBrowseDirectory;

	private JLabel								lblStatusBar;
	private JTextField							txtStatus;
	private JLabel								lblProgress;
	private JLabel								lblResultImage;
	private ImageIcon							lblSuccessIcon		= null;
	private ImageIcon							lblAlertIcon		= null;
	private ImageIcon							lblErrorIcon		= null;
	private ImageIcon							lblInfoIcon			= null;

	private JOptionPane							errorPane			= null;
	private JDialog								errorDialog			= null;
	private JOptionPane							confirmPane			= null;
	private JDialog								confirmDialog		= null;
	private String								warningDot;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	/**
	 * Constructs the view components
	 */
	public SPOTLogCollectorView()
	{
		super( );

		setBackground( SPOTLogCollectorComponentFormatter.getPanelDefaultBackgroundColor( ) );
		setLayout( null );
		// Gilbarco Image
		addGilbarcoLogo( );
		// Add IP Fields
		addIpFields( );
		// Create the dialog to browse a directory
		createFileChooser( );
		// Add Directory Text Field
		addDirectoryTextField( );
		// Add Browse Directory Button
		addBrowseDirectoryButton( );
		// Add Collection Button
		addCollectLogsBtn( );
		// Add Collection Status Fields
		addStatusField( );
		addProgressField( );
		// Add Collection Icon Image Result
		addResultImage( );
		// Add Exit Button
		addExitButton( );
		// Copyright Label in status bar
		addCopyrightGilbarco( );
		// Set the main panel as the required specification
		setRootAppProperties( );
		// Apply default application setting
		formatView( );

		logger.info( getAppName( ) + " Started." );
	}

	public void formatView()
	{
        UIManager.put( "OptionPane.background", Color.white );
        UIManager.put( "Panel.background", Color.white );
        UIManager.put( "Button.background", DEFAULT_BUTTON_COLOR );
        UIManager.put( "Button.border", BorderFactory.createLineBorder( DEFAULT_BUTTON_COLOR, 7 ) );
        UIManager.put( "Label.font", new Font( "Tahoma", Font.PLAIN, 11 ) );
        UIManager.put( "Button.font", new Font( "Tahoma", Font.PLAIN, 13 ) );
        UIManager.put( "ToolTip.font", new Font( "Tahoma", Font.PLAIN, 11 ) );
        UIManager.put( "ToolTip.border", BorderFactory.createLineBorder( DEFAULT_BUTTON_COLOR ) );
        UIManager.put( "ToolTip.background", Color.white );
        UIManager.put("OptionPane.yesButtonText", "Ok");
        UIManager.put("OptionPane.noButtonText", "Cancel");
        
        UIManager.put("FileChooser.usesSingleFilePane",true);
        UIManager.put("FileChooser.listViewWindowsStyle", true);
        UIManager.put("FileChooser.listFont",new Font( "Tahoma", Font.PLAIN, 11 ));
        UIManager.put("FileChooser.border", BorderFactory.createLineBorder( DEFAULT_BUTTON_COLOR ) );
        UIManager.put("TextField.font",new Font( "Tahoma", Font.PLAIN, 11 ));
        UIManager.put("ComboBox.font", new Font( "Tahoma", Font.PLAIN, 11 ));
        UIManager.put("ComboBox.background", Color.white);
}
	
	/**
	 * Create application configuration
	 */
	private void createSPOTLogCollectorAppConfig()
	{
		if (this.getRootApp( ) != null)
		{
			if (this.getRootApp( ).getAppProperties( ) != null)
			{
				_appconfig = SPOTLogCollectorAppConfig.getInstance( this.getRootApp( ).getAppProperties( ) );
			}
		}
	}

	
	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************
	@Override
	public void initialize()
	{
		// Set the main panel as the required specification
		setRootAppProperties( );
		createSPOTLogCollectorAppConfig( );
		setIpField( );
		setDonwloadDirectoryField( );
		setStatusInfo( );
	}

	private void setStatusInfo()
	{
		if (this.getIpInserted( ).length( ) > 0)
		{
			if (this.getDownloadDirectory( ).length( ) > 0)
			{
				showSuccess( "ready to collect" );
				enableCollect( true );
			}
			else
			{
				showInfo( "enter directory" );
				enableCollect( false );
			}
		}
		else
		{
			showInfo( "enter IP Address" );
			enableCollect( false );
		}
	}

	@Override
	public JMenu getAppMenu()
	{
		return null;
	}

	@Override
	public String getAppName()
	{
		return "SPOTLogCollector v1.0.2";
	}

	@Override
	public Icon getAppIcon()
	{
		return null;
	}

	@Override
	public void addStatusBarComponent(JToolBar toolBar)
	{
		toolBar.add( this.lblStatusBar );
	}

	@Override
	protected SPOTLogCollectorController createController()
	{
		return new SPOTLogCollectorController( this );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void setRootAppProperties()
	{
		if (getRootApp( ) != null)
		{
			// no default main panel size
			getRootApp( ).setMainPanelSize( 750, 494 );
			// no default main panel title
			getRootApp( ).setTitle( getAppName( ) );
			// always on top
			getRootApp( ).setAlwaysOnTop( true );
			// no menu bar
			getRootApp( ).getJMenuBar( ).removeAll( );
			// no tab bar
			getRootApp( ).setAppEmptyTabContainer( );
			// not re-sizable
			getRootApp( ).setResizable( false );
			// Use the enter as the default button key
			UIManager.put( "Button.defaultButtonFollowsFocus", Boolean.TRUE );
			// not right button actions
			getRootApp( ).setAllAppRightClickOff( );
			// add action to the exit button
			getRootApp( ).setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
			getRootApp( ).addMainWindowListener( new WindowAdapter( )
			{
				@Override
				public void windowClosing(WindowEvent e)
				{
					getAppController( ).exitApplication( 0 );							
				}
			} );

		}
	}

	private void addGilbarcoLogo()
	{
		lblGilbarcoImg = new JLabel( new ImageIcon( getClass( ).getResource( "/images/gilbarco_top.jpg" ) ) );
		lblGilbarcoImg.setBounds( 480, 17, 250, 60 );
		add( lblGilbarcoImg );
	}

	private void addDirectoryTextField()
	{
		lblDirectory = new JLabel( "Directory:" );
		lblDirectory.setBounds( 64, 106, 70, 36 );
		lblDirectory.setFont( JSPOTLogCollectorDialog.getDefaultFont( ).deriveFont( 18 ) );
		lblDirectory.setHorizontalAlignment( SwingConstants.LEFT );
		add( lblDirectory );

		textDirectory = new JTextField( );
		textDirectory.setBounds( 138, 110, 409, 30 );
		textDirectory.setHorizontalAlignment( SwingConstants.LEFT );
		textDirectory.setFont( new Font( "Tahoma", Font.PLAIN, 14 ) );
		textDirectory.setBackground( Color.WHITE );
		textDirectory.setEditable( false );
		textDirectory.setFocusable( false );
		add( textDirectory );
	}

	private void addBrowseDirectoryButton()
	{
		btnBrowseDirectory = new JButton( "Browse" );
		btnBrowseDirectory.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				fileChooser.setCurrentDirectory( new File( controller.getModel( ).getLogsDownloadPath( ) ) );
				if (fileChooser.showDialog( SPOTLogCollectorView.this, "Choose" ) == JFileChooser.APPROVE_OPTION)
				{
					File directory = fileChooser.getSelectedFile( );
					SPOTLogCollectorView.this.textDirectory.setText( directory.getAbsolutePath( ) );
					saveDownloadDirectory( directory );
					setStatusInfo( );
					logger.info( "Directory selected : " + directory.getAbsolutePath( ) + "." );
				}
				else
				{
					logger.info( "Browse directory cancelled." );
				}
			}
		} );
		btnBrowseDirectory.setBounds( 567, 108, 90, 32 );		
		btnBrowseDirectory.setBackground( DEFAULT_BUTTON_COLOR );
		btnBrowseDirectory.setFont( new Font( "Tahoma", Font.PLAIN, 14 ) );
		
		btnBrowseDirectory.setBorderPainted( false );
		btnBrowseDirectory.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		
		add( btnBrowseDirectory );
	}

	private void addIpFields()
	{
		lblFuelingPositionIp = new JLabel( "IP Address:" );
		lblFuelingPositionIp.setBounds( 64, 61, 120, 30 );
		lblFuelingPositionIp.setFont( JSPOTLogCollectorDialog.getDefaultFont( ).deriveFont( 18 ) );
		lblFuelingPositionIp.setHorizontalAlignment( SwingConstants.LEFT );
		add( lblFuelingPositionIp );

		txtFieldIp1 = new JTextField( );
		txtFieldIp1.setBounds( 184, 57, 40, 30 );

		txtFieldIp2 = new JTextField( );
		txtFieldIp2.setBounds( 246, 57, 40, 30 );

		txtFieldIp3 = new JTextField( );
		txtFieldIp3.setBounds( 308, 57, 40, 30 );

		txtFieldIp4 = new JTextField( );
		txtFieldIp4.setBounds( 374, 57, 40, 30 );

		JTextField txtFieldIps[] = new JTextField[]{txtFieldIp1, txtFieldIp2, txtFieldIp3, txtFieldIp4};
		if (txtFieldIps != null)
		{
			adjustIPRow(txtFieldIps);
			for (JTextField txtFieldIp : txtFieldIps)
			{
				add( txtFieldIp );
			}
		}
	}

	/**
	 * Adjust ip field.
	 *
	 * @param ipField the ip field
	 * @param listener the listener
	 */
	private void adjustIPField(JTextField ipField, KeyListener listener)
	{
		ipField.setHorizontalAlignment( SwingConstants.CENTER );
		ipField.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		ipField.setFont( new Font( "Tahoma", Font.PLAIN, 14 ) );
		ipField.setAlignmentY( CENTER_ALIGNMENT );
		ipField.setAlignmentX( RIGHT_ALIGNMENT );

		ipField.addKeyListener( listener );
		addIpFocusListener(ipField);
	}


	/**
	 * Adds the ip fields focus listener.
	 * @param ipField the ip field
	 */
	private void addIpFocusListener(JTextField ipField)
	{
		ipField.addFocusListener( new FocusListener( )
		{

			/*
			 * (non-Javadoc)
			 * 
			 * @see java.awt.event.FocusListener#focusGained(java.awt.event.FocusEvent)
			 */
			@Override
			public void focusGained(FocusEvent event)
			{
				if (event.getSource( ) instanceof JTextField)
				{
					if (isTabKeyActivated( event ))
					{
						((JTextField) event.getSource( )).select( 0, ((JTextField) event.getSource( )).getText( ).length( ) );
					}
				}
			}

			private boolean isTabKeyActivated(FocusEvent arg0)
			{
				return arg0.toString( ).contains( "cause=TRAVERSAL_" );
			}

			/*
			 * (non-Javadoc)
			 * 
			 * @see java.awt.event.FocusListener#focusLost(java.awt.event.FocusEvent)
			 */
			@Override
			public void focusLost(FocusEvent arg0)
			{
			}
		} );
	}

	/**
	 * Adjust ip row.
	 * @param ipFields the ip fields
	 */
	private void adjustIPRow(JTextField ipFields[])
	{
		if (ipFields != null)
		{
			if (ipFields.length > 1)
			{
				int iFieldFirst = 0;
				int iFieldLast = ipFields.length - 1;
				
				adjustIPField( ipFields[iFieldFirst], getIpFieldKeyListener(1,255 ) );
				adjustIPField( ipFields[iFieldLast], getIpFieldKeyListener(1,254 ) );
				if (ipFields.length > 2) // Middle fields value have to be between 0 and 255.
				{
					for (int iFieldNbr = iFieldFirst + 1; iFieldNbr < iFieldLast; ++iFieldNbr)
					{						
						adjustIPField( ipFields[iFieldNbr], getIpFieldKeyListener( 0, 255) );
					}
				}
			}
		}
	}

	
	/**
	 * Gets the corresponding ip field key listener.
	 *
	 * @param min the min value for field text
	 * @param max the max value for field text
	 * @return the ip field key listener
	 */
	private KeyAdapter getIpFieldKeyListener(final int min, final int max)
	{
		return new KeyAdapter( ) // First field value has to be between 1 and 255.
			{
				@Override
				public void keyTyped(KeyEvent e)
				{
					char code = e.getKeyChar( );
					String text = ((JTextField) e.getSource( )).getText( );
					String textselected = ((JTextField) e.getSource( )).getSelectedText( );
					if ((code != KeyEvent.VK_BACK_SPACE && code != KeyEvent.VK_CLEAR && code != KeyEvent.VK_DELETE)
						&& (!Character.isDigit( code ) || e.isAltDown( ) || ((text.length( ) == 3) && ((textselected == null) || (textselected.length( ) == 0)))))
					{
						e.consume( );
						return;
					}
				}

				@Override
				public void keyReleased(KeyEvent e)
				{
					if(isKeyCodeAllowed( e ) )
			             return;

					JTextField ipField = (JTextField) e.getSource( );
					String ipFieldText = ipField.getText( );

					if (EdtStringUtil.isStringNumeric( ipFieldText ))
					{
						if (ipFieldText.length( ) > 3)
						{
							ipFieldText = ipFieldText.substring( 0, 3 );
						}
						try
						{
							int ipFieldValue = Integer.parseInt( ipFieldText );
							if ((ipFieldValue >= min) && (ipFieldValue <= max)) //>0 <=255 
							{
								ipFieldText = Integer.toString( ipFieldValue );
							}
							else if (ipFieldValue > max)
							{
								ipFieldText = Integer.toString( ipFieldValue ).substring( 0, 2 );
							}
							else
							{
								ipFieldText = "";
							}
						}
						catch (NumberFormatException ex)
						{
							ipFieldText = "";
						}
					}
					else
					{
						assert false : "Shouldn't be here !";
						ipFieldText = "";
					}
					
					setIpFieldText( ipField, ipFieldText );
					saveCurrentIp( );
					setStatusInfo( );
				}
			};
	}
	
	/**
	 * Checks if the event represents an arrow key code allowed.
	 *
	 * @param e the KeyEvent
	 * @return true, if is the key code allowed
	 */
	private boolean isKeyCodeAllowed(KeyEvent e)
	{
		return !Character.isDigit(e.getKeyChar( )) && e.getKeyCode( )>36 && e.getKeyCode( )<40;
	}

	private void setIpFieldText(JTextField ipField, String ipFieldText)
	{
		if(!ipField.getText( ).equals( ipFieldText))
		{
			ipField.setText( ipFieldText );
		}
	}

	private void addCopyrightGilbarco()
	{
		lblStatusBar = new JLabel(SPOTLogCollectorContext.getLocaleText( "CopyRightGilbarco" ));
		Font fds = SPOTLogCollectorComponentFormatter.getDefaultStatusFont( );
		lblStatusBar.setFont( new Font( fds.getFamily( ), Font.BOLD, fds.getSize( ) ) );
	}

	private void addResultImage()
	{
		lblResultImage = new JLabel( );
		lblResultImage.setBounds( 247, 138, 218, 200 );
		add( lblResultImage );
	}

	private void addStatusField()
	{
		txtStatus = new JTextField( );
		//txtStatus.setBorder( new EtchedBorder( EtchedBorder.LOWERED, null, null ) );
		txtStatus.setEditable( false );
		txtStatus.setEnabled( true );
		txtStatus.setFocusable( false );
		txtStatus.setColumns( 10 );
		txtStatus.setHorizontalAlignment( SwingConstants.CENTER );
		txtStatus.setFont( JSPOTLogCollectorDialog.getDefaultSmallFont( ).deriveFont( Font.BOLD ) );
		txtStatus.setBackground( Color.WHITE );
		txtStatus.setBounds( 185, 324, 297, 30 );
		txtStatus.setBackground( SPOTLogCollectorComponentFormatter.getPanelDefaultBackgroundColor( ) );
		add( txtStatus );
	}

	private void addProgressField()
	{
		lblProgress = new JLabel( );
		lblProgress.setHorizontalAlignment( SwingConstants.LEFT );
		lblProgress.setFont( JSPOTLogCollectorDialog.getDefaultSmallFont( ).deriveFont( Font.BOLD ) );
		lblProgress.setBounds( 310, 340, 100, 30 );
		lblProgress.setText( "" );
		lblProgress.setVisible( false );
		add( lblProgress );
	}

	private void addCollectLogsBtn()
	{
		btnCollectLogs = new JButton( "Collect Logs" );
		btnCollectLogs.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				startCollection( );
			}
		} );
		btnCollectLogs.setBounds( 252, 376, 168, 32 );		
		btnCollectLogs.setBackground( DEFAULT_BUTTON_COLOR );
		btnCollectLogs.setFont( new Font( "Tahoma", Font.PLAIN, 14 ) );
		btnCollectLogs.setBorderPainted( false );
		btnCollectLogs.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		
		add( btnCollectLogs );
	}

	private void addExitButton()
	{
		btnExit = new JButton( "Exit" );
		btnExit.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				getAppController( ).exitApplication( 0 );
			}
		} );
		btnExit.setBounds( 580, 430, 120, 40 );
		btnExit.setBackground( DEFAULT_BUTTON_COLOR );
		btnExit.setFont( new Font( "Tahoma", Font.PLAIN, 14 ) );
		btnExit.setBorderPainted( false );
		btnExit.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );

		add( btnExit );
		
		this.getActionMap( ).put( "ON_EXIT", new AbstractAction( )
		{
			private static final long	serialVersionUID	= 1L;

			@Override
			public void actionPerformed(ActionEvent e)
			{
				getAppController( ).exitApplication( 0 );
			}
		} );
		this.getInputMap( JComponent.WHEN_IN_FOCUSED_WINDOW ).put( KeyStroke.getKeyStroke( "ESCAPE" ), "ON_EXIT" );

	}

	private void startCollection()
	{
		IpValidator ipValidator = new IpValidator( );
		String strIP = getIpInserted( );
		if (ipValidator.validate( strIP ))
		{
			String message = "Collect Logs from " + strIP + " ? \n";
			message += "Logs download may take several minutes";
			int opcion = showConfirmDialog( message );
			if (opcion == JOptionPane.YES_OPTION)
			{
				enableEntry( false );
				showCollectionWarning( );
				controller.startCollection( );
			}
		}
		else
		{
			showErrorMesasge( "The IP address is not valid or the directory is no selected" );
		}
	}

	private void showErrorMesasge(String message)
	{
		if (errorPane == null)
		{
			errorPane = new JOptionPane( message, JOptionPane.ERROR_MESSAGE );
		}
		if (errorDialog == null)
		{
			errorDialog = errorPane.createDialog( "Error" );
			errorDialog.setAlwaysOnTop( true );
		}

		errorDialog.setVisible( true );
	}

	int showConfirmDialog(String message)
	{
		if (confirmPane == null)
		{
			confirmPane = new JOptionPane( message, JOptionPane.QUESTION_MESSAGE, JOptionPane.YES_NO_OPTION, getAppIcon( ) );
		}

		if (confirmPane != null && confirmDialog == null)
		{
			confirmDialog = confirmPane.createDialog( "Confirm" );
			confirmDialog.setAlwaysOnTop( true );
		}

		if (confirmPane != null && confirmDialog != null)
		{
			confirmPane.setMessage( message );
			confirmDialog.pack();
			confirmDialog.setVisible( true );
			Object selectedValue = confirmPane.getValue( );
			if (selectedValue instanceof Integer)
			{
				return ((Integer) selectedValue).intValue( );
			}
		}
		return JOptionPane.NO_OPTION;
	}

	private ImageIcon getSuccessIcon()
	{
		if (lblSuccessIcon == null)
		{
			lblSuccessIcon = new ImageIcon( getClass( ).getResource( "/images/GreenCheckOK.jpg" ) );
		}
		return lblSuccessIcon;
	}

	private ImageIcon getAlertIcon()
	{
		if (lblAlertIcon == null)
		{
			lblAlertIcon = new ImageIcon( getClass( ).getResource( "/images/YellowInfo.jpg" ) );
		}
		return lblAlertIcon;
	}

	private ImageIcon getErrorIcon()
	{
		if (lblErrorIcon == null)
		{
			lblErrorIcon = new ImageIcon( getClass( ).getResource( "/images/RedXNO.jpg" ) );
		}
		return lblErrorIcon;
	}

	private ImageIcon getInfoIcon()
	{
		if (lblInfoIcon == null)
		{
			lblInfoIcon = new ImageIcon( getClass( ).getResource( "/images/BlueInfo.jpg" ) );
		}
		return lblInfoIcon;
	}

	private void enableEntry(boolean enable)
	{
		this.btnBrowseDirectory.setEnabled( enable );
		this.txtFieldIp1.setEnabled( enable );
		this.txtFieldIp2.setEnabled( enable );
		this.txtFieldIp3.setEnabled( enable );
		this.txtFieldIp4.setEnabled( enable );
	}

	private void enableCollect(boolean enable)
	{
		this.btnCollectLogs.setEnabled( enable );
	}

	private void createFileChooser()
	{
		fileChooser = new JFileChooser( );
		fileChooser.setFileSelectionMode( JFileChooser.DIRECTORIES_ONLY );
		fileChooser.setDialogTitle( "Choose Directory" );
		fileChooser.setCurrentDirectory( new File( controller.getModel( ).getLogsDownloadPath( ) ) );
	}

	/**
	 * set the directory text field with the application config value
	 */
	private void setDonwloadDirectoryField()
	{
		textDirectory.setText( controller.getModel( ).getLogsDownloadPath( ) );
	}

	/**
	 * save the directory text field in the application config value
	 */
	private void saveDownloadDirectory(File directory)
	{
		controller.getModel( ).setLogsDownloadPath( directory.getAbsolutePath( ) );
	}

	public String getDownloadDirectory()
	{
		return textDirectory.getText( );
	}

	public String getIpInserted()
	{
		return IpValidator.joinIP(EdtStringUtil.trimLeadingZeros(txtFieldIp1.getText()),
				EdtStringUtil.trimLeadingZeros(txtFieldIp2.getText()),
				EdtStringUtil.trimLeadingZeros(txtFieldIp3.getText()),
				EdtStringUtil.trimLeadingZeros(txtFieldIp4.getText()));
	}

	/**
	 * save the ip text field in the application config value
	 */
	private void saveCurrentIp()
	{
		controller.getModel( ).setCurrentIp( this.getIpInserted() );
	}

	/**
	 * set the ip text fields with the application config value
	 */
	private void setIpField()
	{
		String[] strIp = IpValidator.splitIP( controller.getModel( ).getCurrentIp( ) );
		if (strIp.length == 4)
		{
			txtFieldIp1.setText( EdtStringUtil.trimLeadingZeros( strIp[0] ) );
			txtFieldIp2.setText( EdtStringUtil.trimLeadingZeros( strIp[1] ) );
			txtFieldIp3.setText( EdtStringUtil.trimLeadingZeros( strIp[2] ) );
			txtFieldIp4.setText( EdtStringUtil.trimLeadingZeros( strIp[3] ) );
		}
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void showSuccess(String status)
	{
		lblResultImage.setIcon( getSuccessIcon( ) );
		lblResultImage.setVisible( true );
		txtStatus.setText( status );
		lblProgress.setVisible( false );
		enableEntry( true );
		enableCollect( true );
	}

	protected void showError(String status)
	{
		lblResultImage.setIcon( getErrorIcon( ) );
		lblResultImage.setVisible( true );
		txtStatus.setText( status );
		lblProgress.setVisible( false );
		enableEntry( true );
		enableCollect( true );
		this.repaint( );
		this.revalidate( );
	}

	protected void showCollectionWarning()
	{
		warningDot = "...";
		showWarning( "Collecting..." );
		lblProgress.setVisible( true );
		showProgress();		
	}

	protected void showWarning(String status)
	{
		lblResultImage.setIcon( getAlertIcon( ) );
		lblResultImage.setVisible( true );
		txtStatus.setText( status );
		enableEntry( false );
		enableCollect( false );
		this.repaint( );
		this.revalidate( );		
	}

	public void showProgress()
	{
		lblProgress.setText( this.getNextDot( ) );
		this.repaint( );
		this.revalidate( );		
	}

	private String getNextDot()
	{
		if (warningDot.length( ) == MAX_DOT_LINE)
			warningDot = "...";
		else
			warningDot += ".";
		return warningDot;
	}

	protected void showInfo(String status)
	{
		lblResultImage.setIcon( getInfoIcon( ) );
		lblResultImage.setVisible( true );
		enableEntry( true );
		txtStatus.setText( status );
		this.repaint( );
		this.revalidate( );		
	}

	public JButton getCollectLogsButton()
	{
		return this.btnCollectLogs;
	}

	// ******************************************************************
	// MAIN.
	// ******************************************************************
}
