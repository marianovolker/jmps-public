package com.jmps.android.pos.abdiarios;

import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.jmps.android.pos.R;

public class AbDiariosCategoriasActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_diarios_categorias);
      
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
         View rootView = inflater.inflate(R.layout.fragment_ab_diarios_categorias, container, false);
         return rootView;
      }
   }
   
   public void startAbDiariosCategoriasAltaActivity(View view)
   {
      this.startActivity(new Intent(this, AbDiariosCategoriasAltaActivity.class));
   }
   
   public void startAbDiariosCategoriasEliminarActivity(View view)
   {
      this.startActivity(new Intent(this, AbDiariosCategoriasEliminarActivity.class));
   }
   
   public void btnBack_OnClick(View view)
   {
      this.onBackPressed();
   }
}
