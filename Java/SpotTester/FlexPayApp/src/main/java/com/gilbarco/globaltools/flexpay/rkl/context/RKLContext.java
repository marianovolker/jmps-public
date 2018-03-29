
package com.gilbarco.globaltools.flexpay.rkl.context;

import java.util.Arrays;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.process.EdtProcessAdapter;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent;
import com.gilbarco.globaltools.flexpay.rkl.gui.FlexPayRklController;
import com.gilbarco.globaltools.flexpay.rkl.gui.FlexPayRklModel;
import com.gilbarco.globaltools.flexpay.rkl.state.RKLNativeStates;
import com.gilbarco.globaltools.flexpay.rkl.state.RKLStateEventHandler;
import com.gilbarco.globaltools.flexpay.rkl.state.ServerRKLBroker;
import com.gilbarco.globaltools.flexpay.rkl.state.SpotRKLBroker;

public class RKLContext
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static EdtProcessAdapter	processAdapter;
	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	static
	{
		try
		{
			processAdapter = (EdtProcessAdapter) EdtServiceContext.getServiceManager( ).getServiceAdapter(
				RKLServiceDefinitionFactory.getProcessDefinition( ) );
		}
		catch (EdtServiceException e)
		{
		}
	}
	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private static ApplicationContext	context	= null;
	private static RKLContext			instance;
	private SpotRKLBroker				spotBroker;
	private ServerRKLBroker				serverBroker;
	private FlexPayRklController		controller;

	// private RKLStateManager stateManager;
	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public RKLContext(FlexPayRklModel rklModel)
	{
		super( );
		context = new ClassPathXmlApplicationContext( "RKLContext.xml" );
	}

	public static RKLContext getInstance(FlexPayRklModel rklModel)
	{
		if (instance == null)
		{
			instance = new RKLContext(rklModel);
		}
		return instance;
	}

	public static ApplicationContext getContext()
	{
		return context;
	}

	public static EdtProcessAdapter getProcessAdapter()
	{
		return processAdapter;
	}

	public static void setProcessAdapter(EdtProcessAdapter processAdapter)
	{
		RKLContext.processAdapter = processAdapter;
	}

	public void init(String[] args, FlexPayRklController controller) throws EdtServiceException
	{
		System.out.println( "args: " + Arrays.toString( args ) );
		this.controller = controller;
		suscribetoEvents( );
		if( this.configureBrokers( args ))
		{
			// if everything goes ok, let's start the process
		
		((RKLStateEventHandler) RKLContext.context.getBean( "RKLStateEventHandler" )).startRklProcess( );
		
			
		}
		else
		{
			synchronized (context)
			{
				context.notifyAll( );
			}
			
			controller.showRKLError( );
		}
	}

	private boolean configureBrokers(String[] args)
	{
		boolean ret1= false;
		boolean ret2= false;
		
		ret1 = this.configureRKLSpotBroker( args[0], args[1] );
		if (args.length == 4){
			
			ret2 = this.configureRKLServerBroker( args[2], args[3] );
		}
		if( !ret1 || !ret2 )
			return false;
		
		
		return true;
		
	}

	private boolean configureRKLSpotBroker(String ip, String port)
	{
		spotBroker = (SpotRKLBroker) RKLContext.context.getBean( "RKLSpotBroker" );
		spotBroker.setProperties( ip, port, controller );
		if(!spotBroker.registerService( ))
		{
			
			return false;
		}
		spotBroker.initialize( );

		return true;
			
		
	}

	private boolean configureRKLServerBroker(String ip, String port)
	{
		serverBroker = (ServerRKLBroker) RKLContext.context.getBean( "RKLServerBroker" );
		serverBroker.setProperties( ip, port, controller );
		if(!serverBroker.registerService( ))
		{
			return false;
		}

		serverBroker.initialize( );		
		return true;
		
	}

	protected void changeState(String to, String description, Issuer issuer ) throws EdtServiceException
	{
			if (to != null)
			{
				RKLContext.getProcessAdapter( ).callChangeToState( RKLContext.getProcessAdapter( ).getCurrentState( ), to, issuer, description );
			}
		
	}
	
	private void suscribetoEvents() throws EdtServiceException
	{
		EdtServiceContext.getServiceManager( ).subscribe( new EdtEventAction<EdtProcessEvent>( )
		{

			@Override
			public void onInvokeEvent(EdtProcessEvent event)
			{
				if (event.getDestination( ).equals( RKLNativeStates.FINAL_STATE.name( ) )
					|| event.getDestination( ).equals( RKLNativeStates.NOTIFY_ERROR.name( ) ))
				{
					try
					{
						serverBroker.close( "Closing serverBroker" );
						spotBroker.close( "Closing spotBroker" );
					}
					finally
					{
						synchronized (context)
						{
							context.notifyAll( );
						}
					}
					controller.showRKLError( );
				}

			}

			@Override
			public Class<EdtProcessEvent> getEventClass()
			{
				return EdtProcessEvent.class;
			}

			@Override
			public String getSubscriptionFilter()
			{
				return "";
			}

			@Override
			public boolean justOnce()
			{
				return false;
			}
		} );

	}

}
