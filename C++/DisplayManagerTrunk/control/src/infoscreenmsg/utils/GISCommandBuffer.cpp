#include "GISCommandBuffer.h"

GISCommandBuffer::GISCommandBuffer(QByteArray oBuffer):
	m_oList(oBuffer.split('\0')),
	m_iIndex(0)
{
}

GISCommandBuffer::~GISCommandBuffer( )
{
}

QString GISCommandBuffer::GetNextField()
{
	QString strTemp="";
	if( m_iIndex >= 0 && m_iIndex < (m_oList.length()-1) )
	{
		strTemp = m_oList[m_iIndex];
		m_iIndex++;
	}
	return strTemp;
}

int GISCommandBuffer::GetFieldCount( )
{
	return  (m_oList.length()-1);
}
