#include "TestParser.h"

#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/utils/GISCommandBuffer.h"
#include "control/src/infoscreenmsg/factory/GISMsgCmdDataFactory.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "control/src/infoscreenmsg/data/GISCommandResponse.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

#include <stdlib.h>

void TestParser::initTestCase()
{
	m_poCommand = NULL;

	unsetenv("DM_CONFIG_PATH");

  	GLogConfig* poConfig;
  	GLogger* poLogger;

  	// Logger StartUp
  	poConfig = new GLogConfig(GLogConfig::eBoth, GLogConfig::eLevel1, "./DisplayManager.log");
	poLogger = new GLogger(poConfig);
	GLogManager::AddLogger("DisplayManager.log", poLogger, false);

	LOG_LEVEL3("Starting Unit Test...");
}

void TestParser::cleanupTestCase()
{
	LOG_LEVEL4("cleanupTestCase()");

	if( m_poCommand != NULL )
	{
		delete m_poCommand;
		m_poCommand = NULL;
	}
}

void TestParser::CheckAddDisplayCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//# Add Display 
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30303100 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>
		QByteArray text = "303031003100303030003030300030303000303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() == "001");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  0);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "AddDisplayCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckAddScreenCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//#ADD SCREEN 1
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30303200 3100 30303100 30303000 30303000 30303000 30303000 303000 303000 30303100</data>
		//</infoscreen_request>
		QByteArray text = "303032003100303031003030300030303000303030003030300030300030300030303100";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  1);
		QVERIFY(pObjectData->GetCommand() ==  "002");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "AddScreenCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckAddSegmentCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//# Add segment 1 en screen 1 
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//    <interface_version>01.0.01</interface_version>
		//    <data>30303300 3100 30303100 30303100 30303000 30303100 30303100 303000 303000 30303200 30323000 30323000 303000</data>
		//</infoscreen_request>
		QByteArray text = "3030330031003030310030303100303030003030310030303100303000303000303032003032300030323000303000";

		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());

		QVERIFY(pObjectData->GetAttr()			==  0);
		QVERIFY(pObjectData->GetColor()			==  2);
		QVERIFY(pObjectData->GetCommand()		== "003");
		QVERIFY(pObjectData->GetDisplay()		==  1);
		QVERIFY(pObjectData->GetField()			==  0);
		QVERIFY(pObjectData->GetFont()			==  0);
		QVERIFY(pObjectData->GetScreen()		==  1);
		QVERIFY(pObjectData->GetSegment()		==  1);
		QVERIFY(pObjectData->GetXpos()			==  1);
		QVERIFY(pObjectData->GetYpos()			==  1);
		QVERIFY(pObjectData->GetName()			==  "AddSegmentCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  3);
		QVERIFY(oCommandBuffer.GetNextField()	==  "020");
		QVERIFY(oCommandBuffer.GetNextField()	==  "020");
		QVERIFY(oCommandBuffer.GetNextField()	==  "00");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckAddFieldCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//#Add field text 1 en segment 1 screen 1 "ESTE ES UN field"
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30303400 3100 30303100 30303100 30303100 30303100 30303100 303000 303100 30303000 303000 3000 4553544520455320554E206669656C6400</data>
		//</infoscreen_request>
		QByteArray text = "30303400310030303100303031003030310030303100303031003030003031003030300030300030004553544520455320554E206669656C6400";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
		
		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());
		
		QVERIFY(pObjectData->GetAttr()			==  0);
		QVERIFY(pObjectData->GetColor()			==  0);
		QVERIFY(pObjectData->GetCommand()		== "004");
		QVERIFY(pObjectData->GetDisplay()		==  1);
		QVERIFY(pObjectData->GetField()			==  1);
		QVERIFY(pObjectData->GetFont()			==  1);
		QVERIFY(pObjectData->GetScreen()		==  1);
		QVERIFY(pObjectData->GetSegment()		==  1);
		QVERIFY(pObjectData->GetXpos()			==  1);
		QVERIFY(pObjectData->GetYpos()			==  1);
		QVERIFY(pObjectData->GetName()			==  "AddFieldCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  3);
		QVERIFY(oCommandBuffer.GetNextField()	==  "00");
		QVERIFY(oCommandBuffer.GetNextField()	==  "0");
		QVERIFY(oCommandBuffer.GetNextField()	==  "ESTE ES UN field");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckAdjustBacklightIntensityCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//# AdjustBacklightIntensity 
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//    <interface_version>01.0.01</interface_version>
		//    <data>30313200 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>
		QByteArray text = "303132003100303030003030300030303000303030003030300030380030300030303000";

		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());
		
		QVERIFY(pObjectData->GetAttr()			==  8);
		QVERIFY(pObjectData->GetColor()			==  0);
		QVERIFY(pObjectData->GetCommand()		== "012");
		QVERIFY(pObjectData->GetDisplay()		==  1);
		QVERIFY(pObjectData->GetField()			==  0);
		QVERIFY(pObjectData->GetFont()			==  0);
		QVERIFY(pObjectData->GetScreen()		==  0);
		QVERIFY(pObjectData->GetSegment()		==  0);
		QVERIFY(pObjectData->GetXpos()			==  0);
		QVERIFY(pObjectData->GetYpos()			==  0);
		QVERIFY(pObjectData->GetName()			==  "AdjustBacklightIntensityCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  0);

	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckAdjustColorPaletteCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30313000 3100 30303000 30303000 30303000 30303000 30303000 303200 303000 30303000 30303400 31323800 31323500 31323300</data>
		//</infoscreen_request>
		QByteArray text = "30313000310030303000303030003030300030303000303030003032003030003030300030303400313238003132350031323300";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
		
		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());
		
		QVERIFY(pObjectData->GetAttr()			==  2);
		QVERIFY(pObjectData->GetColor()			==  0);
		QVERIFY(pObjectData->GetCommand()		== "010");
		QVERIFY(pObjectData->GetDisplay()		==  1);
		QVERIFY(pObjectData->GetField()			==  0);
		QVERIFY(pObjectData->GetFont()			==  0);
		QVERIFY(pObjectData->GetScreen()		==  0);
		QVERIFY(pObjectData->GetSegment()		==  0);
		QVERIFY(pObjectData->GetXpos()			==  0);
		QVERIFY(pObjectData->GetYpos()			==  0);
		QVERIFY(pObjectData->GetName()			==  "AdjustColorPaletteCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  4);
		QVERIFY(oCommandBuffer.GetNextField()	==  "004");
		QVERIFY(oCommandBuffer.GetNextField()	==  "128");
		QVERIFY(oCommandBuffer.GetNextField()	==  "125");
		QVERIFY(oCommandBuffer.GetNextField()	==  "123");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckAdjustSoundVolumeCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30323700 3100 30303000 30303000 30303000 30303000 30303000 303100 303000 30303000 30303300</data>
		//</infoscreen_request>
		QByteArray text = "30323700310030303000303030003030300030303000303030003031003030003030300030303300";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
		
		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());
		
		QVERIFY(pObjectData->GetAttr()			==  1);
		QVERIFY(pObjectData->GetColor()			==  0);
		QVERIFY(pObjectData->GetCommand()		== "027");
		QVERIFY(pObjectData->GetDisplay()		==  1);
		QVERIFY(pObjectData->GetField()			==  0);
		QVERIFY(pObjectData->GetFont()			==  0);
		QVERIFY(pObjectData->GetScreen()		==  0);
		QVERIFY(pObjectData->GetSegment()		==  0);
		QVERIFY(pObjectData->GetXpos()			==  0);
		QVERIFY(pObjectData->GetYpos()			==  0);
		QVERIFY(pObjectData->GetName()			==  "AdjustSoundVolumeCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  1);
		QVERIFY(oCommandBuffer.GetNextField()	==  "003");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckClearScreenCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//# clear screen
		//<?xml version="1.0" encoding="utf-8"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>303239003100303030003030300030303000303030003030300030300030300030303000</data>
		//</infoscreen_request>
		QByteArray text = "303239003100303030003030300030303000303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() == "029");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  0);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "ClearScreenCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckDisplayFlyTextCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//#FLY TEXT
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30323800 3100 30303000 30303000 30303000 30303100 30303100 303000 303000 30303100 4553544520455320554E20464C59205445585400</data>
		//</infoscreen_request>

		QByteArray text = "30323800310030303000303030003030300030303100303031003030003030003030310054484953204953204120464c59205445585400";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
		
		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());
		
		QVERIFY(pObjectData->GetAttr()    		==  0);
		QVERIFY(pObjectData->GetColor()   		==  1);
		QVERIFY(pObjectData->GetCommand() 		==  "028");
		QVERIFY(pObjectData->GetDisplay() 		==  1);
		QVERIFY(pObjectData->GetField()   		==  0);
		QVERIFY(pObjectData->GetFont()    		==  0);
		QVERIFY(pObjectData->GetScreen()  		==  0);
		QVERIFY(pObjectData->GetSegment() 		==  0);
		QVERIFY(pObjectData->GetXpos()    		==  1);
		QVERIFY(pObjectData->GetYpos()    		==  1);
		QVERIFY(pObjectData->GetName()    		==  "DisplayFlyTextCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  1);
		QVERIFY(oCommandBuffer.GetNextField()	==  "THIS IS A FLY TEXT");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckDisplayScreenCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//#DISPLAY SCREEN_13
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30313400 3100 30303100 30303000 3030300 03030300 03030300 0303000 303000 30303100</data>
		//</infoscreen_request>		

		QByteArray text = "303134003100303031003030300030303000303030003030300030300030300030303100";

        
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
      

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  1);
		QVERIFY(pObjectData->GetCommand() ==  "014");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "DisplayScreenCommand");

    
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckDisplaySegmentCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30313500 3100 30303100 30303100 30303000 30303100 30303100 303100 303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "303135003100303031003030310030303000303031003030310030310030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  1);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "015");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  1);
		QVERIFY(pObjectData->GetXpos()    ==  1);
		QVERIFY(pObjectData->GetYpos()    ==  1);
		QVERIFY(pObjectData->GetName()    ==  "DisplaySegmentCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckEraseApplicationCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30333100 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "303331003100303030003030300030303000303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "031");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  0);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "EraseApplicationCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckModifyFieldCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//#Modify field text 1 en segment 1 screen 1 "ESTE ES UN field"
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30323000 3100 30303100 30303100 30303100 30303100 30303100 303000 303100 30303000 4553544520455320554E206669656C6400</data>
		//</infoscreen_request>
		QByteArray text = "3032300031003030310030303100303031003030310030303100303000303100303030004553544520455320554E206669656C6400";

		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());

		QVERIFY(pObjectData->GetAttr()			==  0);
		QVERIFY(pObjectData->GetColor()			==  0);
		QVERIFY(pObjectData->GetCommand()		== "020");
		QVERIFY(pObjectData->GetDisplay()		==  1);
		QVERIFY(pObjectData->GetField()			==  1);
		QVERIFY(pObjectData->GetFont()			==  1);
		QVERIFY(pObjectData->GetScreen()		==  1);
		QVERIFY(pObjectData->GetSegment()		==  1);
		QVERIFY(pObjectData->GetXpos()			==  1);
		QVERIFY(pObjectData->GetYpos()			==  1);
		QVERIFY(pObjectData->GetName()			==  "ModifyFieldCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  1);
		QVERIFY(oCommandBuffer.GetNextField()	==  "ESTE ES UN field");

	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckModifySegmentCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//#modify segment 67- 211 - visible
		//<?xml version="1.0" encoding="utf-8"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30313600 3100 30303100 30303100 30303000 30303000 30303000 303000 303000 30303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "30313600310030303100303031003030300030303000303030003030003030003030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "016");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  1);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "ModifySegmentCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckObjectAddressCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="utf-8"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30313800 3100 30303100 30303100 30303100 30303000 30303000 303000 303000 30303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "30313800310030303100303031003030310030303000303030003030003030003030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "018");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  1);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  1);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "ObjectAddressCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckSaveApplicationCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30333000 3100 30303000 30303000 30303000 30303000 30303000 303100 303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "303330003100303030003030300030303000303030003030300030310030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  1);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "030");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  0);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "SaveApplicationCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckSegmentDisplayCountCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30313100 3100 30303100 30303100 30303000 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "303131003100303031003030310030303000303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "011");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  1);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "SegmentDisplayCountCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckSetChromakeyColorCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30333300 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303100</data>
		//</infoscreen_request>		

		QByteArray text = "303333003100303030003030300030303000303030003030300030300030300030303100";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  1);
		QVERIFY(pObjectData->GetCommand() ==  "033");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  0);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "SetChromakeyColorCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckSetFieldAttributeCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30303900 3100 30303100 30303100 30303100 30303000 30303000 303600 303000 30303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "30303900310030303100303031003030310030303000303030003036003030003030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  6);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "009");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  1);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  1);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "SetFieldAttributeCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckSetGraphicsTypeCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30333400 3100 30303000 30303000 30303000 30303000 30303000 303200 303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "303334003100303030003030300030303000303030003030300030320030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  2);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "034");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  0);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "SetGraphicsTypeCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckSetScreenColorCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30313300 3100 30303100 30303000 30303000 30303000 30303000 303000 303000 30303700</data>
		//</infoscreen_request>		

		QByteArray text = "303133003100303031003030300030303000303030003030300030300030300030303700";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  7);
		QVERIFY(pObjectData->GetCommand() ==  "013");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "SetScreenColorCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckStatusCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30313900 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "303139003100303030003030300030303000303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "019");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  0);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "StatusCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckVideoFrameSeekCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30323300 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000 39393939393900</data>
		//</infoscreen_request>		

		QByteArray text = "30323300310030303000303030003030300030303000303030003030003030003030300039393939393900";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
		
		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());
		
		QVERIFY(pObjectData->GetAttr()    		==  0);
		QVERIFY(pObjectData->GetColor()   		==  0);
		QVERIFY(pObjectData->GetCommand() 		==  "023");
		QVERIFY(pObjectData->GetDisplay() 		==  1);
		QVERIFY(pObjectData->GetField()   		==  0);
		QVERIFY(pObjectData->GetFont()    		==  0);
		QVERIFY(pObjectData->GetScreen()  		==  0);
		QVERIFY(pObjectData->GetSegment() 		==  0);
		QVERIFY(pObjectData->GetXpos()    		==  0);
		QVERIFY(pObjectData->GetYpos()    		==  0);
		QVERIFY(pObjectData->GetName()    		==  "VideoFrameSeekCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  1);
		QVERIFY(oCommandBuffer.GetNextField()	==  "999999");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckVideoPlayCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30323400 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000 303100</data>
		//</infoscreen_request>		

		QByteArray text = "303234003100303030003030300030303000303030003030300030300030300030303000303100";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
		
		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());
		
		QVERIFY(pObjectData->GetAttr()    		==  0);
		QVERIFY(pObjectData->GetColor()   		==  0);
		QVERIFY(pObjectData->GetCommand() 		==  "024");
		QVERIFY(pObjectData->GetDisplay() 		==  1);
		QVERIFY(pObjectData->GetField()   		==  0);
		QVERIFY(pObjectData->GetFont()    		==  0);
		QVERIFY(pObjectData->GetScreen()  		==  0);
		QVERIFY(pObjectData->GetSegment() 		==  0);
		QVERIFY(pObjectData->GetXpos()    		==  0);
		QVERIFY(pObjectData->GetYpos()    		==  0);
		QVERIFY(pObjectData->GetName()    		==  "VideoPlayCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  1);
		QVERIFY(oCommandBuffer.GetNextField()	==  "01");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckVideoRangePlayCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30323600 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000 30303030303000 39393939393900</data>
		//</infoscreen_request>		

		QByteArray text = "3032360031003030300030303000303030003030300030303000303000303000303030003030303030300039393939393900";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
		
		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());

		QVERIFY(pObjectData->GetAttr()    		==  0);
		QVERIFY(pObjectData->GetColor()   		==  0);
		QVERIFY(pObjectData->GetCommand() 		==  "026");
		QVERIFY(pObjectData->GetDisplay() 		==  1);
		QVERIFY(pObjectData->GetField()   		==  0);
		QVERIFY(pObjectData->GetFont()    		==  0);
		QVERIFY(pObjectData->GetScreen()  		==  0);
		QVERIFY(pObjectData->GetSegment() 		==  0);
		QVERIFY(pObjectData->GetXpos()    		==  0);
		QVERIFY(pObjectData->GetYpos()    		==  0);
		QVERIFY(pObjectData->GetName()    		==  "VideoRangePlayCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  2);
		QVERIFY(oCommandBuffer.GetNextField()	==  "000000");
		QVERIFY(oCommandBuffer.GetNextField()	==  "999999");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckVideoStopCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30323500 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>		

		QByteArray text = "303235003100303030003030300030303000303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
		
		QVERIFY(pObjectData->GetAttr()    		==  0);
		QVERIFY(pObjectData->GetColor()   		==  0);
		QVERIFY(pObjectData->GetCommand() 		==  "025");
		QVERIFY(pObjectData->GetDisplay() 		==  1);
		QVERIFY(pObjectData->GetField()   		==  0);
		QVERIFY(pObjectData->GetFont()    		==  0);
		QVERIFY(pObjectData->GetScreen()  		==  0);
		QVERIFY(pObjectData->GetSegment() 		==  0);
		QVERIFY(pObjectData->GetXpos()    		==  0);
		QVERIFY(pObjectData->GetYpos()    		==  0);
		QVERIFY(pObjectData->GetName()    		==  "VideoStopCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckVideoViewCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//	<interface_version>01.0.01</interface_version>
		//	<data>30323200 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000 303100</data>
		//</infoscreen_request>		

		QByteArray text = "303232003100303030003030300030303000303030003030300030300030300030303000303100";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();
		
		GISCommandBuffer oCommandBuffer(pObjectData->GetVLD());

		QVERIFY(pObjectData->GetAttr()    		==  0);
		QVERIFY(pObjectData->GetColor()   		==  0);
		QVERIFY(pObjectData->GetCommand() 		==  "022");
		QVERIFY(pObjectData->GetDisplay() 		==  1);
		QVERIFY(pObjectData->GetField()   		==  0);
		QVERIFY(pObjectData->GetFont()    		==  0);
		QVERIFY(pObjectData->GetScreen()  		==  0);
		QVERIFY(pObjectData->GetSegment() 		==  0);
		QVERIFY(pObjectData->GetXpos()    		==  0);
		QVERIFY(pObjectData->GetYpos()    		==  0);
		QVERIFY(pObjectData->GetName()    		==  "VideoViewCommand");
		QVERIFY(oCommandBuffer.GetFieldCount()  ==  1);
		QVERIFY(oCommandBuffer.GetNextField()	==  "01");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckDeleteFieldCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30303800 3100 30303100 30303100 30303100 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>
		QByteArray text = "303038003100303031003030310030303100303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "008");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  1);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  1);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "DeleteFieldCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckDeleteSegmentCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30303700 3100 30303100 30303100 30303000 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>
		QByteArray text = "303037003100303031003030310030303000303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "007");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  1);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "DeleteSegmentCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckDeleteScreenCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//#DELETE SCREEN 3
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30303600 3100 30303100 30303000 30303000 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>
		QByteArray text = "303036003100303031003030300030303000303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "006");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  1);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "DeleteScreenCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

void TestParser::CheckDeleteDisplayCommand()
{
	LOG_LEVEL4(".");

	try
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
		//<infoscreen_request xmlns="http://www.gilbarco.com/crind/display_manager/api">
		//<interface_version>01.0.01</interface_version>
		//<data>30303500 3100 30303000 30303000 30303000 30303000 30303000 303000 303000 30303000</data>
		//</infoscreen_request>
		QByteArray text = "303035003100303030003030300030303000303030003030300030300030300030303000";
		
		GISMsgCmdDataFactory* pFactory;
		GISAbsData* pObjectData;
		pFactory = GISMsgCmdDataFactory::GetInstance();
		m_poCommand = pFactory->Create(text);
		pObjectData = m_poCommand->GetData();

		QVERIFY(pObjectData->GetAttr()    ==  0);
		QVERIFY(pObjectData->GetColor()   ==  0);
		QVERIFY(pObjectData->GetCommand() ==  "005");
		QVERIFY(pObjectData->GetDisplay() ==  1);
		QVERIFY(pObjectData->GetField()   ==  0);
		QVERIFY(pObjectData->GetFont()    ==  0);
		QVERIFY(pObjectData->GetScreen()  ==  0);
		QVERIFY(pObjectData->GetSegment() ==  0);
		QVERIFY(pObjectData->GetXpos()    ==  0);
		QVERIFY(pObjectData->GetYpos()    ==  0);
		QVERIFY(pObjectData->GetName()    ==  "DeleteDisplayCommand");
		
	}
	catch(GException& e)
	{
		qDebug() << "Exception:" << " GISMsgCmdDataFactory: " << e.GetMessage();
		QVERIFY(false);
	}
}

QTEST_MAIN(TestParser)
