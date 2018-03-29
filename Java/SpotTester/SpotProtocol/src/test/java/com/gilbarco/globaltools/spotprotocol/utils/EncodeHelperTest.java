package com.gilbarco.globaltools.spotprotocol.utils;

import java.util.Arrays;
import java.util.Map;

import junit.framework.Assert;

import org.apache.commons.collections15.map.HashedMap;
import org.junit.Before;
import org.junit.Test;

import com.gilbarco.globaltools.mgp.api.EncodeHelper;
import com.gilbarco.globaltools.mgp.exceptions.MessageGeneratorParserException;
import com.gilbarco.globaltools.mgp.persistence.PersistenceManager;
import com.gilbarco.globaltools.mgp.persistence.ProtocolProvider;
import com.gilbarco.globaltools.mgp.persistence.XstreamPersistenceManager;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageFactory;

public class EncodeHelperTest {
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

	private EncodeHelper	helper;

	@Before
	public void before() {
		try {
			PersistenceManager manager = new XstreamPersistenceManager();
			manager.setPersistenceDirectory("/src/test/resources/");
			ProtocolProvider provider = new ProtocolProvider(manager);
			provider.setProtocolDefinition("SPOT Protocol_01.04.xml");
			SpotMessageFactory factory = new SpotMessageFactory(provider);
			helper = new EncodeHelper(factory);
		}
		catch (MessageGeneratorParserException e) {
			e.printStackTrace();
			Assert.fail(e.getMessage());
		}
	}

	@Test
	public void LoginTest() {
		Map<String, Object> data = new HashedMap<String, Object>();

		data.put("AppId", new byte[] { 0x00 });
		data.put("ssk", new byte[] { 0x00 });
		data.put("ClientId", new byte[] { 0x64 });
		data.put("ModuloLen", new byte[] { 0x00 });
		data.put("PingTimeout", new byte[] { 0x05 });

		try {
			Assert.assertTrue(Arrays.equals(helper.processMessage(data, new byte[] { 0x01 }).encode(), new byte[] {
							0x00,
							0x09,
							0x00,
							0x00,
							0x01,
							0x64,
							0x00,
							0x00,
							0x05 }));
		}
		catch (Exception e) {
			Assert.fail(e.getMessage());
		}

	}

	@Test
	public void FileBrowserTest() {
		final byte[] result = new byte[] { 0x00, 0x07, 0x01, 0x00, 0x00, 0x03, 0x01 };
		Map<String, Object> data = new HashedMap<String, Object>();
		data.put("AppId ", new byte[] { 0x01 });
		data.put("ssk", new byte[] { 0x00 });
		data.put("FileType", new byte[] { 0x01 });
		try {
			Assert.assertTrue(Arrays.equals(helper.processMessage(data, new byte[] { 0x00, 0x03, 0x01 }).encode(), result));
		}
		catch (Exception e) {
			Assert.fail(e.getMessage());
		}

	}

	@Test
	public void ShowWindowTest() {
		final byte[] result = new byte[] { 0x00, 0x0A, 0x01, 0x00, 0x30, 0x07, 0x01, 0x01, 0x00, 0x00 };
		Map<String, Object> data = new HashedMap<String, Object>();
		Map<String, Object> tag = new HashedMap<String, Object>();

		data.put("AppId ", new byte[] { 0x01 });
		data.put("ssk", new byte[] { 0x00 });
		data.put("WinId", new byte[] { 0x01 });

		tag.put("TagId", new byte[] { 0x00 });
		tag.put("Zorder", new byte[] { 0x00 });

		data.put("TagsArray", tag);
		data.put("numTags", new byte[] { 0x01 });

		try {
			Assert.assertTrue(Arrays.equals(helper.processMessage(data, new byte[] { 0x30, 0x07, 0x01 }).encode(), result));
		}
		catch (Exception e) {
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
