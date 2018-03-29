
package com.gilbarco.globaltools.spottester.managers;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.edt.services.logging.EdtLoggingEvent;
import com.gilbarco.globaltools.edt.services.logging.EdtLoggingEvent.LogType;
import com.gilbarco.globaltools.mgp.api.messageformatter.MgpMessageFormatterType;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.gui.SpotTesterView;
import com.gilbarco.globaltools.spottester.model.MessageFilter;

/**
 * @Class SpotTesterLoggingController
 * 
 * @Description The purpose of this class is to centralize the GUI logging feature. It work entails log every socket event into the Loggui. In the near future every log line will be handler by this
 *              entity.
 * 
 * 
 * @Services <ul>
 *           <li>EdtLoggingService
 *           </ul>
 * 
 * @see SpotTesterView
 * 
 * @version 1.0.0
 * 
 * @author jrubstein
 * 
 * */

public class SpotTesterLoggingManager
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( SpotTesterLoggingManager.class );
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
	private SpotTesterView				view;
	private MgpMessageFormatterType		formatter;
	private boolean						logDecodedMessages;
	private boolean						logRawMessages;
	private MessageFilter<String>		filter;

	private boolean						logSocketMessages;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SpotTesterLoggingManager(SpotTesterView view)
	{
		super( );
		this.view = view;
		this.logDecodedMessages = false;
		this.logRawMessages = false;
		this.logSocketMessages = false;
		setFormatter( MgpMessageFormatterType.MULTY_LINE_TABBED_MESSAGE );
		suscribeToEvents( );
		configFilter( );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	public MgpMessageFormatterType getFormatter()
	{
		return formatter;
	}

	public void setFormatter(MgpMessageFormatterType formatter)
	{
		this.formatter = formatter;
	}

	public boolean getLogDecodedMessages()
	{
		return logDecodedMessages;
	}

	public void setLogDecodedMessages(boolean logDecodedMessages)
	{
		this.logDecodedMessages = logDecodedMessages;
	}

	public boolean getLogRawMessages()
	{
		return logRawMessages;
	}

	public void setLogRawMessages(boolean logRawMessages)
	{
		this.logRawMessages = logRawMessages;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void configFilter()
	{
		filter = new MessageFilter<String>( );
		filter.add( SpotMessageType.PING.getName( ) );
	}

	private void suscribeToEvents()
	{
		try
		{
			EdtServiceContext.getServiceManager( ).subscribe( new EdtEventAction<MgpResultEvent>( )
			{
				@Override
				public void onInvokeEvent(MgpResultEvent event)
				{
				}

				@Override
				public Class<MgpResultEvent> getEventClass()
				{
					return MgpResultEvent.class;
				}

				@Override
				public String getSubscriptionFilter()
				{
					// do not log ping messages
					return MgpResultEvent.MESSAGE_NAME + " <> '" + SpotMessageType.PING.getName( ) + "' AND " + EdtEvent.EVENT_TYPE_FILTER + " <> '"
							+ EdtMgpType.RESPONSE_RESULT.name( ) + "'";
				}

				@Override
				public boolean justOnce()
				{
					return false;
				}
			} );

			EdtServiceContext.getServiceManager( ).subscribe( new EdtEventAction<EdtLoggingEvent>( )
			{

				@Override
				public void onInvokeEvent(EdtLoggingEvent event)
				{
					if (((LogType) event.getEventType( )) != LogType.DEBUG)
					{
						view.getLogGui( ).appendWithCrLf(
							"[" + event.getFormatedTimeStamp( event.getCreateTimeStamp( ), "YYY-MM-dd HH:mm:ss" ) + "]["
											+ event.getEventType( ).name( ) + "] - " + event.getLogText( ) );
					}
				}

				@Override
				public Class<EdtLoggingEvent> getEventClass()
				{
					return EdtLoggingEvent.class;
				}

				@Override
				public String getSubscriptionFilter()
				{
					// do not log ping messages
					return null;
				}

				@Override
				public boolean justOnce()
				{
					return false;
				}
			} );
			if (logSocketMessages)
			{

				EdtServiceContext.getServiceManager( ).subscribe( new EdtEventAction<EdtClientSocketEvent>( )
				{

					@Override
					public Class<EdtClientSocketEvent> getEventClass()
					{
						return EdtClientSocketEvent.class;
					}

					@Override
					public String getSubscriptionFilter()
					{
						return null;
					}

					@Override
					public boolean justOnce()
					{
						return false;
					}

					@Override
					public void onInvokeEvent(EdtClientSocketEvent event)
					{

						String type = "";
						type = getTypeOfEvent( event, type );
						if (!type.isEmpty( ))
						{
							view.getLogGui( ).appendWithCrLf( "" );
							view.getLogGui( ).append( "Message :" );
							view.getLogGui( ).appendWithCrLf( type );
						}
					}

					/**
					 * @param event
					 * @param type
					 * @return
					 */
					private String getTypeOfEvent(EdtClientSocketEvent event, String type)
					{
						switch(event.getEnumType( SocketEventType.class ))
						{
							case INCOMING_DATA :
								type = "RX";
								break;
							case OUTGOING_DATA :
								type = "TX";
								break;
							default :
								break;
						}
						return type;
					}
				} );

			}
		}
		catch (EdtServiceException e)
		{
			logger.error( "SpotTesterLoggingManager - suscribeToEvents - "+e.getMessage( ));
		}
	}
	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
