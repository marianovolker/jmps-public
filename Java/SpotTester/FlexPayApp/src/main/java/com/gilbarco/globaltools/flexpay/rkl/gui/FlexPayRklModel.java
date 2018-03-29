
package com.gilbarco.globaltools.flexpay.rkl.gui;

import com.gilbarco.globaltools.edt.gui.app.EdtAppModel;
import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;

public class FlexPayRklModel extends EdtAppModel
{
	// ******************************************************************
	// INNER TYPES.
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static String	strTechnicianID	= "";

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
	public FlexPayRklModel()
	{
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface)
	// ******************************************************************

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	public static void setTechnicianID(String sTechnicianNo)
	{
		strTechnicianID = sTechnicianNo;

	}

	public static String getTechnicianID()
	{
		return strTechnicianID;
	}

	public Object isLogged()
	{
		// TODO Auto-generated method stub
		return null;
	}

	public Object isCertificatePublished()
	{
		// TODO Auto-generated method stub
		return null;
	}

	public String getRKLServer()
	{
		return "127.0.0.1";// temporary
	}

	public String getRKLPort()
	{
		return Integer.toString( FlexPayAppConfig.getRklTerminalPort( ) );
	}

	public void setCurrentTerminalIP(String joinIP)
	{
		FlexPayAppConfig.setCurrentTerminalIP( joinIP );

	}

	public String getTerminalPort()
	{
		return Integer.toString(FlexPayAppConfig.getTerminalPort( ));
	}

	public String getTerminalIP()
	{
		return FlexPayAppConfig.getCurrentTerminalIP( );
	}

}
