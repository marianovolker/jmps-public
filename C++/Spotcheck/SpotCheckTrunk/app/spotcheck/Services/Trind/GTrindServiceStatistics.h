/*!
 * © 2015 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GTrindServiceStatistics.h
 * @author Sayyed Mohammad
 * @date Feb 24 2015
 * @copyright © 2015 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GTrindServiceStatistics_h
#define GTrindServiceStatistics_h 1

class GTrindServiceStatistics
{
	public:

		GTrindServiceStatistics():
		m_eTRLEDStatistics(eTRInitial)
		{ memset(m_serviceTagStatistics, 0, sizeof(m_serviceTagStatistics) ); };

		virtual ~GTrindServiceStatistics(){};


		enum eTagResult
		{
			eTrPass,
			eTrFail,

			eTrTotal = eTrFail +1,
		};

		enum eTRLEDResult  //Used for Trind LED Results
		{
			eTRInitial,
			eTRPass,
			eTRFail,

			eTRTotal = eTRFail +1
		};

		inline UINT32 GetTagStatistics(eTagResult t) const { return m_serviceTagStatistics[t]; };
		inline void   IncrementTagCount(eTagResult t) { m_serviceTagStatistics[t]++; };
		inline void   UpdateLEDStatistics(eTRLEDResult t) {m_eTRLEDStatistics = t;};

		int GetLEDStatistics() { return (int)m_eTRLEDStatistics; } ;


	protected:
		UINT32 m_serviceTagStatistics[eTrTotal];
		eTRLEDResult m_eTRLEDStatistics;
};

#endif
