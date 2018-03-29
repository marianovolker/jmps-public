package com.jmps.android.pos.abdiarios;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.CategoriaDia;
import com.jmps.android.pos.sqlite.DatabaseHelper;

public class AbDiariosCategoriasAltaActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_diarios_categorias_alta);
      
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
         View rootView = inflater.inflate(R.layout.fragment_ab_diarios_categorias_alta, container, false);
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         // Set buttons' OnClickListeners.
         view.findViewById(R.id.btnAbDiariosCategoriasAltaGuardar).setOnClickListener(btnGuardar);
         view.findViewById(R.id.btnAbDiariosCategoriasAltaCancelar).setOnClickListener(btnCancelar);
         
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
            CategoriaDia categoria = new CategoriaDia();
            
            String nombre = ((EditText) v.findViewById(R.id.txtAbDiariosCategoriasAltaNombre)).getText().toString();
            if (nombre.trim().isEmpty())
            {
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbDiariosCategoriasAltaSinNombre, Toast.LENGTH_LONG).show();
               return;
            }
            else
            {
               categoria.setNombre(nombre);
            }
            
            DatabaseHelper helper = new DatabaseHelper(PlaceholderFragment.this.getActivity());
            helper.insert(categoria);
            helper.close();
            
            Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.toastAbDiariosCategoriasAlta, Toast.LENGTH_SHORT).show();
         }
      };
   }
}
