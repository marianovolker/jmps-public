package com.gilbarco.globaltools.service;

import java.util.Arrays;
import java.util.Map;

import junit.framework.Assert;

import org.apache.commons.collections15.map.HashedMap;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceDefinition;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.EdtServiceManager;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpService;
import com.gilbarco.globaltools.mgp.edt.service.MgpServiceAdapter;

public class EdtMgpTest {
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
	private MgpServiceAdapter	adapter;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Before
	public void initTest() {
		try {
			ClassPathXmlApplicationContext ctx = new ClassPathXmlApplicationContext("SpotProtocolService.xml");
			EdtServiceDefinition spotDefinition = ctx.getBean("SpotServiceDefinition", EdtServiceDefinition.class);
			MgpService spotService = ctx.getBean(MgpService.class);
			
			EdtServiceManager manager = EdtServiceContext.getServiceManager();
			
			manager.registerService(spotService);
			adapter = (MgpServiceAdapter)manager.getServiceAdapter(spotDefinition);
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Test
	public void edtMgpServiceEncodeTest() {
		// Building the message
		Map<String, Object> data = new HashedMap<String, Object>();

		data.put("AppId", new byte[] { 0x00 });
		data.put("ssk", new byte[] { 0x00 });
		data.put("ClientId", new byte[] { 0x64 });
		data.put("ModuloLen", new byte[] { 0x00 });
		data.put("PingTimeout", new byte[] { 0x05 });

		byte[] encodedData = null;
		try {
			encodedData = adapter.callEncode(data, new byte[] { 0x01 });
		}
		catch (EdtServiceException e) {
			e.printStackTrace();
		}
		byte[] loginData = new byte[] { 0x00, 0x09, 0x00, 0x00, 0x01, 0x64, 0x00, 0x00, 0x05 };
		System.out.println("data: " + EdtConvert.bytesToHexString(loginData, 1));
		System.out.println("enco: " + EdtConvert.bytesToHexString(encodedData, 1));
		Assert.assertTrue(Arrays.equals(encodedData, loginData));
		
		Map<String, Object> decodedData = null;
		try {
			decodedData = adapter.callDecode(loginData);
		}
		catch (EdtServiceException e) {
			e.printStackTrace();
		}
		System.out.println("data: " + data);
		System.out.println("deco: " + decodedData);
//		Assert.assertTrue(data.equals(decodedData));
	}

	@Test
	public void EdtMgpServiceDecodeTest() {
		byte[] data = EdtConvert
						.hexStringToBytes("00 FA 80 00 01 00 31 36 30 30 33 36 33 30 30 38 32 30 31 30 05 00 9C C6 12 89 30 32 2E 30 31 2E 31 70 31 32 31 38 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 C0 4D 41 1C 30 32 2E 30 34 2E 31 70 31 32 33 33 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 03 DF AB 9D F6 30 32 2E 30 32 2E 30 70 31 32 33 33 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 04 95 E1 65 8E 30 32 2E 30 32 2E 30 70 31 32 33 33 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 07 50 B3 ED 50 39 39 2E 39 39 2E 30 70 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");

		Map<String, Object> decodedData = null;
		try {
			decodedData = adapter.callDecode(data);
		}
		catch (EdtServiceException e) {
			e.printStackTrace();
		}
		System.out.println(decodedData);

	}

	@Test
	public void edtMgpServiceEncodeInvokeTest() {
		try {
			EdtServiceContext.getServiceManager().subscribe((new EdtEventAction<MgpResultEvent>() {

				@Override
				public void onInvokeEvent(MgpResultEvent event) {
					Assert.assertTrue(Arrays.equals(event.getEncodedData(), new byte[] {
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

				@Override
				public Class<MgpResultEvent> getEventClass() {
					return MgpResultEvent.class;
				}

				@Override
				public boolean justOnce() {
					return false;
				}

				@Override
				public String getSubscriptionFilter() {
					return null;
				}
			}), EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.ENCODE.name() + "'");
		}
		catch (EdtServiceException e1) {
			e1.printStackTrace();
		}

		// Building the message
		Map<String, Object> data = new HashedMap<String, Object>();

		data.put("AppId", new byte[] { 0x00 });
		data.put("ssk", new byte[] { 0x00 });
		data.put("ClientId", new byte[] { 0x64 });
		data.put("ModuloLen", new byte[] { 0x00 });
		data.put("PingTimeout", new byte[] { 0x05 });

		try {
			adapter.callEncode(data, new byte[] { 0x01 });
		}
		catch (EdtServiceException e1) {
			e1.printStackTrace();
		}
		try {
			Thread.sleep(1000);
		}
		catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	@Test
	public void edtMgpServiceDecodeInvokeTest() {
		try {
			EdtServiceContext.getServiceManager().subscribe((new EdtEventAction<MgpResultEvent>() {

				@Override
				public void onInvokeEvent(MgpResultEvent event) {
					System.out.println("Result Map: " + event.getDecodeMap());
				}

				@Override
				public Class<MgpResultEvent> getEventClass() {
					return MgpResultEvent.class;
				}

				@Override
				public boolean justOnce() {
					return false;
				}

				@Override
				public String getSubscriptionFilter() {
					return null;
				}

			}), EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.DECODE.name() + "'");

		}
		catch (EdtServiceException e) {
			e.printStackTrace();
		}

		byte[] data = EdtConvert
						.hexStringToBytes("00 FA 80 00 01 00 31 36 30 30 33 36 33 30 30 38 32 30 31 30 05 00 9C C6 12 89 30 32 2E 30 31 2E 31 70 31 32 31 38 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 C0 4D 41 1C 30 32 2E 30 34 2E 31 70 31 32 33 33 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 03 DF AB 9D F6 30 32 2E 30 32 2E 30 70 31 32 33 33 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 04 95 E1 65 8E 30 32 2E 30 32 2E 30 70 31 32 33 33 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 07 50 B3 ED 50 39 39 2E 39 39 2E 30 70 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00");
		try {
			adapter.callDecode(data);
		}
		catch (EdtServiceException e1) {
			e1.printStackTrace();
		}
		try {
			Thread.sleep(1000);
		}
		catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	@After
	public void after() {
		System.out.println("Done...");
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
