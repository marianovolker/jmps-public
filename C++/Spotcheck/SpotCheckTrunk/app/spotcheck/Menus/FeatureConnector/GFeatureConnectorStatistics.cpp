/*!
 * © 2015 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GFeatureConnectorStatistics.cpp
 * @author Sayyed Mohammad
 * @date Feb 23 2015
 * @copyright © 2015 Gilbarco Inc. Confidential and Propietary
 */

#include <unistd.h>
#include <cstring>

#include "Common/Util/DEBUGOUT.h"

#include "GFeatureConnectorStatistics.h"

GFeatureConnectorStatistics* GFeatureConnectorStatistics::m_pFeatureConnectorStats=NULL;

const char * GFeatureConnectorStatistics::mst_arypcFCRLabel[]=
{
	"No Test",
	"Error could not test",
	"Fail",
	"Pass"
};
