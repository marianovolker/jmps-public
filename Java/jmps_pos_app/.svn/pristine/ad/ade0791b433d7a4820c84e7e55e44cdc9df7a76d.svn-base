package com.jmps.android.pos.abdiarios;

import android.app.Activity;
import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.VehiculoDia;

public class AbDiariosEgresoActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_diarios_egreso);
      
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
         View rootView = inflater.inflate(R.layout.fragment_ab_diarios_egreso, container, false);
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         try
         {
            int id = this.getActivity().getIntent().getExtras().getInt("vehiculoDiaId");
            
            // If it's a valid Id
            if (id >= 1)
            {
               // Gets VehiculoDia and loads EditTexts
               VehiculoDia vehiculo = new VehiculoDia();
               vehiculo.setId(id);
               
               DatabaseHelper helper = new DatabaseHelper(this.getActivity());
               vehiculo = (VehiculoDia) helper.select(vehiculo);
               
               ((EditText) view.findViewById(R.id.txtAbDiariosEgresoPatenteLetras)).setText(vehiculo.getPatenteLetras());
               ((EditText) view.findViewById(R.id.txtAbDiariosEgresoPatenteNumeros)).setText(vehiculo.getPatenteNumeros());
            }
         }
         catch (Exception ex)
         {
         }
         
         // Sets buttons' OnClickListeners.
         view.findViewById(R.id.btnAbDiariosEgresoCancelar).setOnClickListener(btnCancelar);
         view.findViewById(R.id.btnAbDiariosEgresoGuardar).setOnClickListener(btnGuardar);
         
         super.onViewCreated(view, savedInstanceState);
      }
      
      OnClickListener btnCancelar = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            PlaceholderFragment.this.getActivity().onBackPressed();
         }
      };
      
      OnClickListener btnGuardar = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            v = PlaceholderFragment.this.getView();
            Context context = PlaceholderFragment.this.getActivity();
            VehiculoDia vehiculo = new VehiculoDia();
            
            EditText text = (EditText) v.findViewById(R.id.txtAbDiariosEgresoPatenteLetras);
            String s = text.getText().toString().trim();
            if (s.length() != 3)
            {
               Toast.makeText(context, R.string.errAbDiariosEgresoLetrasIncorrectas, Toast.LENGTH_LONG).show();
            }
            else
            {
               vehiculo.setPatenteLetras(s);
            }
            
            text = (EditText) v.findViewById(R.id.txtAbDiariosEgresoPatenteNumeros);
            s = text.getText().toString().trim();
            if (s.length() != 3)
            {
               Toast.makeText(context, R.string.errAbDiariosEgresoNumerosIncorrectos, Toast.LENGTH_LONG).show();
            }
            else
            {
               vehiculo.setPatenteNumeros(s);
            }
            
            AbDiariosEgresoHelper.egresar(context, vehiculo);
            
            Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.toastAbDiariosEgreso, Toast.LENGTH_SHORT).show();
            ((Activity) context).onBackPressed();
         }
      };
   }
}
