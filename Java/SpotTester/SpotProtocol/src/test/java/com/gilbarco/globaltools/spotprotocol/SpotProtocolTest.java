package com.gilbarco.globaltools.spotprotocol;

import java.util.Arrays;
import java.util.List;

import junit.framework.Assert;

import org.junit.BeforeClass;
import org.junit.Test;

import com.gilbarco.globaltools.edt.util.EdtBuffer;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.mgp.domain.ArrayContainer;
import com.gilbarco.globaltools.mgp.domain.Container;
import com.gilbarco.globaltools.mgp.domain.ContainerType;
import com.gilbarco.globaltools.mgp.domain.Field;
import com.gilbarco.globaltools.mgp.domain.Message;
import com.gilbarco.globaltools.mgp.domain.Protocol;
import com.gilbarco.globaltools.mgp.exceptions.MessageDataException;
import com.gilbarco.globaltools.mgp.exceptions.PredicateException;
import com.gilbarco.globaltools.mgp.persistence.PersistenceManager;
import com.gilbarco.globaltools.mgp.persistence.XstreamPersistenceManager;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;

/**
 * @author gparis
 * 
 */
public class SpotProtocolTest
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	public static final String	strHex00to0F	= "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ";
	public static final String	strHex10to1F	= "10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F ";
	public static final String	strHex20to2F	= "20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F ";
	public static final String	strHex30to3F	= "30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F ";
	public static final String	strHex40to4F	= "40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F ";
	public static final String	strHex50to5F	= "50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F ";
	public static final String	strHex60to6F	= "60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F ";
	public static final String	strHex70to7F	= "70 71 72 73 74 75 76 77 78 79 7A 7B 7C 7D 7E 7F ";
	public static final String	strHex80to8F	= "80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F ";
	public static final String	strHex90to9F	= "90 91 92 93 94 95 96 97 98 99 9A 9B 9C 9D 9E 9F ";
	public static final String	strHexA0toAF	= "A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF ";
	public static final String	strHexB0toBF	= "B0 B1 B2 B3 B4 B5 B6 B7 B8 B9 BA BB BC BD BE BF ";
	public static final String	strHexC0toCF	= "C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF ";
	public static final String	strHexD0toDF	= "D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF ";
	public static final String	strHexE0toEF	= "E0 E1 E2 E3 E4 E5 E6 E7 E8 E9 EA EB EC ED EE EF ";

	public static final String	strHexF0toF7	= "F0 F1 F2 F3 F4 F5 F6 F7 ";
	public static final String	strHexF7toFF	= "F8 F9 FA FB FC FD FE FF ";

	private static Protocol		spotProtocol;

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
	// STATIC METHODS
	// ******************************************************************

	@BeforeClass
	public static void prepare()
	{
		SpotProtocol sp = new SpotProtocol( );
		try
		{
			spotProtocol = sp.buildSpotProtocol( );
		}
		catch (PredicateException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	public static String getHex00toF7()
	{
		return strHex00to0F + strHex10to1F + strHex20to2F + strHex30to3F + strHex40to4F + strHex50to5F + strHex60to6F + strHex70to7F + strHex80to8F + strHex90to9F + strHexA0toAF + strHexB0toBF
				+ strHexC0toCF + strHexD0toDF + strHexE0toEF + strHexF0toF7;
	}

	public static String getHex00toFF()
	{
		return strHex00to0F + strHex10to1F + strHex20to2F + strHex30to3F + strHex40to4F + strHex50to5F + strHex60to6F + strHex70to7F + strHex80to8F + strHex90to9F + strHexA0toAF + strHexB0toBF
				+ strHexC0toCF + strHexD0toDF + strHexE0toEF + strHexF0toF7 + strHexF7toFF;
	}

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Test
	public void exportSpotProtocol()
	{
		try
		{
			PersistenceManager manager = new XstreamPersistenceManager( );
			manager.setPersistenceDirectory( "/src/test/resources/" );
			System.out.println( spotProtocol.toString( ) );
			manager.save( spotProtocol );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void login()
	{
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.LOGIN.getName( ) );
			Message login = messages.get( 0 );

			// Filling the fields
			Message workingCopy = login.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.findChildElement( Field.class, "ClientId" );
			tmp.get( 0 ).setData( (byte) 0x64 );
			tmp = workingCopy.findChildElement( Field.class, "ModuloLen" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.findChildElement( Field.class, "PingTimeout" );
			tmp.get( 0 ).setData( (byte) 0x05 );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "Login: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			workingCopy.decode( encoded );
			System.out.println( "Login: " + EdtConvert.bytesToHexString( login.getData( ), 1 ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void loginResponse()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.LOGIN.getName( ), ContainerType.RESPONSE );
			Message login = messages.get( 0 ).getWorkingCopy( );

			byte[] data = EdtConvert
							.hexStringToBytes( "00 FA 80 00 01 00 31 36 30 30 33 36 33 30 30 38 32 30 31 30 05 00 9C C6 12 89 30 32 2E 30 31 2E 31 70 31 32 31 38 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 C0 4D 41 1C 30 32 2E 30 34 2E 31 70 31 32 33 33 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 03 DF AB 9D F6 30 32 2E 30 32 2E 30 70 31 32 33 33 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 04 95 E1 65 8E 30 32 2E 30 32 2E 30 70 31 32 33 33 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 07 50 B3 ED 50 39 39 2E 39 39 2E 30 70 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00" );
			login.decode( data );
			System.out.println( login.getProtocol( ).toString( ) );
			byte[] encode = login.encode( );
			System.out.println( EdtConvert.bytesToHexString( data, 1 ) );
			System.out.println( EdtConvert.bytesToHexString( encode, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void logout()
	{
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.LOGOUT.getName( ) );
			Message logout = messages.get( 0 );

			// Filling the fields
			Message workingCopy = logout.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "Logout: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			workingCopy.decode( encoded );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void ping()
	{
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.PING.getName( ) );
			Message ping = messages.get( 0 );

			// Filling the fields
			Message workingCopy = ping.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			byte[] encoded = workingCopy.encode( );
			System.out.println( "Ping: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			workingCopy.decode( encoded );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void pingResponse()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.PING.getName( ), ContainerType.RESPONSE );
			Message login = messages.get( 0 ).getWorkingCopy( );

			byte[] data = EdtConvert.hexStringToBytes( "00 06 00 00 03 00" );
			login.decode( data );
			byte[] encode = login.encode( );
			System.out.println( EdtConvert.bytesToHexString( data, 1 ) );
			System.out.println( EdtConvert.bytesToHexString( encode, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void createClearWindow()
	{
		try
		{
			final byte[] result = new byte[]
			{ 0x00, 0x1A, 0x01, 0x00, 0x30, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, (byte) 0xF0, 0x64, 0x64, (byte) 0xF0, 0x00, (byte) 0xFF, (byte) 0xC8, 0x00, 0x01, 0x00 };
			final byte[] decodeResult = new byte[]
			{ 0x30, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, (byte) 0xF0, 0x64, 0x64, (byte) 0xF0, 0x00, (byte) 0xFF, (byte) 0xC8, 0x00, 0x01, 0x00 };

			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.CREATE_WINDOW.getName( ) );
			Message clearScreen = messages.get( 0 );

			// Filling the fields

			Message workingCopy = clearScreen.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );

			// Fill common field
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "WinId" );
			tmp.get( 0 ).setData( (byte) 0x01 );

			@SuppressWarnings("rawtypes")
			List<ArrayContainer> aux = workingCopy.getProtocol( ).findChildElement( ArrayContainer.class, "TagsArray" );
			Container container = (Container) aux.get( 0 ).createNewElement( );

			tmp = container.getProtocol( ).findChildElement( Field.class, "TagId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "OriginX" );
			tmp.get( 0 ).setData( (byte) 0x00, (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "OriginY" );
			tmp.get( 0 ).setData( (byte) 0x00, (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "Width" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x01, 0x40 } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "Height" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, (byte) 0xF0 } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "BackRGB" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x64, 0x64, (byte) 0xF0 } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "BorderSize" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "BorderRGB" );
			tmp.get( 0 ).setData( new byte[]
			{ (byte) 0xFF, (byte) 0xC8, 0x00 } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "Visible" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "TagType" );
			tmp.get( 0 ).setData( (byte) 0x00 );

			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "numTags" );
			tmp.get( 0 ).setData( new EdtBuffer( new byte[]
			{ 0x01 } ) );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "Clear Screen encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "Clear Screen result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

			workingCopy = clearScreen.getWorkingCopy( );
			workingCopy.decode( result );
			System.out.println( "Clear Screen after decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "Clear Screen after decode: " + EdtConvert.bytesToHexString( decodeResult, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( workingCopy.getData( ), decodeResult ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void createTextWindow()
	{
		try
		{

			final byte[] text = "Please insert your card".getBytes( );
			final byte[] result = new byte[]
			{
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
							0x00,
							0x64,
							0x64,
							(byte) 0xF0,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							(byte) text.length,
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
			final byte[] decodeResult = new byte[]
			{
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
							0x00,
							0x64,
							0x64,
							(byte) 0xF0,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							(byte) text.length,
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

			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.CREATE_WINDOW.getName( ) );
			Message createText = messages.get( 0 );

			// Filling the fields

			Message workingCopy = createText.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );

			// Fill common field
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "WinId" );
			tmp.get( 0 ).setData( (byte) 0x01 );

			@SuppressWarnings("rawtypes")
			List<ArrayContainer> aux = workingCopy.getProtocol( ).findChildElement( ArrayContainer.class, "TagsArray" );
			Container container = (Container) aux.get( 0 ).createNewElement( );

			tmp = container.getProtocol( ).findChildElement( Field.class, "TagId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "OriginX" );
			tmp.get( 0 ).setData( (byte) 0x00, (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "OriginY" );
			tmp.get( 0 ).setData( (byte) 0x00, (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "Width" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x01, 0x40 } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "Height" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, (byte) 0xF0 } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "BackRGB" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x64, 0x64, (byte) 0xF0 } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "BorderSize" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "BorderRGB" );
			tmp.get( 0 ).setData( new byte[]
			{ (byte) 0xFF, (byte) 0xC8, 0x00 } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "Visible" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "TagType" );
			tmp.get( 0 ).setData( (byte) 0x01 );

			// Text
			tmp = container.getProtocol( ).findChildElement( Field.class, "FontId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "FontRGB" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x64, 0x64, (byte) 0xF0 } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "Alligen" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "posX" );
			tmp.get( 0 ).setData( (byte) 0x00, (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "posY" );
			tmp.get( 0 ).setData( (byte) 0x00, (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "TextLen" );
			tmp.get( 0 ).setData( new byte[]
			{ (byte) text.length } );
			tmp = container.getProtocol( ).findChildElement( Field.class, "Text" );
			tmp.get( 0 ).setData( text );
			// tmp = workingCopy.getProtocol().findChildElement(Field.class, "numTags");
			// tmp.get(0).setData(new EdtBuffer(new byte[]{0x01}));

			byte[] encoded = workingCopy.encode( );
			System.out.println( "Clear Screen encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "Clear Screen result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

			workingCopy = createText.getWorkingCopy( );
			workingCopy.decode( result );
			System.out.println( "Text Screen after decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "Text Screen after decode: " + EdtConvert.bytesToHexString( decodeResult, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( workingCopy.getData( ), decodeResult ) );

		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void showWindow()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x0A, 0x01, 0x00, 0x30, 0x07, 0x01, 0x00, 0x00, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.SHOW_WINDOW.getName( ) );
			Message showScreen = messages.get( 0 );

			Message workingCopy = showScreen.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "WinId" );
			tmp.get( 0 ).setData( (byte) 0x01 );

			@SuppressWarnings("rawtypes")
			List<ArrayContainer> aux = workingCopy.getProtocol( ).findChildElement( ArrayContainer.class, "TagsArray" );
			Container container = (Container) aux.get( 0 ).createNewElement( );

			tmp = container.getProtocol( ).findChildElement( Field.class, "TagId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = container.getProtocol( ).findChildElement( Field.class, "Zorder" );
			tmp.get( 0 ).setData( (byte) 0x00 );

			// tmp = workingCopy.getProtocol().findChildElement(Field.class, "numTags");
			// tmp.get(0).setData(new EdtBuffer(new byte[]{0x01}));

			byte[] encoded = workingCopy.encode( );
			System.out.println( "Show Screen encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "Show Screen result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}

	}

	@Test
	public void hideWindow()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x09, 0x01, 0x00, 0x30, 0x08, 0x01, 0x00, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.HIDE_WINDOW.getName( ) );
			Message hideScreen = messages.get( 0 );

			Message workingCopy = hideScreen.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "WinId" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x01 } );

			@SuppressWarnings("rawtypes")
			List<ArrayContainer> aux = workingCopy.getProtocol( ).findChildElement( ArrayContainer.class, "TagsArray" );
			Container container = (Container) aux.get( 0 ).createNewElement( );

			tmp = container.getProtocol( ).findChildElement( Field.class, "TagId" );
			tmp.get( 0 ).setData( (byte) 0x00 );

			// tmp = workingCopy.getProtocol().findChildElement(Field.class, "numTags");
			// tmp.get(0).setData(new EdtBuffer(new byte[]{0x01}));

			byte[] encoded = workingCopy.encode( );
			System.out.println( "Hide Screen encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "Hide Screen result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}

	}

	@Test
	public void FileBrowse()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x07, 0x01, 0x00, 0x00, 0x03, 0x01 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.FILE_BROWSE.getName( ) );
			Message fileBrowser = messages.get( 0 );

			Message workingCopy = fileBrowser.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileType" );
			tmp.get( 0 ).setData( (byte) 0x01 );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "File browse encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "File browse encode: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void FileBrowseResponse()
	{
		final byte[] data = new byte[]
		{ 0x00, 0x1C, (byte) 0x81, 0x00, 0x00, 0x03, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x44, 0x45, 0x53, 0x43, 0x52, 0x49, 0x50, 0x54, 0x49, 0x4F, 0x4E, 0x10, 0x00 };
		final byte[] result = new byte[]
		{ 0x00, 0x03, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x44, 0x45, 0x53, 0x43, 0x52, 0x49, 0x50, 0x54, 0x49, 0x4F, 0x4E, 0x10, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.FILE_BROWSE.getName( ), ContainerType.RESPONSE );
			Message fileBrowser = messages.get( 0 );

			Message workingCopy = fileBrowser.getWorkingCopy( );
			workingCopy.decode( data );
			System.out.println( "File browse decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "File browse expected: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( workingCopy.getData( ), result ) );

		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void FileDelete()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x08, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.FILE_DELETE.getName( ) );
			Message fileDelete = messages.get( 0 );

			Message workingCopy = fileDelete.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileType" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileId" );
			tmp.get( 0 ).setData( (byte) 0x00 );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "File Delete encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "File Delete result: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void FileDeleteResponse()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x04, 0x00 };
		final byte[] data = new byte[]
		{ 0x00, 0x07, (byte) 0x81, 0x00, 0x00, 0x04, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.FILE_DELETE.getName( ), ContainerType.RESPONSE );
			Message fileDelete = messages.get( 0 );

			Message workingCopy = fileDelete.getWorkingCopy( );
			workingCopy.decode( data );
			System.out.println( "File delete decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "File delete expected: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( java.util.Arrays.equals( workingCopy.getData( ), result ) );

		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void M3M5ROMfsFileDownloadResponse()
	{
		final String sMsg = "00 00 83 00 10 01 00 20 40 08 00";

		commonDecodeThenEncode_Test( SpotMessageType.M3M5_ROMfs_FILE_DOWNLOAD, false, sMsg, "M3M5ROMfsFileDownloadResponse() " );
	}

	@Test
	public void M3M5ROMfsFileDownloadRequest()
	{
		final String sCertif = getHex00toF7( );

		final String sMsg = "01 04 03 00 10 01 00 00 40 08 " + sCertif + "00 00";

		commonDecodeThenEncode_Test( SpotMessageType.M3M5_ROMfs_FILE_DOWNLOAD, true, sMsg, "M3M5ROMfsFileDownloadRequest() " );
	}

	@Test
	public void M3M5ROMfsFileDownloadRequest_EncodeOnly()
	{
		try
		{
			byte[] result;
			final String sCertif = getHex00toF7();
			final String sResult = "01 04 03 00 10 01 00 00 40 08 " + sCertif + "01 00";
			List<Message> messages = spotProtocol.findMessage(SpotMessageType.M3M5_ROMfs_FILE_DOWNLOAD.getName(), ContainerType.REQUEST);

			Message pkgDownloadRequest = messages.get(0).getWorkingCopy();
			List<Field> lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "AppId");
			lst.get(0).setData((byte) 0x03);
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "ssk");
			lst.get(0).setData((byte) 0x00);
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "FileSize");
			lst.get(0).setData(new byte[] { 0x00, 0x00, 0x40, 0x08 });
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "Certificate");
			lst.get(0).setData( EdtConvert.hexStringToBytes(sCertif) );
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "VerifyOrSave");
			lst.get(0).setData(new byte[] { 0x01 });
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "Option");
			lst.get(0).setData(new byte[] { 0x00 });

			byte[] encoded = pkgDownloadRequest.encode();
			result = EdtConvert.hexStringToBytes(sResult);

			System.out.println("M3M5ROMfsFileDownloadRequest_EncodeOnly() encode: " + EdtConvert.bytesToHexString(encoded, 1));
			System.out.println("M3M5ROMfsFileDownloadRequest_EncodeOnly() result: " + EdtConvert.bytesToHexString(result, 1));
			Assert.assertTrue(java.util.Arrays.equals(encoded, result));
		}
		catch (MessageDataException e)
		{
			e.printStackTrace();
			Assert.fail(e.getMessage());
		}
	}

	@Test
	public void vanguardROMfsFileDownloadResponse()
	{
		final String sMsg = "00 00 83 00 60 01 00 20 40 08 00";

		commonDecodeThenEncode_Test( SpotMessageType.VANGUARD_ROMfs_FILE_DOWNLOAD, false, sMsg, "vanguardROMfsFileDownloadResponse() " );
	}

	@Test
	public void vanguardROMfsFileDownloadRequest()
	{
		final String sSig = getHex00toF7( ) + strHexF7toFF;

		final String sCert = sSig + sSig + sSig + getHex00toF7( );

		final String sMsg = "05 05 03 00 60 01 00 00 40 08 03 F8 " + sCert + sSig + "00";

		commonDecodeThenEncode_Test( SpotMessageType.VANGUARD_ROMfs_FILE_DOWNLOAD, true, sMsg, "vanguardROMfsFileDownloadRequest() " );
	}

	@Test
	public void vanguardROMfsFileDownloadRequest_EncodeOnly()
	{
		try
		{
			byte[] result;
			final String sCertif = getHex00toF7();
			final String sSig = getHex00toF7( ) + strHexF7toFF;
			final String sResult = "02 05 03 00 60 01 00 00 40 08 00 F8" + sCertif + sSig + "01";
			List<Message> messages = spotProtocol.findMessage(SpotMessageType.VANGUARD_ROMfs_FILE_DOWNLOAD.getName(), ContainerType.REQUEST);

			Message pkgDownloadRequest = messages.get(0).getWorkingCopy();
			List<Field> lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "AppId");
			lst.get(0).setData((byte) 0x03);
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "ssk");
			lst.get(0).setData((byte) 0x00);
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "FileSize");
			lst.get(0).setData(new byte[] { 0x00, 0x00, 0x40, 0x08 });
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "CertificateSize");
			lst.get(0).setData(new byte[] { 0x00, (byte) 0xF8 });
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "Certificate");
			lst.get(0).setData( EdtConvert.hexStringToBytes(sCertif) );
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "Signature");
			lst.get(0).setData( EdtConvert.hexStringToBytes(sSig) );
			lst = pkgDownloadRequest.getProtocol().findChildElement(Field.class, "VerifyOrSave");
			lst.get(0).setData(new byte[] { 0x01 });

			byte[] encoded = pkgDownloadRequest.encode();
			result = EdtConvert.hexStringToBytes(sResult);

			System.out.println("vanguardROMfsFileDownloadRequest_EncodeOnly() encode: " + EdtConvert.bytesToHexString(encoded, 1));
			System.out.println("vanguardROMfsFileDownloadRequest_EncodeOnly() result: " + EdtConvert.bytesToHexString(result, 1));
			Assert.assertTrue(java.util.Arrays.equals(encoded, result));
		}
		catch (MessageDataException e)
		{
			e.printStackTrace();
			Assert.fail(e.getMessage());
		}
	}

	@Test
	public void FileDownloadRequestResponse()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x01, 0x00, 0x11, 0x00, 0x00, 0x00 };
		final byte[] data = new byte[]
		{ 0x00, 0x0B, (byte) 0x81, 0x00, 0x00, 0x01, 0x00, 0x11, 0x00, 0x00, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.FILE_DOWNLOAD_REQUEST.getName( ), ContainerType.RESPONSE );
			Message fileDownloadRequest = messages.get( 0 );

			Message workingCopy = fileDownloadRequest.getWorkingCopy( );
			workingCopy.decode( data );
			System.out.println( "File download request decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "File download request expected: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( java.util.Arrays.equals( workingCopy.getData( ), result ) );

		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void FileDownloadRequestAuthent0()
	{
		final byte[] result = new byte[]
		{
						0x00,
						0x25,
						0x01,
						0x00,
						0x00,
						0x01,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.FILE_DOWNLOAD_REQUEST.getName( ) );
			Message fileDownloadRequest = messages.get( 0 );

			Message workingCopy = fileDownloadRequest.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileType" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileSz" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x00 } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "CRC" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x00 } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AuthenType" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "Description" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "FileBlockDownload encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "FileBlockDownload result: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void FileDownloadRequestAuthent12()
	{
		final byte[] result = new byte[]
		{
						0x00,
						0x25,
						0x01,
						0x00,
						0x00,
						0x01,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x01,
						0x00,
						0x00,
						0x00,
						0x02,
						0x01,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.FILE_DOWNLOAD_REQUEST.getName( ) );
			Message fileDelete = messages.get( 0 );

			Message workingCopy = fileDelete.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileType" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileSz" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x00 } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "CRC" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x00 } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AuthenType" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "Authent12" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x02 } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "Description" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "FileBlockRequest encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "FileBlockRequest result: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( Arrays.equals( encoded, result ) );

		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void VanguardROMfsBlockDownloadResponse()
	{
		final String sMsg = "00 00 83 00 60 02 00 ";

		commonDecodeThenEncode_Test( SpotMessageType.VANGUARD_ROMfs_BLOCK_DOWNLOAD, false, sMsg, "VanguardROMfsBlockDownloadResponse() " );
	}

	@Test
	public void VanguardROMfsBlockDownloadRequest()
	{
		final String sPage = getHex00toFF( );
		final String sEightPages = sPage + sPage + sPage + sPage + sPage + sPage + sPage + sPage;
		final String sMsg = "08 08 03 00 60 02 08 00 " + sEightPages;

		commonDecodeThenEncode_Test( SpotMessageType.VANGUARD_ROMfs_BLOCK_DOWNLOAD, true, sMsg, "VanguardROMfsBlockDownloadRequest() " );
	}

	@Test
	public void M3M5ROMfsBlockDownloadResponse()
	{
		// sDependencies = "Dependencies list."
		final String sDependencies = " 44 65 70 65 6E 64 65 6E 63 69 65 73 20 6C 69 73 74 2E";
		String sLength = String.format( "%04X", sDependencies.length( ) / 3 );
		final String sMsg = "00 00 83 00 10 02 00 " + sLength + sDependencies;

		commonDecodeThenEncode_Test( SpotMessageType.M3M5_ROMfs_BLOCK_DOWNLOAD, false, sMsg, "M3M5ROMfsBlockDownloadResponse() " );
	}

	@Test
	public void M3M5ROMfsBlockDownloadRequest()
	{
		final String sPage = getHex00toFF( );
		final String sEightPages = sPage + sPage + sPage + sPage + sPage + sPage + sPage + sPage;
		final String sMsg = "08 08 03 00 10 02 08 00 " + sEightPages;

		commonDecodeThenEncode_Test( SpotMessageType.M3M5_ROMfs_BLOCK_DOWNLOAD, true, sMsg, "M3M5ROMfsBlockDownloadRequest() " );
	}

	@Test
	public void FileBlockDownloadRequest()
	{
		final byte[] result = new byte[]
		{
						0x00,
						0x7A,
						0x01,
						0x00,
						0x00,
						0x02,
						0x00,
						0x00,
						0x00,
						0x00,
						0x00,
						0x6E,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11,
						0x11 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.FILE_BLOCK_DOWNLOAD.getName( ) );
			Message fileDelete = messages.get( 0 );

			Message workingCopy = fileDelete.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "BlkOffset" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x00 } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "BlkSize" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x6E } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "BlkData" );
			tmp.get( 0 ).setData(
				new byte[]
				{
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11 } );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "FileBlockDownload encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "FileBlockDownload result: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}


	@Test
	public void vanguardROMfsPackageActivationResponse()
	{
		final String sMsg = "00 07 81 00 60 04 00";

		commonDecodeThenEncode_Test( SpotMessageType.VANGUARD_ROMfs_PACKAGE_ACTIVATION, false, sMsg, "vanguardROMfsPackageActivationResponse() " );
	}

	@Test
	public void vanguardROMfsPackageActivationRequest()
	{
		final String sMsg = "00 06 01 00 60 04";

		commonDecodeThenEncode_Test( SpotMessageType.VANGUARD_ROMfs_PACKAGE_ACTIVATION, true, sMsg, "vanguardROMfsPackageActivationRequest() " );
	}

	@Test
	public void M3M5ROMfsPackageActivationResponse()
	{
		final String sMsg = "00 07 83 00 10 04 00";

		commonDecodeThenEncode_Test( SpotMessageType.M3M5_ROMfs_PACKAGE_ACTIVATION, false, sMsg, "M3M5ROMfsPackageActivationResponse() " );
	}

	@Test
	public void M3M5ROMfsPackageActivationRequest()
	{
		final String sMsg = "00 16 03 00 10 04 30 30 30 30 2E 30 30 2E 30 30 2D 30 30 3A 30 30";

		commonDecodeThenEncode_Test( SpotMessageType.M3M5_ROMfs_PACKAGE_ACTIVATION, true, sMsg, "M3M5ROMfsPackageActivationRequest() " );
	}

	@Test
	public void vanguardROMfsPackageInfoResponse()
	{
		final String sCrt0 = getHex00toF7( );
		final String sCrt1 = getHex00toFF( );
		final String sCrt2 = sCrt0 + sCrt1;

		final String sFmt = "07 F2 83 00 60 03 00 07 " + "00 F8 %s 00 FF %s 00 F8 %s 00 FF %s 00 F8 %s 00 FF %s 01 F7 %s";

		final String sMsg = String.format( sFmt, sCrt0, sCrt1, sCrt0, sCrt1, sCrt0, sCrt1, sCrt2 );
		commonDecodeThenEncode_Test( SpotMessageType.VANGUARD_ROMfs_PACKAGE_INFO, false, sMsg, "vanguardROMfsPackageInfoResponse() " );
	}

	@Test
	public void vanguardROMfsPackageInfoRequest()
	{
		final String sMsg = "00 07 03 00 60 03 01";

		commonDecodeThenEncode_Test( SpotMessageType.VANGUARD_ROMfs_PACKAGE_INFO, true, sMsg, "vanguardPackageInfoRequest() " );
	}

	@Test
	public void M3M5ROMfsPackageInfoResponse()
	{
		final String sCert = getHex00toF7( );
		final String sSevenCertificates = sCert + sCert + sCert + sCert + sCert + sCert + sCert;
		final String sMsg = "06 D0 83 00 10 03 00 07 " + sSevenCertificates;

		commonDecodeThenEncode_Test( SpotMessageType.M3M5_ROMfs_PACKAGE_INFO, false, sMsg, "M3M5ROMfsPackageInfoResponse() " );
	}

	@Test
	public void M3M5ROMfsPackageInfoRequest()
	{
		final String sMsg = "00 07 03 00 10 03 01";

		commonDecodeThenEncode_Test( SpotMessageType.M3M5_ROMfs_PACKAGE_INFO, true, sMsg, "M3M5ROMfsPackageInfoRequest() " );
	}


	@Test
	public void vanguardROMfsPackageActivationRequest_EncodeOnly()
	{
		try
		{
			byte result[] = new byte[]
			{ 0x00, 0x16, 0x01, 0x00, 0x60, 0x04, 0x30, 0x30, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2D, 0x30, 0x30, 0x3A, 0x30, 0x30 };
			// List<Message> messages = spotProtocol.findChildElement(Message.class, SpotMessageType.M3M5_ROMfs_PACKAGE_INFO.getName());
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.VANGUARD_ROMfs_PACKAGE_ACTIVATION.getName( ), ContainerType.REQUEST );

			Message pkgActivationRequest = messages.get( 0 ).getWorkingCopy( );
			List<Field> lst = pkgActivationRequest.getProtocol( ).findChildElement( Field.class, "AppId" );
			lst.get( 0 ).setData( (byte) 0x01 );
			lst = pkgActivationRequest.getProtocol( ).findChildElement( Field.class, "ssk" );
			lst.get( 0 ).setData( (byte) 0x00 );
			lst = pkgActivationRequest.getProtocol( ).findChildElement( Field.class, "Time" );
			lst.get( 0 ).setData( new byte[] { 0x30, 0x30, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2D, 0x30, 0x30, 0x3A, 0x30, 0x30 } );
			byte[] encoded = pkgActivationRequest.encode( );

			System.out.println( "vanguardROMfsPackageActivationRequest_EncodeOnly() encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "vanguardROMfsPackageActivationRequest_EncodeOnly() result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void M3M5ROMfsPackageActivationRequest_EncodeOnly()
	{
		try
		{
			byte result[] = new byte[]
			{ 0x00, 0x16, 0x03, 0x00, 0x10, 0x04, 0x30, 0x30, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2D, 0x30, 0x30, 0x3A, 0x30, 0x30 };

			List<Message> messages = spotProtocol.findMessage( SpotMessageType.M3M5_ROMfs_PACKAGE_ACTIVATION.getName( ), ContainerType.REQUEST );

			Message pkgActivationRequest = messages.get( 0 ).getWorkingCopy( );
			List<Field> lst = pkgActivationRequest.getProtocol( ).findChildElement( Field.class, "AppId" );
			lst.get( 0 ).setData( (byte) 0x03 );
			lst = pkgActivationRequest.getProtocol( ).findChildElement( Field.class, "ssk" );
			lst.get( 0 ).setData( (byte) 0x00 );
			lst = pkgActivationRequest.getProtocol( ).findChildElement( Field.class, "Time" );
			lst.get( 0 ).setData( new byte[] { 0x30, 0x30, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2D, 0x30, 0x30, 0x3A, 0x30, 0x30 } );

			byte[] encoded = pkgActivationRequest.encode( );

			System.out.println( "M3M5ROMfsPackageActivationRequest_EncodeOnly() encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "M3M5ROMfsPackageActivationRequest_EncodeOnly() result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void vanguardROMfsPackageInfoRequest_EncodeOnly()
	{
		try
		{
			byte result[] = new byte[]
			{ 0x00, 0x07, 0x01, 0x00, 0x60, 0x03, 0x00 };
			// List<Message> messages = spotProtocol.findChildElement(Message.class, SpotMessageType.M3M5_ROMfs_PACKAGE_INFO.getName());
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.VANGUARD_ROMfs_PACKAGE_INFO.getName( ), ContainerType.REQUEST );

			Message pkgInfoRequest = messages.get( 0 ).getWorkingCopy( );
			List<Field> lst = pkgInfoRequest.getProtocol( ).findChildElement( Field.class, "AppId" );
			lst.get( 0 ).setData( (byte) 0x01 );
			lst = pkgInfoRequest.getProtocol( ).findChildElement( Field.class, "ssk" );
			lst.get( 0 ).setData( (byte) 0x00 );
			lst = pkgInfoRequest.getProtocol( ).findChildElement( Field.class, "Set" );
			lst.get( 0 ).setData( new byte[] { 0x00 } );

			byte[] encoded = pkgInfoRequest.encode( );

			System.out.println( "vanguardROMfsPackageInfoRequest_EncodeOnly() encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "vanguardROMfsPackageInfoRequest_EncodeOnly() result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void M3M5ROMfsPackageInfoRequest_EncodeOnly()
	{
		try
		{
			byte result[] = new byte[]
			{ 0x00, 0x07, 0x03, 0x00, 0x10, 0x03, 0x00 };
			// List<Message> messages = spotProtocol.findChildElement(Message.class, SpotMessageType.M3M5_ROMfs_PACKAGE_INFO.getName());
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.M3M5_ROMfs_PACKAGE_INFO.getName( ), ContainerType.REQUEST );

			Message pkgInfoRequest = messages.get( 0 ).getWorkingCopy( );
			List<Field> lst = pkgInfoRequest.getProtocol( ).findChildElement( Field.class, "AppId" );
			lst.get( 0 ).setData( (byte) 0x03 );
			lst = pkgInfoRequest.getProtocol( ).findChildElement( Field.class, "ssk" );
			lst.get( 0 ).setData( (byte) 0x00 );
			lst = pkgInfoRequest.getProtocol( ).findChildElement( Field.class, "Set" );
			lst.get( 0 ).setData( new byte[]
			{ 0x00 } );

			byte[] encoded = pkgInfoRequest.encode( );

			System.out.println( "M3M5ROMfsPackageInfoRequest_EncodeOnly() encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "M3M5ROMfsPackageInfoRequest_EncodeOnly() result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void systemStatus()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x07, 0x01, 0x00, 0x10, 0x01, 0x01 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.SYSTEM_STATUS.getName( ) );
			Message hideScreen = messages.get( 0 );

			Message workingCopy = hideScreen.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "Device" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x01 } );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "systemStatus encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "systemStatus result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void systemStatusResponse()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.SYSTEM_STATUS.getName( ), ContainerType.RESPONSE );
			Message workingCopy = messages.get( 0 ).getWorkingCopy( );

			byte[] data = EdtConvert.hexStringToBytes( "00 0C 81 00 10 01 00 01 01 04 00 00" );
			workingCopy.decode( data );
			System.out.println( workingCopy );
			byte[] encode = workingCopy.encode( );
			System.out.println( EdtConvert.bytesToHexString( data, 1 ) );
			System.out.println( EdtConvert.bytesToHexString( encode, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	/**
	 * Simulates a hardware information message "to be sent" to SPOT, having no information item requirement (it means all available items are requested). This message will be decoded (parsed)
	 * according to corresponding template in com.gilbarco.globaltools.spotprotocol.SpotProtocolTest.<b>spotProtocol</b> and then encoded again to retrieve a copy of the original message. Both
	 * original an copy messages should be the same.
	 * 
	 * @author Guillermo Paris
	 * 
	 */
	@Test
	public void hardwareAllInformation()
	{
		final byte result[];
		result = new byte[]
		{ 0x00, 0x07, 0x01, 0x00, 0x10, 0x02, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.HARDWARE_INFO.getName( ), ContainerType.REQUEST );
			Message hardwareInfo = messages.get( 0 );
			Message workingCopy = hardwareInfo.getWorkingCopy( );

			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ItemCount" );
			tmp.get( 0 ).setData( new byte[] { 0x00 } );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "hardwareAllInformation encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "hardwareAllInformation result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	/**
	 * Simulates a hardware information message "to be sent" to SPOT, having four info items. This message will be decoded (parsed) according to corresponding template in
	 * com.gilbarco.globaltools.spotprotocol.SpotProtocolTest.<b>spotProtocol</b> and then encoded again to retrieve a copy of the original message. Both original an copy messages should be the same.
	 * 
	 * @author Guillermo Paris
	 * 
	 */
	@Test
	public void hardwareInformation()
	{
		final byte result[];
		result = new byte[]
		{ 0x00, 0x0B, 0x01, 0x00, 0x10, 0x02, 0x04, 0x01, 0x10, 0x11, 0x42 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.HARDWARE_INFO.getName( ) );
			Message hardwareInfo = messages.get( 0 );

			Message workingCopy = hardwareInfo.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ItemCount" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x04 } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ItemIDs" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x01, 0x10, 0x11, 0x42 } );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "hardwareInformation encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "hardwareInformation result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );
		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	/**
	 * Simulates a hardware information message "received" from SPOT, having one info item. This message will be decoded (parsed) according to corresponding template in
	 * com.gilbarco.globaltools.spotprotocol.SpotProtocolTest.<b>spotProtocol</b> and then encoded again to retrieve a copy of the original message. Both original an copy messages should be the same.
	 * 
	 * @author Guillermo Paris
	 * 
	 */
	@Test
	public void hardwareInformation_OneItemResponse()
	{

		commonDecodeThenEncode_Test( SpotMessageType.HARDWARE_INFO, false, "00 1A 81 00 10 02 00 01 01 10 " + strHex00to0F, "hardwareInformation_OneItemResponse() " );
	}

	/**
	 * Simulates a hardware information message "received" from SPOT, having four info items. This message will be decoded (parsed) according to corresponding template in
	 * com.gilbarco.globaltools.spotprotocol.SpotProtocolTest.<b>spotProtocol</b> and then encoded again to retrieve a copy of the original message. Both original an copy messages should be the same.
	 * 
	 * @author Guillermo Paris
	 * 
	 */
	@Test
	public void hardwareInformation_FourItemResponse()
	{

		commonDecodeThenEncode_Test( SpotMessageType.HARDWARE_INFO, false, "00 42 81 00 10 02 00 04 01 10" + strHex00to0F + // main board SN
																			"10 10 " + strHex10to1F + // device SN
																			"11 02 41 33 " + // hardware release ID (4133 = Dallas 5250)
																			"42 0D 4D 6F 64 65 6C 20 44 65 73 63 72 69 70", // "Model Descrip"
			"hardwareInformation_FourItemResponse() " );
	}

	/*
	@Test
	public void softwareAllInformation() {
		final byte result[];
		result = new byte[] { 0x00, 0x07, 0x01, 0x00, 0x10, 0x03, 0x00};
		try {
			List<Message> messages = spotProtocol.findChildElement(Message.class, SpotMessageType.SOFTWARE_INFO.getName());
			Message softwareInfo = messages.get(0);

			Message workingCopy = softwareInfo.getWorkingCopy();
			List<Field> tmp = workingCopy.getProtocol().findChildElement(Field.class, "AppId");
			tmp.get(0).setData((byte) 0x01);
			tmp = workingCopy.getProtocol().findChildElement(Field.class, "ssk");
			tmp.get(0).setData((byte) 0x00);
			tmp = workingCopy.getProtocol().findChildElement(Field.class, "ItemCount");
			tmp.get(0).setData(new byte[] { 0x00 });

			byte[] encoded = workingCopy.encode();
			System.out.println("softwareAllInformation encode: " + EdtConvert.bytesToHexString(encoded, 1));
			System.out.println("softwareAllInformation result: " + EdtConvert.bytesToHexString(result, 1));
			Assert.assertTrue(java.util.Arrays.equals(encoded, result));
		}
		catch (MessageDataException e) {
			e.printStackTrace();
			Assert.fail(e.getMessage());
		}
	}
*/


	/**Simulates a software information message "to be sent" to SPOT, having seven info item request.
	 * This message will be decoded (parsed) according to corresponding template in
	 * com.gilbarco.globaltools.spotprotocol.SpotProtocolTest.<b>spotProtocol</b>
	 * and then encoded again to retrieve a copy of the original message. Both original an copy messages should be the same. 
	 * @author Guillermo Paris
	 *
	 */
/*
	@Test
	public void softwareInformation() {
		final byte result[];
		result = new byte[] { 0x00, 0x0E, 0x01, 0x00, 0x10, 0x03, 0x07, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x09};
		try {
			List<Message> messages = spotProtocol.findChildElement(Message.class, SpotMessageType.SOFTWARE_INFO.getName());
			Message softwareInfo = messages.get(0);

			Message workingCopy = softwareInfo.getWorkingCopy();
			List<Field> tmp = workingCopy.getProtocol().findChildElement(Field.class, "AppId");
			tmp.get(0).setData((byte) 0x01);
			tmp = workingCopy.getProtocol().findChildElement(Field.class, "ssk");
			tmp.get(0).setData((byte) 0x00);
			tmp = workingCopy.getProtocol().findChildElement(Field.class, "ItemCount");
			tmp.get(0).setData(new byte[] { 0x07 });
			tmp = workingCopy.getProtocol().findChildElement(Field.class, "ItemIDs");
			tmp.get(0).setData(new byte[] { 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x09 } );

			byte[] encoded = workingCopy.encode();
			System.out.println("softwareInformation encode: " + EdtConvert.bytesToHexString(encoded, 1));
			System.out.println("softwareInformation result: " + EdtConvert.bytesToHexString(result, 1));
			Assert.assertTrue(java.util.Arrays.equals(encoded, result));
		}
		catch (MessageDataException e) {
			e.printStackTrace();
			Assert.fail(e.getMessage());
		}
	}
*/

	/**Simulates a software information message "received" from SPOT, having one info item.
	 * This message will be decoded (parsed) according to corresponding template in
	 * com.gilbarco.globaltools.spotprotocol.SpotProtocolTest.<b>spotProtocol</b>
	 * and then encoded again to retrieve a copy of the original message. Both original an copy messages should be the same. 
	 * @author Guillermo Paris
	 *
	 */
/*
	@Test
	public void softwareInformation_OneItemResponse()
	{
		commonDecodeThenEncode_Test(
			SpotMessageType.SOFTWARE_INFO,
			false,
			"00 2A 81 00 10 03 00 01 02 20 " + strHex00to0F + strHex10to1F, // SPOT OS Version
			"softwareInformation_OneItemResponse() "
		);
	}
*/

	/**Simulates a hardware information message "received" from SPOT, having seven info items.
	 * This message will be decoded (parsed) according to corresponding template in
	 * com.gilbarco.globaltools.spotprotocol.SpotProtocolTest.<b>spotProtocol</b>
	 * and then encoded again to retrieve a copy of the original message. Both original an copy messages should be the same. 
	 * @author Guillermo Paris
	 *
	 */
/*
	@Test
	public void softwareInformation_SevenItemResponse()
	{
		commonDecodeThenEncode_Test(
			SpotMessageType.SOFTWARE_INFO,
			false,
			"00 F6 81 00 10 03 00 07 02 20 " +
			strHex00to0F + strHex00to0F + // SPOT OS Version
			"03 20 " + strHex10to1F + strHex10to1F + // ROMFS1 Version
			"04 20 " + strHex20to2F + strHex20to2F + // ROMFS2 Version
			"05 20 " + strHex30to3F + strHex30to3F + // ROMFS3 Version
			"06 20 " + strHex40to4F + strHex40to4F + // ROMFS4 Version
			"07 20 " + strHex50to5F + strHex50to5F + // ROMFS5 Version
			"09 20 " + strHex70to7F + strHex70to7F , // ROMFS7 Version
			"softwareInformation_SevenItemResponse() "
		);
	}

	@Test
	public void softwareInformation_RealWorldResponse()
	{
		commonDecodeThenEncode_Test(
				SpotMessageType.SOFTWARE_INFO,
				false,
				"018281001003000D02204F532030312E3033000000000000000000000000000000000000000000000000032050494E5041442030312E303600000000000000000000000000000000000000000420454D562030312E3035000000000000000000000000000000000000000000000005204F50542030312E30330000000000000000000000000000000000000000000000062053455455502030312E303500000000000000000000000000000000000000000007204756524F5054454D562030312E303600000000000000000000000000000000000820000000000000000000000000000000000000000000000000000000000000000009204352494E4442494F532030312E303100000000000000000000000000000000000A2000000000000000000000000000000000000000000000000000000000000000000D10445930313738000000000000000000000C104B4230313732000000000000000000000B1052443031322D3130303230345F3100000E1000000000000000000000000000000000",
				"softwareInformation_RealWorldResponse() "
			);
	}
*/

	@Test
	public void contactlessDisable()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x06, 0x04, 0x00, 0x50, 0x05 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.DISABLE_CL_READER.getName( ) );
			Message hideScreen = messages.get( 0 );

			Message workingCopy = hideScreen.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x04 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "disableCardReader encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "disableCardReader result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void contactLessResult()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.DISABLE_CL_READER.getName( ), ContainerType.RESPONSE );
			Message workingCopy = messages.get( 0 ).getWorkingCopy( );

			byte[] data = EdtConvert.hexStringToBytes( "00 07 84 00 50 05 00" );
			workingCopy.decode( data );
			System.out.println( workingCopy );
			byte[] encode = workingCopy.encode( );
			System.out.println( EdtConvert.bytesToHexString( data, 1 ) );
			System.out.println( EdtConvert.bytesToHexString( encode, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void enableContactlessCardReader()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x0E, 0x04, 0x00, 0x50, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.ENABLE_CL_READER.getName( ) );
			Message hideScreen = messages.get( 0 );

			Message workingCopy = hideScreen.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x04 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ApplicationMask" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00 } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "Amount" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "enableCardReader encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "enableCardReader result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void ContactlessData()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( "ContactlessData", ContainerType.RESPONSE );
			Message workingCopy = messages.get( 0 ).getWorkingCopy( );

			byte[] data = EdtConvert.hexStringToBytes( "00 07 84 00 50 04 00 00 01 01 00 02 01 02" );
			workingCopy.decode( data );
			System.out.println( workingCopy );
			byte[] encode = workingCopy.encode( );
			System.out.println( EdtConvert.bytesToHexString( data, 1 ) );
			System.out.println( EdtConvert.bytesToHexString( encode, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void OTIGetCommand()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x0F, 0x04, 0x00, 0x50, 0x07, (byte) 0x9F, (byte) 0x1A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.OTI_GET.getName( ) );
			Message hideScreen = messages.get( 0 );

			Message workingCopy = hideScreen.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x04 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "TagData" );
			tmp.get( 0 ).setData( new byte[]
			{ (byte) 0x9F, (byte) 0x1A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "enableCardReader encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "enableCardReader result: " + EdtConvert.bytesToHexString( result, 1 ) );
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

			Message decodingCopy = hideScreen.getWorkingCopy( );
			decodingCopy.decode( result );
			Assert.assertTrue( workingCopy.equals( decodingCopy ) );
		}
		catch (MessageDataException e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void OTIGetCommandResponse()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.OTI_GET.getName( ), ContainerType.RESPONSE );
			Message workingCopy = messages.get( 0 ).getWorkingCopy( );

			byte[] data = EdtConvert.hexStringToBytes( "00 07 84 00 50 07 00 00 01 01" );
			workingCopy.decode( data );
			System.out.println( workingCopy );
			byte[] encode = workingCopy.encode( );
			System.out.println( EdtConvert.bytesToHexString( data, 1 ) );
			System.out.println( EdtConvert.bytesToHexString( encode, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void commonDecodeThenEncode_Test(SpotMessageType msgType, boolean blRequest, String sMsgLayoutData, String sInvoker)
	{
		ContainerType conType = ContainerType.RESPONSE;

		if (blRequest) // Request message
		{
			conType = ContainerType.REQUEST;
		}

		try
		{
			List<Message> messages = spotProtocol.findMessage( msgType.getName( ), conType );
			Message msgCopy = messages.get( 0 ).getWorkingCopy( );

			byte abData[] = EdtConvert.hexStringToBytes( sMsgLayoutData );
			msgCopy.decode( abData );
/*			System.out.println( "Fields of the message" );

			for (ChildElement child : msgCopy.getChildren())
			{
				if(Field.class.isInstance(child))
					System.out.println ("field found: " + child.getName( ));	

				if(Container.class.isInstance(child))
				{
					System.out.println ("Container found: " + child.getName( ));

					@SuppressWarnings("unchecked")
					ArrayContainer<Container> itemArray = (ArrayContainer<Container>) child;

					System.out.println ("Item array: "+ itemArray.getName( ));
					System.out.println ("Item tostring: "+ itemArray.toString( ));
					Collection<ChildElement> col = itemArray.getChildren( );

					for (ChildElement childElement : col)
					{
						System.out.println ("Item tostring: ----> \n");
						System.out.println (childElement.toString( ));
						if( childElement instanceof Container){
							
							Collection<Field> colItemID; 
							
							if(msgType.equals(SpotMessageType.VANGUARD_ROMfs_PACKAGE_INFO.getName( )) )
							{
								colItemID = ((Container) childElement).findChildElement( "PackageArray" );
							}
							else 
							{
								colItemID = ((Container) childElement).findChildElement( "ItemID" );	
							}
							
							
							Field f = (Field) colItemID.toArray( )[0]; 
							if ( f != null )
							{
								System.out.println("ItemID = "+  f.getFormattedData( ));
								
							}
							else{
								System.out.println("ItemID = null ");
							}

							colItemID= null;
							f = null;

							if(msgType.equals(SpotMessageType.VANGUARD_ROMfs_PACKAGE_INFO.getName( )) )
							{
								colItemID = ((Container) childElement).findChildElement( "Certificate" );
							}
							else 
							{
								colItemID = ((Container) childElement).findChildElement( "ItemData" );
							}

							f = (Field) colItemID.toArray( )[0]; 
							if ( f != null )
							{
								System.out.println("ItemData = "+  f.getFormattedData( ));
								System.out.println("ItemData = "+ new String( f.getData( ), "US-ASCII")); 
							}
							else{
								System.out.println("ItemData = null ");
							}
						}
					}
				}
			}
*/

			// System.out.println(msgCopy);
			byte[] encode = msgCopy.encode( );
			System.out.println( sInvoker + " encode: " + EdtConvert.bytesToHexString( encode, 1 ) );
			System.out.println( sInvoker + " result: " + EdtConvert.bytesToHexString( abData, 1 ) );
			Assert.assertTrue( Arrays.equals( abData, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}
	

	
	@Test
	public void maintenanceTest()
	{
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.VANGUARD_MAINTENANCE.getName( ) );
			Message ping = messages.get( 0 );

			// Filling the fields
			Message workingCopy = ping.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );

			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "TokenId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			
			byte[] dataXmlToken = EdtConvert.hexStringToBytes( "3C3F786D6C2076657273696F6E3D22312E302220656E636F64696E673D225554462D38223F3E0A3C56616E6775617264546F6B656E50726F746F636F6C20786D6C6E733A7873693D22687474703A2F2F7777772E67696C626172636F2E636F6D2F56616E6775617264546F6B656E50726F746F636F6C536368656D6122207873693A6E6F4E616D657370616365536368656D614C6F636174696F6E3D2256616E6775617264546F6B656E50726F746F636F6C2E787364223E0A20203C436F6D6D616E643E546F6B656E526573706F6E73653C2F436F6D6D616E643E0A20203C426F64793E0A202020203C56657273696F6E3E303030312E303030312E303030313C2F56657273696F6E3E0A202020203C546F6B656E436F64653E3737494820564A453320554658552059454E5520375245572055324859205946564A204537534B3C2F546F6B656E436F64653E0A202020203C547970653E5465726D696E616E4D61696E74656E616E63653C2F547970653E0A202020203C5374617274696E6754696D653E0A2020202020203C446174653E31322D33312D323031323C2F446174653E0A2020202020203C54696D653E30303A30303A30303C2F54696D653E0A202020203C2F5374617274696E6754696D653E0A202020203C45787069726174696F6E3E0A2020202020203C446174653E31322D33312D323031323C2F446174653E0A2020202020203C54696D653E32333A35393A35393C2F54696D653E0A202020203C2F45787069726174696F6E3E0A202020203C53657373696F6E3E0A2020202020203C4D61784475726174696F6E3E3630303C2F4D61784475726174696F6E3E0A202020203C2F53657373696F6E3E0A202020203C5465726D696E616C3E0A2020202020203C50504E3E3030303030303131313131313C2F50504E3E0A202020203C2F5465726D696E616C3E0A202020203C4F7074696F6E616C446174613E30303030303030303030303030303030303030303030303030303030303030303C2F4F7074696F6E616C446174613E0A20203C2F426F64793E0A20203C582E3530392D43657274696669636174653E0A202020203C5369676E61747572653E36613A37393A65323A61653A38613A34633A63643A66353A30303A62643A39613A37313A63313A39343A30643A61643A61653A63363A31353A35303A35393A65323A35343A35663A31623A37343A64353A36653A39643A66373A62383A31653A39633A61363A61653A32353A66633A65363A34393A36613A64653A65323A61303A35373A38623A37353A32363A33333A37323A37643A35323A61343A30653A65663A66383A62303A65373A39303A30613A37393A39663A30633A64663A36383A32373A34313A66343A61323A61643A32343A62633A35353A38383A36653A31313A65373A34633A36383A36653A37653A38663A38333A33343A35363A37643A64303A62633A63363A64663A33333A30343A31303A36653A31623A66353A62363A36393A31393A38663A36373A35633A63363A65323A35323A33303A62323A65353A38643A36373A32623A30663A36343A62623A30363A35643A65653A62383A32643A61393A33303A31623A39373A61373A32623A36323A65363A62643A65663A34313A31313A36393A33653A66363A63353A63643A65613A37323A32333A36633A62623A63663A37313A34343A64363A32333A35303A34653A34643A33333A35353A33303A62643A63663A65393A64343A61393A62393A30353A65633A39353A34313A66623A61653A36393A34303A33383A63343A61623A65653A66303A62653A32333A61313A36303A62343A35303A61613A34623A62333A64313A61363A31323A30353A39323A39303A35633A32303A63373A64633A37313A38613A39373A35363A63323A38323A38623A32643A63303A64383A62353A36323A65653A62343A33663A37663A64383A64333A63303A62383A31363A63313A63643A62613A31383A34663A30383A66643A39373A34663A63383A31393A61363A34343A61353A36633A61663A63393A38383A62323A39353A30323A36343A37353A64363A65303A31303A61623A64313A66653A39323A30313A35343A65383A64303A36383A32663A30633A37333A66633A61373A33313A31363A39343A30393A61623A33353A63333A35393A30343A34373A31383A62613A62323A33323A35353A61633A31363A65393A65373A66643A33663A34393A64383A64393A39663A39623A39363A64313A66393A37373A30643A39353A35323A65323A38353A37353A65613A65393A39623A30613A37303A31393A36653A36393A65313A36633A32373A38393A37643A61623A64623A66343A35343A63353C2F5369676E61747572653E0A20203C2F582E3530392D43657274696669636174653E0A3C2F56616E6775617264546F6B656E50726F746F636F6C3E0A" );
			
			System.out.println( "dataXmlToken: " + EdtConvert.bytesToHexString( dataXmlToken ) );
			
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "TokenData" );
			
			tmp.get( 0 ).setData( dataXmlToken);
			
			byte[] encoded = workingCopy.encode( );
			
			System.out.println( "message encoded: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			workingCopy.decode( encoded );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void FileUploadRequest()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x05, 0x13, 0x01 };
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.FILE_UPLOAD.getName( ), ContainerType.REQUEST );
			Message workingCopy = messages.get( 0 ).getWorkingCopy( );

			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileType" );
			tmp.get( 0 ).setData( (byte) 0x13 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			
			System.out.println( "File upload request decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "File upload request expected: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( java.util.Arrays.equals( workingCopy.getData( ), result ) );

		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void FileBlockUploadRequest()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x06, 0x00, 0x00, 0x00, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.FILE_BLOCK_UPLOAD.getName( ), ContainerType.REQUEST );
			Message workingCopy = messages.get( 0 ).getWorkingCopy( );

			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "BlkOffset" );
			tmp.get( 0 ).setData( new byte[] { 0x00, 0x00, 0x00, 0x00 } );
			
			System.out.println( "File block upload request decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "File block upload request expected: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( java.util.Arrays.equals( workingCopy.getData( ), result ) );

		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void FileUploadResponse()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00 };
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.FILE_UPLOAD.getName( ), ContainerType.RESPONSE );
			Message workingCopy = messages.get( 0 ).getWorkingCopy( );

			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x81 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AckCode" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "FileSz" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x00 } );
			
			System.out.println( "File upload response decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "File upload response expected: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( java.util.Arrays.equals( workingCopy.getData( ), result ) );

		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void FileBlockUploadResponse()
	{
		final byte[] result = new byte[]
		{ 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6E,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.FILE_BLOCK_UPLOAD.getName( ), ContainerType.RESPONSE );
			Message workingCopy = messages.get( 0 ).getWorkingCopy( );

			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x81 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AckCode" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "Offset" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x00 } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "BlkSize" );
			tmp.get( 0 ).setData( new byte[]
			{ 0x00, 0x00, 0x00, 0x6E } );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "BlkData" );
			tmp.get( 0 ).setData(
				new byte[]
				{
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11,
								0x11 } );
			
			System.out.println( "File block upload response decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "File block upload response expected: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( java.util.Arrays.equals( workingCopy.getData( ), result ) );

		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}
	
	@Test
	public void KeyStatusRequest()
	{
		final byte[] result = new byte[] { 0x40, 0x06 };
		final byte[] resultEncoded = new byte[] { 0x00, 0x06, 0x01, 0x00, 0x40, 0x06 };
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.KEY_STATUS.getName( ), ContainerType.REQUEST );
			Message workingCopy = messages.get( 0 ).getWorkingCopy( );

			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 ); //pinpad
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 ); //no encryption
			
			System.out.println( "KeyStatusRequest decode: " + EdtConvert.bytesToHexString( workingCopy.getData( ), 1 ) );
			System.out.println( "KeyStatusRequest expected: " + EdtConvert.bytesToHexString( result, 1 ) );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "KeyStatusRequest encoded: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "KeyStatusRequest encoded expected: " + EdtConvert.bytesToHexString( resultEncoded, 1 ) );
			workingCopy.decode( encoded );			
			Assert.assertTrue( java.util.Arrays.equals( encoded, resultEncoded ) );

		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void KeyStatusResponse()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.KEY_STATUS.getName( ), ContainerType.RESPONSE );
			Message keystatus = messages.get( 0 ).getWorkingCopy( );

			byte[] data = EdtConvert.hexStringToBytes( "00 67 81 00 40 06 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00" );
			keystatus.decode( data );
			byte[] encode = keystatus.encode( );
			System.out.println( "KeyStatusResponse encoded:" + EdtConvert.bytesToHexString( encode, 1 ) );
			System.out.println( "KeyStatusResponse expected: "+ EdtConvert.bytesToHexString( data, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}
	
	@Test
	public void ApplicationKeyDownloadRequest()
	{
		final byte[] result = new byte[] { 0x00, 0x12, 0x01, 0x00, 0x40, 0x04, 0x20, 0x01, (byte) 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x21, 0x44, (byte)0xDF, 0x1C };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.APPLICATION_KEY_DOWNLOAD.getName( ) );
			Message hideScreen = messages.get( 0 );

			Message workingCopy = hideScreen.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x01 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "KeyId" );
			tmp.get( 0 ).setData( (byte) 0x20 ); //slot
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "KeyUse" );
			tmp.get( 0 ).setData( (byte) 0x01 ); //pin
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "TransportKeyId" );
			tmp.get( 0 ).setData( (byte) 0xFF ); //..
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "KeyType" );
			tmp.get( 0 ).setData( (byte) 0x03 ); //rsa
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "KeyData" );
			tmp.get( 0 ).setData( new byte[] { (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x21, (byte) 0x44, (byte)0xDF, (byte) 0x1C } );

			byte[] encoded = workingCopy.encode( );
			System.out.println( "ApplicationKeyDownloadRequest encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "ApplicationKeyDownloadRequest result: " + EdtConvert.bytesToHexString( result, 1 ) );
			
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

			Message decodingCopy = hideScreen.getWorkingCopy( );
			decodingCopy.decode( result );
			Assert.assertTrue( workingCopy.equals( decodingCopy ) );
		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}
	
	@Test
	public void ApplicationKeyDownloadResponse()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.APPLICATION_KEY_DOWNLOAD.getName( ), ContainerType.RESPONSE );
			Message keydownload = messages.get( 0 ).getWorkingCopy( );

			byte[] data = EdtConvert.hexStringToBytes( "00 06 81 00 40 04 00 20" );
			keydownload.decode( data );
			byte[] encode = keydownload.encode( );
			System.out.println( "ApplicationKeyDownloadResponse encoded:" + EdtConvert.bytesToHexString( encode, 1 ) );
			System.out.println( "ApplicationKeyDownloadResponse expected: "+ EdtConvert.bytesToHexString( data, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void serviceMenuSwitchRequest()
	{		
		final byte[] result = new byte[] { 0x00, 0x06, 0x00, 0x00, 0x0D, 0x01 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.SERVICE_MENU_SWITCH.getName( ) );
			Message serviceMenuSwitch = messages.get( 0 );

			Message workingCopy = serviceMenuSwitch.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "SwitchType" );
			tmp.get( 0 ).setData( (byte) 0x01 ); //activate

			byte[] encoded = workingCopy.encode( );
			System.out.println( "ServiceMenuSwitchRequest encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "ServiceMenuSwitchRequest result: " + EdtConvert.bytesToHexString( result, 1 ) );
			
			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

			Message decodingCopy = serviceMenuSwitch.getWorkingCopy( );
			decodingCopy.decode( result );
			Assert.assertTrue( workingCopy.equals( decodingCopy ) );
		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void serviceMenuSwitchResponse()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.SERVICE_MENU_SWITCH.getName( ), ContainerType.RESPONSE );
			Message serviceMenuSwitchResponse = messages.get( 0 ).getWorkingCopy( );

			byte[] data = EdtConvert.hexStringToBytes( "00 06 80 00 0D 01" );
			serviceMenuSwitchResponse.decode( data );
			byte[] encode = serviceMenuSwitchResponse.encode( );
			System.out.println( "ServiceMenuSwitchResponse encoded:" + EdtConvert.bytesToHexString( encode, 1 ) );
			System.out.println( "ServiceMenuSwitchResponse expected: "+ EdtConvert.bytesToHexString( data, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void getChallengeRequestTest()
	{
		// 00 0E “Technician XXX”
		final byte[] result = new byte[] { 0x00, 0x09, 0x00, 0x00, 0x0E, 0x03, 0x49, 0x50, 0x51 };
		try
		{
			List<Message> messages = spotProtocol.findChildElement( Message.class, SpotMessageType.GET_CHALLENGE.getName( ) );
			Message serviceMenuSwitch = messages.get( 0 );

			Message workingCopy = serviceMenuSwitch.getWorkingCopy( );
			List<Field> tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "AppId" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "ssk" );
			tmp.get( 0 ).setData( (byte) 0x00 );
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "TechIdLen" );
			tmp.get( 0 ).setData( (byte) 0x03 ); // length
			tmp = workingCopy.getProtocol( ).findChildElement( Field.class, "TechnicianId" );
			tmp.get( 0 ).setData( new byte[] { (byte) 0x49, (byte) 0x50, (byte) 0x51 } ); // techinician id

			byte[] encoded = workingCopy.encode( );
			System.out.println( "GetChallengeRequest encode: " + EdtConvert.bytesToHexString( encoded, 1 ) );
			System.out.println( "GetChallengeRequest result: " + EdtConvert.bytesToHexString( result, 1 ) );

			Assert.assertTrue( java.util.Arrays.equals( encoded, result ) );

			Message decodingCopy = serviceMenuSwitch.getWorkingCopy( );
			decodingCopy.decode( result );
			Assert.assertTrue( workingCopy.equals( decodingCopy ) );
		}
		catch (MessageDataException e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	@Test
	public void getChallengeResponseTest()
	{
		try
		{
			List<Message> messages = spotProtocol.findMessage( SpotMessageType.GET_CHALLENGE.getName( ), ContainerType.RESPONSE );
			Message serviceMenuSwitchResponse = messages.get( 0 ).getWorkingCopy( );

			// 80 0E RESULT_CODE <SERIAL NUMBER> <UNIT CHALLENGE>
			byte[] data = EdtConvert.hexStringToBytes( "00 24 80 00 0E 00 50 50 50 50 50 50 50 50 49 49 49 49 49 49 49 49 49 49" );
			serviceMenuSwitchResponse.decode( data );
			byte[] encode = serviceMenuSwitchResponse.encode( );
			System.out.println( "GetChallengeResponse encoded:" + EdtConvert.bytesToHexString( encode, 1 ) );
			System.out.println( "GetChallengeResponse expected: "+ EdtConvert.bytesToHexString( data, 1 ) );
			Assert.assertTrue( Arrays.equals( data, encode ) );
		}
		catch (Exception e)
		{
			e.printStackTrace( );
			Assert.fail( e.getMessage( ) );
		}
	}


	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
