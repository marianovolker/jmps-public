/**
 * @author Guillermo M. Paris
 * @date 24/4/2012
 */

package com.gilbarco.globaltools.flexpay.widgets;

import java.awt.Color;
import java.awt.Container;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.SpinnerDateModel;

import com.gilbarco.globaltools.flexpay.main.gui.FlexPayComponentFormatter;

@SuppressWarnings("serial")
public class DatePickerCtrl extends JPanel
{
	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// Constant Values
	private final int	iTinyGap		= 5;
	private final int	iTrigBtnSize	= 40;
	private final int	iCalBtnWidth	= 44;
	private final int	iCalBtnHeight	= 24;
	private final int	iSpnDateHeight	= 22;
	private final int	iNavBtnHeight	= 25;
	private final int	iWidth			= 308;														// 336;
	private final int	iMinHeight		= iTinyGap + iTrigBtnSize;
	private final int	iCalHeight		= 7 * iCalBtnHeight + 2 * iTinyGap;
	private final int	iMaxHeight		= iMinHeight + iCalHeight + iNavBtnHeight + 2 * iTinyGap;

	private boolean		blExpanded		= false;
	private boolean		blNeverExpanded	= true;
	private int			iLeft			= 10;
	private int			iTop			= 10;
	private int			iMonth			= 0;
	private int			iYear			= 0;
	private String		sDay			= "";
	private JSpinner	spnPickedDate;
	private JButton		btnTrigger;
	private JButton		btnPrev;
	private JButton		btnNext;
	private JButton[]	aDayButtons;
	private JLabel		lblMonth;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public DatePickerCtrl(Container parent, int left, int top)
	{
		super( );
		iLeft = left;
		iTop = top;

		setLayout( null );
		setBackground( parent.getBackground( ) );

		btnTrigger = new JButton( "" );
		btnTrigger.setFont( FlexPayComponentFormatter.getDefaultFont( ) );
		btnTrigger.setIcon( new ImageIcon( this.getClass( ).getResource( "/images/Calendar.png" ) ) );
		btnTrigger.setBounds( 0, 0, iTrigBtnSize, iTrigBtnSize );
		btnTrigger.setVisible( true );
		btnTrigger.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent e)
			{
				if (blExpanded)
					foldUpCalendar( );
				else
					dropDownCalendar( );
			}
		} );
		add( btnTrigger );

		spnPickedDate = new JSpinner( );
		spnPickedDate.setModel( new SpinnerDateModel( new Date( java.lang.System
						.currentTimeMillis( ) ), null, null, Calendar.DAY_OF_MONTH ) );
		spnPickedDate.setEditor( new JSpinner.DateEditor( spnPickedDate, "MM/dd/yyyy" ) );
		spnPickedDate.setFont( FlexPayComponentFormatter.getDefaultFont( ) );
		spnPickedDate.setBounds( iTrigBtnSize + iTinyGap, (iTrigBtnSize - iSpnDateHeight) / 2, 100,
			iSpnDateHeight );
		spnPickedDate.setVisible( true );
		add( spnPickedDate );

		setBounds( iLeft, iTop, iWidth, iMinHeight );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	public String getStringDate()
	{
		SimpleDateFormat sdf = new SimpleDateFormat( "MM/dd/yyyy" );
		return sdf.format( (Date) spnPickedDate.getValue( ) );
	}

	public Date getDate()
	{
		return (Date) spnPickedDate.getValue( );
	}

	public void setDate(Date d)
	{
		spnPickedDate.setValue( d );
	}


	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	private void ConstructCalendar()
	{
		int x0 = 0, y0 = iMinHeight;
		int x = x0, y = y0;
		int xsize = iCalBtnWidth, ysize = iCalBtnHeight;
		// String[] astrHeader = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
		String[] astrHeader =
		{ "S", "M", "T", "W", "T", "F", "S" };
		aDayButtons = new JButton[49];

		for (int nrow = 0; nrow < 7; nrow++)
		{
			x = x0;

			for (int ncol = 0; ncol < 7; ncol++)
			{
				int i = 7 * nrow + ncol; // index of array
				final int selection = i;
				aDayButtons[i] = new JButton( );
				aDayButtons[i].setFocusPainted( false );
				aDayButtons[i].setBackground( new Color( 0xE8E8F4 ) );
				aDayButtons[i].setFont( new Font( "Arial Narrow", Font.BOLD, 10 ) );
				aDayButtons[i].setBounds( x, y, xsize, ysize );

				if (nrow > 0) // not in the first row of buttons
				{
					aDayButtons[i].addActionListener( new ActionListener( )
					{
						public void actionPerformed(ActionEvent ae)
						{
							sDay = aDayButtons[selection].getActionCommand( );
							spnPickedDate.requestFocusInWindow( );
							foldUpCalendar( );
							setPickedDateText( );
						}
					} );
				}
				else
				// header (first row)
				{
					aDayButtons[i].setText( astrHeader[i] );
					aDayButtons[i].setForeground( Color.BLUE );
				}

				aDayButtons[i].setVisible( true );
				add( aDayButtons[i] );

				x += xsize;
			}

			y += ysize;
		}
		
		// Default font for the FlexPay application
		Font fds = FlexPayComponentFormatter.getDefaultFont( );
		
		btnPrev = new JButton( "<< Previous" );
		btnPrev.setFont( new Font( fds.getFamily( ), Font.PLAIN, 10 ) );
		btnPrev.setBounds( 0, y0 + 7 * ysize + iTinyGap, 100, iNavBtnHeight );
		btnPrev.setVisible( true );
		btnPrev.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent ae)
			{
				iMonth--;
				displayDate( );
			}
		} );
		add( btnPrev );

		lblMonth = new JLabel( "", JLabel.CENTER );
		lblMonth.setFont( new Font( fds.getFamily( ), Font.BOLD, 12 ) );
		lblMonth.setBounds( 110, y0 + 7 * ysize + iTinyGap, 100, iNavBtnHeight );
		lblMonth.setVisible( true );
		add( lblMonth );

		btnNext = new JButton( "Next >>" );
		btnNext.setFont( new Font( fds.getFamily( ), Font.PLAIN, 10 ) );
		btnNext.setBounds( 220, y0 + 7 * ysize + 5, 88, iNavBtnHeight );
		btnNext.setVisible( true );
		btnNext.addActionListener( new ActionListener( )
		{
			public void actionPerformed(ActionEvent ae)
			{
				iMonth++;
				displayDate( );
			}
		} );
		add( btnNext );

		blNeverExpanded = false;
	}

	private void dropDownCalendar()
	{
		iMonth = Calendar.getInstance( ).get( Calendar.MONTH );
		iYear = Calendar.getInstance( ).get( Calendar.YEAR );

		setSize( iWidth, iMaxHeight );

		if (blNeverExpanded)
		{
			ConstructCalendar( );
		}
		else
		{
			btnPrev.setVisible( true );
			lblMonth.setVisible( true );
			btnNext.setVisible( true );
			makeCalendarButtonsVisible( true );
		}

		displayDate( );
		blExpanded = true;
	}

	private void foldUpCalendar()
	{
		setSize( iWidth, iMinHeight );
		btnPrev.setVisible( false );
		lblMonth.setVisible( false );
		btnNext.setVisible( false );
		makeCalendarButtonsVisible( false );
		blExpanded = false;
	}

	private void makeCalendarButtonsVisible(boolean blShow)
	{
		for (int i = 0; i < 49; i++)
			aDayButtons[i].setVisible( blShow );
	}

	private void displayDate()
	{
		for (int n = 7; n < aDayButtons.length; n++)
			aDayButtons[n].setText( "" );

		SimpleDateFormat sdf = new SimpleDateFormat( "MMMM yyyy" );
		Calendar cal = Calendar.getInstance( );
		cal.set( iYear, iMonth, 1 );
		int dayOfWeek = cal.get( Calendar.DAY_OF_WEEK );
		int daysInMonth = cal.getActualMaximum( Calendar.DAY_OF_MONTH );

		for (int n = 6 + dayOfWeek, sDay = 1; sDay <= daysInMonth; n++, sDay++)
			aDayButtons[n].setText( "" + sDay );

		lblMonth.setText( sdf.format( cal.getTime( ) ) );
	}

	private void setPickedDateText()
	{
		if (sDay.equals( "" )) return;

		Calendar cal = Calendar.getInstance( );
		cal.set( iYear, iMonth, Integer.parseInt( sDay ) );
		spnPickedDate.setValue( cal.getTime( ) );
	}
}
