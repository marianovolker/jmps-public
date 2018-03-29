//! GISAbsCommand 
/*!
  Implementation of the Class GISAbsCommand
*/

#if !defined(GISAbsCommand_h)
#define GISAbsCommand_h

#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "control/src/infoscreenmsg/data/GISCommandResponse.h"
#include "utils/base/src/GAbsCommand.h"

class GISAbsCommand : public GAbsCommand 
{
public:
	GISAbsCommand();
	virtual ~GISAbsCommand();
	
	virtual void Execute() throw(GException);
	
	virtual void DoExecute() throw(GException) = 0;
	
	/*!
	 * Overwrite setters and getters
 	 */
	virtual GISAbsData * GetData();
	virtual void SetData(GISAbsData * pData);
	virtual GISCommandResponse * GetResponse();
	virtual void SetResponse(GISCommandResponse * pResponse);
};
#endif
