/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceCollectionEngine.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines;

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.util.Calendar;
import java.util.Collection;
import java.util.HashMap;
import java.util.ListIterator;
import java.util.Map;
import java.util.Observable;
import java.util.Observer;
import java.util.Vector;

import javax.xml.bind.DatatypeConverter;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmission;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmissionStorage;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmissionWriter;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationFileResourceTransmission;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationStatusFailed;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationStatusInitiated;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationStatusSucceeded;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageErrorCode;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/** @class FileResourceCollectionEngine FileResourceCollectionEngine.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines"
 * @brief This class provides an engine which will contain the FileResource Collection Business Logic.
 *
 * @see Observable
 * 
 * @since 1.0.0
 */
public class FileResourceCollectionEngine extends Observable
{
	/** @brief Logger instance for this class type */
	static final transient Log logger = LogFactory.getLog( FileResourceCollectionEngine.class );

	/** @class FileResourceCollectionEngine.SPOTFileUploadMgpResultEdtEventActionListener FileResourceCollectionEngine.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.FileResourceCollectionEngine"
	 * @brief This class process the internal MGP/EDT3 spot message flow for the parent engine.
	 *
	 * @see EdtEventAction
	 * @see MgpResultEvent
	 * 
     * @since 1.0.0
	 */
	private class SPOTFileUploadMgpResultEdtEventActionListener implements EdtEventAction<MgpResultEvent>
	{
		/** @brief List of fields retrieved from the currently SPOT Mgp message event received and parsed, to be used in subsequent processes */ 
		private Map<String, Object>	eventProperties;

		/** @brief Default constructor
		 * 
		 * @since 1.0.0
		 */
		public SPOTFileUploadMgpResultEdtEventActionListener ()
		{
			this.eventProperties = new HashMap<String, Object>();
		}

		/** @brief Event class getter
		 * 
		 * @see MgpResultEvent
		 * 
		 * @since 1.0.0
		 */
		@Override
		public Class<MgpResultEvent> getEventClass ()
		{
			return MgpResultEvent.class;
		}

		/** @brief Provides the topics for the event queue subscription filter.
		 * 
		 * @return String which contains the topics for the event queue subscription filter. 
		 * 
		 * @since 1.0.0
		 */
		@Override
		public String getSubscriptionFilter ()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name( ) + "' AND " + MgpResultEvent.MESSAGE_NAME + " IN ('"
					+ SpotMessageType.FILE_UPLOAD.getName( ) + "', '"
					+ SpotMessageType.FILE_BLOCK_UPLOAD.getName( ) + "', '" + "')";
		}

		/** @brief Provides information regarding this event listener execution policy.
		 * 
		 * @return Always false for this class as it is not implemented as a one-shoot instance. 
		 * 
		 * @since 1.0.0
		 */
		@Override
		public boolean justOnce ()
		{
			return false;
		}

		/** @brief Entry point for this event listener.
		 * 
		 * @param Current event received for processing. 
		 * 
		 * @see MgpResultEvent
		 * 
		 * @since 1.0.0
		 */
		@Override
		public void onInvokeEvent (MgpResultEvent event)
		{
			synchronized (messageEventProcessor)
			{
				if (bProcessing)
				{
					try
					{
						if (isInvalidEvent(event))
						{
							logger.warn("Warning. MgpResults Error - " + event.getMessageName() + " -  Error code: " + event.getAckCode() + " - " + SpotMessageErrorCode.getDescription(event.getAckCode()));
							processError("Invalid message received");
							return;
						}
			
						SpotMessageType msg = SpotMessageType.getByName(event.getMessageName());
						switch(msg)
						{
							//SPOT answer to file upload request
							case FILE_UPLOAD :
								this.eventProperties.clear();
								this.eventProperties.put("Ack",event.getAckCode());
								this.eventProperties.put("FileSz",getKeyData(event,"FileSz"));
								getFileUploadResponse(this.eventProperties);
								break;
							//SPOT answer to file block upload request
							case FILE_BLOCK_UPLOAD :
								this.eventProperties.clear();
								this.eventProperties.put("Ack",event.getAckCode());
								this.eventProperties.put("Offset",getKeyData(event,"Offset"));
								this.eventProperties.put("BlkSize",getKeyData(event,"BlkSize"));
								this.eventProperties.put("BlkData",getBlkData(event));
								getFileBlockUploadResponse(this.eventProperties);
								break;
							default :
								logger.warn("Warning. Unexpected SPOT message - '" + msg.name() + "'.");
								break;
						}

						lLastTimestampUpdate = Calendar.getInstance().getTimeInMillis();
					}
					catch (Exception e)
					{
						logger.error("Unknown Exception thrown at the MgpResultEvent/EdtEventAction Listener. " + e.getMessage(),e);
					}
				}
			}
		}

		/** @brief SPOT Mgp message processed validator.
		 * 
		 * @param SPOT MgpResultEvent to be validated.
		 * 
		 * @return True if the MgpResultEvent is NOT valid. False otherwise. 
		 * 
		 * @see MgpResultEvent
		 * 
		 * @since 1.0.0
		 */
		private boolean isInvalidEvent (MgpResultEvent event)
		{
			// Don't consider message invalid when ack is cannot_read_resource and message type is FileUpload.
			boolean ackok = event.getAckCode() != SpotMessageErrorCode.CANNOT_READ_RESOURCE.getCode();
			String messageName = event.getMessageName();
			String spotmsgname = SpotMessageType.FILE_UPLOAD.getName();
			boolean messagenameok = !messageName.equals(spotmsgname);
			return (!event.isValid()) && (ackok || messagenameok);
		}

		/** @brief Extracts the Spot Mgp message field value corresponding to the given key from the given Mgp message 
ed.
		 * 
		 * @param SPOT MgpResultEvent to be serched.
		 * @param SPOT Key to match in search.
		 * 
		 * @return String containing the value obtained for the given Mgp message and key. 
		 * 
		 * @see MgpResultEvent
		 * 
		 * @since 1.0.0
		 */
		private String getKeyData (MgpResultEvent event, String key)
		{
			@SuppressWarnings("unchecked")
			Map<String, Object> map = (Map<String, Object>) event.getDecodeMap().get(event.getDecodeMap().keySet().iterator().next());
			return (String) map.get(key);
		}

		/** @brief Extracts the SPOT Mgp message "BlkData" field value from the given Mgp message processed.
		 * 
		 * @param MgpResultEvent to be serched.
		 * @param Key to match in search.
		 * 
		 * @return String containing the value obtained for the given Mgp message and key.
		 * 
		 * @see MgpResultEvent
		 * 
		 * @since 1.0.0
		 */
		private byte[] getBlkData (MgpResultEvent event)
		{
			@SuppressWarnings("unchecked")
			Map<String, Object> map = (Map<String, Object>) event.getDecodeMap().get(event.getDecodeMap().keySet().iterator().next());

			byte[] byHex = new byte[]{};
			String sHex = (String) map.get("BlkData");
			if (sHex != null)
			{
				try
				{
					if (sHex.length() > 2)
					{
						byHex = DatatypeConverter.parseHexBinary(sHex.substring(2));
					}
				}
				catch (Exception e)
				{
					logger.error("Error while parsing BlkData field. " + e.getMessage(),e);
				}
			}
			return (byte[]) byHex;
		}
	}

	/** @brief Default target unit connection timeout measured in milliseconds */ 
	public static final long DEFAULT_TIMEOUT_MILLISECONDS = 160000L;

	/** @brief Default Caption for the FileResource element */
	private static final String DEFAULT_CAPTION_FILERESOURCE = "FileResource";
	/** @brief Default Caption published if collection was successful */
	private static final String DEFAULT_CAPTION_SUCCESSFUL = DEFAULT_CAPTION_FILERESOURCE + " Collection Successful";
	/** @brief Default Caption published if collection had failed */ 
	private static final String DEFAULT_CAPTION_FAIL = DEFAULT_CAPTION_FILERESOURCE + " Collection Failed";

	/** @brief Caption to be published for the FileResource element */
	private String captionFileResource;
	/** @brief Caption to be published if collection was successful */
	private String captionSucessful;
	/** @brief Caption to be published if collection had failed */ 
	private String captionFail;
	
	/** @brief List of FileResources writers which can keep transmission progress information */ 
	private Vector<IFileResourceTransmissionWriter>       uploadVector;
	/** @brief Iterator for the list of FileResources writers */ 
	private ListIterator<IFileResourceTransmissionWriter> uploadIterator;
	/** @brief Current FileResources writers being processed */ 
	private IFileResourceTransmissionWriter               currentFileResource;
	/** @brief Flag to determine if the engine is currently processing */ 
	private boolean                                       bProcessing;
	/** @brief Flag to determine if the engine latest processing outcome */ 
	private boolean                                       bSuccesful;
	/** @brief Last reported Timestamp update */ 
	private long                                          lLastTimestampUpdate;
	
	/** @brief Engine MGP/EDT3 event listener also used as Monitor for synchronization */ 
	private SPOTFileUploadMgpResultEdtEventActionListener messageEventProcessor;

	/** The socket disconnect event processor. */
	private EdtClientSocketDisconnectAction	socketDisconnectEventProcessor;

	
	/** @brief Default constructor.
	 * 
	 * @since 1.0.0
	 */	
	public FileResourceCollectionEngine () throws EdtServiceException
	{
		super();
		this.resetCaptions();
		this.uploadVector = null;
		this.uploadIterator = null;
		this.currentFileResource = null;
		this.bProcessing = false;
		this.bSuccesful = false;
		this.lLastTimestampUpdate = 0;
		this.messageEventProcessor = null;
		this.clear();
		this.subscribeToEvents();
	}

	/** @brief Edt3 topic event subscriber.
	 * 
	 * @throws EdtServiceException if there is an error while subscribing to event topics.
	 * 
	 * @see EdtServiceException
	 * 
	 * @since 1.0.0
	 */	
	protected void subscribeToEvents () throws EdtServiceException
	{
		if (messageEventProcessor == null)
		{
			messageEventProcessor = new SPOTFileUploadMgpResultEdtEventActionListener();
			SERVICE_MANAGER.subscribe(messageEventProcessor);
		}

		if (socketDisconnectEventProcessor == null)
		{
			socketDisconnectEventProcessor = new EdtClientSocketDisconnectAction();
			SERVICE_MANAGER.subscribe( socketDisconnectEventProcessor );
		}
		
		

	}

	/** @brief Reset internal reporting captions for the engine.
	 * 
	 * @since 1.0.0
	 */	
	protected void resetCaptions ()
	{
		captionFileResource = DEFAULT_CAPTION_FILERESOURCE;
		captionSucessful = DEFAULT_CAPTION_SUCCESSFUL;
		captionFail = DEFAULT_CAPTION_FAIL;
	}

	/** @brief Set internal reporting captions for the engine.
	 * 
	 * @param Caption published for the FileResource type name.
	 * @param Caption published if collection was successful.
	 * @param Caption published if collection had failed.
	 * 
	 * @since 1.0.0
	 */	
	protected void setCaptions (String sCaptionFileResource, String sCaptionSucessful, String sCaptionFail)
	{
		if ((sCaptionFileResource != null) && (sCaptionFileResource.length() > 0))
		{
			captionFileResource = sCaptionFileResource;
		}
		if ((sCaptionSucessful != null) && (sCaptionSucessful.length() > 0))
		{
			captionSucessful = sCaptionSucessful;
		}
		if ((sCaptionFail != null) && (sCaptionFail.length() > 0))
		{
			captionFail = sCaptionFail;
		}
	}
	
	/** @brief Add a single FileResource writer to the engine writers list.
	 * 
	 * @param FileResource writer to be added to the engine writers list.
	 * 
	 * @see IFileResourceTransmissionWriter
	 * 
	 * @since 1.0.0
	 */	
	protected void add (IFileResourceTransmissionWriter fileResource)
	{
		if (uploadVector == null)
		{
			uploadVector = new Vector<IFileResourceTransmissionWriter>();
			uploadIterator = uploadVector.listIterator();
		}
		uploadVector.add(fileResource);
		uploadIterator = uploadVector.listIterator(uploadIterator.nextIndex());
	}

	/** @brief Add a collection of FileResource writers to the engine writers list.
	 * 
	 * @param FileResource writer collection to be added to the engine writers list.
	 * 
	 * @see IFileResourceTransmissionWriter
	 * 
	 * @since 1.0.0
	 */	
	protected void addAll (Collection<IFileResourceTransmissionWriter> fileResourceVector)
	{
		if (uploadVector == null)
		{
			uploadVector = new Vector<IFileResourceTransmissionWriter>();
			uploadIterator = uploadVector.listIterator();
		}
		uploadVector.addAll(fileResourceVector);
		uploadIterator = uploadVector.listIterator(uploadIterator.nextIndex());
	}

	/** @brief Clears the engine writers list.
	 * 
	 * @since 1.0.0
	 */	
	protected void clear ()
	{
		if (uploadVector == null)
		{
			uploadVector = new Vector<IFileResourceTransmissionWriter>();
			uploadIterator = uploadVector.listIterator();
		}
		else
		{
			// add each file and its history file option.
			uploadVector.clear();
			uploadIterator = uploadVector.listIterator();
		}
	}

	/** @brief Notifies to the engine observers the error status update on a single FileResource transmission.
	 * 
	 * @param Caption to be updated along with this status change.
	 * @param FileResource transmission status.
	 * 
	 * @see IFileResourceTransmission.FileResourceTransmissionStatus
	 * 
	 * @since 1.0.0
	 */	
	private void notifyFileResourceTransmissionError (String sCaptionUpdate, IFileResourceTransmission.FileResourceTransmissionStatus status)
	{
		currentFileResource.setTransferStatus(status);
		IFileResourceTransmissionStorage fileResourceTransmitted = currentFileResource.getFileResourceTransmited();
		currentFileResource = null;
		logger.error(sCaptionUpdate);
		this.setChanged();
		this.notifyObservers(new EngineProgressNotificationFileResourceTransmission(sCaptionUpdate,fileResourceTransmitted));
	}

	/** @brief Notifies to the engine observers the warning status update on a single FileResource transmission.
	 * 
	 * @param Caption to be updated along with this status change.
	 * @param FileResource transmission status.
	 * 
	 * @see IFileResourceTransmission.FileResourceTransmissionStatus
	 * 
	 * @since 1.0.0
	 */	
	private void notifyFileResourceTransmissionWarning (String sCaptionUpdate, IFileResourceTransmission.FileResourceTransmissionStatus status)
	{
		currentFileResource.setTransferStatus(status);
		IFileResourceTransmissionStorage fileResourceTransmitted = currentFileResource.getFileResourceTransmited();
		currentFileResource = null;
		logger.warn(sCaptionUpdate);
		this.setChanged();
		this.notifyObservers(new EngineProgressNotificationFileResourceTransmission(sCaptionUpdate,fileResourceTransmitted));
	}

	/** @brief Notifies to the engine observers the info status update on a single FileResource transmission.
	 * 
	 * @param Caption to be updated along with this status change.
	 * @param FileResource transmission status.
	 * 
	 * @see IFileResourceTransmission.FileResourceTransmissionStatus
	 * 
	 * @since 1.0.0
	 */	
	private void notifyFileResourceTransmissionInfo (String sCaptionUpdate, IFileResourceTransmission.FileResourceTransmissionStatus status)
	{
		currentFileResource.setTransferStatus(status);
		IFileResourceTransmissionStorage fileResourceTransmitted = currentFileResource.getFileResourceTransmited();
		currentFileResource = null;
		logger.info(sCaptionUpdate);
		this.setChanged();
		this.notifyObservers(new EngineProgressNotificationFileResourceTransmission(sCaptionUpdate,fileResourceTransmitted));
	}
	
	/** @brief Checks if there are more FileResources to collect and proceeds accordingly.
	 * 
	 * @since 1.0.0
	 */	
	private void checkMoreFilesToUpload ()
	{
		if (uploadIterator.hasNext())
		{
			currentFileResource = uploadIterator.next();

			currentFileResource.setTransferStatus(IFileResourceTransmission.FileResourceTransmissionStatus.IN_PROGRESS);
			
			String fileResourceInfo = "'" + currentFileResource.getName() + "'";

			logger.info("Requesting " + captionFileResource + ": " + fileResourceInfo + "...");

			this.setChanged();
			this.notifyObservers(new EngineProgressNotification("requesting " + captionFileResource + ": " + fileResourceInfo + "..."));

			sendFileUploadRequest();
		}
		else
		{
			logger.info("No more " + captionFileResource + " to collect.");
			processSuccess();
		}
	}

	/** @brief Sends a SPOT FileUploadRequest.
	 * 
	 * @since 1.0.0
	 */	
	private void sendFileUploadRequest ()
	{
		byte[] message = SpotMessages.getInstance().getFileUploadRequestData(currentFileResource.getFileType(),currentFileResource.getFileId());
		try
		{
			ConnectionEngineSwingAdapter.getInstance().send(message);
		}
		catch (NullPointerException e)
		{
			logger.error("Error. Exception thrown while sending synchronouse data to target unit.\nEdtServiceException: '" + e.getMessage() + "'.");
		}
	}

	/** @brief Sends a SPOT FileBlockUploadRequest.
	 * 
	 * @param List of fields and their values to be used for this request. 
	 * 
	 * @since 1.0.0
	 */	
	private void sendFileBlockUploadRequest (Map<String, Object> data)
	{
		int offset = Integer.decode((String)data.get("Offset"));
		byte[] message = SpotMessages.getInstance().getFileBlockUploadData(offset);
		try
		{
			ConnectionEngineSwingAdapter.getInstance().send(message);
		}
		catch (NullPointerException e)
		{
			logger.error("Error. Exception thrown while sending synchronouse data to target unit.\nEdtServiceException: '" + e.getMessage() + "'.");
		}
	}


	/** @brief Verifies if there are more blocks to request for the current FileResource.
	 * 
	 * @param List of fields and their values to be used for this verification.
	 * 
	 * @return True if there are more blocks to request for the current FileResource. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	private boolean blocksToRequest (Map<String, Object> data)
	{
		String fileSz = (String) data.get("FileSz");
		int size = Integer.decode(fileSz);
		logger.info("Filsize = " + fileSz);
		return size > 0;
	}
	
	/** @brief Process a SPOT FileUploadResponse.
	 * 
	 * @param List of fields and their values to be processed from this response. 
	 * 
	 * @since 1.0.0
	 */	
	private void getFileUploadResponse (Map<String, Object> data)
	{
		byte ack = (byte) data.get("Ack");
		String fileResourceInfo = "'" + currentFileResource.getName() + "'";
		
		if (ack == SpotMessageErrorCode.NO_ERROR.getCode())
		{
			if (blocksToRequest(data))
			{
				if (currentFileResource.open())
				{
					Map<String, Object> newData = new HashMap<String, Object>();
					newData.put("Offset","0"); //Request the first block
					sendFileBlockUploadRequest(newData);
				}
				else
				{
					notifyFileResourceTransmissionError("Error. Could not open " + fileResourceInfo + " stream. Proceeding with next " + captionFileResource + "...",IFileResourceTransmission.FileResourceTransmissionStatus.ERROR);
					checkMoreFilesToUpload();
				}
			}
			else
			{
				notifyFileResourceTransmissionWarning("Warning. No blocks to request for " + fileResourceInfo + ". Proceeding with next " + captionFileResource + "...",IFileResourceTransmission.FileResourceTransmissionStatus.NOT_AVAILABLE);
				checkMoreFilesToUpload();
			}
		}
		else if (ack == SpotMessageErrorCode.CANNOT_READ_RESOURCE.getCode()) //File not found at target unit
		{
			notifyFileResourceTransmissionWarning("Warning. Not available " + fileResourceInfo + ". Proceeding with next " + captionFileResource + "...",IFileResourceTransmission.FileResourceTransmissionStatus.NOT_AVAILABLE);
			checkMoreFilesToUpload();
		}
		else
		{
			String sNAck = " ";
			try
			{
				sNAck = String.format("0x%02X",(ack & 0xFF));
			}
			catch (Exception e)
			{
				logger.error("Error while formatting NAck retrieved value. " + e.getMessage(),e);
			}
			notifyFileResourceTransmissionError("Error. Returned [" + sNAck + "] in response message for " + fileResourceInfo + ".",IFileResourceTransmission.FileResourceTransmissionStatus.ERROR);

			//TODO: Check difference with same condition in getFileBlockUploadResponse()
			logger.error("Collection Interrupted...");
			processError("Collection Interrupted.");
		}
	}

	/** @brief Process a SPOT FileBlockUploadResponse.
	 * 
	 * @param List of fields and their values to be processed from this response. 
	 * 
	 * @since 1.0.0
	 */	
	private void getFileBlockUploadResponse (Map<String, Object> data)
	{
		byte ack = (byte) data.get("Ack");
		String fileResourceInfo = "'" + currentFileResource.getName() + "'";

		if (ack == SpotMessageErrorCode.DOWNLOAD_COMPLETED.getCode()) //File complete
		{
			if (currentFileResource.write((byte[])data.get("BlkData")))
			{
				if (currentFileResource.close())
				{
					notifyFileResourceTransmissionInfo("Retrieved " + fileResourceInfo + ".",IFileResourceTransmission.FileResourceTransmissionStatus.DONE);
				}
				else
				{
					notifyFileResourceTransmissionWarning("Warning. Could not close " + fileResourceInfo + " stream. Proceeding with next " + captionFileResource + "...",IFileResourceTransmission.FileResourceTransmissionStatus.NOT_AVAILABLE);
				}
			}
			else
			{
				notifyFileResourceTransmissionError("Error. Could not append data to " + fileResourceInfo + " stream. Proceeding with next " + captionFileResource + "...",IFileResourceTransmission.FileResourceTransmissionStatus.ERROR);
			}
			checkMoreFilesToUpload();
		}
		else if (ack == SpotMessageErrorCode.NO_ERROR.getCode()) //Received Ok and has more blocks to request
		{
			if (currentFileResource.write((byte[])data.get("BlkData")))
			{
				Map<String, Object> newData = new HashMap<String,Object>();
				newData.put("Offset",(String)data.get("Offset"));
				sendFileBlockUploadRequest(newData);
			}
			else
			{
				notifyFileResourceTransmissionError("Error. Could not append data to " + fileResourceInfo + " stream. Proceeding with next " + captionFileResource + "...",IFileResourceTransmission.FileResourceTransmissionStatus.ERROR);
				checkMoreFilesToUpload();
			}
		}
		else
		{
			String sNAck = " ";
			try
			{
				sNAck = String.format("0x%02X",(ack & 0xFF));
			}
			catch (Exception e)
			{
				logger.error("Error while formatting NAck retrieved value. " + e.getMessage(),e);
			}
			notifyFileResourceTransmissionError("Error. Returned [" + sNAck + "] in response message for " + fileResourceInfo + ".",IFileResourceTransmission.FileResourceTransmissionStatus.ERROR);

			//TODO: Check difference with same condition in getFileUploadResponse()
			logger.error("Proceeding with next " + captionFileResource + "...");
			checkMoreFilesToUpload();
		}
	}

	/** @brief Report FileResource list process completion successfully.
	 * 
	 * @since 1.0.0
	 */	
	private void processSuccess ()
	{
		String sCaptionUpdate = captionSucessful;
		if (checkSwiftEnd())
		{
			bSuccesful = true;
			logger.info(captionFileResource + " Collection Successful.");
			this.setChanged();
			this.notifyObservers(new EngineProgressNotificationStatusSucceeded(sCaptionUpdate));
			cleanUpProcess();
		}
		else
		{
			processError("Inconsistent internal state");
		}
	}

	/** @brief Report FileResource list process completion with errors.
	 * 
	 * @since 1.0.0
	 */	
	private void processError (String sCaption)
	{
		String sCaptionUpdate = captionFail;
		if (sCaption.length() > 0)
		{
			sCaptionUpdate = sCaptionUpdate + " - " + sCaption + " - Log Generated";
		}

		checkSwiftEnd();
		this.setChanged();
		this.notifyObservers(new EngineProgressNotificationStatusFailed(sCaptionUpdate));
		cleanUpProcess();
	}
	
	/** @brief Verify there were no pending issues at the end of the process. Report them otherwise as errors.
	 *
	 * @return True if the process ended swiftly. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	private boolean checkSwiftEnd ()
	{
		boolean bResult = false;
		if (currentFileResource != null)
		{
			String fileResourceInfo = "'" + currentFileResource.getName() + "'";
			if (currentFileResource.close())
			{
				notifyFileResourceTransmissionError("Error. Interrupted transmission for " + fileResourceInfo + ". Process interrupted.",IFileResourceTransmission.FileResourceTransmissionStatus.ERROR);
			}
			else
			{
				notifyFileResourceTransmissionError("Error. Interrupted transmission and could not close " + fileResourceInfo + " stream. Process interrupted.",IFileResourceTransmission.FileResourceTransmissionStatus.ERROR);
			}
		}
		else
		{
			bResult = true;
		}
		return bResult;
	}	
	
	/** @brief Clean all engine internal fields.
	 * 
	 * @since 1.0.0
	 */	
	private void cleanUpProcess ()
	{
		resetCaptions ();
		
		currentFileResource = null;

		clear();

		bProcessing = false;
		messageEventProcessor.notifyAll();
	}

	/** @brief Trigger engine collection of resources. This is a blocking call.
	 * 
	 * @param List of file resources writers to be used by this engine.
	 * @param List of engine progress observers to be notified upon updates.
	 * 
	 * @return True if engine process outcome was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	public boolean collectFileResources (Collection<IFileResourceTransmissionWriter> fileResourceVector, Collection<Observer> observers)
	{
		return collectFileResources(fileResourceVector,observers,null,null,null);
	}

	/** @brief Trigger engine collection of resources. This is a blocking call.
	 * 
	 * @param List of file resources writers to be used by this engine.
	 * @param List of engine progress observers to be notified upon updates.
	 * @param Caption published for the FileResource type name.
	 * @param Caption published if collection was successful.
	 * @param Caption published if collection had failed.
	 * 
	 * @return True if engine process outcome was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	public boolean collectFileResources (Collection<IFileResourceTransmissionWriter> fileResourceVector, Collection<Observer> observers, String sCaptionFileResource, String sCaptionSucessful, String sCaptionFail)
	{
		boolean bResult = false;

		synchronized (messageEventProcessor)
		{
			if (!bProcessing)
			{
				bProcessing = true;
				bSuccesful = false;

				setCaptions(sCaptionFileResource,sCaptionSucessful,sCaptionFail);

				clear();
				addAll(fileResourceVector);
				
				if (observers != null)
				{
    				for (Observer observer : observers)
    				{
    					this.deleteObserver(observer);
    					this.addObserver(observer);
    				}
				}

				logger.info("FileResources Collection Initiated");
				this.setChanged();
				this.notifyObservers(new EngineProgressNotificationStatusInitiated(captionFileResource + " Collection Initiated."));
				
				checkMoreFilesToUpload();
		
				try
				{
					boolean bTimeoutOrDisconnect = false;
					long lTimeInterval = DEFAULT_TIMEOUT_MILLISECONDS;
					lLastTimestampUpdate = Calendar.getInstance().getTimeInMillis();
					while (bProcessing)
					{
						messageEventProcessor.wait(lTimeInterval);
						
						if (bProcessing)
						{
							lTimeInterval = (Calendar.getInstance().getTimeInMillis() - lLastTimestampUpdate);
							if (lTimeInterval >= DEFAULT_TIMEOUT_MILLISECONDS)
							{
								bProcessing = false;
								bTimeoutOrDisconnect = true;
							}
							else
							{
								lTimeInterval = DEFAULT_TIMEOUT_MILLISECONDS - lTimeInterval;
							}
						}
					}

					if (bTimeoutOrDisconnect)
					{
						processError("Connection error"); //DISCONNECT or TIMEOUT
					}
					
					bResult = bSuccesful;
				}
				catch (InterruptedException e)
				{
					logger.error("Error. Interrupted while processing " + captionFileResource + " collection messages: " + e.getMessage(), e);
					this.setChanged();
					this.notifyObservers(new EngineProgressNotification("Error. Interrupted while processing " + captionFileResource + " collection messages: " + e.getMessage()));
					processError("Interrupted exception");
				}

				this.deleteObservers();
			}
			else
			{
				logger.error("Error. Collection engine already processing.");
			}
		}
		return bResult;
	}

	/** @brief Abort current collection and publish an error message.
	 * 
	 * @param Extended caption to add as an abort explanation
	 * @param Observers to be included to observers list for this abort
	 * 
	 * @since 1.0.0
	 */	
	public void abortCollection (String sCaption, Collection<Observer> observers)
	{
		abortCollection(sCaption,observers,null,null);
	}

	/** @brief Abort current collection and publish an error message.
	 * 
	 * @param Extended caption to add as an abort explanation
	 * @param Observers to be included to observers list for this abort
	 * @param Update the caption published for the FileResource type name.
	 * @param Update the current fail caption for the notification
	 * 
	 * @since 1.0.0
	 */	
	public void abortCollection (String sCaption, Collection<Observer> observers, String sCaptionFileResource, String sCaptionFail)
	{
		synchronized (messageEventProcessor)
		{
			setCaptions(sCaptionFileResource,null,sCaptionFail);

			if (observers != null)
			{
    			for (Observer observer : observers)
    			{
    				this.deleteObserver(observer);
    				this.addObserver(observer);
    			}
			}
			processError(sCaption);
			this.deleteObservers();
		}
	}
	
	/** @brief Check if engine is currently working.
	 * 
	 * @return True if engine is currently working. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	public boolean isInProgress ()
	{
		boolean bResult = false;
		synchronized (messageEventProcessor)
		{
			bResult = bProcessing;
		}
		return bResult;
	}
	
	/** @brief Check if engine last process outcome was successful.
	 * 
	 * @return True if engine last process outcome was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	public boolean wasSuccessful ()
	{
		boolean bResult = false;
		synchronized (messageEventProcessor)
		{
			bResult = bSuccesful;
		}
		return bResult;
	}
	
	/**
	 * Wakeup.
	 */
	protected synchronized void wakeup()
	{
		this.notify( );
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
			synchronized (messageEventProcessor)
			{
				if (bProcessing)
				{
					if (eventtype.equals( SocketEventType.SOCKET_DISCONNECT ))
					{
						logger.debug( "SOCKET_DISCONNECT" );
						processError("Connection Error");
					}
				}
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
