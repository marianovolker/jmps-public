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
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.TarifaMes;

public class AbMensualesTarifasModificarActivity extends Activity
{   
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_mensuales_tarifas_modificar);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   public static class PlaceholderFragment extends ListFragment
   {
      private List<TarifaMes> tarifasMes;
      
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
         DatabaseHelper dbh = new DatabaseHelper(this.getActivity());
         tarifasMes = Arrays.asList((TarifaMes[]) dbh.selectAll(new TarifaMes()));;
         
         // List containing ListView strings.
         List<Map<String, String>> dataNew = new ArrayList<Map<String, String>>();
         for (TarifaMes item : tarifasMes)
         {
            Map<String, String> m = new HashMap<String, String>(2);
            m.put("title", item.getNombre());
            m.put("subtitle", item.getDescripcion() + "; Per�odo: " + item.getPeriodo() + ", Costo: " + item.getPrecio());
            dataNew.add(m);
         }
         
         // Assign content to ListView.
         SimpleAdapter adapter = new SimpleAdapter(this.getActivity(), dataNew, android.R.layout.simple_list_item_2,
                                                   new String[] { "title", "subtitle" },
                                                   new int[] { android.R.id.text1, android.R.id.text2 });
         
         lv.setAdapter(adapter);
         
         super.onViewCreated(view, savedInstanceState);
      }
      
      @Override
      public void onListItemClick(ListView l, View v, int position, long id)
      {
         Bundle args = new Bundle();
         args.putInt("id", tarifasMes.get(position).getId());
         
         AbMensualesTarifaEdicionFragment frag = new AbMensualesTarifaEdicionFragment();
         frag.setArguments(args);
         
         FragmentTransaction fragTrans = this.getFragmentManager().beginTransaction();
         fragTrans.replace(R.id.container, frag);
         fragTrans.addToBackStack(null);
         fragTrans.commit();
         
         super.onListItemClick(l, v, position, id);
      }
   }
   
}
