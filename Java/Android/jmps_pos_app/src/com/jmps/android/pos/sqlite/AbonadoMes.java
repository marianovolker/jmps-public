package com.jmps.android.pos.sqlite;

import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.content.ContentValues;
import android.database.Cursor;

public class AbonadoMes extends GenericTable implements IGenericTable
{
   private int tarifaId;
   private String nombre;
   private String apellido;
   private String direccion;
   private Date fechaIngreso;
   private String dni;
   private String telefono1;
   private String telefono2;
   private String telefono3;
   
   public AbonadoMes()
   {
      super();
      
      orderBy = DbHelperConsts.ABONADOS_MES_APELLIDO;
      table = DbHelperConsts.TABLE_ABONADOS_MES;
      
      tarifaId = -1;
      nombre = null;
      apellido = null;
      direccion = null;
      fechaIngreso = null;
      dni = null;
      telefono1 = null;
      telefono2 = null;
      telefono3 = null;
   }
   
   public String getWhere()
   {
      String where = DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL";
      
      if (id != -1)
         where += " AND " + DbHelperConsts.KEY_ID + "=" + id;
      
      if (tarifaId != -1)
         where += " AND " + DbHelperConsts.ABONADOS_MES_TARIFA_MES_ID + "=" + tarifaId;
      
      if (nombre != null)
         where += " AND " + DbHelperConsts.ABONADOS_MES_NOMBRE + "=" + nombre;
      
      if (apellido != null)
         where += " AND " + DbHelperConsts.ABONADOS_MES_APELLIDO + "=" + apellido;
      
      if (direccion != null)
         where += " AND " + DbHelperConsts.ABONADOS_MES_DIRECCION + "=" + direccion;
      
      if (fechaIngreso != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         where += " AND " + DbHelperConsts.ABONADOS_MES_FECHA_INGRESO + "=" + formatter.format(fechaIngreso);
      }
      
      if (dni != null)
         where += " AND " + DbHelperConsts.ABONADOS_MES_DNI + "=" + dni;
      
      if (telefono1 != null)
         where += " AND " + DbHelperConsts.ABONADOS_MES_TELEFONO1 + "=" + telefono1;
      
      if (telefono2 != null)
         where += " AND " + DbHelperConsts.ABONADOS_MES_TELEFONO2 + "=" + telefono2;
      
      if (telefono3 != null)
         where += " AND " + DbHelperConsts.ABONADOS_MES_TELEFONO3 + "=" + telefono3;
      
      return where;
   }
   
   public ContentValues getContentValues(boolean isNew)
   {
      super.getContentValues(isNew);
      
      values.put(DbHelperConsts.ABONADOS_MES_TARIFA_MES_ID, tarifaId);
      values.put(DbHelperConsts.ABONADOS_MES_NOMBRE, nombre);
      values.put(DbHelperConsts.ABONADOS_MES_APELLIDO, apellido);
      values.put(DbHelperConsts.ABONADOS_MES_DIRECCION, direccion);
      values.put(DbHelperConsts.ABONADOS_MES_DNI, dni);
      values.put(DbHelperConsts.ABONADOS_MES_TELEFONO1, telefono1);
      
      if (telefono2 != null)
      {
         values.put(DbHelperConsts.ABONADOS_MES_TELEFONO2, telefono2);
      }
      
      if (telefono3 != null)
      {
         values.put(DbHelperConsts.ABONADOS_MES_TELEFONO3, telefono3);
      }
      
      if (fechaIngreso != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         String s = formatter.format(fechaIngreso);
         values.put(DbHelperConsts.ABONADOS_MES_FECHA_INGRESO, s);
      }
      
      return values;
   }
   
   public IGenericTable extractItemFromCursor(Cursor cursor)
   {
      AbonadoMes abonadoMes = new AbonadoMes();
      
      abonadoMes.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
      abonadoMes.setTarifaId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_TARIFA_MES_ID)));
      abonadoMes.setNombre(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_NOMBRE)));
      abonadoMes.setApellido(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_APELLIDO)));
      abonadoMes.setDireccion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_DIRECCION)));
      abonadoMes.setDni(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_DNI)));
      abonadoMes.setTelefono1(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_TELEFONO1)));
      abonadoMes.setTelefono2(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_TELEFONO2)));
      abonadoMes.setTelefono3(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_TELEFONO3)));
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.ABONADOS_MES_FECHA_INGRESO)));
         abonadoMes.setFechaIngreso(d);
      }
      catch (Exception ex)
      {
         abonadoMes.setFechaIngreso(null);
      }
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
         abonadoMes.setFechaEliminacion(d);
      }
      catch (Exception ex)
      {
         abonadoMes.setFechaEliminacion(null);
      }
      
      return abonadoMes;
   }
   
   public IGenericTable[] extractArrayFromCursor(Cursor cursor)
   {
      List<AbonadoMes> abonadosMes = new ArrayList<AbonadoMes>();
      
      do
      {
         AbonadoMes abonadoMes = new AbonadoMes();
         
         abonadoMes.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
         abonadoMes.setTarifaId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_TARIFA_MES_ID)));
         abonadoMes.setNombre(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_NOMBRE)));
         abonadoMes.setApellido(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_APELLIDO)));
         abonadoMes.setDireccion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_DIRECCION)));
         abonadoMes.setDni(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_DNI)));
         abonadoMes.setTelefono1(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_TELEFONO1)));
         abonadoMes.setTelefono2(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_TELEFONO2)));
         abonadoMes.setTelefono3(cursor.getString(cursor.getColumnIndex(DbHelperConsts.ABONADOS_MES_TELEFONO3)));
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.ABONADOS_MES_FECHA_INGRESO)));
            abonadoMes.setFechaIngreso(d);
         }
         catch (Exception ex)
         {
            abonadoMes.setFechaIngreso(null);
         }
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
            abonadoMes.setFechaEliminacion(d);
         }
         catch (Exception ex)
         {
            abonadoMes.setFechaEliminacion(null);
         }
         
         abonadosMes.add(abonadoMes);
      } while (cursor.moveToNext());
      
      return abonadosMes.toArray(new AbonadoMes[abonadosMes.size()]);
   }
   
   public String getNombreCompleto()
   {
      return apellido + " " + nombre;
   }
   
   // Getters and setters.
   public int getTarifaId()
   {
      return tarifaId;
   }
   
   public void setTarifaId(int tarifaId)
   {
      this.tarifaId = tarifaId;
   }
   
   public String getNombre()
   {
      return nombre;
   }
   
   public void setNombre(String nombre)
   {
      this.nombre = nombre;
   }
   
   public String getApellido()
   {
      return apellido;
   }
   
   public void setApellido(String apellido)
   {
      this.apellido = apellido;
   }
   
   public String getDireccion()
   {
      return direccion;
   }
   
   public void setDireccion(String direccion)
   {
      this.direccion = direccion;
   }
   
   public Date getFechaIngreso()
   {
      return fechaIngreso;
   }
   
   public void setFechaIngreso(Date fechaIngreso)
   {
      this.fechaIngreso = fechaIngreso;
   }
   
   public String getDni()
   {
      return dni;
   }
   
   public void setDni(String dni)
   {
      this.dni = dni;
   }
   
   public String getTelefono1()
   {
      return telefono1;
   }
   
   public void setTelefono1(String telefono1)
   {
      this.telefono1 = telefono1;
   }
   
   public String getTelefono2()
   {
      return telefono2;
   }
   
   public void setTelefono2(String telefono2)
   {
      this.telefono2 = telefono2;
   }
   
   public String getTelefono3()
   {
      return telefono3;
   }
   
   public void setTelefono3(String telefono3)
   {
      this.telefono3 = telefono3;
   }
}