/**
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file ComparisonEngine.java
 * @author mgrieco
 * @date 19/09/2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

package com.gilbarco.globaltools.flexpay.engines;

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.util.Calendar;
import java.util.Collection;
import java.util.List;
import java.util.Observable;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.mgp.domain.ArrayContainer;
import com.gilbarco.globaltools.mgp.domain.ChildElement;
import com.gilbarco.globaltools.mgp.domain.Container;
import com.gilbarco.globaltools.mgp.domain.ContainerType;
import com.gilbarco.globaltools.mgp.domain.Field;
import com.gilbarco.globaltools.mgp.domain.Message;
import com.gilbarco.globaltools.mgp.domain.Protocol;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.exceptions.MessageDataException;
import com.gilbarco.globaltools.mgp.exceptions.PredicateException;
import com.gilbarco.globaltools.spotprotocol.SpotProtocol;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;

/**
 * The Class ComparisonEngine.
 */
public class ComparisonEngine extends Observable
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	/** The Constant logger. */
	private static final transient Log		logger					= LogFactory.getLog( ComparisonEngine.class );

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	/** The app versions from local files to be compared */
	private Vector<ApplicationVersion>		appVersionsFromFiles	= null;

	/** The app versions from unit to be compared. */
	private Vector<ApplicationVersion>		appVersionsFromSpot		= null;

	/** The received response boolean value. */
	private boolean							receivedResponse				= false;

	/** The error in response boolean value. */
	private boolean							errorInResponse					= false;

	/** The comparison comparisonStatus. */
	private SoftwareComparisonStatus		comparisonStatus				= SoftwareComparisonStatus.NOTHING_TO_COMPARE;

	/** The compare strategy (about unit model). */
	private ICompareStrategy				compareStrategy;

	/** The socket disconnect event processor. */
	private EdtClientSocketDisconnectAction	socketDisconnectEventProcessor;


	/** Default target unit connection timeout measured in milliseconds */
	public static final long				DEFAULT_TIMEOUT_MILLISECONDS	= 60000L;

	// ******************************************************************
	// PUBLIC ENUM.
	// ******************************************************************
	/**
	 * The Software Comparison Status values enumeration.
	 */
	public enum SoftwareComparisonStatus
	{
		/** The nothing to compare. */
		NOTHING_TO_COMPARE,

		/** The versions do not match. */
		VERSIONS_DO_NOT_MATCH,

		/** The versions match. */
		VERSIONS_MATCH
	}

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	/**
	 * Instantiates a new comparison engine.
	 * 
	 * @throws EdtServiceException the edt service exception
	 */
	public ComparisonEngine() throws EdtServiceException
	{
		super( );
		subscribeToEvents( );
		appVersionsFromFiles = new Vector<ApplicationVersion>( );
		appVersionsFromSpot = new Vector<ApplicationVersion>( );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	/**
	 * Sets the strategy about the unit model. It should be could before process comparison.
	 * 
	 * @param strategy the comparison strategy about the unit model (m3/m5/vgd/etc).
	 */
	public void setStrategy(ICompareStrategy strategy)
	{
		compareStrategy = strategy;
	}

	/**
	 * Gets the comparisonStatus.
	 * 
	 * @return the comparisonStatus
	 */
	public SoftwareComparisonStatus getStatus()
	{
		return comparisonStatus;
	}

	/**
	 * Adds the local application to be verified.
	 * 
	 * @param appName the app name
	 * @param appVersion the app version
	 */
	public void addLocalAppToVerify(String appName, String appVersion)
	{
		ApplicationVersion application = new ApplicationVersion( SoftwarePackageTypes.fromString( appName ), appVersion );
		appVersionsFromFiles.add( application );
	}
	
	/**
	 * Adds the unit application to be verified.
	 * 
	 * @param appName the app name
	 * @param appVersion the app version
	 */
	private void addUnitAppToVerify(String appName, String appVersion)
	{
		ApplicationVersion application = new ApplicationVersion( SoftwarePackageTypes.fromString( appName ), appVersion );
		appVersionsFromSpot.add( application );
	}


	/**
	 * Clear vector apps to verify.
	 */
	public void clearVectorAppsToVerify()
	{
		appVersionsFromFiles.clear( );
	}

	/**
	 * Do comparison.
	 * Gets the spot software information and compares that with local software.
	 *
	 * @return the spot software module comparison status
	 * @throws InterruptedException the interrupted exception
	 */
	public SoftwareComparisonStatus doComparision() throws InterruptedException
	{
		errorInResponse = false;
		receivedResponse = false;
		comparisonStatus = SoftwareComparisonStatus.NOTHING_TO_COMPARE;
		appVersionsFromSpot.clear( );

		sendPackageInfoRequest( ); //request the unit software information by spot protocol.
		
		synchronized (this) // wait for the package info response.
		{
			long t0 = Calendar.getInstance( ).getTimeInMillis( );
			long t1;
			while (!receivedResponse)
			{
				wait( DEFAULT_TIMEOUT_MILLISECONDS );
				t1 = Calendar.getInstance( ).getTimeInMillis( );
				if ( t1 - t0 >= DEFAULT_TIMEOUT_MILLISECONDS )
				{
					break;
				}
			}

			if (!receivedResponse)
			{
				errorInResponse = true;
				return SoftwareComparisonStatus.NOTHING_TO_COMPARE; //error nothing could be compared.
			}

			if (!errorInResponse)
			{
				compareFiles( );
			}
		}
		return getStatus( );
	}

	/**
	 * Compare files from the local folder with the received from the unit.
	 */
	private void compareFiles()
	{
		logger.info( "Comparison Format : <Package Name> = <Local Package Version> - <SPOT Package SPOT version/NOT PRESENT>" );

		comparisonStatus = SoftwareComparisonStatus.VERSIONS_MATCH;
		for (int i = 0; i < appVersionsFromSpot.size( ); i++)
		{
			boolean bIspresent = false;
			ApplicationVersion moduleFromSpot = appVersionsFromSpot.get( i );
			if (SoftwarePackageTypes.isVanguardComparable( moduleFromSpot.getAppId( ) ))
			{
				for (int j = 0; j < appVersionsFromFiles.size( ); j++)
				{
					ApplicationVersion moduleFromFile = appVersionsFromFiles.get( j );
					if (moduleFromSpot.getAppId( ) == moduleFromFile.getAppId( ))
					{
						if (moduleFromSpot.getVersion( ).equals( moduleFromFile.getVersion( ) ))
						{
							logger.info( moduleFromSpot.getAppId( ).getSwrName( ) + " = " + moduleFromSpot.getVersion( ) );
						}
						else
						{
							logger.info( moduleFromSpot.getAppId( ).getSwrName( ) + " = " + moduleFromFile.getVersion( ) + " - " + moduleFromSpot.getVersion( ) );
							comparisonStatus = SoftwareComparisonStatus.VERSIONS_DO_NOT_MATCH;
						}
						bIspresent = true;
						break;
					}
				}

				// files found only in the unit.
				if (!bIspresent)
				{
					logger.info( moduleFromSpot.getAppId( ).getSwrName( ) + " = NOT PRESENT - " + moduleFromSpot.getVersion( ) );
					comparisonStatus = SoftwareComparisonStatus.VERSIONS_DO_NOT_MATCH;
				}
			}

		}

		// files found only in the local folder.
		for (int ind = 0; ind < appVersionsFromFiles.size( ); ind++)
		{
			boolean bIspresent = false;
			ApplicationVersion moduleFromFile = appVersionsFromFiles.get( ind );
			if (SoftwarePackageTypes.isVanguardComparable( moduleFromFile.getAppId( ) ))
			{
				for (int jnd = 0; jnd < appVersionsFromSpot.size( ); jnd++)
				{
					ApplicationVersion moduleFromSpot = appVersionsFromSpot.get( jnd );
					if (moduleFromFile.getAppId( ) == moduleFromSpot.getAppId( ))
					{
						bIspresent = true;
						break;
					}
				}
				if (!bIspresent)
				{
					logger.info( moduleFromFile.getAppId( ).getSwrName( ) + " = " + moduleFromFile.getVersion( ) + " - NOT PRESENT" );
					comparisonStatus = SoftwareComparisonStatus.VERSIONS_DO_NOT_MATCH;
				}
			}
		}
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	/**
	 * Wakeup.
	 */
	protected synchronized void wakeup()
	{
		this.notify( );
	}

	/**
	 * Send package info.
	 * Requests the unit software information by spot protocol.
	 */
	protected void sendPackageInfoRequest()
	{
		logger.info( "requesting packages info..." );
		try
		{
			ConnectionEngineSwingAdapter.getInstance().send(compareStrategy.getPackageInfoRequest());
		}
		catch (NullPointerException e)
		{
			logger.error("Error. Exception thrown while sending synchronouse data to target unit.\nEdtServiceException: '" + e.getMessage() + "'.");
		}
	}

	/**
	 * Subscribe to mgp results events.
	 * @throws EdtServiceException 
	 */
	protected void subscribeToEvents() throws EdtServiceException
	{
		SERVICE_MANAGER.subscribe( new MgpResultAction( ) );
		
		if (socketDisconnectEventProcessor == null)
		{
			socketDisconnectEventProcessor = new EdtClientSocketDisconnectAction();
			SERVICE_MANAGER.subscribe( socketDisconnectEventProcessor );
		}
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	/**
	 * Process software information answer.
	 * 
	 * @param event the mgp result event about the software request information answer
	 */
	private void processSoftwareInformationAnswer(MgpResultEvent event)
	{
		if (event.isValid( ))
		{
			receivedResponse = true;
			errorInResponse = true;
			try
			{
				Protocol spotProtocol = new SpotProtocol( ).buildSpotProtocol( );
				List<Message> messages = spotProtocol.findMessage( compareStrategy.getPackageInfoName( ), ContainerType.RESPONSE );
				Message msgCopy = messages.get( 0 ).getWorkingCopy( );
				msgCopy.decode( event.getRawData( ) );
				for (ChildElement child : msgCopy.getChildren( ))
				{
					if (Container.class.isInstance( child ))
					{
						ArrayContainer<Container> itemArray = (ArrayContainer<Container>) child;
						Collection<ChildElement> col = itemArray.getChildren( );
						for (ChildElement childElement : col)
						{
							if (childElement instanceof Container)
							{
								Collection<Field> colItem = ((Container) childElement).findChildElement( "Certificate" );
								Field fldCertificate = (Field) colItem.toArray( )[0];
								if (fldCertificate != null)
								{
									ROMfsCertificate certificate = compareStrategy.newPackageCertificate( fldCertificate.getData( ) );
									if (certificate.loadCertificate( true ))
									{
										addUnitAppToVerify( new String( certificate.getAppName( ) ), new String( certificate.getAppVersion( ) ) );
									}
								}
							}
						}
					}
				}
				errorInResponse = false;
			}
			catch (PredicateException | MessageDataException e)
			{
			}
		}
		else
		{
			errorInResponse = true;
		}
		wakeup( );
	}


	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	private final class MgpResultAction implements EdtEventAction<MgpResultEvent>
	{
		@Override
		public void onInvokeEvent(MgpResultEvent event)
		{
			if (event.getMessageName( ).equals( compareStrategy.getPackageInfoName( ) ))
			{
				processSoftwareInformationAnswer( event );
			}
		}

		@Override
		public Class<MgpResultEvent> getEventClass()
		{
			return MgpResultEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name( ) +
					"' AND " + MgpResultEvent.MESSAGE_NAME + " IN ('"
					+ SpotMessageType.M3M5_ROMfs_PACKAGE_INFO.getName( ) + "', '"
					+ SpotMessageType.VANGUARD_ROMfs_PACKAGE_INFO.getName( ) + "')";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}
	}
	
	/**
	 * The Class EdtClientSocketDisconnectAction process the socket disconnect event.
	 */
	private final class EdtClientSocketDisconnectAction implements EdtEventAction<EdtClientSocketEvent>
	{
		@Override
		public void onInvokeEvent(EdtClientSocketEvent event)
		{
			SocketEventType eventtype = (SocketEventType) event.getEventType( );
			
			if(eventtype.equals(SocketEventType.SOCKET_DISCONNECT))
			{
				logger.debug("SOCKET_DISCONNECT");
				receivedResponse = true;
				errorInResponse = true;
				wakeup( );
			}
		}

		@Override
		public Class<EdtClientSocketEvent> getEventClass()
		{
			return EdtClientSocketEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.SOCKET_DISCONNECT.name( ) + "'";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}
	}

	/**
	 * The Class ApplicationVersion.
	 */
	public class ApplicationVersion
	{

		/** The app id. */
		private SoftwarePackageTypes	appId;

		/** The version. */
		private String				version;

		/**
		 * Instantiates a new application version.
		 * 
		 * @param appId
		 *            the app id
		 * @param version
		 *            the version
		 */
		public ApplicationVersion(SoftwarePackageTypes appId, String version)
		{
			this.appId = appId;
			this.version = version;
		}

		/**
		 * Gets the app id.
		 * 
		 * @return the app id
		 */
		public SoftwarePackageTypes getAppId()
		{
			return appId;
		}

		/**
		 * Gets the version.
		 * 
		 * @return the version
		 */
		public String getVersion()
		{
			return version;
		}
	}
}
