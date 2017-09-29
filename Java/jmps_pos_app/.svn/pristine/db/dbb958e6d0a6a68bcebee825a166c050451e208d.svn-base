package com.jmps.android.pos.abmensuales;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.FragmentTransaction;
import android.app.ListFragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.SimpleAdapter;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.AbonadoMes;
import com.jmps.android.pos.sqlite.DatabaseHelper;

public class AbMensualesModificarActivity extends Activity
{
   
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_mensuales_modificar);
      
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
      private List<AbonadoMes> abonadosMes;
      
      public PlaceholderFragment()
      {
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_mensuales_modificar, container, false);
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         ListView lv = (ListView) view.findViewById(R.id.lstAbMensualesModificar);
         
         // Get data from DB.
         DatabaseHelper dbh = new DatabaseHelper(this.getActivity());
         AbonadoMes[] array = (AbonadoMes[]) dbh.selectAll(new AbonadoMes());
         
         if (array != null)
         {
            abonadosMes = Arrays.asList(array);
            
            // List containing ListView strings.
            List<Map<String, String>> dataNew = new ArrayList<Map<String, String>>();
            for (AbonadoMes item : abonadosMes)
            {
               Map<String, String> m = new HashMap<String, String>(2);
               m.put("title", item.getNombre() + " " + item.getApellido());
               m.put("subtitle", "DNI: " + item.getDni());
               dataNew.add(m);
            }
            
            // Assign content to ListView.
            SimpleAdapter adapter = new SimpleAdapter(this.getActivity(), dataNew, android.R.layout.simple_list_item_2,
                                                      new String[] { "title", "subtitle" }, new int[] { android.R.id.text1, android.R.id.text2 });
            
            lv.setAdapter(adapter);
         }
         
         super.onViewCreated(view, savedInstanceState);
      }
      
      @Override
      public void onListItemClick(ListView l, View v, int position, long id)
      {
         Bundle args = new Bundle();
         args.putInt("id", abonadosMes.get(position).getId());
         
         AbMensualesAltaActivity.PlaceholderFragment frag = new AbMensualesAltaActivity.PlaceholderFragment();
         frag.setArguments(args);
         
         FragmentTransaction fragTrans = this.getFragmentManager().beginTransaction();
         fragTrans.replace(R.id.container, frag);
         fragTrans.addToBackStack(null);
         fragTrans.commit();
         
         super.onListItemClick(l, v, position, id);
      }
   }
   
}
