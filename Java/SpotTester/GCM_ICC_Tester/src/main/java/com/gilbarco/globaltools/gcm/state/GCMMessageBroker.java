package com.gilbarco.globaltools.gcm.state;

import static com.gilbarco.globaltools.gcm.model.GCMContext.SERVICE_MANAGER;

import java.io.File;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent.ProcessEventType;
import com.gilbarco.globaltools.gcm.engines.DataTransportLayer;
import com.gilbarco.globaltools.gcm.engines.FileDownloadEngine;
import com.gilbarco.globaltools.gcm.engines.OTICommandEngine;
import com.gilbarco.globaltools.gcm.gui.GCMICCModel;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

public class GCMMessageBroker {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
//	private static final transient Log	logger	= LogFactory.getLog(GCMMessageBroker.class);

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
	private DataTransportLayer			dataSender;
	private OTICommandEngine			otiEngine;
	private FileDownloadEngine			fileEngine;
	private GCMICCModel					model;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public GCMMessageBroker(DataTransportLayer dataTransportLayer, GCMICCModel model) throws EdtServiceException {
		super();
		this.dataSender = dataTransportLayer;
		this.otiEngine = new OTICommandEngine(dataTransportLayer, model.isPersistentConfig());
		this.fileEngine = new FileDownloadEngine(dataTransportLayer);
		this.model = model;
		
		this.subscribeToEvents();
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public void sendOTICommands(File cmdFile) {
		this.sendOTICommands(cmdFile, false);
	}

	public void sendOTICommands(File cmdFile, boolean persistCmnd) {
		if (cmdFile == null) {
			this.otiEngine.endConfigProcess(true, "No Configuration process");
			return;
		}

		this.otiEngine.setCommands(cmdFile);
		this.otiEngine.startConfigProcess(persistCmnd);
	}

	public void sendEnableCardReader() {
		this.dataSender.send(SpotMessages.getInstance().getEnableCLReader(new byte[] { 0x00, 0x00 },
			new byte[] { 0x00, 0x00, 0x00, 0x00, 0x12, 0x34 }, new byte[] { 0x10, 0x01, 0x11 }, new byte[] { 0x00 },
				new byte[] { 0x00, 0x00, 0x00 }));
	}

	public void sendDisableCardReader() {
		this.dataSender.send(SpotMessages.getInstance().getOTIDisableCLReader());
	}

	// this method is used with reflection from com.gilbarco.globaltools.gcm.gui.GCMICCController.addMenuItems(JMenu) 
	public void sendEMVTableFile(File emvTableFile) {
		this.fileEngine.setCurrentFile(emvTableFile);
		this.fileEngine.startFileDownload();
	}

	public void sendCreateWindows(String showText, byte winId) {
		this.dataSender.send(SpotMessages.getInstance().getSpotScreenWithTextData(showText, winId));
	}

	public void sendShowScreen(byte id) {
		this.dataSender.send(SpotMessages.getInstance().getShowScreen(id));
	}

	public void sendHideScreen(byte id) {
		this.dataSender.send(SpotMessages.getInstance().getHideScreen(id));
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void subscribeToEvents() throws EdtServiceException {
		SERVICE_MANAGER.subscribe(new StateChangedAction());
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class StateChangedAction implements EdtEventAction<EdtProcessEvent> {
		@Override
		public void onInvokeEvent(EdtProcessEvent event) {
			GCMState destination = GCMState.valueOf(event.getDestination());
			switch(destination){
				case IDLE :
					break;
				case INIT :
					sendOTICommands(model.getOtiCmdFile(), model.isPersistentConfig());
					break;
				case ENABLE_READER :
					sendEnableCardReader();
					break;
				case WAIT_DATA :
					break;
				case LOG_RESULT :
					sendDisableCardReader();
					break;
			}
		}

		@Override
		public Class<EdtProcessEvent> getEventClass() {
			return EdtProcessEvent.class;
		}

		@Override
		public String getSubscriptionFilter() {
			return EdtEvent.EVENT_TYPE_FILTER + "='" + ProcessEventType.STATE_CHANGED.name() + "'";
		}

		@Override
		public boolean justOnce() {
			return false;
		}
	}
	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
