package com.jmps.android.pos.devices.printer;

public class PrinterCommands
{
   public static final byte[] INIT                                  = { 0x1B, 0x40 };

   public static final byte[] FEED_LINE                             = { 0x0A };
   public static final byte[] SELECT_FONT_A                         = { 0x1B, 33, 0 };
   public static final byte[] SET_BAR_CODE_HEIGHT                   = { 0x1D, 104, 100 };
   public static final byte[] PRINT_BAR_CODE_1                      = { 0x1D, 107, 2 };
   public static final byte[] SEND_NULL_BYTE                        = { 0x00 };
   public static final byte[] SELECT_PRINT_SHEET                    = { 0x1B, 0x63, 0x30, 0x02 };
   public static final byte[] FEED_PAPER_AND_CUT                    = { 0x1D, 0x56, 66, 0x00 };
   public static final byte[] SELECT_CYRILLIC_CHARACTER_CODE_TABLE  = { 0x1B, 0x74, 0x11 };
   public static final byte[] SELECT_BIT_IMAGE_MODE_1               = { 0x1B, 0x2A, 0x21, -128, 0x00 };
   public static final byte[] SELECT_BIT_IMAGE_MODE_2               = { 0x1B, 0x2A, 0x21, (byte) 0xFF, 0x03 };
   public static final byte[] SET_LINE_SPACING_0                    = { 0x1B, 0x33, 0 };
   public static final byte[] SET_LINE_SPACING_24                   = { 0x1B, 0x33, 24 };
   public static final byte[] SET_LINE_SPACING_30                   = { 0x1B, 0x33, 30 };
   
   public static final byte[] TRANSMIT_DLE_PRINTER_STATUS           = { 0x10, 0x04, 0x01 };
   public static final byte[] TRANSMIT_DLE_OFFLINE_PRINTER_STATUS   = { 0x10, 0x04, 0x02 };
   public static final byte[] TRANSMIT_DLE_ERROR_STATUS             = { 0x10, 0x04, 0x03 };
   public static final byte[] TRANSMIT_DLE_ROLL_PAPER_SENSOR_STATUS = { 0x10, 0x04, 0x04 };
}
