package com.jmps.android.pos;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class CajaActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_caja);
      
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
         View rootView = inflater.inflate(R.layout.fragment_caja, container, false);
         return rootView;
      }
   }
   
}
