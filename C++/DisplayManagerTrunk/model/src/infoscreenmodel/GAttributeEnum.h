#ifndef ATTRIBUTEENUM_H_
#define ATTRIBUTEENUM_H_

class GAttributeEnum
{
public:
	typedef enum
	{
		eBeforeFirstAttrib				= -1,

		eErasePreviousLocation  			= 0,//For Command GISDisplaySegment
                eNotErasePreviousLocation               	= 1,//For Command GISDisplaySegment

		eVisible					= 0,
		eInvisible					= 1,
		
		eNotEraseTextAreaBeforeDisplay                  = 0,//For Command GISDisplayFlyText
		eEraseTextAreaBeforeDisplay                     = 1,//For Command GISDisplayFlyText
		eEraseLineBeforeDisplay                         = 2,//For Command GISDisplayFlyText

		eStartFlash					= 6,
		eStopFlash					= 7,
		
		eScrollMedium					= 8,
		eStopScroll					= 9,
		
		eCenterText					= 10,
		eLeftText					= 11,
		eRightText                                      = 12,
		
		eUnderscoreOn					= 13,
                eCenterWithUnderscore                           = 14,
		eLeftWithUnderscore				= 15,
		eRightWithUnderscore                            = 16,
		eUnderscoreOff					= 17,
		
		eHorizontalScrollSlow                           = 18,
		eHorizontalScrollMedium                         = 19,
		eHorizontalScrollFast                           = 20,
		
		ePopTextSlow					= 21,
		ePopTextMedium					= 22,
		ePopTextFast					= 23,
		
		eVerticalScrollSlow				= 24,
		eVerticalScrollMedium   			= 25,
		eVerticalScrollFast				= 26,

				

		eAfterLastAttrib
	} eAttributes;
};


#endif /*ATTRIBUTEENUM_H_*/




