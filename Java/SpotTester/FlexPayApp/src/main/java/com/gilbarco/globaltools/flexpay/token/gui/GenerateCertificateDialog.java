
package com.gilbarco.globaltools.flexpay.token.gui;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.security.GeneralSecurityException;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.SwingWorker;
import javax.swing.border.TitledBorder;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.crypto.RSAKeyPair;
import com.gilbarco.globaltools.edt.services.crypto.X509SelfSignCertificate;
import com.gilbarco.globaltools.edt.util.EdtSignData;
import com.gilbarco.globaltools.flexpay.widgets.JFlexPayDialog;

/**
 * @author mgrieco
 * 
 */
public class GenerateCertificateDialog extends JFlexPayDialog
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final String	CERT_SIGN_ALGORITHM	= "SHA256withRSA";
	private static final long	serialVersionUID	= 1L;
	private static final transient Log	logger		= LogFactory.getLog( GenerateCertificateDialog.class );

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

	private String				txtCN;
	private String				txtOrganization;
	private String				txtExpirationDays;
	private String				txtEmailField;
	private String				txtLocality;
	private String				txtCountry;
	private String				txtState;
	private JPanel				pnlParameter;
	private JProgressBar		progressBar;
	private Boolean				rcGeneratedCertificate;
	private Task 				worker;

	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************

	public GenerateCertificateDialog()
	{
		super( );
		commonConstructor( );
	}

	public GenerateCertificateDialog(TokenManagerView tokenManagerView)
	{
		commonConstructor( );
	}

	private void commonConstructor()
	{
		setDefaultCloseOperation( JDialog.HIDE_ON_CLOSE );
		getContentPane( ).setLayout( null );
		setPreferredSize( new Dimension( 440, 200 ) );
		setTitle( "Generate Certificate" );
		setResizable( false );

		addCloseButton( );
		addParametersPanel( );
		
		setMainStatusTextSize( 410 - 2 * 10 - 200, 22 );
		// Apply Gilbarco's GT settings to the panels and controls, and packs it.
		formatDialog( );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	@Override
	protected boolean loadDialogData()
	{
		txtCountry 			= new String( TokenManagerController.getDefaultCountry( ) );
		txtState 			= new String( TokenManagerController.getDefaultState( ) );
		txtLocality 		= new String( TokenManagerController.getDefaultLocality( ) );
		txtEmailField 		= new String( TokenManagerController.getDefaultMail( ) );
		txtExpirationDays 	= new String( Long.toString( TokenManagerController.getDefaultExpirationDays( ) ) );
		txtOrganization 	= new String( TokenManagerController.getDefaultOrganization( ) );
		txtCN 				= new String( TokenManagerController.getDefaultCommonName( ) );
		
		
		// refresh technician id. if needed for the certificate generation
		
		startProgressBar( );
		
		return true;
	}

	@Override
	protected boolean saveDialogData()
	{
		return true; // not implemented yet
	}

	// ******************************************************************
	// PUBLIC METHODS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	protected boolean validateParameters()
	{
		if (getCommonName( ).length( ) < 1)
		{
			showMessageDialog( "Check CN field invalid value" );
		}
		else if (getCertifEmail( ).length( ) < 1)
		{
			showMessageDialog( "Check EMail field invalid value" );
		}
		else if (getOrganization( ).length( ) < 1)
		{
			showMessageDialog( "Check Organization field invalid value" );
		}
		else if (getLocality( ).length( ) < 1 && getState( ).length( ) < 1 && getCountry( ).length( ) < 1)
		{
			showMessageDialog( "Check Location field invalid selection" );
		}
		else if (getExpirationDays( ) < 0)
		{
			showMessageDialog( "Check Expiration Days field invalid value" );
		}
		else
		{
			return true;
		}

		return false;
	}

	protected void closeDialog()
	{
		this.dispose( );
	}

	protected boolean generateCertificate()
	{
		try
		{
			RSAKeyPair keys = new RSAKeyPair( );
			keys.generate( );
			X509SelfSignCertificate cert = new X509SelfSignCertificate(
																		keys,
																		getCommonName( ),
																		getCertifEmail( ),
																		getOrganization( ),
																		getOrganization( ),
																		getLocality( ),
																		getState( ),
																		getCountry( ),
																		getExpirationDays( ),
																		GenerateCertificateDialog.CERT_SIGN_ALGORITHM );
			if (cert != null)
			{
				cert.save( getCertifPath( ), getCommonName( ) );
				keys.save( getCertifPath( ), getCommonName( ) );
				
				EdtSignData.getInstance().loadPrivateKey();

				return true;
			}
		}
		catch (NumberFormatException | GeneralSecurityException | IOException e)
		{
			logger.error( e.getMessage( ));
		}

		return false;
	}

	class Task extends SwingWorker<Boolean, Void>
	{
		@Override
		public Boolean doInBackground()
		{
			return new Boolean( generateCertificate( ) );
		}

		@Override
		public void done()
		{
			rcGeneratedCertificate = new Boolean( false );
			try
			{
				rcGeneratedCertificate = get( );
			}
			catch (InterruptedException ignore)
			{
			}
			catch (java.util.concurrent.ExecutionException e)
			{
				logger.error( e.getMessage( ));
			}
			stopProgressBar( );
		}
	};

	// ******************************************************************
	// PRIVATED METHODS.
	// ******************************************************************
	private void startProgressBar()
	{
		if( progressBar == null )
		{
			progressBar = new JProgressBar( );
			progressBar.setBounds( 37, 40, 300, 40 );
			progressBar.setIndeterminate( true );
			progressBar.setStringPainted( false );
			progressBar.setBorderPainted( true );
			getParametersPanel( ).add( progressBar );
		}
		progressBar.setVisible( true );
		
		if( validateParameters( ) )
		{
			worker = new Task();
			worker.execute( );
		}
	}

	private void stopProgressBar()
	{
		if( worker != null && worker.isDone( ) )
		{
			progressBar.setVisible( false );
			
			if (rcGeneratedCertificate.equals( true ))
			{
				showMessageDialog( "Certificate Generation Success!." );
				logger.info( "The certificate was created!." );
			}
			else
			{
				showErrorDialog( "Certificate Generation Fail - Logfile Generated" );
				logger.error( "Certificate Generation Fail!." );
			}
			closeDialog();
		}
	}

	private void addParametersPanel()
	{
		pnlParameter = new JPanel( );
		pnlParameter.setBounds( 31, 11, 380, 116 );
		pnlParameter.setLayout( null );
		pnlParameter.setBorder( new TitledBorder( null, "Building Certificate",	TitledBorder.LEADING, TitledBorder.TOP ) );

		getContentPane( ).add( pnlParameter );
	}

	private Container getParametersPanel()
	{
		return pnlParameter;
	}

	private void addCloseButton()
	{
		JButton btnClose = new JButton( "Close" );
		btnClose.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				stopProgressBar( );
			}
		} );
		btnClose.setBounds( 158, 140, 112, 30 );
		btnClose.setBorder(BorderFactory.createRaisedBevelBorder());
		getContentPane( ).add( btnClose );
	}

	private String getCertifPath()
	{
		return TokenManagerController.getCertifDefaultFolder( );
	}

	private String getCommonName()
	{
		return this.txtCN;
	}

	private String getOrganization()
	{
		return this.txtOrganization;
	}

	private String getLocality()
	{
		return this.txtLocality;
	}

	private String getState()
	{
		return this.txtState;
	}

	private String getCountry()
	{
		return this.txtCountry;
	}

	private int getExpirationDays()
	{
		return this.txtExpirationDays.length( ) > 0 ? Integer.parseInt( txtExpirationDays ) : -1;
	}

	private String getCertifEmail()
	{
		return this.txtEmailField;
	}
	
	private void showMessageDialog(String message)
	{
		JOptionPane pane = new JOptionPane( message, JOptionPane.INFORMATION_MESSAGE );
		JDialog dialog = pane.createDialog( "Certificate Management" );
		dialog.setAlwaysOnTop( true );
		dialog.setVisible( true );
	}

	private void showErrorDialog(String message)
	{
		JOptionPane pane = new JOptionPane( message, JOptionPane.ERROR_MESSAGE );
		JDialog dialog = pane.createDialog( "Certificate Management" );
		dialog.setAlwaysOnTop( true );
		dialog.setVisible( true );
	}	
}
