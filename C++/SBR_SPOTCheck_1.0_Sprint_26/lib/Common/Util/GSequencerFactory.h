///////////////////////////////////////////////////////////
//  GSequencerFactory.h
//  Interface of the Class GSequencerFactory
//  Created on:      17-Apr-2007 02:15:30 PM  by GMP
//
//  Modified on:     12-Aug-2009 05:55:49 PM  by GMP
//                   Support for forbidden values list of
//                   sequence IDs
//
///////////////////////////////////////////////////////////

#ifndef GSequencerFactory_h
#define GSequencerFactory_h

#include <stddef.h>
#include <map>

using std::map;
class GSequencer;

class GSequencerFactory
{
public:
					 // obtain or create new Sequencer object.
	static GSequencer* GetSequencer(int iseq, int ibasevalue = 0);

					 // release previously requested Sequencer.
	static bool        ReleaseSequencer(int iseq);

					 // return new ID
	static int         GetLowestFreeSeqId(int low = 0, int high = 255);

					 // Set forbidden values to the sequencer ID
	static bool        SetExceptions(unsigned int nExcept, int* piExceptions = NULL);

	inline static void ClearExceptions()   { SetExceptions(0); }

	static bool        IsExceptionValue(unsigned int n);

					 // destroy all the sequencer objects in the map and clear it.
	static void        EraseAll();

private:

	typedef map<int,GSequencer*>::iterator  Iterator;
	typedef map<int,GSequencer*>  SequencerMap;

	static unsigned int           m_useqQty;
	static unsigned int           m_uexcepQty;
	static int*                   m_piExceptions;
	static map<int,GSequencer*>   m_SequencerMap;
};

#endif // GSequencerFactory_h
