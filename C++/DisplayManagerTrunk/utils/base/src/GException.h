#ifndef GEXCEPTION_H_
#define GEXCEPTION_H_

#include <qtconcurrentexception.h>
#include <QString>
#include <stdarg.h>

#define GEXCEPTION(x)\
	(\
		GException(QString("%1 [at %2:%3]").arg(x).arg(__FILE__).arg(__LINE__))\
	)

class GException : public QtConcurrent::Exception
{
public:	
	GException(QString strMsg);
	virtual ~GException() throw();
	
	virtual QString GetMessage();
	
	void raise() const;
	GException* clone() const;
	
	virtual const char * what () const throw ();

protected:
	QString m_strMsg;	
};

#endif
