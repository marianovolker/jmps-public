/**
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file FlexPayMaintenanceView.java
 * @author mgrieco
 * @date 27 Ago 2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

package com.gilbarco.globaltools.flexpay.maintenance.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;
import javax.swing.SwingConstants;
import javax.swing.Timer;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.edt.util.IpValidator;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.strategies.IAuthenticationStrategy;

/**
 * The Class FlexPayMaintenanceView.
 */
public class FlexPayMaintenanceView extends EdtAppView<FlexPayMaintenanceController> 
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	/** The Constant serialVersionUID. */
	private static final long	serialVersionUID				= 1L;

	/** The Constant DEFAULT_BUTTON_COLOR. */
	private static final Color	DEFAULT_BUTTON_COLOR			= new Color( 205, 217, 233 );

	/** The Constant DEFAULT_EDITABLE_COLOR. */
	private static final Color	DEFAULT_EDITABLE_COLOR			= new Color( 213, 223, 234 );

	/** The Constant UNSECURE_GREEN_BUTTON_COLOR. */
	private static final Color	UNSECURE_GREEN_BUTTON_COLOR		= new Color( 107, 159, 0 );

	/** The Constant SECURE_ORANGE_BUTTON_COLOR. */
	private static final Color	SECURE_ORANGE_BUTTON_COLOR		= new Color( 255, 100, 0 );
	
	/** The Constant SESSION_TIME_SECONDS. */
	private static final int SESSION_TIME_SECONDS = 900;

	/** The Constant SESSION_TIME_ALERT_SECONDS when the clock font color should be change to inform red state. */
	private static final int SESSION_TIME_ALERT_SECONDS = 60;

	/** The Constant SESSION_TIME_STRING_STOPPED. */
	private static final String SESSION_TIME_STRING_STOPPED = "--:--";

	/** The panel main. */
	private JPanel				pnlMain;

	/** The panel main ip. */
	private JPanel				pnlMainIp;

	/** The button unit ip. */
	private JLabel				lblUnitIP;

	/** The text ip field 1. */
	private JTextField			txtFieldIp1;

	/** The text ip field 2. */
	private JTextField			txtFieldIp2;

	/** The text ip field 3. */
	private JTextField			txtFieldIp3;

	/** The text ip field 4. */
	private JTextField			txtFieldIp4;

	/** The button gilbarco img. */
	private JLabel				lblGilbarcoImg;

	/** The panel main actions. */
	private JPanel				pnlMainActions;

	/** The button install. */
	private JButton				btnInstall;

	/** The button compare. */
	private JButton				btnCompare;

	/** The panel main image result. */
	private JPanel				pnlMainImageResult;

	/** The button result leyend. */
	private JLabel				lblResultLeyend;

	/** The button result img. */
	private JLabel				lblResultImg;

	/** The progress bar. */
	private JProgressBar		progressBar;

	/** The label success icon. */
	private ImageIcon			lblSuccessIcon					= null;

	/** The label alert icon. */
	private ImageIcon			lblAlertIcon					= null;

	/** The button error icon. */
	private ImageIcon			lblErrorIcon					= null;

	/** The panel buttons. */
	private JPanel				pnlButtons;

	/** The button view log. */
	private JButton				btnViewLog;
	
	/** The button view log. */
	private JButton				btnViewLogsFolder;

	/** The button close. */
	private JButton				btnClose;

	/** The button standard logs. */
	private JButton				btnStandardLogs;

	/** The button pci logs. */
	private JButton				btnPCILogs;

	/** The button usb storage. */
	private JButton				btnUSBStorage;

	/** The button secure menu. */
	private JButton				btnSecureMenu;

	/** The label status leyend. */
	private JLabel				lblStatusLeyend;
	
	/** The status bar panel. */
	private JPanel				pnlStatusBar;
	
	/** The status bar session time count down clock. */
	private JLabel				lblStatusBarCountDownClock;

	/** The session time count down value. */
	private int				    iTimeCountDownClock;
	
	/** The session time count down timer. */
	private Timer				tmrCountDownClock;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	/**
	 * Instantiates a new flexpay maintenance view.
	 */
	public FlexPayMaintenanceView(IAuthenticationStrategy authenticationStrategy)
	{
		super( );

		this.setBackground( Color.WHITE );
		this.setLayout( null );

		addMainPanel( );
		addMainIpPanel( );
		addMainActionsButtonPanel( );
		addMainImageResultPanel( );
		addBottomPanel( );
		addStatusBarPanel( );
		getAppController().setAuthenticationStrategy(authenticationStrategy);
	}

	// ******************************************************************
	// PRIVATE METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	
	/**
	 * Adds the main panel.
	 */
	private void addMainPanel()
	{
		pnlMain = new JPanel( );
		pnlMain.setBounds( 10, 10, 710, 430 );
		pnlMain.setBorder( null );
		pnlMain.setLayout( null );
		pnlMain.setBackground( Color.WHITE );
		add( pnlMain );
	}

	/**
	 * addMainIpPanel.
	 */
	private void addMainIpPanel()
	{
		pnlMainIp = new JPanel( );
		pnlMainIp.setBounds( 10, 0, 690, 73 );
		pnlMainIp.setBorder( null );
		pnlMainIp.setLayout( null );
		pnlMainIp.setBackground( Color.WHITE );
		pnlMain.add( pnlMainIp );

		addIpFields( );
		getDefaultIp( );
		addGilbarcoImage( );
	}

	/**
	 * Adds the gilbarco image.
	 */
	private void addGilbarcoImage()
	{
		lblGilbarcoImg = new JLabel( new ImageIcon( getClass( ).getResource( "/images/gilbarco_top.jpg" ) ) );
		lblGilbarcoImg.setBounds( 425, 0, 250, 60 );
		pnlMainIp.add( lblGilbarcoImg );
	}

	/**
	 * addMainActionsButtonPanel.
	 */
	private void addMainActionsButtonPanel()
	{
		addMainActionsPanel( );

		addInstallButtton( );
		addCompareButton( );

		addStandardLogsButton( );
		addPCILogsButton( );

		addUSBStorageButton( );
		addSecureMenuButton( );
	}

	/**
	 * Adds the main actions panel.
	 */
	private void addMainActionsPanel()
	{
		pnlMainActions = new JPanel( );
		pnlMainActions.setBounds( 10, 84, 690, 82 );
		pnlMainActions.setBorder( null );
		pnlMainActions.setLayout( null );
		pnlMainActions.setBackground( Color.WHITE );
		pnlMain.add( pnlMainActions );
	}

	/**
	 * Adds the install button.
	 */
	private void addInstallButtton()
	{
		btnInstall = new JButton( "Software Upload" );
		btnInstall.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				cleanResults( );
				controller.softwareUpload( );
			}
		} );
		btnInstall.setBounds( 256, 39, 178, 30 );
		btnInstall.setBorderPainted( false );
		btnInstall.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnInstall.setBackground( SECURE_ORANGE_BUTTON_COLOR );
		btnInstall.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnInstall.setToolTipText( "Upload a software package direct to the Payment Terminal." );
		pnlMainActions.add( btnInstall );
	}

	/**
	 * Adds the compare button.
	 */
	private void addCompareButton()
	{
		btnCompare = new JButton( "Software Comparison" );
		btnCompare.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				cleanResults( );
				controller.softwareCompare( );
			}
		} );
		btnCompare.setBounds( 39, 0, 178, 30 );
		btnCompare.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnCompare.setBackground( UNSECURE_GREEN_BUTTON_COLOR );
		btnCompare.setBorderPainted( false );
		btnCompare.setToolTipText( "Compare the version of software you have on this computer with the software version loaded on the Payment Terminal." );
		btnCompare.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		pnlMainActions.add( btnCompare );
	}

	/**
	 * Adds the standard logs button.
	 */
	private void addStandardLogsButton()
	{
		btnStandardLogs = new JButton( "Capture Standard Logs" );
		btnStandardLogs.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				cleanResults( );
				controller.startStandardLogsDownload( );
			}
		} );
		btnStandardLogs.setBounds( 473, 0, 178, 30 );
		btnStandardLogs.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnStandardLogs.setBackground( UNSECURE_GREEN_BUTTON_COLOR );
		btnStandardLogs.setBorderPainted( false );
		btnStandardLogs.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnStandardLogs.setToolTipText( "Download standard logs from the Payment Terminal to this computer." );
		pnlMainActions.add( btnStandardLogs );
	}

	/**
	 * Adds the pci logs button.
	 */
	private void addPCILogsButton()
	{
		btnPCILogs = new JButton( "Capture PCI Logs" );
		btnPCILogs.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				cleanResults( );
				controller.startPCILogsDownload( );				
			}
		} );
		btnPCILogs.setBounds( 473, 39, 178, 30 );
		btnPCILogs.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnPCILogs.setBackground( SECURE_ORANGE_BUTTON_COLOR );
		btnPCILogs.setBorderPainted( false );
		btnPCILogs.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnPCILogs.setToolTipText( "Download PCI Logs from the Payment Terminal to this computer." );

		pnlMainActions.add( btnPCILogs );
	}

	/**
	 * Adds the usb storage button.
	 */
	private void addUSBStorageButton()
	{
		btnUSBStorage = new JButton( "Software USB Storage" );
		btnUSBStorage.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				cleanResults( );
				controller.usbStorageVersion( );
			}
		} );
		btnUSBStorage.setBounds( 256, 0, 178, 30 );
		btnUSBStorage.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnUSBStorage.setBackground( UNSECURE_GREEN_BUTTON_COLOR );
		btnUSBStorage.setOpaque( true );
		btnUSBStorage.setBorderPainted( false );
		btnUSBStorage.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnUSBStorage.setToolTipText( "Transfer a software package to a USB flash drive for upload to the Payment Terminal." );
		pnlMainActions.add( btnUSBStorage );
	}

	/**
	 * Adds the secure menu button.
	 */
	private void addSecureMenuButton()
	{
		btnSecureMenu = new JButton( "Secure Menu" );
		btnSecureMenu.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				controller.activateOrExitSecuremenu( );
			}
		} );
		btnSecureMenu.setBounds( 39, 41, 178, 30 );
		btnSecureMenu.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnSecureMenu.setBackground( SECURE_ORANGE_BUTTON_COLOR );
		btnSecureMenu.setBorderPainted( false );
		btnSecureMenu.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnSecureMenu.setToolTipText( "Enable \"secure menu\" mode on the Payment Terminal." );
		pnlMainActions.add( btnSecureMenu );
	}

	/**
	 * Adds the main image result panel.
	 */
	private void addMainImageResultPanel()
	{
		pnlMainImageResult = new JPanel( );
		pnlMainImageResult.setBounds( 10, 167, 690, 207 );
		pnlMainImageResult.setBorder( null );
		pnlMainImageResult.setLayout( null );
		pnlMainImageResult.setBackground( Color.WHITE );
		pnlMain.add( pnlMainImageResult );

		addProgressBar();
		addResultLeyend("");
		addStatusLeyend("");
		addResultImage(null);
		
		pnlMainImageResult.setVisible( false );
	}

	private void addStatusLeyend(String status)
	{
		lblStatusLeyend = new JLabel( status );
		lblStatusLeyend.setBounds( 100, 122, 490, 23 );
		lblStatusLeyend.setBorder( null );
		lblStatusLeyend.setFont( new Font( "Tahoma", Font.PLAIN, 12 ) );
		lblStatusLeyend.setHorizontalAlignment( SwingConstants.CENTER );
		pnlMainImageResult.add( lblStatusLeyend );	
	}

	private void addProgressBar()
	{
		progressBar = new JProgressBar( 0, 200 );
		progressBar.setBounds( 195, 59, 300, 60 );
		progressBar.setValue( 0 );
		progressBar.setIndeterminate( false );
		progressBar.setStringPainted( true );
		progressBar.setBorderPainted( false );
		progressBar.setMaximum( 0 );
		progressBar.setMinimum( 0 );
		progressBar.setBackground( Color.WHITE );
		pnlMainImageResult.add( progressBar );
	}

	/**
	 * Adds the bottom panel.
	 */
	private void addBottomPanel()
	{
		addBottomButtonsPanel( );
		addViewLogButton( );
		addViewLogsFolderButton();
		addCloseButton( );
	}

	/**
	 * Adds the bottom buttons panel.
	 */
	private void addBottomButtonsPanel()
	{
		pnlButtons = new JPanel( );
		pnlButtons.setBounds( 10, 375, 690, 50 );
		pnlButtons.setBorder( null );
		pnlButtons.setLayout( null );
		pnlButtons.setBackground( Color.WHITE );
		pnlMain.add( pnlButtons );
	}

	/**
	 * Adds the close button.
	 */
	private void addCloseButton()
	{
		btnClose = new JButton( "Close" );
		btnClose.setBackground( DEFAULT_BUTTON_COLOR );
		btnClose.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnClose.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				closeMaintenance( );
			}

		} );
		btnClose.setBounds( 550, 11, 120, 35 );
		btnClose.setBorderPainted( false );
		btnClose.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		pnlButtons.add( btnClose );
		
		this.getActionMap( ).put( "ON_EXIT", new AbstractAction( )
		{
			private static final long	serialVersionUID	= 1L;

			@Override
			public void actionPerformed(ActionEvent e)
			{
				if(btnClose.isEnabled( ))
				{
					closeMaintenance( );
				}
			}
		} );
		this.getInputMap( JComponent.WHEN_IN_FOCUSED_WINDOW ).put( KeyStroke.getKeyStroke( "ESCAPE" ), "ON_EXIT" );
	}

	private void closeMaintenance()
	{
		controller.closedMaintenanceNotification();
		removeStatusBarPanel();
		getRootApp( ).getController( ).closeApp( (FlexPayMaintenanceView) pnlMain.getParent( ) );
		getRootApp( ).getController( ).setSelectedIndex( 1 );
	}

	/**
	 * Adds the view log button.
	 */
	private void addViewLogButton()
	{
		btnViewLog = new JButton( "View Log" );
		btnViewLog.setBackground( DEFAULT_BUTTON_COLOR );
		btnViewLog.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnViewLog.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				controller.showLog( );
			}
		} );
		btnViewLog.setBounds( 20, 11, 120, 35 );
		btnViewLog.setBorderPainted( false );
		btnViewLog.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnViewLog.setVisible( false );
		pnlButtons.add( btnViewLog );
	}

	/**
	 * Adds the view logs folder button.
	 */
	private void addViewLogsFolderButton()
	{
		btnViewLogsFolder = new JButton( "View Logs Folder" );
		btnViewLogsFolder.setBackground( DEFAULT_BUTTON_COLOR );
		btnViewLogsFolder.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		btnViewLogsFolder.setBounds(  20, 11, 125, 35 );
		btnViewLogsFolder.setBorderPainted( false );
		btnViewLogsFolder.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnViewLogsFolder.setVisible( false );
		pnlButtons.add( btnViewLogsFolder );
	}
	
	/**
	 * Adds the ip fields.
	 */
	private void addIpFields()
	{
		lblUnitIP = new JLabel( "Unit IP Address" );
		lblUnitIP.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		lblUnitIP.setBounds( 52, 23, 142, 45 );
		lblUnitIP.setHorizontalAlignment( SwingConstants.CENTER );
		pnlMainIp.add( lblUnitIP );

		txtFieldIp1 = new JTextField( );
		txtFieldIp1.setBounds( 184, 31, 40, 32 );

		txtFieldIp2 = new JTextField( );
		txtFieldIp2.setBounds( 244, 31, 40, 32 );

		txtFieldIp3 = new JTextField( );
		txtFieldIp3.setBounds( 304, 31, 40, 32 );

		txtFieldIp4 = new JTextField( );
		txtFieldIp4.setBounds( 364, 31, 40, 32 );

		JTextField txtFieldIps[] = new JTextField[] { txtFieldIp1, txtFieldIp2, txtFieldIp3, txtFieldIp4 };
		if (txtFieldIps != null)
		{
			adjustIPRow( txtFieldIps );
			for (JTextField txtFieldIp : txtFieldIps)
			{
				txtFieldIp.setHorizontalAlignment( SwingConstants.CENTER );
				pnlMainIp.add( txtFieldIp );
			}
		}
	}

	/**
	 * Adjust ip field.
	 * 
	 * @param ipField
	 *            the ip field
	 * @param keyListener
	 *            the listener
	 */
	private void adjustIPField(JTextField ipField, KeyListener keyListener)
	{		
		ipField.setHorizontalAlignment( SwingConstants.CENTER );
		ipField.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		ipField.setBackground( DEFAULT_EDITABLE_COLOR );
		ipField.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		ipField.setAlignmentY( CENTER_ALIGNMENT );
		ipField.setAlignmentX( RIGHT_ALIGNMENT );
		ipField.addKeyListener( keyListener );
		addIpFocusListener(ipField);
	}


	/**
	 * Adds the ip fields focus listener.
	 *
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
	 * 
	 * @param ipFields
	 *            the ip fields
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
				}
			};
	}

	/**
	 * Save current ip.
	 */
	private void saveCurrentIp()
	{
		String ip1 = EdtStringUtil.trimLeadingZeros( txtFieldIp1.getText( ) );
		String ip2 = EdtStringUtil.trimLeadingZeros( txtFieldIp2.getText( ) );
		String ip3 = EdtStringUtil.trimLeadingZeros( txtFieldIp3.getText( ) );
		String ip4 = EdtStringUtil.trimLeadingZeros( txtFieldIp4.getText( ) );

		controller.getModel( ).setCurrentTerminalIP( IpValidator.joinIP( ip1, ip2, ip3, ip4 ) );
	}

	/**
	 * Gets the default ip.
	 * 
	 * @return the default ip
	 */
	private void getDefaultIp()
	{
		String[] strIp = IpValidator.splitIP( controller.getModel( ).getCurrentTerminalIP( ) );
		if (strIp.length == 4)
		{
			txtFieldIp1.setText( EdtStringUtil.trimLeadingZeros( strIp[0] ) );
			txtFieldIp2.setText( EdtStringUtil.trimLeadingZeros( strIp[1] ) );
			txtFieldIp3.setText( EdtStringUtil.trimLeadingZeros( strIp[2] ) );
			txtFieldIp4.setText( EdtStringUtil.trimLeadingZeros( strIp[3] ) );
		}
	}

	/** @brief Add the Maintenance Status Bar panel extension.
	 *
	 * @since 1.0.0
	 */
	private void addStatusBarPanel ()
	{
		pnlStatusBar = new JPanel(new BorderLayout());
		pnlStatusBar.setOpaque(false);
		addStatusBarCountDownClock();
	}

	/** @brief Add the Maintenance Status Bar session time count down clock. 
	 *
	 * @since 1.0.0
	 */
	private void addStatusBarCountDownClock ()
	{
		lblStatusBarCountDownClock = new JLabel(SESSION_TIME_STRING_STOPPED);
/*
		// Default application status bar font
		Font fds = FlexPayComponentFormatter.getDefaultStatusFont( );
		lblStatusBar.setFont( new Font( fds.getFamily( ), Font.BOLD, fds.getSize( ) ) );
*/
		lblStatusBarCountDownClock.setFont(new Font("Tahoma",Font.BOLD,13));
		lblStatusBarCountDownClock.setForeground(Color.BLACK);
		pnlStatusBar.add(lblStatusBarCountDownClock,BorderLayout.EAST);
		iTimeCountDownClock = 0;
		tmrCountDownClock = new Timer (1000,new ActionListener()
        {
            public void actionPerformed (ActionEvent event)
            {
            	--iTimeCountDownClock;
            	setStatusBarCountDownClock(iTimeCountDownClock);
            }
        });
	}
	
	/** @brief Sets the Maintenance Status Bar session time count down clock. 
	 *
	 * @param iTime [In] is the time to set to the count down clock.
	 *
	 * @since 1.0.0
	 */
	private void setStatusBarCountDownClock (int iTime)
	{
    	boolean bBlink = false;
    	if ((iTime < SESSION_TIME_ALERT_SECONDS) && (lblStatusBarCountDownClock.getForeground() != Color.RED))
    	{
    		lblStatusBarCountDownClock.setForeground(Color.RED);
    	}
    	if (iTime < 0)
    	{
    		bBlink = ((iTime % 2) != 0); //Uses modulo to create a cycle: Shown for one second, no shown for the next 1 second.
        	iTime = 0;
    	}
    	lblStatusBarCountDownClock.setText((bBlink)?(""):(String.format("%02d:%02d",(iTime/60),(iTime%60))));
	}
	
	/** @brief Remove the Maintenance Status Bar panel extension. 
	 *
	 * @since 1.0.0
	 */
	private void removeStatusBarPanel ()
	{
		getRootApp().getStatusToolBar().remove(pnlStatusBar);
		getRootApp().getStatusToolBar().updateUI();
	}
	
	/**
	 * Enable components.
	 * 
	 * @param container
	 *            the container
	 * @param enable
	 *            the enable
	 */
	private void enableComponents(Container container, boolean enable)
	{
		Component[] components = container.getComponents( );
		for (Component component : components)
		{
			if (component instanceof JComboBox || component instanceof JTextField || component instanceof JButton) component.setEnabled( enable );
		}
	}


	/**
	 * Clean the view results.
	 */
	public void cleanResults()
	{
		lblResultImg.setVisible( false );
		progressBar.setVisible( false );
		lblResultLeyend.setVisible( false );
		lblStatusLeyend.setVisible( false );
		pnlMainImageResult.setVisible( false );
	}

	/**
	 * Show result panel.
	 * 
	 * @param result
	 *            the result
	 * @param imageIcon
	 *            the image icon
	 */
	private void showResultPanel(String result, ImageIcon imageIcon)
	{
		pnlMainImageResult.removeAll( );
		addResultLeyend( result );
		addResultImage( imageIcon );
	}

	private void addResultImage(ImageIcon imageIcon)
	{
		lblResultImg = new JLabel( imageIcon );
		lblResultImg.setBounds( 220, 39, 250, 160 );
		lblResultImg.setVisible( true );
		pnlMainImageResult.add( lblResultImg );
	}

	private void addResultLeyend(String result)
	{
		lblResultLeyend = new JLabel( result );
		lblResultLeyend.setBounds( 65, 0, 560, 30 );
		lblResultLeyend.setBorder( BorderFactory.createLineBorder( DEFAULT_BUTTON_COLOR ) );
		lblResultLeyend.setFont( new Font( "Tahoma", Font.PLAIN, 13 ) );
		lblResultLeyend.setHorizontalAlignment( SwingConstants.CENTER );
		pnlMainImageResult.add( lblResultLeyend );
	}

	/**
	 * Gets the success icon.
	 * 
	 * @return the success icon
	 */
	private ImageIcon getSuccessIcon()
	{
		if (lblSuccessIcon == null)
		{
			lblSuccessIcon = new ImageIcon( getClass( ).getResource( "/images/GreenCheckOK.jpg" ) );
		}
		return lblSuccessIcon;
	}

	/**
	 * Gets the alert icon.
	 * 
	 * @return the alert icon
	 */
	private ImageIcon getAlertIcon()
	{
		if (lblAlertIcon == null)
		{
			lblAlertIcon = new ImageIcon( getClass( ).getResource( "/images/YellowInfo.jpg" ) );
		}
		return lblAlertIcon;
	}

	/**
	 * Gets the error icon.
	 * 
	 * @return the error icon
	 */
	private ImageIcon getErrorIcon()
	{
		if (lblErrorIcon == null)
		{
			lblErrorIcon = new ImageIcon( getClass( ).getResource( "/images/RedXNO.jpg" ) );
		}
		return lblErrorIcon;
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	/**
	 * Gets the ip inserted.
	 * 
	 * @return the ip inserted
	 */
	public String getIpInserted()
	{
		return EdtStringUtil.trimLeadingZeros( txtFieldIp1.getText( ) ) + "." + EdtStringUtil.trimLeadingZeros( txtFieldIp2.getText( ) ) + "."
				+ EdtStringUtil.trimLeadingZeros( txtFieldIp3.getText( ) ) + "." + EdtStringUtil.trimLeadingZeros( txtFieldIp4.getText( ) );
	}

	/**
	 * Show secure menu result ok.
	 */
	public void showSecureMenuResultOk()
	{
		showOK( "Secure Menu Activated" );
		btnSecureMenu.setText( "Exit Secure Menu" );
		btnSecureMenu.setToolTipText( "Disable \"secure menu\" mode on the Payment Terminal");
		enableComponents( pnlMainIp, false );
		enableComponents( pnlMainActions, false );
		btnClose.setEnabled( false );
		btnSecureMenu.setEnabled( true );
		pnlMain.getParent( ).repaint( );
	}

	/**
	 * Show exit secure menu result ok.
	 */
	public void showExitSecureMenuResultOk()
	{
		btnSecureMenu.setText( "Secure Menu" );
		btnSecureMenu.setToolTipText( "Enable \"secure menu\" mode on the Payment Terminal." );		
		enableComponents( pnlMainIp, true );
		enableComponents( pnlMainActions, true );
		showOK( "Exited Secure Menu successfully" );

		pnlMain.getParent( ).repaint( );
	}

	/**
	 * Show exit secure menu result error.
	 */
	public void showExitSecureMenuResultError()
	{		
		btnSecureMenu.setText( "Secure Menu" );
		btnSecureMenu.setToolTipText( "Enable \"secure menu\" mode on the Payment Terminal." );		
		enableComponents( pnlMainIp, true );
		enableComponents( pnlMainActions, true );
		showError( "Exit Secure Menu failure - Log Generated" );
		pnlMain.getParent( ).repaint( );
	}
	
	/**
	 * Show error.
	 * 
	 * @param error
	 *            the error
	 */
	public void showError(String error)
	{
		showResultPanel( error, getErrorIcon( ) );

		btnViewLog.setVisible( true );
		btnViewLogsFolder.setVisible( false );
		hideProgressComponnents( );
		enableComponents( pnlMainIp, true );
		enableComponents( pnlMainActions, true );
		enableComponents( pnlButtons, true );

		pnlMainImageResult.setVisible( true );
		pnlMain.getParent( ).repaint( );
	}
	
	/**
	 * Show ok.
	 * 
	 * @param resultmessage
	 *            the resultmessage
	 */
	public void showOK(String resultmessage)
	{
		showResultPanel( resultmessage, getSuccessIcon( ) );

		hideProgressComponnents( );

		enableComponents( pnlMainIp, true );
		enableComponents( pnlMainActions, true );
		enableComponents( pnlButtons, true );

		pnlMainImageResult.setVisible( true );
		pnlMain.getParent( ).repaint( );
	}

	/**
	 * Show warning.
	 * 
	 * @param info
	 *            the info
	 */
	public void showWarning(String info)
	{
		showResultPanel( info, getAlertIcon( ) );

		hideProgressComponnents( );
		btnViewLog.setVisible( true );
		btnViewLogsFolder.setVisible( false );

		enableComponents( pnlMainIp, true );
		enableComponents( pnlMainActions, true );
		enableComponents( pnlButtons, true );

		pnlMainImageResult.setVisible( true );
		pnlMainImageResult.getParent( ).repaint( );
		pnlMain.getParent( ).repaint( );
	}


	/** @brief Shows a clean maintenance screen, as there where no actions performed. Usually called after canceling an operation. 
	 *
	 * @since 1.0.0
	 */
	public void showClean()
	{
		pnlMainImageResult.removeAll( );

		btnViewLog.setVisible( false );
		btnViewLogsFolder.setVisible( false );
		hideProgressComponnents();

		enableComponents( pnlMainIp, true );
		enableComponents( pnlMainActions, true );
		enableComponents( pnlButtons, true );

		pnlMain.getParent( ).repaint( );
	}

	/**
	 * Show logs folder button.
	 *
	 * @param show true to show the button
	 */
	public void showLogsFolderButton(final String logfolder)
	{
		btnViewLogsFolder.setVisible( true );
		
		for(int i=0; i < btnViewLogsFolder.getActionListeners( ).length; i++)
		{
			btnViewLogsFolder.removeActionListener( btnViewLogsFolder.getActionListeners( )[i]);
		}
		
		btnViewLogsFolder.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				controller.showLogsFolder( logfolder );
			}
		} );
	}
	
	
	/**
	 * Hide progress componnents.
	 */
	public void hideProgressComponnents()
	{
		progressBar.setVisible( false );
		lblStatusLeyend.setVisible( false );
	}

	/**
	 * Show progress bar.
	 * 
	 * @param result
	 *            the result
	 * @param during
	 *            the during
	 */
	public void showProgressBar(String result, String during)
	{
		pnlMainImageResult.removeAll( );

		addResultLeyend(result);		
		addProgressBar();
		addStatusLeyend(during);

		enableComponents( pnlMainIp, false );
		enableComponents( pnlButtons, false );

		pnlMainImageResult.setVisible( true );
		pnlMain.getParent( ).repaint( );
	}

	/**
	 * Increment progress.
	 * 
	 * @param step
	 *            the step
	 * @param max
	 *            the max
	 * @param min
	 *            the min
	 * @param text
	 *            the text
	 */
	public void incrementProgress(int step, int max, int min, String text, boolean indeterminate)
	{
		progressBar.setBorderPainted( true );
		progressBar.setIndeterminate( indeterminate );
		if (max == min)
		{
			progressBar.setStringPainted( false );
		}
		else
		{
			progressBar.setMaximum( max );
			progressBar.setMinimum( min );
			progressBar.setValue( step );
		}
		lblStatusLeyend.setText( text );
	}

	/**
	 * Increment progress and not change current min/max value.
	 * 
	 * @param step
	 *            the step
	 * @param text
	 *            the text
	 */
	public void incrementProgress(int step, String text)
	{
		progressBar.setBorderPainted( true );
		progressBar.setValue( progressBar.getValue( ) + step );
		lblStatusLeyend.setText( text );
	}
	
	/** @brief Starts session time count down clock. 
	 *
	 * @since 1.0.0
	 */
	public void startCountDownClock ()
	{
		this.iTimeCountDownClock = SESSION_TIME_SECONDS;
		this.lblStatusBarCountDownClock.setForeground(Color.BLACK);
		this.tmrCountDownClock.restart();
    	setStatusBarCountDownClock(iTimeCountDownClock);
	}
	
	/** @brief Stops session time count down clock. 
	 *
	 * @since 1.0.0
	 */
	public void stopCountDownClock ()
	{
		this.iTimeCountDownClock = 0;
		this.lblStatusBarCountDownClock.setForeground(Color.BLACK);
		this.tmrCountDownClock.stop();
		this.lblStatusBarCountDownClock.setText(SESSION_TIME_STRING_STOPPED);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppView#initialize()
	 */
	@Override
	public void initialize()
	{
		setBackground( Color.WHITE );
		setLayout( null );
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppView#getAppMenu()
	 */
	@Override
	public JMenu getAppMenu()
	{
		return null;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppView#getAppName()
	 */
	@Override
	public String getAppName()
	{
		return controller.getTitleAppName();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppView#getAppIcon()
	 */
	@Override
	public Icon getAppIcon()
	{
		return null;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppView#addStatusBarComponent(javax.swing.JToolBar)
	 */
	@Override
	public void addStatusBarComponent(JToolBar toolBar)
	{
		toolBar.add(this.pnlStatusBar);
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppView#createController()
	 */
	@Override
	protected FlexPayMaintenanceController createController()
	{
		return FlexPayMaintenanceController.getOrCreateController( this );
	}

	/**
	 * Show init action.
	 * 
	 * @param action
	 *            the action
	 * @param duringaction
	 *            the duringaction
	 */
	protected void showInitAction(String action, String duringaction)
	{
		enableComponents( pnlMainIp, false );
		enableComponents( pnlMainActions, false );

		btnViewLog.setVisible( false );
		btnViewLogsFolder.setVisible( false );
		lblResultLeyend.setText( action + " - Please Wait" );
		lblStatusLeyend.setText( duringaction );
		progressBar.setVisible( true );
		lblResultLeyend.setVisible( true );
		lblStatusLeyend.setVisible( true );
		pnlMainImageResult.setVisible( true );
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

	/**
	 * Checks if is close action is enabled.
	 *
	 * @return true, if is close action enabled
	 */
	public boolean isCloseEnabled()
	{
		return btnClose.isEnabled( );
	}
}
