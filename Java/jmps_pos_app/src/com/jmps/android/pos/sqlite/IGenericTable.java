package com.jmps.android.pos.sqlite;

import java.util.Date;

import android.content.ContentValues;
import android.database.Cursor;

public interface IGenericTable
{
   public abstract IGenericTable extractItemFromCursor(Cursor cursor);
   
   public abstract IGenericTable[] extractArrayFromCursor(Cursor cursor);
   
   public abstract ContentValues getContentValues(boolean isNew);
   
   public abstract String getWhere();
   
   // Getters and setters.
   public abstract int getId();
   
   public abstract Date getFechaEliminacion();
   
   public abstract String getOrderBy();
   
   public abstract String getTable();
}
