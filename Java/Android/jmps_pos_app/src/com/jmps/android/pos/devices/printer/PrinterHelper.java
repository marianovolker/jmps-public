package com.jmps.android.pos.devices.printer;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Locale;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.preference.PreferenceManager;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.VehiculoDia;

public final class PrinterHelper
{
   public static boolean isPrinterOk()
   {
      return m_printer_ok;
   }

   public static void setPrinterStatus(boolean m_printer_ok)
   {
      PrinterHelper.m_printer_ok = m_printer_ok;
   }
   
   public final static void testPrinter() throws IOException
   {
      String s = "\n";
      s += "\n--------------------------------";
      s += "Biemvenidos ...\n";
      s += "\n--------------------------------";
      
      PrinterDevice printer = new PrinterDevice();
      
      if( printer.openDevice() )
      {
         printer.write(s.getBytes());
         printer.closeDevice();
      }
      else
      {
         throw new IOException("The printer is not available ...");
      }
   }
   
   public final static void printTicketIn(Context context, VehiculoDia vehiculo, Bitmap bmp)
   {
      String s = getHeader(context);
      s += "\nPatente: " + vehiculo.getPatenteLetras() + " " + vehiculo.getPatenteNumeros();
      s += "\nIngreso: " + new SimpleDateFormat("dd-MM-yyyy HH:mm:ss", Locale.getDefault()).format(vehiculo.getFechaIngreso());
      addBlankLines(s);
      
      print(s.getBytes(), bmp);
   }
   
   public final static void printTicketOut(Context context, VehiculoDia vehiculo)
   {
      String s = getHeader(context);
      s += "\nPatente: " + vehiculo.getPatenteLetras() + " " + vehiculo.getPatenteNumeros();
      s += "\nIngreso: " + new SimpleDateFormat("dd-MM-yyyy HH:mm:ss", Locale.getDefault()).format(vehiculo.getFechaIngreso());
      s += "\nEgreso: " + new SimpleDateFormat("dd-MM-yyyy HH:mm:ss", Locale.getDefault()).format(vehiculo.getFechaEgreso());
      s += "\nImporte: $" + vehiculo.getImporte();
      addBlankLines(s);
      
      print(s.getBytes());
   }
   
   private final static void print(byte[] buffer)
   {
      PrinterDevice printer = new PrinterDevice();
      printer.openDevice();
      printer.write(buffer);
      printer.closeDevice();
   }
   
   private final static void print(byte[] buffer, Bitmap bmp)
   {
      PrinterDevice printer = new PrinterDevice();
      printer.openDevice();
      printer.write(buffer);
      printer.printQR(bmp);
      printer.closeDevice();
   }
   
   private final static String getHeader(Context context)
   {
      SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(context);
      String address = sharedPref.getString("ppsAddress", "");
      String phone = sharedPref.getString("ppsPhone", "");
      
      String s = "\n";
      s += "\n--------------------------------";
      s += context.getResources().getString(R.string.printerEncabezado);
      s += "\n" + address;
      s += "\n" + phone;
      s += "\n--------------------------------";
      
      return s;
   }
   
   private final static String addBlankLines(String string)
   {
      return string + "\n\n\n ";
   }
   
   
   private static boolean m_printer_ok = false; 
}
