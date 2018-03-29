
package com.gilbarco.globaltools.flexpay.rkl.gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;
import javax.swing.SwingConstants;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.edt.util.IpValidator;
import com.gilbarco.globaltools.flexpay.main.gui.FlexPayComponentFormatter;
import com.gilbarco.globaltools.flexpay.maintenance.gui.FlexPayMaintenanceView;
import com.gilbarco.globaltools.flexpay.model.FlexPayContext;
import com.gilbarco.globaltools.flexpay.widgets.JFlexPayDialog;

public class FlexPayRklView extends EdtAppView<FlexPayRklController>
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final long	serialVersionUID	= 1L;
	protected static final int	MAX_IP_FIELD_LEN	= 3;
	private JLabel				lblGilbarcoImg;
	private JTextField			textFieldIp1;
	private JTextField			textFieldIp2;
	private JTextField			textFieldIp3;
	private JTextField			textFieldIp4;
	private JButton				btnViewOutput;
	private JButton				btnExit;
	private JTextField			txtSerialNumber;
	private JTextField			txtDeviceType;
	private JTextField			txtHashValue;
	private JTextField			txtHostKey;
	private JTextField			txtTechNumber;
	private JTextField			txtRklInfo;
	private JTextField			txtRklStatus;
	private JLabel				lblRklResultImage;
	private ImageIcon			lblSuccessIcon		= null;
	private ImageIcon			lblAlertIcon		= null;
	private ImageIcon			lblErrorIcon		= null;
	private JButton				buttonRkl;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public FlexPayRklView()
	{
		super( );
		setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		setLayout( null );

		addIPFields( );
		addSerilaNumberField( );
		addDeviceTypeField( );
		addHashValueField( );
		addHostKeyField( );
		addTechNumberField( );

		addRklStatusField( );
		addRklInfoField( );
		addRklButton( );
		addRklResultImage( );

		addViewOututButton( );
		addGilbarcoLogo( );
		addExitButton( );

		// Apply default application setting
		formatView( );
	}

	private void addTechNumberField()
	{
		JLabel lblTechNumber = new JLabel( FlexPayContext.getLocaleText( "TechNum" ) );
		lblTechNumber.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 12 ) );
		lblTechNumber.setBounds( 538, 17, 133, 18 );
		add( lblTechNumber );

		txtTechNumber = new JTextField( );
		txtTechNumber.setEnabled( false );
		txtTechNumber.setEditable( false );
		txtTechNumber.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtTechNumber.setColumns( 10 );
		txtTechNumber.setBounds( 624, 14, 90, 27 );
		txtTechNumber.setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		txtTechNumber.setText( FlexPayRklModel.getTechnicianID( ) );
		add( txtTechNumber );
	}

	private void addHashValueField()
	{
		JLabel lblHashValue = new JLabel( FlexPayContext.getLocaleText( "HashValue" ) );
		lblHashValue.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 12 ) );
		lblHashValue.setBounds( 505, 66, 133, 18 );
		add( lblHashValue );

		txtHashValue = new JTextField( );
		txtHashValue.setEnabled( false );
		txtHashValue.setEditable( false );
		txtHashValue.setColumns( 10 );
		txtHashValue.setBounds( 624, 60, 89, 27 );
		txtHashValue.setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		txtHashValue.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		add( txtHashValue );
	}

	private void addHostKeyField()
	{
		JLabel lblHostKey = new JLabel( FlexPayContext.getLocaleText( "HostKey" ) );
		lblHostKey.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 12 ) );
		lblHostKey.setBounds( 505, 99, 121, 18 );
		add( lblHostKey );

		txtHostKey = new JTextField( );
		txtHostKey.setEnabled( false );
		txtHostKey.setEditable( false );
		txtHostKey.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtHostKey.setColumns( 10 );
		txtHostKey.setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		txtHostKey.setBounds( 624, 93, 89, 27 );
		add( txtHostKey );
	}

	private void addDeviceTypeField()
	{
		JLabel lblDeviceType = new JLabel( FlexPayContext.getLocaleText( "DeviceType" ) );
		lblDeviceType.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 12 ) );
		lblDeviceType.setBounds( 134, 95, 127, 18 );
		add( lblDeviceType );

		txtDeviceType = new JTextField( );
		txtDeviceType.setEnabled( false );
		txtDeviceType.setEditable( false );
		txtDeviceType.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtDeviceType.setColumns( 10 );
		txtDeviceType.setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		txtDeviceType.setBounds( 291, 93, 99, 27 );
		add( txtDeviceType );
	}

	private void addSerilaNumberField()
	{
		JLabel lblSerialNumber = new JLabel( FlexPayContext.getLocaleText( "SerialNumber" ) );
		lblSerialNumber.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 12 ) );
		lblSerialNumber.setBounds( 134, 62, 139, 18 );
		add( lblSerialNumber );

		txtSerialNumber = new JTextField( );
		txtSerialNumber.setEnabled( false );
		txtSerialNumber.setEditable( false );
		txtSerialNumber.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtSerialNumber.setColumns( 10 );
		txtSerialNumber.setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		txtSerialNumber.setBounds( 291, 55, 99, 27 );
		add( txtSerialNumber );
	}

	private void addIPFields()
	{
		JLabel lblFuelingPositionIp = new JLabel( FlexPayContext.getLocaleText( "FuelingPositionIPAddress" ) );
		lblFuelingPositionIp.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 12 ) );
		lblFuelingPositionIp.setBounds( 50, 17, 189, 18 );
		add( lblFuelingPositionIp );

		textFieldIp1 = new JTextField( );
		textFieldIp1.setBounds( 242, 11, 44, 27 );
		adjustIPField( textFieldIp1 );
		add( textFieldIp1 );

		textFieldIp2 = new JTextField( );
		textFieldIp2.setBounds( 293, 11, 44, 27 );
		adjustIPField( textFieldIp2 );
		add( textFieldIp2 );

		textFieldIp3 = new JTextField( );
		textFieldIp3.setBounds( 349, 11, 44, 27 );
		adjustIPField( textFieldIp3 );
		add( textFieldIp3 );

		textFieldIp4 = new JTextField( );
		textFieldIp4.setBounds( 405, 11, 44, 27 );
		adjustIPField( textFieldIp4 );
		add( textFieldIp4 );

		getDefaultIp( );
	}

	private void adjustIPField(JTextField textFieldIp)
	{
		textFieldIp.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		textFieldIp.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 13 ) );
		textFieldIp.setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		textFieldIp.setAlignmentY( CENTER_ALIGNMENT );
		textFieldIp.setAlignmentX( RIGHT_ALIGNMENT );

		textFieldIp.addKeyListener( new KeyAdapter( )
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
				saveDefaultIp( );
			}

			@Override
			public void keyReleased(KeyEvent e)
			{
				String text = ((JTextField) e.getSource( )).getText( );
				if (!EdtStringUtil.isStringNumeric( text ))
				{
					assert false : "Shouldn't be here !";
					((JTextField) e.getSource( )).setText( "" );
					return;
				}

				if (text.length( ) > 3)
				{
					((JTextField) e.getSource( )).setText( text.substring( 0, 3 ) );
				}
				saveDefaultIp( );
			}
		} );
	}

	protected void saveDefaultIp()
	{
		String ip1 = EdtStringUtil.trimLeadingZeros( textFieldIp1.getText( ) );
		String ip2 = EdtStringUtil.trimLeadingZeros( textFieldIp2.getText( ) );
		String ip3 = EdtStringUtil.trimLeadingZeros( textFieldIp3.getText( ) );
		String ip4 = EdtStringUtil.trimLeadingZeros( textFieldIp4.getText( ) );

		controller.setCurrentTerminalIP( IpValidator.joinIP( ip1, ip2, ip3, ip4 ) );
	}

	private void getDefaultIp()
	{
		String[] strIp = IpValidator.splitIP( controller.getCurrentTerminalIP( ) );
		if (strIp.length == 4)
		{
			textFieldIp1.setText( EdtStringUtil.trimLeadingZeros( strIp[0] ) );
			textFieldIp2.setText( EdtStringUtil.trimLeadingZeros( strIp[1] ) );
			textFieldIp3.setText( EdtStringUtil.trimLeadingZeros( strIp[2] ) );
			textFieldIp4.setText( EdtStringUtil.trimLeadingZeros( strIp[3] ) );
		}
	}

	private void formatView()
	{
		FlexPayComponentFormatter.setPanelDefaultBackgroundColor( this );
		FlexPayComponentFormatter.setDefaultFont( this );
	}

	private void addGilbarcoLogo()
	{
		lblGilbarcoImg = new JLabel( new ImageIcon( getClass( ).getResource( "/images/gilbarco_top.jpg" ) ) );
		lblGilbarcoImg.setBounds( 300, 410, 218, 65 );
		add( lblGilbarcoImg );
	}

	private void addExitButton()
	{
		btnExit = new JButton( "Close" );
		btnExit.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		btnExit.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 11 ) );
		btnExit.setBounds( 598, 428, 148, 33 );
		btnExit.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				closeRKL( );
			}
		} );

		this.getActionMap( ).put( "ON_EXIT", new AbstractAction( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				closeRKL( );
			}
		} );
		this.getInputMap( JComponent.WHEN_IN_FOCUSED_WINDOW ).put( KeyStroke.getKeyStroke( "ESCAPE" ), "ON_EXIT" );

		add( btnExit );
	}

	private void closeRKL()
	{
		getRootApp( ).getController( ).closeApp( this );
		getRootApp( ).getController( ).setSelectedIndex( 1 );
	}

	private void addViewOututButton()
	{
		btnViewOutput = new JButton( FlexPayContext.getLocaleText( "ViewOutput" ) );
		btnViewOutput.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		btnViewOutput.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 11 ) );

		btnViewOutput.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				getRootApp( ).getController( ).closeApp( (FlexPayRklView) btnViewOutput.getParent( ) );
				getRootApp( ).getController( ).setSelectedIndex( 0 );
				getRootApp( ).getController( ).setEnabledAt( 0, true );

			}
		} );
		btnViewOutput.setBounds( 53, 428, 176, 33 );
		add( btnViewOutput );
	}

	private void addRklButton()
	{
		buttonRkl = new JButton( FlexPayContext.getLocaleText( "RemoteKeyLoad" ) );
		buttonRkl.setBorder( null );
		buttonRkl.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 11 ) );
		buttonRkl.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				enableComponents( false );
				getAppController( ).startRKL( );
			}
		} );
		buttonRkl.setEnabled( false );
		buttonRkl.setBounds( 227, 132, 346, 33 );
		add( buttonRkl );
	}

	protected void setDeviceType(String device)
	{
		this.txtDeviceType.setText( device );
	}

	protected void setSerialNumber(String sernum)
	{
		this.txtSerialNumber.setText( sernum );

	}

	protected void setHostKey(String hostkey)
	{
		this.txtHostKey.setText( hostkey );
	}

	protected void setHashValue(String hashvalue)
	{
		this.txtHashValue.setText( hashvalue );
	}

	protected void setRklStatus(String rklStatus)
	{
		if (rklStatus.length( ) > 0)
		{
			txtRklStatus.setText( rklStatus );
		}
	}

	protected void setRklInfo(String rklInfo)
	{
		txtRklInfo.setText( rklInfo );
	}

	protected void rklShowSuccess(String status)
	{
		txtRklInfo.setVisible( false );
		lblRklResultImage.setIcon( getSuccessIcon( ) );
		lblRklResultImage.setVisible( true );
		enableComponents( true );
		setRklStatus( status );
	}

	protected void rklShowError(String status)
	{
		txtRklInfo.setVisible( false );
		lblRklResultImage.setIcon( getErrorIcon( ) );
		lblRklResultImage.setVisible( true );
		enableComponents( true );
		setRklStatus( status );
	}

	protected void rklShowAlert(String status)
	{
		txtRklInfo.setVisible( false );
		lblRklResultImage.setIcon( getAlertIcon( ) );
		lblRklResultImage.setVisible( true );
		enableComponents( true );
		setRklStatus( status );
	}

	private void enableComponents(boolean enable)
	{
		this.btnExit.setEnabled( enable );
		this.btnViewOutput.setEnabled( enable );
		this.buttonRkl.setEnabled( enable );
		this.textFieldIp1.setEnabled( enable );
		this.textFieldIp2.setEnabled( enable );
		this.textFieldIp3.setEnabled( enable );
		this.textFieldIp4.setEnabled( enable );
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

	private void addRklResultImage()
	{
		lblRklResultImage = new JLabel( );
		lblRklResultImage.setBounds( 297, 198, 218, 200 );
		lblRklResultImage.setVisible( false );
		add( lblRklResultImage );
	}

	private void addRklStatusField()
	{
		txtRklStatus = new JTextField( );
		txtRklStatus.setEditable( false );
		txtRklStatus.setEnabled( true );
		txtRklStatus.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 12 ) );
		txtRklStatus.setBorder( null );
		txtRklStatus.setColumns( 10 );
		txtRklStatus.setHorizontalAlignment( SwingConstants.CENTER );
		txtRklStatus.setBounds( 167, 174, 459, 22 );
		txtRklStatus.setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		txtRklStatus.setText( FlexPayContext.getLocaleText( "RKLStatusEnterIp" ) );
		add( txtRklStatus );
	}

	private void addRklInfoField()
	{
		txtRklInfo = new JTextField( );
		txtRklInfo.setFont( JFlexPayDialog.getDefaultFont( ).deriveFont( 12 ) );
		txtRklInfo.setEditable( false );
		txtRklInfo.setVisible( true );
		txtRklInfo.setColumns( 10 );
		txtRklInfo.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtRklInfo.setHorizontalAlignment( SwingConstants.CENTER );
		txtRklInfo.setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		txtRklInfo.setBounds( 134, 202, 536, 207 );
		add( txtRklInfo );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public void initialize()
	{

		btnExit.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				getRootApp( ).getController( ).closeApp( (FlexPayRklView) btnExit.getParent( ) );
				getRootApp( ).getController( ).setSelectedIndex( 1 );
				getRootApp( ).getController( ).setEnabledAt( 1, true );
			}
		} );

	}

	@Override
	public JMenu getAppMenu()
	{
		return null;
	}

	@Override
	public String getAppName()
	{
		return "FlexPay Maintenance Tool";
	}

	@Override
	public Icon getAppIcon()
	{
		return null;
	}

	@Override
	public void addStatusBarComponent(JToolBar toolBar)
	{
	}

	public String getIpInserted()
	{
		return EdtStringUtil.trimLeadingZeros( textFieldIp1.getText( ) ) + "." + EdtStringUtil.trimLeadingZeros( textFieldIp2.getText( ) ) + "."
				+ EdtStringUtil.trimLeadingZeros( textFieldIp3.getText( ) ) + "." + EdtStringUtil.trimLeadingZeros( textFieldIp4.getText( ) );
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	@Override
	protected FlexPayRklController createController()
	{
		return new FlexPayRklController( this );
	}

	/**
	 * Checks if is close action is enabled.
	 *
	 * @return true, if is close action enabled
	 */
	public boolean isCloseEnabled()
	{
		return this.btnExit.isEnabled( );
	}
}
