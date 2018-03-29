package com.gilbarco.globaltools.spotprotocol.utils;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Map;

import com.gilbarco.globaltools.edt.util.Validator;
import com.gilbarco.globaltools.mgp.exceptions.MessageValidatorException;

/**
 * @Class SpotProtocolValidator
 * 
 * @Description Validate an object and collect all the issues in the collection errors.
 * 
 * @Protocol SpotProtocol.
 * 
 * @see Validator
 * 
 * @version 1.0.0
 * 
 * @author jrubstein
 * */

public class SpotProtocolValidator implements Validator
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
   private List<String> rules = new ArrayList<String>();

   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************
   public SpotProtocolValidator()
   {
   }

   // ******************************************************************
   // PUBLIC METHODS (general, getter, setter, interface imp)
   // ******************************************************************

   @SuppressWarnings("unchecked")
   @Override
   public void validate(Object obj, Collection<Throwable> errors)
   {
      if (supports(obj.getClass()))
      {
         errors.add(new MessageValidatorException(obj.getClass() + " is not supported by this validator"));
         return;
      }
      
      Map<String, Object> data = ((Map<String, Object>) obj);
      Map<String, Object> realData = (Map<String, Object>) data.get(data.keySet().iterator().next());
      String code = ((String) realData.get("AckCode")).split("x")[1];
      for (String r : rules)
      {
         if (code.toLowerCase().trim().equals(r))
         {
            return;
         }
      }
      errors.add(new MessageValidatorException("error code: " + code));
   }

   @Override
   public boolean supports(Class<?> clazz)
   {
      return Map.class.equals(clazz);
   }

   public void setRules(List<String> rules)
   {
      this.rules = rules;
   }

   public List<String> getRules()
   {
      return rules;
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
