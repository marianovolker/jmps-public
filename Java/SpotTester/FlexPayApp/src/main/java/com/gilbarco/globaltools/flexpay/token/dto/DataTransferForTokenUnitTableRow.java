/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/
package com.gilbarco.globaltools.flexpay.token.dto;

import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerView;

public class DataTransferForTokenUnitTableRow
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

	private boolean	blCommit	= false;
	private int		iSelectedRow;
	private int		idToken;
	private int		idUnit;
	private String	strTokenVersion;
	private String	strTokenCode;
	private String	strStartTime;
	private String	strEndTime;
	private String	strTokenStatus;
	private String	strPermissions;			// TODO temporary String
	private String	strRemoteMaintenance;
	private String	strPPN;
	private String	strTimes;
	private String	strMaxDuration;
	private String	strTechnicianDevice;	
	private String	strOperatorID;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// Default Constructor
	public DataTransferForTokenUnitTableRow()
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
	 * @return the iSelectedRow
	 */
	public int getSelectedRow()
	{
		return iSelectedRow;
	}

	/**
	 * @param iSelectedRow
	 *            the iSelectedRow to set
	 */
	public void setSelectedRow(int iSelectedRow)
	{
		this.iSelectedRow = iSelectedRow;
	}

	/**
	 * @return the idToken
	 */
	public int getTokenID()
	{
		return idToken;
	}

	/**
	 * @param idToken
	 *            the idToken to set
	 */
	public void setIdToken(int idToken)
	{
		this.idToken = idToken;
	}

	/**
	 * @return the idUnit
	 */
	public int getUnitID()
	{
		return idUnit;
	}

	/**
	 * @param idUnit
	 *            the idUnit to set
	 */
	public void setIdUnit(int idUnit)
	{
		this.idUnit = idUnit;
	}

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

	/**
	 * @return the iProductPartNumber
	 */
	public String getPPN()
	{
		return strPPN;
	}

	/**
	 * @param ppn
	 *            the iProductPartNumber to set
	 */
	public void setTargetUnit(String ppn)
	{
		this.strPPN = ppn;
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
	 * @return the token status
	 */
	public String getTokenStatus()
	{
		return strTokenStatus;
	}

	/**
	 * @param the
	 *            token status
	 */
	public void setTokenState(String strTokenStatus)
	{
		this.strTokenStatus = strTokenStatus;
	}

	public String getPermissions()
	{
		return this.strPermissions;
	}

	public void setPermissions(String permissions)
	{
		this.strPermissions = permissions;
	}

	public String getRemoteMaintenance()
	{
		return strRemoteMaintenance;
	}

	public void setRemoteMaintenance(String remoteMaintenance)
	{
		this.strRemoteMaintenance = remoteMaintenance;
	}
	
	public void setMaxDuration(String duration)
	{
		strMaxDuration = duration;		
	}


	public String getMaxDuration()
	{
		return strMaxDuration;
	}

	public void setTimes(String times)
	{
		this.strTimes = times;		
	}
	
	public String getTimes()
	{
		return this.strTimes;		
	}

	/**
	 * @author gparis
	 * @return String[] data from this dttu to be used to populate a row in the JTable component, at TokenManagerView.
	 */
	public String[] getDataForTokenManagerViewTable()
	{
		Integer token = idToken;
		String[] as = new String[TokenManagerView.TABLE_COLUMNS];		
		as[TokenManagerView.TABLE_COL_TOKEN_ID] = token.toString( );
		as[TokenManagerView.TABLE_COL_OPERATOR_ID] = strOperatorID;		
		as[TokenManagerView.TABLE_COL_PPN] = strPPN;
		as[TokenManagerView.TABLE_COL_START_TIME] = strStartTime;
		as[TokenManagerView.TABLE_COL_END_TIME] = strEndTime;
		as[TokenManagerView.TABLE_COL_TOKEN_CODE] = strTokenCode;
		as[TokenManagerView.TABLE_COL_SESSION_TIMES] = strTimes;
		as[TokenManagerView.TABLE_COL_SESSION_DURATION] = strMaxDuration;
		as[TokenManagerView.TABLE_COL_STATUS] = strTokenStatus;
		
		// hidden "control" fields/columns
		Integer unit = idUnit;
		as[TokenManagerView.TABLE_COL_UNIT_ID] = unit.toString( );
		return as;
	}

	/**
	 * @author gparis
	 * @param as
	 *            : String[] is the row field array obtained from the JTable component at TokenManagerView. This data is used to populate fields of this dttu.
	 */
	public void setDataFromTokenManagerViewTable(String as[])
	{
		// Strings in dttu object are ALWAYS trimmed, no leading or trailing white chars
		strOperatorID = as[TokenManagerView.TABLE_COL_OPERATOR_ID];
		strPPN = as[TokenManagerView.TABLE_COL_PPN];
		strStartTime = as[TokenManagerView.TABLE_COL_START_TIME].trim( );
		strEndTime = as[TokenManagerView.TABLE_COL_END_TIME].trim( );
		strTokenCode = as[TokenManagerView.TABLE_COL_TOKEN_CODE].trim( );
		strTimes = as[TokenManagerView.TABLE_COL_SESSION_TIMES];
		strMaxDuration = as[TokenManagerView.TABLE_COL_SESSION_DURATION];
		strTokenStatus = as[TokenManagerView.TABLE_COL_STATUS].trim( );
		idToken = Integer.parseInt( as[TokenManagerView.TABLE_COL_TOKEN_ID] );
		idUnit = Integer.parseInt( as[TokenManagerView.TABLE_COL_UNIT_ID] );
	}

	public void markToSave()
	{
		blCommit = true;
	}

	public void markToDiscard()
	{
		blCommit = false;
	}

	public void clear()
	{
		blCommit = false;
		iSelectedRow = -1;
		idToken = 0;
		idUnit = 0;
		strTokenCode = null;
		strStartTime = null;
		strPermissions = null;
		strRemoteMaintenance = null;
		strMaxDuration = null;
		strTechnicianDevice = null;
		strOperatorID = null;
	}

	public boolean needToSave()
	{
		return blCommit;
	}

	public String getTechinicianDevice()
	{
		return strTechnicianDevice;
	}

	public void setTechinicianDevice(String strTechinicianDevice)
	{
		this.strTechnicianDevice = strTechinicianDevice;
	}

	public String getOperatorID()
	{
		return strOperatorID;
	}

	public void setOperatorID(String strOperatorID)
	{
		this.strOperatorID = (strOperatorID==null) ? "" : strOperatorID;
	}

	public String getTokenVersion()
	{
		return strTokenVersion;
	}

	public void setTokenVersion(String strTokenVersion)
	{
		this.strTokenVersion = strTokenVersion;
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
