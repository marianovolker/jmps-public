
package com.gilbarco.globaltools.spotdecommissioning.state;

import static com.gilbarco.globaltools.spotdecommissioning.model.SPOTDecommissioningContext.PROCESS_ADAPTER;
import static com.gilbarco.globaltools.spotdecommissioning.model.SPOTDecommissioningContext.SERVICE_MANAGER;
import static com.gilbarco.globaltools.spotdecommissioning.model.SPOTDecommissioningContext.TIMER_ADAPTER;

import java.io.UnsupportedEncodingException;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent.EdtTimerMode;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.mgp.domain.ArrayContainer;
import com.gilbarco.globaltools.mgp.domain.ChildElement;
import com.gilbarco.globaltools.mgp.domain.Container;
import com.gilbarco.globaltools.mgp.domain.ContainerType;
import com.gilbarco.globaltools.mgp.domain.Field;
import com.gilbarco.globaltools.mgp.domain.Message;
import com.gilbarco.globaltools.mgp.domain.Protocol;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.exceptions.MessageDataException;
import com.gilbarco.globaltools.mgp.exceptions.PredicateException;
import com.gilbarco.globaltools.spotprotocol.SpotProtocol;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageErrorCode;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;

public class SPOTDecommissioningStateManager
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log		logger		= LogFactory.getLog( SPOTDecommissioningStateManager.class );
	private static final String				STATE_TIMER	= "STATE_TIMEOUT";

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
	private Issuer							issuer;
	private EdtEventAction<EdtTimerEvent>	timeoutAction;
	private boolean							started;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SPOTDecommissioningStateManager() throws EdtServiceException
	{
		super( );
		this.issuer = new Issuer( this.getClass( ) );
		this.timeoutAction = new StateTimeOut( );
		this.started = false;
		subscribeToEvents( );
		changeState( SPOTDecommissioningState.IDLE.name( ), "Idle" );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public void startDecommissioning()
	{
		started = true;
		changeState( SPOTDecommissioningState.CONNECT_AND_LOGIN.name( ), "ConnectAndLogin" );
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	protected void subscribeToEvents() throws EdtServiceException
	{
		SERVICE_MANAGER.subscribe( new MgpResultsActions( ) );
	}

	protected void startStateTimer(long timeout)
	{
		TIMER_ADAPTER.callElapsedTimeAction( STATE_TIMER, timeout, EdtTimerMode.ONE_SHOT, this.timeoutAction, this.issuer );
	}

	public void stopStateTimer()
	{
		TIMER_ADAPTER.callStopTimerAction( STATE_TIMER );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void changeState(String to, String description)
	{
		Map<String, Object> properties = new HashMap<String, Object>( );
		changeState( null, to, description, properties );
	}

	private void changeState(String to, String description, Map<String, Object> properties)
	{
		changeState( null, to, description, properties );
	}

	private void changeState(String from, String to, String description, Map<String, Object> properties)
	{
		try
		{
			if (EdtStringUtil.isNullOrEmpty( from ) || PROCESS_ADAPTER.getCurrentState( ).equals( from ))
			{
				PROCESS_ADAPTER.callChangeToState( PROCESS_ADAPTER.getCurrentState( ), to, this.issuer, description, properties );
			}
			else
			{
				String error = "WRONG process event, can't move from state " + from + " to " + to + " actual state is: " + PROCESS_ADAPTER.getCurrentState( );
				logger.error( "error: " + error );
			}
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	// --------------------------------------------------------------------------------
	// MgpResultsActions mgp event listener
	// --------------------------------------------------------------------------------
	public class MgpResultsActions implements EdtEventAction<MgpResultEvent>
	{
		private static final String	KEY_STATUS_DATA	= "KeyStatusData";
		private static final String	ACK_KEY			= "Ack";
		public static final String	PPN_KEY			= "PPN";
		private static final String	ENTRIES_KEY		= "Entries_15";

		Protocol					spotProtocol;
		SpotProtocol				sp				= new SpotProtocol( );
		Map<String, Object>			eventProperties	= new HashMap<String, Object>( );

		public MgpResultsActions()
		{

		}

		@Override
		public Class<MgpResultEvent> getEventClass()
		{
			return MgpResultEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name( ) + "' AND " + MgpResultEvent.MESSAGE_NAME + " IN ('"
					+ SpotMessageType.LOGIN.getName( ) + "', '"
					+ SpotMessageType.HARDWARE_INFO.getName( ) + "', '"
					+ SpotMessageType.APPLICATION_KEY_DOWNLOAD.getName( ) + "', '"
					+ SpotMessageType.KEY_STATUS.getName( ) + "', '" + "')";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}

		@Override
		public void onInvokeEvent(MgpResultEvent event)
		{
			if (!started)
			{
				return;
			}

			if (!event.isValid( ))
			{
				logger.info( "MgpResults Error - " + event.getMessageName( ) + " error code: " + event.getAckCode( ) + " - " + SpotMessageErrorCode.getDescription( event.getAckCode( ) ) );
			}

			SpotMessageType msg = SpotMessageType.getByName( event.getMessageName( ) );
			switch(msg)
			{
			// spot answer to login request
				case LOGIN :
					eventProperties.clear( );
					eventProperties.put( ACK_KEY, event.getAckCode( ) );
					changeState( SPOTDecommissioningState.GET_LOGIN_RESPONSE.name( ), "GET_LOGIN_RESPONSE", eventProperties );
					break;
				// spot answer to ppn info request
				case HARDWARE_INFO :
					eventProperties.clear( );
					eventProperties.put( ACK_KEY, event.getAckCode( ) );
					eventProperties.put( PPN_KEY, getPPNData( event ) );
					changeState( SPOTDecommissioningState.GET_PPN_INFO_RESPONSE.name( ), "GET_PPN_INFO_RESPONSE", eventProperties );
					break;
				// spot answer to key status request
				case KEY_STATUS :
					eventProperties.clear( );
					eventProperties.put( ACK_KEY, event.getAckCode( ) );
					eventProperties.put( KEY_STATUS_DATA, getKeyData( event ) );
					changeState( SPOTDecommissioningState.GET_KEY_STATUS_TABLE_RESPONSE.name( ), "GET_KEY_STATUS_TABLE_RESPONSE", eventProperties );
					break;
				// spot answer to clear key request
				case APPLICATION_KEY_DOWNLOAD :
					eventProperties.clear( );
					eventProperties.put( ACK_KEY, event.getAckCode( ) );
					changeState( SPOTDecommissioningState.GET_CLEAR_KEY_RESPONSE.name( ), "GET_CLEAR_KEY_RESPONSE", eventProperties );
					break;
				default :
					logger.info( "invalid spot msg - " + msg.name( ) );
					break;
			}
		}

		@SuppressWarnings("unchecked")
		private String getPPNData(MgpResultEvent event)
		{
			String strPPN="";
			Map<String, Object> dataResponse = (Map<String, Object>) event.getDecodeMap( ).get( event.getDecodeMap( ).keySet( ).iterator( ).next( ) );					
			
			int numItems = EdtConvert.stringToInt( ((String) dataResponse.get("NumItems")) );
			if ( event.isValid( ) && numItems > 0 )
			{
				Protocol spotProtocol;
				SpotProtocol sp = new SpotProtocol( );
				try
				{
					spotProtocol = sp.buildSpotProtocol( );
					List<Message> messages = spotProtocol.findMessage( SpotMessageType.HARDWARE_INFO.getName( ), ContainerType.RESPONSE );

					Message msgCopy = messages.get( 0 ).getWorkingCopy( );
					msgCopy.decode( event.getRawData( ) );

					for (ChildElement child : msgCopy.getChildren( ))
					{
						if (Container.class.isInstance( child ))
						{
							ArrayContainer<Container> itemArray = (ArrayContainer<Container>) child;
							Collection<ChildElement> col = itemArray.getChildren( );

							for (ChildElement childElement : col)
							{
								if (childElement instanceof Container)
								{
									Collection<Field> colItem = ((Container) childElement).findChildElement( "ItemID" );
									Field fldPPNId = (Field) colItem.toArray( )[0];
									byte[] fileId = (fldPPNId != null)? (fldPPNId.getData( )):(null);

									if( (fileId.length > 0) && ((byte)fileId[0]) == ((byte)0x01) )
									{
										colItem = ((Container) childElement).findChildElement( "ItemData" );
										Field fldPPNData = (Field) colItem.toArray( )[0];
										String sHexData;
										try
										{
											sHexData = new String(fldPPNData.getData( ), "US-ASCII");
											int index = (sHexData.length( )/2);
											strPPN = sHexData.substring( index );
											break;
										}
										catch (UnsupportedEncodingException e)
										{
											logger.error( "ERROR getting response from UPM [" + e.getMessage( ) + "]." );
										}
									}
								}
							}
						}
					}
				}
				catch (PredicateException|MessageDataException e)
				{
					logger.error( "ERROR getting response from UPM [" + e.getMessage( ) + "]." );
				}
			}
			return strPPN;
		}

		@SuppressWarnings("rawtypes")
		private Object getKeyData(MgpResultEvent event)
		{
			@SuppressWarnings("unchecked")
			Map<String, Object> map = (Map<String, Object>) event.getDecodeMap( ).get( event.getDecodeMap( ).keySet( ).iterator( ).next( ) );
			HashMap data = (HashMap) map.get( ENTRIES_KEY );
			return data;
		}
	}

	// --------------------------------------------------------------------------------
	// StateTimeOut timer event listener
	// --------------------------------------------------------------------------------
	public class StateTimeOut implements EdtEventAction<EdtTimerEvent>
	{

		@Override
		public Class<EdtTimerEvent> getEventClass()
		{
			return EdtTimerEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.ISSUER_FILTER + "='" + issuer + "'";
		}

		@Override
		public boolean justOnce()
		{
			return true;
		}

		@Override
		public void onInvokeEvent(EdtTimerEvent event)
		{
			if (!started)
			{
				return;
			}
			logger.info( "StateTimeOut - " + event.getEventType( ).name( ) );
			changeState( SPOTDecommissioningState.SHOW_ERROR.name( ), "Timed out..." );
		}
	}
	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
