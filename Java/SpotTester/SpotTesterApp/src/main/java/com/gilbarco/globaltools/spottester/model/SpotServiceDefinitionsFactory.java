package com.gilbarco.globaltools.spottester.model;

import com.gilbarco.globaltools.edt.services.EdtServiceDefinition;

public final class SpotServiceDefinitionsFactory {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

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
	private static EdtServiceDefinition	timerDefinition;
	private static EdtServiceDefinition	mgpDefinition;
	private static EdtServiceDefinition	processDefinition;
	private static EdtServiceDefinition	socketDefinition;
	private static EdtServiceDefinition	loggingDefinition;
	private static EdtServiceDefinition	cryptoDefinition;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public static EdtServiceDefinition getTimerDefinition() {
		return timerDefinition;
	}

	public static EdtServiceDefinition getMgpDefinition() {
		return mgpDefinition;
	}

	public static EdtServiceDefinition getProcessDefinition() {
		return processDefinition;
	}

	public static EdtServiceDefinition getSocketDefinition() {
		return socketDefinition;
	}

	public static EdtServiceDefinition getLoggingDefinition() {
		return loggingDefinition;
	}

	public static EdtServiceDefinition getCryptoDefinition() {
		return cryptoDefinition;
	}

	public void setTimerDefinition(EdtServiceDefinition timerDefinition) {
		SpotServiceDefinitionsFactory.timerDefinition = timerDefinition;
	}

	public void setMgpDefinition(EdtServiceDefinition mgpDefinition) {
		SpotServiceDefinitionsFactory.mgpDefinition = mgpDefinition;
	}

	public void setProcessDefinition(EdtServiceDefinition processDefinition) {
		SpotServiceDefinitionsFactory.processDefinition = processDefinition;
	}

	public void setSocketDefinition(EdtServiceDefinition socketDefinition) {
		SpotServiceDefinitionsFactory.socketDefinition = socketDefinition;
	}

	public void setLoggingDefinition(EdtServiceDefinition loggingDefinition) {
		SpotServiceDefinitionsFactory.loggingDefinition = loggingDefinition;
	}

	public void setCryptoDefinition(EdtServiceDefinition cryptoDefinition) {
		SpotServiceDefinitionsFactory.cryptoDefinition = cryptoDefinition;
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
