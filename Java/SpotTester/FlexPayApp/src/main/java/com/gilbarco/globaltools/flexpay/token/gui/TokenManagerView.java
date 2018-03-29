/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.flexpay.token.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JToolBar;
import javax.swing.ListSelectionModel;
import javax.swing.SwingConstants;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.flexpay.model.FlexPayContext;
import com.gilbarco.globaltools.flexpay.token.controller.TokenController;
import com.gilbarco.globaltools.flexpay.token.controller.UnitsPerTokenController;
import com.gilbarco.globaltools.flexpay.token.domain.Token;
import com.gilbarco.globaltools.flexpay.token.domain.TokenState;
import com.gilbarco.globaltools.flexpay.token.domain.Unit;
import com.gilbarco.globaltools.flexpay.token.domain.UnitsPerToken;
import com.gilbarco.globaltools.flexpay.token.dto.DataTransferForTokenUnitTableRow;
import com.gilbarco.globaltools.flexpay.widgets.JFlexPayDialog;

public class TokenManagerView extends EdtAppView<TokenManagerController>
{
	// ******************************************************************
	// INNER TYPES.
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final long			serialVersionUID			= 1L;
	private static final transient Log	logger						= LogFactory.getLog( TokenManagerView.class );

	private static final String[]		astrColumnNames				= {"ID","Operator","PPN","Start Time","End Time","Token Code","Times","Duration","Status","" };

	public static final int				TABLE_COL_TOKEN_ID			= 0;
	public static final int				TABLE_COL_OPERATOR_ID		= 1;
	public static final int				TABLE_COL_PPN				= 2;
	public static final int				TABLE_COL_START_TIME		= 3;
	public static final int				TABLE_COL_END_TIME			= 4;
	public static final int				TABLE_COL_TOKEN_CODE		= 5;
	public static final int				TABLE_COL_SESSION_TIMES		= 6;
	public static final int				TABLE_COL_SESSION_DURATION	= 7;
	public static final int				TABLE_COL_STATUS			= 8;
	public static final int				TABLE_COL_UNIT_ID			= 9;
	public static final int				TABLE_COLUMNS				= 10;

	protected static final int			MIN_TECHNUM_LEN				= 3;
	protected static final int			MAX_TECHNUM_LEN				= 10;

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private DefaultTableModel			tableModel;

	private JButton						btnRequestToken;
	private JButton						btnStoreToken;
	private JButton						btnPresentToken;
	private JButton						btnClose;
	private JTable						tableTargetUnits;
	private JPanel						pnlUnits;
	private JScrollPane					scrollPane;
	private JMenu						appMenu;	
	private AdvancedOptionsDialog		advancedOptionsDialog;
	private StoreTokensDataDialog		storeTokensDataDialog;
	private GenerateCertificateDialog	generateCertificateDialog;
	private TokenRequestDialog			tokenRequestDialog;
	private JLabel						lblStatus;
	private List<String>				tokenRowToolTips;

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public TokenManagerView()
	{
		super( );

		addButtons( );
		addUnitsPanel( );
		addStatusLabel( );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface)
	// ******************************************************************
	@Override
	public void initialize()
	{
		setBackground( Color.WHITE );
		setLayout( null );
		loadTokenModelData( );
		appMenu = null;
	}

	@Override
	public JMenu getAppMenu()
	{
		if (appMenu == null)
		{
			appMenu = new JMenu( FlexPayContext.getLocaleText( "TokenManagementMenu" ) );
			this.controller.addMenuItems( appMenu );
			enableMenus( );
		}
		return appMenu;
	}

	@Override
	public String getAppName()
	{
		return FlexPayContext.getLocaleText( "TokenManagement" );
	}

	@Override
	public Icon getAppIcon()
	{
		return null;
	}

	@Override
	public boolean hasAppMenu()
	{
		return true;
	}

	@Override
	public void addStatusBarComponent(JToolBar toolBar)
	{
		toolBar.add( lblStatus );
	}

	@Override
	protected TokenManagerController createController()
	{
		return TokenManagerController.getInstance( this );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	/*
	 * @ Calling this method is mandatory when you leave the table loaded during all application life, and then you want to exit. Otherwise you'll get this exception while quitting:
	 * " Exception while removing reference: java.lang.InterruptedException "
	 */
	public void clearTable()
	{
		tableTargetUnits.removeAll( );
		tokenRowToolTips.clear( );
	}

	public Component getRequestTokenButton()
	{
		return this.btnRequestToken;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void deleteTargetUnit(boolean confirm, int idxRow)
	{
		if(idxRow < 0)
		{
			return;
		}
		
		if (confirm)
		{
			String message = new String( "" );
			message += "The selected Token in the table will be removed:\n";
			message += "\nTarget Unit: " + (String) tableModel.getValueAt( idxRow, TABLE_COL_PPN );
			message += "\nToken Code: " + (String) tableModel.getValueAt( idxRow, TABLE_COL_TOKEN_CODE );
			message += "\n\nDo you really want to delete it?";

			int irsp = showConfirmDialog( message, "Confirm Deletion" );
			if (irsp != JOptionPane.YES_OPTION)
			{
				return;
			}
		}

		int iTokenID = Integer.parseInt( (String) tableModel.getValueAt( idxRow, TABLE_COL_TOKEN_ID ) );
		int iUnitID = Integer.parseInt( (String) tableModel.getValueAt( idxRow, TABLE_COL_UNIT_ID ) );
		boolean blOk = UnitsPerTokenController.deleteUnitPerToken( iTokenID, iUnitID );
		if ( !blOk )
		{
			showErrorDialog( "Error deleting selected token" );
		}
		
		tableModel.removeRow( idxRow );
		tokenRowToolTips.remove( idxRow );
	}

	protected void storeTokensBundlesToUSB()
	{
		setStatus( "Saving data to flash drive..." );

		if (!storeTokensDataDialog( ) || !storeTokensToUSB( ))
		{
			if (isReleaseBundleNeeded( ))
			{
				cleanStatus( );
				return;
			}

			if (storeTokensDataDialog.getReleaseVersionSelected( ).isEmpty( ))
			{
				showErrorDialog( "Not found a destination drive." );
				return;
			}

		
			String strSourceVersions = TokenManagerController.getVersionsFolder( ) + storeTokensDataDialog.getReleaseVersionSelected( );
			if (!controller.storeReleaseBundle( strSourceVersions, 
				TokenManagerController.getUSBdrive( ) + TokenManagerController.getUsbFirmwaresPath( ),
				TokenManagerController.getUSBdrive( ) + TokenManagerController.getUsbApplicationsPath( ) ))
			{
				cleanStatus( );
				return;
			}
			else
			{
				showErrorDialog( "Error creating/storing release bundle in the USB" );
			}
		}

		cleanStatus( );
		showMessageDialog( FlexPayContext.getLocaleText( "TokenStoreComplete" ) );
	}

	

	 /**
     * Determine if the selected tokens have permissions to update software unit using a release bundle. <p>
     * @return true if selected tokens have permissions, false otherwise.
     */	
	private boolean isReleaseBundleNeeded()
	{
		int aiSelectedRows[] = tableTargetUnits.getSelectedRows( );
		for (int i = 0; i < aiSelectedRows.length; i++)
		{
			String tokencode = (String) tableTargetUnits.getValueAt( aiSelectedRows[i], TABLE_COL_TOKEN_CODE );
			Token token = Token.getFromDatabase( tokencode );
			if( Token.hasSoftwareUpdatesPermissions(token) )
			{
				return true;
			}
		}
		return false;
	}

	protected boolean storeTokensToUSB()
	{
		if (storeTokensDataDialog.getUSBPathSelected( ).isEmpty( ))
		{
			showErrorDialog( "Not found a destination drive." );
			return false;
		}

		int nrows = tableTargetUnits.getSelectedRowCount( );
		if (nrows <= 0)
		{
			showMessageDialog( FlexPayContext.getLocaleText( "NoTokenSelected" ) );
			return false;
		}

		int nSaved = 0;
		String aSavedTokens[] = new String[nrows];
		int aiSelectedRows[] = tableTargetUnits.getSelectedRows( );
		boolean bErrSaving = false;
		for (int i = 0; i < nrows; i++)
		{
			String sTokenCode = (String) tableTargetUnits.getValueAt( aiSelectedRows[i], TABLE_COL_TOKEN_CODE );
			String sTokenPPN = (String) tableTargetUnits.getValueAt( aiSelectedRows[i], TABLE_COL_PPN );			

			if (!wasTokenAlreadySaved( sTokenCode, nSaved, aSavedTokens ))
			{
				if (TokenController.saveToken( sTokenCode, sTokenPPN ))
				{
					tableTargetUnits.setValueAt( TokenState.STORED.name( ), aiSelectedRows[i], TABLE_COL_STATUS );
					int iTokenID = Integer.parseInt( (String) tableTargetUnits.getValueAt( aiSelectedRows[i], TABLE_COL_TOKEN_ID ) );
					int iUnitID = Integer.parseInt( (String) tableTargetUnits.getValueAt( aiSelectedRows[i], TABLE_COL_UNIT_ID ) );
					UnitsPerToken ut = UnitsPerToken.getFromDatabase( iTokenID, iUnitID );
					ut.setTokenState( TokenState.STORED );

					aSavedTokens[nSaved++] = sTokenCode; // mark the token as saved
				}
				else
				{
					bErrSaving = true;
				}
			}
		}

		if (bErrSaving)
		{
			showMessageDialog( FlexPayContext.getLocaleText( "TokenStoreFailed" ) + " -" + FlexPayContext.getLocaleText( "LogGenerated" ) );
			return false;
		}

		return true;
	}

	private boolean wasTokenAlreadySaved(String sTokenCode, int nSaved, String aSavedTokens[])
	{
		if (nSaved == 0) return false; // no tokens saved yet

		for (int i = 0; i < nSaved; i++)
		{
			if (aSavedTokens[i].equalsIgnoreCase( sTokenCode ))
			{
				return true;
			}
		}

		return false;
	}

	private void addStatusLabel()
	{
		lblStatus = new JLabel( "" );
		lblStatus.setFont( JFlexPayDialog.getDefaultStatusFont( ) );
	}

	public void cleanStatus()
	{
		lblStatus.setText( "" );
	}

	protected void disableMenus()
	{
		controller.disableMenus( );
	}

	/**
	 * @param status
	 *            the status to show at status bar
	 */
	protected void setStatus(String status)
	{
		status = "  --  Status: " + status;
		lblStatus.setText( status );
	}

	protected void enableButtons()
	{
		boolean bOneTokenSelected = (tableTargetUnits.getSelectedRowCount( ) == 1);

		btnRequestToken.setEnabled( true );
		btnStoreToken.setEnabled( bOneTokenSelected );
		btnPresentToken.setEnabled( bOneTokenSelected );
	}
	
	private void disableButtons()
	{
		btnRequestToken.setEnabled( false );
		btnStoreToken.setEnabled( false );
		btnPresentToken.setEnabled( false );
	}

	protected void advancedOptionsDialog()
	{
		if (this.advancedOptionsDialog == null)
		{
			advancedOptionsDialog = new AdvancedOptionsDialog( );
			advancedOptionsDialog.setLocationRelativeTo( null );
			advancedOptionsDialog.setAlwaysOnTop( true );
			advancedOptionsDialog.setModal( true );
		}
		advancedOptionsDialog.setVisible( true );
	}

	protected void generateCertificateDialog()
	{
		if (this.generateCertificateDialog == null)
		{
			generateCertificateDialog = new GenerateCertificateDialog( this );
			generateCertificateDialog.setLocationRelativeTo( null );
			generateCertificateDialog.setAlwaysOnTop( true );
			generateCertificateDialog.setModal( true );
		}
		generateCertificateDialog.setVisible( true );
	}

	protected boolean storeTokensDataDialog()
	{
		if (this.storeTokensDataDialog == null)
		{
			storeTokensDataDialog = new StoreTokensDataDialog(  );
			storeTokensDataDialog.setLocationRelativeTo( null );
			storeTokensDataDialog.setAlwaysOnTop( true );
			storeTokensDataDialog.setModal( true );
		}
		storeTokensDataDialog.setNeedReleaseBundle( isReleaseBundleNeeded() );
		storeTokensDataDialog.setVisible( true );
		return storeTokensDataDialog.dataSelected( );
	}

	private JTable createUnitsTable()
	{
		String[][] initVoidData = {};
		tableModel = new DefaultTableModel( initVoidData, astrColumnNames );

		JTable tblTU = new JTable( tableModel )
		{
			private static final long	serialVersionUID	= 2485117672771964339L;
			DefaultTableCellRenderer	renderCenter		= new DefaultTableCellRenderer( );
			{
				renderCenter.setHorizontalAlignment( SwingConstants.CENTER );
				renderCenter.setVerticalAlignment( SwingConstants.CENTER );
			}

			// Implement table cell tool tips.
			public String getToolTipText(MouseEvent e)
			{
				java.awt.Point p = e.getPoint( );
				int rowIndex = rowAtPoint( p );
				String tip = tokenRowToolTips.get( rowIndex );
				if (tip != null && tip.length( ) > 0)
				{
					tip = "<html> Permissions:<br> " + tip.replace( "\n", "<br> " ) + "</html>";
				}
				return tip;
			}

			@Override
			public boolean isCellEditable(int row, int column)
			{
				return false;
			}

			@Override
			public TableCellRenderer getCellRenderer(int arg0, int arg1)
			{
				return renderCenter;
			}
		};

		tblTU.addMouseListener( new MouseAdapter( )
		{
			@Override
			public void mouseClicked(MouseEvent e)
			{
			}
		} );

		tblTU.getSelectionModel( ).addListSelectionListener( new ListSelectionListener( )
		{
			@Override
			public void valueChanged(ListSelectionEvent arg0)
			{
				enableActions( );
			}
		} );

		tblTU.getTableHeader( ).setFont( JFlexPayDialog.getDefaultSmallFont( ) );
		tblTU.getTableHeader( ).setBackground( JFlexPayDialog.getPanelDefaultBackgroundColor( ) );
		tblTU.setFont( JFlexPayDialog.getDefaultSmallFont( ) );
		tblTU.setBackground( JFlexPayDialog.getPanelDefaultBackgroundColor( ) );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_TOKEN_ID ).setPreferredWidth( 0 );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_OPERATOR_ID ).setPreferredWidth( 55 );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_PPN ).setPreferredWidth( 50 );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_START_TIME ).setPreferredWidth( 92 );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_END_TIME ).setPreferredWidth( 92 );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_TOKEN_CODE ).setPreferredWidth( 245 );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_SESSION_TIMES ).setPreferredWidth( 40 );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_SESSION_DURATION ).setPreferredWidth( 45 );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_STATUS ).setPreferredWidth( 58 );
		tblTU.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		
		// avoid unitID field being visible.
		TableColumn tc = tblTU.getColumnModel( ).getColumn( TABLE_COL_UNIT_ID );
		tc.setMaxWidth( 0 );
		tc.setMinWidth( 0 );
		tc.setPreferredWidth( 0 );
		tc.setResizable( false );

		tc = tblTU.getColumnModel( ).getColumn( TABLE_COL_TOKEN_ID );
		tc.setMaxWidth( 0 );
		tc.setMinWidth( 0 );
		tc.setPreferredWidth( 0 );
		tc.setResizable( false );

		return tblTU;
	}

	protected void enableMenus()
	{
		controller.updateMenus( tableTargetUnits.getSelectedRowCount( ) );
	}

	/**
	 * load the existing token data from memory database
	 */
	private void loadTokenModelData()
	{
		clearTable( );
		Collection<Map<Integer, UnitsPerToken>> tokens = UnitsPerTokenController.getUnitsPerTokens( );
		DataTransferForTokenUnitTableRow dttu = new DataTransferForTokenUnitTableRow( );

		Iterator<Map<Integer, UnitsPerToken>> iter = tokens.iterator( );
		while (iter.hasNext( ))
		{
			Map<Integer, UnitsPerToken> t = iter.next( );
			Iterator<UnitsPerToken> tk = t.values( ).iterator( );
			while (tk.hasNext( ))
			{
				UnitsPerToken element = tk.next( );
				int unitid = element.getIdUnit( );
				int tokenid = element.getTokenID( );
				Token token = Token.getFromDatabase( tokenid );
				Unit unit = Unit.getFromDatabase( unitid );

				dttu.setTokenVersion( token.getTokenVersion( ) );
				dttu.setTokenCode( token.getTokenCode( ) );
				dttu.setTargetUnit( unit.getPPN( ) );
				dttu.setStartTime( token.getStartTime( ) );
				dttu.setEndTime( token.getEndTime( ) );
				dttu.setRemoteMaintenance( token.getRemoteMaintenance( ) );
				dttu.setOperatorID( token.getOperatorID( ) );
				dttu.setPermissions( token.getPermissions( ) );
				dttu.setTechinicianDevice( token.getTechnicianDevice( ) );
				dttu.setTargetUnit( unit.getPPN( ) );
				dttu.setMaxDuration( token.getMaxDuration( ) );
				dttu.setTimes( token.getTimes( ) );

				dttu.setTokenState( UnitsPerToken.getFromDatabase( tokenid, unitid ).getTokenState( ).name( ) );
				tableModel.addRow( dttu.getDataForTokenManagerViewTable( ) );
				tokenRowToolTips.add( dttu.getPermissions( ) );
			}
		}
	}

	private void addUnitsPanel()
	{
		pnlUnits = new JPanel( );
		pnlUnits.setBounds( 15, 10, 770, 405 );
		pnlUnits.setBorder( new LineBorder( new Color( 0, 0, 0 ) ) );
		pnlUnits.setBorder( new TitledBorder( null, "Tokens", TitledBorder.LEADING, TitledBorder.TOP, JFlexPayDialog.getDefaultFont( ), null ) );
		pnlUnits.setLayout( null );
		add( pnlUnits );

		tableTargetUnits = createUnitsTable( );
		scrollPane = new JScrollPane( tableTargetUnits );
		scrollPane.setBounds( 20, 20, 730, 370 );
		pnlUnits.add( scrollPane, BorderLayout.NORTH );

		tokenRowToolTips = new ArrayList<>( );
	}

	private void addButtons()
	{
		btnRequestToken = new JButton( "Request" );
		btnRequestToken.setBounds( 10, 423, 180, 35 );
		btnRequestToken.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		btnRequestToken.setFont( JFlexPayDialog.getDefaultFont( ) );
		btnRequestToken.setEnabled( true );
		btnRequestToken.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				setStatus( FlexPayContext.getLocaleText( "Connecting" ) + "..." );
				disableActions( );
				requestToken( );
				cleanStatus( );
				enableActions( );				
			}
		} );
		add( btnRequestToken );

		btnPresentToken = new JButton( "Present" );
		btnPresentToken.setBounds( 200, 423, 110, 35 );
		btnPresentToken.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		btnPresentToken.setFont( JFlexPayDialog.getDefaultFont( ) );
		btnPresentToken.setEnabled( false );
		btnPresentToken.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				setStatus( FlexPayContext.getLocaleText( "Connecting" ) + "..." );
				disableActions( );
				presentToken( );
				cleanStatus( );
				enableActions( );				
			}
		} );
		add( btnPresentToken );

		btnStoreToken = new JButton( "Store" );
		btnStoreToken.setBounds( 320, 423, 110, 35 );
		btnStoreToken.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		btnStoreToken.setFont( JFlexPayDialog.getDefaultFont( ) );
		btnStoreToken.setEnabled( false );
		btnStoreToken.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				storeTokensBundlesToUSB( );
			}
		} );
		add( btnStoreToken );

		btnClose = new JButton( "Close" );
		btnClose.setBounds( 680, 423, 100, 35 );
		btnClose.setFont( JFlexPayDialog.getDefaultFont( ) );
		btnClose.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				controller.closedAuthenticationManagerNotification();
				getRootApp( ).getController( ).closeApp( (TokenManagerView) btnClose.getParent( ) );
				getRootApp( ).getController( ).setSelectedIndex( 1 );
			}
		} );
		btnClose.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		add( btnClose );
	}

	protected void requestToken()
	{
		if (this.tokenRequestDialog == null) // dialog never instanced
		{
			tokenRequestDialog = new TokenRequestDialog( );
			tokenRequestDialog.setLocationRelativeTo( null );
			tokenRequestDialog.setAlwaysOnTop( true );
			tokenRequestDialog.setModal( true );
		}

		tokenRequestDialog.clearTokenRequested( );
		tokenRequestDialog.enableRequest( );

		// Finally here the control is transfered to the addTargetUnitDialog dialog
		tokenRequestDialog.setVisible( true );
		if (tokenRequestDialog.certificateInfoRequested( ))
		{
			controller.requestToken( tokenRequestDialog.getTokenCode( ) );
		}

		tokenRequestDialog.dispose( );
	}
	
	protected void presentToken()
	{
		int idxRow = tableTargetUnits.getSelectedRow( );

		if (idxRow < 0) return;

		String message = new String("");
		message += "The selected Token in the table will be presented to the SPOT.\n";
		message += "If you really want to present it, please enter the IP address of the SPOT.\n"; 

		String sIPAddress = showInputDialog( message );
		
		if( sIPAddress != null && !sIPAddress.isEmpty( ) )
		{
			int iTokenID = Integer.parseInt( (String) tableModel.getValueAt( idxRow, TABLE_COL_TOKEN_ID ) );
			Token token = Token.getFromDatabase( iTokenID );
			
			if( token != null )
			{
				controller.presentToken( token.getXmlBytes( ), sIPAddress );
			}
			else
			{
				showErrorDialog( "Couldn't present the token pointed. There is an internal error." );
			}
		}
	}

	public void disableActions()
	{
		disableButtons( );
		disableMenus( );
		tableTargetUnits.setEnabled( false );
	}

	public void showSasErrorInfo(String message)
	{
		JOptionPane pane = new JOptionPane( message, JOptionPane.ERROR_MESSAGE );
		JDialog dialog = pane.createDialog( pnlUnits, FlexPayContext.getLocaleText( "Error" ) );
		dialog.setAlwaysOnTop( true );
		dialog.setVisible( true );
		cleanStatus( );
		enableActions( );
	}

	public void enableActions()
	{
		enableButtons( );
		enableMenus( );
		tableTargetUnits.setEnabled( true );
	}

	public void addTokenRequested( String tokencode, String startTime, String endTime, String targetUnit,
								   String duration, String times, String remoteMaintenance, String permissions, String techincianID,
								   String techinicianDevice, String operatorID, String version, byte[] xmlBytes )
	{
		cleanStatus( );
		enableActions( );
		try
		{
			DataTransferForTokenUnitTableRow dttu = new DataTransferForTokenUnitTableRow( );
			dttu.setTokenVersion( version );
			dttu.setTokenCode( tokencode );
			dttu.setTargetUnit( targetUnit );
			dttu.setStartTime( startTime );
			dttu.setEndTime( endTime );
			dttu.setTimes( times );
			dttu.setMaxDuration( duration );
			dttu.setTokenState( TokenState.REQUESTED.name( ) );
			dttu.setPermissions( permissions );
			dttu.setRemoteMaintenance( remoteMaintenance );
			dttu.setTechinicianDevice( techinicianDevice );
			dttu.setOperatorID( operatorID );

			// Add unit per token - new token
			UnitsPerTokenController.addUnitAndToken( dttu, xmlBytes );
			tableModel.addRow( dttu.getDataForTokenManagerViewTable( ) );
			tokenRowToolTips.add( dttu.getPermissions( ) );
		}
		catch (Exception e)
		{
			String message = new String("");
			message += FlexPayContext.getLocaleText( "CannotCreateToken" ) + "\n";
			message += FlexPayContext.getLocaleText( "Error" ) + "\n";
			
			showErrorDialog(message);
		}
	}

	/**
	 * @param validationCode
	 *            the validationCode of the certificate published
	 */
	public void showPublishCertificateOk(String validationCode)
	{
		cleanStatus( );
		enableActions( );

		String message = new String("");
		message += FlexPayContext.getLocaleText( "PublishCertificateSuccesfully" ) + "\n"; 
		message += FlexPayContext.getLocaleText( "CallGilbarcoMessage" ) + "\n";
		message += "  - " + FlexPayContext.getLocaleText( "CertificateValidationCode" ) + ": " + validationCode + "\n";
		message += "  - " + FlexPayContext.getLocaleText( "TechnicianID" ) + ":" + TokenManagerModel.getTechnicianID( ) + "\n";
		
		showMessageDialog( message );
		logger.info( "The publish certificate validation code is: [" + validationCode + "]" );
	}
	

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************	
	private String showInputDialog(String message)
	{
		JOptionPane pane = new JOptionPane( message, JOptionPane.PLAIN_MESSAGE, JOptionPane.OK_CANCEL_OPTION, null, null, null );

		pane.setWantsInput( true );
		pane.setSelectionValues( null );
		pane.setInitialSelectionValue( controller.getCurrentTerminalIP());  //"192.168.67.234"

		JDialog dialog = pane.createDialog( "Token Management" );
		dialog.setAlwaysOnTop( true );
		pane.selectInitialValue( );
		dialog.setVisible( true );

		Object value = pane.getInputValue( );
		if (value == JOptionPane.UNINITIALIZED_VALUE)
		{
			return null;
		}
		return (String) value;
	}

	public void showMessageDialog(String message)
	{
		JOptionPane pane = new JOptionPane( message, JOptionPane.INFORMATION_MESSAGE );
		JDialog dialog = pane.createDialog( "Message" );
		dialog.setAlwaysOnTop( true );
		dialog.setVisible( true );
	}

	public void showErrorDialog(String message)
	{
		JOptionPane pane = new JOptionPane( message, JOptionPane.ERROR_MESSAGE );
		JDialog dialog = pane.createDialog( "Error" );
		dialog.setAlwaysOnTop( true );
		dialog.setVisible( true );
	}
	
	public int showConfirmDialog(String message, String title) 
	{
	   JOptionPane pane = new JOptionPane(message, JOptionPane.QUESTION_MESSAGE, JOptionPane.YES_NO_OPTION, getAppIcon( ));
	   JDialog dialog = pane.createDialog(title);
	   dialog.setAlwaysOnTop(true);
	   dialog.setVisible(true);
	   
	   Object selectedValue = pane.getValue();
	   if( selectedValue instanceof Integer )
	   {
		   return ((Integer)selectedValue).intValue();
	   }

       return JOptionPane.NO_OPTION;
	}
	
	public int getSelectedRow()
	{
		return tableTargetUnits.getSelectedRow( );
	}

	public int getLastRowAdded()
	{
		int idxRow = tableTargetUnits.getRowCount( ) - 1;// get the last created token
		if (idxRow < 0 || !tableModel.getValueAt( idxRow, TABLE_COL_STATUS ).equals( TokenState.VOID.name( ) ))
		{
			return -1;
		}
			
		return idxRow;
	}

}
