package com.gilbarco.globaltools.gcm.util;

import java.io.File;
import java.io.FileReader;

import org.junit.Assert;
import org.junit.Test;

public class OTIParserTest
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

   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************

   // ******************************************************************
   // PUBLIC METHODS (general, getter, setter, interface imp)
   // ******************************************************************
   @Test
   public void emvTagTest()
   {
      try
      {
         parseTest(true,new File("TestOtiWithTag.ini"));
      }
      catch (Exception e)
      {
         Assert.fail(e.getMessage());
      }
   }

   @Test
   public void iniFile()
   {
      @SuppressWarnings("unused")
      OTIParser parser = null;
      try
      {
         parser = new OTIParser(new File("TestOti.txt"));
         Assert.fail("Fail");
      }
      catch (Exception e)
      {
         System.out.println(e.getMessage());
         Assert.assertTrue(true);
      }
   }

   @Test
   public void noLoadTag()
   {
      try
      {
         parseTest(false,new File("TestOtiNoTag.ini"));
         Assert.fail("Fail");
      }
      catch (Exception e)
      {
         System.out.println(e.getMessage());
         Assert.assertTrue(true);

      }
   }

   // ******************************************************************
   // PROTECTED METHODS.
   // ******************************************************************

   // ******************************************************************
   // PRIVATE METHODS.
   // ******************************************************************
   private void parseTest(boolean loadTag,File file) throws Exception
   {
      String tag = "57125413339000001513D12122010000000000004F07A00000000410109F34031F03009A031001119F270180950"
                   + "504000000009B0208009F260827334FD3B01AA2D89F10120110A00003240000000000000000000000FF5A08541"
                   + "33390000015139F02060000000012348407A00000000410109F3602002F9F3704710B4AE88E0A0000000000000" + "0001F0382025980";
      OTIParser parser = null;
      parser = new OTIParser(file);
      if(loadTag)
         parser.loadTags(new FileReader(new File("src/test/resources/OTITags.conf")));
      parser.parseAndDumpToFile(tag);
      
   }
   // ******************************************************************
   // INNER CLASSES.
   // ******************************************************************

   // ******************************************************************
   // MAIN.
   // ******************************************************************

}
