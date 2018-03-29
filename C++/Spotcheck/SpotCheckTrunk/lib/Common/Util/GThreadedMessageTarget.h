// GThreadedMessageTarget.h

#ifndef GThreadedMessageTarget_h
#define GThreadedMessageTarget_h 1


// GMessageTarget
#include "Common/MessageAdmin/GMessageTarget.h"
// GMessage
#include "Common/MessageAdmin/GMessage.h"
// GThreadedQueue
#include "Common/Util/GThreadedQueue.h"
// GQueue
#include "Common/Util/GQueue.h"
// GMessageFactory
#include "Common/MessageAdmin/GMessageFactory.h"


// GMessageQueueType

typedef GQueue< GMessage > GMessageQueueType;


// GMessageQueueThread
//
// Instantiation of GThreadedQueue template with formal parameters GMessage and
// GMessageQueueType.  This instantiated class is created so that a GCardReader
// may be derived from it.  This allows the card reader to process outgoing
// calls without holding up the MessageAdmin.

typedef GThreadedQueue< GMessage,GMessageQueueType > GMessageQueueThread;


//  GThreadedMessageTarget
//
//	Service objects can inherit off this class instead of the GMessageTarget to
//	get access to the MessageAdministrator.  The main benifit of doing this is
//	to improve the MessageAdmin's performance by not blocking it's thread during
//	service object processing tasks.

class GThreadedMessageTarget : public GMessageQueueThread, public GMessageTarget
{
public:
      //	This constructor is used to instantiate a GThreadedQueue object. It as
      //	services as the default constructor.
      GThreadedMessageTarget (UINT32 uiThreadID = 0)
		: GMessageQueueThread(uiThreadID)
      {
      }

      //	Purpose: Standardized message handler. Receives messages from the LON
      //	administrator and places them in the threaded queue for processing.  This
      //	process prevents excessive blocking of the MessageAdmin.
      //
      //	Inputs: pMessage - pointer to a GMessage
      //
      //	Outputs: None
      //
      //	Returns: None
      void MessageHandler (GMessage* pMessage)
      {
		// Clone the GMessage and push it on the queue
		Push(m_pFactory->CreateMessage(*pMessage));
      }

private:
      // Prevent object copy.
      GThreadedMessageTarget(const GThreadedMessageTarget &right);
      const GThreadedMessageTarget & operator=(const GThreadedMessageTarget &right);

};

#endif
