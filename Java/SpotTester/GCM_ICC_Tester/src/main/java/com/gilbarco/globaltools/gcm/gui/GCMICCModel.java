package com.gilbarco.globaltools.gcm.gui;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Date;
import java.util.Map;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.gui.app.EdtAppModel;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent.ProcessEventType;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.gcm.model.GCMContext;
import com.gilbarco.globaltools.gcm.state.GCMMessageBroker;
import com.gilbarco.globaltools.gcm.state.GCMState;
import com.gilbarco.globaltools.gcm.util.OTIParser;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;

public class GCMICCModel extends EdtAppModel {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	// private static final transient Log logger = LogFactory.getLog(GCMICCModel.class);

	private static final String	LOG_NAME			= "GCM_TEST_";
	private static final String	LOG_EXTENSION		= ".log";
	private static final String	INI_EXTENSION		= ".ini";
	private static final String	OTI_TAG_FILE		= "OTITags.conf";
	private static final int	MAX_TRACK_NUMBER	= 5;
	private static final int	TRACK1_NUMBER		= 1;
	private static final int	TRACK2_NUMBER		= 2;
	private static final int	TRACK3_NUMBER		= 4;
	private static final int	OTI_TAG_NUMBER		= 20;

	private static final byte	WAIT_DATA_WIN_ID	= 1;
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
	private String				testId;
	private String				iccLogFilePath;
	private String				otiCmdFilePath;
	private boolean				persistentConfig;
	private String				gcmLogPath;
	private String				notes;
	private String				status;
	private String				track1;
	private String				track2;
	private String				track3;

	private boolean				logged;
	private boolean				started;
	private GCMMessageBroker	messageBroker;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public GCMICCModel() {
		this.logged = false;
		this.started = false;
		try {
			EdtServiceContext.getServiceManager().subscribe(new UpdateGuiAction());
			EdtServiceContext.getServiceManager().subscribe(new ProcessMover());
		}
		catch (EdtServiceException e) {
			e.printStackTrace();
		}
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public String getIccLogFilePath() {
		return iccLogFilePath;
	}

	public void setIccLogFilePath(String logFilePath) {
		this.iccLogFilePath = logFilePath;
	}

	public String getOtiCmdFilePath() {
		return otiCmdFilePath;
	}

	public void setOtiCmdFilePath(String otiCmdFilePath) {
		this.otiCmdFilePath = otiCmdFilePath;
	}

	public String getNotes() {
		return notes;
	}

	public void setNotes(String notes) {
		this.notes = notes;
	}

	public String getStatus() {
		return status;
	}

	public void setStatus(String status) {
		this.status = status;
	}

	public String getTestId() {
		return testId;
	}

	public void setTestId(String testId) {
		this.testId = testId;
	}

	public String getTrack1() {
		return track1;
	}

	public void setTrack1(String track1) {
		this.track1 = track1;
	}

	public String getTrack2() {
		return track2;
	}

	public void setTrack2(String track2) {
		this.track2 = track2;
	}

	public String getTrack3() {
		return track3;
	}

	public void setTrack3(String track3) {
		this.track3 = track3;
	}

	public boolean isLogged() {
		return logged;
	}

	public void setLogged(boolean logged) {
		this.logged = logged;
	}

	public boolean isStarted() {
		return started;
	}

	public void setStarted(boolean started) {
		this.started = started;
	}

	public String getGcmLogPath() {
		return gcmLogPath;
	}

	public void setGcmLogPath(String gcmLogPath) {
		this.gcmLogPath = gcmLogPath;
	}

	public GCMMessageBroker getMessageBroker() {
		return messageBroker;
	}

	public void setMessageBroker(GCMMessageBroker messageBroker) {
		this.messageBroker = messageBroker;
	}

	public File getGcmLogFile() {
		return this.getGcmFile(LOG_EXTENSION);
	}

	public File getGcmIniFile() {
		return this.getGcmFile(INI_EXTENSION);
	}

	public File getOtiCmdFile() {
		if (!EdtStringUtil.isNullOrEmpty(this.otiCmdFilePath))
			return new File(this.otiCmdFilePath);

		return null;
	}

	public boolean isPersistentConfig() {
		return persistentConfig;
	}

	public void setPersistentConfig(boolean persistentConfig) {
		this.persistentConfig = persistentConfig;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("Test ID: " + this.testId + "\n");
		sb.append("ICC Test File: " + this.iccLogFilePath + "\n");
		sb.append("OTI Commands: " + this.otiCmdFilePath + "\n");
		sb.append("OTI Config Persistence: " + this.persistentConfig + "\n");
		sb.append("Notes: " + this.notes + "\n");
		sb.append("Results :" + "\n");
		if (!this.status.equals(""))
			sb.append("\tStatus  : " + this.status + "\n");
		if (!this.track1.equals(""))
			sb.append("\tTrack 1 : " + this.track1 + "\n");
		if (!this.track2.equals(""))
			sb.append("\tTrack 2 : " + this.track2 + "\n");
		if (!this.track3.equals(""))
			sb.append("\tTrack 3 : " + this.track3 + "\n");
		return sb.toString();
	}

	public void modelUpdated() {
		this.setChanged();
		this.notifyObservers();
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void clearResult() {
		this.track1 = "";
		this.track2 = "";
		this.track3 = "";
	}

	protected void updateStatus(String status) {
		this.status = status;
	}

	protected void updateResult(Map<String, Object> decodeMap) {
		String[] data;
		try {
			data = getCRData(decodeMap);
			this.status = "Passed";
			this.track1 = data[TRACK1_NUMBER] != null ? data[TRACK1_NUMBER] : "";
			this.track2 = data[TRACK2_NUMBER] != null ? data[TRACK2_NUMBER] : "";
			this.track3 = data[TRACK3_NUMBER] != null ? data[TRACK3_NUMBER] : "";
			parseOTIData(decodeMap);
		}
		catch (Exception e) {
			this.status = GCMContext.getLocaleText("CardDataMessageFail");
		}
	}

	public File getGcmFile(String fileExtension) {
		String path = this.getGcmLogPath();
		if (EdtStringUtil.isNullOrEmpty(path))
			path = System.getProperty("user.dir");

		if (!path.endsWith(System.getProperty("file.separator")))
			path += System.getProperty("file.separator");

		return new File(path + LOG_NAME + this.testId + fileExtension);

	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	@SuppressWarnings("unchecked")
	private String[] getCRData(Map<String, Object> decodeMap) throws Exception {
		String[] data = new String[MAX_TRACK_NUMBER];
		Map<String, Object> map = (Map<String, Object>) decodeMap.get(decodeMap.keySet().iterator().next());
		Map<String, Object> entries = null;
		for (String s : map.keySet()) {
			if (s.split("_")[0].equals("Entries")) {
				entries = (Map<String, Object>) map.get(s);
				for (String s1 : entries.keySet()) {
					int numero = EdtConvert.stringToInt(((String) ((Map<String, Object>) entries.get(s1)).get("Source"))
									.split("0x")[1]);
					String track = ((String) ((Map<String, Object>) entries.get(s1)).get("Data")).split("0x")[1];
					if (numero < MAX_TRACK_NUMBER)
						data[numero] = track;
				}
				return data;
			}
		}
		throw new Exception("decodeMap is badly formatted");
	}

   @SuppressWarnings("unchecked")
	private void parseOTIData(Map<String, Object> decodeMap) throws Exception {
		InputStreamReader isr = null;
		try {
      Map<String, Object> entries = getEntries(decodeMap);
			for (String s1 : entries.keySet()) {
				int num = EdtConvert.stringToInt(((String) ((Map<String, Object>) entries.get(s1)).get("Source"))
								.split("0x")[1]);
         String tag = ((String) ((Map<String, Object>) entries.get(s1)).get("Data")).split("0x")[1];
				if (num == OTI_TAG_NUMBER) {
            OTIParser parser = new OTIParser(getGcmIniFile());
					isr = new InputStreamReader(this.getClass().getClassLoader().getResource(OTI_TAG_FILE).openStream());
					parser.loadTags(isr);
            parser.parseAndDumpToFile(tag);
         }
      }
   }
		catch (Exception e) {
			e.printStackTrace();
			throw e;
		}
		finally {
			if(isr != null)
				isr.close();
		}
	}

	@SuppressWarnings("unchecked")
	private Map<String, Object> getEntries(Map<String, Object> decodeMap) {
		Map<String, Object> map = (Map<String, Object>) decodeMap.get(decodeMap.keySet().iterator().next());
		Map<String, Object> entries = null;
		for (String s : map.keySet()) {
			if (s.split("_")[0].equals("Entries")) {
				entries = (Map<String, Object>) map.get(s);
			}
		}
		return entries;
	}

	private void createLogFile(String description) {
		FileWriter fwriter = null;
		try {
			fwriter = new FileWriter(this.getGcmLogFile(), true);
			fwriter.write(GCMContext.getLocaleText("LogFileHeader") + "\n");
			fwriter.write("Date: " + new Date(System.currentTimeMillis()).toString() + "\n");
			fwriter.write(this.toString());
			fwriter.write("Description: " + description + "\n");
			fwriter.write(GCMContext.getLocaleText("LogFileFootPrint") + "\n");
		}
		catch (IOException e) {
			this.setStatus(GCMContext.getLocaleText("LogCreationFail"));
		}
		finally {
			try {
				if (fwriter != null)
					fwriter.close();
			}
			catch (IOException e) {
			}
		}
	}

	private void notifyEndTest(String source, String description) {
		try {
			GCMContext.PROCESS_ADAPTER.callChangeToState(source, GCMState.IDLE.name(), new Issuer(this.getClass()),
				description);
		}
		catch (EdtServiceException e) {
			e.printStackTrace();
		}
	}

	private void createWindows() {
		this.messageBroker.sendCreateWindows(GCMContext.getLocaleText("SwipeCard"), WAIT_DATA_WIN_ID);
		}

	public void sendShowScreen(byte id) {
		this.messageBroker.sendShowScreen(id);
		}

	public void sendHideScreen(byte id) {
		this.messageBroker.sendHideScreen(id);
		}

	public void getReady() {
		createWindows();
		updateStatus(GCMContext.getLocaleText("Ready"));
		}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class UpdateGuiAction implements EdtEventAction<EdtProcessEvent> {
		@Override
		public void onInvokeEvent(EdtProcessEvent event) {
			if (!started)
				return;

			GCMState destination = GCMState.valueOf(event.getDestination());
			switch(destination){
				case IDLE :
					setStarted(false);
					break;
				case INIT :
					clearResult();
					updateStatus(event.getDescription());
					break;
				case ENABLE_READER :
					updateStatus(event.getDescription());
					break;
				case WAIT_DATA :
					updateStatus(event.getDescription());
					sendShowScreen(WAIT_DATA_WIN_ID);
					break;
				case LOG_RESULT :
					updateStatus(event.getDescription());
					createLogFile(event.getDescription());
					notifyEndTest(event.getDestination(), event.getDescription());
					sendHideScreen(WAIT_DATA_WIN_ID);
					break;
			}

			modelUpdated();
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

	public class ProcessMover implements EdtEventAction<MgpResultEvent> {

		@Override
		public Class<MgpResultEvent> getEventClass() {
			return MgpResultEvent.class;
		}

		@Override
		public String getSubscriptionFilter() {
			return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name() + "' AND "
					+ MgpResultEvent.MESSAGE_NAME + " IN ('" + SpotMessageType.CL_DATA.getName() + "', '"
					+ SpotMessageType.LOGIN.getName() + "', '" + SpotMessageType.LOGOUT.getName() + "') ";
		}

		@Override
		public boolean justOnce() {
			return false;
		}

		@Override
		public void onInvokeEvent(MgpResultEvent event) {
			if (!event.isValid()) {
				updateStatus(event.getMessageName() + " " + GCMContext.getLocaleText("cmdFailed"));
				return;
			}

			SpotMessageType msg = SpotMessageType.getByName(event.getMessageName());
			switch(msg){
				case LOGIN :
					setLogged(true);
					getReady();
					break;
				case CL_DATA :
					if (started)
						updateResult(event.getDecodeMap());
					break;
				case LOGOUT :
					setLogged(false);
					break;
			}

			modelUpdated();
		}
	}
	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
