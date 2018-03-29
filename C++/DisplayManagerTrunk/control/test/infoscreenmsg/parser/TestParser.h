#include <QtTest/QtTest>
#include <QString>
#include "control/src/infoscreenmsg/base/GISAbsCommand.h"


class TestParser : public QObject
{
	Q_OBJECT
	
private:

	GISAbsCommand* m_poCommand;
	
private slots:		
	void initTestCase();
	void cleanupTestCase();

	// Parser Testing
	void CheckAddDisplayCommand();
	void CheckAddScreenCommand();
	void CheckAddSegmentCommand();
	void CheckAddFieldCommand();	
	void CheckAdjustBacklightIntensityCommand();
	void CheckAdjustColorPaletteCommand();
	void CheckAdjustSoundVolumeCommand();
	void CheckClearScreenCommand();
	void CheckDisplayFlyTextCommand();
	void CheckDisplayScreenCommand();
	void CheckDisplaySegmentCommand();
	void CheckEraseApplicationCommand();
	void CheckModifyFieldCommand();
	void CheckModifySegmentCommand();
	void CheckObjectAddressCommand();
	void CheckSaveApplicationCommand();	
	void CheckSegmentDisplayCountCommand();
	void CheckSetChromakeyColorCommand();
	void CheckSetFieldAttributeCommand();	
	void CheckSetGraphicsTypeCommand();
	void CheckSetScreenColorCommand();	
	void CheckStatusCommand();	
	void CheckVideoFrameSeekCommand();
	void CheckVideoPlayCommand();
	void CheckVideoRangePlayCommand();
	void CheckVideoStopCommand();	
	void CheckVideoViewCommand();
	void CheckDeleteFieldCommand();
	void CheckDeleteSegmentCommand();
	void CheckDeleteScreenCommand();	
	void CheckDeleteDisplayCommand();	
};
