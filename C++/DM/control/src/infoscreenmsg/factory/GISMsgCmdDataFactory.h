//! GISMsgCmdDataFactory 
/*!
  Implementation of the Class GISMsgCmdDataFactory
*/

#if !defined(GISMsgCmdDataFactory_h)
#define GISMsgCmdDataFactory_h

#include "control/src/infoscreenmsg/base/GISAbsCommand.h"

#include "utils/base/src/GAbsCmdDataFactory.h"
#include "utils/base/src/GException.h"

#include <QMutex>

class GISMsgCmdDataFactory: public GAbsCmdDataFactory
{
public:
	virtual ~GISMsgCmdDataFactory();
	GISAbsCommand * Create(QByteArray & oInBuffer) throw(GException);
	static GISMsgCmdDataFactory * GetInstance() throw(GException);

protected:
	GISMsgCmdDataFactory();

private:
	typedef enum
	{
		eCMDFieldNumber		= 1	,
		eDISPFieldNumber		,
		eSCRNFieldNumber		,
		eSEGFieldNumber			,
		eFLDFieldNumber			,
		eXPOSFieldNumber		,
		eYPOSFieldNumber		,
		eATTRFieldNumber		,
		eFONTFieldNumber		,
		eCOLORFieldNumber		,
		eVLDFieldNumber		    ,
		
		eLastFieldNumber

	} eFieldNumber;	

private:
	QByteArray GetField( QByteArray& oInBuffer, eFieldNumber eFieldNumberRequired ) throw(GException);
	void Populate( GISAbsCommand* pCommand, QByteArray & oInBuffer ) throw(GException);

	static GISMsgCmdDataFactory* m_pGISMsgCmdDataFactory;
	static QMutex m_mutex;
};
#endif
