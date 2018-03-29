package com.jmps.android.pos.sqlite;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.content.ContentValues;
import android.database.Cursor;

public class TarifaDia extends GenericTable implements IGenericTable
{
   private int categoriaId;
   private String nombre;
   private String descripcion;
   private int tiempo;
   private int tolerancia;
   private double precio;
   
   public TarifaDia()
   {
      super();
      
      orderBy = DbHelperConsts.TARIFAS_DIA_NOMBRE;
      table = DbHelperConsts.TABLE_TARIFAS_DIA;
      
      categoriaId = -1;
      nombre = null;
      descripcion = null;
      tiempo = -1;
      tolerancia = -1;
      precio = -1;
   }
   
   @Override
   public String getWhere()
   {
      String where = DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL";
      
      if (id != -1)
         where += " AND " + DbHelperConsts.KEY_ID + "=" + id;
      
      if (categoriaId != -1)
         where += " AND " + DbHelperConsts.TARIFAS_DIA_CATEGORIA_DIA_ID + "=" + categoriaId;
      
      if (nombre != null)
         where += " AND " + DbHelperConsts.TARIFAS_DIA_NOMBRE + "=" + nombre;
      
      if (descripcion != null)
         where += " AND " + DbHelperConsts.TARIFAS_DIA_DESCRIPCION + "=" + descripcion;
      
      if (tiempo != -1)
         where += " AND " + DbHelperConsts.TARIFAS_DIA_TIEMPO + "=" + tiempo;
      
      if (tolerancia != -1)
         where += " AND " + DbHelperConsts.TARIFAS_DIA_TOLERANCIA + "=" + tolerancia;
      
      if (precio != -1)
         where += " AND " + DbHelperConsts.TARIFAS_DIA_PRECIO + "=" + precio;
      
      return where;
   }
   
   @Override
   public ContentValues getContentValues(boolean isNew)
   {
      super.getContentValues(isNew);
      
      values.put(DbHelperConsts.TARIFAS_DIA_CATEGORIA_DIA_ID, categoriaId);
      values.put(DbHelperConsts.TARIFAS_DIA_NOMBRE, nombre);
      values.put(DbHelperConsts.TARIFAS_DIA_DESCRIPCION, descripcion);
      values.put(DbHelperConsts.TARIFAS_DIA_TIEMPO, tiempo);
      values.put(DbHelperConsts.TARIFAS_DIA_TOLERANCIA, tolerancia);
      values.put(DbHelperConsts.TARIFAS_DIA_PRECIO, precio);
      
      return values;
   }
   
   @Override
   public IGenericTable extractItemFromCursor(Cursor cursor)
   {
      TarifaDia tarifaDia = new TarifaDia();
      
      tarifaDia.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
      tarifaDia.setCategoriaId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_CATEGORIA_DIA_ID)));
      tarifaDia.setNombre(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_NOMBRE)));
      tarifaDia.setDescripcion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_DESCRIPCION)));
      tarifaDia.setTiempo(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_TIEMPO)));
      tarifaDia.setTolerancia(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_TOLERANCIA)));
      tarifaDia.setPrecio(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_PRECIO)));
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
         tarifaDia.setFechaEliminacion(d);
      }
      catch (Exception ex)
      {
         tarifaDia.setFechaEliminacion(null);
      }
      
      return tarifaDia;
   }
   
   @Override
   public IGenericTable[] extractArrayFromCursor(Cursor cursor)
   {
      List<TarifaDia> tarifasDia = new ArrayList<TarifaDia>();
      
      do
      {
         TarifaDia tarifaDia = new TarifaDia();
         
         tarifaDia.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
         tarifaDia.setCategoriaId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_CATEGORIA_DIA_ID)));
         tarifaDia.setNombre(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_NOMBRE)));
         tarifaDia.setDescripcion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.TARIFAS_MES_DESCRIPCION)));
         tarifaDia.setTiempo(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_TIEMPO)));
         tarifaDia.setTolerancia(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_TOLERANCIA)));
         tarifaDia.setPrecio(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.TARIFAS_DIA_PRECIO)));
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
            tarifaDia.setFechaEliminacion(d);
         }
         catch (Exception ex)
         {
            tarifaDia.setFechaEliminacion(null);
         }
         
         tarifasDia.add(tarifaDia);
      } while (cursor.moveToNext());
      
      return tarifasDia.toArray(new TarifaDia[tarifasDia.size()]);
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
   
   public int getTiempo()
   {
      return tiempo;
   }
   
   public void setTiempo(int tiempo)
   {
      this.tiempo = tiempo;
   }
   
   public int getTolerancia()
   {
      return tolerancia;
   }
   
   public void setTolerancia(int tolerancia)
   {
      this.tolerancia = tolerancia;
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
