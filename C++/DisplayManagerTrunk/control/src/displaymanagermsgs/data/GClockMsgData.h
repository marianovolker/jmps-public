#ifndef CLOCK_MSG_DATA_H
#define CLOCK_MSG_DATA_H

#include "control/src/displaymanagermsgs/base/GAbsMsgData.h"

class GClockMsgData : public GAbsMsgData
{
public:
	typedef enum
	{
		eStart,
		eStop,
		ePause,
		eResume
	} eActionTypes;

	GClockMsgData();
	virtual ~GClockMsgData();

	void SetAction(QString str);
	GClockMsgData::eActionTypes GetAction();

	void SetXpos(int x_pos);
	int GetXpos();

	void SetYpos(int y_pos);
	int GetYpos();

	virtual void Validate() throw(GException);
private:
	GClockMsgData::eActionTypes m_eAction;
	int m_iXpos;
	int m_iYpos;

	/*
	 * true, if data is valid.
	 * false, if data is invalid.
	 */
	bool m_bValid;
};

#endif
