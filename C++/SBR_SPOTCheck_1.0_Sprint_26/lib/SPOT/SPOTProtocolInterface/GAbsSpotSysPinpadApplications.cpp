/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAbsSpotSysPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#include "GAbsSpotSysPinpadApplications.h"

GAbsSpotSysPinpadApplications::GAbsSpotSysPinpadApplications()
{
}

GAbsSpotSysPinpadApplications::~GAbsSpotSysPinpadApplications()
{
}

const char * GAbsSpotSysPinpadApplications::GetStatesDescription(UINT8 u8Status) const
{
	int key = u8Status;
	MapType::const_iterator i = mapStates.find(key);
	if (i != mapStates.end())
	{
		itemType d = (*i).second;
		return d.sDescription.c_str();
	};
	return "";
}

const char * GAbsSpotSysPinpadApplications::GetStatesNemonic(UINT8 u8Status) const
{
	int key = u8Status;
	MapType::const_iterator i = mapStates.find(key);
	if (i != mapStates.end())
	{
		itemType d = (*i).second;
		return d.sNemonic.c_str();
	};
	return "";
}



const char * GAbsSpotSysPinpadApplications::GetStateExtensionsDescription(UINT8 u8StatusEx) const
{
	int key = u8StatusEx;
	MapType::const_iterator i = mapStateExtensions.find(key);
	if (i != mapStateExtensions.end())
	{
		itemType d = (*i).second;
		return d.sDescription.c_str();
	};
	return "";
}

const char * GAbsSpotSysPinpadApplications::GetStateExtensionsNemonic(UINT8 u8StatusEx) const
{
	int key = u8StatusEx;
	MapType::const_iterator i = mapStateExtensions.find(key);
	if (i != mapStateExtensions.end())
	{
		itemType d = (*i).second;
		return d.sNemonic.c_str();
	};
	return "";
}




const char * GAbsSpotSysPinpadApplications::GetErrorsDescription(UINT8 u8Error) const
{
	int key = u8Error;
	MapType::const_iterator i = mapErrors.find(key);
	if (i != mapErrors.end())
	{
		itemType d = (*i).second;
		return d.sDescription.c_str();
	};
	return "";
}

const char * GAbsSpotSysPinpadApplications::GetErrorsNemonic(UINT8 u8Error) const
{
	int key = u8Error;
	MapType::const_iterator i = mapErrors.find(key);
	if (i != mapErrors.end())
	{
		itemType d = (*i).second;
		return d.sNemonic.c_str();
	};
	return "";
}


bool GAbsSpotSysPinpadApplications::SetStatesDescription(UINT8 u8StatusCode, const char* descrip)
{
	int key = u8StatusCode;
	MapType::iterator i = mapStates.find(key);
	if (i != mapStates.end())
	{
		(*i).second.sDescription = descrip;
		return true;
	};
	return false;
}

bool GAbsSpotSysPinpadApplications::SetStatesExDescription(UINT8 u8StatusExCode, const char* descrip)
{
	int key = u8StatusExCode;

	MapType::iterator i = mapStateExtensions.find(key);
	if (i != mapStateExtensions.end())
	{
		(*i).second.sDescription = descrip;
		return true;
	};

	return false;
}

bool GAbsSpotSysPinpadApplications::SetErrorsDescription(UINT8 u8ErrorsCode, const char* descrip)
{
	int key = u8ErrorsCode;
	MapType::iterator i = mapErrors.find(key);
	if (i != mapErrors.end())
	{
		(*i).second.sDescription = descrip;
		return true;
	};
	return false;
}

