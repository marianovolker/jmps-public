/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ConnectionEngine.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines;

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.io.UnsupportedEncodingException;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Observable;
import java.util.Observer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.io.DataCommunicationLayer;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api.AuthenticationResult;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api.IAuthenticationBlock;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionAborted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionClosed;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionGranted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionLost;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionNotGranted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionOpen;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.IWaitCondition;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.WaitCondition;
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
import com.gilbarco.globaltools.spottester.managers.SpotTesterDataManager;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/** @class ConnectionEngine ConnectionEngine.java "com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines"
 * @brief This class provides an engine which will contain the Target Unit Connection Business Logic.
 *
 * @since 1.0.0
 */
public class ConnectionEngine extends Observable
{
	/** @brief Logger instance for this class type */
	private static final transient Log logger = LogFactory.getLog( ConnectionEngine.class );

	/** @brief Byte mask to cast from int to byte */ 
	private static final byte BYTE_MASK = ((byte)0xFF);

	/** @brief Default target unit connection timeout measured in milliseconds */ 
	public static final long DEFAULT_TIMEOUT_MILLISECONDS = 10000L;
	
	/** @brief Maximum value allowed for external apps client ids */ 
	public static final byte EXTERNAL_APP_MAX_CLIENT_ID = ((byte)0xFF);
	/** @brief Minimum value allowed for external apps client ids */ 
	public static final byte EXTERNAL_APP_MIN_CLIENT_ID = ((byte)0x80);

	/** @brief Default Uri address */ 
	public static final String DEFAULT_CONNECTION_URI  = "localhost";
	/** @brief Default port number */ 
	public static final int    DEFAULT_CONNECTION_PORT = 5200;

	/** @brief Instance synchronization monitor */ 
	private Object monitor;

	/** @brief Uri address to which the socket connects  */ 
	private String sUri;
	/** @brief Port number to which the socket connects */ 
	private int iPort;
	/** @brief Client Id used by the current connection */ 
	private byte byClientId;
	
	/** @brief Authentication wrapper to be provided by the user  */ 
	private IAuthenticationBlock authenticationBlock;

	/** @brief Keeps track of the connection status */ 
	private boolean bConnected;

	/** @brief Keeps track of the PPN serial number */ 
	private String sPPN;
	
	/** @brief Expression which track the connection proper initialization */ 
	private IWaitCondition waitConditionExpression;
	
	/** @brief Subscribe to engine events which have to be observed.
	 * 
	 * @throws EdtServiceException If an error occurs while subscribing to events.
	 * 
	 * @since 1.0.0
	 */ 
	protected void subscribeToEvents () throws EdtServiceException
	{
		SERVICE_MANAGER.subscribe(new EdtEventAction<EdtClientSocketEvent>()
		{
			/** @brief Entry point for the coming event. 
			 *
			 * @param event [In] Incoming event to be processed.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
			public void onInvokeEvent(EdtClientSocketEvent event)
			{
				synchronized(monitor)
				{
					switch ((SocketEventType) event.getEventType())
					{
					case CONNECTED:
						bConnected = true;
						break;
					case CLOSED_SOCKET:
						bConnected = false;
						if (authenticationBlock != null)
						{
							authenticationBlock.setAuthenticated(sPPN,false);
						}
						break;
					case SOCKET_DISCONNECT:
						logger.debug( "SOCKET_DISCONNECT" );
						bConnected = false;
						if (authenticationBlock != null)
						{
							authenticationBlock.setAuthenticated(sPPN,false);
						}
						ConnectionEngine.this.setChanged();
						ConnectionEngine.this.notifyObservers(new EngineProgressNotificationConnectionLost("Connection Lost")); 
						break;
					case CONNECT:
					case INCOMING_DATA:
					case OUTGOING_DATA:
					case CLOSE_SOCKET:
						break;
					default:
						break;
					}
				}
			}

			/** @brief Provides information regarding the underline event class. 
			 * 
			 * @return Class<EdtClientSocketEvent> information.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
			public Class<EdtClientSocketEvent> getEventClass()
			{
				return EdtClientSocketEvent.class;
			}

			/** @brief Provides the subscription filter to the service to which it is being subscribed to. 
			 * 
			 * @return Subscription filter.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
			public String getSubscriptionFilter()
			{
				return EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.CONNECTED.name() + "' OR "
						+ EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.CLOSED_SOCKET.name() + "' OR "
						+ EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.SOCKET_DISCONNECT.name() + "'";
			}

			/** @brief Informs if this event action should be disposed after being called. 
			 * 
			 * @return True if the action shoud be disposed after being called. False otherwise.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
			public boolean justOnce()
			{
				return false;
			}
		});

		SERVICE_MANAGER.subscribe(new EdtEventAction<MgpResultEvent>()
		{
			/** @brief List of fields retrieved from the currently SPOT Mgp message event received and parsed, to be used in subsequent processes */ 
			private Map<String, Object>	eventProperties;

			/** @brief Anonymous initializer
			 * 
			 * @return Current anonymous instance.
			 *
			 * @since 1.0.0
			 */
			public EdtEventAction<MgpResultEvent> initialize ()
			{
				this.eventProperties = new HashMap<String, Object>();
				return this;
			}

			/** @brief Entry point for the coming event. 
			 *
			 * @param event [In] Incoming event to be processed.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
			public void onInvokeEvent(MgpResultEvent event)
			{
				synchronized(monitor)
				{
					if (bConnected)
					{
						try
						{
							if (isInvalidEvent(event))
							{
								logger.warn("Warning. MgpResults Error - " + event.getMessageName() + " -  Error code: " + event.getAckCode() + " - " + SpotMessageErrorCode.getDescription(event.getAckCode()));
								return;
							}
				
							SpotMessageType msg = SpotMessageType.getByName(event.getMessageName());
							switch(msg)
							{
							case HARDWARE_INFO :
								this.eventProperties.clear();
								this.eventProperties.put("Ack",event.getAckCode());
								Map<String, String> itemArray = getItemArray(event);
								this.eventProperties.put("ItemArray",itemArray);
								getHardwareInfo(this.eventProperties);
								WaitCondition.wakeCondition(monitor);
								break;
							default :
								logger.warn("Warning. Unexpected SPOT message - '" + msg.name() + "'.");
								break;
    						}
    					}
    					catch (Exception e)
    					{
    						logger.error("Unknown Exception thrown at the MgpResultEvent/EdtEventAction Listener. " + e.getMessage());
    						logger.debug( e );
    					}
					}
				}
			}

			/** @brief Provides information regarding the underline event class. 
			 * 
			 * @return Class<EdtClientSocketEvent> information.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
			public Class<MgpResultEvent> getEventClass()
			{
				return MgpResultEvent.class;
			}

			/** @brief Provides the subscription filter to the service to which it is being subscribed to. 
			 * 
			 * @return Subscription filter.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
			public String getSubscriptionFilter()
			{
				return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name( ) + "' AND " + MgpResultEvent.MESSAGE_NAME + " IN ('"
						+ SpotMessageType.HARDWARE_INFO.getName( ) + "', '" + "')";
			}

			/** @brief Informs if this event action should be disposed after being called. 
			 * 
			 * @return True if the action shoud be disposed after being called. False otherwise.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
			public boolean justOnce()
			{
				return false;
			}

			/** @brief SPOT Mgp message processed validator.
			 * 
			 * @param event [In] SPOT MgpResultEvent to be validated.
			 * 
			 * @return True if the MgpResultEvent is NOT valid. False otherwise. 
			 * 
			 * @see MgpResultEvent
			 * 
			 * @since 1.0.0
			 */
			private boolean isInvalidEvent (MgpResultEvent event)
			{
				// Don't consider message invalid when ack is cannot_read_resource and message type is FileUpload.
				boolean ackok = event.getAckCode() == SpotMessageErrorCode.NO_ERROR.getCode();
				String messageName = event.getMessageName();
				String spotmsgname = SpotMessageType.HARDWARE_INFO.getName();
				boolean messagenameok = !messageName.equals(spotmsgname);
				return (!event.isValid()) && (ackok || messagenameok);
			}

			/** @brief Extracts the Spot Mgp message field value corresponding to the given key from the given Mgp message processed.
			 * 
			 * @param event [In] SPOT MgpResultEvent to be searched.
			 * @param key   [In] SPOT Key to match in search.
			 * 
			 * @return String containing the value obtained for the given Mgp message and key. 
			 * 
			 * @see MgpResultEvent
			 * 
			 * @since 1.0.0
			 */
			private String getKeyData (MgpResultEvent event, String key)
			{
				@SuppressWarnings("unchecked")
				Map<String, Object> map = (Map<String, Object>) event.getDecodeMap().get(event.getDecodeMap().keySet().iterator().next());
				return (String) map.get(key);
			}

			/** @brief Extracts the Spot Mgp message ItemArray field value corresponding to the given key from the given Mgp message processed.
			 * 
			 * @param event [In] is the SPOT MgpResultEvent to be searched.
			 * 
			 * @return Map containing the array of items obtained for the given Mgp message and key. 
			 * 
			 * @see MgpResultEvent
			 * 
			 * @since 1.0.0
			 */
			private Map<String, String> getItemArray (MgpResultEvent event)
			{
				Map<String, String> result = new HashMap<String, String>();
				@SuppressWarnings("unchecked")
				Map<String, Object> map = (Map<String, Object>)event.getDecodeMap().get(event.getDecodeMap().keySet().iterator().next());
				
				int numItems = EdtConvert.stringToInt( ((String) map.get("NumItems")) );
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

							if (Container.class.isInstance(child))
							{
								@SuppressWarnings("unchecked")
								ArrayContainer<Container> itemArray = (ArrayContainer<Container>)child;
								Collection<ChildElement> col = itemArray.getChildren( );
								for (ChildElement childElement : col)
								{
									if (childElement instanceof Container)
									{
										try
										{
											byte itemId = 0x00;
											String itemDataHex = "";

											Collection<Field> colItem = null;
											Field ItemField = null;
											byte[] itemAux = null;
											
											colItem = ((Container)childElement).findChildElement("ItemID");
											ItemField = (Field)colItem.toArray()[0];
											try
											{
												itemAux = (ItemField != null)?(ItemField.getData()):(null);
											}
											catch (MessageDataException e)
											{
												logger.error("Error. Failed obtaining item id." + e.getMessage());
												logger.debug( e );
												throw e;
											}

											if ((itemAux != null) && (itemAux.length > 0))
											{
												itemId = (byte)itemAux[0];

												colItem = ((Container) childElement).findChildElement( "ItemData" );
												ItemField = (Field)colItem.toArray()[0];
												try
												{
													itemDataHex = (ItemField != null)?(new String(ItemField.getData(),"US-ASCII")):("");
												}
												catch (MessageDataException e)
												{
													logger.error("Error. Failed obtaining item data for Item - '" + Integer.toHexString(0xFF & itemId) + "'." + e.getMessage());
													logger.debug( e );
													throw e;
												}
												catch (UnsupportedEncodingException e)
												{
													logger.error("Error. Failed encoding data for Item - '" + Integer.toHexString(0xFF & itemId) + "'." + e.getMessage());
													logger.debug( e );
													throw e;
												}

												switch (itemId)
												{
												case 0x01:
													{
														if (itemDataHex != null)
														{
															int index = (itemDataHex.length()/2);
															result.put("PPN", itemDataHex.substring(index));
														}
													}
													break;
												default:
													logger.warn("Warning. Unhandled Item - '" + Integer.toHexString(0xFF & itemId) + "'.");
													break;
												}
											}
										}
										catch (MessageDataException | UnsupportedEncodingException e)
										{
											logger.warn("Warning. Continuing with next Item.");
										}
									}
								}
							}
						}
					}
					catch (PredicateException | MessageDataException e)
					{
						logger.error("Error. Failed while decoding message data." + e.getMessage());
						logger.debug( e );
					}
				}
				return result;
			}
		}.initialize());
	}
	
	/** @brief Process a SPOT Hardware Info.
	 * 
	 * @param List of fields and their values to be processed from this response. 
	 * 
	 * @since 1.0.0
	 */	
	private void getHardwareInfo (Map<String, Object> data)
	{
		byte ack = (byte) data.get("Ack");
		if (ack == SpotMessageErrorCode.NO_ERROR.getCode()) //Received Ok
		{
			@SuppressWarnings("unchecked")
			Map<String, String> itemArray = (Map<String, String>) data.get("ItemArray");
			if (itemArray != null)
			{
				sPPN = itemArray.get("PPN");
			}
		}
	}
	
	/** @brief Internal connection implementation which connects and Logs-in to target unit. 
	 * 
	 * @param monitor [In] Is the lock monitor to be used for wait and synchronization.
	 * 
	 * @return True if connection and login was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	private boolean callConnectAndLogin (Object monitor)
	{
		boolean bResult = false;
		try
		{
			if ((byClientId >= EXTERNAL_APP_MIN_CLIENT_ID) && (byClientId <= EXTERNAL_APP_MAX_CLIENT_ID))
			{
				if (authenticationBlock != null)
				{
					authenticationBlock.setAuthenticated(sPPN,false);
				}
				sPPN = "";
				SpotTesterDataManager.getInstance().configureAndLogin(sUri,iPort,byClientId);
				bConnected = true;
				callSend(SpotMessages.getInstance().getPPNbyHardwareInfo());
				bResult = WaitCondition.waitConditionExpression(monitor,waitConditionExpression,DEFAULT_TIMEOUT_MILLISECONDS);
				if (bResult == true)
				{
					this.setChanged();
					this.notifyObservers(new EngineProgressNotificationConnectionOpen("Connection Open")); 
				}
				else
				{
					callClose();
				}
			}
			else
			{
				logger.warn("Warning. Client Id value is out of bounds.");
			}
		}
		catch (EdtServiceException e)
		{
			logger.error("Error. Internal error while connecting socket : " + e.getMessage());
			logger.debug( e );
		}
		return bResult;
	}
	
	/** @brief Internal connection implementation which connects and Logs-in to target unit with provided uri address, port and client id.
	 *
	 * @param observers [In] List of engine progress observers to be notified upon updates.
	 * @param uri       [In] Uri address to be connected to.
	 * @param port      [In] Port number to be connected to.
	 * @param clientId  [In] Client Id number to be used for this application.
	 * @param monitor   [In] Is the lock monitor to be used for wait and synchronization.
	 * 
	 * @return True if connection and login was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	private boolean callConnectAndLogin (Collection<Observer> observers, String uri, int port, byte clientId, Object monitor)
	{
		boolean bResult = false;
		try
		{
			if ((clientId >= EXTERNAL_APP_MIN_CLIENT_ID) && (clientId <= EXTERNAL_APP_MAX_CLIENT_ID))
			{
				if (observers != null)
				{
    				for (Observer observer : observers)
    				{
    					deleteObserver(observer);
    					addObserver(observer);
    				}
				}

				if (authenticationBlock != null)
				{
					authenticationBlock.setAuthenticated(sPPN,false);
				}
				sPPN = "";
				SpotTesterDataManager.getInstance().configureAndLogin(uri,port,clientId);
				sUri = uri;
				iPort = port;
				byClientId = clientId;
				bConnected = true;
				callSend(SpotMessages.getInstance().getPPNbyHardwareInfo());
				bResult = WaitCondition.waitConditionExpression(monitor,waitConditionExpression,DEFAULT_TIMEOUT_MILLISECONDS);
				if (bResult == true)
				{
					this.setChanged();
					this.notifyObservers(new EngineProgressNotificationConnectionOpen("Connection Open")); 
				}
				else
				{
					callClose();
				}
			}
			else
			{
				logger.warn("Warning. Client Id value is out of bounds.");
			}
		}
		catch (EdtServiceException e)
		{
			logger.error("Error. Internal error while connecting socket : " + e.getMessage());
			logger.debug( e );
		}
		return bResult;
	}
	
	/** @brief Internal send implementation which asynchronously sends data to connected target unit.
	 *
	 * @param data [In] Buffer holding the data that needs to be sent.
	 * 
	 * @return True if successfully queued for sending (no done yet, waiting to be sent later). False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	private boolean invokeSend(byte [] data)
	{
		boolean bResult = false;
		try
		{
			SpotTesterDataManager.getInstance().invokeSendData(data);
			bResult = true;
		}
		catch (Exception e)
		{
			logger.error("Error. Internal error while async sending data to socket : " + e.getMessage());
			logger.debug( e );
		}
		return bResult;
	}

	/** @brief Internal send implementation which synchronously sends data to connected target unit.
	 *
	 * @param data [In] Buffer holding the data that needs to be sent.
	 * 
	 * @return True if successfully sent. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	private boolean callSend(byte [] data)
	{
		boolean bResult = false;
		try
		{
			SpotTesterDataManager.getInstance().sendMessage(data);
			bResult = true;
		}
		catch (Exception e)
		{
			logger.error("Error. Internal error while sending data to socket : " + e.getMessage());
			logger.debug( e );
		}
		return bResult;
	}

	/** @brief Internal close implementation which closes connection to target unit.
	 *
	 * bDeleteObservers [In] Indicate if observers should be removed upon close.
	 *
	 * @since 1.0.0
	 */ 
	private void callClose ()
	{
		try
		{
			SpotTesterDataManager.getInstance().closeSocket();
		}
		catch (Exception e)
		{
			logger.error("Error. Internal error while closing socket : " + e.getMessage());
			logger.debug( e );
		}
		if (authenticationBlock != null)
		{
			authenticationBlock.setAuthenticated(sPPN,false);
		}
		sPPN = "";
		bConnected = false;
	}

	/** @brief Internal close implementation which closes connection to target unit and notifies observers.
	 *
	 * bDeleteObservers [In] Indicate if observers should be removed upon close.
	 *
	 * @since 1.0.0
	 */ 
	private void callCloseAndNotify ()
	{
		boolean bNotify = bConnected;
		callClose();
		if (bNotify)
		{
    		this.setChanged();
    		this.notifyObservers(new EngineProgressNotificationConnectionClosed("Connection Closed"));
		}
	}

	/** @brief Checks if connection to target unit is active. Connects to it if not with provided uri address, port and client id.
	 *
	 * @param observers [In] List of engine progress observers to be notified upon updates.
	 * @param uri       [In] Uri address to be connected to.
	 * @param port      [In] Port number to be connected to.
	 * @param clientId  [In] Client Id number to be used for this application.
	 * @param monitor   [In] Is the lock monitor to be used for wait and synchronization.
	 * 
	 * @return True if connection and login was already performed or successfully done at this call. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	private boolean checkConnectionAndLogin(Collection<Observer> observers, String uri, int port, int clientId, Object monitor)
	{
		boolean bResult = false;
		byte byClientIdAux = ((byte)(clientId & BYTE_MASK));
		
		if ((bConnected == true) && ((sUri.equals(uri) == false) || (iPort != port) || (byClientId != byClientIdAux)))
		{
			callCloseAndNotify();
		}

		if (bConnected == false)
		{
			bResult = callConnectAndLogin(observers,uri,port,byClientIdAux,monitor);
		}
		else
		{
			bResult = true;
		}
		return bResult;
	}

	/** @brief Checks if connection to target unit was authenticated. Authenticate with it if not.
	 *
	 * @param monitor [In] Is the lock monitor to be used for wait and synchronization.
	 * 
	 * @return         AuthenticationResult.AUTHENTICATION_GRANTED if successfully authenticated or authentication was already performed. AuthenticationResult.AUTHENTICATION_DENIED if authentication failed. AuthenticationResult.AUTHENTICATION_CANCELED if authentication was not even attempted.
	 * 
	 * @since 1.0.0
	 */ 
	private AuthenticationResult checkAuthentication(Object monitor)
	{
		AuthenticationResult authResult = AuthenticationResult.AUTHENTICATION_DENIED;
		if (authenticationBlock != null)
		{
			boolean bConnectedAux = bConnected;

			if (bConnectedAux == false)
			{
				bConnectedAux = callConnectAndLogin(monitor);
			}

			if (bConnectedAux == true)
			{
				if (authenticationBlock.isAuthenticated(sPPN) == false)
				{
						authResult = authenticationBlock.authenticate(sPPN);
				}
				else
				{
					authResult = AuthenticationResult.AUTHENTICATION_GRANTED;
				}
			}
		}
		return authResult;
	}
	
	/** @brief Default constructor.
	 * 
	 * @throws EdtServiceException If an error occurs while subscribing to events.
	 * 
	 * @since 1.0.0
	 */ 
	public ConnectionEngine () throws EdtServiceException
	{
		this.monitor = new Object();
		this.sUri = DEFAULT_CONNECTION_URI;
		this.iPort = DEFAULT_CONNECTION_PORT;
		this.byClientId = EXTERNAL_APP_MIN_CLIENT_ID;

		this.authenticationBlock = null;
		
		this.bConnected = false;

		this.sPPN = "";

		this.waitConditionExpression = new IWaitCondition()
		{
			@Override
			public boolean expression()
			{
				return (sPPN.isEmpty() == false);
			}
		};
		
		this.subscribeToEvents();
	}

	/** @brief Get uri address to which the connection engine was connected to.
	 * 
	 * @return Uri address to which the connection engine was connected to.
	 * 
	 * @since 1.0.0
	 */ 
	public String getUri ()
	{
		String sResult = "";
		synchronized(monitor)
		{
			sResult = sUri;
		}
		return sResult;
	}

	/** @brief Get port number to which the connection engine was connected to.
	 * 
	 * @return Port number to which the connection engine was connected to.
	 * 
	 * @since 1.0.0
	 */ 
	public int getPort ()
	{
		int iResult = 0;
		synchronized(monitor)
		{
			iResult = iPort;
		}
		return iResult;
	}

	/** @brief Get client Id number used for this application.
	 * 
	 * @return Client Id number used for this application.
	 * 
	 * @since 1.0.0
	 */ 
	public int getClientId ()
	{
		int iResult = 0;
		synchronized(monitor)
		{
			iResult = byClientId;
		}
		return iResult;
	}

	/** @brief Get PPN from the connected Target Unit.
	 * 
	 * @return PPN from the connected Target Unit.
	 * 
	 * @since 1.0.0
	 */ 
	public String getPPN ()
	{
		String sResult = "";
		synchronized(monitor)
		{
			sResult = sPPN;
		}
		return sResult;
	}

	/** @brief Get connection status.
	 * 
	 * @return True if connected. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean isConnected ()
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			bResult = bConnected;
		}
		return bResult;
	}

	/** @brief Get login status.
	 * 
	 * @return True if logged. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean isLogged ()
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			bResult = bConnected;
		}
		return bResult;
	}

	/** @brief Get authentication status.
	 * 
	 * @return True if authenticated. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean isAuthenticated ()
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			if (authenticationBlock != null)
			{
				bResult = bConnected && authenticationBlock.isAuthenticated(sPPN);
			}
		}
		return bResult;
	}

	/** @brief Set the authentication implementation done by the user.
	 * 
	 * @param newAuthenticationBlock [In] Authentication implementation to be used.
	 * 
	 * @since 1.0.0
	 */ 
	public void setAuthenticationBlock (IAuthenticationBlock newAuthenticationBlock)
	{
		synchronized(monitor)
		{
			if (authenticationBlock != newAuthenticationBlock)
			{
				authenticationBlock = newAuthenticationBlock;
				authenticationBlock.setAuthenticated(sPPN,false);
			}
		}
	}
	
	/** @brief Get the current authentication implementation being used by this engine.
	 * 
	 * @return Current authentication implementation being used by this engine.
	 * 
	 * @since 1.0.0
	 */ 
	public IAuthenticationBlock getAuthenticationBlock ()
	{
		IAuthenticationBlock result = null;
		synchronized(monitor)
		{
			result = authenticationBlock;
		}
		return result;
	}

	/** @brief Connects and Logs-in to target unit with provided uri address, port and client id.
	 *
	 * @param observers [In] List of engine progress observers to be notified upon updates.
	 * @param uri       [In] Uri address to be connected to.
	 * @param port      [In] Port number to be connected to.
	 * @param clientId  [In] Client Id number to be used for this application.
	 * 
	 * @return True if connection and login was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean connectAndLogin (Collection<Observer> observers, String uri, int port, int clientId)
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			if (!bConnected)
			{
				bResult = callConnectAndLogin(observers,uri,port,((byte)(clientId & BYTE_MASK)),monitor);
				this.setChanged();
				if (bResult == true)
				{
					this.notifyObservers(new EngineProgressNotificationConnectionGranted("Connection Ready"));
				}
				else
				{
					this.notifyObservers(new EngineProgressNotificationConnectionNotGranted("Connection Denied")); 
				}
			}
		}
		return bResult;
	}

	/** @brief Connects and Logs-in to target unit. 
	 * 
	 * @return True if connection and login was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean connectAndLogin ()
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			if (!bConnected)
			{
				bResult = callConnectAndLogin(monitor);
				this.setChanged();
				if (bResult == true)
				{
					this.notifyObservers(new EngineProgressNotificationConnectionGranted("Connection Ready"));
				}
				else
				{
					this.notifyObservers(new EngineProgressNotificationConnectionNotGranted("Connection Denied")); 
				}
			}
		}
		return bResult;
	}

	/** @brief Reconnects and Logs-in to target unit with provided uri address, port and client id.
	 *
	 * @param observers [In] List of engine progress observers to be notified upon updates.
	 * @param uri       [In] Uri address to be connected to.
	 * @param port      [In] Port number to be connected to.
	 * @param clientId  [In] Client Id number to be used for this application.
	 * 
	 * @return True if connection and login was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean reconnectAndLogin (Collection<Observer> observers, String uri, int port, int clientId)
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			callCloseAndNotify();
			bResult = callConnectAndLogin(observers,uri,port,((byte)(clientId & BYTE_MASK)),monitor);
			this.setChanged();
			if (bResult == true)
			{
				this.notifyObservers(new EngineProgressNotificationConnectionGranted("Connection Ready"));
			}
			else
			{
				this.notifyObservers(new EngineProgressNotificationConnectionNotGranted("Connection Denied")); 
			}
		}
		return bResult;
	}
	
	/** @brief Reconnects and Logs-in to target unit. 
	 * 
	 * @return True if connection and login was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean reconnectAndLogin ()
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			callCloseAndNotify();
			bResult = callConnectAndLogin(monitor);
			this.setChanged();
			if (bResult == true)
			{
				this.notifyObservers(new EngineProgressNotificationConnectionGranted("Connection Ready"));
			}
			else
			{
				this.notifyObservers(new EngineProgressNotificationConnectionNotGranted("Connection Denied")); 
			}
		}
		return bResult;
	}
	
	/** @brief Updates connection to target unit if any parameter defers from the ones given or not connected. Uses provided uri address, port, client id and authentication requirement flag.
	 *
	 * @param observers           [In] List of engine progress observers to be notified upon updates.
	 * @param uri                 [In] Uri address to be connected to.
	 * @param port                [In] Port number to be connected to.
	 * @param clientId            [In] Client Id number to be used for this application.
	 * @param needsAuthentication [In] Flag which indicates if authentication should be present.
	 * 
	 * @return True if connection, login and authentication, if required, was already performed or successfully done at this call. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean updateConnectionAndLogin (Collection<Observer> observers, String uri, int port, int clientId, boolean needsAuthentication)
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			if (checkConnectionAndLogin(observers,uri,port,clientId,monitor))
			{
				if (needsAuthentication == true)
				{
					AuthenticationResult authResult = AuthenticationResult.AUTHENTICATION_DENIED;
					authResult = checkAuthentication(monitor);
					if (authResult == AuthenticationResult.AUTHENTICATION_GRANTED)
					{
						this.setChanged();
						this.notifyObservers(new EngineProgressNotificationConnectionGranted("Connection Ready",true));
						bResult = true;
					}
					else if (authResult == AuthenticationResult.AUTHENTICATION_CANCELED)
					{
						this.setChanged();
						this.notifyObservers(new EngineProgressNotificationConnectionGranted("Connection Ready"));
					}
					else if (authResult == AuthenticationResult.AUTHENTICATION_DENIED)
					{
						this.setChanged();
						this.notifyObservers(new EngineProgressNotificationConnectionNotGranted("Connection Denied",true)); 
					}
					else
					{
						this.setChanged();
						this.notifyObservers(new EngineProgressNotificationConnectionNotGranted("Connection Denied")); 
					}
				}
				else
				{
					this.setChanged();
					this.notifyObservers(new EngineProgressNotificationConnectionGranted("Connection Ready"));
					bResult = true;
				}
			}
			else
			{
				this.setChanged();
				this.notifyObservers(new EngineProgressNotificationConnectionNotGranted("Connection Denied")); 
			}
		}
		return bResult;
	}

	/** @brief Updates connection to target unit if any parameter defers from the ones given or not connected. Uses provided uri address, port and client id.
	 *
	 * @param observers [In] List of engine progress observers to be notified upon updates.
	 * @param uri       [In] Uri address to be connected to.
	 * @param port      [In] Port number to be connected to.
	 * @param clientId  [In] Client Id number to be used for this application.
	 * 
	 * @return True if connection and login was already performed or successfully done at this call. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean updateConnectionAndLogin (Collection<Observer> observers, String uri, int port, int clientId)
	{
		return updateConnectionAndLogin(observers,uri,port,clientId,false);
	}

	/** @brief Abort current connection.
	 * 
	 * @param Observers to be included to observers list for this abort
	 * 
	 * @since 1.0.0
	 */	
	public void abortConnection (Collection<Observer> observers)
	{
		synchronized (monitor)
		{
			if (observers != null)
			{
    			for (Observer observer : observers)
    			{
    				this.deleteObserver(observer);
    				this.addObserver(observer);
    			}
			}
			this.setChanged();
			this.notifyObservers(new EngineProgressNotificationConnectionAborted("Connection Aborted")); 
			this.deleteObservers();
		}
	}
	
	/** @brief Commands the authentication implementation to force the target unit to forget current authentication.
	 *
	 * @since 1.0.0
	 */ 
	public void forgetAuthentication ()
	{
		synchronized(monitor)
		{
			if (authenticationBlock != null)
			{
				authenticationBlock.forgetAuthentication(sPPN);
			}
		}
	}

	/** @brief Asynchronously sends data to connected target unit.
	 *
	 * @param data [In] Buffer holding the data that needs to be sent.
	 * 
	 * @return True if successfully queued for sending (no done yet, waiting to be sent later). False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean asyncSend (byte [] data)
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			bResult = invokeSend(data);
		}
		return bResult;
	}

	/** @brief Synchronously sends data to connected target unit.
	 *
	 * @param data [In] Buffer holding the data that needs to be sent.
	 * 
	 * @return True if successfully sent. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean send (byte [] data)
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			bResult = callSend(data);
		}
		return bResult;
	}

	/** @brief Closes connection to target unit.
	 *
	 * bDeleteObservers [In] Indicate if observers should be removed upon close.
	 *
	 * @since 1.0.0
	 */ 
	public void close (boolean bDeleteObservers)
	{
		synchronized(monitor)
		{
			callCloseAndNotify();

			if (bDeleteObservers == true)
			{
				deleteObservers();
			}
		}
	}

	/** @brief Closes connection to target unit.
	 *
	 * @since 1.0.0
	 */ 
	public void close ()
	{
		close(false);
	}

	/** @brief Get a DataCommunicationLayer wrapper around this connection engine.
	 *
	 * @return DataCommunicationLayer wrapper around this connection engine.
	 *
	 * @since 1.0.0
	 */ 
	public DataCommunicationLayer getDataCommunicationLayerWrapper ()
	{
		return new DataCommunicationLayer()
		{
			@Override
			public void invokeSendData(byte[] data) throws Exception
			{
				ConnectionEngine.this.asyncSend(data);
			}

			@Override
			public void callSendData(byte[] data) throws Exception
			{
				ConnectionEngine.this.send(data);
			}
		};
	}
}
