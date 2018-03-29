/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.token.controller;

import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.InvalidPathException;
import java.nio.file.Path;
import java.util.Formatter;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.util.EdtFileUtil;
import com.gilbarco.globaltools.flexpay.token.domain.Token;
import com.gilbarco.globaltools.flexpay.token.dto.DataTransferForTokenUnitTableRow;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerController;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerModel;

/**
 * @author Guillermo Paris
 * @created 19-May-2012 04:53:57 p.m.
 */
public class TokenController
{

	// ******************************************************************
	// INNER TYPES
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( TokenController.class );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	public static Token createToken(String sCode, String sStartDate, String sEndDate)
	{
		Token t = new Token( sCode, sStartDate, sEndDate );
		return t;
	}

	/**
	 * @param tokenCode
	 *            : the unique token code.
	 * @return A reference to the requested token object in memory database, or null if the token code is not found.
	 */
	public static Token getToken(int iTokenID)
	{
		return Token.getFromDatabase( iTokenID );
	}


	public static boolean saveToken(String sTokenCode, String sTokenPPN)
	{
		Token t = Token.getFromDatabase( sTokenCode );
		if (t == null) return false;

		byte abXMLbytes[] = t.getXmlBytes( );
		if (abXMLbytes == null) return false;

		String sFilename = generateTokenFileName( sTokenPPN );
		String strUSBdrive = TokenManagerController.getUSBdrive( );

		String sTokenPath;
		if (strUSBdrive == null || strUSBdrive.isEmpty( ) || strUSBdrive.equalsIgnoreCase( "C:" ))
		{
			logger.error( "Error saving token - Invalid Destination Path." );
			return false;
		}
		else
		{
			sTokenPath = strUSBdrive + TokenManagerModel.getUsbTokenPath( );
		}

		// directory paths		
		Path tokenPath;
		try
		{
			tokenPath = FileSystems.getDefault( ).getPath( sTokenPath );
			if ( !EdtFileUtil.checkOrCreateDirectory( tokenPath ) ) 
			{
				return false;
			}
		}
		catch (InvalidPathException e)
		{
			logger.error( "Error saving token - Invalid Path Exception " + e.getMessage( ) );
			return false;
		}

		// token file path
		try
		{
			tokenPath = FileSystems.getDefault( ).getPath( sTokenPath, sFilename );
			Files.write( tokenPath, abXMLbytes );
		}
		catch (IOException | InvalidPathException e)
		{
			logger.error( "Error saving token - " + e.getMessage( ) );
			return false;
		}

		return true;
	}

	private static String generateTokenFileName(String sTokenPPN)
	{
//		Token file name format is:
//			PPN.token Where:
//				PPN is the unit id.
//				.token: File extension identification.		
		Formatter f = new Formatter( );
		f.format( "%s.token", sTokenPPN );
		return f.out( ).toString( );
	}

	public static void setXmlBytes(String sTokenCode, byte[] xmlBytes)
	{
		Token t = Token.getFromDatabase( sTokenCode );
		if (t == null) return;
		t.setXmlBytes( xmlBytes );
		Token.replaceInDatabase( t );// TODO replace when setXmlBytes for token is implemented
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

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
