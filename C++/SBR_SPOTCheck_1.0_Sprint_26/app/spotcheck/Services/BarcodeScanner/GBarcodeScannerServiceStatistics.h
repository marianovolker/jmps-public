/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GBarcodeScannerService.h
 * @author Mariano Volker
 * @date Jul 16 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GBarcodeScannerServiceStatistics_h
#define GBarcodeScannerServiceStatistics_h 1

class GBarcodeScannerServiceStatistics
{
	public:

		GBarcodeScannerServiceStatistics():m_totalReadings(0)
		{ memset(m_serviceStatistics, 0, sizeof(m_serviceStatistics) ); };

		virtual ~GBarcodeScannerServiceStatistics(){};

		enum eTypeStatistics
		{
			ePass = 0,
			eFail,

			eTotalStatistics = (eFail+1)
		};

		inline UINT32 GetStatistics(eTypeStatistics t) const 		{ return m_serviceStatistics[t]; };
		inline void   SetStatistics(eTypeStatistics t, UINT32 val) 	{ m_serviceStatistics[t]=val; };
		inline void   IncrementStatistics(eTypeStatistics t) 		{ m_serviceStatistics[t]++; };

		inline UINT32 GetTotalReadings() const 		{ return m_totalReadings; };
		inline void   SetTotalReadings(UINT32 val) 	{ m_totalReadings=val; };
		inline void   IncrementTotalReadings() 		{ m_totalReadings++; };

	protected:
		UINT32 m_serviceStatistics[eTotalStatistics];
		UINT32 m_totalReadings;
};

#endif
