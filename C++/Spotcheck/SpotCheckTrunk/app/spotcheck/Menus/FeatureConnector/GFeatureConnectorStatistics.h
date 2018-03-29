/*!
 * © 2015 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GFeatureConnectorStatistics.h
 * @author Sayyed Mohammad
 * @date Feb 23 2015
 * @copyright © 2015 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GFeatureConnectorStatistics_h
#define GFeatureConnectorStatistics_h 1

class GFeatureConnectorStatistics
{
	public:

		GFeatureConnectorStatistics():m_eFCExternalResult(eFCRInitial),
		m_eFCIntNSResult(eFCRInitial)
		{
		};

		virtual ~GFeatureConnectorStatistics(){};


		enum eFCResult  //Used for Feature Connector Results
			{
				eFCRInitial =0,
				eFCRNoTest,
				eFCRFail,
				eFCRPass,

				eFCRLast = eFCRPass
			};

		       inline const char* GetExternalStatistics () const { return mst_arypcFCRLabel[m_eFCExternalResult]; }
		       inline const char* GetInternalNsStatistics () const { return mst_arypcFCRLabel[m_eFCIntNSResult]; }
		       inline void SetExternalStatistics (eFCResult t) { m_eFCExternalResult = t; }
		       inline void SetInternalNsStatistics (eFCResult t) { m_eFCIntNSResult = t; }

		inline static GFeatureConnectorStatistics* GetInstance()
		      {
		    	 if(!m_pFeatureConnectorStats)
		    	 {
		    		 m_pFeatureConnectorStats = new GFeatureConnectorStatistics();
		    	 }

		    	 return m_pFeatureConnectorStats;
		      }

		inline static void DestroyInstance()
		      {
		    	  delete m_pFeatureConnectorStats;
		    	  m_pFeatureConnectorStats=NULL;
		      }

	protected:

	 static const char * mst_arypcFCRLabel[eFCRLast+1];
			 eFCResult   m_eFCExternalResult;
			 eFCResult   m_eFCIntNSResult;
	static GFeatureConnectorStatistics* m_pFeatureConnectorStats;

};

#endif
