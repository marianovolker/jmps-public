
#ifndef GMsgCmdDataFactory_H_
#define GMsgCmdDataFactory_H_

#include <QMutex>
#include <QByteArray>

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"
#include "utils/base/src/GAbsCmdDataFactory.h"


class GMsgCmdDataFactory : public GAbsCmdDataFactory
{
public:
	static GMsgCmdDataFactory* GetInstance();
	GAbsMsgCommand * Create(QByteArray & oInBuffer) throw(GException);
	
private:
	GMsgCmdDataFactory();
	virtual ~GMsgCmdDataFactory();	
	
public:
	static const QString CInterfaceVersion; 
	
private:
	static GMsgCmdDataFactory* m_poCmdDataFactory;
	static QMutex m_oMutex;
};

#endif
