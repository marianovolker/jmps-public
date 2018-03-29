package com.jmps.android.pos.sqlite;

import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.content.ContentValues;
import android.database.Cursor;

public class PagoDia extends GenericTable implements IGenericTable
{
   private int vehiculoId;
   private Date fechaPago;
   private double importe;
   
   public PagoDia()
   {
      super();
      
      orderBy = DbHelperConsts.PAGOS_DIA_FECHA_PAGO;
      table = DbHelperConsts.TABLE_PAGOS_DIA;
      
      vehiculoId = -1;
      fechaPago = null;
      importe = -1;
   }
   
   @Override
   public String getWhere()
   {
      String where = DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL";
      
      if (id != -1)
         where += " AND " + DbHelperConsts.KEY_ID + "=" + id;
      
      if (vehiculoId != -1)
         where += " AND " + DbHelperConsts.PAGOS_DIA_VEHICULO_DIA_ID + "=" + vehiculoId;
      
      if (importe != -1)
         where += " AND " + DbHelperConsts.PAGOS_DIA_IMPORTE + "=" + importe;
      
      if (fechaPago != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         where += " AND " + DbHelperConsts.PAGOS_DIA_FECHA_PAGO + "=" + formatter.format(fechaPago);
      }
      
      return where;
   }
   
   @Override
   public ContentValues getContentValues(boolean isNew)
   {
      super.getContentValues(isNew);
      
      values.put(DbHelperConsts.PAGOS_DIA_VEHICULO_DIA_ID, vehiculoId);
      values.put(DbHelperConsts.PAGOS_DIA_IMPORTE, importe);
      
      if (fechaPago != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         String s = formatter.format(fechaPago);
         values.put(DbHelperConsts.PAGOS_DIA_FECHA_PAGO, s);
      }
      
      return values;
   }
   
   @Override
   public IGenericTable extractItemFromCursor(Cursor cursor)
   {
      PagoDia pagoDia = new PagoDia();
      
      pagoDia.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
      pagoDia.setVehiculoId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.PAGOS_DIA_VEHICULO_DIA_ID)));
      pagoDia.setImporte(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.PAGOS_DIA_IMPORTE)));
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.PAGOS_DIA_FECHA_PAGO)));
         pagoDia.setFechaPago(d);
      }
      catch (Exception ex)
      {
         pagoDia.setFechaPago(null);
      }
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
         pagoDia.setFechaEliminacion(d);
      }
      catch (Exception ex)
      {
         pagoDia.setFechaEliminacion(null);
      }
      
      return pagoDia;
   }
   
   @Override
   public IGenericTable[] extractArrayFromCursor(Cursor cursor)
   {
      List<PagoDia> pagosDia = new ArrayList<PagoDia>();
      
      do
      {
         PagoDia pagoDia = new PagoDia();
         
         pagoDia.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
         pagoDia.setVehiculoId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.PAGOS_DIA_VEHICULO_DIA_ID)));
         pagoDia.setImporte(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.PAGOS_DIA_IMPORTE)));
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.PAGOS_DIA_FECHA_PAGO)));
            pagoDia.setFechaPago(d);
         }
         catch (Exception ex)
         {
            pagoDia.setFechaPago(null);
         }
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
            pagoDia.setFechaEliminacion(d);
         }
         catch (Exception ex)
         {
            pagoDia.setFechaEliminacion(null);
         }
         
         pagosDia.add(pagoDia);
      } while (cursor.moveToNext());
      
      return pagosDia.toArray(new PagoDia[pagosDia.size()]);
   }
   
   // Getters and setters.
   public int getVehiculoId()
   {
      return vehiculoId;
   }
   
   public void setVehiculoId(int vehiculoId)
   {
      this.vehiculoId = vehiculoId;
   }
   
   public Date getFechaPago()
   {
      return fechaPago;
   }
   
   public void setFechaPago(Date fechaPago)
   {
      this.fechaPago = fechaPago;
   }
   
   public double getImporte()
   {
      return importe;
   }
   
   public void setImporte(double importe)
   {
      this.importe = importe;
   }
}
