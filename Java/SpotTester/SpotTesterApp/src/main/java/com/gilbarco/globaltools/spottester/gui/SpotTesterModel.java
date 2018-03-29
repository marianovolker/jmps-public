package com.gilbarco.globaltools.spottester.gui;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.gui.app.EdtAppModel;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.managers.SpotTesterDataManager;

public class SpotTesterModel extends EdtAppModel {
   // ******************************************************************
   // STATIC FIELDS
   // ******************************************************************
   private static final transient Log logger = LogFactory.getLog(SpotTesterModel.class);
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
   private boolean                    connected;
   private boolean                    logged;

   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************

	public SpotTesterModel() {
      super();
      connected = true;
      logged = false;
		try {
			EdtServiceContext.getServiceManager().subscribe(getConnectAction());
			EdtServiceContext.getServiceManager().subscribe(getLoggedAction());
      }
		catch (EdtServiceException e) {
         logger.error(e.getMessage(), e);
      }

   }

   // ******************************************************************
   // PUBLIC METHODS (general, getter, setter, interface imp)
   // ******************************************************************
	public boolean isLogged() {
      return logged;
   }

	public void setLogged(boolean logged) {
      this.logged = logged;
      if(this.logged)
    	  this.setConnected(true);
   }

	public boolean isConnected() {
      return connected;
   }

	public void setConnected(boolean connected) {
      this.connected = connected;
      if(!this.connected)
    	  this.setLogged(false);
   }

   // ******************************************************************
   // PROTECTED METHODS.
   // ******************************************************************

   // ******************************************************************
   // PRIVATE METHODS.
   // ******************************************************************

	private void updateController() {
      setChanged();
      notifyObservers();
   }
   
	private EdtEventAction<?> getLoggedAction() {
		return new EdtEventAction<MgpResultEvent>() {

         @Override
			public void onInvokeEvent(MgpResultEvent event) {
            if (event.getMessageName().equals(SpotMessageType.LOGIN.getName()))
               setLogged(true);
            if (event.getMessageName().equals(SpotMessageType.LOGOUT.getName()))
                setLogged(false);
            
            updateController();
         }

         @Override
			public Class<MgpResultEvent> getEventClass() {
            return MgpResultEvent.class;
         }

         @Override
			public String getSubscriptionFilter() {
            return MgpEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT + "'";
         }

         @Override
			public boolean justOnce() {
            return false;
         }
      };
   }

	private EdtEventAction<?> getConnectAction() {
		return new EdtEventAction<EdtClientSocketEvent>() {

         @Override
			public void onInvokeEvent(EdtClientSocketEvent event) {
            setConnected(SpotTesterDataManager.getInstance().isConnect());

            updateController();
         }

         @Override
			public Class<EdtClientSocketEvent> getEventClass() {
            return EdtClientSocketEvent.class;
         }

         @Override
			public String getSubscriptionFilter() {
				return EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.CLOSE_SOCKET.name() + "' OR "
						+ EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.SOCKET_DISCONNECT.name() + "' OR "
						+ EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.CONNECT.name() + "' OR "
						+ EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.CONNECTED.name() + "'";
         }

         @Override
			public boolean justOnce() {
            return false;
         }
      };
   }

   // ******************************************************************
   // INNER CLASSES.
   // ******************************************************************

   // ******************************************************************
   // MAIN.
   // ******************************************************************

}
