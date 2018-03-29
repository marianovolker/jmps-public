package com.gilbarco.globaltools.gcm.engines;

import static com.gilbarco.globaltools.gcm.model.GCMContext.SERVICE_MANAGER;

import java.io.File;
import java.io.FileInputStream;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

public class FileDownloadEngine
{
   // ******************************************************************
   // STATIC FIELDS
   // ******************************************************************
   private static final transient Log logger       = LogFactory.getLog(FileDownloadEngine.class);
   private static final byte          FILE_TYPE    = 0x45; //spec 45
   private static final byte          FILE_ID      = 0x01;
   private static final byte          AUTHENT_TYPE = 0x02;

   // ******************************************************************
   // STATIC METHODS
   // ******************************************************************

   // ******************************************************************
   // PUBLIC FIELDS.
   // ******************************************************************

   // ******************************************************************
   // PROTECTED FIELDS.
   // ******************************************************************
   protected File                     currentFile;
   private DataTransportLayer         dataSender;

   // ******************************************************************
   // PRIVATE FIELDS.
   // ******************************************************************

   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************
   public FileDownloadEngine(DataTransportLayer dataTransportLayer) throws EdtServiceException
   {
      super();
      this.dataSender = dataTransportLayer;
      this.subscribeToEvents();
   }

   // ******************************************************************
   // PUBLIC METHODS (general, getter, setter, interface imp)
   // ******************************************************************

   public void setCurrentFile(File currentFile)
   {
      this.currentFile = currentFile;
   }

   public File getCurrentFile()
   {
      return currentFile;
   }

   public void startFileDownload()
   {
      if (this.currentFile == null)
      {
         logger.error("EMVTable download can't start because no file has been set");
         throw new Error("EMVTable download can't start because no file has been set");
      }
      this.sendDeleteEMVTable();
   }

   // ******************************************************************
   // PROTECTED METHODS.
   // ******************************************************************
   protected void sendDeleteEMVTable()
   {
      this.sendData(SpotMessages.getInstance().getFileDeleteData(FILE_TYPE, FILE_ID));
   }

   protected void sendEMVTableFile()
   {
      this.sendData(SpotMessages.getInstance().getFileDownloadRequestData(currentFile, FILE_TYPE, FILE_ID, AUTHENT_TYPE, "clessemvtabdef.i"));
   }

   protected boolean sendNextBlock(String offset)
   {
      try
      {
         byte[] data = new byte[2048];
         int offSet = EdtConvert.stringToInt(offset);
         if (offSet <= currentFile.length())
         {
            FileInputStream fis = new FileInputStream(currentFile);
            fis.skip(offSet);
            int length = fis.read(data, 0, data.length);
            if (length > 0)
            {
               sendData(offSet, length, data);
               return true;
            }
         }
      }
      catch (Exception e)
      {
         logger.error(e.getMessage(), e);
      }
      return false;
   }

   protected void sendData(int offSet, int length, byte[] data)
   {
//      this.sendData(Arrays.copyOfRange(data, offSet, offSet + length));
      this.sendData(SpotMessages.getInstance().getFileBlockDownloadData(offSet, length, data));
   }

   protected void sendData(byte[] data)
   {
      System.out.println("data : " + EdtConvert.bytesToHexString(data));
      this.dataSender.send(data);
   }

   // ******************************************************************
   // PRIVATE METHODS.
   // ******************************************************************
   protected void subscribeToEvents()
   {
      try
      {
         SERVICE_MANAGER.subscribe(new EdtEventAction<MgpResultEvent>()
         {

            @Override
            public void onInvokeEvent(MgpResultEvent event)
            {
             //TODO:Notify that the table is loaded ok or not.
               if ((event.getMessageName().equals(SpotMessageType.FILE_BLOCK_DOWNLOAD.getName()) || event.getMessageName().equals(
                  SpotMessageType.FILE_DOWNLOAD_REQUEST.getName())))
               {
                  if (event.isValid())
                  {
                     @SuppressWarnings("unchecked")
                     Map<String, Object> map = (Map<String, Object>) event.getDecodeMap().get(event.getDecodeMap().keySet().iterator().next());
                     sendNextBlock((String) map.get("Offset"));
                  }
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
               return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name() + "'";
            }

            @Override
            public boolean justOnce()
            {
               return false;
            }
         });

         SERVICE_MANAGER.subscribe(new EdtEventAction<MgpResultEvent>()
         {

            @Override
            public void onInvokeEvent(MgpResultEvent event)
            {
               if ((event.getMessageName().equals(SpotMessageType.FILE_DELETE.getName())))
               {
                  sendEMVTableFile();
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
               return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name() + "'";
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
         logger.error(e.getMessage(), e);
      }
   }
   // ******************************************************************
   // INNER CLASSES.
   // ******************************************************************

   // ******************************************************************
   // MAIN.
   // ******************************************************************

}
