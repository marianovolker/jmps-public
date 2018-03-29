
package com.gilbarco.globaltools.spotdecommissioning.main.gui;

import java.awt.CardLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.SwingConstants;
import javax.swing.UIManager;
import javax.swing.WindowConstants;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.edt.util.IpValidator;
import com.gilbarco.globaltools.spotdecommissioning.config.SPOTDecommissioningAppConfig;
import com.gilbarco.globaltools.spotdecommissioning.widgets.JSPOTDecommissioningDialog;

import javax.swing.border.EtchedBorder;

public class SPOTDecommissioningView extends EdtAppView<SPOTDecommissioningController>
{

	private static final int					MAX_DOT_LINE		= 20;
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final long					serialVersionUID	= 1L;
	private static final transient Log			logger				= LogFactory.getLog( SPOTDecommissioningView.class );

	@SuppressWarnings("unused")
	private static SPOTDecommissioningAppConfig	_appconfig;

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

	private JButton								btnClearKeys;

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
	public SPOTDecommissioningView()
	{
		super( );

		setBackground( SPOTDecommissioningComponentFormatter.getPanelDefaultBackgroundColor( ) );
		setLayout( null );
		// Gilbarco Image
		addGilbarcoLogo( );
		// Add IP Fields
		addIpFields( );
		// Add ClearKeys Button
		addClearKeysBtn( );
		// Add Clear Keys Status Fields
		addStatusField( );
		addProgressField( );
		// Add Clear Keys Icon Image Result
		addResultImage( );
		// Add Exit Button
		addExitButton( );
		// Copyright Label in status bar
		addCopyrightGilbarco( );
		// Set the main panel as the required specification
		setRootAppProperties( );

		logger.info( getAppName( ) + " Started." );
	}

	/**
	 * Create application configuration
	 */
	private void createSPOTDecommissioningAppConfig()
	{
		if (this.getRootApp( ) != null)
		{
			if (this.getRootApp( ).getAppProperties( ) != null)
			{
				_appconfig = SPOTDecommissioningAppConfig.getInstance( this.getRootApp( ).getAppProperties( ) );
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
		createSPOTDecommissioningAppConfig( );
		setIpField( );
		setStatusInfo( );
	}

	private void setStatusInfo()
	{
		if (this.getIpInserted( ).length( ) > 0)
		{
			showSuccess( "ready to clear data" );
			enableClearKeys( true );
		}
		else
		{
			showInfo( "enter IP Address" );
			enableClearKeys( false );
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
		return "SPOTDecommissioning v1.0.0";
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
	protected SPOTDecommissioningController createController()
	{
		return new SPOTDecommissioningController( this );
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
			getRootApp( ).setAlwaysOnTop( false );
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
		lblGilbarcoImg.setBounds( 480, 10, 250, 60 );
		add( lblGilbarcoImg );
	}

	private void addIpFields()
	{
		lblFuelingPositionIp = new JLabel( "IP Address:" );
		lblFuelingPositionIp.setBounds( 36, 50, 120, 30 );
		lblFuelingPositionIp.setFont( JSPOTDecommissioningDialog.getDefaultFont( ).deriveFont( 18 ) );
		lblFuelingPositionIp.setHorizontalAlignment( SwingConstants.LEFT );
		add( lblFuelingPositionIp );

		txtFieldIp1 = new JTextField( );
		txtFieldIp1.setBounds( 166, 49, 40, 30 );

		txtFieldIp2 = new JTextField( );
		txtFieldIp2.setBounds( 224, 49, 40, 30 );

		txtFieldIp3 = new JTextField( );
		txtFieldIp3.setBounds( 283, 49, 40, 30 );

		txtFieldIp4 = new JTextField( );
		txtFieldIp4.setBounds( 343, 49, 40, 30 );

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

	private void adjustIPField(JTextField ipField, KeyListener listener)
	{
		ipField.setHorizontalAlignment( SwingConstants.CENTER );
		ipField.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		ipField.setFont( JSPOTDecommissioningDialog.getDefaultFont( ).deriveFont( 13 ) );
		ipField.setAlignmentY( CENTER_ALIGNMENT );
		ipField.setAlignmentX( RIGHT_ALIGNMENT );

		ipField.addKeyListener( listener );
	}

	private void adjustIPRow(JTextField ipFields[])
	{
		if (ipFields != null)
		{
			if (ipFields.length > 1)
			{
				int iFieldFirst = 0;
				int iFieldLast = ipFields.length - 1;

				adjustIPField(ipFields[iFieldFirst],new KeyAdapter( )  // First field value has to be between 1 and 255.
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
						JTextField ipField = (JTextField) e.getSource(); 
						String ipFieldText = ipField.getText();

						if (EdtStringUtil.isStringNumeric(ipFieldText))
						{
							if (ipFieldText.length() > 3)
							{
								ipFieldText = ipFieldText.substring(0, 3);
							}
							try
							{
								int ipFieldValue = Integer.parseInt( ipFieldText );
								if ((ipFieldValue > 0) && (ipFieldValue <= 255))
								{
									ipFieldText = Integer.toString(ipFieldValue);
								}
								else if (ipFieldValue > 255)
								{
									ipFieldText = Integer.toString(ipFieldValue).substring( 0, 2 );
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
						ipField.setText(ipFieldText);

						saveCurrentIp( );
						setStatusInfo( );
					}
				});
				adjustIPField(ipFields[iFieldLast],new KeyAdapter()  // Last field value has to be between 1 and 254.
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
						JTextField ipField = (JTextField) e.getSource(); 
						String ipFieldText = ipField.getText();

						if (EdtStringUtil.isStringNumeric(ipFieldText))
						{
							if (ipFieldText.length() > 3)
							{
								ipFieldText = ipFieldText.substring(0, 3);
							}
							try
							{
								int ipFieldValue = Integer.parseInt( ipFieldText );
								if ((ipFieldValue > 0) && (ipFieldValue < 255))
								{
									ipFieldText = Integer.toString(ipFieldValue);
								}
								else if (ipFieldValue >= 255)
								{
									ipFieldText = Integer.toString(ipFieldValue).substring( 0, 2 );
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
						ipField.setText(ipFieldText);

						saveCurrentIp( );
						setStatusInfo( );
					}
				});
				if (ipFields.length > 2) // Middle fields value have to be between 0 and 255.
				{
					for (int iFieldNbr = iFieldFirst + 1; iFieldNbr < iFieldLast; ++iFieldNbr)
					{
						adjustIPField(ipFields[iFieldNbr],new KeyAdapter()
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
								JTextField ipField = (JTextField) e.getSource(); 
								String ipFieldText = ipField.getText();

								if (EdtStringUtil.isStringNumeric(ipFieldText))
								{
									if (ipFieldText.length() > 3)
									{
										ipFieldText = ipFieldText.substring(0, 3);
									}
									try
									{
										int ipFieldValue = Integer.parseInt( ipFieldText );
										if ((ipFieldValue >= 0) && (ipFieldValue <= 255))
										{
											ipFieldText = Integer.toString(ipFieldValue);
										}
										else if (ipFieldValue > 255)
										{
											ipFieldText = Integer.toString(ipFieldValue).substring( 0, 2 );
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
								ipField.setText(ipFieldText);

								saveCurrentIp( );
								setStatusInfo( );
							}
						});
					}
				}
			}
		}
	}	

	private void addCopyrightGilbarco()
	{
		lblStatusBar = new JLabel( "Copyright Gilbarco Inc. 2013" );
		Font fds = SPOTDecommissioningComponentFormatter.getDefaultStatusFont( );
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
		txtStatus.setBorder( new EtchedBorder( EtchedBorder.LOWERED, null, null ) );
		txtStatus.setEditable( false );
		txtStatus.setEnabled( true );
		txtStatus.setColumns( 10 );
		txtStatus.setHorizontalAlignment( SwingConstants.CENTER );
		txtStatus.setFont( JSPOTDecommissioningDialog.getDefaultSmallFont( ).deriveFont( Font.BOLD ) );
		txtStatus.setBounds( 185, 330, 297, 30 );
		txtStatus.setBackground( SPOTDecommissioningComponentFormatter.getPanelDefaultBackgroundColor( ) );
		add( txtStatus );
	}

	private void addProgressField()
	{
		lblProgress = new JLabel( );
		lblProgress.setHorizontalAlignment( SwingConstants.LEFT );
		lblProgress.setFont( JSPOTDecommissioningDialog.getDefaultSmallFont( ).deriveFont( Font.BOLD ) );
		lblProgress.setBounds( 310, 351, 100, 30 );
		lblProgress.setText( "" );
		lblProgress.setVisible( false );
		add( lblProgress );
	}

	private void addClearKeysBtn()
	{
		btnClearKeys = new JButton( "Clear Data" );
		btnClearKeys.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				startClearKeys( );
			}
		} );
		btnClearKeys.setBounds( 249, 396, 168, 30 );
		btnClearKeys.setFont( JSPOTDecommissioningDialog.getDefaultFont( ).deriveFont( 11 ) );
		btnClearKeys.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		add( btnClearKeys );
	}

	private void addExitButton()
	{
		btnExit = new JButton( "Exit" );
		btnExit.setFont( JSPOTDecommissioningDialog.getDefaultFont( ).deriveFont( 11 ) );
		btnExit.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				getAppController( ).exitApplication( 0 );
			}
		} );
		btnExit.setBounds( 580, 430, 120, 50 );
		btnExit.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		add( btnExit );
	}

	private void startClearKeys()
	{
		IpValidator ipValidator = new IpValidator( );
		String strIP = getIpInserted( );
		if (ipValidator.validate( strIP ))
		{
			int opcion = showConfirmDialog( "Clear Data on " + strIP + " ? " );
			if (opcion == JOptionPane.YES_OPTION)
			{
				enableEntry( false );
				showDecommissioningWarning( );
				controller.startDecommissioning( );
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
		this.txtFieldIp1.setEnabled( enable );
		this.txtFieldIp2.setEnabled( enable );
		this.txtFieldIp3.setEnabled( enable );
		this.txtFieldIp4.setEnabled( enable );
	}

	private void enableClearKeys(boolean enable)
	{
		this.btnClearKeys.setEnabled( enable );
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
		enableClearKeys( true );
	}

	protected void showError(String status)
	{
		lblResultImage.setIcon( getErrorIcon( ) );
		lblResultImage.setVisible( true );
		txtStatus.setText( status );
		lblProgress.setVisible( false );
		enableEntry( true );
		enableClearKeys( true );
		this.repaint( );
		this.revalidate( );
	}

	protected void showDecommissioningWarning()
	{
		warningDot = "...";
		showWarning( "Clearing Keys - Please Wait" );
		lblProgress.setVisible( true );
		showProgress();		
	}

	protected void showWarning(String status)
	{
		lblResultImage.setIcon( getAlertIcon( ) );
		lblResultImage.setVisible( true );
		txtStatus.setText( status );
		enableEntry( false );
		enableClearKeys( false );
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

	public JButton getClearKeysButton()
	{
		return this.btnClearKeys;
	}

	// ******************************************************************
	// MAIN.
	// ******************************************************************
}
