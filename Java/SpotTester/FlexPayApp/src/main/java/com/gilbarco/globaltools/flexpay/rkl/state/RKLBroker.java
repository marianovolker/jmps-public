
package com.gilbarco.globaltools.flexpay.rkl.state;

import java.util.Arrays;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.EdtServiceManager;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.crypto.EdtCryptoAdapter;
import com.gilbarco.globaltools.edt.services.io.DataCommunicationLayer;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSClientSocketAdapter;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSClientSocketService;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSSocketDefinition;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSSocketEvent.TLSSocketEventType;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent.ProcessEventType;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent;
import com.gilbarco.globaltools.edt.util.EdtByteUtil;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.mgp.api.MessageType;
import com.gilbarco.globaltools.mgp.edt.service.MgpServiceAdapter;
import com.gilbarco.globaltools.rklnativeprotocol.api.RklNativeMessageType;
import com.gilbarco.globaltools.flexpay.rkl.context.RKLContext;
import com.gilbarco.globaltools.flexpay.rkl.context.RKLServiceDefinitionFactory;
import com.gilbarco.globaltools.flexpay.rkl.gui.FlexPayRklController;
import com.gilbarco.globaltools.flexpay.rkl.rklexception.*;
import com.gilbarco.globaltools.flexpay.rkl.state.RKLNativeStates;

public abstract class RKLBroker implements DataCommunicationLayer
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger						= LogFactory.getLog( RKLBroker.class );
	private static final byte[]			ACK_CHARACTER				=
																	{ 0x06 };
	private static AtomicInteger		sequenceNumber				= new AtomicInteger( 0 );
	@SuppressWarnings("unused")
	private static final long			serialVersionUID			= 619514742278024561L;
	private static Queue<byte[]>		messageParametersQueue		= null;
	private static final String			TIMEOUT_ERROR_DESCRIPTION	= "Timeout Error.";

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	protected synchronized Queue<byte[]> getQueue()
	{
		return messageParametersQueue;
	}

	static
	{
		messageParametersQueue = new LinkedBlockingQueue<byte[]>( );
	}
	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected Issuer						issuer;
	/**
	 * @return the issuer
	 */
	public Issuer getIssuer()
	{
		return issuer;
	}

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private EdtTLSClientSocketAdapter		socketAdapter	= null;
	private MgpServiceAdapter				mgpAdapter		= null;
	private EdtCryptoAdapter				cryptoAdapter	= null;
	private String							ip;
	private int								port;
	@SuppressWarnings("unused")
	private int								timeoutTime;
	private Issuer							socketIssuer;
	@SuppressWarnings("unused")
	private EdtEventAction<EdtTimerEvent>	timeoutAction;
	private byte[]							blockBuffer		= new byte[] {};
	protected FlexPayRklController			controller;
	private EdtServiceManager				serviceManager;
	private boolean 						registered = false; 
	
	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public RKLBroker(Issuer issuer)
	{
		super( );
		this.issuer = issuer;
		this.timeoutAction = new TimeOutAction( );
		try
		{
			serviceManager = EdtServiceContext.getServiceManager( );
			mgpAdapter = (MgpServiceAdapter) serviceManager.getServiceAdapter( RKLServiceDefinitionFactory.getRKLMgpDefinition( ) );
			cryptoAdapter = (EdtCryptoAdapter) serviceManager.getServiceAdapter( RKLServiceDefinitionFactory.getCryptoDefinition( ) );
			socketAdapter = null;
		}
		catch (EdtServiceException e)
		{
			logger.error( "RKLBroker -" + e.getMessage( ));
		}
	}

	public abstract void initialize();

	public boolean registerService()
	{
		if(!registered)
		{		
			EdtTLSClientSocketService tlsClientService = null;
			try
			{
				String strDefname = "TLSServerSocket" + ip + ":" + port;
				tlsClientService = new EdtTLSClientSocketService( new EdtTLSSocketDefinition( strDefname, EdtTLSClientSocketAdapter.class.getName( ), ip,port, false, null ) );
				serviceManager.registerService( tlsClientService );
				socketAdapter = (EdtTLSClientSocketAdapter) serviceManager.getServiceAdapter( strDefname );
				socketIssuer = tlsClientService.getServiceIssuer( );
				registered = true;
				
				return true;
			}
			catch (Exception e)
			{
				return false;
			}
		}
		else //reuse service
		{
			this.socketAdapter.getTLSSocketServiceDefinition( ).setUriIdentifier( ip );
			this.socketAdapter.getTLSSocketServiceDefinition( ).setLocalPort( port );
			return true;
		}
		
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public void callSendData(byte[] arg0) throws Exception
	{
		starTimer( );
		socketAdapter.callSendData( arg0 );
	}

	@Override
	public void invokeSendData(byte[] arg0) throws Exception
	{
		starTimer( );
		if (socketAdapter != null) socketAdapter.invokeSendData( arg0 );
	}

	public synchronized void send(String data) throws EdtServiceException
	{
		this.send( data.getBytes( ) );
	}

	public synchronized void send(byte[] data) throws EdtServiceException
	{
		logger.debug( "Message sent to RKL server : " + EdtConvert.bytesToHexString( data ) );
		if (!isACK( data ))
		{
			starTimer( );
		}
		if (socketAdapter != null) socketAdapter.invokeSendData( data );
		logger.debug( "Message sent." );
	}

	public void send(EdtTLSClientSocketEvent event) throws EdtServiceException
	{
		starTimer( );
		socketAdapter.invoke( event );
	}

	public void setProperties(String ip, String port, FlexPayRklController controller)
	{
		if (!ip.equals( "" )) this.ip = ip;
		if (!port.equals( "" )) this.port = Integer.valueOf( port );
		this.controller = controller;
	}

	public void setIp(String ip)
	{
		this.ip = ip;
	}

	public void setPort(int port)
	{
		this.port = port;
	}

	// public void setReconnections(int maxReconnection)
	// {
	// this.reconnections = maxReconnection;
	// }

	public void setTimeoutTime(int timeoutTime)
	{
		this.timeoutTime = timeoutTime;
	}

	public boolean connect()
	{
		try
		{
			socketAdapter.callConnect( );
			return true;
		}
		catch (EdtServiceException e)
		{
			logger.error( "connect -" + e.getMessage( ));
			return false;
		}
	}

	public boolean close(String message)
	{
		try
		{
			socketAdapter.callCloseSocket( message );
			logger.debug( "calling to close socket. uri = " + ip + ", port = " + port  );
			return true;
		}
		catch (EdtServiceException e)
		{
			logger.error( "close -" + e.getMessage( ));
			return false;
		}
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void starTimer()
	{
	}

	protected void stopTimer()
	{
	}

	protected void changeState(String to, String description)
	{
		try
		{
			if (to != null)
			{
				RKLContext.getProcessAdapter( ).callChangeToState( RKLContext.getProcessAdapter( ).getCurrentState( ), to, this.issuer, description );
			}
		}
		catch (EdtServiceException e)
		{
			logger.error( "changeState -" + e.getMessage( ));
		}
	}

	protected boolean checkCRC(byte[] data) throws EdtServiceException
	{
		byte[] crc = cryptoAdapter.callCRC16_8005( Arrays.copyOfRange( data, 0, data.length - 2 ), (byte) 1 );
		return Arrays.equals( crc, Arrays.copyOfRange( data, data.length - 2, data.length ) );
	}

	protected void changeToFinalState(String description) throws EdtServiceException
	{
		if (RKLContext.getProcessAdapter( ).getCurrentState( ) != RKLNativeStates.FINAL_STATE.name( ))
		{
			changeState( RKLNativeStates.FINAL_STATE.name( ), description );
			return;
		}
		throw new EdtServiceException( "Current state already is " + RKLNativeStates.FINAL_STATE.name( ) );
	}

	// the cast is correct because we will never send more than 255 messages.
	protected byte getNextSequenceNumber()
	{
		return (byte) sequenceNumber.incrementAndGet( );
	}

	protected byte getSequenceNumber()
	{
		return sequenceNumber.byteValue( );
	}

	protected void init()
	{
		try
		{
			suscribeToEvents( );
		}
		catch (EdtServiceException e)
		{
			logger.error( "RKLBroker - init -" + e.getMessage( ));
		}
	}

	protected void suscribeToEvents() throws EdtServiceException
	{
		serviceManager.subscribe( new EdtEventAction<EdtTLSClientSocketEvent>( )
		{

			@Override
			public Class<EdtTLSClientSocketEvent> getEventClass()
			{
				return EdtTLSClientSocketEvent.class;
			}

			@Override
			public String getSubscriptionFilter()
			{
				return EdtEvent.EVENT_TYPE_FILTER + "='" + TLSSocketEventType.INCOMING_DATA.name( ) + "' AND " + EdtEvent.ISSUER_FILTER + "='"
						+ socketIssuer + "'";
			}

			@Override
			public boolean justOnce()
			{
				return false;
			}

			@Override
			public void onInvokeEvent(EdtTLSClientSocketEvent event)
			{
				// Check if this message is a response from an old message.
				if (isACK( event.getData( ) ))
				{
					stopTimer( );
				}
				else
				{
					try
					{
						if (!checkBlockId( event.getData( ) ))
						{
							return;
						}
						byte[] data = blockBuffer.length != 0 ? blockBuffer : event.getData( );
						if (checkCRC( data ))
						{
							sendACK( );
							mgpAdapter.callValidator( data );
							if (blockBuffer.length != 0) blockBuffer = new byte[] {};
						}
						else
						{
							changeToFinalState( "CRC is wrong." );
						}
					}
					catch (EdtServiceException e)
					{
						logger.error( "onInvokeEvent -" + e.getMessage( ));
					}
				}
			}
		} );

		serviceManager.subscribe( new EdtEventAction<EdtProcessEvent>( )
		{

			@Override
			public Class<EdtProcessEvent> getEventClass()
			{
				return EdtProcessEvent.class;
			}

			@Override
			public String getSubscriptionFilter()
			{
				return EdtEvent.EVENT_TYPE_FILTER + "='" + ProcessEventType.STATE_CHANGED.name( ) + "'";
			}

			@Override
			public boolean justOnce()
			{
				return false;
			}

			@Override
			public void onInvokeEvent(EdtProcessEvent event)
			{
				try
				{
					RKLNativeStates state = RKLNativeStates.getStateByName( event.getDestination( ) );
					synchronized (state)
					{
						state.processEventProperties( getQueue( ), event.getEventProperties( ) );
						handleStatusChanged( state );
						switch(state)
						{
							case NOTIFY_ERROR :
								break;
							case FINAL_STATE :
								break;
							default :
								break;
						}
					}
				}
				catch (Exception e)
				{
					logger.error( "onInvokeEvent -" + e.getMessage( ));
				}

			}
		} );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void checkIP(String ip) throws RklException
	{
		if (ip.matches( ".*gilbarco\\.com" ))
		{
			return;
		}
		else
		{
			String[] dot = ip.split( "\\." );
			if (dot.length != 4) throw new RklException( "ip " + ip + " is not correctly formatted" );
			for (String s : dot)
			{
				int i = Integer.valueOf( s );
				if ((i > 255) || (i < 0)) throw new RklException( "ip " + ip + " is not valid" );
			}
		}
	}

	private void sendACK() throws EdtServiceException
	{
		logger.info( "Sending ACK..." );
		send( ACK_CHARACTER );
	}

	private boolean isACK(byte[] data)
	{
		if (data == null) return false;
		return (data.length == 1 && Arrays.equals( data, ACK_CHARACTER ));
	}

	private boolean checkBlockId(byte[] data) throws EdtServiceException
	{
		if (RklNativeMessageType.getByCmd( data ).equals( ((MessageType) RklNativeMessageType.REMOTE_CERTIFICATE_REQUEST).getName( ) )
			|| blockBuffer.length != 0)
		{
			blockBuffer = EdtByteUtil.concatBytes( blockBuffer, data );
			return checkCRC( blockBuffer );
		}
		return true;
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	class TimeOutAction implements EdtEventAction<EdtTimerEvent>
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
			return false;
		}

		@Override
		public void onInvokeEvent(EdtTimerEvent arg0)
		{

			changeState( RKLNativeStates.NOTIFY_ERROR.name( ), TIMEOUT_ERROR_DESCRIPTION );
		}
	}

	abstract protected void handleStatusChanged(RKLNativeStates state);

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
