package com.jmps.android.pos.devices.printer;

import java.io.IOException;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.jmps.android.pos.JMPSMainActivity;
import com.jmps.android.pos.R;

public class DialogCheckPrinter extends Activity
{
   @Override
   public void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_dialog_check_printer);
      
      try
      {
         PrinterHelper.testPrinter();
         
         Intent i = new Intent(DialogCheckPrinter.this, JMPSMainActivity.class);
         i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
         startActivity(i);
         
         // close this activity
         finish();
      }
      catch (IOException e1)
      {
         PrinterHelper.setPrinterStatus(false);
      }      
   }
   
   public void checkPrinterAgain(View view)
   {
      try
      {
         PrinterHelper.testPrinter();
         
         Intent i = new Intent(DialogCheckPrinter.this, JMPSMainActivity.class);
         i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
         startActivity(i);         
         
         // close this activity
         finish();
      }
      catch (IOException e1)
      {
         PrinterHelper.setPrinterStatus(false);
      }      
   }   
   
   public void endApp(View view)
   {
      android.os.Process.killProcess(android.os.Process.myPid());      
   }   
   
}
