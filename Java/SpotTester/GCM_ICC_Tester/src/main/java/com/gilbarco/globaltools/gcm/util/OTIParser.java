package com.gilbarco.globaltools.gcm.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

public class OTIParser {

   // ******************************************************************
   // STATIC FIELDS
   // ******************************************************************
   private static final File   DEFAULT_FILE_NAME = new File("OTI.ini");
   private static final int    DOUBLE_TAG        = 4;                  // 2 byte
   private static final int    SINGLE_TAG        = 2;                  // 1 byte
   private static final int    LENGTH            = 2;                  // 1 byte
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

   private Map<String, String> OTITag            = null;
   private FileWriter          writer            = null;
   private int                 index             = 0;

   // ******************************************************************
   // CONSTRUCTOR.
   // ******************************************************************

	public OTIParser() throws Exception {
      this(DEFAULT_FILE_NAME);
   }

	public OTIParser(File file) throws Exception {
      super();
      if (!checkFileName(file))
         throw new Exception(file.getName() + " is not an ini file");
      OTITag = new HashMap<String, String>();
      writer = new FileWriter(file);
   }

	public void loadTags(InputStreamReader streamReader) throws IOException {
      BufferedReader reader = new BufferedReader(streamReader);
      String line = null;
		while ((line = reader.readLine()) != null) {
         if(line.startsWith("#"))
            continue;
         OTITag.put(line.split(":")[0], line.split(":")[1]);
      }
   }

	public void parseAndDumpToFile(String tag) throws Exception {
      if(OTITag.isEmpty())
         throw new Exception("OTI tags should be loaded first.");
      
		try {
			while (index < tag.length()) {
				if (OTITag.containsKey(tag.substring(index, index + DOUBLE_TAG))) {
               dump(tag, DOUBLE_TAG);
            }
				else if (OTITag.containsKey(tag.substring(index, index + SINGLE_TAG))) {
               dump(tag, SINGLE_TAG);
            }
				else {
               throw new Exception("EMV tag are not well formatted. Tag : " + tag.substring(index, index + DOUBLE_TAG));
            }
         }
      }
		catch (IOException e) {
         throw new Exception("EMV tag output file has an error.");
      }
		finally {
         if (writer != null)
            writer.close();
      }
   }

   // ******************************************************************
   // PUBLIC METHODS (general, getter, setter, interface imp)
   // ******************************************************************

   // ******************************************************************
   // PROTECTED METHODS.
   // ******************************************************************

   // ******************************************************************
   // PRIVATE METHODS.
   // ******************************************************************

	private void dump(String tag, int tagType) throws IOException {
      int length = 0;
      writer.write("[" + OTITag.get(tag.substring(index, index + tagType)) + "]\n");
      writer.write("Tag : " + tag.substring(index, index + tagType) + "\n");
      index = index + tagType;
      length = Integer.valueOf(tag.substring(index, index + LENGTH), 16);
      index = index + LENGTH;
      writer.write("Length : " + length + "\n");
      writer.write("Value: " + tag.substring(index, index + length * 2) + "\n\n");
      index = index + length * 2;
   }

	private boolean checkFileName(File file) {
      return file.getName().endsWith(".ini") ? true : false;
   }

   // ******************************************************************
   // INNER CLASSES.
   // ******************************************************************

   // ******************************************************************
   // MAIN.
   // ******************************************************************

}
