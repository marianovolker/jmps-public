package com.gilbarco.globaltools.gcm.engines;

import static com.gilbarco.globaltools.gcm.model.GCMContext.PROCESS_ADAPTER;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Queue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.gcm.model.GCMContext;
import com.gilbarco.globaltools.gcm.state.GCMState;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

public class OTICommandEngine {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger		= LogFactory.getLog(OTICommandEngine.class);
	private static final byte[]			DELAY_TAG	= new byte[] { 0, 0 };
	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected Queue<byte[]>				commands	= new LinkedList<byte[]>();

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private boolean						persistent;
	private DataTransportLayer			dataSender;
	private boolean                  processValid;
	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public OTICommandEngine(DataTransportLayer dataTransportLayer, boolean persistent) throws EdtServiceException {
		this.persistent = persistent;
		this.dataSender = dataTransportLayer;
		this.subscribeToEvents();
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public void setCommands(File file) {
		FileReader is = null;
		BufferedReader br = null;
		try {
			is = new FileReader(file);
			br = new BufferedReader(is);
			String cmd = br.readLine();
			while (cmd != null) {
				commands.add(this.getCommand(cmd));
				cmd = br.readLine();
			}
		}
		catch (Exception e) {
			logger.error(e.getMessage(), e);
		}
		finally {
			if (is != null)
				try {
					is.close();
				}
				catch (IOException e) {
					logger.error(e.getMessage(), e);
				}
			if (br != null)
				try {
					br.close();
				}
				catch (IOException e) {
					logger.error(e.getMessage(), e);
				}
		}
	}

	public void startConfigProcess(boolean persistent) {
		System.out.println("Starting oti process");
		this.processValid = true;
		this.persistent = persistent;
		this.sendNextCommand();
	}

	public void endConfigProcess(boolean ok, String mssg) {
	   this.processValid = false;
	   if (ok)
			this.changeState(GCMState.ENABLE_READER.name(), mssg + " " + GCMContext.getLocaleText("EnableingReader"));
		else
			this.changeState(GCMState.LOG_RESULT.name(), mssg);
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected byte[] getNextCommand() {
		return this.commands.poll();
	}

	protected boolean hasNextCommand() {
		return !this.commands.isEmpty();
	}

	protected void sendNextCommand() {
	   if(!this.processValid){
	        return;
	   }
		if (this.hasNextCommand()) {
			byte[] data = this.getNextCommand();
			byte[] tag = Arrays.copyOfRange(data, 0, 2);
			byte[] value = Arrays.copyOfRange(data, 2, data.length);

			boolean send = true;
			if (Arrays.equals(tag, DELAY_TAG)) {
				send = false;
				try {
					Thread.sleep(EdtConvert.bytesToLong(true, value, 0));
				}
				catch (InterruptedException e) {

				}
			}

			if (send) {
				if (this.persistent)
					this.dataSender.send(SpotMessages.getInstance().getOTISetData(tag, value));
				else
					this.dataSender.send(SpotMessages.getInstance().getOTIDoData(tag, value));
			}
			this.sendNextCommand();
		}
		else {
			if (this.persistent) {
				this.dataSender.send(SpotMessages.getInstance().getOTIDoData(new byte[] { (byte) 0xDF, (byte) 0x04 },
					new byte[] {}));
			}

			this.endConfigProcess(true, GCMContext.getLocaleText("configOk"));
		}
	}

	protected void subscribeToEvents() {
		try {
			EdtServiceContext.getServiceManager().subscribe(new EdtEventAction<MgpResultEvent>() {
				@Override
				public void onInvokeEvent(MgpResultEvent event) {
					if (!event.isValid()) {
						endConfigProcess(false, event.getMessageName() + GCMContext.getLocaleText("cmdFailed"));
						return;
					}
				}

				@Override
				public Class<MgpResultEvent> getEventClass() {
					return MgpResultEvent.class;
				}

				@Override
				public String getSubscriptionFilter() {
					return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name() + "' AND "
							+ MgpResultEvent.MESSAGE_NAME + "IN ('" + SpotMessageType.OTI_DO.getName() + "', '"
							+ SpotMessageType.OTI_SET + "')";
				}

				@Override
				public boolean justOnce() {
					return false;
				}
			});
		}
		catch (EdtServiceException e) {
			logger.error(e.getMessage(), e);
		}
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void changeState(String to, String description) {
		try {
			if (PROCESS_ADAPTER.getCurrentState().equals(GCMState.INIT.name()))
				PROCESS_ADAPTER.callChangeToState(PROCESS_ADAPTER.getCurrentState(), to, new Issuer(this.getClass()),
					description);
			else {
				// what happens here?
			}
		}
		catch (EdtServiceException e) {
			logger.error(e.getMessage(), e);
		}
	}

	private byte[] getCommand(String cmd) {
		String[] splited = cmd.split("=");
		String tag = splited[0].trim().split("\\.")[1];
		String val = splited[1].trim();
		return EdtConvert.hexStringToBytes(tag + val);
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************
}