// NGDTypes.h
//	NGDTypes has global data that represents functionality common to the product.
//	This package holds the types which can be used anywhere in the entire model.

#ifndef NGDTYPES_h
#define NGDTYPES_h

//	Enum for specifying operation mode of the CRIND.
//
//	eMOC:  Major Oil Company CRIND mode.
//	eGeneric:  Generic CRIND Mode.
//	eUnspecifiedMode:  CRIND mode has not been programmed.

typedef enum
{
	eMOC = 0,
	eGeneric = 1,
	eUnspecifiedMode = 99
} eCRINDMode;


enum
{
	eDefCrindID = 99
};

#endif
