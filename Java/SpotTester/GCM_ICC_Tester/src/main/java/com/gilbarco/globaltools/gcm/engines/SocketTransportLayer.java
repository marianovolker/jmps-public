package com.gilbarco.globaltools.gcm.engines;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketAdapter;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.spottester.model.SpotServiceDefinitionsFactory;

public class SocketTransportLayer implements DataTransportLayer {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog(SocketTransportLayer.class);
	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected EdtClientSocketAdapter	socketAdapter;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SocketTransportLayer() {
		super();
		try {
			socketAdapter = (EdtClientSocketAdapter) EdtServiceContext.getServiceManager().getServiceAdapter(
				SpotServiceDefinitionsFactory.getSocketDefinition());
		}
		catch (EdtServiceException e) {
			logger.error(e.getMessage(), e);
		}
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public void send(byte[] data) {
		try {
			this.invokeMessage(data);
		}
		catch (EdtServiceException e) {
			e.printStackTrace();
		}
	}

	/**
	 * @Description Send a message and invoke and outgoing message event.
	 * @param data
	 *            Data to be sent.
	 * @throws EdtServiceException.
	 * */
	public void invokeMessage(byte[] data) throws EdtServiceException {
		socketAdapter.invokeSendData(data);
	}

	/**
	 * @Description Send a message and invoke and outgoing message event.
	 * @param event
	 *            EdtClientSocketEvent with the outgoing data.
	 * @throws EdtServiceException
	 * */
	public void invokeMessage(EdtClientSocketEvent event) throws EdtServiceException {
		socketAdapter.invoke(event);
	}

	/**
	 * @Description Send a message.
	 * @param data
	 *            Data to be sent.
	 * @throws EdtServiceException.
	 * */
	public void sendMessage(byte[] data) throws EdtServiceException {
		socketAdapter.callSendData(data);
	}

	/**
	 * @Description Send a message.
	 * @param event
	 *            EdtClientSocketEvent with the outgoing data.
	 * @throws EdtServiceException
	 * */
	public void sendMessage(EdtClientSocketEvent event) throws EdtServiceException {
		socketAdapter.call(event);
	}
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
