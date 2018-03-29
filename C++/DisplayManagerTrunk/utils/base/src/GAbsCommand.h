//! GAbsCommand 
/*!
  Implementation of the Class GAbsCommand
*/

#if !defined(GABSCOMMAND)
#define GABSCOMMAND

#include "GAbsData.h"
#include "GException.h"

class GAbsCommand
{
public:
	GAbsCommand(GAbsData * pData = NULL);
	virtual ~GAbsCommand();
	
	virtual GAbsData * GetData();
	virtual void SetData(GAbsData * pData);
	
	virtual GAbsData * GetResponse();
	virtual void SetResponse(GAbsData * pResponse);	
	
	virtual void Execute() throw(GException) = 0;

protected:
	GAbsData * m_pData;
	GAbsData * m_pResponse;

};
#endif
