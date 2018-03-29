/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */


package com.gilbarco.globaltools.sasprotocol.xml.services;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import javax.xml.stream.XMLStreamException;
import junit.framework.Assert;
import org.junit.Test;

import com.gilbarco.globaltools.edt.util.EdtSignData;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasCertificateApprovalRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasCertificateInformationRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasRemoveCertificateRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenCodeRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenQueryRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenRequest;

public class SasStaxXmlRequestsTest
{
	private File schema;
	private SasXmlService xmlService;
	
	public SasStaxXmlRequestsTest()
	{
		EdtSignData.getInstance().setPrivateKeyFilename(new String("src//test//java//com//gilbarco//globaltools//sasprotocol//xml//services//test.tls_server.private.key"));
		
		schema 		= new File( "src//test//java//com//gilbarco//globaltools//sasprotocol//xml//services//SasSchema.xsd" );
		xmlService 	= SasXmlServiceFactory.getInstance( SasXmlServiceFactory.Parsers.StAX );
	}

	@Test
	public void marshall_SasTokenRequest_test() throws XMLStreamException, IOException
	{
		// Marshall test section code
		////////////////////////////////////////////////////////
		SasTokenRequest sasMessage = new SasTokenRequest( );
		
		sasMessage.setTechnician("12345678901234567890");
		sasMessage.setTokenCode("ABCD SDKJ 3SF7 SLKD ALKG ASFK FGHI Z6IC");
		xmlService.marshall( sasMessage, schema );
		
		Assert.assertEquals( true, sasMessage.getSignature().equalsIgnoreCase("B3:78:FE:DC:B9:FB:11:F6:71:97:60:42:40:D6:53:23:9E:A7:95:EB:AD:D7:30:5A:7A:9A:21:B8:F5:8B:1A:1C:B1:51:B6:B4:D3:32:18:3D:52:52:E1:D6:26:A2:55:CF:8F:E0:15:A6:86:6C:EC:B2:5A:F7:E4:FD:0F:FE:6B:AA:17:0F:21:16:EF:DF:C8:62:53:B0:E4:05:EE:18:E0:26:EC:ED:D5:70:FA:5D:42:88:42:A9:62:73:08:DE:69:CF:8D:0F:C0:1A:FB:82:D7:79:FD:C9:91:D2:B2:B3:D4:3E:23:ED:B9:7A:0F:EF:E7:B3:AE:A1:F3:EB:12:DD:27:80:1A:13:2B:67:DC:39:46:F7:16:B0:52:8C:FA:3C:85:7D:41:8D:4E:9B:3E:20:91:2E:B5:9D:8D:38:7F:11:C6:89:63:55:75:38:95:F6:72:5F:B4:4C:93:C6:33:DC:99:F3:B4:FE:0E:9B:A3:7D:27:A8:18:F3:DA:52:B8:C5:36:1B:16:D6:4A:06:F6:70:F7:10:38:57:D9:08:FA:8E:6A:72:F9:70:AD:01:07:CC:AA:C3:2C:90:DD:7D:99:05:5C:23:2D:DE:98:39:B4:3C:E6:4E:FA:13:33:9C:3D:29:45:70:D6:F0:61:91:80:F4:B1:95:63:96:A6:17:3C:BE:A5:1A"));
		////////////////////////////////////////////////////////
	}
	
	@Test
	public void marshall_and_print_SasTokenRequest_test() throws XMLStreamException, IOException
	{
		// Marshall test section code
		////////////////////////////////////////////////////////		
		SasTokenRequest sasMessage = new SasTokenRequest( );
		sasMessage.setTechnician("12345678901234567890");
		sasMessage.setTokenCode("ABCD SDKJ 3SF7 SLKD ALKG ASFK FGHI Z6IC");
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		////////////////////////////////////////////////////////		
		
		// Print test section code.
		////////////////////////////////////////////////////////
		String toPrint = new String(getInputBytes(newBuffer));
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString() );
		System.out.println( "--------------" );
		////////////////////////////////////////////////////////		
	}
	
	@Test
	public void marshall_and_print_SasTokenCodeRequest_test() throws XMLStreamException, IOException
	{
		// Marshall test section code
		////////////////////////////////////////////////////////		
		SasTokenCodeRequest sasMessage = new SasTokenCodeRequest();
		sasMessage.createTimeWindowGroup( );			
		sasMessage.setStartTime( "2012-11-01T10:15:30" );
		sasMessage.setEndTime( "2012-11-20T22:30:00"  );
		sasMessage.createSessionGroup( );			
		sasMessage.setTimes( "5" );
		sasMessage.createTerminalGroup( );			
		sasMessage.setTargetUnit( "000000111111" );
		sasMessage.setRemoteMaintenance( "Disabled" );
		sasMessage.setPermissions(  "SecureDevicesConfiguration" );
		sasMessage.setTechnicianID( "543" );
		sasMessage.setOperatorID( "Operator-ID1" );		
				
		////////////////////////////////////////////////////////		
		// Print test section code.
		////////////////////////////////////////////////////////		
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		Assert.assertEquals( true, sasMessage.getSignature().equalsIgnoreCase("56:AB:16:81:00:7F:BE:C7:DF:CF:39:AE:4A:F2:85:DB:08:C8:DC:E6:56:02:A3:10:B6:30:2E:45:F6:EB:73:47:03:68:E2:9D:E5:91:48:BD:AE:FB:94:89:9E:6D:E5:0A:E3:80:B7:11:3B:CC:6F:DC:64:49:74:16:1D:8E:F8:C3:81:F1:C1:29:13:55:1F:17:36:A6:E8:94:B8:68:DD:1B:C5:50:70:0D:A8:C7:65:F5:23:C6:97:03:14:C2:BF:11:82:94:8B:40:39:96:24:58:8B:CF:B0:44:AB:CE:A6:E7:2C:0A:CD:0A:D0:71:A8:E9:C8:38:BA:14:44:AB:31:4B:BF:56:0B:40:04:1F:4E:01:D5:69:E3:28:7C:52:24:85:A9:C6:AF:54:06:43:2A:26:6F:AF:CE:8D:A5:BA:E6:36:AF:6B:D7:FB:FF:BD:3B:78:41:7E:A2:65:1C:81:BD:68:44:46:F7:18:44:5D:CC:45:B2:4A:A9:00:60:8A:F8:87:BF:4C:C2:1A:9F:AE:0B:40:63:3F:B6:42:A0:F7:2A:82:C7:31:04:19:51:85:B1:9B:20:57:32:5C:9D:9C:45:13:56:41:27:A5:59:50:B1:47:5F:0B:65:45:A2:A3:16:08:3D:5C:E7:83:D0:C9:96:15:09:0C:F3:BB:5F:10:9F:15"));
		
		String toPrint = new String(getInputBytes(newBuffer));
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString() );
		System.out.println( "--------------" );		
		////////////////////////////////////////////////////////		
	}
	
	@Test
	public void marshall_and_print_SasCertificateInformationRequest_test() throws XMLStreamException, IOException
	{
		// Marshall test section code
		////////////////////////////////////////////////////////		
		SasCertificateInformationRequest sasMessage = new SasCertificateInformationRequest( );
		sasMessage.setTechnicianId("123456789");
		sasMessage.setValidationCode( "ABCD SDKJ 3SF7 SLKD ALKG ASFK LXM6 Z6IC");
		////////////////////////////////////////////////////////		
		
		// Print test section code.
		////////////////////////////////////////////////////////		
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		Assert.assertEquals( true, sasMessage.getSignature().equalsIgnoreCase("9A:1A:A7:C1:95:77:D0:5B:67:00:2C:1C:2E:5A:E7:48:5F:78:D1:D5:51:9F:B4:85:48:D1:DE:98:82:1B:5F:24:F7:C8:5D:20:E0:83:69:C7:8B:4F:C3:B4:8B:42:9B:CC:8B:B5:2B:3E:04:47:B7:AA:91:EE:2F:1B:12:C7:AC:CE:1F:E7:C3:04:F5:61:96:57:02:4F:40:26:8C:2D:4F:CB:D8:AF:8A:BE:30:56:25:79:38:64:65:F0:DF:C5:F7:E8:95:C0:74:EB:E1:DA:45:B0:6C:50:F5:E5:BB:7F:AE:23:C7:05:B5:0A:D8:20:C1:0F:E4:E8:4B:3B:2A:42:7F:F5:CA:62:72:44:26:F8:9B:7B:B7:E3:5B:3F:E1:22:E0:72:18:BD:B8:1D:63:A7:6A:8F:F1:6B:AC:C8:03:80:1A:B7:99:65:00:09:94:24:75:BA:8A:28:1D:FE:DE:8B:54:61:DE:36:B0:C2:1B:DF:2A:A0:FB:3D:AC:45:60:1F:41:0E:C7:F8:8A:29:68:CA:CE:CF:5E:E1:52:77:FD:9A:D2:48:40:37:D0:76:61:DD:10:BF:E3:BB:36:B1:BD:9F:0F:E9:C7:5D:7C:B9:B5:B4:A1:D0:05:F4:0B:B5:F9:8F:D2:0C:59:22:9C:A1:2D:58:67:F5:66:7A:73:47:30:87:BA:09"));
		
		String toPrint = new String(getInputBytes(newBuffer));
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString() );
		System.out.println( "--------------" );		
		////////////////////////////////////////////////////////		
	}
	
	@Test
	public void marshall_and_print_SasCertificateApprovalRequest_test() throws XMLStreamException, IOException
	{
		// Marshall test section code
		////////////////////////////////////////////////////////		
		SasCertificateApprovalRequest sasMessage = new SasCertificateApprovalRequest( );
		sasMessage.setTechnicianId("123456789");
		sasMessage.setValidationCode( "ABCD SDKJ 3SF7 SLKD ALKG ASFK LXM6 Z6IC");
		////////////////////////////////////////////////////////		
		
		// Print test section code.
		////////////////////////////////////////////////////////		
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		Assert.assertEquals( true, sasMessage.getSignature().equalsIgnoreCase("9A:1A:A7:C1:95:77:D0:5B:67:00:2C:1C:2E:5A:E7:48:5F:78:D1:D5:51:9F:B4:85:48:D1:DE:98:82:1B:5F:24:F7:C8:5D:20:E0:83:69:C7:8B:4F:C3:B4:8B:42:9B:CC:8B:B5:2B:3E:04:47:B7:AA:91:EE:2F:1B:12:C7:AC:CE:1F:E7:C3:04:F5:61:96:57:02:4F:40:26:8C:2D:4F:CB:D8:AF:8A:BE:30:56:25:79:38:64:65:F0:DF:C5:F7:E8:95:C0:74:EB:E1:DA:45:B0:6C:50:F5:E5:BB:7F:AE:23:C7:05:B5:0A:D8:20:C1:0F:E4:E8:4B:3B:2A:42:7F:F5:CA:62:72:44:26:F8:9B:7B:B7:E3:5B:3F:E1:22:E0:72:18:BD:B8:1D:63:A7:6A:8F:F1:6B:AC:C8:03:80:1A:B7:99:65:00:09:94:24:75:BA:8A:28:1D:FE:DE:8B:54:61:DE:36:B0:C2:1B:DF:2A:A0:FB:3D:AC:45:60:1F:41:0E:C7:F8:8A:29:68:CA:CE:CF:5E:E1:52:77:FD:9A:D2:48:40:37:D0:76:61:DD:10:BF:E3:BB:36:B1:BD:9F:0F:E9:C7:5D:7C:B9:B5:B4:A1:D0:05:F4:0B:B5:F9:8F:D2:0C:59:22:9C:A1:2D:58:67:F5:66:7A:73:47:30:87:BA:09"));
		
		String toPrint = new String(getInputBytes(newBuffer));
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString() );
		System.out.println( "--------------" );		
		////////////////////////////////////////////////////////		
	}
	
	@Test
	public void marshall_and_print_SasRemoveCertificateRequest_test() throws XMLStreamException, IOException
	{
		// Marshall test section code
		////////////////////////////////////////////////////////		
		SasRemoveCertificateRequest sasMessage = new SasRemoveCertificateRequest( );
		sasMessage.setTechnicianId("123456789");
		////////////////////////////////////////////////////////		
		
		// Print test section code.
		////////////////////////////////////////////////////////		
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		Assert.assertEquals( true, sasMessage.getSignature().equalsIgnoreCase("68:F5:BE:53:3F:2C:B7:56:67:B4:C5:FA:F1:DC:D2:53:3B:46:BF:19:4F:91:A7:92:E9:57:CB:0F:2A:FC:11:36:24:1A:F6:F9:33:B0:A7:DB:2E:BC:9F:F1:4A:5E:85:B4:73:E7:9D:D8:AA:D4:3E:B4:4A:E4:8B:BC:6E:DA:7D:FA:55:67:C0:79:1E:B4:B8:62:67:02:6F:86:5B:9C:0A:8B:B0:F3:A9:27:50:E5:13:EA:07:F3:4E:05:9E:45:14:FE:EB:04:BD:4F:AA:85:37:4B:0D:67:D2:8A:99:CA:C1:21:D1:90:33:16:8B:7E:D0:2D:D5:BF:F2:E9:6C:FE:0C:0A:0E:36:1E:BF:9C:7E:7F:84:0C:A3:5F:33:39:DA:0E:FE:E2:08:F8:1C:EE:3D:08:02:06:81:E1:E3:31:FA:57:34:2D:9E:80:5A:B3:12:2D:6D:33:B3:A6:7F:6F:CB:A7:22:95:48:5A:15:10:05:78:56:39:34:B7:B4:CF:8B:53:19:90:1B:9A:A3:93:AF:75:22:0A:37:78:31:18:E9:29:52:B0:A3:69:57:B4:0D:AC:36:5E:22:6B:1E:5E:E7:3B:55:21:E2:E6:BA:F8:E1:07:85:B4:60:28:80:AC:77:35:FD:0F:25:1C:5D:39:0F:5D:30:ED:2F:32:18:09:E7:92:62"));
		
		String toPrint = new String(getInputBytes(newBuffer));
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString() );
		System.out.println( "--------------" );		
		////////////////////////////////////////////////////////		
	}
	
	@Test
	public void marshall_and_print_SasTokenQueryRequest_test() throws XMLStreamException, IOException
	{
		// Marshall test section code
		////////////////////////////////////////////////////////		
		SasTokenQueryRequest sasMessage = new SasTokenQueryRequest();
		sasMessage.setTechnician("123444");
		sasMessage.setTargetUnit("0000000000000000000000000000001023782333");
		////////////////////////////////////////////////////////
		
		// Print test section code.
		////////////////////////////////////////////////////////		
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );

		Assert.assertEquals( true, sasMessage.getSignature().equalsIgnoreCase("4F:BF:00:A7:EF:7A:6B:83:02:EA:76:EF:13:C9:AE:41:F1:1F:D7:2E:68:F7:0E:3C:E8:75:5C:3E:F4:F6:F5:73:6D:28:44:8F:E8:BD:63:1B:27:87:2C:6E:4E:80:3F:7F:33:9C:52:FA:6B:0E:03:2B:03:80:4B:EE:05:65:00:D3:50:3B:9A:A3:EC:9B:AC:60:D9:FD:2D:E1:66:43:64:85:3D:00:91:C6:49:51:82:A9:29:26:61:04:81:9C:A6:5C:97:05:F4:C0:5E:71:14:8B:4D:B7:26:26:6B:98:08:B2:6F:63:A8:CD:09:1C:2D:EE:B6:F2:65:D3:E7:EF:DB:ED:56:BF:05:24:D7:E8:B1:A4:F5:AA:06:22:30:68:BF:AA:AC:0D:19:F9:8C:3B:D6:8E:43:55:0A:E0:DE:CA:28:23:1E:2E:DB:CB:BF:7B:3D:77:DD:29:66:46:91:84:27:E8:D5:C7:C1:53:0A:03:13:9D:BC:FB:15:4D:F7:77:0B:42:87:60:B2:5B:AF:5E:C4:70:15:B8:C8:5D:37:F1:57:A5:43:EB:B0:93:9B:BB:DD:29:4E:C1:E9:67:47:60:39:6B:C0:85:AA:B8:C3:10:1A:9A:80:96:69:7F:A7:A7:BF:DE:5A:FF:04:8F:B2:12:1D:AB:86:64:82:39:BC:43:D1:92"));
		
		String toPrint = new String(getInputBytes(newBuffer));
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString() );
		System.out.println( "--------------" );		
		////////////////////////////////////////////////////////		
	}	
	
	public static byte[] getInputBytes(InputStream is) throws IOException
	{

		int thisLine;
		ByteArrayOutputStream bos = new ByteArrayOutputStream( );
		while( (thisLine = is.read( )) != -1 )
		{
			bos.write( thisLine );
		}
		bos.flush( );
		byte[] yourBytes = bos.toByteArray( );

		if (bos != null)
		{
			bos.close( );
		}

		return yourBytes;
	}
}
