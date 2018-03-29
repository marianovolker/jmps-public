/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.sasprotocol.xml.services;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.xml.stream.XMLStreamException;

import junit.framework.Assert;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.junit.Test;

import com.gilbarco.globaltools.sasprotocol.xml.messages.SasMessage;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlErrorCodeMsg;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasCertificateInformationResponse;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasPublishCertificateResponse;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenCodeResponse;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenQueryResponse;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenResponse;

public class SasStaxXmlResponsesTest
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	private static final transient Log	logger	= LogFactory.getLog( SasStaxXmlResponsesTest.class );
	private File						schema;
	private SasXmlService				xmlService;

	public SasStaxXmlResponsesTest()
	{
		schema = new File( "src//test//java//com//gilbarco//globaltools//sasprotocol//xml//services//SasSchema.xsd" );
		xmlService = SasXmlServiceFactory.getInstance( SasXmlServiceFactory.Parsers.StAX );
	}

	@Test
	public void unmarshall_and_print_SasErrorCode_test() throws XMLStreamException, IOException
	{
		InputStream buffer = null;
		try
		{
			buffer = new ByteArrayInputStream(
												getInputBytes( new FileInputStream(
																					"src//test//java//com//gilbarco//globaltools//sasprotocol//xml//services//01_SasErrorCode.xml" ) ) );
		}
		catch (IOException e)
		{
			logger.error( e.getMessage( ) );
		}		

		// Unmarshall test section code
		// //////////////////////////////////////////////////////
		SasMessage sasMessage = xmlService.unmarshall( schema, buffer );
		Assert.assertEquals( true, sasMessage instanceof SasXmlErrorCodeMsg );
		Assert.assertEquals( true, Integer.parseInt( ((SasXmlErrorCodeMsg) sasMessage).getErrorCode( ).getValue( ) ) == 1 );
		Assert.assertEquals( true, ((SasXmlErrorCodeMsg) sasMessage).getDescription( ).getValue( ).equalsIgnoreCase( "Description of the error" ) );
		// //////////////////////////////////////////////////////

		// Print test section code.
		// //////////////////////////////////////////////////////
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		String toPrint = new String( getInputBytes( newBuffer ) );
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString( ) );
		System.out.println( "--------------" );
		// //////////////////////////////////////////////////////
	}
	
	@Test
	public void unmarshall_and_print_SasTokenCodeResponse_test() throws XMLStreamException, IOException
	{
		InputStream buffer = null;
		try
		{
			buffer = new ByteArrayInputStream(
												getInputBytes( new FileInputStream(
																					"src//test//java//com//gilbarco//globaltools//sasprotocol//xml//services//02_SasTokenCodeResponse.xml" ) ) );
		}
		catch (IOException e)
		{
			logger.error( e.getMessage( ) );
		}		

		// Unmarshall test section code
		// //////////////////////////////////////////////////////
		SasMessage sasMessage = xmlService.unmarshall( schema, buffer );
		Assert.assertEquals( true, sasMessage instanceof SasTokenCodeResponse );
		Assert.assertEquals( true, ((SasTokenCodeResponse) sasMessage).getTechnician( ).getValue( ).equalsIgnoreCase( "123456789012345" ) );
		Assert.assertEquals( true, ((SasTokenCodeResponse) sasMessage).getTokenCode( ).getValue( ).equalsIgnoreCase( "ABCD SDKJ 3SF7 SLKD ALKG ASFK LXM6 Z6IC" ) );
		// //////////////////////////////////////////////////////

		// Print test section code.
		// //////////////////////////////////////////////////////
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		String toPrint = new String( getInputBytes( newBuffer ) );
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString( ) );
		System.out.println( "--------------" );
		// //////////////////////////////////////////////////////
	}
	 
	@Test
	public void unmarshall_and_print_SasTokenResponse_test() throws XMLStreamException, IOException
	{
		InputStream buffer = null;
		try
		{
			buffer = new ByteArrayInputStream(
												getInputBytes( new FileInputStream(
																					"src//test//java//com//gilbarco//globaltools//sasprotocol//xml//services//03_SasTokenResponse.xml" ) ) );
		}
		catch (IOException e)
		{
			logger.error( e.getMessage( ) );
		}
	
		// Unmarshall test section code
		// //////////////////////////////////////////////////////
		SasMessage sasMessage = xmlService.unmarshall( schema, buffer );
		Assert.assertEquals( true, sasMessage instanceof SasTokenResponse );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getVersion( ).getValue( ).equalsIgnoreCase( "0001.0001.0021" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getStartTime( ).getValue( ).equalsIgnoreCase( "2012-11-01T10:15:30" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getEndTime( ).getValue( ).equalsIgnoreCase( "2012-11-20T22:30:00" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getTimes( ).getValue( ).equalsIgnoreCase( "5" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getMaxDuration( ).getValue( ).equalsIgnoreCase( "20" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getTargetUnit( ).equalsIgnoreCase( "000000111111" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getTokenCode( ).getValue( ).equalsIgnoreCase( "YYXU EVRL TLB3 UEMZ J22P 62AC TK6G W72Z" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getRemoteMaintenance( ).getValue( ).equalsIgnoreCase( "Disabled" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getPermissions( ).getValue( ).contains( "DateAndTimeSetup\n" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getPermissions( ).getValue( ).contains( "NetworkConfiguration\n" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getPermissions( ).getValue( ).contains( "SecureDevicesConfiguration" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getPermissions( ).getValue( ).contains( "SecureDevicesActivation" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getPermissions( ).getValue( ).contains( "SecureLogRetrieval" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getPermissions( ).getValue( ).contains( "FirmwareUpdate" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getPermissions( ).getValue( ).contains( "FirmwareInfo" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getPermissions( ).getValue( ).contains( "UploadSystemKeys" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getTechnicianID( ).getValue( ).equalsIgnoreCase( "123456" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getTechnicianDevice( ).getValue( ).equalsIgnoreCase( "Technician-Device1" ) );
		Assert.assertEquals( true, ((SasTokenResponse) sasMessage).getOperatorID( ).getValue( ).equalsIgnoreCase( "Operator-ID1" ) );
		// //////////////////////////////////////////////////////

		// Print test section code.
		// //////////////////////////////////////////////////////
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		String toPrint = new String( getInputBytes( newBuffer ) );
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString( ) );
		System.out.println( "--------------" );
		// //////////////////////////////////////////////////////
	}

	@Test
	public void unmarshall_and_print_SasPublishCertificateResponse_test() throws XMLStreamException, IOException
	{
		InputStream buffer = null;
		try
		{
			buffer = new ByteArrayInputStream(
												getInputBytes( new FileInputStream(
																					"src//test//java//com//gilbarco//globaltools//sasprotocol//xml//services//04_SasPublishCertificateResponse.xml" ) ) );
		}
		catch (IOException e)
		{
			logger.error( e.getMessage( ) );
		}

		// Unmarshall test section code
		// //////////////////////////////////////////////////////
		SasMessage sasMessage = xmlService.unmarshall( schema, buffer );
		Assert.assertEquals( true, sasMessage instanceof SasPublishCertificateResponse );
		Assert.assertEquals( true, ((SasPublishCertificateResponse) sasMessage).getTechnicianID( ).equalsIgnoreCase( "132" ) );
		Assert.assertEquals( true, ((SasPublishCertificateResponse) sasMessage).getValidationCode( ).equalsIgnoreCase( "DKYN GKMH UR6P XDRO R7IP 2D7L UFXW HTGP" ) );
		// //////////////////////////////////////////////////////

		// Print test section code.
		// //////////////////////////////////////////////////////
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		String toPrint = new String( getInputBytes( newBuffer ) );
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString( ) );
		System.out.println( "--------------" );
		// //////////////////////////////////////////////////////
	}

	@Test
	public void unmarshall_and_print_SasCertificateInformation_test() throws XMLStreamException, IOException
	{
		InputStream buffer = null;
		try
		{
			buffer = new ByteArrayInputStream(
												getInputBytes( new FileInputStream(
																					"src//test//java//com//gilbarco//globaltools//sasprotocol//xml//services//05_SasCertificateInformationResponse.xml" ) ) );
		}
		catch (IOException e)
		{
			logger.error( e.getMessage( ) );
		}

		// Unmarshall test section code
		// //////////////////////////////////////////////////////
		SasMessage sasMessage = xmlService.unmarshall( schema, buffer );
		Assert.assertEquals( true, sasMessage instanceof SasCertificateInformationResponse );
		Assert.assertEquals( true, ((SasCertificateInformationResponse) sasMessage).getTechnician( ).equalsIgnoreCase( "123456789" ) );
		Assert.assertEquals( true, ((SasCertificateInformationResponse) sasMessage).getCertificate( ).equalsIgnoreCase( "Hola!\n" ) );
		// //////////////////////////////////////////////////////

		// Print test section code.
		// //////////////////////////////////////////////////////
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		String toPrint = new String( getInputBytes( newBuffer ) );
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString( ) );
		System.out.println( "--------------" );
		// //////////////////////////////////////////////////////
	}
	
	@Test
	public void unmarshall_and_print_SasTokenQueryResponse_test() throws XMLStreamException, IOException
	{
		InputStream buffer = null;
		try
		{
			buffer = new ByteArrayInputStream(
												getInputBytes( new FileInputStream(
																					"src//test//java//com//gilbarco//globaltools//sasprotocol//xml//services//06_SasTokenQueryResponse.xml" ) ) );
		}
		catch (IOException e)
		{
			logger.error( e.getMessage( ) );
		}

		// Unmarshall test section code
		// //////////////////////////////////////////////////////
		SasMessage sasMessage = xmlService.unmarshall( schema, buffer );
		Assert.assertEquals( true, sasMessage instanceof SasTokenQueryResponse );
		
		SasTokenQueryResponse sasTokenQueryResponse = ((SasTokenQueryResponse) sasMessage);
		
		Assert.assertEquals( true, sasTokenQueryResponse.getTechnicianID( ).equalsIgnoreCase( "123456789" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getTargetUnit( ).equalsIgnoreCase( "0000000000000000000000000000001023782539" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getQuantityOfTokens( ) == 3 );
		
		// Token 1
		Assert.assertEquals( true, sasTokenQueryResponse.getTokenCode			( 0 ).equalsIgnoreCase( "ABCD SDKJ 3SF7 SLKD ALKG ASFK LXM6 Z6IC" ) );		
		Assert.assertEquals( true, sasTokenQueryResponse.getTimes				( 0 ).equalsIgnoreCase( "5" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getMaxDuration			( 0 ).equalsIgnoreCase( "20" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getStartTime			( 0 ).equalsIgnoreCase( "2012-10-01T10:15:30" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getEndTime				( 0 ).equalsIgnoreCase( "2012-10-20T22:30:40" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getRemoteMaintenance	( 0 ).equalsIgnoreCase( "Enabled" ) );
		System.out.println( "--------------" );
		System.out.println( "List of Permission of token 1: [" + sasTokenQueryResponse.getPermissions( 0 ) + "]." );
		System.out.println( "--------------" );		
		
		// Token 2
		Assert.assertEquals( true, sasTokenQueryResponse.getTokenCode			( 1 ).equalsIgnoreCase( "TYJA KISS KLOZ DART 5FG4 33TU L4M3 TROZ" ) );		
		Assert.assertEquals( true, sasTokenQueryResponse.getTimes				( 1 ).equalsIgnoreCase( "7" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getMaxDuration			( 1 ).equalsIgnoreCase( "23" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getStartTime			( 1 ).equalsIgnoreCase( "2012-10-01T10:15:30" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getEndTime				( 1 ).equalsIgnoreCase( "2012-10-20T22:30:40" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getRemoteMaintenance	( 1 ).equalsIgnoreCase( "Enabled" ) );
		System.out.println( "--------------" );
		System.out.println( "List of Permission of token 2: [" + sasTokenQueryResponse.getPermissions( 1 ) + "]." );
		System.out.println( "--------------" );		
		
		// Token 3
		Assert.assertEquals( true, sasTokenQueryResponse.getTokenCode			( 2 ).equalsIgnoreCase( "WFS3 LKG4 ASFK LXM6 SDKJ 3SF7 SLKD Z1HA" ) );		
		Assert.assertEquals( true, sasTokenQueryResponse.getTimes				( 2 ).equalsIgnoreCase( "6" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getMaxDuration			( 2 ).equalsIgnoreCase( "21" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getStartTime			( 2 ).equalsIgnoreCase( "2012-10-01T10:16:30" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getEndTime				( 2 ).equalsIgnoreCase( "2012-11-20T22:30:40" ) );
		Assert.assertEquals( true, sasTokenQueryResponse.getRemoteMaintenance	( 2 ).equalsIgnoreCase( "Disabled" ) );
		System.out.println( "--------------" );
		System.out.println( "List of Permission of token 3: [" + sasTokenQueryResponse.getPermissions( 2 ) + "]." );
		System.out.println( "--------------" );		
		// //////////////////////////////////////////////////////

		// Print test section code.
		/////////////////////////////////////////////////////////
		InputStream newBuffer = xmlService.marshall( sasMessage, schema );
		String toPrint = new String( getInputBytes( newBuffer ) );
		System.out.println( "--------------" );
		System.out.println( toPrint );
		System.out.println( "--------------" );
		System.out.println( sasMessage.toString( ) );
		System.out.println( "--------------" );
		/////////////////////////////////////////////////////////

	}	

	public static byte[] getInputBytes(InputStream is) throws IOException
	{

		int thisLine;
		ByteArrayOutputStream bos = new ByteArrayOutputStream( );
		while ((thisLine = is.read( )) != -1)
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
