package com.jmps.android.pos.abmensuales;

import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.jmps.android.pos.R;

public class AbMensualesTarifasActivity extends Activity
{
   
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_mensuales_tarifas);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   /**
    * A placeholder fragment containing a simple view.
    */
   public static class PlaceholderFragment extends Fragment
   {
      
      public PlaceholderFragment()
      {
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_mensuales_tarifas, container, false);
         return rootView;
      }
   }
   
   public void startAbMensualesTarifasAgregarActivity(View view)
   {
      this.startActivity(new Intent(this, AbMensualesTarifasAgregarActivity.class));
   }
   
   public void startAbMensualesTarifasEliminarActivity(View view)
   {
      this.startActivity(new Intent(this, AbMensualesTarifasEliminarActivity.class));
   }
   
   public void startAbMensualesTarifasModificarActivity(View view)
   {
      this.startActivity(new Intent(this, AbMensualesTarifasModificarActivity.class));
   }
   
   public void startAbMensualesTarifasListarActivity(View view)
   {
      this.startActivity(new Intent(this, AbMensualesTarifasListarActivity.class));
   }
   
   public void btnBack_OnClick(View view)
   {
      this.onBackPressed();
   }
}
