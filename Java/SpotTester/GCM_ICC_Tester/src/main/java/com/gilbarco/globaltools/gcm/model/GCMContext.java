package com.gilbarco.globaltools.gcm.model;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.locale.AppLocaleText;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.EdtServiceManager;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketAdapter;
import com.gilbarco.globaltools.edt.services.logging.EdtLoggingAdapter;
import com.gilbarco.globaltools.edt.services.process.EdtProcessAdapter;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerAdapter;
import com.gilbarco.globaltools.spottester.model.SpotServiceDefinitionsFactory;
import com.gilbarco.globaltools.spottester.model.SpotState;

/**
 * @Class SpotTesterStateController
 * 
 * @Description This entity is in charge of the state machine mechanism. The states are defined in {@linkplain SpotState}.
 *              <p>
 *              {@link SpotTesterPingTimer} is started by this class when a success login answer arrive and is stopped when a logout request is sent.
 * 
 * @Services <ul>
 *           <li>EdtProcessService
 *           </ul>
 * 
 * @see SpotState
 * @see SpotTesterPingTimer
 * 
 * @version 1.0.0
 * 
 * @author jrubstein
 * 
 * */

public final class GCMContext {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	public static EdtServiceManager			SERVICE_MANAGER;
	public static EdtProcessAdapter			PROCESS_ADAPTER;
	public static EdtTimerAdapter			TIMER_ADAPTER;
	public static EdtLoggingAdapter			LOGGING_ADAPTER;
	public static EdtClientSocketAdapter	SOCKET_ADAPTER;
	private static AppLocaleText			APP_LOCALE;

	static {
		SERVICE_MANAGER = EdtServiceContext.getServiceManager();
		try {
			PROCESS_ADAPTER = (EdtProcessAdapter) SERVICE_MANAGER.getServiceAdapter(GCMServiceDefinitionsFactory
							.getGcmProcessDefinition());
			TIMER_ADAPTER = (EdtTimerAdapter) SERVICE_MANAGER.getServiceAdapter(SpotServiceDefinitionsFactory
							.getTimerDefinition());
			LOGGING_ADAPTER = (EdtLoggingAdapter) SERVICE_MANAGER.getServiceAdapter(SpotServiceDefinitionsFactory
							.getLoggingDefinition());
			SOCKET_ADAPTER = (EdtClientSocketAdapter) SERVICE_MANAGER.getServiceAdapter(SpotServiceDefinitionsFactory
							.getSocketDefinition());
		}
		catch (EdtServiceException e) {

		}
	}

	public static void setAppLocale(AppLocaleText locale) {
		APP_LOCALE = locale;
	}

	public static String getLocaleText(String key) {
		if (APP_LOCALE != null)
			return APP_LOCALE.getLocaleText(key);

		try {
			LOGGING_ADAPTER.invokeWarnLog("No i18n locale text found for key: " + key);
		}
		catch (EdtServiceException e) {

		}
		return key;
	}

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

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
