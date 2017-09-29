package com.jmps.android.pos.abmensuales;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Locale;

import android.app.Activity;
import android.app.ListFragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.AbonadoMes;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.PagoMes;

public class AbMensualesPendientesActivity extends Activity
{
   
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_mensuales_pendientes);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   /**
    * A placeholder fragment containing a simple view.
    */
   public static class PlaceholderFragment extends ListFragment
   {
      
      public PlaceholderFragment()
      {
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_mensuales_pendientes, container, false);
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         DatabaseHelper helper = new DatabaseHelper(this.getActivity());
         
         AbonadoMes[] abonados = (AbonadoMes[]) helper.selectAll(new AbonadoMes());
         
         // If there are no "AbonadoMes" registered.
         if (abonados == null)
         {
            helper.close();
            Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbMensualesPendientesSinAbonados, Toast.LENGTH_LONG).show();
            // TODO this.getActivity().onBackPressed();
         }
         else
         {
            ArrayList<String> pendientes = new ArrayList<String>();
            String listItem = "";
            
            for (AbonadoMes abonado : abonados)
            {
               PagoMes pago = new PagoMes();
               pago.setAbonadoId(abonado.getId());
               
               PagoMes[] pagos = (PagoMes[]) helper.selectFilter(pago);
               
               Calendar inDate = Calendar.getInstance(Locale.getDefault());
               Calendar today = Calendar.getInstance(Locale.getDefault());
               
               inDate.setTime(abonado.getFechaIngreso());
               inDate.set(Calendar.DAY_OF_MONTH, inDate.getMinimum(Calendar.MONTH));
               
               if (pagos == null)
               {
                  while (inDate.before(today))
                  {
                     listItem = abonado.getNombreCompleto() + " - ";
                     listItem += inDate.getDisplayName(Calendar.MONTH, Calendar.LONG, Locale.getDefault()) + " ";
                     listItem += inDate.get(Calendar.YEAR);
                     
                     pendientes.add(listItem);
                     
                     inDate.add(Calendar.MONTH, 1);
                  }
               }
               else
               {
                  // "abonado" has some payments.
                  Calendar fromDate = Calendar.getInstance(Locale.getDefault());
                  
                  int i = 0;
                  while (inDate.before(today))
                  {
                     if (pagos.length < i)
                     {
                        fromDate.setTime(pagos[i].getFechaDesde());
                        
                        int inDateInt = inDate.get(Calendar.YEAR) * 100 + inDate.get(Calendar.MONTH);
                        int fromDateInt = fromDate.get(Calendar.YEAR) * 100 + fromDate.get(Calendar.MONTH);
                        
                        if (inDateInt < fromDateInt)
                        {
                           listItem = abonado.getNombreCompleto() + " - ";
                           listItem += inDate.getDisplayName(Calendar.MONTH, Calendar.LONG, Locale.getDefault()) + " ";
                           listItem += inDate.get(Calendar.YEAR);
                           
                           pendientes.add(listItem);
                        }
                        else if (inDateInt == fromDateInt)
                        {
                           i++;
                        }
                        
                        inDate.add(Calendar.MONTH, 1);
                     }
                     else
                     {
                        // No more payments to check, then "abonado" owes payment until today.
                        listItem = abonado.getNombreCompleto() + " - ";
                        listItem += inDate.getDisplayName(Calendar.MONTH, Calendar.LONG, Locale.getDefault()) + " ";
                        listItem += inDate.get(Calendar.YEAR);
                        
                        pendientes.add(listItem);
                        
                        inDate.add(Calendar.MONTH, 1);
                     }
                  }
               }
            }
            
            // Sets content to ListView.
            ArrayAdapter<String> adapter = new ArrayAdapter<String>(this.getActivity(), android.R.layout.simple_list_item_1, pendientes);
            ListView list = (ListView) view.findViewById(R.id.lstAbMensualesPendientes);
            list.setAdapter(adapter);
         }
         
         helper.close();
         
         super.onViewCreated(view, savedInstanceState);
      }
   }
   
}
