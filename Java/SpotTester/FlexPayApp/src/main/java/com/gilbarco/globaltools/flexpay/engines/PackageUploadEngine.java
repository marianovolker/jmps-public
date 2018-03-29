/**
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file FlexPayMaintenanceController.java
 * @author gparis
 * @date 27 Ago 2012
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

package com.gilbarco.globaltools.flexpay.engines;

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.io.File;
import java.io.IOException;
import java.util.Calendar;
import java.util.Formatter;
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
import com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsPackage;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/**
 * @author gparis
 *
 */
public class PackageUploadEngine extends Observable
{
	// ******************************************************************
	// INNER TYPES 
	// ******************************************************************
	private enum PackageState
	{
		IDLE, UPLOADING, ERROR;
	}


	// ******************************************************************
	// STATIC FIELDS 
	// ******************************************************************
	private static final transient Log		logger							= LogFactory.getLog( PackageUploadEngine.class );
	public static final String				STR_EDTLOG_ERROR				= "Could not log in the EDT service log: ";
	private static final int				MAX_FILEBLOCK_LENGTH			= 8192;

	/** The socket disconnect event processor. */
	private EdtClientSocketDisconnectAction	socketDisconnectEventProcessor;

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************
	public Vector<ROMfsCertificate>		filesToUploadVector	= null;


	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected int						iBlockSize = -1;
	protected ROMfsPackage				currentPackage = null;
	protected IROMfsPackageUploadStrategy packageStrategy = null;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private volatile boolean			blSendingBlock = false;
	private volatile boolean			blOperationError = false;
	private volatile boolean			blAuthenticationReplyReceived = false;
	private volatile boolean			blActivationReplyReceived = false;
	private volatile byte				bLastAckCode = 0;
	private volatile int				nUpdatedBlocks = 0;
	private volatile int				nTotalBlocks = 0;
	private volatile PackageState		currentState = PackageState.IDLE;


	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public PackageUploadEngine(IROMfsPackageUploadStrategy strategy)
			throws EdtServiceException
	{
		super( );
		iBlockSize = MAX_FILEBLOCK_LENGTH;
		currentState = PackageState.IDLE;
		packageStrategy = strategy;
		subscribeToEvents( );
		filesToUploadVector = new Vector<ROMfsCertificate>( );
	}


	// ******************************************************************
	// STATIC METHODS 
	// ******************************************************************


	// ******************************************************************
	// OVERRIDDEN METHODS  (invoked from polymorphic interface).
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS	  (general, getter, setter, interface imp)
	// ******************************************************************

	public void setCurrentPackage(ROMfsPackage currentPackage)
	{
		this.currentPackage = currentPackage;
	}

	public File getCurrentFile()
	{
		return currentPackage.getFile();
	}

	public void clearErrorCondition()
	{
		blOperationError = false;
		blAuthenticationReplyReceived = false;
		blActivationReplyReceived = false;
	}

	public synchronized boolean hasErrors()
	{
		return blOperationError;
	}

	public synchronized boolean authenticationPresentationReplied()
	{
		return blAuthenticationReplyReceived;
	}

	public synchronized boolean activationReplied()
	{
		return blActivationReplyReceived;
	}

	public synchronized boolean waitAuthenticationReply() throws InterruptedException
	{
		wait( (int) packageStrategy.getAuthenticationPresentationTimeout() );
		return blAuthenticationReplyReceived;
	}

	public synchronized boolean waitActivationReply() throws InterruptedException
	{
		wait( (int) packageStrategy.getPackageActivationTimeout() );
		return blActivationReplyReceived;
	}

	public synchronized byte getLastAckCode()
	{
		return bLastAckCode;
	}

	public void addFileToUpload(ROMfsCertificate cert)
	{
		long lFileLength = cert.getFile().length();
		int nBlocks = (int) (lFileLength / MAX_FILEBLOCK_LENGTH);
		if( lFileLength % MAX_FILEBLOCK_LENGTH != 0 )
			nBlocks++;

		nTotalBlocks += nBlocks;
		filesToUploadVector.add( cert );
	}

	public void clearVectorFilesToUpload()
	{
		nTotalBlocks = nUpdatedBlocks = 0;
		filesToUploadVector.clear( );
	}

	public void upgradePackages() throws InterruptedException
	{
		for (ROMfsCertificate cert : filesToUploadVector)
		{
			ROMfsPackage romfsPackage = null;

			romfsPackage = packageStrategy.createROMfsPackage(cert);

			if(romfsPackage.hasError())
			{
				blOperationError = true;
			}
			else
			{
				setCurrentPackage( romfsPackage );
				romfsPackageUpload( );
			}
		}
	}

	public void activatePackagesAndRebootOPT()
	{
		sendData( packageStrategy.getPackageActivationRequest(null) );
	}

	public void rebootOPT()
	{
		sendData( SpotMessages.getInstance().getResetRequest() );
	}


	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected synchronized void wakeup()
	{
		this.notify();
	}

	protected void sendPackageRequest()
	{
		sendData( packageStrategy.getFileDownloadRequestMsgBytes
									(
										(int) currentPackage.length(),
										currentPackage.getCertificateSize(),
										1, // save package
										0, // restart (no resume operation)
										currentPackage.getCertificateData(),
										currentPackage.getSignatureData()
									) );
	}

	protected boolean sendNextBlock()
	{
		try
		{
			byte[] data = null;

			if( currentPackage.eof() ) // last null block already sent
				return false; // end of transfer

			if( currentPackage.hasError() )
				throw new Exception("Package " + currentPackage.getName() + " has errors (1).");

			data = currentPackage.getNextBlock(iBlockSize);

			if( currentPackage.hasError() )
				throw new Exception("Package " + currentPackage.getName() + " has errors (2).");

			if (data == null) // last data block has already been sent
			{
				// still another empty block is required to signal the end of the transfer.
				sendBlockData(0, null );
			}
			else // some package bytes are pending to be sent
			{
				sendBlockData(data.length, data );
				nUpdatedBlocks++;
			}

			blSendingBlock = false; // end of block sending operation
			return true;
		}
		catch (Exception e)
		{
			currentState = PackageState.ERROR;
			String sErr = "PackageUploadEngine - Error sending data : " + e.getMessage();
			logger.error( sErr );
		}

		return false;
	}

	protected void sendBlockData(int length, byte[] data)
	{
		sendData( packageStrategy.getBlockDownloadRequestMsgBytes( length, data ) );
	}

	protected void sendData(byte[] data)
	{
		logger.debug( "Request message : " + EdtConvert.bytesToHexString( data, 0, data.length, 1 ) );
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
					if( isROMfsPackageUpdateResponseAppID( event ) ) 
					{
						bLastAckCode = event.getAckCode();
						blActivationReplyReceived = true;

						String sError = "";
						if( event.isValid() )
						{
							sError = "successfully.";
						}
						else
						{
							sError = "with errors - " + bLastAckCode + ".";
							blOperationError = true;
						}

						logger.info("Package activation reply received " + sError);
						wakeup();
					}
					else if( isFileOrBlockDownloadAnswer( event ) )
					{
						if (event.isValid( ))
						{
							logger.debug("event received = "+ EdtConvert.bytesToHexString(event.getRawData()));
							if ( !sendNextBlock() )
							{
								// look for another file in the list
								if( ! (currentState == PackageState.ERROR) )
								{
									currentState = PackageState.IDLE;
								}
							}
						}
						else
						{
							logger.error("Error = "+ event.getAckCode( ));
							currentState = PackageState.ERROR;
						}						
						wakeup();
					}
				}

				/**
				 * Checks if is rOmfs package update response app id.
				 *
				 * @param event the event
				 * @return true, if is rO mfs package update response app id
				 */
				private boolean isROMfsPackageUpdateResponseAppID(MgpResultEvent event)
				{
					return event.getRawData()[2] == packageStrategy.getROMfsPackageUpdateResponseAppID()
							 && packageStrategy.isPackageActivationCommandName(event.getMessageName());
				}

				/**
				 * Checks if is file or block download answer.
				 *
				 * @param event the event
				 * @return true, if is file or block download answer
				 */
				private boolean isFileOrBlockDownloadAnswer(MgpResultEvent event)
				{
					return event.getRawData()[2] == packageStrategy.getROMfsPackageUpdateResponseAppID()
						     && packageStrategy.isFileOrBlockDownloadCommandName(event.getMessageName());
				}

				@Override
				public Class<MgpResultEvent> getEventClass()
				{
					return MgpResultEvent.class;
				}

				@Override
				public String getSubscriptionFilter()
				{				
					return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name( ) + "' AND " + MgpResultEvent.MESSAGE_NAME 
									+ " IN ('" + SpotMessageType.M3M5_ROMfs_PACKAGE_ACTIVATION.getName( ) 
									+ "', '"+ SpotMessageType.VANGUARD_ROMfs_PACKAGE_ACTIVATION.getName( ) 
									+ "', '"+ SpotMessageType.VANGUARD_ROMfs_FILE_DOWNLOAD.getName( )
									+ "', '"+ SpotMessageType.VANGUARD_ROMfs_BLOCK_DOWNLOAD.getName( )
									+ "', '"+ SpotMessageType.M3M5_ROMfs_FILE_DOWNLOAD.getName( )
									+ "', '"+ SpotMessageType.M3M5_ROMfs_BLOCK_DOWNLOAD.getName( )
									+ "')";					
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
			logger.warn(STR_EDTLOG_ERROR + e.getMessage());
		}
	}


	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void romfsPackageUpload() throws InterruptedException
	{
		if( currentPackage == null )
		{
			logger.error( "Package upload skipped because no file has been set." );
			blOperationError = true;
			return;
		}

		int iPkgLength = (int) currentPackage.length();
		String sTempl, sMsg, sFilename = "", sFullFilePath = "";
		StringBuilder sMsgBuffer = new StringBuilder();
		Formatter f = new Formatter(sMsgBuffer);

		try
		{
			sFilename = currentPackage.getFile().getName();
			sFullFilePath = currentPackage.getFile().getCanonicalPath();
			sTempl = "Updating package %s , %d bytes long....";
			sMsg = f.format( sTempl, sFullFilePath, iPkgLength ).out().toString();
			logger.info( sMsg );
		}
		catch (IOException e)
		{
			logger.error( "Error logging package name: " + e.getMessage( ) );
		}
		

		sMsgBuffer.delete(0, sMsgBuffer.length());
		blSendingBlock = true;
		currentState = PackageState.UPLOADING;

		sendPackageRequest();
		notifyProgress(false);

		synchronized (this)
		{
			// This is for NOT notifying the GUI each time a block is sent.
			// Otherwise the GUI may look overwhelmed for large packages ( > 1000 blocks)
			// and the CPU consumption may delay request/replies beyond the 5s timeout.
			// So, upper limit is 100 notifications per package (1% in progress bar)
			int iThresholdBlockQty = (nTotalBlocks > 99 ? nTotalBlocks / 100 : 1);

			long t0 = Calendar.getInstance().getTimeInMillis();
			long t1;

			while( currentState == PackageState.UPLOADING )
			{
				wait( packageStrategy.getMsgAnswerTimeout() );
				
				if( currentState == PackageState.IDLE || currentState == PackageState.ERROR )
				{
					break; // end of package transfer
				}

				if( !blSendingBlock ) // it gets awake each time a block is entirely sent
				{
					// marks the beginning of the next block sending operation
					blSendingBlock = true;

					// only every "iThresholdBlockQty" blocks, the GUI will be updated
					if(nUpdatedBlocks % iThresholdBlockQty == 0)
					{
						notifyProgress( blOperationError );
					}
					continue;
				}

				t1 = Calendar.getInstance().getTimeInMillis();

				if( t1 - t0 >= packageStrategy.getMsgAnswerTimeout() ) // Spurious wake-up prevention
				{
					break; // real file sending TIMEOUT or DISCONNECT
				}
			}
			
			if( currentState == PackageState.UPLOADING || currentState == PackageState.ERROR )
			{
				String sErr = "ERROR";
				if( currentState == PackageState.UPLOADING )
				{
					sErr = "DISCONNECT or TIMEOUT on reply";
				}

				sTempl = "%s uploading package %s . Skipped.";
				sMsg = f.format( sTempl, sErr, sFilename ).out().toString();
				logger.error( sMsg );
				sMsgBuffer.delete(0, sMsgBuffer.length());
				blOperationError = true;
			}
			else
			{
				sTempl = "Package %s has been sent successfully.";
				sMsg = f.format( sTempl, sFilename ).out().toString();
				logger.info( sMsg );				
				sMsgBuffer.delete(0, sMsgBuffer.length());
			}
			
			f.close( );

			return;
		}
	}


	/**
	 * Notify the progress of the uploading process.
	 *
	 * @param operationError the operation error
	 */
	private void notifyProgress(boolean operationError)
	{
		UploadNotification u;
		u = new UploadNotification(operationError, nTotalBlocks, nUpdatedBlocks);
		setChanged();					
		notifyObservers( u );
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
				currentState = PackageState.ERROR;
				blOperationError = true;
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
