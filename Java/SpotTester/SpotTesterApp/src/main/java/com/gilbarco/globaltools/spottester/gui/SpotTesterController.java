package com.gilbarco.globaltools.spottester.gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenu;
import javax.swing.JMenuItem;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppController;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.mgp.api.messageformatter.MgpMessageFormatterType;
import com.gilbarco.globaltools.spottester.managers.SpotTesterDataManager;
import com.gilbarco.globaltools.spottester.managers.SpotTesterLoggingManager;
import com.gilbarco.globaltools.spottester.managers.SpotTesterStateManager;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/**
 * @Class SpotTesterController
 * 
 * @Description This class is the middleman between the application and the GUI. It builds the menus and fill the options with theirs callbacks.
 * 
 * @see SpotTesterDataManager
 * 
 * @version 1.0.0
 * 
 * @author ajadzinsky Sep 27, 2010
 * 
 */
public class SpotTesterController extends EdtAppController<SpotTesterView, SpotTesterModel>{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog(SpotTesterController.class);

	//According to the spec is 0x43 but Alejandro J. said to me that Stefano changed to 0x45. Both answers came empty. 
	private static final byte FILE_TYPE_TO_BROWSE = 0x45;
	
	private static final byte	SPOT_CLIENT_ID	= (byte) 0x64;
	
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
	private SpotTesterLoggingManager	loggingManager;
	private SpotTesterModel model;
	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SpotTesterController(SpotTesterView view) {
		super(view);
		this.model = new SpotTesterModel();
		setAppModel(model);
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	/**
	 * @Description Fill the commands sub-menu.
	 * @param commands
	 *            Root of the commands sub-menu.
	 * */
	@Override
	public void addMenuItems(JMenu menu) {
		JMenu commands = new JMenu(this.view.getAppLocale().getLocaleText("Commands"));
		commands.add(this.getConnectMenuItem());
		commands.add(this.getDisconnectMenuItem());
		commands.add(this.getLoginMenuItem());
		commands.add(this.getLogoutMenuItem());
		commands.add(this.getFileBrowseItem());
		JMenu logging = new JMenu(this.view.getAppLocale().getLocaleText("Logging"));
		JMenu loggingStyle = new JMenu(this.view.getAppLocale().getLocaleText("LoggingStyle"));
		loggingStyle.add(this.getLoggingSingleMode());
		logging.add(loggingStyle);
		logging.add(this.getLogginLevelMenu());
		menu.add(commands);
		menu.add(logging);
	}

	@Override
	public void updateMenuItemsState(JMenu menu) {
	}

	@Override
	public void initController() {
		try {
			loggingManager = new SpotTesterLoggingManager(view);
			new SpotTesterStateManager();
		}
		catch (EdtServiceException e) {
			logger.error(e.getMessage(), e);
		}
	}

	@Override
	protected void onEdtAppModelChanged(SpotTesterModel model) {
	   this.view.connected.setSelected(model.isConnected());
	   this.view.logged.setSelected(model.isLogged());
	}

	protected void updateModel()
	{
	 //  this.model.setConnected(true);
	 //  this.model.setLogged(true);
	}
	
	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private JMenu getLogginLevelMenu() {
		JMenu loggingLevel = new JMenu(this.view.getAppLocale().getLocaleText("LoggingLevel"));
		final JCheckBoxMenuItem rawLevel = new JCheckBoxMenuItem(this.view.getAppLocale().getLocaleText("LoggingRawLevel"));
		rawLevel.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				loggingManager.setLogRawMessages(e.getStateChange() == ItemEvent.SELECTED);
			}
		});
		//rawLevel.doClick();
		final JCheckBoxMenuItem decodedLevel = new JCheckBoxMenuItem(this.view.getAppLocale().getLocaleText(
			"LoggingDecodedLevel"));
		decodedLevel.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				loggingManager.setLogDecodedMessages(e.getStateChange() == ItemEvent.SELECTED);
			}
		});
		//decodedLevel.doClick();
		loggingLevel.add(rawLevel);
		loggingLevel.add(decodedLevel);
		return loggingLevel;
	}

	private JMenuItem getLoggingSingleMode() {
		JMenuItem item = new JMenuItem(this.view.getAppLocale().getLocaleText("SingleLineFormatter"));
		item.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				changeFormatter(MgpMessageFormatterType.SINGLE_LINE_MESSAGE);
			}
		});

		return item;
	}

	private JMenuItem getLoginMenuItem() {
		JMenuItem item = new JMenuItem(this.view.getAppLocale().getLocaleText("Login"));
		item.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				SpotTesterDataManager manager = SpotTesterDataManager.getInstance();
				if(!manager.isConnect())
				{
				   try
				{
					manager.connect();
				}
				catch (EdtServiceException e1)
				{
					logger.debug("error:"+ e1.getMessage( ));
				}
				}
				manager.sendMessage(SpotMessages.getInstance().getSpotLoginData(SPOT_CLIENT_ID));
				updateModel();
			}
		});

		return item;
	}

	private JMenuItem getLogoutMenuItem() {
		JMenuItem item = new JMenuItem(this.view.getAppLocale().getLocaleText("Logout"));
		item.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				SpotTesterDataManager manager = SpotTesterDataManager.getInstance();
				manager.sendMessage(SpotMessages.getInstance().getSpotLogoutData());
//				manager.closeSocket();
			}
		});

		return item;
	}

	private JMenuItem getFileBrowseItem() {
		JMenuItem fileBrowse = new JMenuItem(this.view.getAppLocale().getLocaleText("FileBrowse"));
		fileBrowse.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				SpotTesterDataManager.getInstance().sendMessage(SpotMessages.getInstance().getFileBrowse(FILE_TYPE_TO_BROWSE));
			}
		});

		return fileBrowse;
	}

	private void changeFormatter(MgpMessageFormatterType type) {
		logger.debug("Formatter changed from " + loggingManager.getFormatter().name() + " to " + type.name());
		loggingManager.setFormatter(type);
	}
	
	private JMenuItem getConnectMenuItem() {
		JMenuItem connect = new JMenuItem(this.view.getAppLocale().getLocaleText("ConnectMenu"));
		connect.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try
				{
					SpotTesterDataManager.getInstance().connect();
				}
				catch (EdtServiceException e1)
				{
					logger.debug("error:"+ e1.getMessage( ));

				}
			}
		});
		return connect;
	}
	
	private JMenuItem getDisconnectMenuItem() {
      JMenuItem disconnect = new JMenuItem(this.view.getAppLocale().getLocaleText("DisconnectMenu"));
      disconnect.addActionListener(new ActionListener() {
         @Override
         public void actionPerformed(ActionEvent e) {
            SpotTesterDataManager.getInstance().closeSocket();
         }
      });
      return disconnect;
   }
	
	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
