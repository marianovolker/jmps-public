package com.jmps.android.pos.abmensuales;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

import android.app.Activity;
import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.sqlite.AbonadoMes;
import com.jmps.android.pos.sqlite.AutoMes;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.TarifaMes;

public class AbMensualesAltaActivity extends Activity
{
   
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_mensuales_alta);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   /**
    * A placeholder fragment containing a simple view.
    */
   public static class PlaceholderFragment extends Fragment
   {
      AbonadoMes abonadoMes;
      List<TarifaMes> tarifasMes;
      List<AutoMes> autosMes;
      int currentAutoMes;
      boolean isNew;
      
      public PlaceholderFragment()
      {
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_mensuales_alta, container, false);
         
         autosMes = new ArrayList<AutoMes>();
         tarifasMes = null;
         currentAutoMes = 0;
         
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
            abonadoMes = null;
         }
         else
         {
            isNew = false;
            abonadoMes = new AbonadoMes();
            abonadoMes.setId(args.getInt("id"));
         }
         
         DatabaseHelper helper = new DatabaseHelper(this.getActivity());
         
         TarifaMes[] tarifasArray = (TarifaMes[]) helper.selectAll(new TarifaMes());
         if (tarifasArray == null)
         {
            helper.close();
            Toast.makeText(this.getActivity(), R.string.errAbMensualesAltaSinTarifas, Toast.LENGTH_LONG).show();
            // TODO this.getActivity().onBackPressed();
         }
         else
         {
            tarifasMes = Arrays.asList(tarifasArray);
            
            List<String> items = new ArrayList<String>();
            for (TarifaMes item : tarifasMes)
            {
               items.add(item.getNombre());
            }
            
            ArrayAdapter<String> adapter = new ArrayAdapter<String>(this.getActivity(), android.R.layout.simple_spinner_item, items);
            
            Spinner spinner = (Spinner) view.findViewById(R.id.txtAbMensualesAltaTarifa);
            spinner.setAdapter(adapter);
         }
         
         if (!isNew)
         {
            // Gets existing values.
            abonadoMes = (AbonadoMes) helper.select(abonadoMes);
            
            AutoMes autoMes = new AutoMes();
            autoMes.setAbonadoId(abonadoMes.getId());
            autosMes = Arrays.asList((AutoMes[]) helper.selectFilter(autoMes));
            
            EditText edit = (EditText) view.findViewById(R.id.txtAbMensualesAltaNombre);
            edit.setText(abonadoMes.getNombre());
            
            edit = (EditText) view.findViewById(R.id.txtAbMensualesAltaApellido);
            edit.setText(abonadoMes.getApellido());
            
            edit = (EditText) view.findViewById(R.id.txtAbMensualesAltaDni);
            edit.setText(abonadoMes.getDni());
            
            edit = (EditText) view.findViewById(R.id.txtAbMensualesAltaDireccion);
            edit.setText(abonadoMes.getDireccion());
            
            edit = (EditText) view.findViewById(R.id.txtAbMensualesAltaTelefono1);
            edit.setText(abonadoMes.getTelefono1());
            
            edit = (EditText) view.findViewById(R.id.txtAbMensualesAltaTelefono2);
            edit.setText(abonadoMes.getTelefono2());
            
            edit = (EditText) view.findViewById(R.id.txtAbMensualesAltaTelefono3);
            edit.setText(abonadoMes.getTelefono3());
            
            // Looks for TarifaMes on loaded tarifas, so it can choose the correct spinner's item.
            int index = -1;
            for (int i = 0; i < tarifasMes.size(); i++)
            {
               if (tarifasMes.get(i).getId() == abonadoMes.getTarifaId())
               {
                  index = i;
                  break;
               }
            }
            
            if (index == -1)
            {
               helper.close();
               Toast.makeText(this.getActivity(), R.string.errAbMensualesAltaTarifaNotFound, Toast.LENGTH_SHORT).show();
               // TODO this.getActivity().onBackPressed();
            }
            else
            {
               Spinner spinner = (Spinner) view.findViewById(R.id.txtAbMensualesAltaTarifa);
               spinner.setSelection(index);
            }
            
            // Selects first AutoMes.
            btnAbMensualesAltaSiguiente_OnClick.onClick(view);
         }
         
         helper.close();
         
         // Sets onClick listeners.
         view.findViewById(R.id.btnAbMensualesAltaCancelar).setOnClickListener(btnAbMensualesAltaCancelar_OnClick);
         view.findViewById(R.id.btnAbMensualesAltaAnterior).setOnClickListener(btnAbMensualesAltaAnterior_OnClick);
         view.findViewById(R.id.btnAbMensualesAltaSiguiente).setOnClickListener(btnAbMensualesAltaSiguiente_OnClick);
         view.findViewById(R.id.btnAbMensualesAltaGuardar).setOnClickListener(btnAbMensualesAltaGuardar_OnClick);
         
         super.onViewCreated(view, savedInstanceState);
      }
      
      View.OnClickListener btnAbMensualesAltaCancelar_OnClick = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            PlaceholderFragment.this.getActivity().onBackPressed();
         }
      };
      
      View.OnClickListener btnAbMensualesAltaAnterior_OnClick = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            v = PlaceholderFragment.this.getView();
            
            if (currentAutoMes > 0)
            {
               currentAutoMes--;
               setAutoMesFields(v, currentAutoMes);
            }
         }
      };
      
      View.OnClickListener btnAbMensualesAltaSiguiente_OnClick = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            v = PlaceholderFragment.this.getView();
            
            if (currentAutoMes == autosMes.size())
            {
               if (checkAutoMesFields(v))
               {
                  // Add new AutoMes to list.
                  currentAutoMes++;
                  autosMes.add(newAutoMes(v));
                  clearAutoMesFields(v);
               }
            }
            else if (currentAutoMes == autosMes.size() - 1)
            {
               // Clear fields to add new AutoMes.
               autosMes.set(currentAutoMes, newAutoMes(v));
               currentAutoMes++;
               clearAutoMesFields(v);
            }
            else if (currentAutoMes < autosMes.size() - 1)
            {
               currentAutoMes++;
               setAutoMesFields(v, currentAutoMes);
            }
         }
      };
      
      View.OnClickListener btnAbMensualesAltaGuardar_OnClick = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            Context context = PlaceholderFragment.this.getActivity();
            v = PlaceholderFragment.this.getView();
            
            String s = "";
            
            // Check if required fields are completed.
            s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaNombre)).getText().toString();
            if (s.trim().isEmpty())
            {
               Toast.makeText(context, R.string.errAbMensualesAltaNombre, Toast.LENGTH_LONG).show();
               return;
            }
            else
               abonadoMes.setNombre(s);
            
            s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaApellido)).getText().toString();
            if (s.trim().isEmpty())
            {
               Toast.makeText(context, R.string.errAbMensualesAltaApellido, Toast.LENGTH_LONG).show();
               return;
            }
            else
               abonadoMes.setApellido(s);
            
            s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaDni)).getText().toString();
            if (s.trim().isEmpty())
            {
               Toast.makeText(context, R.string.errAbMensualesAltaDni, Toast.LENGTH_LONG).show();
               return;
            }
            else
               abonadoMes.setDni(s);
            
            s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaDireccion)).getText().toString();
            if (s.trim().isEmpty())
            {
               Toast.makeText(context, R.string.errAbMensualesAltaDireccion, Toast.LENGTH_LONG).show();
               return;
            }
            else
               abonadoMes.setDireccion(s);
            
            s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaTelefono1)).getText().toString();
            s += ((EditText) v.findViewById(R.id.txtAbMensualesAltaTelefono2)).getText().toString();
            s += ((EditText) v.findViewById(R.id.txtAbMensualesAltaTelefono3)).getText().toString();
            if (s.trim().isEmpty())
            {
               Toast.makeText(context, R.string.errAbMensualesAltaTelefono, Toast.LENGTH_LONG).show();
               return;
            }
            else
            {
               abonadoMes.setTelefono1(((EditText) v.findViewById(R.id.txtAbMensualesAltaTelefono1)).getText().toString());
               abonadoMes.setTelefono2(((EditText) v.findViewById(R.id.txtAbMensualesAltaTelefono2)).getText().toString());
               abonadoMes.setTelefono3(((EditText) v.findViewById(R.id.txtAbMensualesAltaTelefono3)).getText().toString());
            }
            
            // Gets TarifaMes id based on selected item on spinner.
            Spinner spinner = (Spinner) v.findViewById(R.id.txtAbMensualesAltaTarifa);
            int selectedTarifa = spinner.getSelectedItemPosition();
            abonadoMes.setTarifaId(tarifasMes.get(selectedTarifa).getId());
            
            if (autosMes.size() <= 0)
            {
               if (checkAutoMesFields(v)) // Next button wasn't pressed but a car was inputed.
               {
                  autosMes.add(newAutoMes(v));
               }
               else
               {
                  Toast.makeText(context, R.string.errAbMensualesAltaAuto, Toast.LENGTH_LONG).show();
                  return;
               }
            }
            abonadoMes.setFechaIngreso(new Date());
            Log.d("Fernando", "abonadomes id = " + abonadoMes.getId());
            // Insert data on database.
            DatabaseHelper helper = new DatabaseHelper(context);
            int id;
            if (isNew)
               id = helper.insert(abonadoMes);
            else
            {
               helper.update(abonadoMes);
               id = abonadoMes.getId();
            }
            
            if (id == -1)
            {
               helper.close();
               Toast.makeText(context, R.string.errAbMensualesAltaInsert, Toast.LENGTH_LONG).show();
               return;
            }
            
            for (AutoMes item : autosMes)
            {
               item.setAbonadoId(id);
               helper.insert(item);
            }
            
            helper.close();
            
            Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.toastAbMensualesAlta, Toast.LENGTH_SHORT).show();
            PlaceholderFragment.this.getActivity().onBackPressed();
         }
      };
      
      private void clearAutoMesFields(View v)
      {
         EditText t = (EditText) v.findViewById(R.id.txtAbMensualesAltaMarca);
         t.setText("");
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaModelo);
         t.setText("");
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaColor);
         t.setText("");
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaPatente);
         t.setText("");
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaNroMotor);
         t.setText("");
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaTurno);
         t.setText("");
      }
      
      private void setAutoMesFields(View v, int currentAutoMes)
      {
         clearAutoMesFields(v);
         
         EditText t = (EditText) v.findViewById(R.id.txtAbMensualesAltaMarca);
         t.setText(autosMes.get(currentAutoMes).getMarca());
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaModelo);
         t.setText(autosMes.get(currentAutoMes).getModelo());
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaColor);
         t.setText(autosMes.get(currentAutoMes).getColor());
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaPatente);
         t.setText(autosMes.get(currentAutoMes).getPatente());
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaNroMotor);
         t.setText(autosMes.get(currentAutoMes).getNroMotor());
         
         t = (EditText) v.findViewById(R.id.txtAbMensualesAltaTurno);
         t.setText(autosMes.get(currentAutoMes).getTurno());
      }
      
      private boolean checkAutoMesFields(View v)
      {
         Context context = this.getActivity();
         
         String s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaMarca)).getText().toString();
         if (s.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbMensualesAltaMarca, Toast.LENGTH_LONG).show();
            return false;
         }
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaModelo)).getText().toString();
         if (s.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbMensualesAltaModelo, Toast.LENGTH_LONG).show();
            return false;
         }
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaPatente)).getText().toString();
         if (s.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbMensualesAltaPatente, Toast.LENGTH_LONG).show();
            return false;
         }
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaColor)).getText().toString();
         if (s.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbMensualesAltaColor, Toast.LENGTH_LONG).show();
            return false;
         }
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaNroMotor)).getText().toString();
         if (s.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbMensualesAltaNroMotor, Toast.LENGTH_LONG).show();
            return false;
         }
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaTurno)).getText().toString();
         if (s.trim().isEmpty())
         {
            Toast.makeText(context, R.string.errAbMensualesAltaTurno, Toast.LENGTH_LONG).show();
            return false;
         }
         
         return true;
      }
      
      private AutoMes newAutoMes(View v)
      {
         AutoMes autoMes = new AutoMes();
         
         String s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaMarca)).getText().toString();
         autoMes.setMarca(s);
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaModelo)).getText().toString();
         autoMes.setModelo(s);
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaPatente)).getText().toString();
         autoMes.setPatente(s);
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaColor)).getText().toString();
         autoMes.setColor(s);
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaNroMotor)).getText().toString();
         autoMes.setNroMotor(s);
         
         s = ((EditText) v.findViewById(R.id.txtAbMensualesAltaTurno)).getText().toString();
         autoMes.setTurno(s);
         
         return autoMes;
      }
   }
}
