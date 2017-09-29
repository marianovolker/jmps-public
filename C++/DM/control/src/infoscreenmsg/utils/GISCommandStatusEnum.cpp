#include "GISCommandStatusEnum.h"


const char* GISCommandStatusEnum::strCommandStatusData[eLastError+1] =
{
	"No Errors",
	"Command Error",
	"Display Number Already Used",
	"Screen Number Already Used",
	"Segment number Already Used",
	"Field Number Already Used",
	"Invalid Display Number or Display Number Does Not Exist",
	"Invalid Screen Number or Screen Number Does Not Exist",
	"Invalid Segement Number or Segment Number Does Not Exist",
	"Invalid Field Number or Field Number Does Not Exist",
	"Invalid X Position or X Range Exceeded",
	"Invalid Y Position or Y Range Exceeded",
	"Invalid Attribute",
	"Font Number Undefined",
	"Font Number Already Used",
	"Variable Length Data Contains NonDisplayable Characters",
	"Video Window Size Error",
	"Video Media Player Error",
	"Video Not Available",
	"Display Object Space Exceeded",
	"Display Frame Buffer Sapce Exceeded",
	"Busy Processing a Command",
	"Diagnostic Tests Complete",
	"Invalid Download Type",
	"Backlight Condition Not Detected",
	"Speech Processing Not Available",
	"Flash RAM WriteRead Error",
	"Flash RAM Not Available",
	"Flash RAM Sapce Exceeded",
	"Image Decompression Complete",
	"Application Save Complete",
	"Display And Volume Adjustments Complete",
	"Invalid Color or Color Register",
	"All Expected Packets Received",
	"Font Number Specified Not Expected",
	"Segment Number Specified Not Expected",
	"Packet Number 1 Expected",
	"Variable Length Data Contains Invalid Length",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"High Temperature",
	"Video Switched On",
	
	"Last Error"
};

GISCommandStatusEnum::GISCommandStatusEnum( )
{
}

GISCommandStatusEnum::GISCommandStatusEnum( eCommandStatusEnum e )
{
	m_eCommandStatusEnumValue = e;
}

GISCommandStatusEnum::~GISCommandStatusEnum()
{
}

void GISCommandStatusEnum::SetError( eCommandStatusEnum e )
{
	m_eCommandStatusEnumValue = e;
}

GISCommandStatusEnum::eCommandStatusEnum GISCommandStatusEnum::GetError( )
{
	return m_eCommandStatusEnumValue;
}

QString GISCommandStatusEnum::toString()
{
	return (m_eCommandStatusEnumValue>=eNoErrors && m_eCommandStatusEnumValue<=eLastError)? (GISCommandStatusEnum::strCommandStatusData[m_eCommandStatusEnumValue]):("Unknow Error");
}
