
package com.gilbarco.globaltools.flexpay.token.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingConstants;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellRenderer;

import com.gilbarco.globaltools.flexpay.token.dto.DataTransferForTokenCodeTableRow;
import com.gilbarco.globaltools.flexpay.widgets.JFlexPayDialog;

/**
 * @author mvolker
 * 
 */
/* TODO: TOKEN_COMMENTED_CODE
This class is never used as token management was removed.
*/
public class SelectTokenCodeDialog extends JFlexPayDialog
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final long			serialVersionUID				= 1L;

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC STATIC FIELDS.
	// ******************************************************************
	public static final int			TABLE_COL_TOKEN_CODE			= 0;
	public static final int			TABLE_COL_TOKEN_STARTING_TIME	= 1;	
	public static final int			TABLE_COL_TOKEN_END_TIME		= 2;
	public static final int			TABLE_COLUMNS					= 3;	

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private static final String[]	astrColumnNames = {"Token Code", "Starting Time", "End Time" };
	private DefaultTableModel		tableModel;
	private JPanel					pnlTokens;
	private JTable					tableTokens;
	private JScrollPane				scrollPane;
	private String					selectedToken;
	private JButton 				btnOK;
	private JButton 				btnCancel;
	

	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************
	public SelectTokenCodeDialog()
	{
		super( );
		commonConstructor( );
	}

	private void commonConstructor()
	{
		setDefaultCloseOperation( JDialog.HIDE_ON_CLOSE );
		getContentPane( ).setLayout( null );
		setPreferredSize( new Dimension( 800, 500 ) );
		setTitle( "Selecting Token Code to Present at UPM..." );
		setResizable( false );

		addTokensPanel( );
		addOKButton( );
		addCancelButton( );
		
		setMainStatusTextSize( 410 - 2 * 10 - 200, 22 );
		
		formatDialog( );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************
	@Override
	protected boolean loadDialogData()
	{
		selectedToken = null;
		tableTokens.setEnabled( true );
		if( btnOK != null )
		{
			btnOK.setEnabled( false );
		}
		
		return true;
	}

	@Override
	protected boolean saveDialogData()
	{
		return true;
	}

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************
	public void addNewTokenCode(DataTransferForTokenCodeTableRow dttu)
	{
		tableModel.addRow( dttu.getDataForSelectTokenCodeDialogTable( ) );
	}

	public String getSelectedToken()
	{
		return selectedToken;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	public void clearTable()
	{
		tableModel.getDataVector().removeAllElements();
	}
	
	protected void closeDialogOnCancel()
	{
		selectedToken = null;
		dispose( );
	}
	
	protected void closeDialogOnAccept()
	{
		selectedToken = null;
		int idxRow = tableTokens.getSelectedRow( );
		if ( idxRow >= 0 )
		{
			selectedToken = (String) tableModel.getValueAt( idxRow, TABLE_COL_TOKEN_CODE );
		}
		
		dispose( );
	}	

	// ******************************************************************
	// PRIVATED METHODS.
	// ******************************************************************
	private JTable createTokensTable()
	{
		String[][] initVoidData = {};
		tableModel = new DefaultTableModel( initVoidData, astrColumnNames );

		JTable tblTU = new JTable( tableModel )
		{
			private static final long	serialVersionUID	= 1L;
			DefaultTableCellRenderer	renderCenter		= new DefaultTableCellRenderer( );
			{
				renderCenter.setHorizontalAlignment( SwingConstants.CENTER );
				renderCenter.setVerticalAlignment( SwingConstants.CENTER );
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

		tblTU.getSelectionModel( ).addListSelectionListener( new ListSelectionListener( )
		{
			@Override
			public void valueChanged(ListSelectionEvent arg0)
			{
				btnOK.setEnabled( true );
			}
		} );

		tblTU.getTableHeader( ).setFont( JFlexPayDialog.getDefaultSmallFont( ) );
		tblTU.getTableHeader( ).setBackground( JFlexPayDialog.getPanelDefaultBackgroundColor( ) );
		tblTU.setFont( JFlexPayDialog.getDefaultSmallFont( ) );
		tblTU.setBackground( JFlexPayDialog.getPanelDefaultBackgroundColor( ) );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_TOKEN_CODE ).setPreferredWidth( 245 );		
		tblTU.getColumnModel( ).getColumn( TABLE_COL_TOKEN_STARTING_TIME ).setPreferredWidth( 92 );
		tblTU.getColumnModel( ).getColumn( TABLE_COL_TOKEN_END_TIME ).setPreferredWidth( 92 );
		tblTU.setSelectionMode( ListSelectionModel.SINGLE_SELECTION );

		return tblTU;
	}
	
	private void addTokensPanel()
	{
		pnlTokens = new JPanel( );
		pnlTokens.setBounds( 15, 10, 770, 405 );
		pnlTokens.setBorder( new LineBorder( new Color( 0, 0, 0 ) ) );
		pnlTokens.setBorder( new TitledBorder( null, "Tokens", TitledBorder.LEADING, TitledBorder.TOP, JFlexPayDialog.getDefaultFont( ), null ) );
		pnlTokens.setLayout( null );
		add( pnlTokens );

		tableTokens = createTokensTable( );
		scrollPane = new JScrollPane( tableTokens );
		scrollPane.setBounds( 20, 20, 730, 370 );
		pnlTokens.add( scrollPane, BorderLayout.NORTH );
	}	
	
	private void addCancelButton()
	{
		btnCancel = new JButton( "Cancel" );
		btnCancel.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				closeDialogOnCancel();
			}
		} );
		btnCancel.setBounds( 670, 420, 110, 30 );
		btnCancel.setBorder(BorderFactory.createRaisedBevelBorder());
		getContentPane( ).add( btnCancel );
	}
	
	private void addOKButton()
	{
		btnOK = new JButton( "OK" );
		btnOK.setEnabled( false );
		btnOK.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				closeDialogOnAccept();
			}
		} );
		btnOK.setBounds( 550, 420, 110, 30 );
		btnOK.setBorder(BorderFactory.createRaisedBevelBorder());
		getContentPane( ).add( btnOK );
	}	
}
