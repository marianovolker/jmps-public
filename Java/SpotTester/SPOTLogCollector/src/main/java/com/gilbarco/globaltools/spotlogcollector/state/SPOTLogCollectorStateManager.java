
package com.gilbarco.globaltools.spotlogcollector.state;

import static com.gilbarco.globaltools.spotlogcollector.model.SPOTLogCollectorContext.PROCESS_ADAPTER;
import static com.gilbarco.globaltools.spotlogcollector.model.SPOTLogCollectorContext.SERVICE_MANAGER;
import static com.gilbarco.globaltools.spotlogcollector.model.SPOTLogCollectorContext.TIMER_ADAPTER;

import java.util.HashMap;
import java.util.Map;

import javax.xml.bind.DatatypeConverter;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent.EdtTimerMode;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.mgp.domain.Protocol;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.spotprotocol.SpotProtocol;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageErrorCode;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;

public class SPOTLogCollectorStateManager
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log		logger		= LogFactory.getLog( SPOTLogCollectorStateManager.class );
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
	public SPOTLogCollectorStateManager() throws EdtServiceException
	{
		super( );
		this.issuer = new Issuer( this.getClass( ) );
		this.timeoutAction = new StateTimeOut( );
		this.started = false;
		subscribeToEvents( );
		changeState( SPOTLogCollectorState.IDLE.name( ), "Idle" );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public void startCollection()
	{
		started = true;
		changeState( SPOTLogCollectorState.CONNECT_AND_LOGIN.name( ), "ConnectAndLogin" );
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
		Protocol			spotProtocol;
		SpotProtocol		sp				= new SpotProtocol( );
		Map<String, Object>	eventProperties	= new HashMap<String, Object>( );

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
					+ SpotMessageType.FILE_UPLOAD.getName( ) + "', '"
					+ SpotMessageType.FILE_BLOCK_UPLOAD.getName( ) + "', '" + "')";
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

			if ( isInvalidEvent( event ) )
			{
				logger.info( "MgpResults Error - " + event.getMessageName( ) + " error code: "+ event.getAckCode( ) + " - "+ SpotMessageErrorCode.getDescription(event.getAckCode( )));
				changeState( SPOTLogCollectorState.SHOW_ERROR.name( ), event.getMessageName( ) + " command Fail" );
				return;
			}

			SpotMessageType msg = SpotMessageType.getByName( event.getMessageName( ) );
			switch(msg)
			{
				// spot answer to file upload request
				case LOGIN :
					eventProperties.clear( );
					changeState( SPOTLogCollectorState.SETUP_PROCESS.name( ), "ConnectAndLogin", eventProperties );
					break;
				// spot answer to file upload request
				case FILE_UPLOAD :
					eventProperties.clear( );
					eventProperties.put( "Ack", event.getAckCode( ) );
					eventProperties.put( "FileSz", getKeyData( event, "FileSz" ) );
					changeState( SPOTLogCollectorState.GET_FILE_UPLOAD_RESPONSE.name( ), "FileUploadResponse", eventProperties );
					break;
				// spot answer to file block upload request
				case FILE_BLOCK_UPLOAD :
					eventProperties.clear( );
					eventProperties.put( "Ack", event.getAckCode( ) );
					eventProperties.put( "Offset", getKeyData( event, "Offset" ) );
					eventProperties.put( "BlkSize", getKeyData( event, "BlkSize" ) );
					eventProperties.put( "BlkData", getBlkData( event ) );
					changeState( SPOTLogCollectorState.GET_FILE_BLOCK_UPLOAD_RESPONSE.name( ), "FileBlockUploadResponse", eventProperties );
					break;
				default :
					logger.info( "invalid spot msg - " + msg.name( ) );
					break;
			}
		}

		private boolean isInvalidEvent(MgpResultEvent event)
		{
			// Don't consider message invalid when ack is cannot_read_resource and message type is FileUpload.
			boolean ackok = event.getAckCode( ) != SpotMessageErrorCode.CANNOT_READ_RESOURCE.getCode();
			String messageName = event.getMessageName( );
			String spotmsgname = SpotMessageType.FILE_UPLOAD.getName( );
			boolean messagenameok = !messageName.equals( spotmsgname );
			return (!event.isValid( )) && (ackok || messagenameok);
		}

		private String getKeyData(MgpResultEvent event, String key)
		{
			@SuppressWarnings("unchecked")
			Map<String, Object> map = (Map<String, Object>) event.getDecodeMap( ).get( event.getDecodeMap( ).keySet( ).iterator( ).next( ) );
			return (String) map.get( key );
		}

		private byte[] getBlkData(MgpResultEvent event)
		{
			@SuppressWarnings("unchecked")
			Map<String, Object> map = (Map<String, Object>) event.getDecodeMap( ).get( event.getDecodeMap( ).keySet( ).iterator( ).next( ) );

			byte[] byHex = new byte[]{};
			String sHex = (String) map.get( "BlkData" );
			if (sHex != null)
			{
				try
				{
					if (sHex.length() > 2)
					{
						byHex = DatatypeConverter.parseHexBinary( sHex.substring(2) );
					}
				}
				catch (Exception e)
				{
					logger.error( "Error while parsing BlkData field." + e.getMessage(), e);
				}
			}
			return (byte[]) byHex;
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
			changeState( SPOTLogCollectorState.SHOW_ERROR.name( ), "Timed out..." );
		}
	}
	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
