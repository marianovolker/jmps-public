package com.jmps.android.pos.abdiarios;

import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.jmps.android.pos.R;

public class AbDiariosActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_diarios);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   public static class PlaceholderFragment extends Fragment
   {
      public PlaceholderFragment()
      {
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_diarios, container, false);
         return rootView;
      }
   }
   
   public void startAbDiariosIngresoActivity(View view)
   {
      this.startActivity(new Intent(this, AbDiariosIngresoActivity.class));
   }
   
   public void startAbDiariosEgresoActivity(View view)
   {
      this.startActivity(new Intent(this, AbDiariosEgresoActivity.class));
   }
   
   public void startAbDiariosEgresoQrActivity(View view)
   {
      this.startActivity(new Intent(this, AbDiariosEgresoQrActivity.class));
   }
   
   public void startAbDiariosTarifasActivity(View view)
   {
      this.startActivity(new Intent(this, AbDiariosTarifasActivity.class));
   }
   
   public void startAbDiariosCategoriasActivity(View view)
   {
      this.startActivity(new Intent(this, AbDiariosCategoriasActivity.class));
   }
   
   public void startAbDiariosPlayaActivity(View view)
   {
      this.startActivity(new Intent(this, AbDiariosPlayaActivity.class));
   }
   
   public void btnBack_OnClick(View view)
   {
      this.onBackPressed();
   }
}
