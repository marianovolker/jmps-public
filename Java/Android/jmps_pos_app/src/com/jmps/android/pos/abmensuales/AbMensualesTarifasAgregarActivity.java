package com.jmps.android.pos.abmensuales;

import android.app.Activity;
import android.os.Bundle;

import com.jmps.android.pos.R;

public class AbMensualesTarifasAgregarActivity extends Activity
{
   
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_mensuales_tarifas_agregar);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new AbMensualesTarifaEdicionFragment()).commit();
      }
   }
}
