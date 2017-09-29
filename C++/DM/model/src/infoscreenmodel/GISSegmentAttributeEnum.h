#ifndef SEGMENTATTRIBUTEENUM_H_
#define SEGMENTATTRIBUTEENUM_H_

class GISSegmentAttributeEnum
{
public:

	typedef enum{
		eBeforeFistAttribute = -1,
		eErasePreviousLocation  			= 0,//For Command GISDisplaySegment
		eNotErasePreviousLocation               	= 1,//For Command GISDisplaySegment
		eVisible = 0, 					//Make segment visible
		eInvisible = 1,					//Make segment invisible
		eFlashSegmentStart = 2, 		//Flash segment start - not supported, reserved for future use.
		eFlashSegmentStop = 3, 			//Flash segment stop - not supported, reserved for future use.
		eAfterLastAttribute
	}eSegmentAttribute;
};

#endif

