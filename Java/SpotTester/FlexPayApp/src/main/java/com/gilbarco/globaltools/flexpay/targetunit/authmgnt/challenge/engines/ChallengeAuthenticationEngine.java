/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file RequestAuthenticationDialog.java
 * @author pcantarini
 * @date Tuesday, October 10, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.authmgnt.challenge.engines;

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Observable;
import java.util.Observer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api.AuthenticationResult;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.IWaitCondition;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.WaitCondition;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageErrorCode;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/** @class ChallengeAuthenticationEngine ChallengeAuthenticationEngine.java "com.gilbarco.globaltools.flexpay.token.engines"
 * @brief This class provides the challenge authentication engine.
 *
 * @see IAuthenticationStrategy
 *
 * @since 1.0.0
 */
public class ChallengeAuthenticationEngine extends Observable
{
	/** @brief Logger instance for this class type */
	private static final transient Log logger = LogFactory.getLog( ChallengeAuthenticationEngine.class );

	/** @brief Default target unit connection timeout measured in milliseconds */ 
	public static final long DEFAULT_TIMEOUT_MILLISECONDS = 10000L;

	/** @brief Instance synchronization monitor */ 
	private Object  monitor;
	/** @brief Flag to determine if the engine is currently processing */ 
	private boolean bProcessing;
	/** @brief Flag to determine if the engine is done with its request */ 
	private boolean bDone;
	/** @brief Flag to determine if the engine latest processing outcome */ 
	private boolean bSuccesful;
	
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
					if (bProcessing)
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
							case VANGUARD_MAINTENANCE :
								this.eventProperties.clear();
								this.eventProperties.put("Ack",event.getAckCode());
								getMaintenanceResponse(this.eventProperties);
								bDone = true;
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
						+ SpotMessageType.VANGUARD_MAINTENANCE.getName( ) + "', '" + "')";
			}

			/** @brief Informs if this event action should be disposed after being called. 
			 * 
			 * @return True if the action should be disposed after being called. False otherwise.
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
				String spotmsgname = SpotMessageType.VANGUARD_MAINTENANCE.getName();
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
		}.initialize());
	}

	/** @brief Sends a SPOT Maintenance Request.
	 * 
	 * @param challengePassword [In] Is the Challenge Password to be used for authentication.
	 *  
	 * @since 1.0.0
	 */	
	private void sendMaintenanceRequest (byte [] challengePassword)
	{
		final byte authMaintenanceId = 0x01; // This is an authenticated Maintenance
		byte[] message = SpotMessages.getInstance().getMaintenance(authMaintenanceId,challengePassword);
		try
		{
			ConnectionEngineSwingAdapter.getInstance().send(message);
		}
		catch (NullPointerException e)
		{
			logger.error("Error. Exception thrown while sending synchronouse data to target unit.\nEdtServiceException: '" + e.getMessage() + "'.");
		}
	}

	/** @brief Process a SPOT Maintenance Response.
	 * 
	 * @param List of fields and their values to be processed from this response. 
	 * 
	 * @since 1.0.0
	 */	
	private void getMaintenanceResponse (Map<String, Object> data)
	{
		byte ack = (byte) data.get("Ack");
		if (ack == SpotMessageErrorCode.NO_ERROR.getCode()) //Received Ok and has more blocks to request
		{
			bSuccesful = true;
		}
		else
		{
			bSuccesful = false;
		}
	}

	/** @brief Clean up internal information.
	 * 
	 * @since 1.0.0
	 */	
	private void cleanUp ()
	{
		this.bProcessing = false;
		this.bDone = false;

		this.bSuccesful = false;
	}

	/** @brief Internal representation of the successful condition.
	 * 
	 * @since 1.0.0
	 */	
	private boolean successfulExpression ()
	{
		return bSuccesful;
	}
	
	/** @brief Default constructor.
	 * 
	 * @throws EdtServiceException If an error occurs while subscribing to events.
	 * 
	 * @since 1.0.0
	 */ 
	public ChallengeAuthenticationEngine () throws EdtServiceException
	{
		this.monitor = new Object();

		this.waitConditionExpression = new IWaitCondition()
		{
			@Override
			public boolean expression()
			{
				return successfulExpression() || bDone;
			}
		};
		
		this.cleanUp();

		this.subscribeToEvents();
	}
	
	/** @brief Request authentication with the given Challenge Password.
	 *
	 * @param observers         [In]  List of engine progress observers to be notified upon updates.
	 * @param challengePassword [In]  Challenge Password to present to the connected target unit.
	 * 
	 * @return AuthenticationResult.AUTHENTICATION_GRANTED if request was successful. AuthenticationResult.AUTHENTICATION_DENIED if it failed or AuthenticationResult.AUTHENTICATION_INTERRUPTED if connection was lost during authentication.
	 * 
	 * @since 1.0.0
	 */ 
	public AuthenticationResult authenticate (Collection<Observer> observers, String challengePassword)
	{
		AuthenticationResult authResult = AuthenticationResult.AUTHENTICATION_DENIED;
		synchronized(monitor)
		{
			cleanUp();
			if ((challengePassword != null) && (challengePassword.isEmpty() == false))
			{

				if (observers != null)
				{
					for (Observer observer : observers)
					{
						deleteObserver(observer);
						addObserver(observer);
					}
				}

				this.bProcessing = true;

				logger.debug( "sending maintenance request - token/challengepwd:" + challengePassword );				
				sendMaintenanceRequest(challengePassword.getBytes());

				this.setChanged();
				this.notifyObservers(new EngineProgressNotification("Requesting Authentication"));
				WaitCondition.waitConditionExpression(monitor,waitConditionExpression,DEFAULT_TIMEOUT_MILLISECONDS);
	    		if (successfulExpression())
	    		{
	    			authResult = AuthenticationResult.AUTHENTICATION_GRANTED;
	    		}
	    		else if (this.bDone == false)
	    		{
	    			authResult = AuthenticationResult.AUTHENTICATION_INTERRUPTED;
	    		}

				this.setChanged();
	    		if (authResult == AuthenticationResult.AUTHENTICATION_GRANTED)
	    		{
	    			this.notifyObservers(new EngineProgressNotification("Authentication Granted")); 
	    		}
	    		else if (authResult == AuthenticationResult.AUTHENTICATION_DENIED)
	    		{
	    			this.notifyObservers(new EngineProgressNotification("Authentication Denied")); 
	    		}
	    		else
	    		{
	    			this.notifyObservers(new EngineProgressNotification("Authentication Interrupted")); 
	    		}

	    		this.bProcessing = false;
	    		this.bDone = false;
			}
		}
		return authResult;
	}

	/** @brief Abort current authentication.
	 * 
	 * @param Observers to be included to observers list for this abort
	 * 
	 * @since 1.0.0
	 */	
	public void abortAuthentication (Collection<Observer> observers)
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
			this.notifyObservers(new EngineProgressNotification("Authentication Aborted")); 
			this.deleteObservers();
		}
	}

	/** @brief Check if engine is currently working.
	 * 
	 * @return True if engine is currently working. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	public boolean isInProgress ()
	{
		boolean bResult = false;
		synchronized (monitor)
		{
			bResult = this.bProcessing;
		}
		return bResult;
	}

	/** @brief Check if engine last process outcome was successful.
	 * 
	 * @return True if engine last process outcome was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	public boolean wasSuccessful ()
	{
		boolean bResult = false;
		synchronized (monitor)
		{
			bResult = successfulExpression();
		}
		return bResult;
	}
}
