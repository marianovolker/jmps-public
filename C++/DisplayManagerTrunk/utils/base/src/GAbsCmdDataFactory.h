//! GAbsCmdDataFactory 
/*!
  Implementation of the Class GAbsCmdDataFactory
*/

#ifndef GAbsCmdDataFactory_H_
#define GAbsCmdDataFactory_H_

#include "GException.h"
#include "QByteArray"

class GAbsCommand;

class GAbsCmdDataFactory
{
public:
	static GAbsCmdDataFactory* GetInstance();
	virtual GAbsCommand * Create(QByteArray & oInBuffer) throw(GException) = 0;

protected:
	GAbsCmdDataFactory();
	virtual ~GAbsCmdDataFactory();
};
#endif
