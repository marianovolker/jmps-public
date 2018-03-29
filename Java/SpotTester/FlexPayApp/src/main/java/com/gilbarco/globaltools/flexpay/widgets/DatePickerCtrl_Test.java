
package com.gilbarco.globaltools.flexpay.widgets;

/**
 * @author Guillermo M. Paris
 * @date 24/4/2012
 * 
 * This is a quick functional test (standalone java build) for the widget
 * DatePickerCtrl
 */

import java.awt.Dimension;
import java.awt.Font;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;

public class DatePickerCtrl_Test
{
	/**
	 * Create the GUI and show it. For thread safety, this method should be invoked from the event-dispatching thread.
	 */
	private static void createAndShowGUI()
	{
		JLabel label = new JLabel( "Selected Date:" );
		label.setFont( new Font( "Verdana", Font.PLAIN, 14 ) );
		label.setBounds( 25, 35, 150, 17 );

		JLabel lblPic = new JLabel( "." );
		lblPic.setIcon( new ImageIcon( lblPic.getClass( ).getResource( "/images/Calendar.png" ) ) );
		lblPic.setBounds( 55, 50, 32, 32 );

		final JFrame mainFrame = new JFrame( );
		mainFrame.getContentPane( ).setLayout( null );
		// mainFrame.getContentPane().add(label);
		mainFrame.add( label );
		mainFrame.add( lblPic );

		DatePickerCtrl datePicker = new DatePickerCtrl( mainFrame.getContentPane( ), 145, 25 );
		datePicker.setOpaque( true );
		mainFrame.add( datePicker );

		mainFrame.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE ); // RECOMENDED!!
		mainFrame.setPreferredSize( new Dimension( 640, 480 ) );
		mainFrame.pack( );
		mainFrame.setVisible( true );

	}

	public static void main(String[] args)
	{
		// Schedule a job for the event-dispatching thread:
		// creating and showing this application's GUI.
		javax.swing.SwingUtilities.invokeLater( new Runnable( )
		{
			public void run()
			{
				createAndShowGUI( );
			}
		} );
	}
}
