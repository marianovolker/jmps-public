package com.jmps.android.pos.devices.printer;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.BitSet;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;

public class PrinterDevice
{
   private final String pathNameDevice = "/dev/ttyUSB0";
   private OutputStream osDevice = null;
   
   private BitSet dots;
   
   public PrinterDevice()
   {
      openDevice();
   }
   
   public boolean openDevice()
   {
      try
      {
         if( osDevice == null )
         {
            osDevice = new FileOutputStream(pathNameDevice);
         }
         
         return true;
      }
      catch (FileNotFoundException e)
      {
         e.printStackTrace();
      }
      
      return false;
   }
   
   public boolean closeDevice()
   {
      try
      {
         osDevice.close();
         
         return true;
      }
      catch (IOException e)
      {
         e.printStackTrace();
      }
      
      return false;
   }
   
   public boolean write(byte buf[])
   {
      try
      {
         osDevice.write(buf);
         osDevice.write(PrinterCommands.FEED_LINE);
         return true;
      }
      catch (IOException e)
      {
         e.printStackTrace();
      }
      
      return false;
   }
   
   public boolean convertBitmap(Bitmap inputBitmap)
   {
      
      int mWidth = inputBitmap.getWidth();
      int mHeight = inputBitmap.getHeight();
      
      convertArgbToGrayscale(inputBitmap, mWidth, mHeight);
      
      return true;
      
   }
   
   private void convertArgbToGrayscale(Bitmap bmpOriginal, int width, int height)
   {
      int pixel;
      int k = 0;
      int B = 0, G = 0, R = 0;
      dots = new BitSet();
      try
      {
         
         for (int x = 0; x < height; x++)
         {
            for (int y = 0; y < width; y++)
            {
               // get one pixel color
               pixel = bmpOriginal.getPixel(y, x);
               
               // retrieve color of all channels
               R = Color.red(pixel);
               G = Color.green(pixel);
               B = Color.blue(pixel);
               // take conversion up to one single value by calculating
               // pixel intensity.
               R = G = B = (int) (0.299 * R + 0.587 * G + 0.114 * B);
               // set bit into bitset, by calculating the pixel's luma
               if (R < 55)
               {
                  dots.set(k);// this is the bitset that i'm printing
               }
               k++;
            }
         }
      }
      catch (Exception e)
      {
         // TODO: handle exception
         // Log.e(TAG, e.toString());
      }
   }
   
   private void print_image(String file)
   {
      try
      {
         File fl = new File(file);
         if (fl.exists())
         {
            Bitmap bmp = BitmapFactory.decodeFile(file);
            convertBitmap(bmp);
            osDevice.write(PrinterCommands.SET_LINE_SPACING_24);
            
            int offset = 0;
            while (offset < bmp.getHeight())
            {
               osDevice.write(PrinterCommands.SELECT_BIT_IMAGE_MODE_2);
               for (int x = 0; x < bmp.getWidth(); ++x)
               {
                  for (int k = 0; k < 3; ++k)
                  {
                     byte slice = 0;
                     for (int b = 0; b < 8; ++b)
                     {
                        int y = (((offset / 8) + k) * 8) + b;
                        int i = (y * bmp.getWidth()) + x;
                        boolean v = false;
                        if (i < dots.length())
                        {
                           v = dots.get(i);
                        }
                        slice |= (byte) ((v ? 1 : 0) << (7 - b));
                     }
                     osDevice.write(slice);
                  }
               }
               offset += 24;
               osDevice.write(PrinterCommands.FEED_LINE);
               osDevice.write(PrinterCommands.FEED_LINE);
               osDevice.write(PrinterCommands.FEED_LINE);
               osDevice.write(PrinterCommands.FEED_LINE);
               osDevice.write(PrinterCommands.FEED_LINE);
               osDevice.write(PrinterCommands.FEED_LINE);
            }
            osDevice.write(PrinterCommands.SET_LINE_SPACING_30);
         }
         else
         {
            // Toast.makeText(this.getApplicationContext(), "file doesn't exists", Toast.LENGTH_SHORT).show();
         }
      }
      catch (IOException e)
      {
         // TODO AutoMes-generated catch block
         e.printStackTrace();
      }
   }
   
   public void printQR(Bitmap bmp)
   {
      try
      {
         // convertBitmap(bmp);
         osDevice.write(PrinterCommands.SET_LINE_SPACING_0);
         
         int offset = 0;
         while (offset < bmp.getHeight())
         {
            
            byte bBmpData[];
            bBmpData = ReadBmpData(bmp, offset);
            if (bBmpData == null)
            {
               break;
            }
            else
            {
               osDevice.write(bBmpData);
               offset += 8;
               //               try
               //               {
               //                  Thread.sleep(20);
               //               }
               //               catch (InterruptedException e)
               //               {
               //               }
            }
         }
         osDevice.write(PrinterCommands.SET_LINE_SPACING_30);
      }
      catch (IOException ex)
      {
         ex.printStackTrace();
      }
   }
   
   private byte[] ReadBmpData(Bitmap b, int startline)
   {
      int nWidth;
      int nHeight;
      int nRealLines;
      int nSize;
      byte[] bData;
      nWidth = b.getWidth();
      nHeight = b.getHeight();
      
      if (startline >= nHeight)
      {
         return null;
      }
      
      nRealLines = nHeight - startline;
      if (nRealLines > 8)
      {
         nRealLines = 8;
      }
      
      nSize = nWidth + 6;
      bData = new byte[nSize];
      for (int i = 0; i < nSize; i++)
      {
         bData[i] = 0x00;
      }
      
      bData[0] = 0x1B;
      bData[1] = 0x2A;
      bData[2] = 0x01;
      bData[3] = (byte) (nWidth % 256);
      bData[4] = (byte) (nWidth / 256);
      
      int nTempValue;
      int nTempData;
      int nPixColor;
      int nDataIndex;
      int nBitIndex;
      
      nDataIndex = 5;
      for (int w = 0; w < nWidth; w++)
      {
         nBitIndex = 0;
         for (int h = startline; h < startline + nRealLines; h++)
         {
            nPixColor = b.getPixel(w, h);
            nPixColor = nPixColor & 0x00111111;
            nBitIndex++;
            
            if (nPixColor == 0)
            {
               nTempValue = 1;
               nTempValue = nTempValue << (8 - nBitIndex);
               nTempData = bData[nDataIndex];
               nTempData = nTempData | nTempValue;
               bData[nDataIndex] = (byte) nTempData;
            }
         }
         nDataIndex++;
      }
      
      bData[nDataIndex] = 0x0a;
      return bData;
   }
}
