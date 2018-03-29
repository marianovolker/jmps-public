/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.engines;

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.io.File;
import java.io.IOException;
import java.util.Calendar;
import java.util.Formatter;
import java.util.Map;
import java.util.Observable;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.flexpay.maintenance.resources.SignedResource;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

public class ResourceUploadEngine extends Observable
{
	// ******************************************************************
	// INNER TYPES
	// ******************************************************************
	private enum ResourceState
	{
		IDLE, DELETING, UPLOADING, ERROR;
	}

	public class BadResourceFilenameException extends Exception
	{
		private static final long	serialVersionUID	= 1L;

		public BadResourceFilenameException()
		{
			super();
		}

		public BadResourceFilenameException(String sErrorDescription)
		{
			super(sErrorDescription);
		}
	}


	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( ResourceUploadEngine.class );
	private static final String sFilenameError = "Bad filename for parsing resource parameters.";
	private static final byte	BAD_FILE_TYPE		= (byte) 0xFF;
	private static final byte	BAD_AUTHENT_TYPE	= (byte) 0xFF;
	private static final byte	BAD_FILE_ID			=			0;
	private static final int	MAX_FILEBLOCK_LENGTH = 		8192;

	/** Default target unit connection timeout measured in milliseconds */ 
	public static final long DEFAULT_TIMEOUT_MILLISECONDS = 60000L;

	/** The socket disconnect event processor. */
	private EdtClientSocketDisconnectAction	socketDisconnectEventProcessor;


	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************
	public Vector<File>				filesToUploadVector	= null;

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected SignedResource		currentResource;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private byte					currentFileType		= BAD_FILE_TYPE;
	private byte					currentAuthentType	= BAD_AUTHENT_TYPE;
	private byte					currentFileId		= BAD_FILE_ID;
	private volatile boolean		blOperationError 	= false;
	private volatile ResourceState	currentState		= ResourceState.IDLE;


	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public ResourceUploadEngine() throws EdtServiceException
	{
		super( );
		currentState = ResourceState.IDLE;
		subscribeToEvents( );
		filesToUploadVector = new Vector<File>( );
	}


	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	public void setCurrentResource(SignedResource currentResource)
	{
		this.currentResource = currentResource;
	}

	public File getCurrentFile()
	{
		return currentResource.getFile();
	}

	public void clearErrorCondition()
	{
		blOperationError = false;
	}

	public synchronized boolean hasErrors()
	{
		return blOperationError;
	}

	public void addFileToUpload(File file)
	{
		filesToUploadVector.add( file );
	}

	public void clearVectorFilesToUpload()
	{
		filesToUploadVector.clear( );
	}

	/**
	 * 
	 * Resource name format:  
	 * <code>IDXXX_Y_comment.ZZZ</code>
	 *
	 * Where:
	 * <p>
	 * <code>XXX<code> is the zero-padded decimal representation of the FileId value (000,255) to be
	 * used for downloading to SPOT.
	 * <p>
	 * <code>Y<code> is the authentication flag with the following meaning:
	 * // comment is the optional ASCII string used as download command parameters to provide
	 * some description of the resource file when stored on SPOT. In particular, the file
	 * browse command will return this description to the master program.
	 * <p>
	 * <code>ZZZ<code> This is the file extension and used to be related with the FileType and to the file internal // format. // Message files (0x10): .TXT //
	 * Images (0x11): .GIF or .JPG or .BMP // Font(0x12): .FNT or .BIN // Log(0x13): .LOG // Display Template(0x15): .TMP
	 * <p>
	 * This is no longer valid, all resources are .bin right now.
	 * @throws InterruptedException 
	 * 
	 */
	public void upgradeResources() throws InterruptedException
	{
		int iUpdatedFiles = 0;
		
		clearErrorCondition();

		for (File file : filesToUploadVector)
		{
			String sFilename = file.getName().toLowerCase();
			String sFullFilePath = null;
			SignedResource resource = null;
			
			iUpdatedFiles++;

			try
			{
				sFullFilePath = file.getCanonicalPath();
			}
			catch (IOException e)
			{
				logger.error(e.getMessage());

				blOperationError = true;
				continue;
			}

			try
			{
				currentFileType		= BAD_FILE_TYPE;
				currentAuthentType	= BAD_AUTHENT_TYPE;
				currentFileId		= BAD_FILE_ID;

				this.setFileId( sFilename );
				this.setAuthenticationType( sFilename );
				this.setFileType( sFullFilePath );
			}
			catch (BadResourceFilenameException e)
			{
				String sErr = e.getMessage() + "  File : " + sFullFilePath;
				logger.error( sErr );

				blOperationError = true;
				continue;
			}

			resource = new SignedResource( file );
			if(resource.hasError())
			{
				blOperationError = true;
			}
			else
			{
				setCurrentResource( resource );
				fileDeletionAndUpload( );
				setChanged();
				UploadNotification u = new UploadNotification(blOperationError, filesToUploadVector.size(), iUpdatedFiles);
				notifyObservers( u );
			}
		}
	}


	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected synchronized void wakeup()
	{
		this.notify();
	}

	protected void sendDeleteFile()
	{
		this.sendData( SpotMessages.getInstance( ).getFileDeleteData( currentFileType,
						currentFileId ) );
	}

	protected void sendFileRequest()
	{
		logger.debug("Download Request for resource #" + currentFileId + ", type " + currentAuthentType + ", auth " + currentAuthentType);

		String sName = null;
		String sFileName = currentResource.getName().trim();

		if(sFileName.length() > 16)
		{
			sName = sFileName.substring(0, 15);
		}
		else
		{
			sName = sFileName;
		}

		sendData( SpotMessages.getInstance( ).getFileDownloadRequestData(
						currentResource.getData(),
						currentFileType,
						currentFileId,
						currentAuthentType,
						sName
					)
				); /* len <=16 for currentFile.getName() */
	}

	protected boolean sendNextBlock(String offset)
	{
		try
		{
			byte[] data = null;
			int iOffSet = EdtConvert.stringToInt( offset );

			if( currentResource.hasError() )
			{
				throw new Exception("Resource [" + currentFileId + "] has errors (1).");
			}

			data = currentResource.getDataBlock(iOffSet, MAX_FILEBLOCK_LENGTH);

			if( currentResource.hasError() )
			{
				throw new Exception("Resource [" + currentFileId + "] has errors (2).");
			}

			if (data != null)
			{
				logger.debug("Next Block Request, ID: " + currentFileId +
							", offset " + offset);
				sendData( iOffSet, data.length, data );
				return true;
			}

		}
		catch (Exception e)
		{
			currentState = ResourceState.ERROR;
			String sErr = "ResourceUploadEngine - Error sending data : " + e.getMessage();
			logger.error( sErr );
		}

		return false;
	}

	protected void sendData(int offSet, int length, byte[] data)
	{
		sendData( SpotMessages.getInstance( ).getFileBlockDownloadData( offSet, length, data ) );
	}

	protected void sendData(byte[] data)
	{
		logger.debug( "Request message : " + EdtConvert.bytesToHexString( data, 0, data.length, 1) );
		try
		{
			ConnectionEngineSwingAdapter.getInstance().send(data);
		}
		catch (NullPointerException e)
		{
			logger.error("Error. Exception thrown while sending synchronouse data to target unit.\nEdtServiceException: '" + e.getMessage() + "'.");
		}
	}

	protected void subscribeToEvents()
	{
		try
		{
			if (socketDisconnectEventProcessor == null)
			{
				socketDisconnectEventProcessor = new EdtClientSocketDisconnectAction();
				SERVICE_MANAGER.subscribe( socketDisconnectEventProcessor );
			}

			SERVICE_MANAGER.subscribe( new EdtEventAction<MgpResultEvent>()
			{
				@Override
				public void onInvokeEvent(MgpResultEvent event)
				{
					if( event.getMessageName().equals(SpotMessageType.FILE_DELETE.getName()) )
					{
						int iAck = event.getAckCode();
						if( iAck == 0x00 || iAck == 0x17 )
						{
							// file deleted, or didn't exist, now it enables the block uploading
							currentState = ResourceState.UPLOADING;
						}
						else
						{
							currentState = ResourceState.ERROR;
						}

						wakeup(); // wakes up the wait()
					}
					else if
					(
						event.getRawData()[2] == (byte) 0x81 &&
						(
							event.getMessageName( )
								.equals( SpotMessageType.FILE_BLOCK_DOWNLOAD.getName( ) )
							||
							event.getMessageName( )
								.equals( SpotMessageType.FILE_DOWNLOAD_REQUEST.getName( ) )
						)
					)
					{
						if (event.isValid( ))
						{
							@SuppressWarnings("unchecked")
							Map<String, Object> map = (Map<String, Object>) event
											.getDecodeMap()
											.get( event.getDecodeMap().keySet().iterator().next() );

							if ( !sendNextBlock( (String) map.get( "Offset" ) ) )
							{
								// look for another file in the list
								if( ! (currentState == ResourceState.ERROR) )
									currentState = ResourceState.IDLE;

								wakeup(); // wakes up the wait()
							}
						}
						else
						{
							currentState = ResourceState.ERROR;
							wakeup(); // wakes up the wait()
						}
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
					return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name( )
							+ "'";
				}

				@Override
				public boolean justOnce()
				{
					return false;
				}
			} );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ));
		}
	}


	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	private void fileDeletionAndUpload() throws InterruptedException
	{
		if( currentResource == null || currentFileType == BAD_FILE_TYPE
			|| currentAuthentType == BAD_AUTHENT_TYPE || currentFileId == BAD_FILE_ID )
		{
			logger.error( "Resource upload skipped because no file has been set." );
			blOperationError = true;
			return;
		}

		// In Java ... bytes are signed ??!!
		int idRes = (currentFileId >= 0 ? currentFileId : currentFileId + 256);
		String sTempl, sMsg, sFilename = "", sFullFilePath = "";
		StringBuilder sMsgBuffer = new StringBuilder();
		Formatter f = new Formatter(sMsgBuffer);

		try
		{
			sFilename = currentResource.getFile().getName();
			sFullFilePath = currentResource.getFile().getCanonicalPath();
			sTempl = "Deleting filename [%d]: %s ....";
			sMsg = f.format( sTempl, idRes, sFullFilePath ).out().toString();
			logger.debug( sMsg );
			sMsgBuffer.delete(0, sMsgBuffer.length());
		}
		catch (IOException e)
		{
			String sErr = "Error logging resource name: " + e.getMessage( );
			logger.error( sErr );
		}

		// Deletes the current file on list from OPT terminal, to ensure no upload errors.
		// After the proper OPT reply of this, the new resource on list will be updated.
		currentState = ResourceState.DELETING;
		sendDeleteFile();

		synchronized (this)
		{
			long t0 = Calendar.getInstance().getTimeInMillis();
			long t1;
			while( currentState == ResourceState.DELETING )
			{
				wait(DEFAULT_TIMEOUT_MILLISECONDS);
				t1 = Calendar.getInstance().getTimeInMillis();
				if(t1 - t0 >= DEFAULT_TIMEOUT_MILLISECONDS) // Spurious wakeup prevention
				{
					break;
				}
			}

			// Timeout or Error deleting resource file
			if( currentState == ResourceState.DELETING || currentState == ResourceState.ERROR)
			{
				sTempl = "Resource [%d]: %s file upload skipped because previous deletion ";
				String sTempl1 = null;
				if(currentState == ResourceState.DELETING)
				{
					sTempl1 = "had answer DISCONNECT or TIMEOUT.";
				}
				else
				{
					sTempl1 = "couldn't be done.";
				}

				sMsg = f.format( sTempl + sTempl1, idRes, sFilename ).out().toString();
				logger.error( sMsg );
				
				sMsgBuffer.delete(0, sMsgBuffer.length());
				blOperationError = true;
				
				f.close( );
				
				return;
			}
			else
			{
				sTempl = "Resource [%d]: %s has been deleted successfully.";
				sMsg = f.format( sTempl, idRes, sFilename ).out().toString();
				logger.info( sMsg );
				sMsgBuffer.delete(0, sMsgBuffer.length());
			}

			// current STATE is UPLOADING now
			sTempl = "Updating filename [%d]: %s ....";
			sMsg = f.format( sTempl, idRes, sFullFilePath ).out().toString();
			logger.debug( sMsg );			
			sMsgBuffer.delete(0, sMsgBuffer.length());
			sendFileRequest();

			while( currentState == ResourceState.UPLOADING )
			{
				wait(50000L);
				t1 = Calendar.getInstance().getTimeInMillis();
				if(t1 - t0 >= 50000L) // Spurious wake-up prevention
				{
					break;
				}
			}
			if(currentState == ResourceState.UPLOADING || currentState == ResourceState.ERROR)
			{
				String sErr = "ERROR";
				if(currentState == ResourceState.UPLOADING)
				{
					sErr = "DISCONNECT or TIMEOUT on reply";
				}

				sTempl = "Resource [%d]: %s uploading file %s . Skipped.";
				sMsg = f.format( sTempl, idRes, sErr, sFilename ).out().toString();
				logger.error( sMsg );
				sMsgBuffer.delete(0, sMsgBuffer.length());
				blOperationError = true;
			}
			else
			{
				sTempl = "Resource [%d]: %s has been sent successfully.";
				sMsg = f.format( sTempl, idRes, sFilename ).out().toString();
				logger.info( sMsg );

				sMsgBuffer.delete(0, sMsgBuffer.length());
			}
			
			f.close( );

			return;
		}
	}

	private void setFileId(String sFilename) throws BadResourceFilenameException
	{
		int i0 = 0; // some indexes
		String sFileId;

		// search for "id"
		i0 = sFilename.indexOf("id");
		if(i0 < 0)
		{
			throw new BadResourceFilenameException(sFilenameError);
		}

		// set file id
		sFileId = sFilename.substring(i0 + 2, i0 + 5);
		this.currentFileId = (byte) Integer.parseInt( sFileId );
	}

	private void setAuthenticationType(String sFilename) throws BadResourceFilenameException
	{
		int i0 = 0, i1 = 0; // some indexes
		String sAuthenType;

		// search for '_'
		i0 = sFilename.indexOf('_');
		if(i0 < 0)
		{
			throw new BadResourceFilenameException(sFilenameError);
		}

		// search for '_' again
		i1 = sFilename.indexOf('_', 1 + i0);
		if(i1 < 0)
		{
			throw new BadResourceFilenameException(sFilenameError);
		}

		// set authentication type
		sAuthenType = sFilename.substring( 1 + i0, i1 );
		this.currentAuthentType = (byte) Integer.parseInt( sAuthenType );
	}

	private void setFileType(String sFullFilePath) throws BadResourceFilenameException
	{
		currentFileType = getResourceTypeFromFilePath(sFullFilePath);
	}

	private byte getResourceTypeFromFilePath(String sFullFilePath) throws BadResourceFilenameException
	{
		String sPath = sFullFilePath.toLowerCase();
		
		// questions in decreasing order of probability
		if(sPath.indexOf("pinpad" + File.separator + "displayscreen") >= 0)
		{
			return 0x15;
		}
		else if(sPath.indexOf("pinpad" + File.separator + "image") >= 0)
		{
			return 0x11;
		}
		else if(sPath.indexOf("pinpad" + File.separator + "font") >= 0)
		{
			return 0x12;
		}
		else if(sPath.indexOf("pinpad" + File.separator + "messages") >= 0)
		{
			return 0x10;
		}
		else if(sPath.indexOf("pinpad" + File.separator + "log") >= 0)
		{
			return 0x16;
		}
		else if(sPath.indexOf("emv" + File.separator + "messages") >= 0)
		{
			return 0x21;
		}
		else if(sPath.indexOf("emv" + File.separator + "table") >= 0)
		{
			return 0x20;
		}
		else if(sPath.indexOf("emv" + File.separator + "script") >= 0)
		{
			return 0x22;
		}
		else if(sPath.indexOf("emv" + File.separator + "log") >= 0)
		{
			return 0x23;
		}
		else if(sPath.indexOf("application" + File.separator + "crindbios") >= 0)
		{
			return 0x7A;
		}
		else if(sPath.indexOf("application" + File.separator + "rescrind") >= 0)
		{
			return 0x7A;
		}
		else if(sPath.indexOf("pinpad" + File.separator + "keymaps") >= 0)
		{
			return 0x14;
		}
		else if(sPath.indexOf("opt" + File.separator + "bitmap") >= 0)
		{
			return 0x42;
		}
		else // invalid resource filename => invalid resource type
		{
			throw new BadResourceFilenameException(sFilenameError);
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
				currentState = ResourceState.ERROR;
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

}
