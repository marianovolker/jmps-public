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

#include <assert.h>
#include "Common/Util/GSequencerFactory.h"
#include "Common/Util/GSequencer.h"
#include "Common/Util/DEBUGOUT.h"


// Class GSequencerFactory static data member values

unsigned int                      GSequencerFactory::m_useqQty = 0;
unsigned int                      GSequencerFactory::m_uexcepQty = 0;
int*                              GSequencerFactory::m_piExceptions = NULL;
GSequencerFactory::SequencerMap   GSequencerFactory::m_SequencerMap;


// Class GSequencerFactory implementation

GSequencer*  GSequencerFactory::GetSequencer(int iseq, int ibasevalue)
{
	if(m_useqQty) // if the map isn't empty
	{
		Iterator it = m_SequencerMap.find(iseq), itend = m_SequencerMap.end();
		if(it != itend)
		{
			return it->second; // sequencer already created
		}
	}

	GSequencer* pSeq = new GSequencer(ibasevalue);
	assert(pSeq);
	m_SequencerMap[iseq] = pSeq;
	m_useqQty++;
	return pSeq;
}

bool  GSequencerFactory::ReleaseSequencer(int iseq)
{
	Iterator it = m_SequencerMap.find(iseq);
	Iterator itend = m_SequencerMap.end();
	if(it == itend)
		return false; // sequencer not found

	delete it->second;
	m_SequencerMap.erase(it);
	m_useqQty--;
}

int  GSequencerFactory::GetLowestFreeSeqId(int low /* = 0 */,
                                           int high /* = 255 */)
{
	if(!m_useqQty) return low; // Factory is empty, "low" is the first value.

	int SeqId = low;
	Iterator it = m_SequencerMap.begin();
	Iterator itend = m_SequencerMap.end();
	while( it != itend )
	{
		if(IsExceptionValue(SeqId))
		{
			++SeqId; // discarded value because forbidden
			continue;
		}

		if( SeqId < it->first )
		{
			return SeqId;
		}
		else if( SeqId == it->first )
		{
			++it;
			++SeqId;
		}
		else if( SeqId > it->first )
		{
			it = m_SequencerMap.find(SeqId);
		}
	}

	if( it == itend && SeqId <= high )
	{
		while(IsExceptionValue(SeqId))
		{
			++SeqId; // discarded value because forbidden
		}
		if(SeqId <= high)
		{
			return SeqId;
		}
	}

	return low - 1; // no more available IDs
}

bool GSequencerFactory::SetExceptions(unsigned int nExcept,
                                      int* piExceptions /* == NULL */)
{
	if(!nExcept)
	{
		m_piExceptions = NULL; // no exceptions at all for the sequencer ID values
		return false;
	}

	if(piExceptions== NULL) // no way to read the exception values
		return false;

	if( *(piExceptions + nExcept) != 0)
		return false; // bad terminated user array

	m_uexcepQty = nExcept; // exception values has been set
	m_piExceptions = piExceptions;
	return true;
}

bool GSequencerFactory::IsExceptionValue(unsigned int n)
{
	if(!m_uexcepQty || !m_piExceptions)
		return false; // no exceptions at all

	for(unsigned int u = 0; u < m_uexcepQty; u++)
		if( n == *(m_piExceptions + u) )
			return true;

	return false;
}

void GSequencerFactory::EraseAll()
{
	for(Iterator it = m_SequencerMap.begin(), itend = m_SequencerMap.end();
			   it != itend; it++)
		if(it->second)
			delete it->second;

	m_SequencerMap.clear();
	m_useqQty = 0;
}
