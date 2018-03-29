/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file 
 * @author 
 * @date 18/10/2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

package com.gilbarco.globaltools.flexpay.engines;

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.util.Calendar;
import java.util.Observable;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/**
 * @author mgrieco
 * 
 * @brief This class provides the logic to activate the secure menu on the remote unit.
 */
public class SecureMenuEngine extends Observable
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger				= LogFactory.getLog( SecureMenuEngine.class );

	/** @brief Default target unit connection timeout measured in milliseconds */ 
	public static final long DEFAULT_TIMEOUT_MILLISECONDS = 60000L;


	// ******************************************************************
	// PRIVATE FIELDS
	// ******************************************************************
	/** The error in response. */
	private boolean							errorInResponse					= true;

	/** The received response. */
	public boolean							receivedResponse				= false;

	/** The socket disconnect event processor. */
	private EdtClientSocketDisconnectAction	socketDisconnectEventProcessor;

	private MgpResultAction					messageEventProcessor;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	/**
	 * Instantiates a new comparison engine.
	 * 
	 * @throws EdtServiceException
	 *             the edt service exception
	 */
	public SecureMenuEngine() throws EdtServiceException
	{
		super( );
		errorInResponse = true;
		receivedResponse = false;
		subscribeToEvents( );
	}

	// ******************************************************************
	// PUBLIC METHODS
	// ******************************************************************

	/**
	 * Subscribe to mgp results events.
	 * 
	 * @throws EdtServiceException
	 */
	protected void subscribeToEvents() throws EdtServiceException
	{
		if (messageEventProcessor == null)
		{
			messageEventProcessor = new MgpResultAction( );
			SERVICE_MANAGER.subscribe( messageEventProcessor );
		}

		if (socketDisconnectEventProcessor == null)
		{
			socketDisconnectEventProcessor = new EdtClientSocketDisconnectAction( );
			SERVICE_MANAGER.subscribe( socketDisconnectEventProcessor );
		}
	}

	/**
	 * Executes the activation of secure menu.
	 * 
	 * @return true, if the activation was successfully
	 */
	public boolean activate()
	{
		return sendAndReceiveActivation( true );
	}

	/**
	 * Deactivates the secure menu.
	 * 
	 * @return true, if the deactivation was successfully
	 */
	public boolean deactivate()
	{
		return sendAndReceiveActivation( false ); // deactivate the secure menu by spot protocol.
	}

	/**
	 * Sends and receives the answer of secure menu activation/deactivation.
	 * 
	 * @param activate
	 *            true for activate the secure menu, false to exit secure menu.
	 * @return true, if the activation was successfully
	 */
	private boolean sendAndReceiveActivation(boolean activate)
	{
		errorInResponse = false;
		receivedResponse = false;

		sendActivateSecureMenuRequest( activate ); // request the secure menu activation by spot protocol.

		synchronized (this) // wait for the package info response.
		{
			long t0 = Calendar.getInstance( ).getTimeInMillis( );
			long t1;
			while (!receivedResponse && !errorInResponse)
			{
				try
				{
					wait( DEFAULT_TIMEOUT_MILLISECONDS );
					t1 = Calendar.getInstance( ).getTimeInMillis( );
					if (t1 - t0 >= DEFAULT_TIMEOUT_MILLISECONDS)
					{
						break;
					}
				}
				catch (InterruptedException e)
				{
					logger.debug( e.getMessage( ) );
				}
			}
		}
		return receivedResponse && !errorInResponse;
	}

	/**
	 * Send the activation secure menu request message.
	 */
	private void sendActivateSecureMenuRequest(boolean activate)
	{
		String startinfo = activate ? "activating secure menu..." : "deactivating secure menu...";
		logger.info( startinfo );
		ConnectionEngineSwingAdapter.getInstance( ).send( SpotMessages.getInstance( ).getServiceMenuSwitchData( activate ));
	}


	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	/**
	 * The Class MgpResultAction process the mgp results for SERVICE_MENU_SWITCH answers.
	 */
	private final class MgpResultAction implements EdtEventAction<MgpResultEvent>
	{
		@Override
		public void onInvokeEvent(MgpResultEvent event)
		{
			if (event.getMessageName( ).equals( SpotMessageType.SERVICE_MENU_SWITCH.getName( ) ))
			{
				processSecureMenuAnswer( event );
			}
		}

		/**
		 * Process secure menu request answer.
		 * 
		 * @param event the mgp result event.
		 */
		private void processSecureMenuAnswer(MgpResultEvent event)
		{
			receivedResponse = true;
			errorInResponse = true;
			if (event.isValid( ))
			{
				errorInResponse = false;
			}
			else
			{
				logger.error( "Error = " + EdtConvert.byteToHexString( event.getAckCode( ) ) );
			}

			wakeup( );
		}

		@Override
		public Class<MgpResultEvent> getEventClass()
		{
			return MgpResultEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name( ) +
					"' AND " + MgpResultEvent.MESSAGE_NAME + " IN ('" + SpotMessageType.SERVICE_MENU_SWITCH.getName( ) + "')";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}
	}
	
	/**
	 * Wakeup.
	 */
	protected synchronized void wakeup()
	{
		this.notify( );
	}
	
	/**
	 * The Class EdtClientSocketDisconnectAction process the socket disconnect event.
	 */
	private final class EdtClientSocketDisconnectAction implements EdtEventAction<EdtClientSocketEvent>
	{
		@Override
		public void onInvokeEvent(EdtClientSocketEvent event)
		{
			SocketEventType eventtype = (SocketEventType) event.getEventType( );
			
			if(eventtype.equals(SocketEventType.SOCKET_DISCONNECT))
			{
				logger.debug("SOCKET_DISCONNECT");
				receivedResponse = true;
				errorInResponse = true;
				wakeup( );
			}
		}

		@Override
		public Class<EdtClientSocketEvent> getEventClass()
		{
			return EdtClientSocketEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.SOCKET_DISCONNECT.name( ) + "'";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}
	}
}
