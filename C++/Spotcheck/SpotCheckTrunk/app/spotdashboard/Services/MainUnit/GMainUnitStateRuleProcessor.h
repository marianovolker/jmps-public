/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GMainUnitStateRuleProcessor.h
 * @author Guillermo Paris
 * @date Oct 07 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GMainUnitStateRuleProcessor_h
#define GMainUnitStateRuleProcessor_h 1

#include "Services/Common/GAbsStateRuleProcessor.h"

class GMainUnitStateRuleProcessor: public GAbsStateRuleProcessor
{
	public:

					 GMainUnitStateRuleProcessor();
			 virtual ~GMainUnitStateRuleProcessor();

		virtual void ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);
		virtual void ProcessServiceNotification(UINT8 u8Ack);

	protected:

		virtual bool AddNewDescription(eCodeType eType, UINT8 u8Code);
};


#endif
