/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.engines;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.EdtServiceManager;
import com.gilbarco.globaltools.edt.services.crypto.X509EdtTrustManager;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSClientSocketAdapter;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSClientSocketService;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSSocketDefinition;
import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;
public class TLSSocketTransportLayer implements DataTransportLayer
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( TLSSocketTransportLayer.class );
	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected EdtTLSClientSocketAdapter	socketAdapter;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public TLSSocketTransportLayer(String serverUri, int serverPort)
	{
		super( );
		try
		{
			String strDefname = "TLSServerSocket" + serverUri + ":" + serverPort;
			EdtTLSSocketDefinition definition = new EdtTLSSocketDefinition( strDefname,
																			EdtTLSClientSocketAdapter.class.getName( ),
																			serverUri, serverPort, false, null );
			EdtServiceManager manager = EdtServiceContext.getServiceManager( );
			manager.registerService( new EdtTLSClientSocketService( definition ) );
			this.socketAdapter = (EdtTLSClientSocketAdapter) manager.getServiceAdapter( strDefname );
			
			X509EdtTrustManager.setExpectedServerCertificateInfo( FlexPayAppConfig.getHandshakeCaPemFile( ),  
				FlexPayAppConfig.getServerCertifCommonName( ), FlexPayAppConfig.getServerCertifOrg( ), FlexPayAppConfig.getServerCertifDNSName( ));
			
		}
		catch (EdtServiceException e)
		{
			logger.debug( "TLSSocketTransportLayer - EdtServiceException - "+e.getMessage( ) );
		}
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	/**
	 * @return the serverUri
	 */
	public String getServerUri()
	{
		return socketAdapter.getTLSSocketServiceDefinition().getUriIdentifier( );
	}

	/**
	 * @return the serverPort
	 */
	public int getServerPort()
	{
		return socketAdapter.getTLSSocketServiceDefinition().getLocalPort( );
	}
	
	/**
	 * @param serverUri the serverUri to set
	 */
	public void setServerUri(String serverUri)
	{
		socketAdapter.getTLSSocketServiceDefinition().setUriIdentifier( serverUri );
	}

	/**
	 * @param serverPort the serverPort to set
	 */
	public void setServerPort(int serverPort)
	{
		socketAdapter.getTLSSocketServiceDefinition().setLocalPort( serverPort );
	}

	/*
	 * Connect and send message to service.
	 * @see com.gilbarco.globaltools.flexpay.engines.DataTransportLayer#send(byte[])
	 * @throws EdtServiceException if occurs an error.
	 */
	@Override
	public void send(byte[] data) throws EdtServiceException
	{
		try
		{
			// This protocol is simple, connect, and then send the message
			// (will be disconnected after response is received).
			socketAdapter.callConnect();
			this.invokeMessage( data );
		}
		catch (EdtServiceException e)
		{
			throw e; 
		}
	}
	
	/*
	 * Connect and send message to service setting a timeout to do this action.
	 * @see com.gilbarco.globaltools.flexpay.engines.DataTransportLayer#send(byte[])
	 * @throws EdtServiceException if occurs an error.
	 */
	@Override
	public void send(byte[] data, long timeout) throws EdtServiceException
	{
		try
		{
			// This protocol is simple, connect, and then send the message
			// (will be disconnected after response is received).
			socketAdapter.callConnect(timeout);
			this.invokeMessage( data );
		}
		catch (EdtServiceException e)
		{
			throw e; 
		}
	}	

	/**
	 * @Description Send a message and invoke and outgoing message event.
	 * @param data
	 *            Data to be sent.
	 * @throws EdtServiceException.
	 * */
	public void invokeMessage(byte[] data) throws EdtServiceException
	{
		socketAdapter.invokeSendData( data );
	}

	/**
	 * @Description Send a message and invoke and outgoing message event.
	 * @param event
	 *            EdtClientSocketEvent with the outgoing data.
	 * @throws EdtServiceException
	 * */
	public void invokeMessage(EdtTLSClientSocketEvent event) throws EdtServiceException
	{
		socketAdapter.invoke( event );
	}

	/**
	 * @Description Send a message.
	 * @param data
	 *            Data to be sent.
	 * @throws EdtServiceException.
	 * */
	public void sendMessage(byte[] data) throws EdtServiceException
	{
		socketAdapter.callSendData( data );
	}

	/**
	 * @Description Send a message.
	 * @param event
	 *            EdtClientSocketEvent with the outgoing data.
	 * @throws EdtServiceException
	 * */
	public void sendMessage(EdtTLSClientSocketEvent event) throws EdtServiceException
	{
		socketAdapter.call( event );
	}

	public void close()
	{
		try
		{
			socketAdapter.callCloseSocket( "closing socket." );
		}
		catch (EdtServiceException e)
		{
			logger.error( "Closing socket [" + e.getMessage( ) + "]" );
		}
	}
	

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
