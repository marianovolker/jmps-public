///////////////////////////////////////////////////////////
//  GSequencer.h
//  Interface of the Class GSequencer
//  Created on:      17-Apr-2007 08:50:37 AM
///////////////////////////////////////////////////////////

#ifndef GSequencer_h
#define GSequencer_h

#include <set>

using namespace std;

class GSequencer
{
public:
				GSequencer(int iStart = 0);

	inline int    GetCurrent();
	int           GetNext();
	bool          GiveBack(int iUsedValue);
	void          Reset();

	private:
	friend class GSequencerFactory;
	typedef set<int>::iterator Iterator;
	int           m_iBaseValue;
	int           m_iLastValue;
	set<int>      m_ReleasedSet;
};


inline int GSequencer::GetCurrent()
{
	return m_iLastValue;
}



#endif // GSequencer

