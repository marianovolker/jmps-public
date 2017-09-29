package com.jmps.android.pos.sqlite;

import java.util.ArrayList;
import java.util.List;

import android.content.ContentValues;
import android.database.Cursor;

public class CategoriaDia extends GenericTable implements IGenericTable
{
   private String nombre;
   
   public CategoriaDia()
   {
      super();
      
      orderBy = DbHelperConsts.CATEGORIAS_DIA_NOMBRE;
      table = DbHelperConsts.TABLE_CATEGORIAS_DIA;
      
      nombre = null;
   }
   
   @Override
   public IGenericTable extractItemFromCursor(Cursor cursor)
   {
      CategoriaDia categoriaDia = new CategoriaDia();
      
      categoriaDia.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
      categoriaDia.setNombre(cursor.getString(cursor.getColumnIndex(DbHelperConsts.CATEGORIAS_DIA_NOMBRE)));
      
      return categoriaDia;
   }
   
   @Override
   public IGenericTable[] extractArrayFromCursor(Cursor cursor)
   {
      List<CategoriaDia> categoriasDia = new ArrayList<CategoriaDia>();
      
      do
      {
         CategoriaDia categoriaDia = new CategoriaDia();
         
         categoriaDia.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
         categoriaDia.setNombre(cursor.getString(cursor.getColumnIndex(DbHelperConsts.CATEGORIAS_DIA_NOMBRE)));
         
         categoriasDia.add(categoriaDia);
      } while (cursor.moveToNext());
      
      return categoriasDia.toArray(new CategoriaDia[categoriasDia.size()]);
   }
   
   @Override
   public ContentValues getContentValues(boolean isNew)
   {
      super.getContentValues(isNew);
      
      values.put(DbHelperConsts.CATEGORIAS_DIA_NOMBRE, nombre);
      
      return values;
   }
   
   @Override
   public String getWhere()
   {
      String where = DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL";
      
      if (id != -1)
         where += " AND " + DbHelperConsts.KEY_ID + "=" + id;
      
      if (nombre != null)
         where += " AND " + DbHelperConsts.TARIFAS_MES_NOMBRE + "=" + nombre;
      
      return where;
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
}
