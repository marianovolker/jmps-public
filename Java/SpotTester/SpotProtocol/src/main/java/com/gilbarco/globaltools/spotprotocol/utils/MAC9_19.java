package com.gilbarco.globaltools.spotprotocol.utils;

import java.io.File;
import java.io.InputStream;
import java.util.Arrays;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtServiceDefinition;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.EdtServiceManager;
import com.gilbarco.globaltools.edt.services.crypto.EdtCryptoAdapter;
import com.gilbarco.globaltools.edt.util.EdtByteUtil;
import com.gilbarco.globaltools.edt.util.EdtByteUtil.Op;

public class MAC9_19
{
   // ******************************************************************
   // STATIC FIELDS
   // ******************************************************************
   private static final int PIN_AUTH = 1;
   private static final int KEY_LEN  = 8;
   private static final int MACLEN   = 8; 
   private static final int DATALEN  = 8; 
   private static final int ANSI_MAC_LEN= 4;
   private static final String FILE = "keys.txt";
   
	private static final transient Log	logger				= LogFactory.getLog( MAC9_19.class );

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
   private byte[] rightKey;
   private byte[] leftKey;
   private EdtCryptoAdapter cryptoAdapter;
               
   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************
   public MAC9_19(EdtServiceDefinition serviceDefintion)
   {
      super();
      try
      {
         byte[] keys = new byte[2*KEY_LEN];
//         FileInputStream bis = new FileInputStream(new File(FILE));
         InputStream bis = this.getClass().getClassLoader().getResourceAsStream(FILE); 
         bis.read(keys);
         this.rightKey = Arrays.copyOf(keys, KEY_LEN);
         this.leftKey = Arrays.copyOfRange(keys, KEY_LEN, 2*KEY_LEN);
         bis.close();
         
         EdtServiceManager manager = EdtServiceContext.getServiceManager();
         cryptoAdapter = (EdtCryptoAdapter) manager.getServiceAdapter(serviceDefintion);
         
      }
      catch (Exception e)
      {
			logger.error( e.getMessage( ) );
      }
   }
   // ******************************************************************
   // PUBLIC METHODS (general, getter, setter, interface imp)
   // ******************************************************************
   public byte[] getValue(byte[] abData, int authent)
   {
      
      byte[] sha1;
      try
      {
         sha1 = EdtByteUtil.concatBytes(cryptoAdapter.callSHA1((abData)), authent == PIN_AUTH ? "PIN".getBytes() : "CLEAR".getBytes());
         return Arrays.copyOf(getMac9_19Value(sha1),ANSI_MAC_LEN);
      }
      catch (EdtServiceException e)
      {
			logger.error( e.getMessage( ) );
      }
      return null;
   }

   public byte[] getValue(File file, int authent)
   {
      
      byte[] sha1;
      try
      {
         sha1 = EdtByteUtil.concatBytes(cryptoAdapter.callSHA1((file)), authent == PIN_AUTH ? "PIN".getBytes() : "CLEAR".getBytes());
         return Arrays.copyOf(getMac9_19Value(sha1),ANSI_MAC_LEN);
      }
      catch (EdtServiceException e)
      {
			logger.error( e.getMessage( ) );
      }
      return null;
   }

   public byte[] getMac9_19Value(byte[] data) throws EdtServiceException
   {
      byte[] mac = new byte[MACLEN];
      byte[] temp = new byte[DATALEN];
      int offset = 0;

      while (offset <= data.length)
      {
         temp = Arrays.copyOfRange(data, offset, offset + DATALEN);
         EdtByteUtil.byteOp(Op.XOR, temp, 0, DATALEN, mac, 0, DATALEN);
         mac = cryptoAdapter.callDesEncrypt(temp, this.leftKey);
         offset+=DATALEN;
      }

      cryptoAdapter.callDesDecrypt(temp, this.rightKey);
      cryptoAdapter.callDesEncrypt(temp, this.leftKey);

      return mac;
   }

   

   // ******************************************************************
   // PROTECTED METHODS.
   // ******************************************************************

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
