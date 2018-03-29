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
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.IWaitCondition;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.WaitCondition;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageErrorCode;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/** @class ChallengeRequestEngine ChallengeRequestEngine.java "com.gilbarco.globaltools.flexpay.token.engines"
 * @brief This class provides the challenge authentication engine.
 *
 * @see IAuthenticationStrategy
 */
public class ChallengeRequestEngine extends Observable
{
	/** @brief Logger instance for this class type */
	private static final transient Log logger = LogFactory.getLog( ChallengeRequestEngine.class );

	/** @brief Default target unit connection timeout measured in milliseconds */ 
	public static final long DEFAULT_TIMEOUT_MILLISECONDS = 10000L;
	
	/** @brief Instance synchronization monitor */ 
	private Object  monitor;
	/** @brief Flag to determine if the engine is currently processing */ 
	private boolean bProcessing;
	/** @brief Flag to determine if the engine is done with its request */ 
	private boolean bDone;

	/** @brief Keeps track of the PPN serial number */ 
	private String sChallengeNumber;
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
							//TODO: Replace with future request challenge number implementation
							case VANGUARD_MAINTENANCE:
								this.eventProperties.clear();
								this.eventProperties.put("Ack",event.getAckCode());
								getMaintenanceRequirementResponse(this.eventProperties);
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
				//TODO: Replace with future request challenge number implementation
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
				//TODO: Replace with future request challenge number implementation
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

	/** @brief Sends a SPOT Maintenance Requirement Request.
	 * 
	 * @param technicianNumber [In] Is the technician Id requesting the authentication requirements.
	 * 
	 * @since 1.0.0
	 */	
	private void sendMaintenanceRequirementRequest (String technicianNumber)
	{
		byte[] message = null; //SpotMessages.getInstance().getMaintenanceRequirement();
//		try
		{
			//--------------------------------------------------------------------
			//TODO: Replace with future request challenge number implementation --
			this.sChallengeNumber = "131111311113";
			this.sPPN = "15731202";
			//--------------------------------------------------------------------

//			ConnectionEngineSwingAdapter.getInstance().send(message);
		}
//		catch (NullPointerException e)
		{
//			logger.error("Error. Exception thrown while sending synchronouse data to target unit.\nEdtServiceException: '" + e.getMessage() + "'.");
		}
	}

	/** @brief Process a SPOT Maintenance Requirement Response.
	 * 
	 * @param List of fields and their values to be processed from this response. 
	 * 
	 * @since 1.0.0
	 */	
	private void getMaintenanceRequirementResponse (Map<String, Object> data)
	{
		byte ack = (byte) data.get("Ack");
		//TODO: Replace with future request challenge number implementation
		if (ack == SpotMessageErrorCode.NO_ERROR.getCode()) //Received Ok
		{
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

		this.sChallengeNumber = "";
		this.sPPN = "";
	}

	/** @brief Internal representation of the successful condition.
	 * 
	 * @since 1.0.0
	 */	
	private boolean successfulExpression ()
	{
		return ((sChallengeNumber.isEmpty() == false) && (sPPN.isEmpty() == false));
	}
	
	/** @brief Default constructor.
	 * 
	 * @throws EdtServiceException If an error occurs while subscribing to events.
	 * 
	 * @since 1.0.0
	 */ 
	public ChallengeRequestEngine () throws EdtServiceException
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
	
	/** @brief Get the last Challenge Number obtained from the connected Target Unit.
	 * 
	 * @return Challenge Number from the connected Target Unit.
	 * 
	 * @since 1.0.0
	 */ 
	public String getChallengeNumber ()
	{
		String sResult = "";
		synchronized(monitor)
		{
			sResult = sChallengeNumber;
		}
		return sResult;
	}

	/** @brief Get the last PPN obtained from the connected Target Unit.
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

	/** @brief Request Challenge Number required for authentication.
	 *
	 * @param observers        [In]  List of engine progress observers to be notified upon updates.
	 * @param technicianNumber [In]  Is the technician Id requesting the authentication requirements.
	 * @param ppn              [Out] PPN obtained from the connected target unit.
	 * @param challengeNumber  [Out] Challenge Number obtained from the connected target unit.
	 * 
	 * @return True if request was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean requestChallengeNumber (Collection<Observer> observers, String technicianNumber, final StringBuilder ppn, final StringBuilder challengeNumber)
	{
		boolean bResult = false;
		synchronized(monitor)
		{
			cleanUp();

			if (observers != null)
			{
    			for (Observer observer : observers)
    			{
    				deleteObserver(observer);
    				addObserver(observer);
    			}
			}

			this.bProcessing = true;

			ppn.setLength(0);
			challengeNumber.setLength(0);

			sendMaintenanceRequirementRequest(technicianNumber);

			this.setChanged();
			this.notifyObservers(new EngineProgressNotification("Requesting Challenge Number"));
			WaitCondition.waitConditionExpression(monitor,waitConditionExpression,DEFAULT_TIMEOUT_MILLISECONDS);
    		bResult = successfulExpression(); 
			this.setChanged();
    		if (bResult == true)
    		{
    			this.notifyObservers(new EngineProgressNotification("Obtained Challenge Number")); 
    		}
    		else
    		{
    			this.notifyObservers(new EngineProgressNotification("Failed obtaining Challenge Number")); 
    		}

			ppn.append(sPPN);
			challengeNumber.append(sChallengeNumber);

    		this.bProcessing = false;
    		this.bDone = false;
		}
		return bResult;
	}
	
	/** @brief Abort current request for challenge number.
	 * 
	 * @param observers [In] which are to be included to observers list for this abort
	 * 
	 * @since 1.0.0
	 */	
	public void abortRequestChallengeNumber (Collection<Observer> observers)
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
			this.notifyObservers(new EngineProgressNotification("Requesting Challenge Number Aborted")); 
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
