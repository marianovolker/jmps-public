package com.gilbarco.globaltools.spotprotocol.api;

import java.util.Arrays;
import java.util.List;

import com.gilbarco.globaltools.mgp.api.MessageFactory;
import com.gilbarco.globaltools.mgp.api.MessageType;
import com.gilbarco.globaltools.mgp.persistence.ProtocolProvider;

/**
 * MessageFactory.
 * 
 * @Description This factory can create a message and fill it.
 * */
public class SpotMessageFactory extends MessageFactory {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	// private static final transient Log logger = LogFactory.getLog(SpotMessageFactory.class);
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

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SpotMessageFactory(ProtocolProvider provider) {
		super(provider);
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public List<? extends MessageType> getMessageTypes() {
		return Arrays.asList(SpotMessageType.values());
	}

	@Override
	public byte[] getCommand(byte[] data) {
		return SpotMessageDecodeInfo.getCommand(data);
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
