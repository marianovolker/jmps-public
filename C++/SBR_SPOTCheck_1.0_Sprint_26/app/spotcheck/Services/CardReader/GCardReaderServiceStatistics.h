/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GCardReaderService.h
 * @author Mariano Volker
 * @date May 09 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GCardReaderServiceStatistics_h
#define GCardReaderServiceStatistics_h 1

class GCardReaderServiceStatistics
{
	public:

		GCardReaderServiceStatistics():m_totalSwipes(0)
		{ memset(m_serviceStatistics, 0, sizeof(m_serviceStatistics) ); };

		virtual ~GCardReaderServiceStatistics(){};

		enum eTypeStatistics
		{
			ePass = 0,
			eFail,
			eNotPresent,

			eTotalStatistics = (eNotPresent+1)
		};

		enum eTrack
		{
			eTrack1 = 0,
			eTrack2,
			eTrack3,
			eTrack4,
			eChip=eTrack4,

			eTotalTracks= (eChip+1)
		};

		inline UINT32 GetStatistics(eTypeStatistics t, eTrack c) const { return m_serviceStatistics[t][c]; };
		inline void   SetStatistics(eTypeStatistics t, eTrack c, UINT32 val) { m_serviceStatistics[t][c]=val; };
		inline void   IncrementStatistics(eTypeStatistics t, eTrack c) { m_serviceStatistics[t][c]++; };

		inline UINT32 GetTotalSwipes() const { return m_totalSwipes; };
		inline void   SetTotalSwipes(UINT32 val) { m_totalSwipes=val; };
		inline void   IncrementTotalSwipes() { m_totalSwipes++; };

	protected:
		UINT32 m_serviceStatistics[eTotalStatistics][eTotalTracks];
		UINT32 m_totalSwipes;
};

#endif
