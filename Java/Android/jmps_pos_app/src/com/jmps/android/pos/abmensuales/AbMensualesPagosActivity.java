package com.jmps.android.pos.abmensuales;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Locale;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.AbonadoMes;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.PagoMes;
import com.jmps.android.pos.sqlite.TarifaMes;

public class AbMensualesPagosActivity extends Activity
{
   
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_mensuales_pagos);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   
   public static class PlaceholderFragment extends Fragment
   {
      AbonadoMes[] abonadosMes;
      int range; // Per�odo range.
      
      public PlaceholderFragment()
      {
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_mensuales_pagos, container, false);
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         // Sets listeners for UI elements.
         view.findViewById(R.id.btnAbMensualesPagosAceptar).setOnClickListener(btnAbMensualesPagosAceptar_OnClick);
         view.findViewById(R.id.btnAbMensualesPagosCancelar).setOnClickListener(btnAbMensualesPagosCancelar_OnClick);
         ((Spinner) view.findViewById(R.id.spiAbMensualesPagosAbonado)).setOnItemSelectedListener(spiAbMensualesPagosAbonado_OnItemSelected);
         
         
         // Fills spiAbMensualesPagosAbonado with current month, "range" months before and "range" months after.
         range = 3;
         Calendar calendar = Calendar.getInstance();
         calendar.add(Calendar.MONTH, -range);
         
         ArrayList<String> items = new ArrayList<String>();
         String month, year;
         for (int i = -range; i <= range; i++)
         {
            month = calendar.getDisplayName(Calendar.MONTH, Calendar.LONG, Locale.getDefault());
            year = String.valueOf(calendar.get(Calendar.YEAR));
            items.add(month + " " + year);
            calendar.add(Calendar.MONTH, 1);
         }
         
         ArrayAdapter<String> adapter = new ArrayAdapter<String>(PlaceholderFragment.this.getActivity(),
                                                                 android.R.layout.simple_spinner_item,
                                                                 items);
         
         Spinner spinner = (Spinner) PlaceholderFragment.this.getView().findViewById(R.id.spiAbMensualesPagosPeriodo);
         spinner.setAdapter(adapter);
         spinner.setSelection(range + 1);
         
         
         // Fills AbonadosMes spinner. If there is no AbonadoMes available, returns to previous screen.
         DatabaseHelper helper = new DatabaseHelper(this.getActivity());
         abonadosMes = (AbonadoMes[]) helper.selectAll(new AbonadoMes());
         helper.close();
         
         if (abonadosMes != null)
         {
            ArrayList<String> nombresAbonadosMes = new ArrayList<String>();
            for (AbonadoMes abonado : abonadosMes)
               nombresAbonadosMes.add(abonado.getApellido() + " " + abonado.getNombre());
            
            adapter = new ArrayAdapter<String>(this.getActivity(), android.R.layout.simple_spinner_item, nombresAbonadosMes);
            spinner = (Spinner) view.findViewById(R.id.spiAbMensualesPagosAbonado);
            spinner.setAdapter(adapter);
         }
         else
         {
            Toast.makeText(this.getActivity(), R.string.errAbMensualesPagosSinAbonados, Toast.LENGTH_LONG).show();
            // TODO this.getActivity().onBackPressed();
         }
         
         super.onViewCreated(view, savedInstanceState);
      }
      
      // spiAbMensualesPagosAbonados OnItemSelected definition.
      public OnItemSelectedListener spiAbMensualesPagosAbonado_OnItemSelected = new OnItemSelectedListener()
      {
         @Override
         public void onItemSelected(AdapterView<?> parent, View view, int position, long id)
         {
            if (abonadosMes != null)
            {
               
            }
            else
            {
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbMensualesPagosSinAbonados, Toast.LENGTH_LONG).show();
               // TODO this.getActivity().onBackPressed();
            }
         }
         
         @Override
         public void onNothingSelected(AdapterView<?> parent)
         {
         }
      };
      
      // Buttons onClick definitions.
      public OnClickListener btnAbMensualesPagosCancelar_OnClick = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            PlaceholderFragment.this.getActivity().onBackPressed();
         }
      };
      
      public OnClickListener btnAbMensualesPagosAceptar_OnClick = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            v = PlaceholderFragment.this.getView();
            PagoMes pago = new PagoMes();
            
            // Checks if fields are completed and fills "pago".
            Spinner spinner = (Spinner) v.findViewById(R.id.spiAbMensualesPagosPeriodo);
            if (spinner.getSelectedItemPosition() < 0)
            {
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbMensualesPagosPeriodo, Toast.LENGTH_LONG).show();
               return;
            }
            else
            {
               Calendar calendar = Calendar.getInstance(Locale.getDefault());
               pago.setFechaPago(calendar.getTime());
               
               calendar.add(Calendar.MONTH, spinner.getSelectedItemPosition() - (range + 1));
               
               calendar.set(Calendar.DAY_OF_MONTH, 1);
               pago.setFechaDesde(calendar.getTime());
               
               calendar.set(Calendar.DAY_OF_MONTH, calendar.getMaximum(Calendar.DAY_OF_MONTH));
               pago.setFechaHasta(calendar.getTime());
            }
            
            EditText descripcion = (EditText) v.findViewById(R.id.txtAbMensualesPagosDescripcion);
            if (descripcion.getText().toString() == "")
            {
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbMensualesPagosExcepcion, Toast.LENGTH_LONG).show();
               return;
            }
            else
            {
               pago.setDescripcion(descripcion.getText().toString());
            }
            
            spinner = (Spinner) v.findViewById(R.id.spiAbMensualesPagosAbonado);
            if (spinner.getSelectedItemPosition() < 0)
            {
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbMensualesPagosAbonados, Toast.LENGTH_LONG).show();
               return;
            }
            else
            {
               pago.setAbonadoId(abonadosMes[spinner.getSelectedItemPosition()].getId());
            }
            
            // Gets corresponding TarifaMes based on selected AbonadoMes.
            DatabaseHelper helper = new DatabaseHelper(PlaceholderFragment.this.getActivity());
            
            TarifaMes tarifaMes = new TarifaMes();
            tarifaMes.setId(abonadosMes[spinner.getSelectedItemPosition()].getTarifaId());
            tarifaMes = ((TarifaMes[]) helper.selectFilter(tarifaMes))[0];
            
            pago.setPrecio(tarifaMes.getPrecio());
            pago.setTarifaId(tarifaMes.getId());
            
            int insertSuccess = helper.insert(pago);
            helper.close();
            
            if (insertSuccess != -1)
            {
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.toastAbMensualesPagosPagado, Toast.LENGTH_SHORT).show();
               PlaceholderFragment.this.getActivity().onBackPressed();
            }
            else
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbMensualesPagosInsert, Toast.LENGTH_LONG).show();
         }
      };
   }
}
