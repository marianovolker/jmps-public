package com.jmps.android.pos;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.jmps.android.pos.abdiarios.AbDiariosActivity;
import com.jmps.android.pos.abmensuales.AbMensualesActivity;
import com.jmps.android.pos.config.SettingsActivity;

public class JMPSMainActivity extends Activity
{
   Context mContext;
  
   @Override
   public void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_main);

      mContext = this;
      
      AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(mContext);
      alertDialogBuilder.setTitle("Really?");
      alertDialogBuilder.setMessage("Are you sure?");
      //null should be your on click listener
      alertDialogBuilder.setPositiveButton("OK", null);
      alertDialogBuilder.setNegativeButton("Cancel", new DialogInterface.OnClickListener()
      {
          @Override
          public void onClick(DialogInterface dialog, int which)
          {
              dialog.dismiss();
          }
      });
   }
   
   
  
   public void startAbMensualesActivity(View view)
   {
      this.startActivity(new Intent(this, AbMensualesActivity.class));
   }
   
   public void startAbDiariosActivity(View view)
   {
      this.startActivity(new Intent(this, AbDiariosActivity.class));
   }
   
   public void startSettingsActivity(View view)
   {
      this.startActivity(new Intent(this, SettingsActivity.class));
   }
   
   public void endApp(View view)
   {
      android.os.Process.killProcess(android.os.Process.myPid());      
   }
}
