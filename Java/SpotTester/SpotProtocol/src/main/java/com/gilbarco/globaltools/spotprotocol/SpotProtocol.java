
package com.gilbarco.globaltools.spotprotocol;

import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.CREATE_WINDOW;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.DIAGNOSTIC;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.DISABLE_CL_READER;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.ENABLE_CL_READER;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.FILE_BLOCK_DOWNLOAD;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.FILE_BROWSE;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.FILE_DELETE;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.FILE_DOWNLOAD_REQUEST;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.HARDWARE_INFO;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.HIDE_WINDOW;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.LOGIN;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.LOGOUT;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.M3M5_ROMfs_BLOCK_DOWNLOAD;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.M3M5_ROMfs_FILE_DOWNLOAD;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.M3M5_ROMfs_PACKAGE_ACTIVATION;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.M3M5_ROMfs_PACKAGE_INFO;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.OTI_DO;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.OTI_GET;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.OTI_SET;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.PING;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.RESET;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.SHOW_WINDOW;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.SYSTEM_STATUS;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.VANGUARD_MAINTENANCE;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.VANGUARD_ROMfs_BLOCK_DOWNLOAD;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.VANGUARD_ROMfs_FILE_DOWNLOAD;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.VANGUARD_ROMfs_PACKAGE_ACTIVATION;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.VANGUARD_ROMfs_PACKAGE_INFO;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.FILE_UPLOAD;
import static com.gilbarco.globaltools.spotprotocol.api.SpotMessageType.FILE_BLOCK_UPLOAD;

import java.math.BigInteger;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.util.EdtBuffer;
import com.gilbarco.globaltools.mgp.api.MessageType;
import com.gilbarco.globaltools.mgp.domain.ArrayContainer;
import com.gilbarco.globaltools.mgp.domain.ChainRule;
import com.gilbarco.globaltools.mgp.domain.Condition;
import com.gilbarco.globaltools.mgp.domain.Container;
import com.gilbarco.globaltools.mgp.domain.ContainerType;
import com.gilbarco.globaltools.mgp.domain.FieldData;
import com.gilbarco.globaltools.mgp.domain.FunctionOperator;
import com.gilbarco.globaltools.mgp.domain.FunctionOperator.MathOperator;
import com.gilbarco.globaltools.mgp.domain.FunctionOperator.NumberOperator;
import com.gilbarco.globaltools.mgp.domain.FunctionOperator.Operator;
import com.gilbarco.globaltools.mgp.domain.Holder;
import com.gilbarco.globaltools.mgp.domain.HolderContainer;
import com.gilbarco.globaltools.mgp.domain.Message;
import com.gilbarco.globaltools.mgp.domain.Operation;
import com.gilbarco.globaltools.mgp.domain.Protocol;
import com.gilbarco.globaltools.mgp.domain.datatype.AlphanumericFormatter;
import com.gilbarco.globaltools.mgp.domain.datatype.DateFormatter;
import com.gilbarco.globaltools.mgp.domain.datatype.DateFormatter.DateFormatType;
import com.gilbarco.globaltools.mgp.domain.datatype.NumberingSystemFormatter;
import com.gilbarco.globaltools.mgp.domain.datatype.NumberingSystemFormatter.Types;
import com.gilbarco.globaltools.mgp.domain.extensions.ArrayContainerLengthAsValue;
import com.gilbarco.globaltools.mgp.domain.extensions.FixedField;
import com.gilbarco.globaltools.mgp.domain.extensions.FixedLength;
import com.gilbarco.globaltools.mgp.domain.extensions.LengthDeterminedByElementLength;
import com.gilbarco.globaltools.mgp.domain.extensions.LengthDeterminedByFieldValue;
import com.gilbarco.globaltools.mgp.domain.extensions.LengthDeterminedByTLVData;
import com.gilbarco.globaltools.mgp.domain.extensions.OtherElementLengthAsValue;
import com.gilbarco.globaltools.mgp.domain.extensions.PaddingData;
import com.gilbarco.globaltools.mgp.domain.extensions.RandomField;
import com.gilbarco.globaltools.mgp.domain.extensions.UserDeterminedField;
import com.gilbarco.globaltools.mgp.domain.extensions.VariableLength;
import com.gilbarco.globaltools.mgp.domain.templates.Template;
import com.gilbarco.globaltools.mgp.exceptions.MessageDataException;
import com.gilbarco.globaltools.mgp.exceptions.PredicateException;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;

/**
 * @author gparis
 * 
 */
public class SpotProtocol
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	private static final long	KEY_STATUS_COUNT	= 48L;
	private static final int	KEY_DATA_MAX_LENGTH	= 576;
	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( SpotProtocol.class );

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
	public Protocol buildSpotProtocol() throws PredicateException
	{
		Protocol protocol = new Protocol( "SPOT Protocol", "", "01.04-ext.2.3", "" );
		this.buildHeader( protocol );
		return protocol;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void buildHeader(Protocol protocol) throws PredicateException
	{
		HolderContainer tpdu = new HolderContainer( protocol, "Header", "", ContainerType.BOTH );
		OtherElementLengthAsValue len = new OtherElementLengthAsValue( tpdu, "Len", "2 bytes specifying the message length.", new FixedLength( 2, new PaddingData( (byte) 0x00, true ) ), 2,
																		new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		UserDeterminedField appId = new UserDeterminedField( tpdu, "AppId", "1 byte for specifying the command-target application on SPOT.", new FixedLength( 1, new PaddingData( false ) ),
																new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		len.addDependency( appId );

		UserDeterminedField ssk = new UserDeterminedField( tpdu, "ssk", "1 byte for EncryptionSpec", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		len.addDependency( ssk );

		// ssk dependant fields
		// if ssk == 0xFF
		UserDeterminedField sk = new UserDeterminedField( tpdu, "SK", "8+8 binary bytes representing the SK (left and right components encrypted separately).",
															new FixedLength( 16, new PaddingData( false ) ), new NumberingSystemFormatter( Types.BINARY, "" ) );
		sk.addPresenceCondition( ChainRule.AND, this.buildSskCondition( ssk, NumberOperator.EQUAL, (byte) 0xFF ) );
		len.addDependency( sk );
		UserDeterminedField crc32 = new UserDeterminedField( tpdu, "crc32", "4 bytes for CRC32 calculated on cleartext SK + 4 random bytes for 8-bytes alignment",
																new FixedLength( 8, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		crc32.addPresenceCondition( ChainRule.AND, this.buildSskCondition( ssk, NumberOperator.EQUAL, (byte) 0xFF ) );
		len.addDependency( crc32 );
		// RandomField rnd = new RandomField(header, "rnd", "4 random bytes for 8-bytes alignment", 4, new NumberingSystemFormatter(Types.HEXA, "0x%s"));
		// rnd.addPresenceCondition(ChainRule.AND, this.buildSskCondition(ssk, NumberOperator.EQUAL, (byte)0xFF));

		// if ssk != 0x00
		OtherElementLengthAsValue rem = new OtherElementLengthAsValue(
																		tpdu,
																		"rem",
																		"1 bytes SK-encrypted counter which specifies the number of padding bytes (not including itself) "
																						+ "to be discarded before APDU and crc16. This counter is required to be in the range [1,8]. Value zeronot allowed.",
																		new FixedLength( 1, new PaddingData( false ) ), 0, new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		rem.addPresenceCondition( ChainRule.AND, this.buildSskCondition( ssk, NumberOperator.DISTINCT, (byte) 0x00 ) );
		len.addDependency( rem );

		LengthDeterminedByElementLength dumLength = new LengthDeterminedByElementLength( new PaddingData( true ) );
		dumLength.addLengthOperation( new Operation( MathOperator.MOD, new EdtBuffer( BigInteger.valueOf( 8L ).toByteArray( ) ) ) );
		RandomField dum = new RandomField( tpdu, "dum", "1 to 8 random bytes (SK-encrypted) used to align the length of the encrypted section to 8 bytes multiple.", dumLength,
											new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		dum.addPresenceCondition( ChainRule.AND, this.buildSskCondition( ssk, NumberOperator.DISTINCT, (byte) 0x00 ) );
		rem.addDependency( dum );
		len.addDependency( dum );

		UserDeterminedField crc16 = new UserDeterminedField( tpdu, "crc16", "2 bytes for SK-encrypted CRC", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA,
																																															"0x%s" ) );
		crc16.addPresenceCondition( ChainRule.AND, this.buildSskCondition( ssk, NumberOperator.DISTINCT, (byte) 0x00 ) );
		len.addDependency( crc16 );

		// MessageHolder
		Holder apdu = new Holder( tpdu, "APDU holder", "" );
		rem.addDependency( apdu );
		len.addDependency( apdu );

		// Set user defined fields data
		try
		{
			appId.setData( (byte) 0x00 );
			ssk.setData( (byte) 0x00 );
		}
		catch (MessageDataException e)
		{
			logger.error( e.getMessage( ) );
		}

		this.buildAPDU( apdu );
	}

	private Condition buildSskCondition(UserDeterminedField ssk, Operator operator, byte argument)
	{
		EdtBuffer buffer = new EdtBuffer( new byte[] { argument } );
		return new Condition( ssk, operator, buffer, FunctionOperator.TRUE_EDT_BUFFER );
	}

	private Condition buildLoginAckCondition(UserDeterminedField ack, Operator operator, byte argument)
	{
		EdtBuffer buffer = new EdtBuffer( new byte[] { argument } );
		return new Condition( ack, operator, buffer, FunctionOperator.TRUE_EDT_BUFFER );
	}
	
	private void buildAPDU(Holder holder) throws PredicateException
	{
		this.buildLogin( holder );
		this.buildLoginResponse( holder );
		this.buildLogout( holder );
		this.buildLogoutResponse( holder );
		this.buildPing( holder );
		this.buildReset( holder );
		this.buildPingResponse( holder );
		this.buildCreateWindow( holder );
		this.buildShowWindow( holder );
		this.buildHideWindow( holder );
		this.buildFileBrowse( holder );
		this.buildFileBrowseResponse( holder );
		this.buildFileDelete( holder );
		this.buildFileDeleteResponse( holder );
		this.buildVanguardROMfsPackageActivationRequest( holder );
		this.buildVanguardROMfsPackageActivationResponse( holder );
		this.buildM3M5ROMfsPackageActivationRequest( holder );
		this.buildM3M5ROMfsPackageActivationResponse( holder );
		this.buildVanguardROMfsPackageInfoRequest( holder );
		this.buildVanguardROMfsPackageInfoResponse( holder );
		this.buildM3M5ROMfsPackageInfoRequest( holder );
		this.buildM3M5ROMfsPackageInfoResponse( holder );
		this.buildVanguardROMfsFileDownloadRequest( holder );
		this.buildVanguardROMfsFileDownloadResponse( holder );
		this.buildM3M5ROMfsFileDownloadRequest( holder );
		this.buildM3M5ROMfsFileDownloadResponse( holder );
		this.buildFileDownloadRequest( holder );
		this.buildFileDownloadResponse( holder );
		this.buildVanguardROMfsBlockDownloadRequest( holder );
		this.buildVanguardROMfsBlockDownloadResponse( holder );
		this.buildM3M5ROMfsBlockDownloadRequest( holder );
		this.buildM3M5ROMfsBlockDownloadResponse( holder );
		this.buildFileBlockDownload( holder );
		this.buildFileBlockDownloadResponse( holder );
		this.buildSystemStatus( holder );
		this.buildSystemStatusResponse( holder );
		this.buildHardwareInfo( holder );
		this.buildHardwareInfoResponse( holder );
		// this.buildSoftwareInfo( holder );
		// this.buildSoftwareInfoResponse( holder );
		this.buildDiagnostic( holder );
		this.buildEnableContactlessCardReader( holder );
		this.buildEnableContactlessCardReaderResponse( holder );
		this.buildDisableContactlessCardReader( holder );
		this.buildDisableContactlessCardReaderResponse( holder );
		this.buildContactlessCardDataResponse( holder );

		this.buildOTIPassthroughCommand( holder, OTI_DO );
		this.buildOTIPassthroughCommandResponse( holder, OTI_DO );
		this.buildOTIPassthroughCommand( holder, OTI_SET );
		this.buildOTIPassthroughCommandResponse( holder, OTI_SET );
		this.buildOTIPassthroughCommand( holder, OTI_GET );
		this.buildOTIPassthroughCommandResponse( holder, OTI_GET );
		
		this.buildVanguardMaintenance(holder);
		this.buildVanguardPresentTokenResponse(holder);

		this.buildFileUploadRequest(holder);
		this.buildFileBlockUploadRequest(holder);
		this.buildFileUploadResponse(holder);
		this.buildFileBlockUploadResponse(holder);
		
		this.buildKeyStatusRequest(holder);
		this.buildKeyStatusResponse(holder);	
		
		buildApplicationKeyDownloadRequest(holder);
		buildApplicationKeyDownloadResponse(holder);
		
		this.buildServiceMenuSwitch( holder );
		this.buildServiceMenuSwitchResponse( holder );
		
		buildGetChallengeRequest( holder );
		buildGetChallengeResponse( holder );
	}

	private void buildOTIPassthroughCommand(Holder holder, MessageType otiMessage)
	{
		Message command = new Message( holder, otiMessage.getName( ), otiMessage.getName( ) + " command", ContainerType.REQUEST, "" );

		new FixedField( command, "Command", "Command", new FieldData( otiMessage.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( command, "SubCommand", "SubCommand", new FieldData( otiMessage.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( command, "TagData", "TLV data", new LengthDeterminedByTLVData( ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildOTIPassthroughCommandResponse(Holder holder, MessageType otiMessage)
	{
		Message command = new Message( holder, otiMessage.getName( ), otiMessage.getName( ) + " command", ContainerType.RESPONSE, "" );

		new FixedField( command, "Command", "Command", new FieldData( otiMessage.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( command, "SubCommand", "SubCommand", new FieldData( otiMessage.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( command, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField tagSize = new UserDeterminedField( command, "TagListLen", "Tag len", new FixedLength( 2, new PaddingData( (byte) 0x00, true ) ), new NumberingSystemFormatter( Types.HEXA,
																																															"0x%s" ) );
		new UserDeterminedField( command, "TagListData", "Tag data", new LengthDeterminedByFieldValue( tagSize, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	// private void buildOTIGetCommandResponse(Holder holder) {
	// Message getCommand = new Message(holder, OTI_GET.getName(), "OTI get command", ContainerType.RESPONSE, "");
	// new FixedField(getCommand, "Command", "Command", new FieldData(OTI_GET.getCommand()[0]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new FixedField(getCommand, "SubCommand", "SubCommand", new FieldData(OTI_GET.getCommand()[1]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(getCommand, "AckCode", "Result internal code", new FixedLength(1, new PaddingData(false)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// UserDeterminedField tagSize = new UserDeterminedField(getCommand, "TagListLen", "Tag len",
	// new FixedLength(2, new PaddingData((byte) 0x00, true)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(getCommand, "TagListData", "Tag data", new LengthDeterminedByFieldValue(tagSize, false),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	//
	// }

	// private void buildOTISetCommandResponse(Holder holder) {
	// Message setCommand = new Message(holder, OTI_SET.getName(), "OTI set command", ContainerType.RESPONSE, "");
	// new FixedField(setCommand, "Command", "Command", new FieldData(OTI_SET.getCommand()[0]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new FixedField(setCommand, "SubCommand", "SubCommand", new FieldData(OTI_SET.getCommand()[1]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(setCommand, "AckCode", "Result internal code", new FixedLength(1, new PaddingData(false)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// UserDeterminedField tagSize = new UserDeterminedField(setCommand, "TagListLen", "Tag len",
	// new FixedLength(2, new PaddingData((byte) 0x00, true)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(setCommand, "TagListData", "Tag data", new LengthDeterminedByFieldValue(tagSize, false),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	//
	// }

	// private void buildOTIDoCommandResponse(Holder holder) {
	// Message doCommand = new Message(holder, OTI_DO.getName(), "OTI do command", ContainerType.RESPONSE, "");
	// new FixedField(doCommand, "Command", "Command", new FieldData(OTI_DO.getCommand()[0]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new FixedField(doCommand, "SubCommand", "SubCommand", new FieldData(OTI_DO.getCommand()[1]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(doCommand, "AckCode", "Result internal code", new FixedLength(1, new PaddingData(false)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// UserDeterminedField tagSize = new UserDeterminedField(doCommand, "TagListLen", "Tag len",
	// new FixedLength(2, new PaddingData((byte) 0x00, true)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(doCommand, "TagListData", "Tag data", new LengthDeterminedByFieldValue(tagSize, false),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// }

	// private void buildOTIGetCommand(Holder holder) {
	// Message getCommand = new Message(holder, OTI_GET.getName(), "OTI get command",
	// ContainerType.REQUEST, "");
	// new FixedField(getCommand, "Command", "Command", new FieldData(OTI_GET.getCommand()[0]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new FixedField(getCommand, "SubCommand", "SubCommand", new FieldData(OTI_GET.getCommand()[1]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(getCommand, "Tag", "Tag", new FixedLength(2, new PaddingData(false)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// UserDeterminedField tagSize = new UserDeterminedField(getCommand, "TagLen", "Tag len",
	// new FixedLength(2, new PaddingData((byte) 0x00, true)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(getCommand, "TagData", "Tag data", new LengthDeterminedByFieldValue(tagSize, false),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// }

	// private void buildOTISetCommand(Holder holder) {
	// Message setCommand = new Message(holder, OTI_SET.getName(), "OTI set command",
	// ContainerType.REQUEST, "");
	// new FixedField(setCommand, "Command", "Command", new FieldData(OTI_SET.getCommand()[0]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new FixedField(setCommand, "SubCommand", "SubCommand", new FieldData(OTI_SET.getCommand()[1]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(setCommand, "Tag", "Tag", new FixedLength(2, new PaddingData(false)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// UserDeterminedField tagSize = new UserDeterminedField(setCommand, "TagLen", "Tag len",
	// new FixedLength(2, new PaddingData((byte) 0x00, true)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(setCommand, "TagData", "Tag data", new LengthDeterminedByFieldValue(tagSize, false),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// }

	// private void buildOTIDoCommand(Holder holder) {
	// Message doCommand = new Message(holder, OTI_DO.getName(), "OTI do command", ContainerType.REQUEST, "");
	// new FixedField(doCommand, "Command", "Command", new FieldData(OTI_DO.getCommand()[0]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new FixedField(doCommand, "SubCommand", "SubCommand", new FieldData(OTI_DO.getCommand()[1]),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(doCommand, "Tag", "Tag", new FixedLength(2, new PaddingData(false)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// UserDeterminedField tagSize = new UserDeterminedField(doCommand, "TagLen", "Tag len",
	// new FixedLength(2, new PaddingData((byte) 0x00, true)),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(doCommand, "TagData", "Tag data", new LengthDeterminedByFieldValue(tagSize, false),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// new UserDeterminedField(doCommand, "TagData", "Tag data", new LengthDeterminedByTLVData(),
	// new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	// }

	private void buildContactlessCardDataResponse(Holder holder)
	{
		Message cRData = new Message( holder, "ContactlessData", "Contactless card data.", ContainerType.RESPONSE, "" );
		new FixedField( cRData, "Command", "Command", new FieldData( new byte[] { 0x50 } ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( cRData, "SubCommand", "SubCommand", new FieldData( new byte[] { 0x04 } ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( cRData, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( cRData, "TI", "Transaction number [0-255]", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		UserDeterminedField numEntries = new UserDeterminedField( cRData, "NumFields", "number of fields", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA,
																																															"0x%s" ) );

		Template<Container> entriesTemplate = Container.getNewTemplate( "ContactlessData", "Contactless card data" );
		new UserDeterminedField( (Container) entriesTemplate, "Source", "Source", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField length = new UserDeterminedField( (Container) entriesTemplate, "DataLen", "Length of the data", new FixedLength( 2, new PaddingData( false ) ),
																new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) entriesTemplate, "Data", "card data", new LengthDeterminedByFieldValue( length, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		ArrayContainer<Container> entries = new ArrayContainer<Container>( cRData, "Entries", "Device status description", 0, entriesTemplate );
		entries.addLengthOperation( new Operation( MathOperator.ADD, numEntries ) );

	}

	private void buildDisableContactlessCardReaderResponse(Holder holder)
	{
		Message disableCR = new Message( holder, DISABLE_CL_READER.getName( ), "Disable contactless card reader", ContainerType.RESPONSE, "" );
		new FixedField( disableCR, "Command", "Command", new FieldData( DISABLE_CL_READER.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( disableCR, "SubCommand", "SubCommand", new FieldData( DISABLE_CL_READER.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( disableCR, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildDisableContactlessCardReader(Holder holder)
	{
		Message disableCR = new Message( holder, DISABLE_CL_READER.getName( ), "Disable contactless card reader", ContainerType.REQUEST, "" );
		new FixedField( disableCR, "Command", "Command", new FieldData( DISABLE_CL_READER.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( disableCR, "SubCommand", "SubCommand", new FieldData( DISABLE_CL_READER.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildEnableContactlessCardReaderResponse(Holder holder)
	{
		Message enableCR = new Message( holder, ENABLE_CL_READER.getName( ), "Enable contactless card reader", ContainerType.RESPONSE, "" );
		new FixedField( enableCR, "Command", "Command", new FieldData( ENABLE_CL_READER.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( enableCR, "SubCommand", "SubCommand", new FieldData( ENABLE_CL_READER.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( enableCR, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildEnableContactlessCardReader(Holder holder)
	{
		Message enableCR = new Message( holder, ENABLE_CL_READER.getName( ), "Enable contactless card reader", ContainerType.REQUEST, "" );
		new FixedField( enableCR, "Command", "Command", new FieldData( ENABLE_CL_READER.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( enableCR, "SubCommand", "SubCommand", new FieldData( ENABLE_CL_READER.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( enableCR, "ApplicationMask", "Parameter to keep backward compatibility", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA,
																																														"0x%s" ) );
		// optional parameters
		new UserDeterminedField( enableCR, "Amount", "Amount", new FixedLength( 6, new PaddingData( false ) ), new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		new UserDeterminedField( enableCR, "TransactionDate", "Transaction date", new FixedLength( 3, new PaddingData( false ) ), new DateFormatter( "yyMMdd", "yyyy/MM/dd", DateFormatType.BCD ) );
		new UserDeterminedField( enableCR, "TransactionType", "Transaction type", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( enableCR, "TransactionTime", "Transaction time", new FixedLength( 3, new PaddingData( false ) ), new DateFormatter( "HHmmss", "HH:mm:ss", DateFormatType.BCD ) );
	}

	private void buildDiagnostic(Holder holder)
	{
		Message diagnostic = new Message( holder, DIAGNOSTIC.getName( ), "diagnostic", ContainerType.RESPONSE, "" );
		new FixedField( diagnostic, "Command", "Command", new FieldData( DIAGNOSTIC.getCommand( ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( diagnostic, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildSystemStatusResponse(Holder holder)
	{
		Message systemStatus = new Message( holder, SYSTEM_STATUS.getName( ), "System Status", ContainerType.RESPONSE, "" );
		new FixedField( systemStatus, "Command", "Command", new FieldData( SYSTEM_STATUS.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( systemStatus, "SubCommand", "SubCommand", new FieldData( SYSTEM_STATUS.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( systemStatus, "AckCode", "result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField numEntries = new UserDeterminedField( systemStatus, "NumEntries", "Number of device queried", new FixedLength( 1, new PaddingData( (byte) 0x00, true ) ),
																	new NumberingSystemFormatter( Types.NUMERIC, "" ) );

		Template<Container> entriesTemplate = Container.getNewTemplate( "DeviceStatus", "Device status description" );
		new UserDeterminedField( (Container) entriesTemplate, "DeviceCode", "Device code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) entriesTemplate, "status", "Device status", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) entriesTemplate, "StatusExt", "Additional details aboute the module condition", new FixedLength( 1, new PaddingData( (byte) 0x00, false ) ),
									new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) entriesTemplate, "ErrorCode", "An error condition or 0x00 if any, specific for the module", new FixedLength( 1, new PaddingData( (byte) 0x00, false ) ),
									new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		ArrayContainer<Container> entries = new ArrayContainer<Container>( systemStatus, "Entries", "Device status description", 0, entriesTemplate );
		entries.addLengthOperation( new Operation( MathOperator.ADD, numEntries ) );
	}

	private void buildSystemStatus(Holder holder)
	{

		Message systemStatus = new Message( holder, SYSTEM_STATUS.getName( ), "System Status", ContainerType.REQUEST, "" );

		new FixedField( systemStatus, "Command", "Command", new FieldData( SYSTEM_STATUS.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( systemStatus, "SubCommand", "SubCommand", new FieldData( SYSTEM_STATUS.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( systemStatus, "Device", "Device to be queried for status", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	/**
	 * Generates a message template tree corresponding to the hardware information message response from SPOT.
	 * 
	 * @author Guillermo Paris
	 * @param holder
	 */
	private void buildHardwareInfoResponse(Holder holder)
	{

		Message hardInfo = new Message( holder, HARDWARE_INFO.getName( ), "Hardware Information Response", ContainerType.RESPONSE, "" );

		new FixedField( hardInfo, "Command", "Command", new FieldData( HARDWARE_INFO.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new FixedField( hardInfo, "SubCommand", "SubCommand", new FieldData( HARDWARE_INFO.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( hardInfo, "AckCode", "result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		UserDeterminedField numItems = new UserDeterminedField( hardInfo, "NumItems", "Number of items queried", new FixedLength( 1, new PaddingData( (byte) 0x00, true ) ),
																new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		Template<Container> items = Container.getNewTemplate( "Items", "Container of items" );

		new UserDeterminedField( (Container) items, "ItemID", "Item ID.", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		UserDeterminedField itemLength = new UserDeterminedField( (Container) items, "ItemLength", "Item Length in bytes.", new FixedLength( 1, new PaddingData( false ) ),
																	new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( (Container) items, "ItemData", "Hardware information data related to the item.", new LengthDeterminedByFieldValue( itemLength, false ),
									new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		ArrayContainer<Container> itemArray = new ArrayContainer<Container>( hardInfo, "ItemArray", "Item Array", 16, items /* max 15 possible items for hardware info */);

		itemArray.addLengthOperation( new Operation( MathOperator.ADD, numItems ) );
	}

	/**
	 * Generates a message template tree corresponding to the hardware information message request to SPOT.
	 * 
	 * @author Guillermo Paris
	 * @param holder
	 */
	private void buildHardwareInfo(Holder holder)
	{
		Message hardInfo = new Message( holder, HARDWARE_INFO.getName( ), "Hardware Information", ContainerType.REQUEST, "" );

		new FixedField( hardInfo, "Command", "Command", new FieldData( HARDWARE_INFO.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( hardInfo, "SubCommand", "SubCommand", new FieldData( HARDWARE_INFO.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		OtherElementLengthAsValue itemCount = new OtherElementLengthAsValue( hardInfo, "ItemCount", "Hardware information item count", new FixedLength( 1, new PaddingData( false ) ), 0,
																				new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		// here itemIDs is modeled as variable length field
		UserDeterminedField itemIDs;
		itemIDs = new UserDeterminedField( hardInfo, "ItemIDs", "Item IDs", new LengthDeterminedByFieldValue( itemCount, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		// itemIDs field won't be present if itemCount is zero !
		itemIDs.setPresenceCondition( new Condition( itemCount, new Operation( NumberOperator.DISTINCT, new EdtBuffer( new byte[] { 0x00 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );

		itemCount.addDependency( itemIDs );
	}

	/**
	 * Generates a message template tree corresponding to the software information message response from SPOT.
	 * 
	 * @author Guillermo Paris
	 * @param holder
	 */
	/*
	 * private void buildSoftwareInfoResponse(Holder holder) {
	 * 
	 * Message softInfo = new Message(holder, SOFTWARE_INFO.getName(), "Software Information", ContainerType.RESPONSE, "");
	 * 
	 * new FixedField(softInfo, "Command", "Command", new FieldData(SOFTWARE_INFO.getCommand()[0]), new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	 * 
	 * new FixedField(softInfo, "SubCommand", "SubCommand", new FieldData(SOFTWARE_INFO.getCommand()[1]), new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	 * 
	 * new UserDeterminedField(softInfo, "AckCode", "result internal code", new FixedLength(1, new PaddingData(false)), new NumberingSystemFormatter(Types.HEXA, "0x%s"));
	 * 
	 * UserDeterminedField numItems = new UserDeterminedField( softInfo, "NumItems", "Number of items queried", new FixedLength(1, new PaddingData((byte) 0x00, true)), new
	 * NumberingSystemFormatter(Types.HEXA, "0x%s") );
	 * 
	 * Template<Container> items = Container.getNewTemplate("Items", "Container of items");
	 * 
	 * new UserDeterminedField( (Container) items, "ItemID", "Item ID.", new FixedLength(1, new PaddingData(false)), new NumberingSystemFormatter(Types.HEXA, "0x%s") );
	 * 
	 * UserDeterminedField itemLength = new UserDeterminedField ( (Container) items, "ItemLength", "Item Length in bytes.", new FixedLength( 1, new PaddingData(false) ), new
	 * NumberingSystemFormatter(Types.HEXA, "0x%s") );
	 * 
	 * new UserDeterminedField((Container) items, "ItemData", "Software information data related to the item.", new LengthDeterminedByFieldValue(itemLength, false), new
	 * NumberingSystemFormatter(Types.HEXA, "0x%s") );
	 * 
	 * ArrayContainer<Container> itemArray = new ArrayContainer<Container>( softInfo, "ItemArray", "Item Array", 16, items // max 12 possible items for software info );
	 * 
	 * itemArray.addLengthOperation(new Operation(MathOperator.ADD, numItems)); }
	 */

	/**
	 * Generates a message template tree corresponding to the software information message request to SPOT.
	 * 
	 * @author Guillermo Paris
	 * @param holder
	 */
	/*
	 * private void buildSoftwareInfo(Holder holder) { Message softInfo = new Message( holder, SOFTWARE_INFO.getName( ), "Software Information", ContainerType.REQUEST, "" );
	 * 
	 * new FixedField( softInfo, "Command", "Command", new FieldData( SOFTWARE_INFO.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) ); new FixedField( softInfo, "SubCommand",
	 * "SubCommand", new FieldData( SOFTWARE_INFO.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	 * 
	 * OtherElementLengthAsValue itemCount = new OtherElementLengthAsValue ( softInfo, "ItemCount", "Software information item count", new FixedLength( 1, new PaddingData( false ) ), 0, new
	 * NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	 * 
	 * // here itemIDs is modeled as variable length field UserDeterminedField itemIDs; itemIDs = new UserDeterminedField ( softInfo, "ItemIDs", "Item IDs", new LengthDeterminedByFieldValue(itemCount,
	 * false), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	 * 
	 * // itemIDs field won't be present if itemCount is zero ! itemIDs.setPresenceCondition ( new Condition ( itemCount, new Operation( NumberOperator.DISTINCT, new EdtBuffer( new byte[] { 0x00 } )
	 * ), FunctionOperator.TRUE_EDT_BUFFER ) );
	 * 
	 * itemCount.addDependency( itemIDs ); }
	 */


	private void buildM3M5ROMfsPackageActivationResponse(Holder holder)
	{
		buildDownloadCommonResponse( holder, M3M5_ROMfs_PACKAGE_ACTIVATION, "M3/M5 ROMfs Package Activation Answer", false );
	}

	private void buildM3M5ROMfsPackageActivationRequest(Holder holder)
	{
		buildROMfsDownloadCommonRequest( holder, M3M5_ROMfs_PACKAGE_ACTIVATION, "M3/M5 ROMfs Package Activation Request", false, 1 );
	}

	private void buildVanguardROMfsPackageActivationResponse(Holder holder)
	{
		buildDownloadCommonResponse( holder, VANGUARD_ROMfs_PACKAGE_ACTIVATION, "Vanguard ROMfs Package Activation Answer", false );
	}

	private void buildVanguardROMfsPackageActivationRequest(Holder holder)
	{
		buildROMfsDownloadCommonRequest( holder, VANGUARD_ROMfs_PACKAGE_ACTIVATION, "Vanguard ROMfs Package Activation Request", false, 1 );
	}

	private void buildM3M5ROMfsPackageInfoResponse(Holder holder)
	{
		Message msg = buildDownloadCommonResponse( holder, M3M5_ROMfs_PACKAGE_INFO, "M3M5 ROMfs Package Information Answer", false );

		UserDeterminedField numPacks = new UserDeterminedField( msg, "PackNum", "Number of queried packages", new FixedLength( 1, new PaddingData( (byte) 0x00, true ) ),
																new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		Template<Container> packs = Container.getNewTemplate( "Packs", "Container of packages" );

		new UserDeterminedField( (Container) packs, "Certificate", "package Certificate", new FixedLength( 248, null ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		ArrayContainer<Container> pkgArray = new ArrayContainer<Container>( msg, "PackageArray", "Package Array", 8, packs /* max 8 possible ROMfs packages */);

		pkgArray.addLengthOperation( new Operation( MathOperator.ADD, numPacks ) );
	}

	private void buildM3M5ROMfsPackageInfoRequest(Holder holder)
	{
		buildROMfsDownloadCommonRequest( holder, M3M5_ROMfs_PACKAGE_INFO, "M3M5 ROMfs Package Information Request", false, 1 );
	}

	private void buildVanguardROMfsPackageInfoResponse(Holder holder)
	{
		Message msg = buildDownloadCommonResponse( holder, VANGUARD_ROMfs_PACKAGE_INFO, "Vanguard ROMfs Package Information Answer", false );

		UserDeterminedField numPacks = new UserDeterminedField( msg, "PackNum", "Number of queried packages", new FixedLength( 1, new PaddingData( (byte) 0x00, true ) ),
																new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		Template<Container> packs = Container.getNewTemplate( "Items", "Container of packages" );

		UserDeterminedField certSize = new UserDeterminedField( (Container) packs, "CertSize", "certificate Size in bytes.", new FixedLength( 2, null ), new NumberingSystemFormatter( Types.HEXA,
																																														"0x%s" ) );

		new UserDeterminedField( (Container) packs, "Certificate", "package Certificate", new LengthDeterminedByFieldValue( certSize, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		ArrayContainer<Container> pkgArray = new ArrayContainer<Container>( msg, "PackageArray", "Package Array", 8, packs // max 8 possible ROMfs packages
		);

		pkgArray.addLengthOperation( new Operation( MathOperator.ADD, numPacks ) );
	}

	private void buildVanguardROMfsPackageInfoRequest(Holder holder)
	{
		buildROMfsDownloadCommonRequest( holder, VANGUARD_ROMfs_PACKAGE_INFO, "Vanguard ROMfs Package Information Request", false, 1 );
	}

	private void buildVanguardROMfsBlockDownloadResponse(Holder holder)
	{
		buildDownloadCommonResponse( holder, VANGUARD_ROMfs_BLOCK_DOWNLOAD, "Vanguard ROMfs block download Answer", false );
	}

	private void buildVanguardROMfsBlockDownloadRequest(Holder holder)
	{
		buildROMfsDownloadCommonRequest( holder, VANGUARD_ROMfs_BLOCK_DOWNLOAD, "Vanguard ROMfs Block download Request", false, 2 );
	}

	private void buildM3M5ROMfsBlockDownloadResponse(Holder holder)
	{
		Message msg = buildDownloadCommonResponse( holder, M3M5_ROMfs_BLOCK_DOWNLOAD, "M3M5 ROMfs block download Answer", false );

		UserDeterminedField depLength = new UserDeterminedField( msg, "DepStringLen", "Dependencies string Length", new FixedLength( 2, new PaddingData( (byte) 0x00, true ) ),
																	new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( msg, "DepString", "Debian Dependencies String", new LengthDeterminedByFieldValue( depLength, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildM3M5ROMfsBlockDownloadRequest(Holder holder)
	{
		buildROMfsDownloadCommonRequest( holder, M3M5_ROMfs_BLOCK_DOWNLOAD, "M3M5 ROMfs Block download Request", false, 2 );
	}

	private void buildFileBlockDownloadResponse(Holder holder)
	{
		buildDownloadCommonResponse( holder, FILE_BLOCK_DOWNLOAD, "file Block download Answer", true );
	}

	private void buildFileBlockDownload(Holder holder)
	{
		buildROMfsDownloadCommonRequest( holder, FILE_BLOCK_DOWNLOAD, "file Block download Request", true, 2 );
	}

	private void buildM3M5ROMfsFileDownloadResponse(Holder holder)
	{
		buildDownloadCommonResponse( holder, M3M5_ROMfs_FILE_DOWNLOAD, "M3M5 ROMfs file download Answer", true );
	}

	private void buildM3M5ROMfsFileDownloadRequest(Holder holder)
	{
		Message msg = buildROMfsDownloadCommonRequest( holder, M3M5_ROMfs_FILE_DOWNLOAD, "M3M5 ROMfs file download Request", false, 4 );

		new UserDeterminedField( msg, "Certificate", "file Certificate", new FixedLength( 248, null ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( msg, "VerifyOrSave", "0 - Verify / 1 - Save command", new FixedLength( 1, null ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( msg, "Option", "start/resume Option", new FixedLength( 1, null ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildVanguardROMfsFileDownloadResponse(Holder holder)
	{
		buildDownloadCommonResponse( holder, VANGUARD_ROMfs_FILE_DOWNLOAD, "Vanguard ROMfs file download Answer", true );
	}

	private void buildVanguardROMfsFileDownloadRequest(Holder holder)
	{
		Message msg = buildROMfsDownloadCommonRequest( holder, VANGUARD_ROMfs_FILE_DOWNLOAD, "Vanguard ROMfs file download Request", false, 4 );
		UserDeterminedField blkSize = null;

		blkSize = new UserDeterminedField( msg, "CertificateSize", "certificate Size", new FixedLength( 2, new PaddingData( 0x00, true ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( msg, "Certificate", "file Certificate", new LengthDeterminedByFieldValue( blkSize, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( msg, "Signature", "file Signature", new FixedLength( 256, null ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( msg, "VerifyOrSave", "0 - Verify / 1 - Save command", new FixedLength( 1, null ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		// TODO TO CONSULT with Italy why this byte is not on Software Upload 3.1 Spec
		new UserDeterminedField( msg, "Option", "start/resume Option", new FixedLength( 1, null ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private Message buildROMfsDownloadCommonRequest(Holder holder, SpotMessageType msgType, String sRequestTypeName, boolean blOffsetPresent, int length)
	{
		Message msg = new Message( holder, msgType.getName( ), sRequestTypeName, ContainerType.REQUEST, "" );

		new FixedField( msg, "Command", "Command", new FieldData( msgType.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new FixedField( msg, "SubCommand", "SubCommand", new FieldData( msgType.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		if (blOffsetPresent)
		{
			new UserDeterminedField( msg, "BlkOffset", "Current block position", new FixedLength( 4, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		}

		UserDeterminedField blkSize = null;

		if (length > 1) // It is a real [block/file] length field.
		{
			String sFldName = (length == 2 ? "BlkSize" : "FileSize");
			String sFldDescrip = (length == 2 ? "Block Size 16 bit [1 - 2048 bytes]" : "File Length 32 bit");

			blkSize = new UserDeterminedField( msg, sFldName, sFldDescrip, new FixedLength( length, new PaddingData( 0x00, true ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		}
		else if (msgType == M3M5_ROMfs_PACKAGE_INFO || msgType == VANGUARD_ROMfs_PACKAGE_INFO)
		{
			// It is actually a byte field saying current or saved package set info.
			new UserDeterminedField( msg, "Set", "package Set to request info [0 - Active (current), 1 - New (saved)]", new FixedLength( 1, null ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		}
		else if (msgType == M3M5_ROMfs_PACKAGE_ACTIVATION || msgType == VANGUARD_ROMfs_PACKAGE_ACTIVATION)
		{
			// A 16 byte field saying the precise time of activation, or next reboot time
			// if all bytes are 00.
			new UserDeterminedField( msg, "Time", "Activation timestamp", new FixedLength( 16, null ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		}

		if (length == 2) // block download request, 2 bytes length field
		{
			// A block data byte field (up to 2048 bytes max) depending on previous
			// defined "blkSize" field value
			new UserDeterminedField( msg, "BlkData", "Block data to be sent to SPOT", new LengthDeterminedByFieldValue( blkSize, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		}

		return msg;
	}

	private void buildFileDownloadResponse(Holder holder)
	{
		buildDownloadCommonResponse( holder, FILE_DOWNLOAD_REQUEST, "file download request Answer", true );
	}

	private Message buildDownloadCommonResponse(Holder holder, SpotMessageType msgType, String sAnswerTypeName, boolean blOffsetPresent)
	{
		Message msg = new Message( holder, msgType.getName( ), sAnswerTypeName, ContainerType.RESPONSE, "" );

		new FixedField( msg, "Command", "Command", new FieldData( msgType.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new FixedField( msg, "SubCommand", "SubCommand", new FieldData( msgType.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( msg, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		if (blOffsetPresent)
		{
			new UserDeterminedField( msg, "Offset", "Offset for the next block transfer", new FixedLength( 4, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		}

		return msg;
	}

	private void buildFileDownloadRequest(Holder holder)
	{
		Message fileDownloadRequest = new Message( holder, FILE_DOWNLOAD_REQUEST.getName( ), "File Download Request", ContainerType.REQUEST, "" );
		new FixedField( fileDownloadRequest, "Command", "Command", new FieldData( FILE_DOWNLOAD_REQUEST.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( fileDownloadRequest, "SubCommand", "SubCommand", new FieldData( FILE_DOWNLOAD_REQUEST.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileDownloadRequest, "FileType", "file type", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileDownloadRequest, "FileId", "File unique identification number", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileDownloadRequest, "FileSz", "File size", new FixedLength( 4, new PaddingData( (byte) 0x00, true ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileDownloadRequest, "CRC", "CRC32 on whole file", new FixedLength( 4, new PaddingData( (byte) 0x00, true ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField authent = new UserDeterminedField( fileDownloadRequest, "AuthenType", "Aunthentication type", new FixedLength( 1, new PaddingData( false ) ),
																new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		FixedField authent0 = new FixedField( fileDownloadRequest, "Authent0", "Four zeros.", new FieldData( new byte[] { 0x00, 0x00, 0x00, 0x00 } ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField authent12 = new UserDeterminedField( fileDownloadRequest, "Authent12", "Mac calculated in secure room.", new FixedLength( 4, new PaddingData( false ) ),
																	new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField authent34 = new UserDeterminedField( fileDownloadRequest, "Authent34", "RSA-certificate calculated in secure room.", new FixedLength( 248, new PaddingData( false ) ),
																	new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileDownloadRequest, "Description", "Ascii description of the file", new FixedLength( 16, new PaddingData( (byte) 0x00, false ) ), new AlphanumericFormatter( "%s" ) );

		try
		{
			authent0.addPresenceCondition( ChainRule.AND, new Condition( authent, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x00 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			authent12.addPresenceCondition( ChainRule.AND, new Condition( authent, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x01 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			authent12.addPresenceCondition( ChainRule.OR, new Condition( authent, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x02 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			authent34.addPresenceCondition( ChainRule.AND, new Condition( authent, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x03 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			authent34.addPresenceCondition( ChainRule.OR, new Condition( authent, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x04 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
		}
		catch (PredicateException e)
		{
			logger.error( e.getMessage( ) );
		}
	}

	private void buildFileDelete(Holder holder)
	{
		Message fileDelete = new Message( holder, FILE_DELETE.getName( ), "File delete message", ContainerType.REQUEST, "" );
		new FixedField( fileDelete, "Command", "Command", new FieldData( FILE_DELETE.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( fileDelete, "SubCommand", "SubCommand", new FieldData( FILE_DELETE.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileDelete, "FileType", "Type of file to be deleted", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileDelete, "FileId", "id of file to be deleted", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildFileDeleteResponse(Holder holder)
	{
		Message fileDelete = new Message( holder, FILE_DELETE.getName( ), "File delete message", ContainerType.RESPONSE, "" );
		new FixedField( fileDelete, "Command", "Command", new FieldData( FILE_DELETE.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( fileDelete, "SubCommand", "SubCommand", new FieldData( FILE_DELETE.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileDelete, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildFileBrowse(Holder holder)
	{
		Message fileBrower = new Message( holder, FILE_BROWSE.getName( ), "File browse message", ContainerType.REQUEST, "" );
		new FixedField( fileBrower, "Command", "Command", new FieldData( FILE_BROWSE.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( fileBrower, "SubCommand", "SubCommand", new FieldData( FILE_BROWSE.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileBrower, "FileType", "Type of file to be listed", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildFileBrowseResponse(Holder holder)
	{
		Message fileBrowser = new Message( holder, FILE_BROWSE.getName( ), "File browse message", ContainerType.RESPONSE, "" );
		new FixedField( fileBrowser, "Command", "Command", new FieldData( FILE_BROWSE.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( fileBrowser, "SubCommand", "SubCommand", new FieldData( FILE_BROWSE.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileBrowser, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( fileBrowser, "FileType", "File usage and contents", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField numEntries = new UserDeterminedField( fileBrowser, "NumEntries", "Number of files retrieved", new FixedLength( 1, new PaddingData( false ) ),
																	new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		Template<Container> Tags = Container.getNewTemplate( "TAG", "Array of tags" );
		new UserDeterminedField( (Container) Tags, "FieldId", "File unique identification number", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "CRC", "File signature CRC32", new FixedLength( 4, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "Autenticated", "File authentication flag", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		new UserDeterminedField( (Container) Tags, "Description", "ASCII description of the file contents", new FixedLength( 16, new PaddingData( false ) ), new AlphanumericFormatter( "%s" ) );
		ArrayContainer<Container> TagsArray = new ArrayContainer<Container>( fileBrowser, "TagsArray", "tags array", 0, Tags );
		TagsArray.addLengthOperation( new Operation( MathOperator.ADD, numEntries ) );
		new UserDeterminedField( fileBrowser, "ResourceMemoryAvailable", "Available memory in kilobytes", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.NUMERIC,
																																														"" ) );
	}

	private void buildHideWindow(Holder holder)
	{
		Message hideWindow = new Message( holder, HIDE_WINDOW.getName( ), "Hide window", ContainerType.REQUEST, "" );
		new FixedField( hideWindow, "Command", "Command", new FieldData( HIDE_WINDOW.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( hideWindow, "SubCommand", "SubCommand", new FieldData( HIDE_WINDOW.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( hideWindow, "WinId", "Window Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		// new UserDeterminedField(showWindow, "numTags", "num Tag", new FixedLength(1,null), new NumberingSystemFormatter(Types.NUMERIC,""));
		ArrayContainerLengthAsValue numTags = new ArrayContainerLengthAsValue( hideWindow, "numTags", "num Tag", new FixedLength( 1, new PaddingData( false ) ), 0,
																				new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		Template<Container> Tags = Container.getNewTemplate( "TAG", "Array of tags" );
		new UserDeterminedField( (Container) Tags, "TagId", "Tag Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		ArrayContainer<Container> array = new ArrayContainer<Container>( hideWindow, "TagsArray", "tags array", 10, Tags );
		numTags.addDependency( array );
	}

	private void buildShowWindow(Holder holder)
	{
		Message showWindow = new Message( holder, SHOW_WINDOW.getName( ), "Show window", ContainerType.REQUEST, "" );
		new FixedField( showWindow, "Command", "Command", new FieldData( SHOW_WINDOW.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( showWindow, "SubCommand", "SubCommand", new FieldData( SHOW_WINDOW.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( showWindow, "WinId", "Window Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		// new UserDeterminedField(showWindow, "numTags", "num Tag", new FixedLength(1,null), new NumberingSystemFormatter(Types.NUMERIC,""));
		ArrayContainerLengthAsValue numTags = new ArrayContainerLengthAsValue( showWindow, "numTags", "num Tag", new FixedLength( 1, new PaddingData( false ) ), 0,
																				new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		Template<Container> Tags = Container.getNewTemplate( "TAG", "Array of tags" );
		new UserDeterminedField( (Container) Tags, "TagId", "Tag Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "Zorder", "Zorder", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		ArrayContainer<Container> array = new ArrayContainer<Container>( showWindow, "TagsArray", "tags array", 100, Tags );
		numTags.addDependency( array );
	}

	protected void buildCreateWindow(Holder holder)
	{
		Message createWindow = new Message( holder, CREATE_WINDOW.getName( ), "Create window", ContainerType.REQUEST, "" );
		new FixedField( createWindow, "Command", "Command", new FieldData( CREATE_WINDOW.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( createWindow, "SubCommand", "SubCommand", new FieldData( CREATE_WINDOW.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( createWindow, "WinId", "Window Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		// UserDeterminedField numTags = new UserDeterminedField(createWindow, "numTags", "num Tag", new FixedLength(1,null), new NumberingSystemFormatter(Types.NUMERIC,""));
		ArrayContainerLengthAsValue numTags = new ArrayContainerLengthAsValue( createWindow, "numTags", "num Tag", new FixedLength( 1, new PaddingData( false ) ), 0,
																				new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		Template<Container> Tags = Container.getNewTemplate( "TAG", "Array of tags" );

		// common fields
		new UserDeterminedField( (Container) Tags, "TagId", "Tag Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "OriginX", "Origin X", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "OriginY", "Origin Y", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "Width", "Width", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "Height", "Height", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "BackRGB", "Background RGB", new FixedLength( 3, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "BorderSize", "Border Size", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "BorderRGB", "border RGB", new FixedLength( 3, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) Tags, "Visible", "visible", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField tagType = new UserDeterminedField( (Container) Tags, "TagType", "tagType", new FixedLength( 1, new PaddingData( false ) ),
																new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		// if TagType == 1

		UserDeterminedField fontid = new UserDeterminedField( (Container) Tags, "FontId", "Font id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField fontRGB = new UserDeterminedField( (Container) Tags, "FontRGB", "font RGB", new FixedLength( 3, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA,
																																														"0x%s" ) );
		UserDeterminedField alligen = new UserDeterminedField( (Container) Tags, "Alligen", "Alligen", new FixedLength( 1, new PaddingData( false ) ),
																new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField posX = new UserDeterminedField( (Container) Tags, "posX", "pos X", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField posY = new UserDeterminedField( (Container) Tags, "posY", "pos Y", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField textLen = new UserDeterminedField( (Container) Tags, "TextLen", "text length", new FixedLength( 2, new PaddingData( (byte) 0x00, true ) ),
																new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		UserDeterminedField text = new UserDeterminedField( (Container) Tags, "Text", "Text", new LengthDeterminedByFieldValue( textLen, false ), new AlphanumericFormatter( "%s" ) );

		// if TagType == 2

		UserDeterminedField imageid = new UserDeterminedField( (Container) Tags, "ImageId", "Image id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA,
																																														"0x%s" ) );
		FixedField imageAllign = new FixedField( (Container) Tags, "ImageAllign", "ImageAllign", new FieldData( new EdtBuffer( new byte[] { 0x00 } ) ), new NumberingSystemFormatter( Types.HEXA,
																																														"0x%s" ) );
		UserDeterminedField imageposX = new UserDeterminedField( (Container) Tags, "imageposX", "imagepos X", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA,
																																															"0x%s" ) );
		UserDeterminedField imageposY = new UserDeterminedField( (Container) Tags, "imageposY", "imagepos Y", new FixedLength( 2, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA,
																																															"0x%s" ) );

		try
		{
			fontid.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x01 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			fontRGB.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x01 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			alligen.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x01 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			posX.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x01 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			posY.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x01 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			textLen.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x01 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			text.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x01 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );

			imageid.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x02 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			imageAllign.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x02 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			imageposX.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x02 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );
			imageposY.addPresenceCondition( ChainRule.AND, new Condition( tagType, new Operation( NumberOperator.EQUAL, new EdtBuffer( new byte[] { 0x02 } ) ), FunctionOperator.TRUE_EDT_BUFFER ) );

		}
		catch (PredicateException e)
		{
			logger.error( e.getMessage( ) );
		}

		ArrayContainer<Container> TagsArray = new ArrayContainer<Container>( createWindow, "TagsArray", "tags array", 100, Tags );
		numTags.addDependency( TagsArray );
	}

	protected void buildLogin(Holder holder)
	{
		Message login = new Message( holder, LOGIN.getName( ), "Login", ContainerType.REQUEST, "" );
		new FixedField( login, "Id", "Code 0x01", new FieldData( LOGIN.getCommand( ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( login, "ClientId", "Application id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( login, "ModuloLen", "Length of PMK RSA module", new FixedLength( 2, new PaddingData( (byte) 0x00, true ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( login, "PingTimeout", "Specifies the maximum allowed time between two messages from master program (0x00=no timeout)",
									new FixedLength( 1, new PaddingData( (byte) 0x00, true ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildLoginResponse(Holder holder) throws PredicateException
	{
		Message login = new Message( holder, LOGIN.getName( ), "Login", ContainerType.RESPONSE, "" );
		new FixedField( login, "Id", "Code 0x01", new FieldData( LOGIN.getCommand( ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		
		UserDeterminedField ack = new UserDeterminedField( login, "AckCode", "result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );		
		
		
		UserDeterminedField datetime = new UserDeterminedField( login, "DateTime", "Date and time of last login hh:mm:ss, dd/mm/yyyy", new FixedLength( 14, new PaddingData( (byte) 0x00, true ) ),
									new DateFormatter( "HHmmssddMMyyyy", "yyyy/MM/dd - HH:mm:ss", DateFormatType.ASCII ) );		
		datetime.addPresenceCondition( ChainRule.AND, this.buildLoginAckCondition( ack, NumberOperator.EQUAL, (byte) 0x00 ) );

		
		UserDeterminedField numApps = new UserDeterminedField( login, "NumApps", "Number of SPOT local applications available on SPOT", new FixedLength( 1, new PaddingData( (byte) 0x00, true ) ),
																new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		numApps.addPresenceCondition( ChainRule.AND, this.buildLoginAckCondition( ack, NumberOperator.EQUAL, (byte) 0x00 ) );

		// Spot App Description
		Template<Container> spotApp = Container.getNewTemplate( "SpotApp", "Spot Application description" );
		new UserDeterminedField( (Container) spotApp, "AppId", "ClientId of available SPOT local application", new FixedLength( 1, new PaddingData( false ) ),
									new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		new UserDeterminedField( (Container) spotApp, "Sign", "CRC32. It allows to verify the release of the local application on SPOT", new FixedLength( 4, new PaddingData( false ) ),
									new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) spotApp, "AppAscii", "Application version in text format", new FixedLength( 40, new PaddingData( (byte) 0x00, false ) ), new AlphanumericFormatter( "%s" ) );
		ArrayContainer<Container> entries = new ArrayContainer<Container>( login, "Entries", "Spot Application description", 0, spotApp );
		entries.addLengthOperation( new Operation( MathOperator.ADD, numApps ) );
		entries.addPresenceCondition( ChainRule.AND, this.buildLoginAckCondition( ack, NumberOperator.EQUAL, (byte) 0x00 ) );

		// RSA Key
		Container rsa = new Container( login, "RSAKey", "If public key (PMK) was required." );
		UserDeterminedField moduloLen = new UserDeterminedField( rsa, "ModuloLen", "PK Modulus length", new FixedLength( 2, new PaddingData( false ) ),
																	new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		UserDeterminedField exponentLen = new UserDeterminedField( rsa, "ExponentLen", "PK Exponent length", new FixedLength( 2, new PaddingData( false ) ),
																	new NumberingSystemFormatter( Types.NUMERIC, "" ) );
		new UserDeterminedField( rsa, "Modulo", "PK Modulo", new LengthDeterminedByFieldValue( moduloLen, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( rsa, "Exponent", "PK Exponent", new LengthDeterminedByFieldValue( exponentLen, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		
		rsa.addPresenceCondition( ChainRule.AND, this.buildLoginAckCondition( ack, NumberOperator.EQUAL, (byte) 0x00 ) );
	}

	protected void buildLogout(Holder holder)
	{
		Message logout = new Message( holder, LOGOUT.getName( ), "Logout", ContainerType.REQUEST, "" );
		new FixedField( logout, "Code", "Code 0x02", new FieldData( LOGOUT.getCommand( ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildLogoutResponse(Holder holder)
	{
		Message logout = new Message( holder, LOGOUT.getName( ), "Logout", ContainerType.RESPONSE, "" );
		new FixedField( logout, "Code", "Code 0x02", new FieldData( LOGOUT.getCommand( ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( logout, "AckCode", "result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildPing(Holder holder)
	{
		Message ping = new Message( holder, PING.getName( ), "Ping", ContainerType.REQUEST, "" );
		new FixedField( ping, "Code", "Code 0x03", new FieldData( PING.getCommand( ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildPingResponse(Holder holder)
	{
		Message ping = new Message( holder, PING.getName( ), "Ping", ContainerType.RESPONSE, "" );
		new FixedField( ping, "Code", "Code 0x03", new FieldData( PING.getCommand( ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( ping, "AckCode", "result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildReset(Holder holder)
	{
		Message reset = new Message( holder, RESET.getName( ), "Reset", ContainerType.REQUEST, "" );
		new FixedField( reset, "Code", "Code 0x06", new FieldData( RESET.getCommand( ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildVanguardMaintenance(Holder holder)
	{
		Message presentToken = new Message( holder, VANGUARD_MAINTENANCE.getName( ), "Maintenance Message", ContainerType.REQUEST, "" );
		new FixedField( presentToken, "Command", "Command", new FieldData( VANGUARD_MAINTENANCE.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( presentToken, "TokenId", "TokenId", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( presentToken, "TokenData", "xml token data", new VariableLength(8192), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildVanguardPresentTokenResponse(Holder holder)
	{
		Message presentToken = new Message( holder, VANGUARD_MAINTENANCE.getName( ), "Maintenance Message Response", ContainerType.RESPONSE, "" );
		new FixedField( presentToken, "Command", "Command", new FieldData( VANGUARD_MAINTENANCE.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( presentToken, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}
	
	private void buildFileUploadRequest(Holder holder)
	{
		Message msg = new Message( holder, FILE_UPLOAD.getName( ), "File Upload Request", ContainerType.REQUEST, "" );

		new FixedField( msg, "Command", "Command", new FieldData( FILE_UPLOAD.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( msg, "SubCommand", "SubCommand", new FieldData( FILE_UPLOAD.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "FileType", "file type", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "FileId", "File unique identification number", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildFileBlockUploadRequest(Holder holder)
	{
		Message msg = new Message( holder, FILE_BLOCK_UPLOAD.getName( ), "File Block Upload Request", ContainerType.REQUEST, "" );

		new FixedField( msg, "Command", "Command", new FieldData( FILE_BLOCK_UPLOAD.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( msg, "SubCommand", "SubCommand", new FieldData( FILE_BLOCK_UPLOAD.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "BlkOffset", "Offset for the next block transfer", new FixedLength( 4, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildFileUploadResponse(Holder holder)
	{
		Message msg = new Message( holder, FILE_UPLOAD.getName( ), "File Upload Response", ContainerType.RESPONSE, "" );

		new FixedField( msg, "Command", "Command", new FieldData( FILE_UPLOAD.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( msg, "SubCommand", "SubCommand", new FieldData( FILE_UPLOAD.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "FileSz", "File size", new FixedLength( 4, new PaddingData( (byte) 0x00, true ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	private void buildFileBlockUploadResponse(Holder holder)
	{
		Message msg = new Message( holder, FILE_BLOCK_UPLOAD.getName( ), "File Upload Response", ContainerType.RESPONSE, "" );
		UserDeterminedField blkSize = null;

		new FixedField( msg, "Command", "Command", new FieldData( FILE_BLOCK_UPLOAD.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( msg, "SubCommand", "SubCommand", new FieldData( FILE_BLOCK_UPLOAD.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "Offset", "Offset for the next block transfer", new FixedLength( 4, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		blkSize = new UserDeterminedField( msg, "BlkSize", "Block Size 16 bit [1 - 2048 bytes]", new FixedLength( 4, new PaddingData( 0x00, true ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "BlkData", "Block data to be sent to SPOT", new LengthDeterminedByFieldValue( blkSize, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}
	
	private void buildKeyStatusRequest(Holder holder)
	{
		Message msg = new Message( holder, SpotMessageType.KEY_STATUS.getName( ), "Key Status Request", ContainerType.REQUEST, "" );
		new FixedField( msg, "Command", "Command", new FieldData( SpotMessageType.KEY_STATUS.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( msg, "SubCommand", "SubCommand", new FieldData( SpotMessageType.KEY_STATUS.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildKeyStatusResponse(Holder holder) throws PredicateException
	{
		Message keystatus = new Message( holder, SpotMessageType.KEY_STATUS.getName( ), "Key Status Response", ContainerType.RESPONSE, "" );

		new FixedField( keystatus, "Command", "Command", new FieldData( SpotMessageType.KEY_STATUS.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( keystatus, "SubCommand", "SubCommand", new FieldData( SpotMessageType.KEY_STATUS.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		
		new UserDeterminedField( keystatus, "AckCode", "result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );		
		
		// Spot App Description
		Template<Container> keyTable = Container.getNewTemplate( "KeyTable", "Key Status Data" );
		new UserDeterminedField( (Container) keyTable, "KeyUse", "Key Use Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( (Container) keyTable, "KeyType", "Key Type Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		
		ArrayContainer<Container> entries = new ArrayContainer<Container>( keystatus, "Entries", "Key Status Table", 0, keyTable );
		entries.addLengthOperation( new Operation( MathOperator.ADD, new EdtBuffer( BigInteger.valueOf( KEY_STATUS_COUNT ).toByteArray( ) ) ) );
	}

	private void buildApplicationKeyDownloadRequest(Holder holder)
	{
		Message msg = new Message( holder, SpotMessageType.APPLICATION_KEY_DOWNLOAD.getName( ), "Application Key Download Request", ContainerType.REQUEST, "" );
		new FixedField( msg, "Command", "Command", new FieldData( SpotMessageType.APPLICATION_KEY_DOWNLOAD.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( msg, "SubCommand", "SubCommand", new FieldData( SpotMessageType.APPLICATION_KEY_DOWNLOAD.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );

		new UserDeterminedField( msg, "KeyId", "Key Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "KeyUse", "Key Use", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "TransportKeyId", "Transport Key Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "KeyType", "Key Type", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "KeyData", "Key Data", new VariableLength( KEY_DATA_MAX_LENGTH ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}
	
	private void buildApplicationKeyDownloadResponse(Holder holder)
	{
		Message msg = new Message( holder, SpotMessageType.APPLICATION_KEY_DOWNLOAD.getName( ), "Application Key Download Response", ContainerType.RESPONSE, "" );
		new FixedField( msg, "Command", "Command", new FieldData( SpotMessageType.APPLICATION_KEY_DOWNLOAD.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new FixedField( msg, "SubCommand", "SubCommand", new FieldData( SpotMessageType.APPLICATION_KEY_DOWNLOAD.getCommand( )[1] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "AckCode", "Result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( msg, "KeyId", "Key Id", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}
	
	protected void buildServiceMenuSwitch(Holder holder)
	{
		Message menuswitch = new Message( holder, SpotMessageType.SERVICE_MENU_SWITCH.getName( ), "ServiceMenuSwitch", ContainerType.REQUEST, "" );
		new FixedField( menuswitch, "Code", "Code", new FieldData( SpotMessageType.SERVICE_MENU_SWITCH.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( menuswitch, "SwitchType", "Switch Type", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildServiceMenuSwitchResponse(Holder holder)
	{
		Message menuswitch = new Message( holder, SpotMessageType.SERVICE_MENU_SWITCH.getName( ), "ServiceMenuSwitch", ContainerType.RESPONSE, "" );
		new FixedField( menuswitch, "Code", "Code", new FieldData( SpotMessageType.SERVICE_MENU_SWITCH.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( menuswitch, "AckCode", "result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildGetChallengeRequest(Holder holder)
	{
		Message getchallenge = new Message( holder, SpotMessageType.GET_CHALLENGE.getName( ), "GetChallengeRequest", ContainerType.REQUEST, "" );
		new FixedField( getchallenge, "Code", "Code", new FieldData( SpotMessageType.GET_CHALLENGE.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		
		UserDeterminedField techIdLen = new UserDeterminedField( getchallenge, "TechIdLen", "Switch Type", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( getchallenge, "TechnicianId", "Switch Type", new LengthDeterminedByFieldValue( techIdLen, false ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}

	protected void buildGetChallengeResponse(Holder holder)
	{
		Message getchallenge = new Message( holder, SpotMessageType.GET_CHALLENGE.getName( ), "GetChallengeResponse", ContainerType.RESPONSE, "" );
		new FixedField( getchallenge, "Code", "Code", new FieldData( SpotMessageType.GET_CHALLENGE.getCommand( )[0] ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( getchallenge, "AckCode", "result internal code", new FixedLength( 1, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( getchallenge, "PPN", "ppn", new FixedLength( 8, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
		new UserDeterminedField( getchallenge, "ChallengeCode", "challenge code", new FixedLength( 10, new PaddingData( false ) ), new NumberingSystemFormatter( Types.HEXA, "0x%s" ) );
	}


	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
