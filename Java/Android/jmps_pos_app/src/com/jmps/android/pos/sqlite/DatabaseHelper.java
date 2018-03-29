package com.jmps.android.pos.sqlite;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class DatabaseHelper extends SQLiteOpenHelper
{
   public DatabaseHelper(Context context)
   {
      super(context, DbHelperConsts.DATABASE_NAME, null, DbHelperConsts.DATABASE_VERSION);
   }
   
   @Override
   public void onCreate(SQLiteDatabase db)
   {
      // Tables for "Abonados Mensuales".
      db.execSQL("CREATE TABLE " + DbHelperConsts.TABLE_TARIFAS_MES + "("
                 + DbHelperConsts.KEY_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                 + DbHelperConsts.TARIFAS_MES_NOMBRE + " TEXT NOT NULL, "
                 + DbHelperConsts.TARIFAS_MES_DESCRIPCION + " TEXT NOT NULL, "
                 + DbHelperConsts.TARIFAS_MES_EXCEPCION + " TEXT NOT NULL, "
                 + DbHelperConsts.TARIFAS_MES_PERIODO + " TEXT NOT NULL, "
                 + DbHelperConsts.TARIFAS_MES_COSTO + " REAL NOT NULL, "
                 + DbHelperConsts.KEY_FECHA_ELIMINACION + " TEXT"
                 + ");");
      
      db.execSQL("CREATE TABLE " + DbHelperConsts.TABLE_ABONADOS_MES + "("
                 + DbHelperConsts.KEY_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                 + DbHelperConsts.ABONADOS_MES_TARIFA_MES_ID + " INTEGER NOT NULL, "
                 + DbHelperConsts.ABONADOS_MES_NOMBRE + " TEXT NOT NULL, "
                 + DbHelperConsts.ABONADOS_MES_APELLIDO + " TEXT NOT NULL, "
                 + DbHelperConsts.ABONADOS_MES_DIRECCION + " TEXT NOT NULL, "
                 + DbHelperConsts.ABONADOS_MES_FECHA_INGRESO + " TEXT NOT NULL, "
                 + DbHelperConsts.ABONADOS_MES_DNI + " TEXT NOT NULL, "
                 + DbHelperConsts.ABONADOS_MES_TELEFONO1 + " TEXT NOT NULL, "
                 + DbHelperConsts.ABONADOS_MES_TELEFONO2 + " TEXT, "
                 + DbHelperConsts.ABONADOS_MES_TELEFONO3 + " TEXT, "
                 + DbHelperConsts.KEY_FECHA_ELIMINACION + " TEXT, "
                 + "FOREIGN KEY(" + DbHelperConsts.ABONADOS_MES_TARIFA_MES_ID + ") REFERENCES " + DbHelperConsts.TABLE_TARIFAS_MES + "(" + DbHelperConsts.KEY_ID + ")"
                 + ");");
      
      db.execSQL("CREATE TABLE " + DbHelperConsts.TABLE_AUTOS_MES + "("
                 + DbHelperConsts.KEY_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                 + DbHelperConsts.AUTOS_MES_ABONADO_MES_ID + " INTEGER NOT NULL, "
                 + DbHelperConsts.AUTOS_MES_MARCA + " TEXT NOT NULL, "
                 + DbHelperConsts.AUTOS_MES_MODELO + " TEXT NOT NULL, "
                 + DbHelperConsts.AUTOS_MES_COLOR + " TEXT NOT NULL, "
                 + DbHelperConsts.AUTOS_MES_PATENTE + " TEXT NOT NULL, "
                 + DbHelperConsts.AUTOS_MES_NRO_MOTOR + " TEXT NOT NULL, "
                 + DbHelperConsts.AUTOS_MES_TURNO + " TEXT NOT NULL, "
                 + DbHelperConsts.KEY_FECHA_ELIMINACION + " TEXT, "
                 + "FOREIGN KEY(" + DbHelperConsts.AUTOS_MES_ABONADO_MES_ID + ") REFERENCES " + DbHelperConsts.TABLE_ABONADOS_MES + "(" + DbHelperConsts.KEY_ID + ")"
                 + ");");
      
      db.execSQL("CREATE TABLE " + DbHelperConsts.TABLE_PAGOS_MES + "("
                 + DbHelperConsts.KEY_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                 + DbHelperConsts.PAGOS_MES_ABONADO_MES_ID + " INTEGER NOT NULL, "
                 + DbHelperConsts.PAGOS_MES_TARIFA_MES_ID + " INTEGER NOT NULL, "
                 + DbHelperConsts.PAGOS_MES_PRECIO + " REAL NOT NULL,"
                 + DbHelperConsts.PAGOS_MES_DESCRIPCION + " TEXT, "
                 + DbHelperConsts.PAGOS_MES_FECHA_PAGO + " TEXT NOT NULL, "
                 + DbHelperConsts.PAGOS_MES_FECHA_DESDE + " TEXT NOT NULL, "
                 + DbHelperConsts.PAGOS_MES_FECHA_HASTA + " TEXT NOT NULL, "
                 + DbHelperConsts.KEY_FECHA_ELIMINACION + " TEXT, "
                 + "FOREIGN KEY(" + DbHelperConsts.PAGOS_MES_ABONADO_MES_ID + ") REFERENCES " + DbHelperConsts.TABLE_ABONADOS_MES + "(" + DbHelperConsts.KEY_ID + "), "
                 + "FOREIGN KEY(" + DbHelperConsts.PAGOS_MES_TARIFA_MES_ID + ") REFERENCES " + DbHelperConsts.TABLE_TARIFAS_MES + "(" + DbHelperConsts.KEY_ID + ")"
                 + ");");
      
      // Tables for "Abonados diarios".
      db.execSQL("CREATE TABLE " + DbHelperConsts.TABLE_CATEGORIAS_DIA + "("
                 + DbHelperConsts.KEY_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                 + DbHelperConsts.CATEGORIAS_DIA_NOMBRE + " TEXT NOT NULL, "
                 + DbHelperConsts.KEY_FECHA_ELIMINACION + " TEXT"
                 + ");");
      
      db.execSQL("CREATE TABLE " + DbHelperConsts.TABLE_TARIFAS_DIA + "("
                 + DbHelperConsts.KEY_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                 + DbHelperConsts.TARIFAS_DIA_CATEGORIA_DIA_ID + " INTEGER NOT NULL, "
                 + DbHelperConsts.TARIFAS_DIA_NOMBRE + " TEXT NOT NULL, "
                 + DbHelperConsts.TARIFAS_DIA_DESCRIPCION + " TEXT,"
                 + DbHelperConsts.TARIFAS_DIA_TIEMPO + " INTEGER NOT NULL, "
                 + DbHelperConsts.TARIFAS_DIA_TOLERANCIA + " INTEGER NOT NULL, "
                 + DbHelperConsts.TARIFAS_DIA_PRECIO + " REAL NOT NULL, "
                 + DbHelperConsts.KEY_FECHA_ELIMINACION + " TEXT, "
                 + "FOREIGN KEY(" + DbHelperConsts.TARIFAS_DIA_CATEGORIA_DIA_ID + ") REFERENCES " + DbHelperConsts.TABLE_CATEGORIAS_DIA + "(" + DbHelperConsts.KEY_ID + ")"
                 + ");");
      
      db.execSQL("CREATE TABLE " + DbHelperConsts.TABLE_VEHICULOS_DIA + "("
                 + DbHelperConsts.KEY_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                 + DbHelperConsts.VEHICULOS_DIA_CATEGORIA_DIA_ID + " INTEGER NOT NULL, "
                 + DbHelperConsts.VEHICULOS_DIA_PATENTE_LETRAS + " TEXT NOT NULL, "
                 + DbHelperConsts.VEHICULOS_DIA_PATENTE_NUMEROS + " TEXT NOT NULL,"
                 + DbHelperConsts.VEHICULOS_DIA_FECHA_INGRESO + " TEXT NOT NULL, "
                 + DbHelperConsts.VEHICULOS_DIA_FECHA_EGRESO + " TEXT, "
                 + DbHelperConsts.VEHICULOS_DIA_IMPORTE + " REAL, "
                 + DbHelperConsts.KEY_FECHA_ELIMINACION + " TEXT, "
                 + "FOREIGN KEY(" + DbHelperConsts.VEHICULOS_DIA_CATEGORIA_DIA_ID + ") REFERENCES " + DbHelperConsts.TABLE_CATEGORIAS_DIA + "(" + DbHelperConsts.KEY_ID + ")"
                 + ");");
      
      db.execSQL("CREATE TABLE " + DbHelperConsts.TABLE_PAGOS_DIA + "("
                 + DbHelperConsts.KEY_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                 + DbHelperConsts.PAGOS_DIA_VEHICULO_DIA_ID + " INTEGER NOT NULL, "
                 + DbHelperConsts.PAGOS_DIA_FECHA_PAGO + " TEXT NOT NULL, "
                 + DbHelperConsts.PAGOS_DIA_IMPORTE + " REAL NOT NULL,"
                 + DbHelperConsts.KEY_FECHA_ELIMINACION + " TEXT, "
                 + "FOREIGN KEY(" + DbHelperConsts.PAGOS_DIA_VEHICULO_DIA_ID + ") REFERENCES " + DbHelperConsts.TABLE_VEHICULOS_DIA + "(" + DbHelperConsts.KEY_ID + ")"
                 + ");");
   }
   
   @Override
   public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion)
   {
      //db.execSQL("DROP TABLE IF EXISTS " + DbHelperConsts.TABLE_PAGOS_MES + "");
      //db.execSQL("DROP TABLE IF EXISTS " + DbHelperConsts.TABLE_AUTOS_MES + ";");
      //db.execSQL("DROP TABLE IF EXISTS " + DbHelperConsts.TABLE_ABONADOS_MES + ";");
      //db.execSQL("DROP TABLE IF EXISTS " + DbHelperConsts.TABLE_TARIFAS_MES + ";");
      
      this.onCreate(db);
   }
   
   
   // Inserts new item in database. Returns inserted item's ID or -1 on error.
   public int insert(IGenericTable item)
   {
      SQLiteDatabase database = this.getWritableDatabase();
      
      ContentValues values = item.getContentValues(true);
      // Inserts new item and gets its ID. 
      long rowid = database.insert(item.getTable(), null, values);
      Cursor cursor = database.query(item.getTable(), new String[] { DbHelperConsts.KEY_ID }, "ROWID=" + rowid, null, null, null, null);
      
      int insertedId = -1;
      if (cursor.moveToFirst())
         insertedId = cursor.getInt(cursor.getColumnIndex(DbHelperConsts.KEY_ID));
      
      cursor.close();
      database.close();
      return insertedId;
   }
   
   
   // Gets a valid item with specified id from table. Returns null if it couldn't be found.
   public IGenericTable select(IGenericTable item)
   {
      SQLiteDatabase database = this.getReadableDatabase();
      
      Cursor cursor = database.query(item.getTable(), null, DbHelperConsts.KEY_ID + "=? AND " + DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL",
         new String[] { String.valueOf(item.getId()) }, null, null, item.getOrderBy());
      
      IGenericTable returnValue = null;
      if (cursor.moveToFirst())
         returnValue = item.extractItemFromCursor(cursor);
      
      cursor.close();
      database.close();
      return returnValue;
   }
   
   
   // Gets all valid items from table. Item is only used to know the type of object. Returns null on error.
   public IGenericTable[] selectAll(IGenericTable item)
   {
      SQLiteDatabase database = this.getReadableDatabase();
      
      Cursor cursor = database.query(item.getTable(), null, DbHelperConsts.KEY_FECHA_ELIMINACION + " IS NULL", null, null, null, item.getOrderBy());
      
      IGenericTable[] genericTables = null;
      if (cursor.moveToFirst())
         genericTables = item.extractArrayFromCursor(cursor);
      
      cursor.close();
      database.close();
      return genericTables;
   }
   
   
   // Gets all valid items that match filter's values. Returns null on error.
   public IGenericTable[] selectFilter(IGenericTable item)
   {
      SQLiteDatabase database = this.getReadableDatabase();
      
      String where = item.getWhere();
      Cursor cursor = database.query(item.getTable(), null, where, null, null, null, item.getOrderBy());
      
      IGenericTable[] genericTables = null;
      if (cursor.moveToFirst())
         genericTables = item.extractArrayFromCursor(cursor);
      
      cursor.close();
      database.close();
      return genericTables;
   }
   
   
   // Executes custom "select" SQL query. Returns cursor.
   public Cursor selectQuery(String query)
   {
      SQLiteDatabase database = this.getReadableDatabase();
      
      Cursor cursor = database.rawQuery(query, null);
      
      database.close();
      return cursor;
   }
   
   
   // Updates item on table. Returns number of affected rows.
   public long update(IGenericTable item)
   {
      SQLiteDatabase database = this.getWritableDatabase();
      
      ContentValues values = item.getContentValues(false);
      long affectedRows = database.update(item.getTable(), values,
         DbHelperConsts.KEY_ID + " = ?",
         new String[] { String.valueOf(item.getId()) });
      
      database.close();
      return affectedRows;
   }
   
   
   // Logically deletes an item on table. Returns number of affected rows.
   public long delete(IGenericTable item)
   {
      SQLiteDatabase database = this.getWritableDatabase();
      
      String timeStamp = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault()).format(Calendar.getInstance().getTime());
      ContentValues values = new ContentValues();
      values.put(DbHelperConsts.KEY_FECHA_ELIMINACION, timeStamp);
      
      long affectedRows = database.update(item.getTable(), values, DbHelperConsts.KEY_ID + "=?", new String[] { String.valueOf(item.getId()) });
      
      database.close();
      return affectedRows;
   }
}
