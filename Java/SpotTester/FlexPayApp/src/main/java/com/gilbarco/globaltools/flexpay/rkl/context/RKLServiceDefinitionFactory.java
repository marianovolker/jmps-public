package com.gilbarco.globaltools.flexpay.rkl.context;

import com.gilbarco.globaltools.edt.services.EdtServiceDefinition;

public final class RKLServiceDefinitionFactory
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
   private static EdtServiceDefinition timerDefinition;
   private static EdtServiceDefinition mgpRKLDefinition;
   private static EdtServiceDefinition processDefinition;
   private static EdtServiceDefinition cryptoDefinition;
   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************
   
   // ******************************************************************
   // PUBLIC METHODS       (general, getter, setter, interface imp)
   // ******************************************************************
   public void setTimerDefinition(EdtServiceDefinition timerDefinition)
   {
      RKLServiceDefinitionFactory.timerDefinition = timerDefinition;
   }
   public static EdtServiceDefinition getTimerDefinition()
   {
      return timerDefinition;
   }
   public void setMgpRKLDefinition(EdtServiceDefinition mgpRKLDefinition)
   {
      RKLServiceDefinitionFactory.mgpRKLDefinition = mgpRKLDefinition;
   }
   public static EdtServiceDefinition getRKLMgpDefinition()
   {
      return mgpRKLDefinition;
   }
   public void setProcessDefinition(EdtServiceDefinition processDefinition)
   {
      RKLServiceDefinitionFactory.processDefinition = processDefinition;
   }
   public static EdtServiceDefinition getProcessDefinition()
   {
      return processDefinition;
   }
   public void setCryptoDefinition(EdtServiceDefinition cryptoDefinition)
   {
      RKLServiceDefinitionFactory.cryptoDefinition = cryptoDefinition;
   }
   public static EdtServiceDefinition getCryptoDefinition()
   {
      return cryptoDefinition;
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
