package com.jmps.android.pos.util;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.jmps.android.pos.R;

public class GenericDialogActivity extends Activity
{
   @Override
   public void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      //setContentView(R.layout.activity_dialog_check_printer);
      
      LinearLayout dialogContainer = (LinearLayout) findViewById(R.id.container);
      
      Button button = new Button(this);
      
      button.setText("Press Me");
      
      dialogContainer.addView(button);
      
 
      
      
      // 1. get passed intent
      Intent intent = getIntent();

      // 2. get message value from intent
      String message = intent.getStringExtra("message");

      // 3. show message on textView
      //((TextView)findViewById(R.id.tvMessage)).setText(message);

      // 4. get bundle from intent
      Bundle bundle = intent.getExtras();

      // 5. get status value from bundle
      String status = bundle.getString("status");

      // 6. show status on Toast
      Toast toast = Toast.makeText(this, status, Toast.LENGTH_LONG);
      toast.show();
      
   }
   
}
