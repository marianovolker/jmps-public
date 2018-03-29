/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GContactlessReaderService.h
 * @author Mariano Volker
 * @date Jul 04 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GContactlessReaderServiceStatistics_h
#define GContactlessReaderServiceStatistics_h 1

class GContactlessReaderServiceStatistics
{
	public:

		GContactlessReaderServiceStatistics():m_totalWaves(0)
		{ memset(m_serviceStatistics, 0, sizeof(m_serviceStatistics) ); };

		virtual ~GContactlessReaderServiceStatistics(){};

		enum eTypeStatistics
		{
			ePass = 0,
			eFail,

			eTotalStatistics = (eFail+1)
		};

		inline UINT32 GetStatistics(eTypeStatistics t) const 		{ return m_serviceStatistics[t]; };
		inline void   SetStatistics(eTypeStatistics t, UINT32 val) 	{ m_serviceStatistics[t]=val; };
		inline void   IncrementStatistics(eTypeStatistics t) 		{ m_serviceStatistics[t]++; };

		inline UINT32 GetTotalWaves() const 		{ return m_totalWaves; };
		inline void   SetTotalWaves(UINT32 val) 	{ m_totalWaves=val; };
		inline void   IncrementTotalWaves() 		{ m_totalWaves++; };

	protected:
		UINT32 m_serviceStatistics[eTotalStatistics];
		UINT32 m_totalWaves;
};

#endif
