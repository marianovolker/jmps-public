
package com.gilbarco.globaltools.spottester.managers;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.edt.services.process.EdtProcessAdapter;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerAdapter;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent.EdtTimerMode;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;
import com.gilbarco.globaltools.spottester.model.SpotServiceDefinitionsFactory;
import com.gilbarco.globaltools.spottester.model.SpotState;

/**
 * @Class SpotTesterStateController
 * 
 * @Description This entity is in charge of the state machine mechanism. The states are defined in {@linkplain SpotTesterState}.
 *              <p>
 *              {@link SpotTesterPingTimerManager} is started by this class when a success login answer arrive and is stopped when a logout request is sent.
 * 
 * @Services <ul>
 *           <li>EdtProcessService
 *           </ul>
 * 
 * @see SpotTesterState
 * @see SpotTesterPingTimerManager
 * 
 * @version 1.0.0
 * 
 * @author jrubstein
 * 
 * */

public class SpotTesterStateManager
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger					= LogFactory.getLog( SpotTesterStateManager.class );
	private static final String			PING_TIMEOUT_TIMER_NAME	= "pingTimeoutTimer";
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
	private EdtProcessAdapter			spotProcess;
	private EdtTimerAdapter				timer;
	private Issuer						localIssuer;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public SpotTesterStateManager() throws EdtServiceException
	{
		try
		{
			this.spotProcess = (EdtProcessAdapter) EdtServiceContext.getServiceManager( ).getServiceAdapter( SpotServiceDefinitionsFactory.getProcessDefinition( ) );
			this.timer = (EdtTimerAdapter) EdtServiceContext.getServiceManager( ).getServiceAdapter( SpotServiceDefinitionsFactory.getTimerDefinition( ) );
			this.spotProcess.callChangeToState( "", SpotState.DISCONNECTED.name( ), "" );
			this.localIssuer = new Issuer( this.getClass( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}
		subscribeToEvents( );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void subscribeToEvents()
	{
		try
		{
			EdtServiceContext.getServiceManager( ).subscribe( new EdtEventAction<MgpResultEvent>( )
			{
				@Override
				public void onInvokeEvent(MgpResultEvent event)
				{
					try
					{
						if ((spotProcess.getCurrentState( ) == SpotState.DISCONNECTED.name( ) 
							|| spotProcess.getCurrentState( ) == SpotState.SPOT_ERROR.name( ) ) 
							&& event.isValid( )
							)
						{
							spotProcess.callChangeToState( spotProcess.getCurrentState( ), SpotState.LOGGED.name( ), "" );
							startPingTimer( SpotMessages.getInstance( ).getPingTimeoutValue( ) );
						}
						
						//TODO add the login result error.
					}
					catch (EdtServiceException e)
					{
						logger.error( e.getMessage( ) );
					}
				}

				@Override
				public Class<MgpResultEvent> getEventClass()
				{
					return MgpResultEvent.class;
				}

				@Override
				public boolean justOnce()
				{
					return false;
				}

				@Override
				public String getSubscriptionFilter()
				{
					return MgpResultEvent.MESSAGE_NAME + "='" + SpotMessageType.LOGIN.getName( )
							+ "' AND " + MgpEvent.EVENT_TYPE_FILTER + "='"
							+ EdtMgpType.RESPONSE_RESULT + "'";
				}
			} );

			EdtServiceContext.getServiceManager( ).subscribe(
				new EdtEventAction<EdtClientSocketEvent>( )
				{
					@Override
					public void onInvokeEvent(EdtClientSocketEvent event)
					{
						reset( );						
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
						return EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.CLOSE_SOCKET.name( ) + "' OR "
								+ EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.SOCKET_DISCONNECT.name( ) + "'";
					}
				} );

			EdtServiceContext.getServiceManager( ).subscribe(
								new EdtEventAction<EdtClientSocketEvent>( )
								{
									@Override
									public void onInvokeEvent(EdtClientSocketEvent event)
									{
										try
										{
											SpotTesterDataManager.getInstance( ).closeSocket( );
											spotProcess.callChangeToState( spotProcess.getCurrentState( ), SpotState.SPOT_ERROR.name( ), "" );
										}
										catch (EdtServiceException e)
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
								},
								EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.SOCKET_DISCONNECT.name( ) + "'" );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}
	}

	protected void handleTimeOut()
	{
		reset( );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void reset()
	{
		try
		{
			spotProcess.callChangeToState( SpotState.LOGGED.name( ), SpotState.DISCONNECTED.name( ), "" );
			this.stopPingTimer( ); 
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}
	}

	private void startPingTimer(byte timeout)
	{
		if (timeout > 0)
		{
			timer.callElapsedTimeAction( PING_TIMEOUT_TIMER_NAME,
				(timeout * 1000) - 500, EdtTimerMode.CONTINUOUS, new PingTimeOut( ),
				localIssuer );
		}
	}

	private void stopPingTimer()
	{
		timer.callStopTimerAction( PING_TIMEOUT_TIMER_NAME );
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class PingTimeOut implements EdtEventAction<EdtTimerEvent>
	{

		@Override
		public Class<EdtTimerEvent> getEventClass()
		{
			return EdtTimerEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtTimerEvent.TIMER_NAME_FILTER + "='" + PING_TIMEOUT_TIMER_NAME + "'";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}

		@Override
		public void onInvokeEvent(EdtTimerEvent event)
		{
			SpotTesterDataManager.getInstance( ).sendMessage(
				SpotMessages.getInstance( ).getSpotPingData( ) );
		}
	}
	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
