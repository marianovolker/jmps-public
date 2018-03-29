/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSecurityModuleStateRuleProcessor.h
 * @author Guillermo Paris
 * @date Oct 08 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GSecurityModuleStateRuleProcessor_h
#define GSecurityModuleStateRuleProcessor_h 1

#include "Services/Common/GAbsStateRuleProcessor.h"

class GSecurityModuleStateRuleProcessor: public GAbsStateRuleProcessor
{
	public:

					 GSecurityModuleStateRuleProcessor();
			 virtual ~GSecurityModuleStateRuleProcessor();

		virtual void ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);
		virtual void ProcessServiceNotification(UINT8 u8Ack);

	protected:

		virtual bool AddNewDescription(eCodeType eType, UINT8 u8Code);
};


#endif
