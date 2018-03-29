/**
 * 
 */

package com.gilbarco.globaltools.flexpay.main.gui;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.Panel;

import javax.swing.JPanel;

/**
 * @author Guillermo M. Paris
 * @date 03-May-2012
 */
public class FlexPayComponentFormatter
{

	// ******************************************************************
	// INNER TYPES
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static Color	defaultBackgroundColor;
	private static Font		defaultComponentFont;
	private static Font		defaultStatusBarFont;
	private static Font		defaultPanelTitleFont;
	private static Font		defaultSmallFont;
	private static Font		defaultButtonFont;
	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	static
	{
		defaultBackgroundColor = FlexPayDialogSettings.getBackgroundColor( );
		defaultComponentFont = FlexPayDialogSettings.getcomponentFont( );
		defaultStatusBarFont = FlexPayDialogSettings.getstatusBarFont( );
		defaultPanelTitleFont = FlexPayDialogSettings.getPanelTitleFont( );
		defaultSmallFont = FlexPayDialogSettings.getComponentSmallFont( );
		defaultButtonFont = FlexPayDialogSettings.getButtonFont();
	}

	public static Font getDefaultFont()
	{
		return defaultComponentFont;
	}

	public static Font getDefaultStatusFont()
	{
		return defaultStatusBarFont;
	}

	public static Color getPanelDefaultBackgroundColor()
	{
		return defaultBackgroundColor;
	}

	public static Font getDefaultTitlePanelFont()
	{
		return defaultPanelTitleFont;
	}
	public static Font getDefaultSmallFont()
	{
		return defaultSmallFont;
	}
	public static Font getDefaultLargeFont()
	{
		return defaultSmallFont;
	}
	public static Font getDefaultButtonFont()
	{
		return defaultButtonFont;
	}
	
	
	
	public static Component setDefaultFont(Component c)
	{
		return setFont( c, defaultComponentFont );
	}

	public static Component setDefaultStatusFont(Component c)
	{
		return setFont( c, defaultStatusBarFont );
	}

	public static Container setPanelDefaultBackgroundColor(Container c)
	{
		setPanelBackgroundColor( c, defaultBackgroundColor );
		return c;
	}

	public static Component setFont(Component c, Font f)
	{
		int ncomp = 0;
		Container con = null;

		if (c instanceof Container)
		{
			con = (Container) c;
			ncomp = con.getComponentCount( );
		}

		if (ncomp == 0) // not a container or not having child components
		{
			c.setFont( f );
			return c;
		}

		Component[] acomp = con.getComponents( );

		for (int i = 0; i < ncomp; i++) // process every child component
		{
			Component child = acomp[i];
			if (child instanceof Container)
				FlexPayComponentFormatter.setFont( child, f ); // recursive

			else
				// not a container
				child.setFont( f );
		}

		c.setFont( f ); // container's font is the last thing to touch
		return c;
	}

	public static Container setPanelBackgroundColor(Container c, Color col)
	{
		if (c instanceof Panel || c instanceof JPanel)
		{
			int ncomp = c.getComponentCount( );
			Component[] acomp = c.getComponents( );

			for (int i = 0; i < ncomp; i++) // Process every child panel component, if any.
			{
				Component child = acomp[i];
				if (child instanceof Panel || child instanceof JPanel)
																		FlexPayComponentFormatter
																						.setPanelBackgroundColor(
																							(Container) child,
																							col ); // recursive
			}

			c.setBackground( col );
		}
		return c;
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
