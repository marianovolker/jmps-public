package com.jmps.android.pos.sqlite;

import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.content.ContentValues;

public class GenericTable
{
   protected int id;
   protected Date fechaEliminacion;
   protected String orderBy;
   protected String table;
   protected ContentValues values;
   
   public GenericTable()
   {
      this.id = -1;
      this.fechaEliminacion = null;
      this.orderBy = null;
      this.table = null;
      this.values = new ContentValues();
   }
   
   public ContentValues getContentValues(boolean isNew)
   {
      if (!isNew)
         values.put(DbHelperConsts.KEY_ID, id);
      
      if (fechaEliminacion != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
         String s = formatter.format(fechaEliminacion);
         
         values.put(DbHelperConsts.KEY_FECHA_ELIMINACION, s);
      }
      return values;
   }
   
   // Getters and setters.
   public String getOrderBy()
   {
      return orderBy;
   }
   
   public String getTable()
   {
      return table;
   }
   
   public int getId()
   {
      return id;
   }
   
   public void setId(int id)
   {
      this.id = id;
   }
   
   public Date getFechaEliminacion()
   {
      return fechaEliminacion;
   }
   
   public void setFechaEliminacion(Date fechaEliminacion)
   {
      this.fechaEliminacion = fechaEliminacion;
   }
}
