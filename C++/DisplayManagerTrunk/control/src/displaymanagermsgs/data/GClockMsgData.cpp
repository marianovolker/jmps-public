#include "GClockMsgData.h"

GClockMsgData::GClockMsgData()
{
}

GClockMsgData::~GClockMsgData()
{
}

void GClockMsgData::SetAction(QString str)
{
	m_bValid = true;

	if (!str.compare(QString("start")))
	{
		m_eAction = GClockMsgData::eStart;
	}
	else if (!str.compare(QString("stop")))
	{
		m_eAction = GClockMsgData::eStop;
	}
	else if (!str.compare(QString("pause")))
	{
		m_eAction = GClockMsgData::ePause;
	}
	else if (!str.compare(QString("resume")))
	{
		m_eAction = GClockMsgData::eResume;
	}
	else
	{
		m_bValid = false;
	}
}

GClockMsgData::eActionTypes GClockMsgData::GetAction()
{
	return m_eAction;
}

void GClockMsgData::SetXpos(int x_pos)
{
	m_iXpos = x_pos;
}

int GClockMsgData::GetXpos()
{
	return m_iXpos;
}

void GClockMsgData::SetYpos(int y_pos)
{
	m_iYpos = y_pos;
}

int GClockMsgData::GetYpos()
{
	return m_iYpos;
}

void GClockMsgData::Validate() throw(GException)
{
	if (!m_bValid)
	{
		throw GEXCEPTION("Invalid tag [action].");
	}

	if (!(m_iXpos >= 1 && m_iXpos <= 320))
	{
		throw GEXCEPTION("Invalid tag [x_loc].");
	}

	if (!(m_iYpos >= 1 && m_iYpos <= 240))
	{
		throw GEXCEPTION("Invalid tag [y_loc].");
	}
}
