package com.jmps.android.pos.sqlite;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.content.ContentValues;
import android.database.Cursor;

public class AutoMes extends GenericTable implements IGenericTable
{
   private int abonadoId;
   private String marca;
   private String modelo;
   private String color;
   private String patente;
   private String nroMotor;
   private String turno;
   
   public AutoMes()
   {
      super();
      orderBy = DbHelperConsts.AUTOS_MES_MARCA;
      table = DbHelperConsts.TABLE_AUTOS_MES;
   }
   
   public String getWhere()
   {
      String where = DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL";
      
      if (id != -1)
         where += " AND " + DbHelperConsts.KEY_ID + "=" + id;
      
      if (abonadoId != -1)
         where += " AND " + DbHelperConsts.AUTOS_MES_ABONADO_MES_ID + "=" + abonadoId;
      
      if (marca != null)
         where += " AND " + DbHelperConsts.AUTOS_MES_MARCA + "=" + marca;
      
      if (modelo != null)
         where += " AND " + DbHelperConsts.AUTOS_MES_MODELO + "=" + modelo;
      
      if (color != null)
         where += " AND " + DbHelperConsts.AUTOS_MES_COLOR + "=" + color;
      
      if (patente != null)
         where += " AND " + DbHelperConsts.AUTOS_MES_PATENTE + "=" + patente;
      
      if (nroMotor != null)
         where += " AND " + DbHelperConsts.AUTOS_MES_NRO_MOTOR + "=" + nroMotor;
      
      if (turno != null)
         where += " AND " + DbHelperConsts.AUTOS_MES_TURNO + "=" + turno;
      
      return where;
   }
   
   public ContentValues getContentValues(boolean isNew)
   {
      super.getContentValues(isNew);
      
      values.put(DbHelperConsts.AUTOS_MES_ABONADO_MES_ID, abonadoId);
      values.put(DbHelperConsts.AUTOS_MES_MARCA, marca);
      values.put(DbHelperConsts.AUTOS_MES_MODELO, modelo);
      values.put(DbHelperConsts.AUTOS_MES_COLOR, color);
      values.put(DbHelperConsts.AUTOS_MES_PATENTE, patente);
      values.put(DbHelperConsts.AUTOS_MES_NRO_MOTOR, nroMotor);
      values.put(DbHelperConsts.AUTOS_MES_TURNO, turno);
      
      return values;
   }
   
   public IGenericTable extractItemFromCursor(Cursor cursor)
   {
      AutoMes autoMes = new AutoMes();
      
      autoMes.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
      autoMes.setAbonadoId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_ABONADO_MES_ID)));
      autoMes.setMarca(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_MARCA)));
      autoMes.setModelo(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_MODELO)));
      autoMes.setColor(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_COLOR)));
      autoMes.setPatente(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_PATENTE)));
      autoMes.setNroMotor(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_NRO_MOTOR)));
      autoMes.setTurno(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_TURNO)));
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
         autoMes.setFechaEliminacion(d);
      }
      catch (Exception ex)
      {
         autoMes.setFechaEliminacion(null);
      }
      
      return autoMes;
   }
   
   public IGenericTable[] extractArrayFromCursor(Cursor cursor)
   {
      List<AutoMes> autosMes = new ArrayList<AutoMes>();
      
      do
      {
         AutoMes autoMes = new AutoMes();
         
         autoMes.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
         autoMes.setAbonadoId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_ABONADO_MES_ID)));
         autoMes.setMarca(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_MARCA)));
         autoMes.setModelo(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_MODELO)));
         autoMes.setColor(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_COLOR)));
         autoMes.setPatente(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_PATENTE)));
         autoMes.setNroMotor(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_NRO_MOTOR)));
         autoMes.setTurno(cursor.getString(cursor.getColumnIndex(DbHelperConsts.AUTOS_MES_TURNO)));
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
            autoMes.setFechaEliminacion(d);
         }
         catch (Exception ex)
         {
            autoMes.setFechaEliminacion(null);
         }
         
         autosMes.add(autoMes);
      } while (cursor.moveToNext());
      
      return autosMes.toArray(new AutoMes[autosMes.size()]);
   }
   
   // Getters and setters.
   public int getAbonadoId()
   {
      return abonadoId;
   }
   
   public void setAbonadoId(int abonadoId)
   {
      this.abonadoId = abonadoId;
   }
   
   public String getMarca()
   {
      return marca;
   }
   
   public void setMarca(String marca)
   {
      this.marca = marca;
   }
   
   public String getModelo()
   {
      return modelo;
   }
   
   public void setModelo(String modelo)
   {
      this.modelo = modelo;
   }
   
   public String getColor()
   {
      return color;
   }
   
   public void setColor(String color)
   {
      this.color = color;
   }
   
   public String getPatente()
   {
      return patente;
   }
   
   public void setPatente(String patente)
   {
      this.patente = patente;
   }
   
   public String getNroMotor()
   {
      return nroMotor;
   }
   
   public void setNroMotor(String nroMotor)
   {
      this.nroMotor = nroMotor;
   }
   
   public String getTurno()
   {
      return turno;
   }
   
   public void setTurno(String turno)
   {
      this.turno = turno;
   }
}