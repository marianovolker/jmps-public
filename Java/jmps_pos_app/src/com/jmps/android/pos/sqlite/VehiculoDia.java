package com.jmps.android.pos.sqlite;

import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.content.ContentValues;
import android.database.Cursor;

public class VehiculoDia extends GenericTable implements IGenericTable
{
   private int categoriaId;
   private String patenteLetras;
   private String patenteNumeros;
   private Date fechaIngreso;
   private Date fechaEgreso;
   private double importe;
   
   public VehiculoDia()
   {
      super();
      
      orderBy = DbHelperConsts.VEHICULOS_DIA_FECHA_INGRESO;
      table = DbHelperConsts.TABLE_VEHICULOS_DIA;
      
      categoriaId = -1;
      patenteLetras = null;
      patenteNumeros = null;
      fechaIngreso = null;
      fechaEgreso = null;
      importe = -1;
   }
   
   @Override
   public String getWhere()
   {
      String where = DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL";
      
      if (id != -1)
         where += " AND " + DbHelperConsts.KEY_ID + "=" + id;
      
      if (categoriaId != -1)
         where += " AND " + DbHelperConsts.VEHICULOS_DIA_CATEGORIA_DIA_ID + "=" + categoriaId;
      
      if (patenteLetras != null)
         where += " AND " + DbHelperConsts.VEHICULOS_DIA_PATENTE_LETRAS + "='" + patenteLetras + "'";
      
      if (patenteNumeros != null)
         where += " AND " + DbHelperConsts.VEHICULOS_DIA_PATENTE_NUMEROS + "='" + patenteNumeros + "'";
      
      if (fechaIngreso != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         where += " AND " + DbHelperConsts.VEHICULOS_DIA_FECHA_INGRESO + "=" + formatter.format(fechaIngreso);
      }
      
      if (fechaEgreso != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         where += " AND " + DbHelperConsts.VEHICULOS_DIA_FECHA_EGRESO + "=" + formatter.format(fechaEgreso);
      }
      
      if (importe != -1)
         where += " AND " + DbHelperConsts.VEHICULOS_DIA_IMPORTE + "=" + importe;
      
      return where;
   }
   
   @Override
   public ContentValues getContentValues(boolean isNew)
   {
      super.getContentValues(isNew);
      
      values.put(DbHelperConsts.VEHICULOS_DIA_CATEGORIA_DIA_ID, categoriaId);
      values.put(DbHelperConsts.VEHICULOS_DIA_PATENTE_LETRAS, patenteLetras);
      values.put(DbHelperConsts.VEHICULOS_DIA_PATENTE_NUMEROS, patenteNumeros);
      values.put(DbHelperConsts.VEHICULOS_DIA_IMPORTE, importe);
      
      if (fechaIngreso != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         String s = formatter.format(fechaIngreso);
         values.put(DbHelperConsts.VEHICULOS_DIA_FECHA_INGRESO, s);
      }
      
      if (fechaEgreso != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         String s = formatter.format(fechaEgreso);
         values.put(DbHelperConsts.VEHICULOS_DIA_FECHA_EGRESO, s);
      }
      
      return values;
   }
   
   @Override
   public IGenericTable extractItemFromCursor(Cursor cursor)
   {
      VehiculoDia vehiculoDia = new VehiculoDia();
      
      vehiculoDia.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
      vehiculoDia.setCategoriaId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.VEHICULOS_DIA_CATEGORIA_DIA_ID)));
      vehiculoDia.setPatenteLetras(cursor.getString(cursor.getColumnIndex(DbHelperConsts.VEHICULOS_DIA_PATENTE_LETRAS)));
      vehiculoDia.setPatenteNumeros(cursor.getString(cursor.getColumnIndex(DbHelperConsts.VEHICULOS_DIA_PATENTE_NUMEROS)));
      vehiculoDia.setImporte(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.VEHICULOS_DIA_IMPORTE)));
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.VEHICULOS_DIA_FECHA_INGRESO)));
         vehiculoDia.setFechaIngreso(d);
      }
      catch (Exception ex)
      {
         vehiculoDia.setFechaIngreso(null);
      }
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.VEHICULOS_DIA_FECHA_EGRESO)));
         vehiculoDia.setFechaEgreso(d);
      }
      catch (Exception ex)
      {
         vehiculoDia.setFechaEgreso(null);
      }
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
         vehiculoDia.setFechaEliminacion(d);
      }
      catch (Exception ex)
      {
         vehiculoDia.setFechaEliminacion(null);
      }
      
      return vehiculoDia;
   }
   
   @Override
   public IGenericTable[] extractArrayFromCursor(Cursor cursor)
   {
      List<VehiculoDia> vehiculosDia = new ArrayList<VehiculoDia>();
      
      do
      {
         VehiculoDia vehiculoDia = new VehiculoDia();
         
         vehiculoDia.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
         vehiculoDia.setCategoriaId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.VEHICULOS_DIA_CATEGORIA_DIA_ID)));
         vehiculoDia.setPatenteLetras(cursor.getString(cursor.getColumnIndex(DbHelperConsts.VEHICULOS_DIA_PATENTE_LETRAS)));
         vehiculoDia.setPatenteNumeros(cursor.getString(cursor.getColumnIndex(DbHelperConsts.VEHICULOS_DIA_PATENTE_NUMEROS)));
         vehiculoDia.setImporte(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.VEHICULOS_DIA_IMPORTE)));
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.VEHICULOS_DIA_FECHA_INGRESO)));
            vehiculoDia.setFechaIngreso(d);
         }
         catch (Exception ex)
         {
            vehiculoDia.setFechaIngreso(null);
         }
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.VEHICULOS_DIA_FECHA_EGRESO)));
            vehiculoDia.setFechaEgreso(d);
         }
         catch (Exception ex)
         {
            vehiculoDia.setFechaEgreso(null);
         }
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
            vehiculoDia.setFechaEliminacion(d);
         }
         catch (Exception ex)
         {
            vehiculoDia.setFechaEliminacion(null);
         }
         
         vehiculosDia.add(vehiculoDia);
      } while (cursor.moveToNext());
      
      return vehiculosDia.toArray(new VehiculoDia[vehiculosDia.size()]);
   }
   
   // Getters and setters.
   public int getCategoriaId()
   {
      return categoriaId;
   }
   
   public void setCategoriaId(int categoriaId)
   {
      this.categoriaId = categoriaId;
   }
   
   public String getPatenteLetras()
   {
      return patenteLetras;
   }
   
   public void setPatenteLetras(String patenteLetras)
   {
      this.patenteLetras = patenteLetras;
   }
   
   public String getPatenteNumeros()
   {
      return patenteNumeros;
   }
   
   public void setPatenteNumeros(String patenteNumeros)
   {
      this.patenteNumeros = patenteNumeros;
   }
   
   public String getPatente()
   {
      return getPatenteLetras() + " " + getPatenteNumeros();
   }
   
   public Date getFechaIngreso()
   {
      return fechaIngreso;
   }
   
   public void setFechaIngreso(Date fechaIngreso)
   {
      this.fechaIngreso = fechaIngreso;
   }
   
   public Date getFechaEgreso()
   {
      return fechaEgreso;
   }
   
   public void setFechaEgreso(Date fechaEgreso)
   {
      this.fechaEgreso = fechaEgreso;
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
