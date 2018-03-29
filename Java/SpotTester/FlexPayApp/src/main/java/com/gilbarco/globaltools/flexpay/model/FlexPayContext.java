
package com.gilbarco.globaltools.flexpay.model;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.locale.AppLocaleText;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.EdtServiceManager;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketAdapter;
import com.gilbarco.globaltools.edt.services.logging.EdtLoggingAdapter;
import com.gilbarco.globaltools.edt.services.process.EdtProcessAdapter;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerAdapter;
import com.gilbarco.globaltools.flexpay.rkl.gui.FlexPayRklController;
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
 * @author slipchak
 * 
 * */

public final class FlexPayContext
{
	
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( FlexPayContext.class );

	public static EdtServiceManager			SERVICE_MANAGER;
	public static EdtProcessAdapter			PROCESS_ADAPTER;
	public static EdtTimerAdapter			TIMER_ADAPTER;
	public static EdtClientSocketAdapter	SOCKET_ADAPTER;
	private static AppLocaleText			APP_LOCALE;

	static
	{
		SERVICE_MANAGER = EdtServiceContext.getServiceManager( );
		try
		{
			PROCESS_ADAPTER = (EdtProcessAdapter) SERVICE_MANAGER
							.getServiceAdapter( FlexPayServiceDefinitionsFactory.getFlexPayProcessDefinition( ) );
			TIMER_ADAPTER = (EdtTimerAdapter) SERVICE_MANAGER
							.getServiceAdapter( SpotServiceDefinitionsFactory.getTimerDefinition( ) );
			SOCKET_ADAPTER = (EdtClientSocketAdapter) SERVICE_MANAGER
							.getServiceAdapter( SpotServiceDefinitionsFactory.getSocketDefinition( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage() );
		}
	}

	public static void setAppLocale(AppLocaleText locale)
	{
		APP_LOCALE = locale;
	}

	public static String getLocaleText(String key)
	{
		if (APP_LOCALE != null)
		{
			return APP_LOCALE.getLocaleText( key );
		}

		logger.info( "No i18n locale text found for key: " + key );
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
