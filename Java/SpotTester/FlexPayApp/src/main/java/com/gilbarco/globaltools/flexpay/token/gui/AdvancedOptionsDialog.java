
package com.gilbarco.globaltools.flexpay.token.gui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.edt.util.IpValidator;
import com.gilbarco.globaltools.flexpay.token.engines.SASTokenManagementEngine;
import com.gilbarco.globaltools.flexpay.widgets.JFlexPayDialog;

/**
 * @author mgrieco
 * 
 */
public class AdvancedOptionsDialog extends JFlexPayDialog
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger				= LogFactory.getLog( AdvancedOptionsDialog.class );
	private static final long			serialVersionUID	= 1L;
	protected static final int			MAX_PORT_LEN		= 10;
	protected static final String		SHOULDNT_BE_HERE	= "Shouldn't be here !";

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
	private JButton					btnSetDefaultTokenServerPort;
	private JButton					btnSetDefaultTerminalPort;
	private JTextField				textServerIp1;
	private JTextField				textServerIp2;
	private JTextField				textTerminalPort;
	private JTextField				textServerPort;
	private JPanel					pnlNetworkOp;

	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************

	public AdvancedOptionsDialog()
	{
		super( );
		setLocation( -223, -44 );
		commonConstructor( );
	}

	private void commonConstructor()
	{
		setLayoutSettings( );

		addNetworkOptionPanel( );

		addSaveButton( );

		addCloseButton( );

		formatDialog( );

		setNotDefaulGraphicSettings( );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	@Override
	protected boolean loadDialogData()
	{
		textServerIp1.setText( TokenManagerModel.getTokenServerIp1( ) );
		textServerIp2.setText( TokenManagerModel.getTokenServerIp2( ) );
		textServerPort.setText( Integer.toString( TokenManagerModel.getTokenServerPort( ) ) );
		textTerminalPort.setText( Integer.toString( TokenManagerModel.getTerminalPort( ) ) );

		return true;
	}

	@Override
	protected boolean saveDialogData()
	{
		TokenManagerModel.setTokenServerIp1( textServerIp1.getText( ) );
		TokenManagerModel.setTokenServerIp2( textServerIp2.getText( ) );
		TokenManagerModel.setTokenServerPort( textServerPort.getText( ) );
		TokenManagerModel.setTerminalPort( textTerminalPort.getText( ) );
		
		try
		{
			SASTokenManagementEngine.getInstance( ).reconfigure( );
			
			logger.info( "Saved configuration!." );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}

		return true;
	}

	@Override
	protected boolean clearDialogData()
	{
		textServerIp1.removeAll( );
		textServerIp2.removeAll( );
		textServerPort.removeAll( );
		textTerminalPort.removeAll( );
		
		return true;
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	public String getServerIP1String()
	{
		return textServerIp1.getText( );
	}

	public String getServerIP2String()
	{
		return textServerIp2.getText( );
	}

	public String getServerPortString()
	{
		return textServerPort.getText( );
	}

	public String getTerminalPortString()
	{
		return textTerminalPort.getText( );
	}

	// ******************************************************************
	// PROTECTED METHODS
	// ******************************************************************

	protected void closeDialog()
	{
		this.dispose( );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	private JPanel getNetworkOpPanel()
	{
		return this.pnlNetworkOp;
	}

	private void setLayoutSettings()
	{
		setDefaultCloseOperation( JDialog.HIDE_ON_CLOSE );
		getContentPane( ).setLayout( null );

		setPreferredSize( new Dimension( 375, 265 ) );
		setTitle( "Advanced Options" );
		setResizable( false );
	}
	
	private void addNetworkOptionPanel()
	{
		pnlNetworkOp = new JPanel( );
		pnlNetworkOp.setBounds( 10, 8, 350, 174 );
		pnlNetworkOp.setBorder( new LineBorder( new Color( 0, 0, 0 ) ) );
		pnlNetworkOp.setBorder( new TitledBorder( null, "Network Options",  TitledBorder.LEADING,
			TitledBorder.TOP, JFlexPayDialog.getDefaultFont( ),
			null ) );
		pnlNetworkOp.setLayout( null );

		addTerminalPortTextFiled( );
		addServerPortTextField( );

		addServerIp1TextField( );
		addServerIp2TextField( );

		addDefaultPortButton( );
		addDefaulServerButton( );

		getContentPane( ).add( pnlNetworkOp );
	}	

	private void addCloseButton()
	{
		JButton btnClose = new JButton( "Close" );
		btnClose.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				closeDialog( );
			}
		} );
		btnClose.setBounds( 30, 193, 140, 30 );
		btnClose.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		getContentPane( ).add( btnClose );
	}

	private void addSaveButton()
	{
		JButton btnSave = new JButton( "Save" );
		btnSave.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				markDataForSaving( );
				closeDialog( );
			}
		} );
		btnSave.setBounds( 200, 193, 140, 30 );
		btnSave.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		getContentPane( ).add( btnSave );
	}
	
	private void addTerminalPortTextFiled()
	{
		JLabel lblTerminalPort = new JLabel( "Terminal Port:" );
		lblTerminalPort.setBounds( 10, 30, 141, 20 );
		getNetworkOpPanel( ).add( lblTerminalPort );

		textTerminalPort = new JTextField( );
		textTerminalPort.setColumns( 4 );
		textTerminalPort.setBounds( 147, 30, 60, 20 );
		textTerminalPort.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		textTerminalPort.addKeyListener( new KeyAdapter( )
		{
			@Override
			public void keyTyped(KeyEvent e)
			{
				char code = e.getKeyChar( );
				String text = ((JTextField) e.getSource( )).getText( );
				if ((code != KeyEvent.VK_BACK_SPACE && code != KeyEvent.VK_CLEAR && code != KeyEvent.VK_DELETE)
					&& (!Character.isDigit( code ) || e.isAltDown( ) || text.length( ) == MAX_PORT_LEN))
				{
					e.consume( );
					return;
				}
			}

			@Override
			public void keyReleased(KeyEvent e)
			{
				String text = ((JTextField) e.getSource( )).getText( );
				if (!EdtStringUtil.isStringNumeric( text ))
				{
					assert false : SHOULDNT_BE_HERE;
					((JTextField) e.getSource( )).setText( "" );
					return;
				}

				if (text.length( ) > MAX_PORT_LEN)
				{
					((JTextField) e.getSource( )).setText( text.substring( 0, 10 ) );
				}
			}
		} );

		getNetworkOpPanel( ).add( textTerminalPort );
	}	

	private void addDefaultPortButton()
	{
		btnSetDefaultTerminalPort = new JButton( "Set Default" );
		btnSetDefaultTerminalPort.setBounds( 220, 30, 100, 20 );
		btnSetDefaultTerminalPort.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		btnSetDefaultTerminalPort.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				getTerminalPortField( ).setText( Integer.toString( TokenManagerModel.getDefaultTerminalPort( ) ) );
			}
		} );
		getNetworkOpPanel( ).add( btnSetDefaultTerminalPort );
	}

	private void addServerPortTextField()
	{
		JLabel lblTokenServerport = new JLabel( "Token ServerPort:" );
		lblTokenServerport.setBounds( 10, 60, 141, 20 );
		getNetworkOpPanel( ).add( lblTokenServerport );

		textServerPort = new JTextField( );
		textServerPort.setColumns( 4 );
		textServerPort.setBounds( 147, 60, 60, 20 );
		textServerPort.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		textServerPort.addKeyListener( new KeyAdapter( )
		{
			@Override
			public void keyTyped(KeyEvent e)
			{
				char code = e.getKeyChar( );
				String text = ((JTextField) e.getSource( )).getText( );
				if ((code != KeyEvent.VK_BACK_SPACE && code != KeyEvent.VK_CLEAR && code != KeyEvent.VK_DELETE)
					&& (!Character.isDigit( code ) || e.isAltDown( ) || text.length( ) == MAX_PORT_LEN))
				{
					e.consume( );
					return;
				}
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

				if (text.length( ) > MAX_PORT_LEN)
				{
					((JTextField) e.getSource( )).setText( text.substring( 0, 10 ) );
				}
			}
		} );

		getNetworkOpPanel( ).add( textServerPort );
	}

	private JTextField getTerminalPortField()
	{
		return textTerminalPort;
	}

	private JTextField getServerPortField()
	{
		return textServerPort;
	}

	private void addDefaulServerButton()
	{
		btnSetDefaultTokenServerPort = new JButton( "Set Default" );
		btnSetDefaultTokenServerPort.setBounds( 220, 60, 100, 20 );
		btnSetDefaultTokenServerPort.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		btnSetDefaultTokenServerPort.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				getServerPortField( ).setText( Integer.toString( TokenManagerModel.getDefaultTokenServerPort( ) ) );
			}
		} );
		this.getNetworkOpPanel( ).add( btnSetDefaultTokenServerPort );
	}

	private void addServerIp2TextField()
	{
		JLabel lblTokenServerIp2 = new JLabel( "Token Server IP 2:" );
		lblTokenServerIp2.setBounds( 10, 129, 141, 21 );
		getNetworkOpPanel( ).add( lblTokenServerIp2 );

		textServerIp2 = new JTextField( );
		textServerIp2.setColumns( 4 );
		textServerIp2.setBounds( 147, 127, 120, 23 );
		textServerIp2.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		textServerIp2.addKeyListener( new KeyAdapter( )
		{
			@Override
			public void keyTyped(KeyEvent e)
			{
				char code = e.getKeyChar( );
				if ((code != KeyEvent.VK_BACK_SPACE && code != KeyEvent.VK_CLEAR && code != KeyEvent.VK_DELETE) && (!Character.isDigit( code ))
					&& (code != '.'))
				{
					e.consume( );
					return;
				}
			}
		} );

		textServerIp2.addFocusListener( new FocusAdapter( )
		{
			String		oldtext		= "";
			IpValidator	ipValidator	= new IpValidator( );

			@Override
			public void focusLost(FocusEvent e)
			{
				String text = ((JTextField) e.getSource( )).getText( );
				if (!ipValidator.validate( text ))
				{
					assert false : SHOULDNT_BE_HERE;
					((JTextField) e.getSource( )).setText( oldtext );
					return;
				}
				oldtext = text;
			}
		} );

		getNetworkOpPanel( ).add( textServerIp2 );
	}

	private void addServerIp1TextField()
	{
		JLabel lblTokenServerIp1 = new JLabel( "Token Server IP 1:" );
		lblTokenServerIp1.setBounds( 10, 97, 141, 21 );
		getNetworkOpPanel( ).add( lblTokenServerIp1 );

		textServerIp1 = new JTextField( );
		textServerIp1.setColumns( 4 );
		textServerIp1.setBounds( 147, 95, 120, 23 );
		textServerIp1.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		textServerIp1.setText( TokenManagerModel.getDefaultTokenIp1( ) );
		textServerIp1.addKeyListener( new KeyAdapter( )
		{
			@Override
			public void keyTyped(KeyEvent e)
			{
				char code = e.getKeyChar( );
				if ((code != KeyEvent.VK_BACK_SPACE && code != KeyEvent.VK_CLEAR && code != KeyEvent.VK_DELETE) && (!Character.isDigit( code ))
					&& (code != '.'))
				{
					e.consume( );
					return;
				}
			}
		} );

		textServerIp1.addFocusListener( new FocusAdapter( )
		{
			String		oldtext		= "";
			IpValidator	ipValidator	= new IpValidator( );

			@Override
			public void focusLost(FocusEvent e)
			{
				String text = ((JTextField) e.getSource( )).getText( );
				if (!ipValidator.validate( text ))
				{
					assert false : SHOULDNT_BE_HERE;
					((JTextField) e.getSource( )).setText( oldtext );
					return;
				}
				oldtext = text;
			}
		} );
		getNetworkOpPanel( ).add( textServerIp1 );
	}

	private void setNotDefaulGraphicSettings()
	{
		btnSetDefaultTerminalPort.setFont( new Font( "Tahoma", Font.PLAIN, 11 ) );
		btnSetDefaultTokenServerPort.setFont( new Font( "Tahoma", Font.PLAIN, 11 ) );
	}
}
