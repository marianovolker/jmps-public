package com.jmps.android.pos.abdiarios;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

import android.app.Activity;
import android.app.Fragment;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.google.zxing.BarcodeFormat;
import com.google.zxing.common.BitMatrix;
import com.google.zxing.qrcode.QRCodeWriter;
import com.jmps.android.pos.R;
import com.jmps.android.pos.devices.printer.PrinterHelper;
import com.jmps.android.pos.sqlite.CategoriaDia;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.VehiculoDia;

public class AbDiariosIngresoActivity extends Activity
{
   @Override
   protected void onCreate(Bundle savedInstanceState)
   {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_ab_diarios_ingreso);
      
      if (savedInstanceState == null)
      {
         getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
      }
   }
   
   public static class PlaceholderFragment extends Fragment
   {
      List<CategoriaDia> categoriasDiaSpinnerContent;
      
      public PlaceholderFragment()
      {
      }
      
      @Override
      public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
      {
         View rootView = inflater.inflate(R.layout.fragment_ab_diarios_ingreso, container, false);
         
         categoriasDiaSpinnerContent = null;
         
         return rootView;
      }
      
      @Override
      public void onViewCreated(View view, Bundle savedInstanceState)
      {
         // Set buttons' OnClickListeners.
         view.findViewById(R.id.btnAbDiariosIngresoGuardar).setOnClickListener(btnGuardar);
         view.findViewById(R.id.btnAbDiariosIngresoCancelar).setOnClickListener(btnCancelar);
         
         // Fills "Categoria" spinner.
         DatabaseHelper helper = new DatabaseHelper(this.getActivity());
         CategoriaDia[] items = (CategoriaDia[]) helper.selectAll(new CategoriaDia());
         
         // If there are no categories.
         if (items == null)
         {
            helper.close();
            Toast.makeText(this.getActivity(), R.string.errAbDiariosIngresarSinCategorias, Toast.LENGTH_LONG).show();
            // TODO this.getActivity().onBackPressed();
         }
         else
         {
            categoriasDiaSpinnerContent = Arrays.asList(items);
            
            List<String> itemsAsString = new ArrayList<String>();
            for (CategoriaDia item : categoriasDiaSpinnerContent)
            {
               itemsAsString.add(item.getNombre());
            }
            
            ArrayAdapter<String> adapter = new ArrayAdapter<String>(this.getActivity(), android.R.layout.simple_spinner_item, itemsAsString);
            Spinner spinner = (Spinner) view.findViewById(R.id.spiAbDiariosIngresoCategoria);
            spinner.setAdapter(adapter);
         }
         
         helper.close();
         
         super.onViewCreated(view, savedInstanceState);
      }
      
      // Buttons' OnClickListeners.
      OnClickListener btnCancelar = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            PlaceholderFragment.this.getActivity().onBackPressed();
         }
      };
      
      OnClickListener btnGuardar = new OnClickListener()
      {
         @Override
         public void onClick(View v)
         {
            v = PlaceholderFragment.this.getView();
            VehiculoDia vehiculo = new VehiculoDia();
            
            EditText text = (EditText) v.findViewById(R.id.txtAbDiariosIngresoPatenteLetras);
            String s = text.getText().toString();
            if (s.trim().length() != 3)
            {
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbDiariosIngresoSinPatenteLetras, Toast.LENGTH_LONG).show();
               return;
            }
            else
            {
               vehiculo.setPatenteLetras(s);
            }
            
            text = (EditText) v.findViewById(R.id.txtAbDiariosIngresoPatenteNumeros);
            s = text.getText().toString();
            if (s.trim().length() != 3)
            {
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbDiariosIngresoSinPatenteNumeros, Toast.LENGTH_LONG).show();
               return;
            }
            else
            {
               vehiculo.setPatenteNumeros(s);
            }
            
            Spinner spiCategorias = (Spinner) v.findViewById(R.id.spiAbDiariosIngresoCategoria);
            CategoriaDia categoria = categoriasDiaSpinnerContent.get(spiCategorias.getSelectedItemPosition());
            vehiculo.setCategoriaId(categoria.getId());
            
            vehiculo.setFechaIngreso(new Date());
            
            DatabaseHelper helper = new DatabaseHelper(PlaceholderFragment.this.getActivity());
            helper.insert(vehiculo);
            helper.close();
            
            // Generate QR code
            QRCodeWriter writer = new QRCodeWriter();
            BitMatrix matrix;
            try
            {
               int qrHeight = 400;
               int qrWidth = 400;
               matrix = writer.encode(vehiculo.getPatente(), BarcodeFormat.QR_CODE, qrWidth, qrHeight);
            }
            catch (Exception ex)
            {
               Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.errAbDiariosIngresoQr, Toast.LENGTH_LONG);
               return;
            }
            
            Bitmap bmp = toBitmap(matrix);
            
            PrinterHelper.printTicketIn(PlaceholderFragment.this.getActivity(), vehiculo, bmp);
            
            Toast.makeText(PlaceholderFragment.this.getActivity(), R.string.toastAbDiariosIngreso, Toast.LENGTH_SHORT).show();
            PlaceholderFragment.this.getActivity().onBackPressed();
         }
         
         private Bitmap toBitmap(BitMatrix matrix)
         {
            int h = matrix.getHeight();
            int w = matrix.getWidth();
            Bitmap bmp = Bitmap.createBitmap(w, h, Bitmap.Config.RGB_565);
            for (int x = 0; x < w; x++)
            {
               for (int y = 0; y < h; y++)
               {
                  bmp.setPixel(x, y, matrix.get(x, y) ? Color.BLACK : Color.WHITE);
               }
            }
            return bmp;
         }
      };
   }
   
}
