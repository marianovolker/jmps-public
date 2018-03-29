/**
 * @author Guillermo M. Paris
 * @date 24/4/2012
 */

package com.gilbarco.globaltools.flexpay.token.gui;

import java.io.UnsupportedEncodingException;


public class ExampleDataSupplier
{

	// ******************************************************************
	// INNER TYPES
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	private static String[] aExampleContries = { "US", "AR", "BR", "UY" };

	private static String[]	aExampleStates = {"NC","NY","FL","BA","SF"};

	private static String[]	aExampleLocalities =
	{
		"Greensboro","Raleigh","Miami","New York","Buenos Aires","Rosario"
	};

	protected static final String[] aFuelPositions =
	{
		"01", "02", "03", "04", "05", "06", "07", "08", "09", "10",
		"11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
		"21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
		"31"
	};

	private static String[] aExampleLocations =
	{
		"Buttock Hill, LO",
		"Pinehurst, NC",
		"Greensboro, NC",
		"Miami, FL"
	};

	private static String[] aExampleSites =
	{
		"Pinehurst, SC",
		"Pinehurst, NC",
		"Pop Shope, GSO",
		"Pet Shop, BOY",
		"UGas St. Louis"
	};

	private static String[][] aaExampleUnitTokens =
	{
		{ "01234567890123456789","Terminal Diagnostics","15731202","10.5.133.57","Pinehurst SS","8","GSO NC","03/09/2012 8:06 AM" },
		{ "56473829105647382910","Software Update","01621241","10.5.166.128","PopShoppe A7","1","GSO NC","01/25/2012 9:12 AM" },
		{ "98765432109876543210","Software Update","14033049","10.5.166.130","PopShoppe A7","3","GSO NC","03/09/2012 5:27 PM" }
	};

	private static String[][] aaExampleTargetUnits =
	{
		{ "Pinehurst SS", "07", "NC", "10.5.133.57", "Terminal Maintenance", "03/09/2012 8:00 AM" },
		{ "PopShoppe", "10", "GSO NC", "10.5.133.57", "Terminal Maintenance", "03/08/2012 9:00 AM" },
		{ "PopShoppe 2", "12", "GSO NC", "10.5.133.57", "Software Update", "03/09/2012 5:00 PM" },
		{ "Pinehurst SS", "07", "NC", "10.5.133.57", "Terminal Maintenance", "03/09/2012 8:00 AM" },
		{ "PopShoppe", "10", "GSO NC", "10.5.133.57", "Terminal Maintenance", "03/08/2012 9:00 AM" },
		{ "PopShoppe 2", "12", "GSO NC", "10.5.133.57", "Software Update", "03/09/2012 5:00 PM" },
		{ "Pinehurst SS", "07", "NC", "10.5.133.57", "Terminal Maintenance", "03/09/2012 8:00 AM" },
		{ "PopShoppe", "10", "GSO NC", "10.5.133.57", "Terminal Maintenance", "03/08/2012 9:00 AM" },
		{ "PopShoppe 2", "12", "GSO NC", "10.5.133.57", "Software Update", "03/09/2012 17:00 AM" },
		{ "Pinehurst SS", "07", "NC", "10.5.133.57", "Terminal Maintenance","03/09/2012 8:00 AM" },
		{ "PopShoppe", "10", "GSO NC", "10.5.133.57", "Terminal Maintenance", "03/08/2012 9:00 AM" },
		{ "PopShoppe 2", "12", "GSO NC", "10.5.133.57", "Software Update", "03/09/2012 5:00 PM" },
		{ "Pinehurst SS", "07", "NC", "10.5.133.57", "Terminal Maintenance", "03/09/2012 8:00 AM" },
		{ "PopShoppe", "10", "GSO NC", "10.5.133.57", "Terminal Maintenance","03/08/2012 9:00 AM" },
		{ "PopShoppe 2", "12", "GSO NC", "10.5.133.57", "Software Update", "03/09/2012 5:00 PM" },
		{ "UGas St. Louis", "07", "MO", "10.5.147.101", "Terminal Maintenance", "02/29/2012 8:10 PM" }
	};

	private static String  strTokenExample =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
		"<VanguardTokenProtocol xmlns:xsi=\"http://www.gilbarco.com/VanguardTokenProtocolSchema\" xsi:noNamespaceSchemaLocation=\"VanguardTokenProtocol.xsd\">\n" +
		"<Command>Token</Command>\n" +
		"	<Body>\n" +
		"		<Id>01234567890123456789</Id>\n" +
		"		<Version>0001.0001.0020</Version>\n" +
		"		<Type>TerminalDiagnostics</Type>\n" +
		"		<Expiration>\n" +
		"			<Date>03-09-2012</Date>\n" +
		"			<Time>08:06:00.00</Time>\n" +
		"		</Expiration>\n" +
		"		<Session>\n" +
		"			<MaxDuration>600</MaxDuration>\n" +
		"		</Session>\n" +
		"		<Terminal>\n" +
		"			<PPN>0000000000000000000000000000000015731202</PPN>\n" +
		"		</Terminal>\n" +
		"		<OptionalData>00000000000000000000000000000000</OptionalData>\n" +
		"		<X.509-Certificate>\n" +
		"			<Signature>93:5f:8f:5f:c5:af:bf:0a:ab:a5:6d:fb:24:5f:b6:59:5d:9d:92:2e:4a:1b:8b:ac:7d:99:17:5d:cd:19:f6:ad:ef:63:2f:92:ab:2f:4b:cf:0a:13:90:ee:2c:0e:43:03:be:f6:ea:8e:9c:67:d0:a2:40:03:f7:ef:6a:15:09:79:a9:46:ed:b7:16:1b:41:72:0d:19:aa:ad:dd:9a:df:ab:97:50:65:f5:5e:85:a6:ef:19:d1:5a:de:9d:ea:63:cd:cb:cc:6d:5d:01:85:b5:6d:c8:f3:d9:f7:8f:0e:fc:ba:1f:34:e9:96:6e:6c:cf:f2:ef:9b:bf:de:b5:22:68:9f:97:50:65:f5:5e:85:a6:ef:19:d1:5a:de:9d:ea:63:cd:ef:63:2f:92:ab:2f:4b:cf:0a:13:90:ee:2c:0e:43:03:93:5f:8f:5f:c5:af:bf:0a:ab:a5:6d:fb:24:5f:b6:59:1f:34:e9:96:6e:6c:cf:f2:ef:9b:bf:de:b5:22:68:9f:be:f6:ea:8e:9c:67:d0:a2:40:03:f7:ef:6a:15:09:79:cb:cc:6d:5d:01:85:b5:6d:c8:f3:d9:f7:8f:0e:fc:ba:5d:9d:92:2e:4a:1b:8b:ac:7d:99:17:5d:cd:19:f6:ad:a9:46:ed:b7:16:1b:41:72:0d:19:aa:ad:dd:9a:df:ab</Signature>\n" +
		"		</X.509-Certificate>\n" +
		"	</Body>\n" +
		"</VanguardTokenProtocol>\n\n";

	private static String  strTokenExample1 =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
		"<VanguardTokenProtocol xmlns:xsi=\"http://www.gilbarco.com/VanguardTokenProtocolSchema\" xsi:noNamespaceSchemaLocation=\"VanguardTokenProtocol.xsd\">\n" +
		"<Command>Token</Command>\n" +
		"	<Body>\n" +
		"		<Id>56473829105647382910</Id>\n" +
		"		<Version>0001.0001.0020</Version>\n" +
		"		<Type>Software Update</Type>\n" +
		"		<Expiration>\n" +
		"			<Date>01-25-2012</Date>\n" +
		"			<Time>09:12:00.00</Time>\n" +
		"		</Expiration>\n" +
		"		<Session>\n" +
		"			<MaxDuration>600</MaxDuration>\n" +
		"		</Session>\n" +
		"		<Terminal>\n" +
		"			<PPN>0000000000000000000000000000000001621241</PPN>\n" +
		"		</Terminal>\n" +
		"		<OptionalData>00000000000000000000000000000000</OptionalData>\n" +
		"		<X.509-Certificate>\n" +
		"			<Signature>93:5f:8f:5f:c5:af:bf:0a:ab:a5:6d:fb:24:5f:b6:59:5d:9d:92:2e:4a:1b:8b:ac:7d:99:17:5d:cd:19:f6:ad:ef:63:2f:92:ab:2f:4b:cf:0a:13:90:ee:2c:0e:43:03:be:f6:ea:8e:9c:67:d0:a2:40:03:f7:ef:6a:15:09:79:a9:46:ed:b7:16:1b:41:72:0d:19:aa:ad:dd:9a:df:ab:97:50:65:f5:5e:85:a6:ef:19:d1:5a:de:9d:ea:63:cd:cb:cc:6d:5d:01:85:b5:6d:c8:f3:d9:f7:8f:0e:fc:ba:1f:34:e9:96:6e:6c:cf:f2:ef:9b:bf:de:b5:22:68:9f:97:50:65:f5:5e:85:a6:ef:19:d1:5a:de:9d:ea:63:cd:ef:63:2f:92:ab:2f:4b:cf:0a:13:90:ee:2c:0e:43:03:93:5f:8f:5f:c5:af:bf:0a:ab:a5:6d:fb:24:5f:b6:59:1f:34:e9:96:6e:6c:cf:f2:ef:9b:bf:de:b5:22:68:9f:be:f6:ea:8e:9c:67:d0:a2:40:03:f7:ef:6a:15:09:79:cb:cc:6d:5d:01:85:b5:6d:c8:f3:d9:f7:8f:0e:fc:ba:5d:9d:92:2e:4a:1b:8b:ac:7d:99:17:5d:cd:19:f6:ad:a9:46:ed:b7:16:1b:41:72:0d:19:aa:ad:dd:9a:df:ab</Signature>\n" +
		"		</X.509-Certificate>\n" +
		"	</Body>\n" +
		"</VanguardTokenProtocol>\n\n";

	private static String  strTokenExample2 =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
		"<VanguardTokenProtocol xmlns:xsi=\"http://www.gilbarco.com/VanguardTokenProtocolSchema\" xsi:noNamespaceSchemaLocation=\"VanguardTokenProtocol.xsd\">\n" +
		"<Command>Token</Command>\n" +
		"	<Body>\n" +
		"		<Id>98765432109876543210</Id>\n" +
		"		<Version>0001.0001.0020</Version>\n" +
		"		<Type>Software Update</Type>\n" +
		"		<Expiration>\n" +
		"			<Date>03-09-2012</Date>\n" +
		"			<Time>17:27:00.00</Time>\n" +
		"		</Expiration>\n" +
		"		<Session>\n" +
		"			<MaxDuration>600</MaxDuration>\n" +
		"		</Session>\n" +
		"		<Terminal>\n" +
		"			<PPN>0000000000000000000000000000000014033049</PPN>\n" +
		"		</Terminal>\n" +
		"		<OptionalData>00000000000000000000000000000000</OptionalData>\n" +
		"		<X.509-Certificate>\n" +
		"			<Signature>93:5f:8f:5f:c5:af:bf:0a:ab:a5:6d:fb:24:5f:b6:59:5d:9d:92:2e:4a:1b:8b:ac:7d:99:17:5d:cd:19:f6:ad:ef:63:2f:92:ab:2f:4b:cf:0a:13:90:ee:2c:0e:43:03:be:f6:ea:8e:9c:67:d0:a2:40:03:f7:ef:6a:15:09:79:a9:46:ed:b7:16:1b:41:72:0d:19:aa:ad:dd:9a:df:ab:97:50:65:f5:5e:85:a6:ef:19:d1:5a:de:9d:ea:63:cd:cb:cc:6d:5d:01:85:b5:6d:c8:f3:d9:f7:8f:0e:fc:ba:1f:34:e9:96:6e:6c:cf:f2:ef:9b:bf:de:b5:22:68:9f:97:50:65:f5:5e:85:a6:ef:19:d1:5a:de:9d:ea:63:cd:ef:63:2f:92:ab:2f:4b:cf:0a:13:90:ee:2c:0e:43:03:93:5f:8f:5f:c5:af:bf:0a:ab:a5:6d:fb:24:5f:b6:59:1f:34:e9:96:6e:6c:cf:f2:ef:9b:bf:de:b5:22:68:9f:be:f6:ea:8e:9c:67:d0:a2:40:03:f7:ef:6a:15:09:79:cb:cc:6d:5d:01:85:b5:6d:c8:f3:d9:f7:8f:0e:fc:ba:5d:9d:92:2e:4a:1b:8b:ac:7d:99:17:5d:cd:19:f6:ad:a9:46:ed:b7:16:1b:41:72:0d:19:aa:ad:dd:9a:df:ab</Signature>\n" +
		"		</X.509-Certificate>\n" +
		"	</Body>\n" +
		"</VanguardTokenProtocol>\n\n";

	private static String[] astrTokenExamples =
	{
		strTokenExample, strTokenExample1, strTokenExample2
	};


	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	public static String[] getFuelingPositions()
	{
		return aFuelPositions;
	}

	public static String[] getLocationData()
	{
		return aExampleLocations;
	}

	public static String[] getSiteData()
	{
		return aExampleSites;
	}

	public static String[][] getUnitTokens()
	{
		return aaExampleUnitTokens;
	}

	public static String[][] getTargetUnitsData()
	{
		return aaExampleTargetUnits;
	}

	public static String[] getLocalityData()
	{
		return aExampleLocalities;
	}

	public static String[] getStateData()
	{
		return aExampleStates;
	}

	public static String[] getCountrieData()
	{
		return aExampleContries;
	}

	public static byte[] getXMLExample(int index) throws UnsupportedEncodingException
	{
		return astrTokenExamples[index % 3].getBytes( "UTF-8" );
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
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

}
