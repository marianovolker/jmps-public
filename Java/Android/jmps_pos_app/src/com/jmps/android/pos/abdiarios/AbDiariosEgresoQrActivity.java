package com.jmps.android.pos.abdiarios;

import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.devices.barcodescan.IntentIntegrator;
import com.jmps.android.pos.devices.barcodescan.IntentResult;
import com.jmps.android.pos.sqlite.VehiculoDia;

public class AbDiariosEgresoQrActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_diarios_egreso_qr);
      
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
         View rootView = inflater.inflate(R.layout.fragment_ab_diarios_egreso_qr, container, false);
         return rootView;
      }
   }
   
   
   public void btnAbDiariosEgresoQr_OnClick(View v)
   {
      IntentIntegrator ii = new IntentIntegrator(this);
      ii.initiateScan();
   }
   
   @Override
   public void onActivityResult(int requestCode, int resultCode, Intent data)
   {
      IntentResult ir = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
      if (ir != null)
      {
         String scanContent = ir.getContents();
         String patenteLetras, patenteNumeros;
         int indexOfSpace = scanContent.indexOf(" ");
         
         patenteLetras = scanContent.substring(0, indexOfSpace).trim();
         patenteNumeros = scanContent.substring(indexOfSpace).trim();
         
         VehiculoDia vehiculo = new VehiculoDia();
         vehiculo.setPatenteLetras(patenteLetras);
         vehiculo.setPatenteNumeros(patenteNumeros);
         Log.d("Fernando", patenteLetras + " " + patenteNumeros);
         AbDiariosEgresoHelper.egresar(this, vehiculo);
      }
      else
      {
         Toast.makeText(this, R.string.errAbDiariosEgresoQrSinQr, Toast.LENGTH_LONG).show();
      }
      
      super.onActivityResult(requestCode, resultCode, data);
   }
}
