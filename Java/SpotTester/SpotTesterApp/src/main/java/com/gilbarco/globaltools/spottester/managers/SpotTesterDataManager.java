
package com.gilbarco.globaltools.spottester.managers;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import org.apache.activemq.util.ByteArrayOutputStream;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.io.DataCommunicationLayer;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketAdapter;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketDefinition;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.edt.services.process.EdtProcessAdapter;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.mgp.edt.service.MgpServiceAdapter;
import com.gilbarco.globaltools.spottester.model.SpotMessages;
import com.gilbarco.globaltools.spottester.model.SpotServiceDefinitionsFactory;
import com.gilbarco.globaltools.spottester.model.SpotState;

/**
 * @Class SpotTesterDataController
 * 
 * @Description This class is the middleman between the application and the EdtClientSocketService.
 *              <p>
 *              It work entails decode and validate every incoming message and reconnect the communication. It also connect the application for the first time.
 *              <p>
 *              It has the old LAP functionality.Singleton.
 * 
 * @actions Reconnection feature.
 * 
 * @Services <ul>
 *           <li>EdtMgpService<li>EdtClientSocketService
 *           </ul>
 * 
 * @see MgpServiceAdapter
 * @see EdtClientSocketAdapter
 * 
 * @version 1.0.0
 * 
 * @author jrubstein
 * 
 * */

public class SpotTesterDataManager implements DataCommunicationLayer
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static SpotTesterDataManager	instance	= null;
	private static final transient Log		logger		= LogFactory.getLog( SpotTesterDataManager.class );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	public synchronized static SpotTesterDataManager getInstance()
	{
		if (instance == null)
		{
			instance = new SpotTesterDataManager( );
		}
		return instance;
	}

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private MgpServiceAdapter		edtMgpAdapter;
	private EdtClientSocketAdapter	socketAdapter;
	private EdtProcessAdapter	spotProcess;
	private boolean					connected;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private SpotTesterDataManager()
	{
		super( );
		try
		{
			edtMgpAdapter = (MgpServiceAdapter) EdtServiceContext.getServiceManager( ).getServiceAdapter(SpotServiceDefinitionsFactory.getMgpDefinition( ) );
			socketAdapter = (EdtClientSocketAdapter) EdtServiceContext.getServiceManager( ).getServiceAdapter(SpotServiceDefinitionsFactory.getSocketDefinition( ) );
			spotProcess = (EdtProcessAdapter) EdtServiceContext.getServiceManager( ).getServiceAdapter( SpotServiceDefinitionsFactory.getProcessDefinition( ) );
			suscribeToEvents( );

			EdtSocketDefinition sdef = (EdtSocketDefinition) SpotServiceDefinitionsFactory.getSocketDefinition( );
			if (sdef.isAutoConnect( ))
			{
				this.connect( );
				connected = true;
			}

		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}

	}

	/**
	 * @author stomasini @
	 * @param serverUri
	 * @param serverPort
	 * @throws EdtServiceException
	 */
	public void configureAndLogin(String serverUri, int serverPort, byte clientID) throws EdtServiceException
	{
		logger.debug( "configureAndLogin - " + serverUri + ":" + serverPort + "." + clientID );

		// check if we are connected
		if (isConnect( ))
		{
			logger.info( "configureAndLogin. Close Connection" );
			this.closeSocket( );
		}
		
		// with the socket closed now we change the config with new port and uri
		try
		{
			this.socketAdapter.connect( serverUri, String.valueOf( serverPort ) );
			this.login( clientID );

		}
		catch (EdtServiceException e)
		{
			logger.fatal( e.getMessage( ) );
			throw e;
		}
	}

	public void login( byte clientID)
	{
		connected = true;
		SpotTesterDataManager.getInstance( ).sendMessage( SpotMessages.getInstance( ).getSpotLoginData( clientID ) );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public void callSendData(byte[] data) throws Exception
	{
		this.socketAdapter.callSendData( data );
	}

	@Override
	public void invokeSendData(byte[] data) throws Exception
	{
		this.socketAdapter.invokeSendData( data );
	}

	/**
	 * @Description Close the current connection.
	 * */
	public void closeSocket()
	{
		try
		{
			connected = false;
			socketAdapter.callCloseSocket( "Logout" );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
	}

	/**
	 * @throws EdtServiceException
	 * @Description Open a connection.
	 * */
	public void connect() throws EdtServiceException
	{
		try
		{
			connected = true;
			socketAdapter.callConnect( );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
			throw e;
		}
	}

	public void sendMessage(byte[] data)
	{
		try
		{
			this.socketAdapter.invokeSendData( data );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}
	}

	public boolean isConnect()
	{
		return connected;
	}
	

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void suscribeToEvents()
	{
		try
		{
			EdtServiceContext.getServiceManager( ).subscribe( new EdtEventAction<EdtClientSocketEvent>( )
			{
				private byte [] bufferedByteArray;

				@Override
				public void onInvokeEvent(EdtClientSocketEvent event)
				{

					try
					{
						if (event.getData( ) != null)
						{
							byte [] nextByteArray = event.getData( );
							if (bufferedByteArray != null)
							{
								try
								{
									ByteArrayOutputStream baos = new ByteArrayOutputStream(bufferedByteArray.length + nextByteArray.length);
									baos.write(bufferedByteArray);
									baos.write(nextByteArray);
									bufferedByteArray = baos.toByteArray();

									baos.close();
								}
								catch (IOException e)
								{
									logger.error( e.getMessage( ) );
								}
							}
							else
							{
								try
								{
									ByteArrayOutputStream baos = new ByteArrayOutputStream(nextByteArray.length);
									baos.write(nextByteArray);
									bufferedByteArray = baos.toByteArray();
									baos.close();
								}
								catch (IOException e)
								{
									logger.error( e.getMessage( ) );
								}
							}
						}
						else
						{
							logger.error( "Unexpected null data buffer." );
						}
						
						if (bufferedByteArray != null)
						{

							boolean bKeepReading = true;
							while (bKeepReading)
							{
								byte [] currentByteArray = null;
								if (bufferedByteArray != null)
								{
									int currentMessageSize = (((int)(bufferedByteArray[0] & 0xFF)) << 8) | ((int)(bufferedByteArray[1] & 0xFF));
									if (bufferedByteArray.length > currentMessageSize)
									{
										int newBufferedSize = bufferedByteArray.length - currentMessageSize;
										try
										{
											ByteArrayOutputStream baos = null;
											if (currentMessageSize > newBufferedSize)
											{
												baos = new ByteArrayOutputStream(currentMessageSize);
											}
											else
											{
												baos = new ByteArrayOutputStream(newBufferedSize);
											}
											
											baos.write(bufferedByteArray, 0, currentMessageSize);
											currentByteArray = baos.toByteArray();
											baos.reset();
											
											baos.write(bufferedByteArray, currentMessageSize, newBufferedSize);
											bufferedByteArray = baos.toByteArray();

											baos.close();
										}
										catch (IOException e)
										{
											logger.error( e.getMessage( ) );
										}
									}
									else if (bufferedByteArray.length == currentMessageSize)
									{
										currentByteArray = bufferedByteArray;
										bufferedByteArray = null;
										bKeepReading = false;
									}
									else
									{
										bKeepReading = false;
									}
								}
								else
								{
									bKeepReading = false;
								}
								
								if (currentByteArray != null)
								{
									logger.debug( "Response message full : " + EdtConvert.bytesToHexString( currentByteArray ) );
									edtMgpAdapter.callValidator( currentByteArray ); //false if ack with error or mgp exception
								}
							}
						}
					}
					catch (EdtServiceException e)
					{
						bufferedByteArray = null;
						SpotResponseMessageError( e.getMessage( ));
					}
				}

				private void SpotResponseMessageError(String msg)
				{
					logger.fatal( "SPOT RESPONSE ERROR - "+msg );
					try
					{
						spotProcess.callChangeToState( spotProcess.getCurrentState( ), SpotState.SPOT_ERROR.name( ), "" );//spot error
					}
					catch (EdtServiceException e1)
					{
					}
				}

				@Override
				public Class<EdtClientSocketEvent> getEventClass()
				{
					return EdtClientSocketEvent.class;
				}

				@Override
				public boolean justOnce()
				{
					return false;
				}

				@Override
				public String getSubscriptionFilter()
				{
					return null;
				}
			}, EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.INCOMING_DATA.name( ) + "'" );

		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************
}
