/**
 * 
 */
package com.gilbarco.globaltools.flexpay.main.gui;

import java.awt.Color;
import java.awt.Font;


/**
 * @author Guillermo M. Paris
 * @ date 3-May-2012
 */
public class FlexPayDialogSettings
{
	// ******************************************************************
	// INNER TYPES
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	// default values if property file is not found
	private static Color	backgroundColor		= new Color( 255, 255, 255 );			// (Color.WHITE);
	private static Font		componentFont		= new Font( "Tahoma", Font.PLAIN, 14 );
	private static Font		componentLargeFont	= new Font( "Tahoma", Font.PLAIN, 14 );
	private static Font		statusBarFont		= new Font( "Tahoma", Font.PLAIN, 11 );
	private static Font		componentSmallFont	= new Font( "Tahoma", Font.PLAIN, 11 );
	private static Font		titlePanelFont		= new Font( "Tahoma", Font.PLAIN, 11 );
	private static Font		buttonFont			= new Font( "Tahoma", Font.PLAIN, 14 );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	static
	// In the near future we'll load these properties from file, HERE
	{
		backgroundColor = new Color( 255, 255, 255 );// (Color.WHITE);
	}

	public static Color getBackgroundColor()
	{
		return backgroundColor;
	}

	public static Font getcomponentFont()
	{
		return componentFont;
	}

	public static Font getComponentSmallFont()
	{
		return componentSmallFont;
	}

	public static Font getstatusBarFont()
	{
		return statusBarFont;
	}

	public static Font getPanelTitleFont()
	{
		return titlePanelFont;
	}

	public static Font getCommponentLargeFont()
	{
		return componentLargeFont;
	}

	public static Font getButtonFont()
	{
		return buttonFont;
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
