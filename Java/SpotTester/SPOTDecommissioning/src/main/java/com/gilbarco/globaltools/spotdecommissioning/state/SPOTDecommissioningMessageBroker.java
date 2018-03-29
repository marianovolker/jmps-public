
package com.gilbarco.globaltools.spotdecommissioning.state;

import static com.gilbarco.globaltools.spotdecommissioning.model.SPOTDecommissioningContext.PROCESS_ADAPTER;
import static com.gilbarco.globaltools.spotdecommissioning.model.SPOTDecommissioningContext.SERVICE_MANAGER;

import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent.ProcessEventType;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageErrorCode;
import com.gilbarco.globaltools.spottester.managers.SpotTesterDataManager;
import com.gilbarco.globaltools.spottester.model.SpotMessages;
import com.gilbarco.globaltools.spotdecommissioning.main.gui.SPOTDecommissioningModel;
import com.gilbarco.globaltools.spotdecommissioning.main.gui.SPOTDecommissioningProgressObserver;

public class SPOTDecommissioningMessageBroker
{

	private static final int					MAX_KEY_COUNT	= 48;

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log			logger			= LogFactory.getLog( SPOTDecommissioningMessageBroker.class );

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private SPOTDecommissioningProgressObserver	progressObserver;
	private SPOTDecommissioningModel			model;

	private Vector<KeyDataElement>				keysToClearVector;
	private Iterator<KeyDataElement>			keysIterator;
	private int									currentKey;
	private boolean								checkAllCleared;

	private BufferedWriter						decommisioningLog;

	private Object	currentPPN;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SPOTDecommissioningMessageBroker(SPOTDecommissioningModel model, SPOTDecommissioningProgressObserver progressObserver) throws EdtServiceException
	{
		super( );
		this.model = model;
		this.progressObserver = progressObserver;
		this.subscribeToEvents( );
	}

	private void createStatusKeyTable()
	{
		if (keysToClearVector != null)
		{
			keysToClearVector.clear( );
		}
		else
		{
			keysToClearVector = new Vector<KeyDataElement>( );
		}
	}

	private void changeState(SPOTDecommissioningState to, String description)
	{
		Map<String, Object> properties = new HashMap<String, Object>( );
		changeState( to, description, properties );
	}

	private void changeState(SPOTDecommissioningState to, String description, Map<String, Object> properties)
	{
		try
		{
			PROCESS_ADAPTER.callChangeToState( PROCESS_ADAPTER.getCurrentState( ), to.name( ), new Issuer( this.getClass( ) ), description, properties );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
	}

	private void connectAndLogin()
	{
		try
		{
			SpotTesterDataManager.getInstance( ).configureAndLogin( model.getCurrentIp( ), model.getCurrentPort( ), model.getSpotClientID( ) );
			if (SpotTesterDataManager.getInstance( ).isConnect( ))
			{
				logger.info( "Login Requested to: " + model.getCurrentIp( ) + ":" + model.getCurrentPort( ) );
			}
			else
			{
				logger.error( "ConnectAndLoging Error - connecting to: " + model.getCurrentIp( ) + ":" + model.getCurrentPort( ) );
				changeState( SPOTDecommissioningState.SHOW_ERROR, "Cannot connect to device" );
			}
		}
		catch (EdtServiceException e)
		{
			logger.error( "ConnectAndLoging Error - " + e.getMessage( ) );
			changeState( SPOTDecommissioningState.SHOW_ERROR, "Cannot connect to device" );
		}
	}

	private void disconnect()
	{
		logger.info( "Close Connection." );
		SpotTesterDataManager.getInstance( ).closeSocket( );
	}

	public void getLoginResponse(Map<String, Object> data)
	{
		byte ack = (byte) data.get( "Ack" );
		if (ack != SpotMessageErrorCode.NO_ERROR.getCode( ))
		{
			changeState( SPOTDecommissioningState.SHOW_ERROR, "Login failed" );
			return;
		}
		changeState( SPOTDecommissioningState.SEND_PPN_INFO_REQUEST, "SetupProcess" );
	}

	protected void sendPPNInfoRequest()
	{
		byte[] message = SpotMessages.getInstance( ).getPPNbyHardwareInfo( );
		SpotTesterDataManager.getInstance( ).sendMessage( message );
	}

	public void getPPNInfoResponse(Map<String, Object> data)
	{
		byte ack = (byte) data.get( "Ack" );
		if (ack != SpotMessageErrorCode.NO_ERROR.getCode( ))
		{
			changeState( SPOTDecommissioningState.SHOW_ERROR, "Retrieve PPN failed" );
			return;
		}
		currentPPN = data.get( "PPN" );
		changeState( SPOTDecommissioningState.SETUP_PROCESS, "SetupProcess" );		
	}

	private void sendKeyStatusTableRequest(Map<String, Object> data)
	{
		byte[] message = SpotMessages.getInstance( ).getKeyStatusRequestData( );
		SpotTesterDataManager.getInstance( ).sendMessage( message );
	}

	private void getKeyStatusTableResponse(Map<String, Object> data)
	{
		byte ack = (byte) data.get( "Ack" );
		if (ack != SpotMessageErrorCode.NO_ERROR.getCode( ))
		{
			changeState( SPOTDecommissioningState.SHOW_ERROR, "Retrieve keys failed" );
			return;
		}

		getKeyStatusData( data );
		if (hasToCheckAllCleared( ))
		{
			if (allCleared( data ))
			{
				changeState( SPOTDecommissioningState.SHOW_SUCCESS, "AllCleared" );
			}
			else
			{
				changeState( SPOTDecommissioningState.SHOW_ERROR, "Clear keys failed - Try Again" );
			}
		}
		else
		{
			changeState( SPOTDecommissioningState.GET_KEY_TO_CLEAR, "KeyStatusTableResponse" );
		}
	}

	@SuppressWarnings("unchecked")
	private void getKeyStatusData(Map<String, Object> properties)
	{
		this.keysToClearVector.clear( );
		HashMap<String, HashMap<String, Object>> data = (HashMap<String, HashMap<String, Object>>) properties.get( "KeyStatusData" );
		int index = 1;
		while (index <= MAX_KEY_COUNT)
		{
			HashMap<String, Object> element = data.get( getNextKeyMapEntrie( index ) );
			byte use = getMapByteElement( element.get( "KeyUse" ) );
			byte type = getMapByteElement( element.get( "KeyType" ) );
			this.keysToClearVector.add( new KeyDataElement( index, use, type ) );
			if (use != (byte) 0x00)
			{
				logger.debug( "status key:" + index + " - " + use + " - " + type );
			}
			index++;
		}
		keysIterator = keysToClearVector.iterator( );
	}

	/*
	 * returns the key string to get the Edt map entry for the KeyStatus table generated as Mgp result
	 */
	public String getNextKeyMapEntrie(int index)
	{
		return "KeyTable_" + (13 + (index * 3));
	}

	private void sendClearKeyRequest()
	{
		byte keyUse = (byte) 0x01;
		byte transportKeyId = (byte) 0xFF;
		byte keyType = (byte) 0x03; // rsa
		byte[] keyData = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x21, 0x44, (byte) 0xDF, 0x1C }; // empty rsa encrypted key used to clear

		byte[] message = SpotMessages.getInstance( ).getApplicationKeyDownloadRequestData( currentKey, keyUse, transportKeyId, keyType, keyData );
		SpotTesterDataManager.getInstance( ).sendMessage( message ); 
	}

	private void getClearKeyResponse(Map<String, Object> data)
	{
		byte resultcode = (byte) data.get( "Ack" );
		if (resultcode != SpotMessageErrorCode.NO_ERROR.getCode( ))
		{
			keyToDecommissioningLog( "FAIL", resultcode );
			changeState( SPOTDecommissioningState.GET_KEY_TO_CLEAR, "ClearKeyRequest" );
		}
		else
		{
			keyToDecommissioningLog( "SUCCESS", resultcode );
			changeState( SPOTDecommissioningState.GET_KEY_TO_CLEAR, "ClearKeyRequest" );
		}

	}

	private void keyToDecommissioningLog(String result, byte resultcode)
	{
		try
		{
			DateFormat dateFormat = new SimpleDateFormat( "yyyy/MM/dd HH:mm:ss" );
			Date date = new Date( );
			String clearKeydata = dateFormat.format( date ) + "," + currentPPN + "," + currentKey + "," + result + "," + SpotMessageErrorCode.getDescription( resultcode );
			decommisioningLog.write( clearKeydata );
			decommisioningLog.newLine( );
		}
		catch (IOException e)
		{
			logger.error( "Error creating Decommissioning Log - " + e.getMessage( ) );
			changeState( SPOTDecommissioningState.SHOW_ERROR, "Log file update failed" );
		}
	}

	/* return true if all the keys are cleared */
	private boolean allCleared(Map<String, Object> data)
	{
		Iterator<KeyDataElement> it = keysToClearVector.iterator( );
		while (it.hasNext( ))
		{
			KeyDataElement next = it.next( );
			if ( next.type != (byte) 0x00 )
			{
				return false;
			}
		}
		return true;
	}

	private void checkMoreKeysToDelete()
	{
		if (keysIterator.hasNext( ))
		{
			KeyDataElement element = keysIterator.next( );
			this.currentKey = element.getId( );
			if (element.getType( ) != (byte) 0x00 ) 
			{
				logger.debug( "Delete key:" + currentKey + "-" + element.getType( ) + "-" + element.getUse( ) );
				changeState( SPOTDecommissioningState.SEND_CLEAR_KEY_REQUEST, "MoreKeysToDelete" );
			}
			else
			{
				checkMoreKeysToDelete( );
			}
		}
		else
		{
			setCheckAllCleared( true );
			logger.info( "All Keys have been cleared - Check the status again" );
			changeState( SPOTDecommissioningState.SEND_KEY_STATUS_TABLE_REQUEST, "DecommissioningComplete" );
		}
	}

	private void processSuccess()
	{
		changeState( SPOTDecommissioningState.CLEANUP_PROCESS, "CleanUpProcess" );
	}

	private void processError()
	{
		changeState( SPOTDecommissioningState.CLEANUP_PROCESS, "CleanUpProcess" );
	}

	private void processSpotError()
	{
		changeState( SPOTDecommissioningState.SHOW_ERROR, "Spot Error" );
	}

	private void setuProcess()
	{
		setCheckAllCleared( false );
		createStatusKeyTable( );
		createDecommissioningLog( );
		changeState( SPOTDecommissioningState.SEND_KEY_STATUS_TABLE_REQUEST, "Key Status Request" );
	}

	private void createDecommissioningLog()
	{
		try
		{
			decommisioningLog = new BufferedWriter( new OutputStreamWriter( new FileOutputStream( model.getSPOTDecommissioningLogName( ) , true ) ) );
		}
		catch (FileNotFoundException e)
		{
			logger.error( "Error creating Decommissioning Log - " + e.getMessage( ) );
			changeState( SPOTDecommissioningState.SHOW_ERROR, "Log file update failed" );
		}
	}

	private void cleanUpProcess()
	{
		setCheckAllCleared( false );
		closeDecommissioningState( );
		changeState( SPOTDecommissioningState.DISCONNECT, "Disconnect" );
	}

	private void closeDecommissioningState()
	{
		if (decommisioningLog != null)
		{
			try
			{
				decommisioningLog.close( );
			}
			catch (IOException e)
			{
				logger.error( "Error closing Decommissioning Log - " + e.getMessage( ) );
			}
		}
	}

	private void setCheckAllCleared(boolean check)
	{
		checkAllCleared = check;
	}

	private boolean hasToCheckAllCleared()
	{
		return checkAllCleared;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void subscribeToEvents() throws EdtServiceException
	{
		SERVICE_MANAGER.subscribe( new StateChangedAction( ) );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class StateChangedAction implements EdtEventAction<EdtProcessEvent>
	{
		@Override
		public void onInvokeEvent(EdtProcessEvent event)
		{
			SPOTDecommissioningState appState = SPOTDecommissioningState.IDLE;
			appState = SPOTDecommissioningState.valueOf( event.getDestination( ) );
			switch(appState)
			{
				case IDLE :
					break;
				case CONNECT_AND_LOGIN :
					connectAndLogin( );
					progressObserver.notifyConnected( );
					break;
				case GET_LOGIN_RESPONSE :
					getLoginResponse( event.getEventProperties( ) );
					break;
				case SEND_PPN_INFO_REQUEST:
					sendPPNInfoRequest( );
					break;				
				case GET_PPN_INFO_RESPONSE:
					getPPNInfoResponse( event.getEventProperties( ) );
					break;				
				case SETUP_PROCESS :
					setuProcess( );
					break;
				case SEND_KEY_STATUS_TABLE_REQUEST :
					sendKeyStatusTableRequest( event.getEventProperties( ) );
					progressObserver.notifyProgress( );
					break;
				case GET_KEY_STATUS_TABLE_RESPONSE :
					getKeyStatusTableResponse( event.getEventProperties( ) );
					progressObserver.notifyProgress( );
					break;
				case GET_KEY_TO_CLEAR :
					checkMoreKeysToDelete( );
					progressObserver.notifyProgress( );
					break;
				case SEND_CLEAR_KEY_REQUEST :
					sendClearKeyRequest( );
					progressObserver.notifyProgress( );
					break;
				case GET_CLEAR_KEY_RESPONSE :
					getClearKeyResponse( event.getEventProperties( ) );
					progressObserver.notifyProgress( );
					break;
				case SPOT_ERROR :
					processSpotError( );
					break;
				case SHOW_SUCCESS :
					processSuccess( );
					progressObserver.notifySuccess( );
					break;
				case SHOW_ERROR :
					processError( );
					progressObserver.notifyFailed( event.getDescription( ) );
					break;
				case CLEANUP_PROCESS :
					cleanUpProcess( );
					break;
				case DISCONNECT :
					disconnect( );
					progressObserver.notifyDisconnected( );
					break;
				default :
					logger.info( "state not valid " + appState.name( ) );
					break;
			}
		}

		@Override
		public Class<EdtProcessEvent> getEventClass()
		{
			return EdtProcessEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + ProcessEventType.STATE_CHANGED.name( ) + "' AND "
					+ EdtProcessEvent.DESTINATION_STATE_FILTER + " IN ('"
					+ SPOTDecommissioningState.IDLE + "', '"
					+ SPOTDecommissioningState.CONNECT_AND_LOGIN + "', '"
					+ SPOTDecommissioningState.GET_LOGIN_RESPONSE + "', '"
					+ SPOTDecommissioningState.SEND_PPN_INFO_REQUEST + "', '"
					+ SPOTDecommissioningState.GET_PPN_INFO_RESPONSE + "', '"
					+ SPOTDecommissioningState.SETUP_PROCESS + "', '"
					+ SPOTDecommissioningState.SEND_KEY_STATUS_TABLE_REQUEST + "', '"
					+ SPOTDecommissioningState.GET_KEY_STATUS_TABLE_RESPONSE + "', '"
					+ SPOTDecommissioningState.GET_KEY_TO_CLEAR + "', '"
					+ SPOTDecommissioningState.SEND_CLEAR_KEY_REQUEST + "', '"
					+ SPOTDecommissioningState.GET_CLEAR_KEY_RESPONSE + "', '"
					+ SPOTDecommissioningState.SHOW_ERROR + "', '"
					+ SPOTDecommissioningState.SPOT_ERROR + "', '"
					+ SPOTDecommissioningState.SHOW_SUCCESS + "', '"
					+ SPOTDecommissioningState.CLEANUP_PROCESS + "', '"
					+ SPOTDecommissioningState.DISCONNECT + "')";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}
	}

	class KeyDataElement
	{
		private int		id;
		private byte	use;
		private byte	type;

		public KeyDataElement(int id, byte use, byte type)
		{
			this.id = id;
			this.use = use;
			this.type = type;
		}

		public int getId()
		{
			return id;
		}

		public byte getUse()
		{
			return use;
		}

		public byte getType()
		{
			return type;
		}
	}

	public byte getMapByteElement(Object obj)
	{
		if (obj.getClass( ) != String.class)
											return 0;
		String sValue = (String) obj;
		int iHexMarkFirstAttempt = sValue.indexOf( "0x" );
		int iHexMark = -1;
		String sParsed = null;

		if (iHexMarkFirstAttempt >= 0) // prefix found
			iHexMark = iHexMarkFirstAttempt;
		else
			iHexMark = sValue.indexOf( "0X" );

		if (iHexMark >= 0) // any of the two prefixes is found
			sParsed = sValue.substring( iHexMark + 2 );
		else
			sParsed = sValue;

		return javax.xml.bind.DatatypeConverter.parseHexBinary( sParsed )[0];
	}

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
