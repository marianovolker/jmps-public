package com.jmps.android.pos.abdiarios;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.CategoriaDia;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.TarifaDia;


public class AbDiariosTarifaEdicionFragment extends Fragment
{
   boolean isNew;
   TarifaDia tarifaDia;
   List<CategoriaDia> categoriasDia;
   
   @Override
   public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
   {
      View rootView = inflater.inflate(R.layout.fragment_ab_diarios_tarifas_agregar, container, false);
      
      categoriasDia = null;
      
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
         tarifaDia = null;
      }
      else
      {
         isNew = false;
         tarifaDia = new TarifaDia();
         tarifaDia.setId(args.getInt("id"));
      }
      
      // Fills CategoriaDia spinner.
      DatabaseHelper helper = new DatabaseHelper(this.getActivity());
      
      CategoriaDia[] categoriasDiaArray = (CategoriaDia[]) helper.selectAll(new CategoriaDia());
      if (categoriasDiaArray == null)
      {
         helper.close();
         Toast.makeText(this.getActivity(), R.string.errAbDiariosTarifasSinCategorias, Toast.LENGTH_LONG).show();
         // TODO this.getActivity().onBackPressed();
      }
      else
      {
         categoriasDia = Arrays.asList(categoriasDiaArray);
         
         List<String> items = new ArrayList<String>();
         for (CategoriaDia item : categoriasDia)
         {
            items.add(item.getNombre());
         }
         
         ArrayAdapter<String> adapter = new ArrayAdapter<String>(this.getActivity(), android.R.layout.simple_spinner_item, items);
         
         Spinner spinner = (Spinner) view.findViewById(R.id.spiAbDiariosTarifasAgregarCategoria);
         spinner.setAdapter(adapter);
      }
      
      
      if (!isNew)
      {
         // Gets TarifaMes and loads its contents.
         tarifaDia = (TarifaDia) helper.select(tarifaDia);
         
         // Fills UI.
         EditText edit = (EditText) view.findViewById(R.id.txtAbDiariosTarifasAgregarNombre);
         edit.setText(tarifaDia.getNombre());
         
         edit = (EditText) view.findViewById(R.id.txtAbDiariosTarifasAgregarDescripcion);
         edit.setText(tarifaDia.getDescripcion());
         
         edit = (EditText) view.findViewById(R.id.txtAbDiariosTarifasAgregarTiempo);
         edit.setText(tarifaDia.getTiempo());
         
         edit = (EditText) view.findViewById(R.id.txtAbDiariosTarifasAgregarTolerancia);
         edit.setText(tarifaDia.getTolerancia());
         
         edit = (EditText) view.findViewById(R.id.txtAbDiariosTarifasAgregarPrecio);
         edit.setText(String.valueOf(tarifaDia.getPrecio()));
         
         // Selects CategoriaDia on spinner.
         int index = -1;
         for (int i = 0; i < categoriasDia.size(); i++)
         {
            if (categoriasDia.get(i).getId() == tarifaDia.getCategoriaId())
            {
               index = i;
               break;
            }
         }
         
         if (index == -1)
         {
            helper.close();
            Toast.makeText(this.getActivity(), R.string.errAbDiariosTarifasAltaCategoriaNotFound, Toast.LENGTH_LONG).show();
            // TODO this.getActivity().onBackPressed();
         }
         else
         {
            Spinner spinner = (Spinner) view.findViewById(R.id.spiAbDiariosTarifasAgregarCategoria);
            spinner.setSelection(index);
         }
      }
      helper.close();
      
      // Sets onClick listeners.
      view.findViewById(R.id.btnAbDiariosTarifasAgregarAceptar).setOnClickListener(btnAceptar_OnClick);
      view.findViewById(R.id.btnAbDiariosTarifasAgregarCancelar).setOnClickListener(btnCancelar_OnClick);
      
      super.onViewCreated(view, savedInstanceState);
   }
   
   View.OnClickListener btnAceptar_OnClick = new OnClickListener()
   {
      @Override
      public void onClick(View v)
      {
         Context context = AbDiariosTarifaEdicionFragment.this.getActivity();
         v = AbDiariosTarifaEdicionFragment.this.getView();
         
         if (isNew)
            tarifaDia = new TarifaDia();
         
         // Loads data into object.
         EditText edit = (EditText) v.findViewById(R.id.txtAbDiariosTarifasAgregarNombre);
         String content = edit.getText().toString();
         if (content.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbDiariosTarifasAgregarNombre, Toast.LENGTH_LONG).show();
            return;
         }
         else
            tarifaDia.setNombre(content);
         
         // "Descripci�n" can be null, so it's not checked.
         edit = (EditText) v.findViewById(R.id.txtAbDiariosTarifasAgregarDescripcion);
         content = edit.getText().toString();
         tarifaDia.setDescripcion(content);
         
         edit = (EditText) v.findViewById(R.id.txtAbDiariosTarifasAgregarTiempo);
         content = edit.getText().toString();
         if (content.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbDiariosTarifasAgregarTiempo, Toast.LENGTH_LONG).show();
            return;
         }
         else
            tarifaDia.setTiempo(Integer.parseInt(content));
         
         edit = (EditText) v.findViewById(R.id.txtAbDiariosTarifasAgregarTolerancia);
         content = edit.getText().toString();
         if (content.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbDiariosTarifasAgregarTolerancia, Toast.LENGTH_LONG).show();
            return;
         }
         else
            tarifaDia.setTolerancia(Integer.parseInt(content));
         
         edit = (EditText) v.findViewById(R.id.txtAbDiariosTarifasAgregarPrecio);
         content = edit.getText().toString();
         if (content.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbDiariosTarifasAgregarPrecio, Toast.LENGTH_LONG).show();
            return;
         }
         else
            tarifaDia.setPrecio(Double.parseDouble(content));
         
         // Gets CategoriaDia id based on selected item on spinner.
         Spinner spinner = (Spinner) v.findViewById(R.id.spiAbDiariosTarifasAgregarCategoria);
         int selectedTarifa = spinner.getSelectedItemPosition();
         tarifaDia.setCategoriaId(categoriasDia.get(selectedTarifa).getId());
         
         DatabaseHelper helper = new DatabaseHelper(AbDiariosTarifaEdicionFragment.this.getActivity());
         if (isNew)
         {
            // Inserts tarifaMes into database.
            helper.insert(tarifaDia);
            
            Toast toast = Toast.makeText(AbDiariosTarifaEdicionFragment.this.getActivity(), R.string.toastAbDiariosTarifaAlta, Toast.LENGTH_SHORT);
            toast.show();
         }
         else
         {
            // Updates database.
            helper.update(tarifaDia);
            
            Toast toast = Toast.makeText(AbDiariosTarifaEdicionFragment.this.getActivity(), R.string.toastAbDiariosTarifaModificacion, Toast.LENGTH_SHORT);
            toast.show();
         }
         helper.close();
         
         AbDiariosTarifaEdicionFragment.this.getActivity().onBackPressed();
      }
   };
   
   View.OnClickListener btnCancelar_OnClick = new OnClickListener()
   {
      @Override
      public void onClick(View v)
      {
         AbDiariosTarifaEdicionFragment.this.getActivity().onBackPressed();
      }
   };
}