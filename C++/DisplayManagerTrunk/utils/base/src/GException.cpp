#include "GException.h"

GException::GException(QString strMsg) : m_strMsg(strMsg)
{
}

GException::~GException() throw() { };

QString GException::GetMessage()
{
	return m_strMsg;
}

void GException::raise() const 
{
	throw *this; 
}

GException* GException::clone() const 
{ 
	return new GException(*this); 
}

const char * GException::what() const throw ()
{
	return qPrintable(m_strMsg);
}
