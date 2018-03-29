package com.gilbarco.globaltools.spottester.model;

import java.util.HashSet;
import java.util.Set;

public class MessageFilter<S>
{

   // ******************************************************************
   // STATIC FIELDS
   // ******************************************************************

   // ******************************************************************
   // STATIC METHODS
   // ******************************************************************

   // ******************************************************************
   // PUBLIC FIELDS.
   // ******************************************************************

   // ******************************************************************
   // PROTECTED FIELDS.
   // ******************************************************************

   // ******************************************************************
   // PRIVATE FIELDS.
   // ******************************************************************
   private Set<S> filter;

   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************
   public MessageFilter()
   {
      super();
      filter = new HashSet<S>();
   }

   // ******************************************************************
   // PUBLIC METHODS (general, getter, setter, interface imp)
   // ******************************************************************
   public boolean contains(S input)
   {
      return filter.contains(input);
   }

   public boolean add(S input)
   {
      return filter.add(input);
   }

   public Set<S> getFilter()
   {
      return this.filter;
   }

   // ******************************************************************
   // PROTECTED METHODS.
   // ******************************************************************

   // ******************************************************************
   // PRIVATE METHODS.
   // ******************************************************************

   // ******************************************************************
   // INNER CLASSES.
   // ******************************************************************

   // ******************************************************************
   // MAIN.
   // ******************************************************************

}
