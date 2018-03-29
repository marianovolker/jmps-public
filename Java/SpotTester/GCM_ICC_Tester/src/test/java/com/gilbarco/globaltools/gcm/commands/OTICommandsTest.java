package com.gilbarco.globaltools.gcm.commands;

import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.junit.Test;
import org.springframework.util.Assert;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.EdtServiceManager;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketAdapter;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.mgp.edt.service.MgpServiceAdapter;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

public class OTICommandsTest {
	private static final byte	SPOT_CLIENT_ID	= (byte) 0x64;

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log		logger		= LogFactory.getLog(OTICommandsTest.class);

	private static final transient String	nl			= "\r";
	private static final transient String	tab			= "\t";
	private static final transient String	COMMANDS	= "------COMMANDS------" + nl + tab + "0 = Exit" + nl + tab
															+ "1 = Enable Card Reader" + nl + tab
															+ "2 = Disable Card Reader" + nl + tab + "3 = Browse EMV File"
															+ nl + tab + "4 = Load Public Keys" + nl + tab
															+ "7 = OTI Get Command" + nl + tab + "8 = OTI Set Command" + nl
															+ tab + "9 = OTI Do Command" + nl + tab;

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	public static void main(String[] args) {
		OTICommandsTest oct = new OTICommandsTest();
		oct.prepare();
		try {
			boolean run = true;
			while (run) {
				System.out.println(COMMANDS);
				int i = readNextOption();
				switch(i){
					case 0 :
						run = false;
						break;
					case 1 :
						oct.enableReader();
						break;
					case 2 :
						oct.disableReader();
						break;
					case 3 :
						oct.fileBrowse();
						break;
					case 4 :
						oct.loadPublicKey();
						break;
					case 7 :
					{
						System.out.println("Get" + nl);
						System.out.println("Enter TAG:" + nl);
						String tag = readNextInput();
						oct.sendOtiGetCommand(tag);

						break;
					}
					case 8 :
					{
						System.out.println("Set" + nl);
						System.out.println("Enter TAG:" + nl);
						String tag = readNextInput();
						System.out.println("Enter DATA (w/out length):" + nl);
						String data = readNextInput();
						oct.sendOtiSetCommand(tag, data);

						break;
					}
					case 9 :
					{
						System.out.println("Do" + nl);
						System.out.println("Enter TAG:" + nl);
						String tag = readNextInput();
						System.out.println("Enter DATA (w/out length):" + nl);
						String data = readNextInput();
						oct.sendOtiDoCommand(tag, data);

						break;
					}
					default :
//						System.out.println("OPTION IS NOT SUPPORTED !");
				}
			}
		}
		catch (Exception e) {
			logger.error(e.getMessage(), e);
			System.exit(1);
		}

		oct.terminate();
		System.exit(0);
	}

	private static int readNextOption() {
		try {
			return Integer.parseInt(readNextInput());
		}
		catch (Exception e) {

		}

		return -1;
	}

	private static String readNextInput() throws IOException {
		byte[] b = new byte[1024];
		int l = System.in.read(b);
		return new String(b, 0, l - 1);
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
	private EdtClientSocketAdapter	socketService;
	private MgpServiceAdapter		mgpService;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public void prepare() {
		this.loadServices();
		this.login();
	}

	public void enableReader() {
		this.sendData(SpotMessages.getInstance().getEnableCLReader(new byte[] { 0x00, 0x00 }), "Enable CR");
	}

	public void disableReader() {
		this.sendData(SpotMessages.getInstance().getOTIDisableCLReader(), "DisableCLReader");
	}

	public void sendOtiGetCommand(String tag) {
		this.sendData(SpotMessages.getInstance().getOTIGetData(EdtConvert.hexStringToBytes(tag), new byte[] { 0x00, 0x00 }),
			"GET " + tag);
	}

	public void sendOtiSetCommand(String tag, String data) {
		this.sendData(SpotMessages.getInstance().getOTISetData(EdtConvert.hexStringToBytes(tag),
			EdtConvert.hexStringToBytes(data)), "Set " + tag + " " + data);
	}

	public void sendOtiDoCommand(String tag, String data) {
		this.sendData(SpotMessages.getInstance().getOTIDoData(EdtConvert.hexStringToBytes(tag),
			EdtConvert.hexStringToBytes(data)), "Do " + tag + " " + data);
	}

	public void fileBrowse() {
		this.sendData(SpotMessages.getInstance().getFileBrowse((byte) 0x02), "FileBrowse");
	}

	public void loadPublicKey() {
		String tag = "DF51";
		String data;
		// Visa
		data = "A0000000039600010180B74586D19A207BE6627C5B0AAFBC44A2ECF5A2942D3A26CE19C4FFAEEE920521868922E893E7838225A3947A2614796FB2C0628CE8C11E3825A56D3B1BBAEF783A5C6A81F36F8625395126FA983C5216D3166D48ACDE8A431212FF763A7F79D9EDB7FED76B485DE45BEB829A3D4730848A366D3324C3027032FF8D16A1E44D8D01037616E9AC8BE014AF88CA11A8FB17967B7394030E";
		this.sendData(SpotMessages.getInstance().getOTISetData(EdtConvert.hexStringToBytes(tag),
			EdtConvert.hexStringToBytes(data)), "Set " + tag + " A00000000396");

		// Mastercard FE
		data = "A000000004FE00010180A653EAC1C0F786C8724F737F172997D63D1C3251C44402049B865BAE877D0F398CBFBE8A6035E24AFA086BEFDE9351E54B95708EE672F0968BCD50DCE40F783322B2ABA04EF137EF18ABF03C7DBC5813AEAEF3AA7797BA15DF7D5BA1CBAF7FD520B5A482D8D3FEE105077871113E23A49AF3926554A70FE10ED728CF793B62A101039A295B05FB390EF7923F57618A9FDA2941FC34E0";
		this.sendData(SpotMessages.getInstance().getOTISetData(EdtConvert.hexStringToBytes(tag),
			EdtConvert.hexStringToBytes(data)), "Set " + tag + " A000000004FE");

		// Mastercard F3
		data = "A000000004F30001019098F0C770F23864C2E766DF02D1E833DFF4FFE92D696E1642F0A88C5694C6479D16DB1537BFE29E4FDC6E6E8AFD1B0EB7EA0124723C333179BF19E93F10658B2F776E829E87DAEDA9C94A8B3382199A350C077977C97AFF08FD11310AC950A72C3CA5002EF513FCCC286E646E3C5387535D509514B3B326E1234F9CB48C36DDD44B416D23654034A66F403BA511C5EFA30103A69AC7603DAF566E972DEDC2CB433E07E8B01A9A";
		this.sendData(SpotMessages.getInstance().getOTISetData(EdtConvert.hexStringToBytes(tag),
			EdtConvert.hexStringToBytes(data)), "Set " + tag + " A000000004F3");

		// Mastercard F8
		data = "A000000004F800010180A1F5E1C9BD8650BD43AB6EE56B891EF7459C0A24FA84F9127D1A6C79D4930F6DB1852E2510F18B61CD354DB83A356BD190B88AB8DF04284D02A4204A7B6CB7C5551977A9B36379CA3DE1A08E69F301C95CC1C20506959275F41723DD5D2925290579E5A95B0DF6323FC8E9273D6F849198C4996209166D9BFC973C361CC826E10103F06ECC6D2AAEBF259B7E755A38D9A9B24E2FF3DD";
		this.sendData(SpotMessages.getInstance().getOTISetData(EdtConvert.hexStringToBytes(tag),
			EdtConvert.hexStringToBytes(data)), "Set " + tag + " A000000004F8");

		// Mastercard FA
		data = "A000000004FA00010190A90FCD55AA2D5D9963E35ED0F440177699832F49C6BAB15CDAE5794BE93F934D4462D5D12762E48C38BA83D8445DEAA74195A301A102B2F114EADA0D180EE5E7A5C73E0C4E11F67A43DDAB5D55683B1474CC0627F44B8D3088A492FFAADAD4F42422D0E7013536C3C49AD3D0FAE96459B0F6B1B6056538A3D6D44640F94467B108867DEC40FAAECD740C00E2B7A8852D01035BED4068D96EA16D2D77E03D6036FC7A160EA99C";
		this.sendData(SpotMessages.getInstance().getOTISetData(EdtConvert.hexStringToBytes(tag),
			EdtConvert.hexStringToBytes(data)), "Set " + tag + " A000000004FA");

		// Mastercard EF
		data = "A000000004EF000101F8A191CB87473F29349B5D60A88B3EAEE0973AA6F1A082F358D849FDDFF9C091F899EDA9792CAF09EF28F5D22404B88A2293EEBBC1949C43BEA4D60CFD879A1539544E09E0F09F60F065B2BF2A13ECC705F3D468B9D33AE77AD9D3F19CA40F23DCF5EB7C04DC8F69EBA565B1EBCB4686CD274785530FF6F6E9EE43AA43FDB02CE00DAEC15C7B8FD6A9B394BABA419D3F6DC85E16569BE8E76989688EFEA2DF22FF7D35C043338DEAA982A02B866DE5328519EBBCD6F03CDD686673847F84DB651AB86C28CF1462562C577B853564A290C8556D818531268D25CC98A4CC6A0BDFFFDA2DCCA3A94C998559E307FDDF915006D9A987B07DDAEB3B010321766EBB0EE122AFB65D7845B73DB46BAB65427A";
		this.sendData(SpotMessages.getInstance().getOTISetData(EdtConvert.hexStringToBytes(tag),
			EdtConvert.hexStringToBytes(data)), "Set " + tag + " A000000004EF");

		// Mastercard F1
		data = "A000000004F1000101B0A0DCF4BDE19C3546B4B6F0414D174DDE294AABBB828C5A834D73AAE27C99B0B053A90278007239B6459FF0BBCD7B4B9C6C50AC02CE91368DA1BD21AAEADBC65347337D89B68F5C99A09D05BE02DD1F8C5BA20E2F13FB2A27C41D3F85CAD5CF6668E75851EC66EDBF98851FD4E42C44C1D59F5984703B27D5B9F21B8FA0D93279FBBF69E090642909C9EA27F898959541AA6757F5F624104F6E1D3A9532F2A6E51515AEAD1B43B3D7835088A2FAFA7BE70103D8E68DA167AB5A85D8C3D55ECB9B0517A1A5B4BB";
		this.sendData(SpotMessages.getInstance().getOTISetData(EdtConvert.hexStringToBytes(tag),
			EdtConvert.hexStringToBytes(data)), "Set " + tag + " A000000004F1");
	}

	public void terminate() {
		try {
			this.sendData(SpotMessages.getInstance().getSpotLogoutData(), "LogOut");
			Thread.sleep(1000);
			this.socketService.callCloseSocket("End Test");
		}
		catch (EdtServiceException e) {
			logger.error("Loging out ERROR: " + e.getMessage(), e);
		}
		catch (InterruptedException e) {
			logger.error("sleeping ERROR: " + e.getMessage(), e);
		}
	}

	@Test
	public void dummy() {
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void loadServices() {
		EdtServiceManager manager = EdtServiceContext.getServiceManager();
		if (this.socketService == null) {
			try {
				this.socketService = (EdtClientSocketAdapter) manager.getServiceAdapter("ClientSocketDefinition");
				Assert.notNull(this.socketService);
				EdtServiceContext.getServiceManager().subscribe(new EdtEventAction<EdtClientSocketEvent>() {

					@Override
					public Class<EdtClientSocketEvent> getEventClass() {
						return EdtClientSocketEvent.class;
					}

					@Override
					public String getSubscriptionFilter() {
						return EdtClientSocketEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.INCOMING_DATA + "' OR "
								+ EdtClientSocketEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.OUTGOING_DATA + "'";
					}

					@Override
					public boolean justOnce() {
						return false;
					}

					@Override
					public void onInvokeEvent(EdtClientSocketEvent event) {
						String log = "";
						if (event.getEventType().equals(SocketEventType.INCOMING_DATA)) {
							log = "RX: ";
						}
						if (event.getEventType().equals(SocketEventType.OUTGOING_DATA)) {
							log = "TX: ";
						}
						try {
							logger.debug(log + EdtConvert.bytesToHexString(event.getData(), 1));
							logger.debug(log + mgpService.callDecode(event.getData()));
						}
						catch (EdtServiceException e) {
							e.printStackTrace();
						}
					}
				});
			}
			catch (EdtServiceException e) {
				logger.error("SocketService ERROR: " + e.getMessage(), e);
				e.printStackTrace();
			}
		}

		if (this.mgpService == null) {
			try {
				this.mgpService = (MgpServiceAdapter) manager.getServiceAdapter("SpotServiceDefinition");
				Assert.notNull(this.mgpService, "MGP Service was not created");

			}
			catch (EdtServiceException e) {
				logger.error("MgpService ERROR: " + e.getMessage(), e);
				e.printStackTrace();
			}
		}
	}

	private void login() {

		try {
			this.socketService.callConnect();
			this.socketService.invokeSendData(SpotMessages.getInstance().getSpotLoginData(SPOT_CLIENT_ID));
		}
		catch (Exception e) {
			logger.error("Loging in ERROR: " + e.getMessage(), e);
		}
	}

	private void sendData(byte[] data, String cmd) {
		logger.debug("TX: " + cmd);
		try {
			this.socketService.invokeSendData(data);
		}
		catch (EdtServiceException e) {
			logger.error(cmd + " ERROR: " + e.getMessage(), e);
		}
	}
	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
