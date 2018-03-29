package com.gilbarco.globaltools.flexpay.rkl.rklnativeprotocol;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import com.gilbarco.globaltools.edt.util.Validator;
import com.gilbarco.globaltools.flexpay.rkl.rklexception.RklException;

public class RKLNativeValidator implements Validator {
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final String RC_OK = "0x0000";
	private static final String RSA_CODE = "0x0FFF";
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

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	@Override
	public boolean supports(Class<?> arg0) {
		return HashMap.class.equals(arg0);
	}

	@SuppressWarnings("unchecked")
   @Override
	public void validate(Object obj, Collection<Throwable> errors) {

	   if(!supports(obj.getClass()))
	   {
	      errors.add(new Throwable(obj.getClass().getName() + " is not supported"));
	   }
	   String rc;
      try
      {
         rc = (String) RKLNativeMessagesHelper.getParameter((Map<String, Object>) obj, "RC");
         if (!RC_OK.equals(rc) && !RSA_CODE.equals(rc) ) {
            errors.add(new Throwable("RC reported the error: " + rc));
         }
      }
      catch (RklException e)
      {
         errors.add(new Throwable("RC is not a valid parameter."));
      }
      
	   
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
