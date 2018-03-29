package com.gilbarco.globaltools.gcm.gui;

import java.awt.HeadlessException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.lang.reflect.Method;

import javax.swing.JFileChooser;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppController;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.gcm.engines.DataTransportLayer;
import com.gilbarco.globaltools.gcm.engines.SocketTransportLayer;
import com.gilbarco.globaltools.gcm.model.GCMContext;
import com.gilbarco.globaltools.gcm.state.GCMMessageBroker;
import com.gilbarco.globaltools.gcm.state.GCMStateManager;

public class GCMICCController extends EdtAppController<GCMICCView, GCMICCModel> {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog(GCMICCController.class);
	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected GCMICCModel				model;
	protected GCMStateManager			stateManager;
	protected GCMMessageBroker			messageBroker;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public GCMICCController(GCMICCView view) {
		super(view);
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	@Override
	public void initController() {
		this.model = new GCMICCModel();
		this.setAppModel(model);
		
		try {
			this.stateManager = new GCMStateManager();
			this.messageBroker = new GCMMessageBroker(this.createDataTransportLayer(), this.model);
			this.model.setMessageBroker(this.messageBroker);
		}
		catch (EdtServiceException e) {
			logger.error(e.getMessage(), e);
		}
		
		GCMContext.setAppLocale(this.getAppLocale());
	}

	public void startButtonAction() {
		this.model.setStarted(true);
		this.updateModel();
		this.stateManager.startTest();
	}

	public void stopButtonAction() {
		this.stateManager.stopTest();
		this.model.setStarted(false);
		this.model.modelUpdated();
	}

	public GCMICCModel getModel() {
		this.updateModel();
		return this.model;
	}

	@Override
	public void updateMenuItemsState(JMenu menu) {
	}

	@Override
	public void addMenuItems(JMenu menu) {
		JMenuItem sendFile = new JMenuItem(this.getLocaleText("SendEMVTable"));
		sendFile.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					showLoadScreen(messageBroker.getClass().getMethod("sendEMVTableFile", File.class), messageBroker);
				}
				catch (Exception e1) {
					logger.error(e1.getMessage(), e1);
				}
			}

		});
		JMenuItem oTICommand = new JMenuItem(this.getLocaleText("SendOTICommands"));
		oTICommand.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					showLoadScreen(messageBroker.getClass().getMethod("sendOTICommands", File.class), messageBroker);
				}
				catch (Exception e1) {
					logger.error(e1.getMessage(), e1);
				}
			}
		});

		menu.add(sendFile);
		menu.add(oTICommand);

	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected DataTransportLayer createDataTransportLayer() {
		return new SocketTransportLayer();
	}
	
	@Override
	protected void onEdtAppModelChanged(GCMICCModel model) {
		this.view.otiCmdFileField.setText(model.getOtiCmdFilePath());
		this.view.iccLogFileField.setText(model.getIccLogFilePath());
		this.view.noteField.setText(model.getNotes());
		this.view.statusField.setText(model.getStatus());
		this.view.track1Field.setText(model.getTrack1());
		this.view.track2Field.setText(model.getTrack2());
		this.view.track3Field.setText(model.getTrack3());
		this.updateButtons(model.isLogged(), model.isStarted());
	}

	protected void updateModel() {
		this.model.setTestId(this.view.testIdField.getText());
		this.model.setNotes(this.view.noteField.getText());
		this.model.setIccLogFilePath(this.view.iccLogFileField.getText());
		this.model.setOtiCmdFilePath(this.view.otiCmdFileField.getText());
		this.model.setStatus(this.view.statusField.getText());
		this.model.setPersistentConfig(this.view.persistentChkBox.isSelected());
		this.model.setGcmLogPath(this.view.gcmLogPathField.getText());
	}

	protected void updateButtons(boolean logged, boolean started) {
		if (logged) {
			view.btnStartTest.setEnabled(!started);
			view.btnStopTest.setEnabled(started);
		}
		else {
			view.btnStartTest.setEnabled(false);
			view.btnStopTest.setEnabled(false);
		}
	}

	protected File showLoadScreen(Method action, Object obj) {
		try {
			JFileChooser fc = new JFileChooser();
			fc.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
			int returnValue = fc.showOpenDialog(this.getView());
			if (returnValue == JFileChooser.APPROVE_OPTION) {
				File file = fc.getSelectedFile();
				if (file.isFile()) {
					if (action != null) {
						try {
							action.invoke(obj, file);
						}
						catch (Exception e1) {
							logger.error(e1.getMessage(), e1);
						}
					}
					return file;
				}
				else {
					showMessage(this.getLocaleText("FileCheck"));
				}
			}
		}

		catch (HeadlessException e) {
			logger.error(e.getMessage(), e);
		}
		return new File("");
	}

	protected void showMessage(String message) {
		JOptionPane.showMessageDialog(null, message);
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
