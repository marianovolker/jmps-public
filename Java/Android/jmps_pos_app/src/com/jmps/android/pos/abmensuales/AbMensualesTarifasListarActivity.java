package com.jmps.android.pos.abmensuales;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.ListFragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.TarifaMes;

public class AbMensualesTarifasListarActivity extends Activity
{
   
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_mensuales_tarifas_listar);
      
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
         View rootView = inflater.inflate(R.layout.fragment_ab_mensuales_tarifas_listar, container, false);
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         ListView lv = (ListView) view.findViewById(R.id.lstAbMensualesTarifas);
         
         // Get data from DB.
         DatabaseHelper helper = new DatabaseHelper(this.getActivity());
         TarifaMes[] array = (TarifaMes[]) helper.selectAll(new TarifaMes());
         helper.close();
         
         if (array != null)
         {
            List<TarifaMes> tarifasMes = Arrays.asList(array);
            
            // List containing ListView strings.
            List<Map<String, String>> dataNew = new ArrayList<Map<String, String>>();
            for (TarifaMes item : tarifasMes)
            {
               Map<String, String> m = new HashMap<String, String>(2);
               m.put("title", item.getNombre());
               m.put("subtitle", item.getDescripcion() + "; Período: " + item.getPeriodo() + ", Costo: " + item.getPrecio());
               dataNew.add(m);
            }
            
            // Assign content to ListView.
            SimpleAdapter adapter = new SimpleAdapter(this.getActivity(), dataNew, android.R.layout.simple_list_item_2,
                                                      new String[] { "title", "subtitle" },
                                                      new int[] { android.R.id.text1, android.R.id.text2 });
            
            lv.setAdapter(adapter);
         }
         else
         {
            Toast.makeText(this.getActivity(), R.string.errAbMensualesTarifasListarSinTarifas, Toast.LENGTH_LONG).show();
            // TODO this.getActivity().onBackPressed();
         }
         
         super.onViewCreated(view, savedInstanceState);
      }
   }
   
}
