package com.gilbarco.globaltools.spottester.gui;

import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JMenu;
import javax.swing.JRadioButton;
import javax.swing.JToolBar;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.edt.gui.widgets.EdtInputGui;
import com.gilbarco.globaltools.edt.gui.widgets.EdtLogGui;
import com.gilbarco.globaltools.spottester.managers.SpotTesterDataManager;

/**
 * @class SpotTesterView
 * @super EdtAppView
 * 
 * @Description This class is in charge of the GUI configuration.
 * 
 * @see SpotTesterController
 * @see EdtLogGui
 * @see EdtAppView
 * 
 * @version 1.0.0
 * 
 * @author ajadzinsky Sep 27, 2010
 */
public class SpotTesterView extends EdtAppView<SpotTesterController> {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final long serialVersionUID = 1L;
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
	private EdtInputGui input;
	private EdtLogGui logGui;
	protected JRadioButton connected;
	protected JRadioButton logged;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SpotTesterView() {
		super();
		initialize();
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public void initialize() {
		this.setLayout(new BorderLayout());

		this.input = new EdtInputGui();
		this.input.setDataSender(SpotTesterDataManager.getInstance());
		this.add(this.input, BorderLayout.NORTH);
		
		JButton btnNewButton = new JButton("Close");
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				getRootApp( ).getController( ).setSelectedIndex(1  );
			}
		});
		input.add(btnNewButton, BorderLayout.EAST);

		this.logGui = new EdtLogGui();
		this.add(this.logGui, BorderLayout.CENTER);

		connected = new JRadioButton(this.getAppLocale().getLocaleText(	"Connected"), false);
		connected.setFont(new Font("Tahoma",Font.BOLD,11));
		connected.setEnabled(false);
		logged = new JRadioButton(this.getAppLocale().getLocaleText("Logged"), false);
		logged.setFont(new Font("Tahoma",Font.BOLD,11));
		logged.setEnabled(false);
	}

	@Override
	public String getAppName() {
		return "SpotTester";
	}

	@Override
	public Icon getAppIcon() {
		return null;
	}

	@Override
	public JMenu getAppMenu() {
		JMenu menu = new JMenu(this.getAppLocale().getLocaleText("SpotTester"));
		this.controller.addMenuItems(menu);
		menu.add(menu);
		return menu;
	}

	@Override
	public void addStatusBarComponent(JToolBar toolBar) {
		toolBar.add(this.connected);
		//toolBar.add(this.logged);
		toolBar.addSeparator();
	}
	
	/**
	 * @return the logGui
	 */
	public EdtLogGui getLogGui() {
		return logGui;
	}

	public JRadioButton getConnected() {
		return connected;
	}

	public void setConnected(JRadioButton connected) {
		this.connected = connected;
	}

	public JRadioButton getLogged() {
		return logged;
	}

	public void setLogged(JRadioButton logged) {
		this.logged = logged;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	@Override
	protected SpotTesterController createController() {
		return new SpotTesterController(this);
	}
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
