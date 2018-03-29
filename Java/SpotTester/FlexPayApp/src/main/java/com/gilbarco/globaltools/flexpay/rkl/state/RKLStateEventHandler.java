
package com.gilbarco.globaltools.flexpay.rkl.state;

import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.process.EdtProcessAdapter;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.flexpay.rkl.context.RKLContext;
import com.gilbarco.globaltools.flexpay.rkl.rklexception.RklException;
import com.gilbarco.globaltools.flexpay.rkl.rklnativeprotocol.RKLNativeMessagesHelper;
import com.gilbarco.globaltools.mgp.edt.service.MgpRawEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.rklnativeprotocol.api.RklNativeMessageType;

public class RKLStateEventHandler
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger			= LogFactory.getLog( RKLStateEventHandler.class );
	private static EdtProcessAdapter	processAdapter	= RKLContext.getProcessAdapter( );
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
	private Issuer						issuer;
	private EdtEventAction<MgpRawEvent>	responseAction;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public RKLStateEventHandler() throws EdtServiceException
	{
		super( );
		this.issuer = new Issuer( this.getClass( ) );
		this.responseAction = new ResponseAction( );
		subscribeToEvents( );
		this.changeState( null, RKLNativeStates.IDLE, "Ready", null );

	}

	public void startRklProcess()
	{
		int sernum = 12345678;
		Map<String, Object> messageParameters = new HashMap<String, Object>( );
		messageParameters.put( "SerialNumber", Integer.toString( sernum ).getBytes( ) );
		changeState( null, RKLNativeStates.SEND_SPOT_IDENTIFICATION, "Send spot identification", messageParameters );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void subscribeToEvents() throws EdtServiceException
	{
		EdtServiceContext.getServiceManager( ).subscribe( responseAction );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void changeState(String from, RKLNativeStates to, String description, Map<String, Object> messageParameters)
	{
		try
		{
			if (!processAdapter.getCurrentState( ).equals( to.name( ) ) && to.getStep( ) != 0)
			{
				logger.info( "Step: " + to.getStep( ) + " of " + RKLNativeStates.TOTAL_STATES );
			}
			if (EdtStringUtil.isNullOrEmpty( from ) || processAdapter.getCurrentState( ).equals( from ))
			{
				logger.info( "State change: " + processAdapter.getCurrentState( ) + " to " + to.getName( ) );
				processAdapter.callChangeToState( processAdapter.getCurrentState( ), to.name( ), this.issuer, description, messageParameters );
			}
			else
			{
				String error = "WRONG process event, can't move from state " + from + " to " + to.name( ) + " actual state is: "
								+ processAdapter.getCurrentState( );
				logger.error( "error: " + error );
			}
		}
		catch (EdtServiceException e)
		{
			logger.error( "changeState -" + e.getMessage( ));
		}
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	public class ResponseAction implements EdtEventAction<MgpRawEvent>
	{

		@Override
		public Class<MgpRawEvent> getEventClass()
		{
			return MgpRawEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name( ) + "'";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}

		@Override
		public void onInvokeEvent(MgpRawEvent event)
		{
			if (!event.isValid( ))
			{
				try
				{
					changeState( processAdapter.getCurrentState( ), RKLNativeStates.NOTIFY_ERROR, event.getMessageName( ) + " command Fail", null );
				}
				catch (EdtServiceException e)
				{
					logger.error( event );
				}
				return;
			}
			RKLNativeStates msg = RKLNativeStates.getStateByName( event.getMessageName( ) );
			Map<String, Object> messageParameters = new HashMap<String, Object>( );
			switch(msg)
			{
				case GET_SPOT_IDENTIFICATION :
					changeState( RKLNativeStates.GET_SPOT_IDENTIFICATION.name( ), RKLNativeStates.SEND_SPOT_IDENTIFICATION, "change state",
						messageParameters );
					break;
				case SEND_SPOT_IDENTIFICATION :
					changeState( RKLNativeStates.SEND_SPOT_IDENTIFICATION.name( ), RKLNativeStates.GET_SPOT_CERTIFICATE, "change state",
						messageParameters );
					break;
				case GET_SPOT_CERTIFICATE :
					changeState( RKLNativeStates.GET_SPOT_CERTIFICATE.name( ), RKLNativeStates.SEND_SPOT_CERTIFICATE, "change state",
						messageParameters );
					break;
				case SEND_SPOT_CERTIFICATE :
					changeState( RKLNativeStates.SEND_SPOT_CERTIFICATE.name( ), RKLNativeStates.SEND_SPOT_CERTIFICATE_VERIFICATION, "change state",
						messageParameters );
					break;
				case SEND_SPOT_CERTIFICATE_VERIFICATION :
					changeState( RKLNativeStates.SEND_SPOT_CERTIFICATE_VERIFICATION.name( ), RKLNativeStates.CONTACT_GILBARCO, "change state",
						messageParameters );
					break;
				case CONTACT_GILBARCO :
					changeState( RKLNativeStates.CONTACT_GILBARCO.name( ), RKLNativeStates.GET_KMS_HOST_CERTIFICATE, "change state",
						messageParameters );
					break;
				case GET_KMS_HOST_CERTIFICATE :
					changeState( RKLNativeStates.GET_KMS_HOST_CERTIFICATE.name( ), RKLNativeStates.SEND_HOST_CERTIFICATE, "change state",
						messageParameters );
					break;
				case SEND_HOST_CERTIFICATE :
					changeState( RKLNativeStates.SEND_HOST_CERTIFICATE.name( ), RKLNativeStates.GET_SPOT_NONCE_DATA, "change state",
						messageParameters );
					break;
				case GET_SPOT_NONCE_DATA :
					changeState( RKLNativeStates.GET_SPOT_NONCE_DATA.name( ), RKLNativeStates.REQUEST_KEY_MANAGEMENT, "change state",
						messageParameters );
					break;
				case REQUEST_KEY_MANAGEMENT :
					changeState( RKLNativeStates.REQUEST_KEY_MANAGEMENT.name( ), RKLNativeStates.SEND_KEY_MANAGEMENT_DATA, "change state",
						messageParameters );
					break;
				case SEND_KEY_MANAGEMENT_DATA :
					changeState( RKLNativeStates.SEND_KEY_MANAGEMENT_DATA.name( ), RKLNativeStates.GET_KEYS, "change state", messageParameters );
					break;
				case GET_KEYS :
					changeState( RKLNativeStates.GET_KEYS.name( ), RKLNativeStates.SEND_KEY_RESPONSE, "change state", messageParameters );
					break;
				case SEND_KEY_RESPONSE :
					if (moreKeysToLoad( ))
						changeState( RKLNativeStates.SEND_KEY_RESPONSE.name( ), RKLNativeStates.GET_KEYS, "change state", messageParameters );
					else
						changeState( RKLNativeStates.SEND_KEY_RESPONSE.name( ), RKLNativeStates.SEND_CLOSE_KEY_SESSION, "change state",
							messageParameters );
					break;
				case SEND_CLOSE_KEY_SESSION :
					changeState( RKLNativeStates.SEND_CLOSE_KEY_SESSION.name( ), RKLNativeStates.SEND_CLOSE_KEY_SESSION_ANSWER, "change state",
						messageParameters );
					break;
				case SEND_CLOSE_KEY_SESSION_ANSWER :
					changeState( RKLNativeStates.SEND_CLOSE_KEY_SESSION_ANSWER.name( ), RKLNativeStates.FINAL_STATE, "change state",
						messageParameters );
					break;
				case NOTIFY_ERROR :
					changeState( RKLNativeStates.NOTIFY_ERROR.name( ), RKLNativeStates.FINAL_STATE, "change state", messageParameters );
					break;
				case FINAL_STATE :
					// nothing to do
					break;
				default :
					messageParameters.put( "Error", "INVALID RKL STEP" );
					changeState( RKLNativeStates.NOTIFY_ERROR.name( ), RKLNativeStates.NOTIFY_ERROR, "Notify Error", messageParameters );
					break;
			}
		}

		private boolean moreKeysToLoad()
		{
			// TODO Auto-generated method stub
			return false;
		}
	}
	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
