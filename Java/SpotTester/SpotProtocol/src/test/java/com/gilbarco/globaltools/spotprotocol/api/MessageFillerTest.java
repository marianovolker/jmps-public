package com.gilbarco.globaltools.spotprotocol.api;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import org.junit.Assert;
import org.junit.Test;

import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.mgp.api.filler.AbstractFiller;
import com.gilbarco.globaltools.mgp.api.filler.ArrayContainerFiller;
import com.gilbarco.globaltools.mgp.api.filler.FieldFiller;
import com.gilbarco.globaltools.mgp.domain.Message;
import com.gilbarco.globaltools.mgp.persistence.PersistenceManager;
import com.gilbarco.globaltools.mgp.persistence.ProtocolProvider;
import com.gilbarco.globaltools.mgp.persistence.XstreamPersistenceManager;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageFactory;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;

public class MessageFillerTest {
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

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Test
	public void MessageFillerCreateWindow() {
		final byte[] result = new byte[] {
						0x00,
						(byte) 0x3C,
						0x01,
						0x00,
						0x30,
						0x03,
						0x01,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x01,
						0x40,
						0x00,
						(byte) 0xF0,
						0x64,
						0x64,
						(byte) 0xF0,
						0x00,
						(byte) 0xFF,
						(byte) 0xC8,
						0x00,
						0x01,
						0x01,
						0x01,
						0x64,
						0x64,
						(byte) 0xF0,
						0x00,
						0x00,
						0x10,
						0x00,
						0x10,
						0x00,
						(byte) ("Please insert your card".getBytes()).length,
						0x50,
						0x6C,
						0x65,
						0x61,
						0x73,
						0x65,
						0x20,
						0x69,
						0x6E,
						0x73,
						0x65,
						0x72,
						0x74,
						0x20,
						0x79,
						0x6F,
						0x75,
						0x72,
						0x20,
						0x63,
						0x61,
						0x72,
						0x64 };

		Map<String, AbstractFiller<?>> tagMap = new HashMap<String, AbstractFiller<?>>();
		tagMap.put("TagId", new FieldFiller((byte) 0x00));
		tagMap.put("OriginX", new FieldFiller(new byte[] { 0x00, 0x00 }));
		tagMap.put("OriginY", new FieldFiller(new byte[] { 0x00, 0x00 }));
		tagMap.put("Width", new FieldFiller(new byte[] { 0x01, 0x40 }));
		tagMap.put("Height", new FieldFiller(new byte[] { 0x00, (byte) 0xF0 }));
		tagMap.put("BackRGB", new FieldFiller(new byte[] { 0x64, 0x64, (byte) 0xF0 }));
		tagMap.put("BorderSize", new FieldFiller((byte) 0x00));
		tagMap.put("BorderRGB", new FieldFiller(new byte[] { (byte) 0xFF, (byte) 0xC8, 0x00 }));
		tagMap.put("Visible", new FieldFiller(new byte[] { 0x01 }));
		tagMap.put("TagType", new FieldFiller((byte) 0x01));
		tagMap.put("FontId", new FieldFiller((byte) 0x01));
		tagMap.put("FontRGB", new FieldFiller(new byte[] { 0x64, 0x64, (byte) 0xF0 }));
		tagMap.put("Alligen", new FieldFiller(new byte[] { 0x00 }));
		tagMap.put("posX", new FieldFiller((byte) 0x00, (byte) 0x10));
		tagMap.put("posY", new FieldFiller((byte) 0x00, (byte) 0x10));
		// TODO: turn TextLen Field into OtherElementLengthAsValue dependant on Text Field
		tagMap.put("TextLen", new FieldFiller((byte) "Please insert your card".getBytes().length));
		tagMap.put("Text", new FieldFiller("Please insert your card".getBytes()));

		ArrayContainerFiller tag = new ArrayContainerFiller(tagMap);

		Map<String, AbstractFiller<?>> map = new HashMap<String, AbstractFiller<?>>();
		map.put("Command", new FieldFiller(new byte[] { 0x30 }));
		map.put("SubCommand", new FieldFiller((byte) 0x03));
		map.put("WinId", new FieldFiller((byte) 0x01));
		map.put("AppId", new FieldFiller((byte) 0x01));
		map.put("ssk", new FieldFiller((byte) 0x00));
		map.put("TagsArray", tag);

		try {
			PersistenceManager manager = new XstreamPersistenceManager();
			manager.setPersistenceDirectory("/src/test/resources/");
			ProtocolProvider provider = new ProtocolProvider(manager);
			provider.setProtocolDefinition("SPOT Protocol_01.04-ext.2.3.xml");
			SpotMessageFactory mf = new SpotMessageFactory(provider);
			Message m = mf.getMessage(SpotMessageType.CREATE_WINDOW, map);

			byte[] encoded = m.encode();
			System.out.println("expected: " + EdtConvert.bytesToHexString(result, 1));
			System.out.println("received: " + EdtConvert.bytesToHexString(encoded, 1));
			Assert.assertTrue(Arrays.equals(encoded, result));
		}
		catch (Exception e) {
			e.printStackTrace();
			Assert.fail(e.getMessage());
		}
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
