//! GISCommandBuffer
/*!
  Implementation of the Class GISCommandBuffer
  A helper for a infoscreen command parser. 
*/

#if !defined(GISCommandBuffer_h)
#define GISCommandBuffer_h

#include <QString>
#include <QByteArray>
#include <QList>

class GISCommandBuffer
{
public:
	GISCommandBuffer( QByteArray oBuffer );
	virtual ~GISCommandBuffer();
	
	QString GetNextField();
	int GetFieldCount();
	
private:
	QList<QByteArray> m_oList;
	int m_iIndex;
};
#endif
