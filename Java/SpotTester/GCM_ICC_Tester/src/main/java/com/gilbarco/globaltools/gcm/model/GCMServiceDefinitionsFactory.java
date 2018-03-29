package com.gilbarco.globaltools.gcm.model;

import com.gilbarco.globaltools.edt.services.EdtServiceDefinition;

public final class GCMServiceDefinitionsFactory {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static EdtServiceDefinition gcmProcessDefinition;
	
	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	public static EdtServiceDefinition getGcmProcessDefinition() {
		return gcmProcessDefinition;
	}

	public void setGcmProcessDefinition(EdtServiceDefinition gcmProcessDefinition) {
		GCMServiceDefinitionsFactory.gcmProcessDefinition = gcmProcessDefinition;
	}
	
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

	// ******************************************************************
	// PUBLIC METHODS       (general, getter, setter, interface imp)
	// ******************************************************************

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