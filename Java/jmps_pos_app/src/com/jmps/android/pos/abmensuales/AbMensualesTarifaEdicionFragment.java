package com.jmps.android.pos.abmensuales;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.TarifaMes;


public class AbMensualesTarifaEdicionFragment extends Fragment
{
   boolean isNew;
   TarifaMes tarifaMes;
   
   @Override
   public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
   {
      View rootView = inflater.inflate(R.layout.fragment_ab_mensuales_tarifas_agregar, container, false);
      
      return rootView;
   }
   
   @Override
   public void onViewCreated(View view, Bundle savedInstanceState)
   {
      // Gets arguments.
      Bundle args = this.getArguments();
      
      if (args == null)
      {
         isNew = true;
         tarifaMes = null;
      }
      else
      {
         isNew = false;
         tarifaMes = new TarifaMes();
         tarifaMes.setId(args.getInt("id"));
         
         // Gets TarifaMes and loads its contents.
         DatabaseHelper helper = new DatabaseHelper(this.getActivity());
         tarifaMes = (TarifaMes) helper.select(tarifaMes);
         helper.close();
         
         // Fills UI.
         EditText edit = (EditText) view.findViewById(R.id.txtAbMensualesTarifasAgregarNombre);
         edit.setText(tarifaMes.getNombre());
         
         edit = (EditText) view.findViewById(R.id.txtAbMensualesTarifasAgregarDescripcion);
         edit.setText(tarifaMes.getDescripcion());
         
         edit = (EditText) view.findViewById(R.id.txtAbMensualesTarifasAgregarExcepcion);
         edit.setText(tarifaMes.getExcepcion());
         
         edit = (EditText) view.findViewById(R.id.txtAbMensualesTarifasAgregarPeriodo);
         edit.setText(tarifaMes.getPeriodo());
         
         edit = (EditText) view.findViewById(R.id.txtAbMensualesTarifasAgregarCosto);
         edit.setText(String.valueOf(tarifaMes.getPrecio()));
      }
      
      // Sets onClick listeners.
      view.findViewById(R.id.btnAbMensualesTarifasAgregarAceptar).setOnClickListener(btnAceptar_OnClick);
      view.findViewById(R.id.btnAbMensualesTarifasAgregarCancelar).setOnClickListener(btnCancelar_OnClick);
      
      super.onViewCreated(view, savedInstanceState);
   }
   
   View.OnClickListener btnAceptar_OnClick = new OnClickListener()
   {
      @Override
      public void onClick(View v)
      {
         v = AbMensualesTarifaEdicionFragment.this.getView();
         
         if (isNew)
            tarifaMes = new TarifaMes();
         
         // Loads data into object.
         EditText edit = (EditText) v.findViewById(R.id.txtAbMensualesTarifasAgregarNombre);
         tarifaMes.setNombre(edit.getText().toString());
         
         edit = (EditText) v.findViewById(R.id.txtAbMensualesTarifasAgregarDescripcion);
         tarifaMes.setDescripcion(edit.getText().toString());
         
         edit = (EditText) v.findViewById(R.id.txtAbMensualesTarifasAgregarExcepcion);
         tarifaMes.setExcepcion(edit.getText().toString());
         
         edit = (EditText) v.findViewById(R.id.txtAbMensualesTarifasAgregarPeriodo);
         tarifaMes.setPeriodo(edit.getText().toString());
         
         edit = (EditText) v.findViewById(R.id.txtAbMensualesTarifasAgregarCosto);
         tarifaMes.setPrecio(Double.parseDouble(edit.getText().toString()));
         
         if (isNew)
         {
            // Inserts tarifaMes into database.
            DatabaseHelper helper = new DatabaseHelper(AbMensualesTarifaEdicionFragment.this.getActivity());
            helper.insert(tarifaMes);
            helper.close();
            
            Toast toast = Toast.makeText(AbMensualesTarifaEdicionFragment.this.getActivity(), R.string.toastAbMensualesTarifaAlta, Toast.LENGTH_SHORT);
            toast.show();
         }
         else
         {
            // Updates database.
            DatabaseHelper helper = new DatabaseHelper(AbMensualesTarifaEdicionFragment.this.getActivity());
            helper.update(tarifaMes);
            helper.close();
            
            Toast toast = Toast.makeText(AbMensualesTarifaEdicionFragment.this.getActivity(), R.string.toastAbMensualesTarifaModificacion, Toast.LENGTH_SHORT);
            toast.show();
         }
         
         AbMensualesTarifaEdicionFragment.this.getActivity().onBackPressed();
      }
   };
   
   View.OnClickListener btnCancelar_OnClick = new OnClickListener()
   {
      @Override
      public void onClick(View v)
      {
         AbMensualesTarifaEdicionFragment.this.getActivity().onBackPressed();
      }
   };
}