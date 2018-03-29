package com.jmps.android.pos.abdiarios;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ListFragment;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.TarifaDia;

public class AbDiariosTarifasEliminarActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_diarios_tarifas_eliminar);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   
   public static class PlaceholderFragment extends ListFragment
   {
      private List<TarifaDia> tarifasDia;
      
      public PlaceholderFragment()
      {
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_diarios_tarifas_eliminar, container, false);

         tarifasDia = null;
         
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         ListView lv = (ListView) view.findViewById(R.id.lstAbDiariosTarifas);
         
         // Get data from DB.
         DatabaseHelper helper = new DatabaseHelper(this.getActivity());
         TarifaDia[] array = (TarifaDia[]) helper.selectAll(new TarifaDia());
         helper.close();
         
         if (array != null)
         {
            tarifasDia = Arrays.asList(array);
            
            // List containing ListView strings.
            List<Map<String, String>> dataNew = new ArrayList<Map<String, String>>();
            for (TarifaDia item : tarifasDia)
            {
               Map<String, String> m = new HashMap<String, String>(2);
               m.put("title", item.getNombre());
               m.put("subtitle", item.getDescripcion() + "; Precio: " + item.getPrecio());
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
         PopupListener listener = new PopupListener(position);
         
         AlertDialog.Builder builder = new AlertDialog.Builder(this.getActivity());
         builder.setTitle("Eliminar");
         builder.setMessage("�Est� seguro que quiere eliminar la tarifa " + tarifasDia.get(position).getDescripcion() + "?");
         builder.setPositiveButton(android.R.string.yes, listener);
         builder.setNegativeButton(android.R.string.no, listener);
         
         AlertDialog popup = builder.create();
         popup.show();
         
         super.onListItemClick(l, v, position, id);
      }
      
      private class PopupListener implements OnClickListener
      {
         private int position;
         
         public PopupListener(int position)
         {
            this.position = position;
         }
         
         @Override
         public void onClick(DialogInterface dialog, int which)
         {
            switch(which)
            {
               case DialogInterface.BUTTON_POSITIVE :
                  DatabaseHelper helper = new DatabaseHelper(PlaceholderFragment.this.getActivity());
                  helper.delete(tarifasDia.get(position));
                  helper.close();
                  Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.toastAbDiariosTarifaEliminacion,
                     Toast.LENGTH_SHORT).show();
                  PlaceholderFragment.this.getActivity().onBackPressed();
                  break;
               case DialogInterface.BUTTON_NEGATIVE :
                  break;
            }
         }
      }
   }
   
}
