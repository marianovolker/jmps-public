package com.gilbarco.globaltools.gcm.state;

import static com.gilbarco.globaltools.gcm.model.GCMContext.PROCESS_ADAPTER;
import static com.gilbarco.globaltools.gcm.model.GCMContext.SERVICE_MANAGER;
import static com.gilbarco.globaltools.gcm.model.GCMContext.TIMER_ADAPTER;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent.EdtTimerMode;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.gcm.model.GCMContext;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;

public class GCMStateManager {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log		logger			= LogFactory.getLog(GCMStateManager.class);
	private static final String				STATE_TIMER		= "STATE_TIMEOUT";
	private static final int				WAIT_DATA_TIMEOUT = 1000 * 60 * 1;

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
	public GCMStateManager() throws EdtServiceException {
		super();
		this.issuer = new Issuer(this.getClass());
		this.timeoutAction = new StateTimeOut();
		this.started = false;
		subscribeToEvents();
		this.changeState(null, GCMState.IDLE.name(), GCMContext.getLocaleText("Ready"));
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public void startTest() {
		this.started = true;
		this.changeState(null, GCMState.INIT.name(), GCMContext.getLocaleText("Initializing"));
	}

	public void stopTest() {
		this.changeState(null, GCMState.LOG_RESULT.name(), GCMContext.getLocaleText("TestStoped"));
		this.started = false;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	protected void subscribeToEvents() throws EdtServiceException {
		SERVICE_MANAGER.subscribe(new ProcessMover());
	}

	protected void startStateTimer(long timeout) {
		TIMER_ADAPTER.callElapsedTimeAction(STATE_TIMER, timeout, EdtTimerMode.ONE_SHOT, this.timeoutAction, this.issuer);
	}

	public void stopStateTimer() {
		TIMER_ADAPTER.callStopTimerAction(STATE_TIMER);
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void changeState(String from, String to, String description) {
		try {
			if (EdtStringUtil.isNullOrEmpty(from) || PROCESS_ADAPTER.getCurrentState().equals(from)) {
				PROCESS_ADAPTER.callChangeToState(PROCESS_ADAPTER.getCurrentState(), to, this.issuer, description);
				this.stopStateTimer();
				// the INIT option is because when the reader is not working the enable reader message is not sent by the spot. 
				if (to.equals(GCMState.WAIT_DATA.name()) || to.equals(GCMState.INIT.name()))
					this.startStateTimer(WAIT_DATA_TIMEOUT);
			}
			else {
				String error = "WRONG process event, can't move from state " + from + " to " + to + " actual state is: "
								+ PROCESS_ADAPTER.getCurrentState();
				logger.error("error: " + error);
			}
		}
		catch (EdtServiceException e) {
			logger.error(e.getMessage(), e);
		}
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class ProcessMover implements EdtEventAction<MgpResultEvent> {

		@Override
		public Class<MgpResultEvent> getEventClass() {
			return MgpResultEvent.class;
		}

		@Override
		public String getSubscriptionFilter() {
			return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name() + "' AND "
					+ MgpResultEvent.MESSAGE_NAME + " IN ('" + SpotMessageType.ENABLE_CL_READER.getName() + "', '"
					+ SpotMessageType.CL_DATA.getName() + "', '" + "')";
		}

		@Override
		public boolean justOnce() {
			return false;
		}

		@Override
		public void onInvokeEvent(MgpResultEvent event) {
			if (!started)
				return;

			if (!event.isValid()) {
				changeState(null, GCMState.LOG_RESULT.name(), event.getMessageName() + " command Fail");
				logger.debug(event);
				return;
			}

			SpotMessageType msg = SpotMessageType.getByName(event.getMessageName());
			switch(msg){
				case ENABLE_CL_READER :
					changeState(GCMState.ENABLE_READER.name(), GCMState.WAIT_DATA.name(), GCMContext
									.getLocaleText("SwipeCard"));
					break;
				case CL_DATA :
					changeState(GCMState.WAIT_DATA.name(), GCMState.LOG_RESULT.name(), GCMContext.getLocaleText("TestEnd"));
					break;
			}
		}
	}

	public class StateTimeOut implements EdtEventAction<EdtTimerEvent> {

		@Override
		public Class<EdtTimerEvent> getEventClass() {
			return EdtTimerEvent.class;
		}

		@Override
		public String getSubscriptionFilter() {
			return EdtEvent.ISSUER_FILTER + "='" + issuer + "'";
		}

		@Override
		public boolean justOnce() {
			return true;
		}

		@Override
		public void onInvokeEvent(EdtTimerEvent event) {
			if (!started)
				return;
			
			changeState(null, GCMState.LOG_RESULT.name(), "Timed out...");
		}
	}
	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
