/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.flexpay.token.gui;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;

import com.gilbarco.globaltools.edt.util.EdtSwingUtil;
import com.gilbarco.globaltools.flexpay.widgets.JFlexPayDialog;
import com.gilbarco.globaltools.flexpay.model.FlexPayContext;
import com.gilbarco.globaltools.flexpay.token.domain.Token;

public class TokenRequestDialog extends JFlexPayDialog
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	private static final long	serialVersionUID				= 8755710539831593576L;
	private static final int	MAX_TOKEN_CODE_FIELD_LEN	= 4;

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	private JButton				btnClose;
	private JButton				btnRequestToken;
	private JLabel				lblTokenCodeField;
	private JTextField			txtTokenCodeField1;
	private JTextField			txtTokenCodeField2;
	private JTextField			txtTokenCodeField3;
	private JTextField			txtTokenCodeField4;
	private JTextField			txtTokenCodeField5;
	private JTextField			txtTokenCodeField6;
	private JTextField			txtTokenCodeField7;
	private JTextField			txtTokenCodeField8;
	private Boolean				bTokenRequested;

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

	public TokenRequestDialog()
	{
		super( );
		setLocation( -223, -44 );
		commonConstructor( );
	}

	private void commonConstructor()
	{
		setLayoutSettings( );
		addTokenCodeFields( );
		addRequestButton( );
		addCloseButton( );
		addStatusBar( );
		clearTokenRequested( );
		pack( );
		getContentPane( ).setLayout( null );

	}

	private void addCloseButton()
	{
		btnClose = new JButton( FlexPayContext.getLocaleText( "Close" ) );
		btnClose.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				bTokenRequested = false;
				closeDialog( );
			}
		} );
		btnClose.setBounds( 438, 89, 112, 30 );
		btnClose.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		btnClose.setFont( JFlexPayDialog.getDefaultButtonFont( ) );
		getContentPane( ).add( btnClose );
	}

	private void addRequestButton()
	{
		btnRequestToken = new JButton( FlexPayContext.getLocaleText( "Request" ) );
		btnRequestToken.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				bTokenRequested = true;
				closeDialog( );
			}

		} );
		btnRequestToken.setBounds( 292, 89, 120, 30 );
		btnRequestToken.setBorder( BorderFactory.createRaisedBevelBorder( ) );
		btnRequestToken.setFont( JFlexPayDialog.getDefaultButtonFont( ) );
		enableRequest( );
		getContentPane( ).add( btnRequestToken );
	}

	public String getTokenCode()
	{
		return txtTokenCodeField1.getText( ) + " " + txtTokenCodeField2.getText( ) + " " + txtTokenCodeField3.getText( ) + " " + txtTokenCodeField4.getText( )
			+ " " + txtTokenCodeField5.getText( ) + " " + txtTokenCodeField6.getText( ) + " " + txtTokenCodeField7.getText( ) + " " + txtTokenCodeField8.getText( );
	}

	private void addTokenCodeFields()
	{		
		lblTokenCodeField = new JLabel( FlexPayContext.getLocaleText( "TokenCode" ) + ":" );
		lblTokenCodeField.setFont( JFlexPayDialog.getDefaultFont( ) );
		lblTokenCodeField.setBounds( 36, 37, 134, 18 );
		getContentPane( ).add( lblTokenCodeField );
		
		txtTokenCodeField1 = new JTextField( "" ); 
		txtTokenCodeField1.setToolTipText( (String) null );
		txtTokenCodeField1.setBounds( 173, 37, 42, 20 );
		txtTokenCodeField1.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtTokenCodeField1.setFont( JFlexPayDialog.getDefaultFont( ) );
		addValidationCodeFieldListeners( txtTokenCodeField1 );
		getContentPane( ).add( txtTokenCodeField1 );

		txtTokenCodeField2 = new JTextField( "" );
		txtTokenCodeField2.setToolTipText( (String) null );
		txtTokenCodeField2.setBounds( 217, 37, 42, 20 );
		txtTokenCodeField2.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtTokenCodeField2.setFont( JFlexPayDialog.getDefaultFont( ) );
		addValidationCodeFieldListeners( txtTokenCodeField2 );
		getContentPane( ).add( txtTokenCodeField2 );

		txtTokenCodeField3 = new JTextField( "" );
		txtTokenCodeField3.setToolTipText( (String) null );
		txtTokenCodeField3.setBounds( 261, 37, 42, 20 );
		txtTokenCodeField3.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtTokenCodeField3.setFont( JFlexPayDialog.getDefaultFont( ) );
		addValidationCodeFieldListeners( txtTokenCodeField3 );
		getContentPane( ).add( txtTokenCodeField3 );

		txtTokenCodeField4 = new JTextField( "" );
		txtTokenCodeField4.setToolTipText( (String) null );
		txtTokenCodeField4.setBounds( 306, 37, 42, 20 );
		txtTokenCodeField4.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtTokenCodeField4.setFont( JFlexPayDialog.getDefaultFont( ) );
		addValidationCodeFieldListeners( txtTokenCodeField4 );
		getContentPane( ).add( txtTokenCodeField4 );

		txtTokenCodeField5 = new JTextField( "" ); 
		txtTokenCodeField5.setToolTipText( (String) null );
		txtTokenCodeField5.setBounds( 350, 37, 42, 20 );
		txtTokenCodeField5.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtTokenCodeField5.setFont( JFlexPayDialog.getDefaultFont( ) );
		addValidationCodeFieldListeners( txtTokenCodeField5 );
		getContentPane( ).add( txtTokenCodeField5 );

		txtTokenCodeField6 = new JTextField( "" );
		txtTokenCodeField6.setToolTipText( (String) null );
		txtTokenCodeField6.setBounds( 394, 37, 42, 20 );
		txtTokenCodeField6.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtTokenCodeField6.setFont( JFlexPayDialog.getDefaultFont( ) );
		addValidationCodeFieldListeners( txtTokenCodeField6 );
		getContentPane( ).add( txtTokenCodeField6 );

		txtTokenCodeField7 = new JTextField( "" );
		txtTokenCodeField7.setToolTipText( (String) null );
		txtTokenCodeField7.setBounds( 438, 37, 42, 20 );
		txtTokenCodeField7.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtTokenCodeField7.setFont( JFlexPayDialog.getDefaultFont( ) );
		addValidationCodeFieldListeners( txtTokenCodeField7 );
		getContentPane( ).add( txtTokenCodeField7 );

		txtTokenCodeField8 = new JTextField( "" );
		txtTokenCodeField8.setToolTipText( (String) null );
		txtTokenCodeField8.setBounds( 482, 37, 42, 20 );
		txtTokenCodeField8.setBorder( BorderFactory.createLoweredBevelBorder( ) );
		txtTokenCodeField8.setFont( JFlexPayDialog.getDefaultFont( ) );
		addValidationCodeFieldListeners( txtTokenCodeField8 );
		getContentPane( ).add( txtTokenCodeField8 );
	}

	private void addValidationCodeFieldListeners(final JTextField txtTokenCodeField)
	{
		txtTokenCodeField.addKeyListener( new TokenCodeFieldListener(txtTokenCodeField) );

		txtTokenCodeField.addFocusListener( new FocusAdapter( )
		{
			@Override
			public void focusLost(FocusEvent e)
			{
				String text = ((JTextField) e.getSource( )).getText( ).toUpperCase( );
				((JTextField) e.getSource( )).setText( text );
			}
		} );
	}
	
	private void addStatusBar()
	{
		addStatusBar( " ", false );
	}

	private void setLayoutSettings()
	{
		setDefaultCloseOperation( JDialog.HIDE_ON_CLOSE );
		getContentPane( ).setLayout( null );

		setPreferredSize( new Dimension( 600, 190 ) );
		setTitle( FlexPayContext.getLocaleText( "RequestToken" ) );
		setResizable( false );
	}

	protected void closeDialog()
	{
		this.setVisible( false );// dispose( );
	}

	public void enableRequest()
	{
		boolean enable = txtTokenCodeField1.getText( ).length( ) == MAX_TOKEN_CODE_FIELD_LEN
							&& txtTokenCodeField2.getText( ).length( ) == MAX_TOKEN_CODE_FIELD_LEN
							&& txtTokenCodeField3.getText( ).length( ) == MAX_TOKEN_CODE_FIELD_LEN
							&& txtTokenCodeField4.getText( ).length( ) == MAX_TOKEN_CODE_FIELD_LEN
							&& txtTokenCodeField5.getText( ).length( ) == MAX_TOKEN_CODE_FIELD_LEN
							&& txtTokenCodeField6.getText( ).length( ) == MAX_TOKEN_CODE_FIELD_LEN
							&& txtTokenCodeField7.getText( ).length( ) == MAX_TOKEN_CODE_FIELD_LEN
							&& txtTokenCodeField8.getText( ).length( ) == MAX_TOKEN_CODE_FIELD_LEN
							&& Token.getFromDatabase(getTokenCode( ))== null;
		btnRequestToken.setEnabled( enable );
	}

	public boolean certificateInfoRequested()
	{
		return this.bTokenRequested;
	}

	public void clearTokenRequested()
	{
		bTokenRequested = false;
	}

	public JButton getRequestButton()
	{
		return btnRequestToken;
		
	}


	protected void fillTokenCode(String text) {
		
		fillTokenCodeField(txtTokenCodeField1,text.substring(0, 4));		
		fillTokenCodeField(txtTokenCodeField2,text.substring(4, 8));		
		fillTokenCodeField(txtTokenCodeField3,text.substring(8, 12));		
		fillTokenCodeField(txtTokenCodeField4,text.substring(12, 16));		
		fillTokenCodeField(txtTokenCodeField5,text.substring(16, 20));		
		fillTokenCodeField(txtTokenCodeField6,text.substring(20, 24));		
		fillTokenCodeField(txtTokenCodeField7,text.substring(24, 28));		
		fillTokenCodeField(txtTokenCodeField8,text.substring(28, 32));		
	}

	protected void fillTokenCodeField(JTextField field, String text)
	{	
		field.setText(text);
		KeyListener[] listeners = field.getKeyListeners();		
		for(int i=0; i< listeners.length; ++i )
		{
			if(listeners[i] instanceof TokenCodeFieldListener)
			{
				((TokenCodeFieldListener) listeners[i]).setOldText(text.substring(0, 3));
				break;
			}
		}		
	}
	
	// ******************************************************************
	// INNER CLASS
	// ******************************************************************
	
	protected final class TokenCodeFieldListener extends KeyAdapter 
	{
		private String	oldtext;

		private TokenCodeFieldListener(JTextField txtTokenCodeField) 
		{
			oldtext = txtTokenCodeField.getText( );
		}

		@Override
		public void keyTyped(KeyEvent e)
		{
			char code = e.getKeyChar( );				
			String text = ((JTextField) e.getSource( )).getText( ).toUpperCase( );
			String textselected = ((JTextField) e.getSource( )).getSelectedText( );
			if ((code != KeyEvent.VK_BACK_SPACE && code != KeyEvent.VK_CLEAR && code != KeyEvent.VK_DELETE)
				&& (!Token.isTokenCodeDigitString( text ) || e.isAltDown( ) || ((text.length( ) == MAX_TOKEN_CODE_FIELD_LEN) && ((textselected == null) || (textselected.length( ) == 0)))))
			{
				e.consume( );
				return;
			}
		}

		@Override
		public void keyReleased(KeyEvent e)
		{
			String text = ((JTextField) e.getSource( )).getText( ).toUpperCase( ).replace(" ", "");;
			if ( !Token.isTokenCodeDigitString( text ) ) 
			{
				assert false : "Shouldn't be here !";
				((JTextField) e.getSource( )).setText( oldtext );
				return;
			}

			if ((text.length( ) == MAX_TOKEN_CODE_FIELD_LEN * 8) || (text.length( ) == MAX_TOKEN_CODE_FIELD_LEN * 9 && text.endsWith(oldtext) ) || (text.length( ) == MAX_TOKEN_CODE_FIELD_LEN * 9 && text.startsWith(oldtext) ))
			{
				if(text.length( ) == MAX_TOKEN_CODE_FIELD_LEN * 9 && text.startsWith(oldtext))
				{
					text=text.substring(MAX_TOKEN_CODE_FIELD_LEN);
				}					
				fillTokenCode(text);
			}
			else 
			{
				if (text.length( ) > MAX_TOKEN_CODE_FIELD_LEN)
				{
					((JTextField) e.getSource( )).setText( text.substring( 0, MAX_TOKEN_CODE_FIELD_LEN ) );
				}
				if (oldtext.length( ) < MAX_TOKEN_CODE_FIELD_LEN && text.length( ) == MAX_TOKEN_CODE_FIELD_LEN)
				{
					EdtSwingUtil.focusNextElement( (Component) e.getSource( ));
				}
								
				oldtext = text;					
			}
			
			enableRequest( );
		}
		public void setOldText(String text)
		{
			oldtext = text;
		}
	}
}
