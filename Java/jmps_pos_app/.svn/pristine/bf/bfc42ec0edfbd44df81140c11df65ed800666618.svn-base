package com.jmps.android.pos.sqlite;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.content.ContentValues;
import android.database.Cursor;

public class TarifaMes extends GenericTable implements IGenericTable
{
   private String nombre;
   private String descripcion;
   private String excepcion;
   private String periodo;
   private double precio;
   
   public TarifaMes()
   {
      super();
      
      orderBy = DbHelperConsts.TARIFAS_MES_DESCRIPCION;
      table = DbHelperConsts.TABLE_TARIFAS_MES;
      
      nombre = null;
      descripcion = null;
      excepcion = null;
      periodo = null;
      precio = -1;
   }
   
   public String getWhere()
   {
      String where = DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL";
      
      if (id != -1)
         where += " AND " + DbHelperConsts.KEY_ID + "=" + id;
      
      if (nombre != null)
         where += " AND " + DbHelperConsts.TARIFAS_MES_NOMBRE + "=" + nombre;
      
      if (descripcion != null)
         where += " AND " + DbHelperConsts.TARIFAS_MES_DESCRIPCION + "=" + descripcion;
      
      if (excepcion != null)
         where += " AND " + DbHelperConsts.TARIFAS_MES_EXCEPCION + "=" + excepcion;
      
      if (periodo != null)
         where += " AND " + DbHelperConsts.TARIFAS_MES_PERIODO + "=" + periodo;
      
      if (precio != -1)
         where += " AND " + DbHelperConsts.TARIFAS_MES_COSTO + "=" + precio;
      
      return where;
   }
   
   public ContentValues getContentValues(boolean isNew)
   {
      super.getContentValues(isNew);
      
      values.put(DbHelperConsts.TARIFAS_MES_NOMBRE, nombre);
      values.put(DbHelperConsts.TARIFAS_MES_DESCRIPCION, descripcion);
      values.put(DbHelperConsts.TARIFAS_MES_EXCEPCION, excepcion);
      values.put(DbHelperConsts.TARIFAS_MES_PERIODO, periodo);
      values.put(DbHelperConsts.TARIFAS_MES_COSTO, precio);
      
      return values;
   }
   
   public IGenericTable extractItemFromCursor(Cursor cursor)
   {
      TarifaMes tarifaMes = new TarifaMes();
      
      tarifaMes.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
      tarifaMes.setNombre(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_NOMBRE)));
      tarifaMes.setDescripcion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_DESCRIPCION)));
      tarifaMes.setExcepcion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_EXCEPCION)));
      tarifaMes.setPeriodo(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_PERIODO)));
      tarifaMes.setPrecio(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_COSTO)));
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
         tarifaMes.setFechaEliminacion(d);
      }
      catch (Exception ex)
      {
         tarifaMes.setFechaEliminacion(null);
      }
      
      return tarifaMes;
   }
   
   public IGenericTable[] extractArrayFromCursor(Cursor cursor)
   {
      List<TarifaMes> tarifasMes = new ArrayList<TarifaMes>();
      
      do
      {
         TarifaMes tarifaMes = new TarifaMes();
         tarifaMes.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
         tarifaMes.setNombre(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_NOMBRE)));
         tarifaMes.setDescripcion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_DESCRIPCION)));
         tarifaMes.setExcepcion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_EXCEPCION)));
         tarifaMes.setPeriodo(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_PERIODO)));
         tarifaMes.setPrecio(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_COSTO)));
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
            tarifaMes.setFechaEliminacion(d);
         }
         catch (Exception ex)
         {
            tarifaMes.setFechaEliminacion(null);
         }
         
         tarifasMes.add(tarifaMes);
      } while (cursor.moveToNext());
      
      return tarifasMes.toArray(new TarifaMes[tarifasMes.size()]);
   }
   
   // Getters and setters.
   public String getNombre()
   {
      return nombre;
   }
   
   public void setNombre(String nombre)
   {
      this.nombre = nombre;
   }
   
   public String getDescripcion()
   {
      return descripcion;
   }
   
   public void setDescripcion(String descripcion)
   {
      this.descripcion = descripcion;
   }
   
   public String getExcepcion()
   {
      return excepcion;
   }
   
   public void setExcepcion(String excepcion)
   {
      this.excepcion = excepcion;
   }
   
   public String getPeriodo()
   {
      return periodo;
   }
   
   public void setPeriodo(String periodo)
   {
      this.periodo = periodo;
   }
   
   public double getPrecio()
   {
      return precio;
   }
   
   public void setPrecio(double precio)
   {
      this.precio = precio;
   }
}