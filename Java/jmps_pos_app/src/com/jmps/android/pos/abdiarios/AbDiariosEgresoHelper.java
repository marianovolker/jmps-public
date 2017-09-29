package com.jmps.android.pos.abdiarios;

import java.util.Date;

import android.content.Context;
import android.widget.Toast;

import com.jmps.android.pos.R;
import com.jmps.android.pos.devices.printer.PrinterHelper;
import com.jmps.android.pos.sqlite.DatabaseHelper;
import com.jmps.android.pos.sqlite.TarifaDia;
import com.jmps.android.pos.sqlite.VehiculoDia;

public class AbDiariosEgresoHelper
{
   public static void egresar(Context context, VehiculoDia vehiculo)
   {
      DatabaseHelper helper = new DatabaseHelper(context);
      VehiculoDia[] vehiculos = (VehiculoDia[]) helper.selectFilter(vehiculo);
      if (vehiculos == null)
      {
         Toast.makeText(context, R.string.errAbDiariosEgresoVehiculoInexistente, Toast.LENGTH_LONG).show();
      }
      else
      {
         vehiculo = vehiculos[0];
         Date fechaEgreso = new Date();
         vehiculo.setFechaEgreso(fechaEgreso);
         
         vehiculo.setImporte(calcularImporte(context, vehiculo));
         
         if (vehiculo.getImporte() == -1)
         {
            Toast.makeText(context, R.string.errAbDiariosEgresoCalcularImporte, Toast.LENGTH_LONG).show();
            return;
         }
         else
         {
            helper.update(vehiculo);
            helper.delete(vehiculo);
            
            PrinterHelper.printTicketOut(context, vehiculo);
         }
      }
      
      helper.close();
      
   }
   
   private static double calcularImporte(Context context, VehiculoDia vehiculo)
   {
      double importe = -1;
      
      DatabaseHelper helper = new DatabaseHelper(context);
      
      TarifaDia tarifa = new TarifaDia();
      tarifa.setCategoriaId(vehiculo.getCategoriaId());
      TarifaDia[] tarifas = (TarifaDia[]) helper.selectAll(tarifa);
      
      helper.close();
      
      if (tarifas != null)
      {
         // Gets time in milliseconds
         long time = vehiculo.getFechaEgreso().getTime() - vehiculo.getFechaIngreso().getTime();
         time /= 1000; // Converts to seconds
         time /= 60; // Converts to minutes
         
         int tarifaTiempo = -1;
         double tarifaImporte = -1;
         long diferenciaTiempo = -1;
         long diferenciaTiempoAnterior = -1;
         
         for (int i = 0; i < tarifas.length; i++)
         {
            diferenciaTiempo = tarifas[i].getTiempo() - time;
            diferenciaTiempo = Math.abs(diferenciaTiempo);
            
            if (diferenciaTiempo == 0)
            {
               
               break;
            }
            else if (diferenciaTiempo < diferenciaTiempoAnterior)
            {
               tarifaTiempo = tarifas[i].getTiempo();
               tarifaImporte = tarifas[i].getPrecio();
            }
            
            diferenciaTiempoAnterior = diferenciaTiempo;
         }
         
         importe = tarifaImporte * (time / tarifaTiempo);
      }
      
      return importe;
   }
}
