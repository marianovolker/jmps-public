#include "GControlChannelHandler.h"

#include <QXmlStreamReader>

#include "control/src/displaymanagermsgs/base/GAbsMsgCommand.h"

const QByteArray GControlChannelHandler::CXMLStartTag ("<?xml");
const QByteArray GControlChannelHandler::CXMLDeclarationEndTag ("?>");
const QString GControlChannelHandler::CXMLCommandEndTag ("</%1>");

GControlChannelHandler::GControlChannelHandler(int iSocketDescriptor, QObject *poParent, GAbsCmdDataFactory *poMsgCmdDataFactory)
	: m_poTcpSocket(NULL),
	  m_iSocketDescriptor(iSocketDescriptor),
	  m_strCommandName(""),
	  bCommandNameFound(false),
	  m_oBuffer(""),
	  m_poMsgCmdDataFactory(poMsgCmdDataFactory),
	  m_poParent(poParent)
{
	LOG_LEVEL4("GControlChannelHandler()");

	moveToThread(this);
}

GControlChannelHandler::~GControlChannelHandler()
{
	LOG_LEVEL4("~GControlChannelHandler()");

	if( m_poTcpSocket != NULL )
	{
		delete m_poTcpSocket;
		m_poTcpSocket = NULL;
	}
}

void GControlChannelHandler::run()
{
	LOG_LEVEL3("run()");

    if( !connect(m_poParent, SIGNAL(SignalStopChildren()), this, SLOT(SlotStop()), Qt::QueuedConnection) )
    {
    	LOG_LEVEL1("Can't connect Control Channel Handler to Control Channel TcpServer.");
    }

	m_poTcpSocket = new QTcpSocket();
	
	if( !connect(m_poTcpSocket, SIGNAL(readyRead()), SLOT(ReadMessage())) )
	{
		LOG_LEVEL1("Can't connect QTcpSocket::readyRead() to method GControlChannelHandler::ReadMessage().");
	}

    if( !connect(m_poTcpSocket, SIGNAL(disconnected()), SLOT(SlotStop())) )
    {
    	LOG_LEVEL1("Can't connect QTcpSocket::disconnected() to method GControlChannelHandler::SlotStop().");
    }

    if( !connect(this, SIGNAL(finished()), SLOT(SlotThreadFinished())) )
    {
    	LOG_LEVEL1("Can't connect GControlChannelHandler::finished() to method GControlChannelHandler::SlotThreadFinished().");
    }
	
    if( m_poTcpSocket->setSocketDescriptor(m_iSocketDescriptor) )
    {
    	exec();    	
    }

    LOG_LEVEL4("The event loop of this thread has been finished");
}

void GControlChannelHandler::SlotStop()
{
	LOG_LEVEL3("SlotStop()");

	// We must avoid cross-signal situations
    if( !disconnect(m_poTcpSocket, 0, 0, 0) )
    {
    	LOG_LEVEL1("Can't disconnect to all signals.");
    }
    
    if( m_poTcpSocket != NULL && m_poTcpSocket->isOpen() )
    {
    	m_poTcpSocket->close();
    }

	quit();
}

void GControlChannelHandler::SlotThreadFinished()
{
	LOG_LEVEL4("SlotThreadFinished()");

	emit SignalHandlerStopped(this);
}

void GControlChannelHandler::starttrimmed(QByteArray &oBuffer)
{
	LOG_LEVEL4("starttrimmed()");

	char cStartChar;

	if( !oBuffer.isEmpty() )
	{
		cStartChar = oBuffer.at(0);
		while( cStartChar==' ' || cStartChar=='\n' || cStartChar=='\t' || cStartChar=='\v' || cStartChar=='\f' || cStartChar=='\r' )
		{
			oBuffer.remove(0,1);
			if (!oBuffer.isEmpty())
			{
				cStartChar = oBuffer.at(0);
			}
			else
			{
				break;
			}
		}
	}
}

void GControlChannelHandler::ReadMessage()
{
	LOG_LEVEL4("ReadMessage()");

	QByteArray oMsgReceived = m_poTcpSocket->readAll();

	int iNewMessagePos, iSecondNewMessagePos;
	bool bContinue = true;

	while( bContinue ) // Process while there isn't any new message left
	{
		iNewMessagePos = oMsgReceived.indexOf(GControlChannelHandler::CXMLStartTag);

		if (iNewMessagePos < 0) // The received packet is the last part of an old XML message
		{
			m_oBuffer.append(oMsgReceived);
			bContinue = false;
		}
		else
		{
			if (iNewMessagePos > 0) // The beginning of the buffer is the remainder of the last message
			{
				m_oBuffer.append(oMsgReceived.left(iNewMessagePos));
				oMsgReceived = oMsgReceived.mid(iNewMessagePos);
			}
			else
			{

				iSecondNewMessagePos = oMsgReceived.indexOf(GControlChannelHandler::CXMLStartTag, iNewMessagePos+1);
				if (iSecondNewMessagePos < 0) // There is just one new message at the position 0 of the buffer
				{
					m_oBuffer = oMsgReceived;
					bCommandNameFound = false;
					bContinue = false;
				}
				else // There is more than one new message.  The first is at position 0
				{
					m_oBuffer.append(oMsgReceived.left(iSecondNewMessagePos));
					oMsgReceived = oMsgReceived.mid(iSecondNewMessagePos);
					bCommandNameFound = false;
				}
			}
		}

		if (IsReady())
		{
			Dispatch();
			m_oBuffer.clear(); // Reset the buffer
			bCommandNameFound = false;
		}
	}
}

bool GControlChannelHandler::IsReady()
{
	LOG_LEVEL4("IsReady()");

	bool bMsgComplete = false;

	if (!bCommandNameFound) // If the CommandName hasn't been already found, we look it up
	{
		starttrimmed(m_oBuffer);
		QXmlStreamReader oXmlBuffer(m_oBuffer);
		int iEndOfXmlDeclaration(m_oBuffer.indexOf(GControlChannelHandler::CXMLDeclarationEndTag));
		bool bWellFormedXmlBeginning(true);
		
		if ( !(bWellFormedXmlBeginning = !(iEndOfXmlDeclaration < 0)) )
		{
			LOG_LEVEL4("XMLDeclaration End Tag not found.");
		}

		if (!(bWellFormedXmlBeginning = (bWellFormedXmlBeginning && oXmlBuffer.readNext()==QXmlStreamReader::StartDocument )))
		{
			LOG_LEVEL4("Start Document not found.");
		}

		if (!(bWellFormedXmlBeginning = (bWellFormedXmlBeginning && oXmlBuffer.readNext()==QXmlStreamReader::StartElement )))
		{
			LOG_LEVEL4("Start Element not found.");
		}

		if (!(bWellFormedXmlBeginning = (bWellFormedXmlBeginning && (oXmlBuffer.characterOffset()>iEndOfXmlDeclaration ))))
		{
			LOG_LEVEL4("CharacterOffSet<=iEndOfXmlDeclaration.");
		}

		if (bWellFormedXmlBeginning)
		{
			LOG_LEVEL4("XML Beginning Well Formed.");

			m_strCommandName = oXmlBuffer.qualifiedName().toString();

			bCommandNameFound = (m_strCommandName.size() > 0);
		}
		else
		{
			LOG_LEVEL4("XML Beginning BAD Formed.");
		}

		if (bCommandNameFound)
		{
			LOG_LEVEL3(QString("Got new XML command name: ") + m_strCommandName)
		}
		else
		{
			LOG_LEVEL3(QString("Couldn't find XML command name\n[") + m_oBuffer + "]");
		}
	}

	if (bCommandNameFound) // Do we have the closing tag in the buffer ?
	{
		bMsgComplete = ( m_oBuffer.indexOf(GControlChannelHandler::CXMLCommandEndTag.arg(m_strCommandName)) > 0 );
	}

	return bCommandNameFound && bMsgComplete;
}

void GControlChannelHandler::Dispatch()
{
	LOG_LEVEL4("Dispatch()");

	GAbsCommand * poCommand(NULL);
	QByteArray oResponse;
	GResponseMsgData * poResponse(NULL);

	LOG_LEVEL3(QString("Got complete XML message. Dispatching![") + m_oBuffer + "]");

	try
	{
		poCommand = m_poMsgCmdDataFactory->Create(m_oBuffer);
		poCommand->Execute();
		poResponse = static_cast<GResponseMsgData *> (poCommand->GetResponse());

		if (poResponse != NULL)
		{
			LOG_LEVEL3(QString("Sending Response: ") + QString::number(poResponse->GetResponseCode()));
			oResponse = GResponseMsgDataXML::Marshall(poResponse);
			SendResponse(oResponse);
		}
	}
	catch (GException &ex)
	{
		LOG_LEVEL1(QString("Exception creating command: ") + ex.GetMessage());
		SendErrorResponse(GResponseMsgData::eParseError, ex.GetMessage());
	}
	
	if( poCommand != NULL )
	{
		delete poCommand;
		poCommand = NULL;
	}
}

void GControlChannelHandler::SendResponse(QByteArray & oResponse)
{
	LOG_LEVEL4("SendResponse()");

	if (!oResponse.isEmpty())
	{
		m_poTcpSocket->write(oResponse);
	}
}

void GControlChannelHandler::SendErrorResponse(GResponseMsgData::eResponseCode eResponseCode, QString strErrorMsg)
{
	LOG_LEVEL4("SendErrorResponse()");

	GResponseMsgData oResponse;

	oResponse.SetInterfaceVersion(GMsgCmdDataFactory::CInterfaceVersion);

	if (bCommandNameFound)
	{
		oResponse.SetCommand(m_strCommandName);
	}

	if (!strErrorMsg.isEmpty())
	{
		oResponse.SetData(strErrorMsg.toUtf8());
	}

	oResponse.SetResponseCode(eResponseCode);

	try
	{
		m_poTcpSocket->write(GResponseMsgDataXML::Marshall(&oResponse));
	}
	catch (GException &ex)
	{
		LOG_LEVEL1(QString("Exception creating command: ") + ex.GetMessage());
	}
}
