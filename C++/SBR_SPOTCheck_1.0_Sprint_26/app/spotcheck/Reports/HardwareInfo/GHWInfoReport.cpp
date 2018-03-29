/**
 * HWInfoReport.cpp
 *
 *  Created on: May 06 2013
 *      Author: G.Paris
 */

#include "GHWInfoReport.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"

const char* GHWInfoReport::mst_arypcTextLines[HARDWARE_REPORT_MAX_LINES] =
{
	"------------------------------\n", // line  0
	"Security Module\n",                // line  1
	"\n",                               // line  2
	"S/N: %s\n",                        // line  3
	"Firmware Ver: %s\n",               // line  4
	"\n",                               // line  5
	"------------------------------\n", // line  6
	"Note Acceptor\n",                  // line  7
	"\n",                               // line  8
	"Firmware: %s\n",                   // line  9
	"\n",                               // line 10
	"------------------------------\n", // line 11
	"Card Reader\n",                    // line 12
	"\n",                               // line 13
	"Type: %02X\n",                     // line 14
	"Model: %s\n",                      // line 15
	"S/N: %s\n",                        // line 16
	"Firmware Ver: %s\n",               // line 17
	"\n",                               // line 18
	"------------------------------\n", // line 19
	"Contactless Reader \n",            // line 20
	"\n",                               // line 21
	"Model: %s\n",                      // line 22
	"S/N: %s\n",                        // line 23
	"------------------------------\n", // line 24
	"Keypad \n",                        // line 25
	"\n",                               // line 26
	"Type: %02X\n",                     // line 27
	"S/N: %s\n",                        // line 28
	"Firmware Ver: %s\n",               // line 29
	"\n",                               // line 30
	"------------------------------\n", // line 31
	"Display \n",                       // line 32
	"\n",                               // line 33
	"Type: %02X\n",                     // line 34
	"Model: %s\n",                      // line 35
	"Functions: %02X %02X %02X %02X\n", // line 36
	"Resolution: %d x %d x %d\n",       // line 37
	"\n",                               // line 38
	"------------------------------\n", // line 39
	"Printer\n",                        // line 40
	"\n",                               // line 41
	"Type: %02X\n",                     // line 42
	"Model: %s\n",                      // line 43
	"S/N: %s\n",                        // line 44
	"------------------------------\n", // line 45
	"\n",                               // line 46
	"\n",                               // line 47
	"SPOT Operating System Version\n",  // line 48
	"\n",                               // line 49
	"OS: %s\n",                         // line 50
	"\n",                               // line 51
	"------------------------------\n", // line 52
	"ROMfs Software Information\n",     // line 53
	"\n",                               // line 54
	"ROMfs1: %s\n",                     // line 55
	"ROMfs2: %s\n",                     // line 56
	"ROMfs3: %s\n",                     // line 57
	"ROMfs4: %s\n",                     // line 58
	"ROMfs5: %s\n",                     // line 59
	"ROMfs6: %s\n",                     // line 60
	"ROMfs7: %s\n",                     // line 61
	"ROMfs8: %s\n",                     // line 62
	"\n",                               // line 63
	"------------------------------\n", // line 64
	"\n",                               // line 65
	"%c%c\n"                            // line 66
};


GHWInfoReport::GHWInfoReport()
{
}

GHWInfoReport::~GHWInfoReport()
{
}

int  GHWInfoReport::TextDump(GReportHeader::eReportType eType)
{
	int nLine = 0;
	GGlobalSystemInformation* pInfo = GGlobalSystemInformation::GetInstance();

	DumpHeaderText(eType);

	if( NoMoreRoomOnBuffer() )
		return GetPartialCount(); // no more room for another header text line

	while(nLine < 3) // lines 0 to 2
	{
		PutVoidDataLine(nLine++);
		if( NoMoreRoomOnBuffer() )
			return GetPartialCount(); // no more room for another header text line
	}

	PopulateLine(nLine++, pInfo->GetSecModuleInfo().m_pcSerialNumber); // line 3
	if ( NoMoreRoomOnBuffer() )
		return GetPartialCount();

	PopulateLine(nLine++, pInfo->GetFirmwareInfo(eSecFwrVersion)); // line 4
	if ( NoMoreRoomOnBuffer() )
		return GetPartialCount();

	while(nLine < 7) // lines 5 to 6
	{
		PutVoidDataLine(nLine++);
		if( NoMoreRoomOnBuffer() )
			return GetPartialCount();
	}

	// Cash Acceptor test not evaluated yes for release 1.0 - REMOVE this when ready
	nLine = 12;
/*
	while(nLine < 9) // lines 5 to 8
	{
		PutVoidDataLine(nLine++);
		if( NoMoreRoomOnBuffer() )
			return GetPartialCount();
	}

	PopulateLine(nLine++, "xxxx"); // line 9 - Cash Acceptor firmware version
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();
*/
	while(nLine < 14) // lines 10 to 13
	{
		PutVoidDataLine(nLine++);
		if( NoMoreRoomOnBuffer() )
			return GetPartialCount();
	}

	PopulateLine(nLine++, pInfo->GetCardReaderInfo().m_u8DevType); // line 14
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	PopulateLine(nLine++, pInfo->GetCardReaderInfo().m_pcDevModel); // line 15
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	PopulateLine(nLine++, pInfo->GetCardReaderInfo().m_pcSerialNumber); // line 16
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	PopulateLine(nLine++, pInfo->GetFirmwareInfo(eCRFwrVersion)); // line 17
	if ( NoMoreRoomOnBuffer() )
		return GetPartialCount();

	while(nLine < 22) // lines 18 to 19
	{
		PutVoidDataLine(nLine++);
		if( NoMoreRoomOnBuffer() )
			return GetPartialCount();
	}

	// Contactless HW info, is not available by SPOT protocol.

	PopulateLine(nLine++, "Not available"); // line 22 - Contactless Model
	if (NoMoreRoomOnBuffer())
	    return GetPartialCount();

	PopulateLine(nLine++, "Not available"); // line 23 - Contactless S/N
	if (NoMoreRoomOnBuffer())
	    return GetPartialCount();

	while(nLine < 27) // lines 25 to 26
	{
		PutVoidDataLine(nLine++);
		if( NoMoreRoomOnBuffer() )
			return GetPartialCount();
	}

	PopulateLine(nLine++, pInfo->GetKeypadInfo().m_u8DevType); // line 27
	if (NoMoreRoomOnBuffer())
			return GetPartialCount();

	PopulateLine(nLine++, pInfo->GetKeypadInfo().m_pcSerialNumber); // line 28
	if (NoMoreRoomOnBuffer())
			return GetPartialCount();

	PopulateLine(nLine++, pInfo->GetFirmwareInfo(eKPFwrVersion)); // line 29
	if ( NoMoreRoomOnBuffer() )
		return GetPartialCount();

	while(nLine < 34) // lines 30 to 33
	{
		PutVoidDataLine(nLine++);
		if( NoMoreRoomOnBuffer() )
			return GetPartialCount();
	}

	PopulateLine(nLine++, pInfo->GetDisplayInfo().m_u8DevType); // line 34
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	PopulateLine(nLine++, pInfo->GetDisplayInfo().m_pcDevModel); // line 35
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	// line 36
	PopulateLine(nLine++, pInfo->GetDisplayInfo().m_aryu8DevFunctions[0], pInfo->GetDisplayInfo().m_aryu8DevFunctions[1], pInfo->GetDisplayInfo().m_aryu8DevFunctions[2], pInfo->GetDisplayInfo().m_aryu8DevFunctions[3]);
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	// line 37
	PopulateLine(nLine++, pInfo->GetDisplayInfo().m_DevHwrConfig.m_u16Xres, pInfo->GetDisplayInfo().m_DevHwrConfig.m_u16Yres, pInfo->GetDisplayInfo().m_DevHwrConfig.m_u8ColorDepth );
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	while(nLine < 42) // lines 38 to 40
	{
		PutVoidDataLine(nLine++);
		if( NoMoreRoomOnBuffer() )
			return GetPartialCount();
	}

	PopulateLine(nLine++, pInfo->GetPrinterInfo().m_u8DeviceType); // line 42
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	PopulateLine(nLine++, pInfo->GetPrinterInfo().m_pcDevModel); // line 43
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	PopulateLine(nLine++, pInfo->GetPrinterInfo().m_pcSerialNumber); // line 44
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	while(nLine < 50) // lines 44 to 49
	{
		PutVoidDataLine(nLine++);
		if( NoMoreRoomOnBuffer() )
			return GetPartialCount();
	}

	PopulateLine(nLine++, pInfo->GetSoftwareInfo(eOSVersion)); // line 50
	if (NoMoreRoomOnBuffer())
		return GetPartialCount();

	int idx = eROMFs1Version;

	while(nLine < 66) // lines 51 to 65
	{
		if(nLine > 54 && nLine < 63)  // lines 55 to 62 contain data
			PopulateLine(nLine++, pInfo->GetSoftwareInfo(idx++));
		else
			PutVoidDataLine(nLine++);

		if( NoMoreRoomOnBuffer() )
			return GetPartialCount(); // no more room for another header text line
	}

	if( NoMoreRoomOnBuffer(2) )
		return GetPartialCount(); // no more room for two more characters !!

	PopulateLine(66, 0x1B, 'E');

	return GetPartialCount();

}


