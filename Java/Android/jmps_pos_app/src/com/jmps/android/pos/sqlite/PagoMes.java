package com.jmps.android.pos.sqlite;

import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.content.ContentValues;
import android.database.Cursor;

public class PagoMes extends GenericTable implements IGenericTable
{
   private int tarifaId;
   private int abonadoId;
   private double precio;
   private String descripcion;
   private Date fechaPago;
   private Date fechaDesde;
   private Date fechaHasta;
   
   public PagoMes()
   {
      super();
      
      orderBy = DbHelperConsts.PAGOS_MES_FECHA_DESDE;
      table = DbHelperConsts.TABLE_PAGOS_MES;
      
      tarifaId = -1;
      abonadoId = -1;
      precio = -1;
      descripcion = null;
      fechaPago = null;
      fechaDesde = null;
      fechaHasta = null;
   }
   
   public String getWhere()
   {
      String where = DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL";
      
      if (id != -1)
         where += " AND " + DbHelperConsts.KEY_ID + "=" + id;
      
      if (tarifaId != -1)
         where += " AND " + DbHelperConsts.PAGOS_MES_TARIFA_MES_ID + "=" + tarifaId;
      
      if (abonadoId != -1)
         where += " AND " + DbHelperConsts.PAGOS_MES_ABONADO_MES_ID + "=" + abonadoId;
      
      if (precio != -1)
         where += " AND " + DbHelperConsts.PAGOS_MES_PRECIO + "=" + precio;
      
      if (descripcion != null)
         where += " AND " + DbHelperConsts.PAGOS_MES_DESCRIPCION + "=" + descripcion;
      
      if (fechaPago != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         where += " AND " + DbHelperConsts.PAGOS_MES_FECHA_PAGO + "=" + formatter.format(fechaPago);
      }
      
      if (fechaDesde != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         where += " AND " + DbHelperConsts.PAGOS_MES_FECHA_DESDE + "=" + formatter.format(fechaDesde);
      }
      
      if (fechaHasta != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         where += " AND " + DbHelperConsts.PAGOS_MES_FECHA_HASTA + "=" + formatter.format(fechaHasta);
      }
      
      return where;
   }
   
   public ContentValues getContentValues(boolean isNew)
   {
      super.getContentValues(isNew);
      
      values.put(DbHelperConsts.PAGOS_MES_TARIFA_MES_ID, tarifaId);
      values.put(DbHelperConsts.PAGOS_MES_ABONADO_MES_ID, abonadoId);
      values.put(DbHelperConsts.PAGOS_MES_PRECIO, precio);
      values.put(DbHelperConsts.PAGOS_MES_DESCRIPCION, descripcion);
      
      if (fechaPago != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         String s = formatter.format(fechaPago);
         
         values.put(DbHelperConsts.PAGOS_MES_FECHA_PAGO, s);
      }
      
      if (fechaDesde != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         String s = formatter.format(fechaDesde);
         
         values.put(DbHelperConsts.PAGOS_MES_FECHA_DESDE, s);
      }
      
      if (fechaHasta != null)
      {
         Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
         String s = formatter.format(fechaHasta);
         
         values.put(DbHelperConsts.PAGOS_MES_FECHA_HASTA, s);
      }
      
      return values;
   }
   
   public IGenericTable extractItemFromCursor(Cursor cursor)
   {
      PagoMes pagoMes = new PagoMes();
      
      pagoMes.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
      pagoMes.setAbonadoId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.PAGOS_MES_ABONADO_MES_ID)));
      pagoMes.setTarifaId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.PAGOS_MES_TARIFA_MES_ID)));
      pagoMes.setPrecio(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.PAGOS_MES_PRECIO)));
      pagoMes.setDescripcion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.PAGOS_MES_DESCRIPCION)));
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.PAGOS_MES_FECHA_PAGO)));
         pagoMes.setFechaPago(d);
      }
      catch (Exception ex)
      {
         pagoMes.setFechaPago(null);
      }
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.PAGOS_MES_FECHA_DESDE)));
         pagoMes.setFechaDesde(d);
      }
      catch (Exception ex)
      {
         pagoMes.setFechaDesde(null);
      }
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.PAGOS_MES_FECHA_HASTA)));
         pagoMes.setFechaHasta(d);
      }
      catch (Exception ex)
      {
         pagoMes.setFechaHasta(null);
      }
      
      try
      {
         Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                  .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
         pagoMes.setFechaEliminacion(d);
      }
      catch (Exception ex)
      {
         pagoMes.setFechaEliminacion(null);
      }
      
      return pagoMes;
   }
   
   public IGenericTable[] extractArrayFromCursor(Cursor cursor)
   {
      List<PagoMes> pagosMes = new ArrayList<PagoMes>();
      
      do
      {
         PagoMes pagoMes = new PagoMes();
         
         pagoMes.setId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID)));
         pagoMes.setAbonadoId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.PAGOS_MES_ABONADO_MES_ID)));
         pagoMes.setTarifaId(cursor.getInt(cursor.getColumnIndex(DbHelperConsts.PAGOS_MES_TARIFA_MES_ID)));
         pagoMes.setPrecio(cursor.getDouble(cursor.getColumnIndex(DbHelperConsts.PAGOS_MES_PRECIO)));
         pagoMes.setDescripcion(cursor.getString(cursor.getColumnIndex(DbHelperConsts.PAGOS_MES_DESCRIPCION)));
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.PAGOS_MES_FECHA_PAGO)));
            pagoMes.setFechaPago(d);
         }
         catch (Exception ex)
         {
            pagoMes.setFechaPago(null);
         }
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.PAGOS_MES_FECHA_DESDE)));
            pagoMes.setFechaDesde(d);
         }
         catch (Exception ex)
         {
            pagoMes.setFechaDesde(null);
         }
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.PAGOS_MES_FECHA_HASTA)));
            pagoMes.setFechaHasta(d);
         }
         catch (Exception ex)
         {
            pagoMes.setFechaHasta(null);
         }
         
         try
         {
            Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).parse(cursor.getString(cursor
                     .getColumnIndex(DbHelperConsts.KEY_FECHA_ELIMINACION)));
            pagoMes.setFechaEliminacion(d);
         }
         catch (Exception ex)
         {
            pagoMes.setFechaEliminacion(null);
         }
         
         pagosMes.add(pagoMes);
      } while (cursor.moveToNext());
      
      return pagosMes.toArray(new PagoMes[pagosMes.size()]);
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
   
   public int getAbonadoId()
   {
      return abonadoId;
   }
   
   public void setAbonadoId(int abonadoId)
   {
      this.abonadoId = abonadoId;
   }
   
   public double getPrecio()
   {
      return precio;
   }
   
   public void setPrecio(double precio)
   {
      this.precio = precio;
   }
   
   public String getDescripcion()
   {
      return descripcion;
   }
   
   public void setDescripcion(String descripcion)
   {
      this.descripcion = descripcion;
   }
   
   public Date getFechaPago()
   {
      return fechaPago;
   }
   
   public void setFechaPago(Date fechaPago)
   {
      this.fechaPago = fechaPago;
   }
   
   public Date getFechaDesde()
   {
      return fechaDesde;
   }
   
   public void setFechaDesde(Date fechaDesde)
   {
      this.fechaDesde = fechaDesde;
   }
   
   public Date getFechaHasta()
   {
      return fechaHasta;
   }
   
   public void setFechaHasta(Date fechaHasta)
   {
      this.fechaHasta = fechaHasta;
   }
}