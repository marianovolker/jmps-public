
package com.gilbarco.globaltools.flexpay.main.gui;

import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.Color;
import java.awt.Cursor;

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
import javax.swing.UIManager;
import javax.swing.WindowConstants;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;
import com.gilbarco.globaltools.flexpay.model.FlexPayContext;

public class FlexPayView extends EdtAppView<FlexPayController>
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final long			serialVersionUID		= 1L;

	protected static final int			MAX_TECHNUM_LEN			= 10;
	protected static final int			MIN_TECHNUM_LEN			= 3;

	protected static final Color		DEFAULT_BUTTON_COLOR	= new Color( 205, 217, 233 );

	/** The Constant DEFAULT_EDITABLE_COLOR. */
	private static final Color	DEFAULT_EDITABLE_COLOR			= new Color( 213, 223, 234 );
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
	private JLabel						lblGilbarcoImg;
	private JLabel						lblTechincianNumber;
	private JTextField					technicianNumber;
	private JButton						btnMaintenance;
	private JButton						btnRKL;

	private JButton						btnAuthenticationManager;

	private JButton						btnExit;
	private JLabel						lblStatusBar;

	private static FlexPayAppConfig		_appconfig;
	private static final transient Log	logger					= LogFactory.getLog( FlexPayView.class );

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public FlexPayView()
	{
		super( );
		setFocusable( false );
		setFocusTraversalKeysEnabled( false );

		setBackground( FlexPayComponentFormatter.getPanelDefaultBackgroundColor( ) );
		setLayout( null );

		// Gilbarco Image
		addGilbarcoLogo( );
		// Technician Number
		addTechnicianNumberField( );
		// Maintenance Button
		addMaintenanceButton( );
		// Remote Key Load Button
		addRKLButton( );
		// Authentication Manager Button
		addAuthenticationManagerButton( );
		// Exit Button
		addExitButton( );
		// Copyright Label in status bar
		addCopyrightGilbarco( );
		// Set the main panel as the required specification
		setRootAppProperties( );
		// Apply default application setting
		formatView( );

		logger.info( getAppName( ) + " Started." );
	}

	public FlexPayAppConfig getFlexPayAppConfig()
	{
		if (_appconfig == null)
		{
			createFlexPayAppConfig( );
		}

		return _appconfig;
	}

	private void createFlexPayAppConfig()
	{
		if (this.getRootApp( ) != null)
		{
			if (this.getRootApp( ).getAppProperties( ) != null)
			{
				_appconfig = FlexPayAppConfig.getInstance( this.getRootApp( ).getAppProperties( ) );
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
		_appconfig = this.getFlexPayAppConfig( );
	}

	@Override
	public JMenu getAppMenu()
	{
		return null;
	}

	@Override
	public String getAppName()
	{
		return controller.getTitleAppName();
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
	protected FlexPayController createController()
	{
		return new FlexPayController( this );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	public void formatView()
	{
		
        UIManager.put("Panel.background",Color.white);

        UIManager.put("Button.background",DEFAULT_BUTTON_COLOR);
        UIManager.put("Button.border",BorderFactory.createLineBorder(DEFAULT_BUTTON_COLOR,4));
        UIManager.put("Button.font",new Font("Tahoma",Font.PLAIN,13));
        
        UIManager.put("TextField.border",BorderFactory.createLineBorder(UIManager.getColor("TextField.shadow"),1));
        
        UIManager.put("ScrollPane.border",BorderFactory.createLineBorder(UIManager.getColor("TextField.shadow"),1));
        UIManager.put("ScrollPane.font",new Font("Tahoma",Font.PLAIN,11));

        UIManager.put("ScrollBarUI","javax.swing.plaf.basic.BasicScrollBarUI");
        UIManager.put("ScrollBar.width",17);
        UIManager.put("ScrollBar.thumb",DEFAULT_BUTTON_COLOR);
        UIManager.put("ScrollBar.thumbHighlight",DEFAULT_BUTTON_COLOR);
        UIManager.put("ScrollBar.thumbShadow",DEFAULT_BUTTON_COLOR);
        
        UIManager.put("Label.font",new Font("Tahoma",Font.PLAIN,11));
        
        UIManager.put("ToolTip.font",new Font("Tahoma",Font.PLAIN,11));
        UIManager.put("ToolTip.border",BorderFactory.createLineBorder(DEFAULT_BUTTON_COLOR));
        UIManager.put("ToolTip.background",Color.white);

	    UIManager.put("OptionPane.yesButtonText","  "+FlexPayContext.getLocaleText("Yes")+"  ");
	    UIManager.put("OptionPane.noButtonText","  "+FlexPayContext.getLocaleText("No")+"  ");
		UIManager.put("OptionPane.okButtonText","  "+FlexPayContext.getLocaleText("Ok")+"  ");
	    UIManager.put("OptionPane.cancelButtonText","  "+FlexPayContext.getLocaleText("Cancel")+"  ");

	    UIManager.put("OptionPane.background",Color.white);
	    UIManager.put("Panel.background",Color.white);
	    UIManager.put("OptionPane.messageFont",new Font("Tahoma",Font.PLAIN,12)); 
	    UIManager.put("OptionPane.buttonFont",new Font("Tahoma",Font.PLAIN,12));

        UIManager.put("FileChooser.usesSingleFilePane",true);
        UIManager.put("FileChooser.listViewWindowsStyle",true);
        UIManager.put("FileChooser.listFont",new Font("Tahoma",Font.PLAIN,11));
        UIManager.put("FileChooser.border",BorderFactory.createLineBorder(DEFAULT_BUTTON_COLOR));
        UIManager.put("FileChooser.openButtonText","  "+"Choose"+"  ");
        UIManager.put("FileChooser.cancelButtonText","  "+FlexPayContext.getLocaleText("Cancel")+"  ");
        
        UIManager.put("ToggleButton.background",DEFAULT_BUTTON_COLOR);
        UIManager.put("ToggleButton.border",BorderFactory.createLineBorder(DEFAULT_BUTTON_COLOR,4));
        
        UIManager.put("TextField.font",new Font("Tahoma",Font.PLAIN,11));

        UIManager.put("ComboBoxUI","javax.swing.plaf.basic.BasicComboBoxUI");
        UIManager.put("ComboBox.font",new Font("Tahoma",Font.PLAIN,11));
        UIManager.put("ComboBox.background",Color.white);
        UIManager.put("ComboBox.buttonBackground",DEFAULT_BUTTON_COLOR);
        UIManager.put("ComboBox.border",BorderFactory.createLineBorder(UIManager.getColor("ComboBox.buttonShadow"),1));

        FlexPayComponentFormatter.setPanelDefaultBackgroundColor(this);
        FlexPayComponentFormatter.setDefaultFont(this);
}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	public JTextField getTechnicianNumberField()
	{
		return this.technicianNumber;
	}

	public JButton getAuthenticationManagerButton()
	{
		return this.btnAuthenticationManager;
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	private void addGilbarcoLogo()
	{
		lblGilbarcoImg = new JLabel( new ImageIcon( getClass( ).getResource( "/images/gilbarco_top.jpg" ) ) );
		lblGilbarcoImg.setFocusable( false );
		lblGilbarcoImg.setFocusTraversalKeysEnabled( false );
		lblGilbarcoImg.setBounds( 214, 24, 286, 100 );
		add( lblGilbarcoImg );
	}

	private void addTechnicianNumberField()
	{
		lblTechincianNumber = new JLabel( FlexPayContext.getLocaleText( "TechnicianNumber" ) );
		lblTechincianNumber.setFocusable( false );
		lblTechincianNumber.setFocusTraversalKeysEnabled( false );
		lblTechincianNumber.setBounds( 234, 129, 136, 30 );
		add( lblTechincianNumber );

		technicianNumber = new JTextField( );
		technicianNumber.setToolTipText( FlexPayContext.getLocaleText( "TechnicianNumberToolTip" ) );
		technicianNumber.addKeyListener( new KeyAdapter( )
		{
			@Override
			public void keyTyped(KeyEvent e)
			{
				char code = e.getKeyChar( );
				String text = ((JTextField) e.getSource( )).getText( );
				if ((code != KeyEvent.VK_BACK_SPACE && code != KeyEvent.VK_CLEAR && code != KeyEvent.VK_DELETE)
					&& (!Character.isDigit( code ) || e.isAltDown( ) || text.length( ) == MAX_TECHNUM_LEN))
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
					btnMaintenance.setEnabled( false );
					btnRKL.setEnabled( false );

					if (btnAuthenticationManager != null)
					{
						btnAuthenticationManager.setEnabled( false );
					}
					return;
				}

				if (text.length( ) > MAX_TECHNUM_LEN)
				{
					((JTextField) e.getSource( )).setText( text.substring( 0, MAX_TECHNUM_LEN - 1 ) );
				}

				boolean enableBtn = text.length( ) >= MIN_TECHNUM_LEN;
				btnMaintenance.setEnabled( enableBtn );
/*TODO: Commented as this feature will not be enabled any time soon.
				btnRKL.setEnabled( enableBtn );
*/

				if (btnAuthenticationManager != null)
				{
					btnAuthenticationManager.setEnabled( enableBtn );
				}
			}
		} );

		technicianNumber.setBounds( 360, 127, 100, 35 );
		technicianNumber.setBorder( null );
		technicianNumber.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		//technicianNumber.setBorder( BorderFactory.createBevelBorder( BevelBorder.RAISED, Color.GRAY, Color.LIGHT_GRAY ) );
		technicianNumber.setBackground( DEFAULT_EDITABLE_COLOR );

		add( technicianNumber );
		technicianNumber.setColumns( MAX_TECHNUM_LEN );
	}

	private void addMaintenanceButton()
	{
		btnMaintenance = new JButton( FlexPayContext.getLocaleText( "Maintenance" ) );
		btnMaintenance.setToolTipText( FlexPayContext.getLocaleText( "MaintenanceToolTip" ) );
		btnMaintenance.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnMaintenance.setEnabled( false );
		btnMaintenance.addActionListener( new ActionListener( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				getRootApp( ).setAlwaysOnTop( false );
				controller.launchFlexPayMaintenanceView( getRootApp( ), technicianNumber.getText( ), getAppName( ) );
				getRootApp( ).setAlwaysOnTop( true );
			}
		} );

		if (controller.hasAuthenticationManagerView())
		{
			btnMaintenance.setBounds( 163, 217, 120, 50 );
		}
		else
		{
			btnMaintenance.setBounds( 214, 217, 120, 50 );
		}
		btnMaintenance.setBorderPainted( false );
		btnMaintenance.setBackground( DEFAULT_BUTTON_COLOR );
		add( btnMaintenance );
	}

	private void addRKLButton()
	{
		btnRKL = new JButton( FlexPayContext.getLocaleText( "RKL" ) );
		btnRKL.setToolTipText( FlexPayContext.getLocaleText( "RKLToolTip" ) );
		btnRKL.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnRKL.setEnabled( false );
		btnRKL.addActionListener( new ActionListener( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				getRootApp( ).setAlwaysOnTop( false );
				controller.launchRKLView( getRootApp( ), technicianNumber.getText( ), getAppName( ) );
				getRootApp( ).setAlwaysOnTop( true );
			}
		} );

		if (controller.hasAuthenticationManagerView())
		{
			btnRKL.setBounds( 433, 217, 120, 50 );
		}
		else
		{
			btnRKL.setBounds( 380, 217, 120, 50 );
		}
		btnRKL.setBorderPainted( false );
		btnRKL.setBackground( DEFAULT_BUTTON_COLOR );
		add( btnRKL );
	}

    private void addAuthenticationManagerButton()
	{
		if (controller.hasAuthenticationManagerView())
		{
			btnAuthenticationManager = new JButton( FlexPayContext.getLocaleText( "AuthenticationManagement" ) );
			btnAuthenticationManager.setToolTipText(FlexPayContext.getLocaleText( "AuthenticationManagementToolTip" ));
			btnAuthenticationManager.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
			btnAuthenticationManager.setEnabled( false );
			btnAuthenticationManager.addActionListener( new ActionListener( )
			{
				@Override
				public void actionPerformed(ActionEvent e)
				{
					getRootApp( ).setAlwaysOnTop( false );
					controller.launchAuthenticationManagerView( getRootApp( ), technicianNumber.getText( ), getAppName( ) );
					getRootApp( ).setAlwaysOnTop( true );
				}
			} );
			btnAuthenticationManager.setBounds( 293, 217, 130, 50 );
			btnAuthenticationManager.setBorderPainted( false );
			btnAuthenticationManager.setBackground( DEFAULT_BUTTON_COLOR );
			add( btnAuthenticationManager );
		}
	}

	@SuppressWarnings("serial")
	private void addExitButton()
	{
		btnExit = new JButton( FlexPayContext.getLocaleText( "Exit" ) );
		btnExit.setToolTipText( FlexPayContext.getLocaleText( "ExitToolTip" ) );
		btnExit.setCursor( Cursor.getPredefinedCursor( Cursor.HAND_CURSOR ) );
		btnExit.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				getAppController( ).exitApplication( 0 );
			}
		} );
		btnExit.setBounds( 298, 311, 120, 50 );
		btnExit.setBorderPainted( false );
		btnExit.setBackground( DEFAULT_BUTTON_COLOR );
		add( btnExit );

		this.getActionMap( ).put( "ON_EXIT", new AbstractAction( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				onExit( );
			}
		} );
		this.getInputMap( JComponent.WHEN_IN_FOCUSED_WINDOW )
						.put( KeyStroke.getKeyStroke( "ESCAPE" ), "ON_EXIT" );
	}

	private void setRootAppProperties()
	{
		if (getRootApp( ) != null)
		{
			// no default main panel size for flexpay application
			getRootApp( ).setMainPanelSize( 730, 450 );
			// no default main panel title for flexpay application
			getRootApp( ).setTitle( getAppName( ) );
			// always on top for flexpay application
			getRootApp( ).setAlwaysOnTop( true );
			// no menu bar on flexpay application
			getRootApp( ).getJMenuBar( ).removeAll( );

			// no tab bar on flexpay application
			// commented to enable tabs again to the application
			// getRootApp( ).setAppTabContainer( new CardLayout( ) );

			// not re-sizable
			getRootApp( ).setResizable( false );

			// not menubar and minimize button
			// setNotMenuMinimizeButton( ); //disabled because menubar is requiered!

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
					onExit( );
				}
			} );
		}
	}

	private void addCopyrightGilbarco()
	{
		lblStatusBar = new JLabel( FlexPayContext.getLocaleText( "CopyRightGilbarco" ) );

		// Default application status bar font
		Font fds = FlexPayComponentFormatter.getDefaultStatusFont( );

		lblStatusBar.setFont( new Font( fds.getFamily( ), Font.BOLD, fds.getSize( ) ) );
	}

	private void onExit()
	{
		controller.exitApplication( 0 );
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************
}
