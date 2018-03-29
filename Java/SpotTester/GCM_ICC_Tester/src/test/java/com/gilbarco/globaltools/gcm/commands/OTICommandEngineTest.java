package com.gilbarco.globaltools.gcm.commands;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.junit.Test;
import org.springframework.util.Assert;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.EdtServiceManager;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketAdapter;
import com.gilbarco.globaltools.edt.services.io.socket.EdtClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.gcm.engines.OTICommandEngine;
import com.gilbarco.globaltools.gcm.engines.SocketTransportLayer;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.edt.service.MgpServiceAdapter;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;

public class OTICommandEngineTest
{
   // ******************************************************************
   // STATIC FIELDS
   // ******************************************************************
   private static final transient Log logger = LogFactory.getLog(OTICommandEngineTest.class);
   private static final boolean PERSISTANCE = false;

   // ******************************************************************
   // STATIC METHODS
   // ******************************************************************
   public static void main(String[] args)
   {
      OTICommandEngineTest engine = new OTICommandEngineTest();
      engine.start();
      try
      {
         engine.setContinueTest(true);
         while (engine.isContinueTest())
         {
            Thread.sleep(100);
         }
         System.out.println("Done ...");
         Thread.sleep(1000);
          System.exit(0);
      }
      catch (InterruptedException e)
      {
      }
      
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
   private EdtClientSocketAdapter socketService;
   private MgpServiceAdapter      mgpService;
   private OTICommandEngine       OTIEngine;
   private boolean                continueTest;

   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************

   public boolean isContinueTest()
   {
      return continueTest;
   }

   public void setContinueTest(boolean continueTest)
   {
      this.continueTest = continueTest;
   }

   public OTICommandEngineTest()
   {
      super();
      try
      {
         OTIEngine = new OTICommandEngine(new SocketTransportLayer(), PERSISTANCE);
      }
      catch (EdtServiceException e)
      {
         e.printStackTrace();
      }
   }

   public void startTest()
   {
      OTIEngine.setCommands(new File("src/test/resources/OTITest.txt"));
      OTIEngine.startConfigProcess(PERSISTANCE);
   }

   public void stopTest(boolean ok, String msg)
   {
      OTIEngine.endConfigProcess(ok, msg);
   }
   
   @Test
   public void dummy(){
	   
   }

   // ******************************************************************
   // PUBLIC METHODS (general, getter, setter, interface imp)
   // ******************************************************************

   // ******************************************************************
   // PROTECTED METHODS.
   // ******************************************************************

   // ******************************************************************
   // PRIVATE METHODS.
   // ******************************************************************
   private void start()
   {
      loadServices();
      login();
   }

   private void loadServices()
   {
      EdtServiceManager manager = EdtServiceContext.getServiceManager();
      if (this.socketService == null)
      {
         try
         {
            this.socketService = (EdtClientSocketAdapter) manager.getServiceAdapter("ClientSocketDefinition");
            if (this.mgpService == null)
            {
               try
               {
                  this.mgpService = (MgpServiceAdapter) manager.getServiceAdapter("SpotServiceDefinition");
                  Assert.notNull(this.mgpService, "MGP Service was not created");

               }
               catch (EdtServiceException e)
               {
                  logger.error("MgpService ERROR: " + e.getMessage(), e);
                  e.printStackTrace();
               }
            }
            Assert.notNull(this.socketService);

            EdtServiceContext.getServiceManager().subscribe(new EdtEventAction<MgpResultEvent>()
            {
               @Override
               public void onInvokeEvent(MgpResultEvent event)
               {
                  startTest();
               }

               @Override
               public Class<MgpResultEvent> getEventClass()
               {
                  return MgpResultEvent.class;
               }

               @Override
               public String getSubscriptionFilter()
               {
                  return MgpResultEvent.MESSAGE_NAME + " ='" + SpotMessageType.LOGIN.getName() + "' AND " + 
                          MgpEvent.EVENT_TYPE_FILTER + " ='" + EdtMgpType.RESPONSE_RESULT.name() + "'"; 
               }

               @Override
               public boolean justOnce()
               {
                  return false;
               }
            });

            EdtServiceContext.getServiceManager().subscribe(new EdtEventAction<MgpResultEvent>()
            {
               @Override
               public void onInvokeEvent(MgpResultEvent event)
               {
                  System.out.println(event.getMessageName() + " - type : " + event.getEventType().name());
                  if (!event.isValid())
                  {
                     continueTest = false;
                     stopTest(false, "ERRORRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR");
                  }
               }

               @Override
               public Class<MgpResultEvent> getEventClass()
               {
                  return MgpResultEvent.class;
               }

               @Override
               public String getSubscriptionFilter()
               {
                  return MgpResultEvent.MESSAGE_NAME + " ='" + SpotMessageType.OTI_DO.getName() +"' AND " + 
                  MgpEvent.EVENT_TYPE_FILTER + " ='" + EdtMgpType.RESPONSE_RESULT.name() + "'"; 
                  // return "";
               }

               @Override
               public boolean justOnce()
               {
                  return false;
               }
            });

            EdtServiceContext.getServiceManager().subscribe(new EdtEventAction<EdtClientSocketEvent>()
            {

               @Override
               public void onInvokeEvent(EdtClientSocketEvent event)
               {
                  try
                  {
                     mgpService.callValidator(event.getData());
                  }
                  catch (EdtServiceException e)
                  {
                     logger.error(e.getMessage(), e);
                  }
               }

               @Override
               public Class<EdtClientSocketEvent> getEventClass()
               {
                  return EdtClientSocketEvent.class;
               }

               @Override
               public String getSubscriptionFilter()
               {
                  return EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.INCOMING_DATA.name() + "'";
               }

               @Override
               public boolean justOnce()
               {
                  return false;
               }
            });
         }
         catch (EdtServiceException e)
         {
            logger.error("SocketService ERROR: " + e.getMessage(), e);
            e.printStackTrace();
         }
      }
   }

   private void login()
   {

      try
      {
         this.socketService.callConnect();

         Map<String, Object> map = new HashMap<String, Object>();
         map.put("ssk", new byte[] { 0x00 });
         map.put("AppId", new byte[] { 0x00 });
         map.put("ClientId", new byte[] { 0x64 });
         map.put("ModuloLen", new byte[] { 0x00 });
         map.put("PingTimeout", new byte[] { 0x00 });
         byte[] loginData = this.mgpService.callEncode(map, SpotMessageType.LOGIN.getCommand());
         System.out.println("Sending logging");
         this.socketService.invokeSendData(loginData);
         System.out.println("Logging Sent");
      }
      catch (Exception e)
      {
         logger.error("Loging in ERROR: " + e.getMessage(), e);
      }
   }
   // ******************************************************************
   // INNER CLASSES.
   // ******************************************************************

   // ******************************************************************
   // MAIN.
   // ******************************************************************

}
