package com.gilbarco.globaltools.flexpay.rkl.rklnativeprotocol;

import java.util.Map;

import com.gilbarco.globaltools.flexpay.rkl.rklexception.*;

public class RKLNativeMessagesHelper
{
   // ******************************************************************
   // STATIC FIELDS
   // ******************************************************************

   // ******************************************************************
   // STATIC METHODS
   // ******************************************************************
   @SuppressWarnings("unchecked")
	public static Object getParameter(Map<String,Object> data , String param) throws RklException
   {
      Map<String, Object> map = (Map<String, Object>) data.get(data.keySet().iterator().next());
      Map<String, Object> info = null;
      for (String s : map.keySet())
      {
        if (s.split("-")[0].toLowerCase().equals(param.toLowerCase()))
          {
             info = (Map<String, Object>) map.get(s);
             for (String s1 : info.keySet())
             {
                if(s1.toLowerCase().equals(param.toLowerCase()))
                   return info.get(s1);
             }
          }
      }
      throw new RklException("parameter "+ param + " was not found.");
   }
      
   // ******************************************************************
   // PUBLIC FIELDS.
   // ******************************************************************

   // ******************************************************************
   // PROTECTED FIELDS.
   // ******************************************************************

   // ******************************************************************
   // PRIVATE FIELDS.
   // ******************************************************************

   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************
   private RKLNativeMessagesHelper(){}

   // ******************************************************************
   // PUBLIC METHODS       (general, getter, setter, interface imp)
   // ******************************************************************

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
