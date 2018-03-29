/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/
package com.gilbarco.globaltools.flexpay.token.dto;

import com.gilbarco.globaltools.flexpay.token.gui.SelectTokenCodeDialog;

public class DataTransferForTokenCodeTableRow
{

	// ******************************************************************
	// INNER TYPES
	// ******************************************************************

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
	private String		strTokenCode;
	private String		strStartTime;
	private String		strEndTime;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	// Default Constructor
	public DataTransferForTokenCodeTableRow()
	{
		clear( );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	/**
	 * @return the strTokenCode
	 */
	public String getTokenCode()
	{
		return strTokenCode;
	}

	/**
	 * @param strTokenCode
	 *            the strTokenCode to set
	 */
	public void setTokenCode(String strTokenCode)
	{
		this.strTokenCode = strTokenCode;
	}

	public String getStartTime()
	{
		return strStartTime;
	}

	public void setStartTime(String startTime)
	{
		this.strStartTime = startTime;
	}

	public String getEndTime()
	{
		return strEndTime;
	}

	public void setEndTime(String endTime)
	{
		this.strEndTime = endTime;
	}

	/**
	 * @author mvolker
	 * @return String[] data from this dttu to be used to populate a row in the JTable component, at SelectTokenCodeDialog.
	 */
	public String[] getDataForSelectTokenCodeDialogTable()
	{
		String[] as = new String[SelectTokenCodeDialog.TABLE_COLUMNS];
		
		as[SelectTokenCodeDialog.TABLE_COL_TOKEN_CODE] 			= strTokenCode;
		as[SelectTokenCodeDialog.TABLE_COL_TOKEN_STARTING_TIME] = strStartTime;		
		as[SelectTokenCodeDialog.TABLE_COL_TOKEN_END_TIME] 		= strEndTime;
		
		return as;
	}

	/**
	 * @author mvolker
	 * @param as
	 *            : String[] is the row field array obtained from the JTable component at SelectTokenCodeDialog. This data is used to populate fields of this dttu.
	 */
	public void setDataFromSelectTokenCodeDialogTable(String as[])
	{
		strStartTime	= as[SelectTokenCodeDialog.TABLE_COL_TOKEN_STARTING_TIME].trim( );
		strEndTime		= as[SelectTokenCodeDialog.TABLE_COL_TOKEN_END_TIME].trim( );
		strTokenCode	= as[SelectTokenCodeDialog.TABLE_COL_TOKEN_CODE].trim( );
	}

	public void clear()
	{
		strTokenCode 	= null;
		strStartTime 	= null;
		strEndTime		= null;
	}


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
