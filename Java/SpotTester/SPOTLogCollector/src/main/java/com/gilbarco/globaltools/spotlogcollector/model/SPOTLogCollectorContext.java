
package com.gilbarco.globaltools.spotlogcollector.model;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.locale.AppLocaleText;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.EdtServiceManager;
import com.gilbarco.globaltools.edt.services.logging.EdtLoggingAdapter;
import com.gilbarco.globaltools.edt.services.process.EdtProcessAdapter;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerAdapter;
import com.gilbarco.globaltools.spottester.model.SpotServiceDefinitionsFactory;

/**
 * @Class SPOTLogCollectorController
 * 
 * @version 1.0.0
 * 
 * @author mgrieco
 * 
 * */

public final class SPOTLogCollectorContext
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log			logger				= LogFactory.getLog( SPOTLogCollectorContext.class );

	public static EdtServiceManager	SERVICE_MANAGER;
	public static EdtProcessAdapter	PROCESS_ADAPTER;
	public static EdtTimerAdapter	TIMER_ADAPTER;
	public static EdtLoggingAdapter	LOGGING_ADAPTER;
	private static AppLocaleText	APP_LOCALE;

	static
	{
		try
		{
			SERVICE_MANAGER = EdtServiceContext.getServiceManager( );
			PROCESS_ADAPTER = (EdtProcessAdapter) SERVICE_MANAGER.getServiceAdapter( SPOTLogCollectorServiceDefinitionsFactory.getSpotLogCollectorProcessDefinition( ) );
			TIMER_ADAPTER = (EdtTimerAdapter) SERVICE_MANAGER.getServiceAdapter( SpotServiceDefinitionsFactory.getTimerDefinition( ) );
			LOGGING_ADAPTER = (EdtLoggingAdapter) SERVICE_MANAGER.getServiceAdapter( SpotServiceDefinitionsFactory.getLoggingDefinition( ) );
		}
		catch (EdtServiceException e)
		{

		}
	}

	public static void setAppLocale(AppLocaleText locale)
	{
		APP_LOCALE = locale;
	}

	public static String getLocaleText(String key)
	{
		if (APP_LOCALE != null) return APP_LOCALE.getLocaleText( key );

		logger.error( "No i18n locale text found for key: " + key );
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
