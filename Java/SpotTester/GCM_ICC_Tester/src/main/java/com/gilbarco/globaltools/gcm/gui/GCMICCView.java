package com.gilbarco.globaltools.gcm.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.BoxLayout;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JMenu;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.edt.gui.widgets.fields.LabelTextComponent;
import com.gilbarco.globaltools.edt.util.EdtFileUtil;

import javax.swing.UIManager;

public class GCMICCView extends EdtAppView<GCMICCController> {

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final long		serialVersionUID	= 1L;

	protected JButton				btnStartTest;
	protected JButton				btnStopTest;
	protected JButton				loadOTI;
	protected JCheckBox				persistentChkBox;

	protected LabelTextComponent	testIdField;
	protected LabelTextComponent	iccLogFileField;
	protected LabelTextComponent	otiCmdFileField;
	protected LabelTextComponent	noteField;

	protected JPanel				resultPanel;
	protected LabelTextComponent	statusField;
	protected LabelTextComponent	track1Field;
	protected LabelTextComponent	track2Field;
	protected LabelTextComponent	track3Field;
	private JPanel					OTIPanel;
	protected LabelTextComponent	gcmLogPathField;
	private JPanel					gcmLogPanel;
	private JButton					gcmLogPathBtn;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public GCMICCView() {
		super();
		initialize();
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public void initialize() {
		setLayout(new BorderLayout(3, 3));

		JPanel centerPanel = new JPanel();
		add(centerPanel, BorderLayout.CENTER);
		centerPanel.setLayout(new GridLayout(0, 1, 0, 0));

		JPanel testPanel = new JPanel();
		centerPanel.add(testPanel);
		testPanel.setBorder(new TitledBorder(null, "Test", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		testPanel.setLayout(new GridLayout(0, 1, 0, 0));

		this.testIdField = new LabelTextComponent(this.getAppLocale().getLocaleText("TestID"));
		this.testIdField.setTextEnabled(true);
		this.testIdField.setTextEditable(true);
		testPanel.add(testIdField);

		this.iccLogFileField = new LabelTextComponent(this.getAppLocale().getLocaleText("ICCLogLocationLabel"));
		testPanel.add(iccLogFileField);

		this.noteField = new LabelTextComponent(this.getAppLocale().getLocaleText("TestNotes"));
		testPanel.add(noteField);

		OTIPanel = new JPanel();
		testPanel.add(OTIPanel);
		OTIPanel.setLayout(new BoxLayout(OTIPanel, BoxLayout.X_AXIS));

		this.otiCmdFileField = new LabelTextComponent(this.getAppLocale().getLocaleText("otiConfigFile"));
		OTIPanel.add(otiCmdFileField);

		JButton loadOTICmdFile = new JButton("...");
		OTIPanel.add(loadOTICmdFile);
		loadOTICmdFile.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				otiCmdFileField.setText(controller.showLoadScreen(null, null).getPath());
			}
		});

		loadOTICmdFile.setToolTipText(this.getAppLocale().getLocaleText("LoadOTI"));

		persistentChkBox = new JCheckBox("");
		OTIPanel.add(persistentChkBox);
		persistentChkBox.setToolTipText(this.getAppLocale().getLocaleText("persistentCmndToolTip"));

		gcmLogPanel = new JPanel();
		testPanel.add(gcmLogPanel);
		gcmLogPanel.setLayout(new BoxLayout(gcmLogPanel, BoxLayout.X_AXIS));

		gcmLogPathField = new LabelTextComponent(this.getAppLocale().getLocaleText("gcmLogPath"));
		gcmLogPanel.add(gcmLogPathField);
		
		gcmLogPathBtn = new JButton("...");
		gcmLogPanel.add(gcmLogPathBtn);
		gcmLogPathBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				File folder = EdtFileUtil.openFolder(gcmLogPathField.getText(), getAppLocale().getLocaleText("gcmLogPath"));
				gcmLogPathField.setText(folder == null ? gcmLogPathField.getText() : folder.getPath());
			}
		});
		this.resultPanel = new JPanel();
		this.resultPanel.setBorder(new TitledBorder(new LineBorder(new Color(184, 207, 229)), "Result",
													TitledBorder.LEADING, TitledBorder.TOP, null, new Color(51, 51, 51)));
		this.resultPanel.setLayout(new GridLayout(0, 1, 0, 0));

		this.statusField = new LabelTextComponent(this.getAppLocale().getLocaleText("StatusLabel"));
		statusField.getTextField().setBackground(UIManager.getColor("Button.background"));
		statusField.getTextField().setForeground(Color.BLACK);
		this.statusField.setTextEnabled(false);
		this.statusField.setTextEditable(false);
		this.resultPanel.add(this.statusField);
		centerPanel.add(this.resultPanel);

		this.track1Field = new LabelTextComponent(this.getAppLocale().getLocaleText("Track1Label"));
		this.track1Field.setTextEnabled(false);
		this.track1Field.setTextEditable(false);
		this.resultPanel.add(this.track1Field);

		this.track2Field = new LabelTextComponent(this.getAppLocale().getLocaleText("Track2Label"));
		this.track2Field.setTextEnabled(false);
		this.track2Field.setTextEditable(false);
		this.resultPanel.add(this.track2Field);

		this.track3Field = new LabelTextComponent(this.getAppLocale().getLocaleText("Track3Label"));
		this.track3Field.setTextEditable(false);
		this.track3Field.setTextEnabled(false);
		this.resultPanel.add(this.track3Field);

		JPanel southPanel = new JPanel();
		add(southPanel, BorderLayout.SOUTH);

		this.btnStartTest = new JButton(this.getAppLocale().getLocaleText("StartTestButton"));
		this.btnStartTest.setEnabled(false);
		this.btnStopTest = new JButton(this.getAppLocale().getLocaleText("StopTestButton"));
		this.btnStopTest.setEnabled(false);
		southPanel.setLayout(new FlowLayout(FlowLayout.RIGHT, 5, 5));
		southPanel.add(this.btnStartTest);
		southPanel.add(this.btnStopTest);
		this.btnStopTest.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				controller.stopButtonAction();
			}
		});
		this.btnStartTest.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (canStartTest()) {
					controller.startButtonAction();
				}
				else {
					controller.showMessage(getAppLocale().getLocaleText("ViewAvailable"));
				}
			}
		});
	}

	@Override
	public JMenu getAppMenu() {
		JMenu menu = new JMenu("GCM");
		controller.addMenuItems(menu);
		return menu;
	}

	@Override
	public String getAppName() {
		return "GCM";
	}

	@Override
	public Icon getAppIcon() {
		return null;
	}

	@Override
	public void addStatusBarComponent(JToolBar toolBar) {
	}
	
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	@Override
	protected GCMICCController createController() {
		return new GCMICCController(this);
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private boolean canStartTest() {
		return this.testIdField.getText() != null && !this.testIdField.getText().equals("");
	}
}