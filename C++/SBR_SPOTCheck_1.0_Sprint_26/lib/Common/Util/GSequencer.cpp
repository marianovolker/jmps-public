///////////////////////////////////////////////////////////
//  GSequencer.cpp
//  Implementation of the Class GSequencer
//  Created on:      17-Apr-2007 08:51:58 AM
///////////////////////////////////////////////////////////

#include "Common/Util/GSequencer.h"
#include "Common/Util/DEBUGOUT.h"


GSequencer::GSequencer(int iStart /* = 0 */)
  :m_iBaseValue(iStart - 1), m_iLastValue(iStart - 1)
{
}

int GSequencer::GetNext()
{
	int iret;

	if( m_ReleasedSet.size() > 0 )
	{
		if( m_ReleasedSet.size() == (m_iLastValue - m_iBaseValue) )
		{
			Reset(); // all the values generated were released
			iret = ++m_iLastValue;
		}
		else
		{
			Iterator iter = m_ReleasedSet.begin();
			iret = *iter;
			m_ReleasedSet.erase(iter);
		}
	}
	else
	{
		iret = ++m_iLastValue;
	}
	return iret;
}

bool GSequencer::GiveBack(int iUsedValue)
{
	bool blret = false;
	if(m_ReleasedSet.find(iUsedValue) == m_ReleasedSet.end()) // the released value wasn't previously released
	{
		m_ReleasedSet.insert(iUsedValue);
		blret = true;
	}
	return blret;
}

void GSequencer::Reset()
{
	m_ReleasedSet.clear();
	m_iLastValue = m_iBaseValue;
}
