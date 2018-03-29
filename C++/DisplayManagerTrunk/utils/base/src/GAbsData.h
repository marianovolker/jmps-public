//! GAbsData 
/*!
  Implementation of the Class GAbsData
*/

#if !defined(GABSDATA)
#define GABSDATA

#include "GException.h"

class GAbsData
{
public:
	GAbsData();
	virtual ~GAbsData();
	
	//! This method should be implemented to validate that the data values are within allowed ranges
	virtual void Validate() throw(GException) = 0;
};
#endif
