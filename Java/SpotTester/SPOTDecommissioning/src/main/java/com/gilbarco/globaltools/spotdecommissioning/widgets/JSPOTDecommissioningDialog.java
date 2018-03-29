/**
 * 
 */

package com.gilbarco.globaltools.spotdecommissioning.widgets;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.GraphicsConfiguration;
import java.awt.Window;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToolBar;

import com.gilbarco.globaltools.spotdecommissioning.main.gui.SPOTDecommissioningComponentFormatter;

@SuppressWarnings("serial")
public class JSPOTDecommissioningDialog extends JDialog implements WindowListener
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

	public static Font getDefaultFont()
	{
		return SPOTDecommissioningComponentFormatter.getDefaultFont( );
	}

	public static Font getDefaultStatusFont()
	{
		return SPOTDecommissioningComponentFormatter.getDefaultStatusFont( );
	}

	public static Color getPanelDefaultBackgroundColor()
	{
		return SPOTDecommissioningComponentFormatter.getPanelDefaultBackgroundColor( );
	}

	public static Component setDefaultFont(Component c)
	{
		return SPOTDecommissioningComponentFormatter.setDefaultFont( c );
	}

	public static Component setDefaultStatusFont(Component c)
	{
		return SPOTDecommissioningComponentFormatter.setDefaultStatusFont( c );
	}

	public static Container setPanelDefaultBackgroundColor(Container c)
	{
		return SPOTDecommissioningComponentFormatter.setPanelDefaultBackgroundColor( c );
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
	/**
	 * The object from model, that is handled in this View, and should be provided by the controller between the derived dialog and the model. TODO this should be on the Controller
	 */
	private Object				dataModel;

	private volatile boolean	blDataLoaded;
	private volatile boolean	blSavingData;
	private JLabel				lblStatusText;
	private JTextField			txtInToolBar;
	private JPanel				pnlMain;
	private JPanel				pnlStatus;
	private JToolBar			tlbStatus;

	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************
	public JSPOTDecommissioningDialog()
	{
		commonConstructor( );
	}

	/**
	 * @param owner
	 */
	public JSPOTDecommissioningDialog(Frame owner)
	{
		super( owner );
		commonConstructor( );
	}

	/**
	 * @param owner
	 */
	public JSPOTDecommissioningDialog(Dialog owner)
	{
		super( owner );
		commonConstructor( );
	}

	/**
	 * @param owner
	 */
	public JSPOTDecommissioningDialog(Window owner)
	{
		super( owner );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param modal
	 */
	public JSPOTDecommissioningDialog(Frame owner, boolean modal)
	{
		super( owner, modal );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param title
	 */
	public JSPOTDecommissioningDialog(Frame owner, String title)
	{
		super( owner, title );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param modal
	 */
	public JSPOTDecommissioningDialog(Dialog owner, boolean modal)
	{
		super( owner, modal );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param title
	 */
	public JSPOTDecommissioningDialog(Dialog owner, String title)
	{
		super( owner, title );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param modalityType
	 */
	public JSPOTDecommissioningDialog(Window owner, ModalityType modalityType)
	{
		super( owner, modalityType );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param title
	 */
	public JSPOTDecommissioningDialog(Window owner, String title)
	{
		super( owner, title );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param title
	 * @param modal
	 */
	public JSPOTDecommissioningDialog(Frame owner, String title, boolean modal)
	{
		super( owner, title, modal );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param title
	 * @param modal
	 */
	public JSPOTDecommissioningDialog(Dialog owner, String title, boolean modal)
	{
		super( owner, title, modal );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param title
	 * @param modalityType
	 */
	public JSPOTDecommissioningDialog(Window owner, String title, ModalityType modalityType)
	{
		super( owner, title, modalityType );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param title
	 * @param modal
	 * @param gc
	 */
	public JSPOTDecommissioningDialog(Frame owner, String title, boolean modal, GraphicsConfiguration gc)
	{
		super( owner, title, modal, gc );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param title
	 * @param modal
	 * @param gc
	 */
	public JSPOTDecommissioningDialog(Dialog owner, String title, boolean modal, GraphicsConfiguration gc)
	{
		super( owner, title, modal, gc );
		commonConstructor( );
	}

	/**
	 * @param owner
	 * @param title
	 * @param modalityType
	 * @param gc
	 */
	public JSPOTDecommissioningDialog(Window owner, String title, ModalityType modalityType,
									GraphicsConfiguration gc)
	{
		super( owner, title, modalityType, gc );
		commonConstructor( );
	}

	/**
	 * This is the method invoked from ALL the other JSPOTDecommissioningDialog(...) constructors. Then this delegated "constructor" is in charge to construct the main panel (the area between caption and
	 * status bars), to set an initial null layout manager and apply the default settings to it.
	 */
	private void commonConstructor()
	{
		super.getContentPane( ).setLayout( new BorderLayout( 0, 0 ) );
		blDataLoaded = false;
		blSavingData = false;
		createMainPanel( );
		SPOTDecommissioningComponentFormatter.setPanelDefaultBackgroundColor( super.getContentPane( ) );
		SPOTDecommissioningComponentFormatter.setDefaultFont( super.getContentPane( ) );
		this.addWindowListener( this );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	/**
	 * Returns the main panel object (the area between caption and status bars). If you want to access to the dialog's pane, then call super.getContentPane() instead of this method.
	 * 
	 * @return JPanel : The main panel object of this JSPOTDecommissioningDialog.
	 * @see javax.swing.JDialog#getContentPane()
	 */
	@Override
	public Container getContentPane()
	{
		return pnlMain; // main panel is what user thinks in the dialog pane.
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.awt.event.WindowListener#windowOpened(java.awt.event.WindowEvent)
	 */
	@Override
	public void windowOpened(WindowEvent e)
	{
		if ((Window) this == e.getWindow( ))
											onOpened( );
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.awt.event.WindowListener#windowClosing(java.awt.event.WindowEvent)
	 */
	@Override
	public void windowClosing(WindowEvent e)
	{
		if ((Window) this == e.getWindow( ))
											onClosing( );
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.awt.event.WindowListener#windowClosed(java.awt.event.WindowEvent)
	 */
	@Override
	public void windowClosed(WindowEvent e)
	{
		if ((Window) this == e.getWindow( ))
											onClosed( );
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.awt.event.WindowListener#windowIconified(java.awt.event.WindowEvent)
	 */
	@Override
	public void windowIconified(WindowEvent e)
	{
		if ((Window) this == e.getWindow( ))
											onIconified( );
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.awt.event.WindowListener#windowDeiconified(java.awt.event.WindowEvent)
	 */
	@Override
	public void windowDeiconified(WindowEvent e)
	{
		if ((Window) this == e.getWindow( ))
											onDeiconified( );
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.awt.event.WindowListener#windowActivated(java.awt.event.WindowEvent)
	 */
	@Override
	public void windowActivated(WindowEvent e)
	{
		if ((Window) this == e.getWindow( ))
											onActivated( );
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.awt.event.WindowListener#windowDeactivated(java.awt.event.WindowEvent)
	 */
	@Override
	public void windowDeactivated(WindowEvent e)
	{
		if ((Window) this == e.getWindow( ))
											onDeactivated( );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	/**
	 * @return a dataModel reference to be used by the derived class dialog. This is intended to retrieve/save the data to be manipulated.
	 */
	public Object getDataModel()
	{
		return dataModel;
	}

	/**
	 * By this method the launcher of the derived class dialog can set the model data to be manipulated by the dialog.
	 * 
	 * @param dataModel
	 *            the dataModel to set
	 */
	public void setDataModel(Object dataModel)
	{
		this.dataModel = dataModel;
	}

	/**
	 * The flag saying "data was loaded into the dialog" is always set to true when either the OnActivated() or OnOpened() event response method have finished.
	 * 
	 * @return boolean saying if the polymorphic loadDialogData() was invoked to load the data according to the derived dialog class rules.
	 */
	public boolean isDataLoaded()
	{
		return blDataLoaded;
	}

	/**
	 * Default delegator to formatDialog(boolean, boolean, boolean) It applies ALL the default formats (font, status bar font, and the backgroud color.
	 * 
	 * @see formatDialog(boolean blMainFont, boolean blBkColor)
	 */
	public void formatDialog()
	{
		formatDialog( true, true, true );
	}

	/**
	 * Delegator to formatDialog(boolean, boolean, boolean) It conditionally applies the default component font and always applies the other default formats.
	 * 
	 * @param blMainFont
	 * @see formatDialog(boolean blMainFont, boolean blBkColor)
	 */
	public void formatDialog(boolean blMainFont)
	{
		formatDialog( blMainFont, true, true );
	}

	/**
	 * Delegator to formatDialog(boolean, boolean, boolean) It conditionally applies the default component font and the background color. It always applies the status bar font format.
	 * 
	 * @param blMainFont
	 * @param blBkColor
	 */
	public void formatDialog(boolean blMainFont, boolean blBkColor)
	{
		formatDialog( blMainFont, true, blBkColor );
	}

	/**
	 * Apply Gilbarco's GT settings to the panels and controls, and packs it. As you can see in the parameters list, there are three optional actions: 1) Setting the default background color for all
	 * the panels in the derived classes. 2) Setting the default font for all the components in the "main panel" derived classes. 3) Setting the default status font for all the components in status
	 * bar, in the derived classes.
	 * 
	 * @param blMainFont
	 *            controls 2)
	 * @param blStatusFont
	 *            controls 3)
	 * @param blBkColor
	 *            controls 1)
	 */
	public void formatDialog(boolean blMainFont, boolean blStatusFont, boolean blBkColor)
	{
		if (blBkColor)
						SPOTDecommissioningComponentFormatter.setPanelDefaultBackgroundColor( pnlMain );

		if (blMainFont)
						SPOTDecommissioningComponentFormatter.setDefaultFont( pnlMain );

		if (blStatusFont && pnlStatus != null)
												SPOTDecommissioningComponentFormatter.setDefaultStatusFont( pnlStatus );

		pack( );
	}

	public void addStatusBar(String sMainStatusText)
	{
		addStatusBar( sMainStatusText, true, "" );
	}

	public void addStatusBar(String sMainStatusText, boolean blOptBox)
	{
		addStatusBar( sMainStatusText, blOptBox, "" );
	}

	public void addStatusBar(String sMainStatusText, boolean blOptBox,
								String sToolBarText)
	{
		if (pnlStatus != null) return; // nothing to do

		pnlStatus = createStatusPanel( sMainStatusText, blOptBox, sToolBarText );
		SPOTDecommissioningComponentFormatter.setDefaultStatusFont( pnlStatus );
		pack( );
	}

	public void setMainStatusTextSize(int width, int height)
	{
		if (lblStatusText != null)
		{
			lblStatusText.setSize( width, height );
			lblStatusText.setPreferredSize( new Dimension( width, height ) );
		}
	}

	public void setInToolBarTextSize(int width, int height)
	{
		if (txtInToolBar != null)
		{
			txtInToolBar.setSize( width, height );
			txtInToolBar.setPreferredSize( new Dimension( width, height ) );
		}
	}

	public void setMainStatusText(String stxt)
	{
		if (lblStatusText != null) lblStatusText.setText( stxt );
	}

	public void setInToolBarText(String stxt)
	{
		if (txtInToolBar != null) txtInToolBar.setText( stxt );
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	/**
	 * 
	 * @return a reference to The Main Panel object.
	 */
	protected JPanel getMainPanel()
	{
		return pnlMain;
	}

	protected JPanel getStatusPanel()
	{
		return pnlStatus;
	}

	protected JToolBar getStatusToolBar()
	{
		return tlbStatus;
	}

	protected void markDataForSaving()
	{
		blSavingData = true;
	}

	protected boolean onOpened()
	{
		if (!blDataLoaded)
		{
			loadDialogData( );
			blDataLoaded = true;
		}
		return true;
	}

	protected boolean onClosing()
	{
		return true;
	}

	protected boolean onClosed()
	{
		if (blSavingData)
		{
			saveDialogData( );
			blSavingData = false;
		}

		clearDialogData( );
		blDataLoaded = false;
		return true;
	}

	protected boolean onIconified()
	{
		return true;
	}

	protected boolean onDeiconified()
	{
		return true;
	}

	protected boolean onActivated()
	{
		if (!blDataLoaded)
		{
			loadDialogData( );
			blDataLoaded = true;
		}

		synchronized (this)
		{
			notifyAll( ); // notify to any waiting thread, the data has already been loaded.
		}

		return true;
	}

	protected boolean onDeactivated()
	{
		return true;
	}

	/**
	 * Called ONCE when windowActivated() or windowOpened() events are delivered to this window/dialog as a target, no matter which event comes first. Derived dialog classes must be responsible to
	 * override this function. You may want to use getDataModel() to get the reference to the model and retrieve the data for you derived class dialog. {@link dataModel} {@link getDataModel}
	 * 
	 */
	protected boolean loadDialogData()
	{
		return true;
	}

	/**
	 * Called first of all when windowClosed() event is delivered to this window/dialog as a target. Calling to this function or overridden one shall not automatically be performed by this base class
	 * unless you explicitly set the need to do it. This is achieved by calling needToSaveData() before you close the dialog, by means of <code>closeDialog()</code> for instance. Derived dialog
	 * classes must be responsible to override this function. You may want to use getDataModel() to get the reference to the model and save the data from you derived class dialog onto the model.
	 * {@link dataModel} {@link getDataModel}
	 * 
	 */
	protected boolean saveDialogData()
	{
		return true;
	}

	/**
	 * Called immediately after saveDialogData() when windowClosed() event is delivered to this window/dialog as a target. Derived dialog classes must be responsible to override this function
	 * 
	 */
	protected boolean clearDialogData()
	{
		return true;
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private JPanel createMainPanel()
	{
		pnlMain = new JPanel( );
		super.getContentPane( ).add( pnlMain, BorderLayout.CENTER );

		return pnlMain;
	}

	@SuppressWarnings("unused")
	private JPanel createStatusPanel(String strStatus)
	{
		return createStatusPanel( strStatus, true, "" );
	}

	@SuppressWarnings("unused")
	private JPanel createStatusPanel(String strStatus, boolean blOptBox)
	{
		return createStatusPanel( strStatus, blOptBox, "" );
	}

	/**
	 * @param strStatus
	 *            is the main status text in the tool bar.
	 * @param blOptBox
	 *            if you want to create the optional JTextField inside the toolbar.
	 * @param strInToolBar
	 *            the text inside the JTextField in the toolbar.
	 * @return
	 */
	private JPanel createStatusPanel(String strStatus, boolean blOptBox,
										String strInToolBar)
	{
		pnlStatus = new JPanel( );
		JSeparator separator = new JSeparator( );

		pnlStatus.setLayout( new BorderLayout( 0, 0 ) );
		pnlStatus.setSize( 300, 22 );
		pnlStatus.add( separator, BorderLayout.NORTH );

		createStatusToolBar( strStatus, blOptBox, strInToolBar );
		pnlStatus.add( tlbStatus, BorderLayout.CENTER );

		super.getContentPane( ).add( pnlStatus, BorderLayout.SOUTH );
		return pnlStatus;
	}

	/**
	 * @param strStatus
	 *            is the main status text in the tool bar.
	 * @param blOptBox
	 *            if you want to create the optional JTextField inside the toolbar.
	 * @param strInToolBar
	 *            the text inside the JTextField in the toolbar.
	 * @return
	 */
	private JToolBar createStatusToolBar(String strStatus, boolean blOptBox,
											String strInToolBar)
	{
		tlbStatus = new JToolBar( );
		lblStatusText = new JLabel( strStatus );
		txtInToolBar = new JTextField( strInToolBar );

		tlbStatus.setPreferredSize( new Dimension( 300, 22 ) );
		lblStatusText.setPreferredSize( new Dimension( 400, 22 ) );
		txtInToolBar.setPreferredSize( new Dimension( 50, 22 ) );

		txtInToolBar.setEditable( false );
		txtInToolBar.setVisible( blOptBox );
		tlbStatus.setFloatable( false );

		tlbStatus.add( lblStatusText, BorderLayout.CENTER );
		tlbStatus.addSeparator( );
		tlbStatus.add( txtInToolBar, BorderLayout.EAST );
		tlbStatus.addSeparator( );

		return tlbStatus;
	}

	// ******************************************************************
	// MAIN.
	// ******************************************************************

	public static Font getDefaultTitlePanelFont()
	{
		return SPOTDecommissioningComponentFormatter.getDefaultTitlePanelFont( );
	}

	public static Font getDefaultSmallFont()
	{
		return SPOTDecommissioningComponentFormatter.getDefaultSmallFont( );
	}

	public static Font getDefaultLargeFont()
	{
		return SPOTDecommissioningComponentFormatter.getDefaultLargeFont( );
	}

	public static Font getDefaultButtonFont()
	{
		return SPOTDecommissioningComponentFormatter.getDefaultButtonFont( );
	}

}
