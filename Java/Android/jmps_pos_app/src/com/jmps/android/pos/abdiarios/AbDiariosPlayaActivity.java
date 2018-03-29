package com.jmps.android.pos.abdiarios;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import android.app.Activity;
import android.app.ListFragment;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.VehiculoDia;

public class AbDiariosPlayaActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_diarios_playa);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   public static class PlaceholderFragment extends ListFragment
   {
      List<VehiculoDia> vehiculosDia;
      
      public PlaceholderFragment()
      {
         vehiculosDia = null;
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_diarios_playa, container, false);
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         ListView lv = (ListView) view.findViewById(R.id.lstAbDiariosPlayaAutos);
         
         // Get data from DB.
         DatabaseHelper helper = new DatabaseHelper(this.getActivity());
         VehiculoDia[] array = (VehiculoDia[]) helper.selectAll(new VehiculoDia());
         helper.close();
         
         if (array != null)
         {
            vehiculosDia = Arrays.asList(array);
            
            // List containing ListView strings.
            List<Map<String, String>> dataNew = new ArrayList<Map<String, String>>();
            for (VehiculoDia item : vehiculosDia)
            {
               String fechaIngreso = new SimpleDateFormat("dd-MM-yyyy HH:mm:ss", Locale.getDefault()).format(item.getFechaIngreso());
               
               Map<String, String> m = new HashMap<String, String>(2);
               m.put("title", item.getPatente());
               m.put("subtitle", fechaIngreso);
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
            Toast.makeText(this.getActivity(), R.string.errAbDiariosPlayaVacia, Toast.LENGTH_LONG).show();
         }
         
         super.onViewCreated(view, savedInstanceState);
      }
      
      @Override
      public void onListItemClick(ListView l, View v, int position, long id)
      {
         Intent intent = new Intent(this.getActivity(), AbDiariosEgresoActivity.class);
         Bundle extras = new Bundle();
         
         extras.putInt("vehiculoDiaId", vehiculosDia.get(position).getId());
         intent.putExtras(extras);
         
         this.getActivity().startActivity(intent);
         this.getActivity().finish();
      }
   }
   
}
