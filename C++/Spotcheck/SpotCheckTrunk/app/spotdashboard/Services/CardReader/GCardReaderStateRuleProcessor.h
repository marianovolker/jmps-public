/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GCardReaderStateRuleProcessor.h
 * @author Mariano Volker
 * @date Oct 01 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GCardReaderStateRuleProcessor_h
#define GCardReaderStateRuleProcessor_h 1

#include "Services/Common/GAbsStateRuleProcessor.h"

class GCardReaderStateRuleProcessor: public GAbsStateRuleProcessor
{
	public:

					 GCardReaderStateRuleProcessor();
			 virtual ~GCardReaderStateRuleProcessor();

		virtual void ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);
		virtual void ProcessServiceNotification(UINT8 u8Ack);

	protected:

		virtual bool AddNewDescription(eCodeType eType, UINT8 u8Code);
};


#endif
