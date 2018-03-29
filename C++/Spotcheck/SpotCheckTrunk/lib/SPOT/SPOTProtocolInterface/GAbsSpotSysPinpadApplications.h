/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAbsSpotSysPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#ifndef GAbsSpotSysPinpadApplications_h
#define GAbsSpotSysPinpadApplications_h 1

#include <map>
#include <string>

#include "GTypes.h"
#include <GMutex.h>

class GAbsSpotSysPinpadApplications
{
	public:

		typedef struct itemType
		{
			std::string sNemonic;
			std::string sDescription;

			itemType(const char *s1, const char *s2)
			{
				sNemonic=s1;
				sDescription=s2;
			}
		} itemType;

				   typedef std::map<int, itemType> MapType;

						 	   virtual const char* GetStatesDescription(UINT8 u8Status) const;
						 	   virtual const char* GetStatesNemonic(UINT8 u8Status) const;

						 	   virtual const char* GetStateExtensionsDescription(UINT8 u8StatusEx) const;
						 	   virtual const char* GetStateExtensionsNemonic(UINT8 u8StatusEx) const;

						 	   virtual const char* GetErrorsDescription(UINT8 u8Error) const;
						 	   virtual const char* GetErrorsNemonic(UINT8 u8Error) const;

						 	          virtual bool SetStatesDescription(UINT8 u8StatusCode, const char* descrip);
						 	          virtual bool SetStatesExDescription(UINT8 u8StatusEx, const char* descrip);
						 	          virtual bool SetErrorsDescription(UINT8 u8Error, const char* descrip);

	protected:
												   GAbsSpotSysPinpadApplications();
										   virtual ~GAbsSpotSysPinpadApplications();

										   virtual void populateStatesMap() = 0;
										   virtual void populateStateExtensionsMap() = 0;
										   virtual void populateErrorsMap() = 0;

										   MapType mapStates;
										   MapType mapStateExtensions;
										   MapType mapErrors;
};

#endif
