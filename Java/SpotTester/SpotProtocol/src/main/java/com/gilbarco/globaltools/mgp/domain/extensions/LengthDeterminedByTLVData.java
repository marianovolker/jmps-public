package com.gilbarco.globaltools.mgp.domain.extensions;

import com.gilbarco.globaltools.edt.util.EdtBuffer;
import com.gilbarco.globaltools.mgp.domain.ChildElement;
import com.gilbarco.globaltools.mgp.domain.FieldData;
import com.gilbarco.globaltools.mgp.domain.FieldLength;
import com.gilbarco.globaltools.mgp.exceptions.MessageDataException;

public class LengthDeterminedByTLVData extends FieldLength {
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
	private int	maxLength;
	private int	dataSize;
	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public LengthDeterminedByTLVData() {
		super(null);
		this.maxLength = 900;
		this.dataSize = 0;
	}

	// ******************************************************************
	// PUBLIC METHODS       (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public int getMaxLength() {

		if (this.dataSize > 0) {
			return Math.min(this.maxLength, this.dataSize);
		}
		else {
			return this.maxLength;
		}

	}

	@Override
	public int getMinLength() {

		return 0;
	}
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	@Override
	protected EdtBuffer calculateLength(byte[] data, int from) throws MessageDataException {
		if (from == 0) {
			//we are encoding
			this.dataSize = data.length;
			return new EdtBuffer(FieldData.convertToByteArray(data.length));
		}
			
		//we are decoding
		return this.calculateLengthFromData(data, from);
	}

	@Override
	protected void copyDependencies(ChildElement copiedElement, FieldLength copiedLength) {

	}
	
	protected EdtBuffer calculateLengthFromData(byte[] data, int from) throws MessageDataException {
		//the TAG has 2 bytes
		int totalLength = 2;
		
		byte byte1 = data[from + totalLength];
		
		switch(byte1){
			case (byte) 0x81:
				totalLength += (data[from + totalLength + 1] & 0xFF);
				totalLength += 1;
				break;
			case (byte) 0x82:
				totalLength += (((data[from + totalLength + 1] & 0xFF) << 8) | (data[from + totalLength + 2] & 0xFF));
				totalLength += 2;
				break;
			default:
				totalLength += byte1;
		}
		
		return new EdtBuffer(FieldData.convertToByteArray(totalLength));
	}
	
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
