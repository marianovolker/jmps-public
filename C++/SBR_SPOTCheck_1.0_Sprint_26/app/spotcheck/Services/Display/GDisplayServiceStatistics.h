/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GDisplayServiceStatistics.h
 * @author Mariano Volker
 * @date June 03 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GDisplayServiceStatistics_h
#define GDisplayServiceStatistics_h 1

class GDisplayServiceStatistics
{
	public:

		GDisplayServiceStatistics():m_totalTests(0)
		{ memset(m_serviceStatistics, 0, sizeof(m_serviceStatistics) ); };

		virtual ~GDisplayServiceStatistics(){};

		enum eTypeStatistics
		{
			ePass = 0,
			eFail,

			eTotalTypeStatistics = (eFail+1)
		};

		enum eTests
		{
			eTest1_White_Color = 0	,
			eTest2_Black_Color		,
			eTest3_Red_Color		,
			eTest4_Green_Color		,
			eTest5_Blue_Color		,

			eTotalTests= (eTest5_Blue_Color+1)
		};

		       inline UINT32 GetStatistics (eTypeStatistics t, eTests c) const { return m_serviceStatistics[t][c]; }
		         inline void SetStatistics (eTypeStatistics t, eTests c, UINT32 val) { m_serviceStatistics[t][c]=val; }
		         inline void IncrementStatistics (eTypeStatistics t, eTests c) { m_serviceStatistics[t][c]++; m_totalTests++; }

		       inline UINT32 GetTotalTests () const { return m_totalTests; }
		         inline void SetTotalTests (UINT32 val) { m_totalTests=val; }

	     static const char * GetTestLabel (eTests t) { return mst_arypcTestLabel[t]; }

	protected:
		 static const char * mst_arypcTestLabel[eTotalTests];
		              UINT32 m_serviceStatistics[eTotalTypeStatistics][eTotalTests];
		              UINT32 m_totalTests;
};

#endif
