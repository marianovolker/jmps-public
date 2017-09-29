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
import com.jmps.android.pos.sqlite.CategoriaDia;
import com.jmps.android.pos.sqlite.DatabaseHelper;

public class AbDiariosCategoriasEliminarActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_diarios_categorias_eliminar);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   public static class PlaceholderFragment extends ListFragment
   {
      List<CategoriaDia> categoriasDia;
      
      public PlaceholderFragment()
      {
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_diarios_categorias_eliminar, container, false);
         
         categoriasDia = null;
         
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         ListView lv = (ListView) view.findViewById(R.id.lstAbDiariosCategoriasEliminar);
         
         // Get data from DB.
         DatabaseHelper helper = new DatabaseHelper(this.getActivity());
         CategoriaDia[] array = (CategoriaDia[]) helper.selectAll(new CategoriaDia());
         helper.close();
         
         if (array != null)
         {
            categoriasDia = Arrays.asList(array);
            
            // List containing ListView strings.
            List<Map<String, String>> dataNew = new ArrayList<Map<String, String>>();
            for (CategoriaDia item : categoriasDia)
            {
               Map<String, String> m = new HashMap<String, String>(2);
               m.put("title", item.getNombre());
               dataNew.add(m);
            }
            
            // Assign content to ListView.
            SimpleAdapter adapter = new SimpleAdapter(this.getActivity(), dataNew, android.R.layout.simple_list_item_1,
                                                      new String[] { "title" }, new int[] { android.R.id.text1 });
            
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
         builder.setMessage("�Est� seguro que quiere eliminar la categor�a " + categoriasDia.get(position).getNombre() + "?");
         builder.setPositiveButton(android.R.string.yes, listener);
         builder.setNegativeButton(android.R.string.no, listener);
         
         AlertDialog popup = builder.create();
         popup.show();
         
         super.onListItemClick(l, v, position, id);
      }
      
      private class PopupListener implements OnClickListener
      {
         private final int position;
         
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
                  helper.delete(categoriasDia.get(position));
                  helper.close();
                  Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.toastAbDiariosCategoriaEliminacion, Toast.LENGTH_SHORT).show();
                  PlaceholderFragment.this.getActivity().onBackPressed();
                  break;
               case DialogInterface.BUTTON_NEGATIVE :
                  break;
            }
         }
      }
   }
   
}
