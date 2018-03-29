
package com.gilbarco.globaltools.spotlogcollector.state;

import static com.gilbarco.globaltools.spotlogcollector.model.SPOTLogCollectorContext.PROCESS_ADAPTER;
import static com.gilbarco.globaltools.spotlogcollector.model.SPOTLogCollectorContext.SERVICE_MANAGER;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.DirectoryNotEmptyException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent.ProcessEventType;
import com.gilbarco.globaltools.edt.util.EdtUtil;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageErrorCode;
import com.gilbarco.globaltools.spottester.managers.SpotTesterDataManager;
import com.gilbarco.globaltools.spottester.model.SpotMessages;
import com.gilbarco.globaltools.spottester.model.SpotState;
import com.gilbarco.globaltools.spotlogcollector.main.gui.SPOTLogCollectorModel;
import com.gilbarco.globaltools.spotlogcollector.main.gui.SPOTLogCollectorProgressObserver;

public class SPOTLogCollectorMessageBroker
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log			logger					= LogFactory.getLog( SPOTLogCollectorMessageBroker.class );


	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private SPOTLogCollectorProgressObserver    progressObserver;
	private SPOTLogCollectorModel				model;
	private Vector<SPOTLogCollectorLogFiles>	filesToUploadVector;
	private Iterator<SPOTLogCollectorLogFiles>	filesIterator;
	private SPOTLogCollectorLogFiles			currentFile;
	
	private Path currentSessionDirectory;
	private OutputStream currentSessionFileOutputStream;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SPOTLogCollectorMessageBroker(SPOTLogCollectorModel model, SPOTLogCollectorProgressObserver progressObserver) throws EdtServiceException
	{
		super( );
		this.model = model;
		this.progressObserver = progressObserver;
		this.subscribeToEvents( );
	}

	private void createFilesToUploadVector()
	{
		// add each file and its history file option.
		if (filesToUploadVector != null)
		{
			filesToUploadVector.clear();
		}
		else
		{
			filesToUploadVector = new Vector<SPOTLogCollectorLogFiles>( );
		}

		filesToUploadVector.add( SPOTLogCollectorLogFiles.ABI );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.ABI_HIST );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.DISPCLIB );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.DISPCLIB_HIST );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.EMV );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.EMV_HIST );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.GVROPTEMV );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.GVROPTEMV_HIST );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.JOURNAL );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.JOURNAL_HIST );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.OPT );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.OPT_HIST );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.OPTJOURNAL );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.OPTJOURNAL_HIST );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.PINPAD );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.PINPAD_HIST );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.SETUP );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.SETUP_HIST );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.SYSMANSERV );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.SYSMANSERV_HIST );
		
		filesToUploadVector.add( SPOTLogCollectorLogFiles.AS2805 );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.AS2805_HIST );
		
		filesToUploadVector.add( SPOTLogCollectorLogFiles.CVXADAPTER );
		filesToUploadVector.add( SPOTLogCollectorLogFiles.CVXADAPTER_HIST );		
		
		filesToUploadVector.add( SPOTLogCollectorLogFiles.LINUXKERNEL );

		filesToUploadVector.add( SPOTLogCollectorLogFiles.EXTAPP_EXT0 );//only M3 returns with data
		filesToUploadVector.add( SPOTLogCollectorLogFiles.EXTAPP_EXT1 );//only M5 returns with data
		filesToUploadVector.add( SPOTLogCollectorLogFiles.EXTAPP_EXT2 );//only M5 returns with data
		filesIterator = filesToUploadVector.iterator( );
	}

	private void cleanFilesToUploadVector()
	{
		// add each file and its history file option.
		filesToUploadVector.clear();
		filesIterator = filesToUploadVector.iterator( );
	}

	private void changeState(SPOTLogCollectorState to, String description)
	{
		Map<String, Object> properties = new HashMap<String, Object>( );
		changeState(to, description, properties);
	}

	private void changeState(SPOTLogCollectorState to, String description, Map<String, Object> properties)
	{
		try
		{
			PROCESS_ADAPTER.callChangeToState( PROCESS_ADAPTER.getCurrentState( ), to.name( ), new Issuer( this.getClass( ) ), description, properties );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
	}

	private void connectAndLogin()
	{
		try
		{
			SpotTesterDataManager.getInstance( ).configureAndLogin( model.getCurrentIp( ), model.getCurrentPort( ), model.getSpotClientID() );
			if (SpotTesterDataManager.getInstance( ).isConnect( ))
			{
				logger.info( "Login Requested to: " + model.getCurrentIp( ) + ":" + model.getCurrentPort( ) );
			}
			else
			{
				logger.error( "ConnectAndLoging Error - connecting to: " + model.getCurrentIp( ) + ":" + model.getCurrentPort( ) );
				changeState( SPOTLogCollectorState.SHOW_ERROR, "ConnectAndLoging Error - connecting to: " + model.getCurrentIp( ) + ":" + model.getCurrentPort( ) );
			}
		}
		catch (EdtServiceException e)
		{
			logger.error( "ConnectAndLoging Error - " + e.getMessage( ) );
			changeState( SPOTLogCollectorState.SHOW_ERROR, "Error not connected to: " + model.getCurrentIp( ) + ":" + model.getCurrentPort( ) );
		}
	}

	private void disconnect()
	{
		logger.info( "Close Connection." );
		SpotTesterDataManager.getInstance( ).closeSocket( );
	}

	private void sendFileUploadRequest()
	{
		byte[] message = SpotMessages.getInstance( ).getFileUploadRequestData( currentFile.getFileType( ), currentFile.getFileId( ) );
		SpotTesterDataManager.getInstance( ).sendMessage( message );
	}

	private void sendFileBlockUploadRequest(Map<String, Object> data)
	{
		int offset = Integer.decode((String)data.get( "Offset" ));
		byte[] message = SpotMessages.getInstance( ).getFileBlockUploadData( offset );
		SpotTesterDataManager.getInstance( ).sendMessage( message );
	}

	private void getFileBlockUploadResponse(Map<String, Object> data)
	{
		if ((byte) data.get( "Ack" ) == SpotMessageErrorCode.DOWNLOAD_COMPLETED.getCode( )) // file completed
		{
			try
			{
				appendToCurrentFileInSession( (byte[]) data.get( "BlkData" ) );
				closeCurrentFileInSession( );
				logger.info( "File completed" );
				changeState( SPOTLogCollectorState.GET_FILE_TO_REQUEST, "UploadFileRequest" );
			}
			catch (IOException e)
			{
				logger.error( "Error in file completion... " + e.getMessage( ), e );
				logger.error( "Proceeding to next file. " );
				changeState( SPOTLogCollectorState.SHOW_ERROR, "Error in file completion" );
			}
		}
		else if ((byte) data.get( "Ack" ) == SpotMessageErrorCode.NO_ERROR.getCode( )) // more blocks to request
		{
			try
			{
				appendToCurrentFileInSession( (byte[]) data.get( "BlkData" ) );
				Map<String, Object> newData = new HashMap<String, Object>( );
				newData.put( "Offset", (String) data.get( "Offset" ) );
				changeState( SPOTLogCollectorState.SEND_FILE_BLOCK_UPLOAD_REQUEST, "RequestMoreData", newData );
			}
			catch (IOException e)
			{
				logger.error( "Error in file appending storage... " + e.getMessage( ), e );
				logger.error( "Proceeding to next file. " );
				changeState( SPOTLogCollectorState.SHOW_ERROR, "Error in file appending storage" );
			}
		}
		else
		{
			String sNAck = " ";
			try
			{
				sNAck = String.format( " 0x%02X ", (((byte) data.get( "Ack" )) & 0xFF) );
			}
			catch (Exception e)
			{
				logger.error( "Error while formatting NAck retrieved value. [" + e.getMessage( ) + "]", e );
			}
			logger.error( "Error" + sNAck + "in file block upload response message." );
			logger.error( "Proceeding to next file." );
			changeState( SPOTLogCollectorState.GET_FILE_TO_REQUEST, "UploadFileRequest" );
		}
	}

	private void getFileUploadResponse(Map<String, Object> data)
	{
		byte ack = (byte) data.get( "Ack" );
		if (ack == SpotMessageErrorCode.NO_ERROR.getCode())
		{
			if (blocksToRequest( data ))
			{
				try
				{
					createCurrentFileInSession( );
					Map<String, Object> newData = new HashMap<String, Object>( );
					newData.put( "Offset", "0" ); //get the first block
					changeState( SPOTLogCollectorState.SEND_FILE_BLOCK_UPLOAD_REQUEST, "UploadFileRequest" , newData);
				}
				catch (IOException e)
				{
					logger.error( "File could not be created. " + e.getMessage( ), e );
					changeState( SPOTLogCollectorState.SHOW_ERROR, "File could not be created." );
				}
			}
			else
			{
				logger.info( "No blocks to request. Proceeding to next file." );
				changeState( SPOTLogCollectorState.GET_FILE_TO_REQUEST, "UploadFileRequest" );
			}
		}
		else if (ack == SpotMessageErrorCode.CANNOT_READ_RESOURCE.getCode()) // file not found
		{
			logger.info( "File not found. Proceeding to next file." );
			changeState( SPOTLogCollectorState.GET_FILE_TO_REQUEST, "UploadFileRequest" );
		}
		else
		{
			String sNAck = " ";
			try
			{
				sNAck = String.format(" 0x%02X ", (ack & 0xFF));
			}
			catch (Exception e)
			{
				logger.error( "Error while formatting NAck retrieved value. [" + e.getMessage() + "]", e );
			}
			logger.error( "Error" + sNAck + "in file upload response message." );
			logger.error( "Collection Interrupted..." );
			changeState( SPOTLogCollectorState.SHOW_ERROR, "FileUploadResponseError" );
		}
	}

	private boolean blocksToRequest(Map<String, Object> data)
	{
		String fileSz = (String) data.get( "FileSz" );
		int size = Integer.decode( fileSz );
		logger.info( "Filsize = " + fileSz );
		return size > 0;
	}

	private void checkMoreFilesToUpload()
	{
		if (filesIterator.hasNext( ))
		{
			currentFile = filesIterator.next( );
			logger.info( "File to request - " + currentFile.getFileName( ) );
			changeState( SPOTLogCollectorState.SEND_FILE_UPLOAD_REQUEST, "FilesToSend" );
		}
		else
		{
			logger.info( "Collection Completed" );
			changeState( SPOTLogCollectorState.SHOW_SUCCESS, "CollectionComplete" );
		}
	}

	private void setUpProcess()
	{
		createFilesToUploadVector( );
		Path sessionDirectory = null;
		try
		{
			sessionDirectory = Paths.get(model.getLogsDownloadPath(),model.getCurrentIp(),EdtUtil.getDateTimeStamp("yyyy-MM-dd'T'HHmmss"));
		}
		catch (RuntimeException e)
		{
			sessionDirectory = Paths.get(model.getLogsDownloadPath(),model.getCurrentIp());
		}
		try
		{
			currentSessionDirectory = Files.createDirectories(sessionDirectory);
			logger.info( "Collection Initiated" );
			changeState( SPOTLogCollectorState.GET_FILE_TO_REQUEST, "GetFileToSend" );
		}
		catch (IOException e)
		{
			logger.error( "Directory " + sessionDirectory.toString() + " could not be created. " + e.getMessage( ), e );
			changeState( SPOTLogCollectorState.SHOW_ERROR, "Directory " + sessionDirectory.toString() + " could not be created. " + e.getMessage( ) );
		}
	}

	private void createCurrentFileInSession() throws IOException
	{
		if (currentSessionFileOutputStream != null)
		{
			try
			{
				currentSessionFileOutputStream.close();
			}
			catch (IOException e)
			{
				logger.error( "Error while closing unexpected old currentSessionFileOutputStream. " + e.getMessage( ), e );
			}
			currentSessionFileOutputStream = null;
		}
		if (currentFile != null)
		{
			if (currentSessionDirectory != null)
			{
				try
				{
					currentSessionFileOutputStream = new BufferedOutputStream( Files.newOutputStream(Paths.get(currentSessionDirectory.toString(),currentFile.getFileName()), StandardOpenOption.CREATE, StandardOpenOption.WRITE, StandardOpenOption.TRUNCATE_EXISTING), 524288 ); // Buffer size to hold before flushing is 512kb. 
				}
				catch (IOException e)
				{
					logger.error( "File " + Paths.get(currentSessionDirectory.toString(),currentFile.getFileName()).toString() + " could not be created. " + e.getMessage( ), e );
					throw e;					
				}
			}
		}
	}

	private void appendToCurrentFileInSession(byte[] data) throws IOException
	{
		currentSessionFileOutputStream.write(data);
	}

	private void closeCurrentFileInSession() throws IOException
	{
		if (currentSessionFileOutputStream != null)
		{
			try
			{
				currentSessionFileOutputStream.close();
				currentSessionFileOutputStream = null;
			}
			catch (IOException e)
			{
				if (currentFile != null)
				{
					if (currentSessionDirectory != null)
					{
						logger.error( "File " + Paths.get(currentSessionDirectory.toString(),currentFile.getFileName()).toString() + " could not be properly closed. " + e.getMessage( ), e );
					}
				}
				currentSessionFileOutputStream = null;
				throw e;					
			}

			if (currentFile != null)
			{
				if (currentSessionDirectory != null)
				{
					Path closedFileToCheck = Paths.get(currentSessionDirectory.toString(),currentFile.getFileName());
					try
					{
						if (Files.size(closedFileToCheck) > 0)
						{
							logger.info( "File " + closedFileToCheck.toString() + " has been saved." );								
						}
						else
						{
						    Files.delete(closedFileToCheck);
							logger.info( "File " + closedFileToCheck.toString() + " has been reported but no content is stored. File removed." );								
						}
					}
					catch (NoSuchFileException e)
					{
						logger.error( "File " + closedFileToCheck.toString() + " could not be properly checked. File not exists. " + e.getMessage( ), e );
					}
					catch (DirectoryNotEmptyException e)
					{
						logger.error( "File " + closedFileToCheck.toString() + " could not be properly checked. Directory conflict. " + e.getMessage( ), e );
					}
					catch (IOException e)
					{
						logger.error( "File " + closedFileToCheck.toString() + " could not be properly checked. " + e.getMessage( ), e );
					}
					catch (SecurityException e)
					{
						logger.error( "File " + closedFileToCheck.toString() + " could not be properly accessed for check. " + e.getMessage( ), e );
					}
				}
			}
		}
	}

	private void processSuccess()
	{
		changeState( SPOTLogCollectorState.CLEANUP_PROCESS, "CleanUpProcess" );
	}

	private void processError()
	{
		changeState( SPOTLogCollectorState.CLEANUP_PROCESS, "CleanUpProcess" );
	}
	
	private void processSpotError()
	{
		changeState( SPOTLogCollectorState.SHOW_ERROR, "Spot Error" );
	}
	
	private void cleanUpProcess()
	{
		try
		{
			closeCurrentFileInSession();
		}
		catch (IOException e)
		{
			logger.error( "Error while trying to close currentSessionFileOutputStream. " + e.getMessage( ), e );
		}
		currentSessionFileOutputStream = null;
		currentSessionDirectory = null;
		cleanFilesToUploadVector( );
		changeState( SPOTLogCollectorState.DISCONNECT, "Disconnect" ); 
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void subscribeToEvents() throws EdtServiceException
	{
		SERVICE_MANAGER.subscribe( new StateChangedAction( ) );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class StateChangedAction implements EdtEventAction<EdtProcessEvent>
	{
		@Override
		public void onInvokeEvent(EdtProcessEvent event)
		{
			SPOTLogCollectorState appState = SPOTLogCollectorState.IDLE;
			appState = SPOTLogCollectorState.valueOf( event.getDestination( ) );
			switch(appState)
			{
				case IDLE :
					break;
				case CONNECT_AND_LOGIN :
					connectAndLogin( );
					progressObserver.notifyConnected();
					break;
				case SETUP_PROCESS :
					setUpProcess( );
					break;
				case GET_FILE_TO_REQUEST :
					checkMoreFilesToUpload( );
					progressObserver.notifyProgress();
					break;
				case SEND_FILE_UPLOAD_REQUEST :
					sendFileUploadRequest( );
					progressObserver.notifyProgress();
					break;
				case GET_FILE_UPLOAD_RESPONSE :
					getFileUploadResponse( event.getEventProperties( ) );
					progressObserver.notifyProgress();
					break;
				case SEND_FILE_BLOCK_UPLOAD_REQUEST :
					sendFileBlockUploadRequest( event.getEventProperties( ) );
					progressObserver.notifyProgress();
					break;
				case GET_FILE_BLOCK_UPLOAD_RESPONSE :
					getFileBlockUploadResponse( event.getEventProperties( ) );
					progressObserver.notifyProgress();
					break;
				case SPOT_ERROR :
					processSpotError( );
					break;
				case SHOW_SUCCESS :
					processSuccess( );
					progressObserver.notifySuccess();
					break;
				case SHOW_ERROR :
					processError( );
					progressObserver.notifyFailed();
					break;
				case CLEANUP_PROCESS :
					cleanUpProcess( );
					break;
				case DISCONNECT :
					disconnect( );
					progressObserver.notifyDisconnected();
					break;
				default :
					logger.info( "state not valid " + appState.name( ) );
					break;
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
			return EdtEvent.EVENT_TYPE_FILTER + "='" + ProcessEventType.STATE_CHANGED.name( ) + "' AND "
					+ EdtProcessEvent.DESTINATION_STATE_FILTER + " IN ('"
					+ SPOTLogCollectorState.IDLE + "', '"
					+ SPOTLogCollectorState.CONNECT_AND_LOGIN + "', '"
					+ SPOTLogCollectorState.SETUP_PROCESS + "', '"
					+ SPOTLogCollectorState.GET_FILE_TO_REQUEST + "', '"
					+ SPOTLogCollectorState.SEND_FILE_UPLOAD_REQUEST + "', '"
					+ SPOTLogCollectorState.GET_FILE_UPLOAD_RESPONSE + "', '"
					+ SPOTLogCollectorState.SEND_FILE_BLOCK_UPLOAD_REQUEST + "', '"
					+ SPOTLogCollectorState.GET_FILE_BLOCK_UPLOAD_RESPONSE + "', '"
					+ SPOTLogCollectorState.SHOW_ERROR + "', '"
					+ SPOTLogCollectorState.SPOT_ERROR + "', '" // should be equal to SpotState.SPOT_ERROR.name( ) because it is SpotTesterApp client.
					+ SPOTLogCollectorState.SHOW_SUCCESS + "', '"
					+ SPOTLogCollectorState.CLEANUP_PROCESS + "', '"
					+ SPOTLogCollectorState.DISCONNECT + "')";
		}
		
		@Override
		public boolean justOnce()
		{
			return false;
		}
	}

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
