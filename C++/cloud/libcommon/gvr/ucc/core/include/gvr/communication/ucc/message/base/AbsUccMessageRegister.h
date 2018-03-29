/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageRegister.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSUCCMESSAGEREGISTER_H_
# define _ABSUCCMESSAGEREGISTER_H_

#include <gvr/communication/ucc/message/base/AbsUccMessage.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Ucc Domain
      namespace ucc
      {
        /**
         * @brief This class is the base class for all cloud ucc messages.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class AbsUccMessageRegister : public AbsUccMessage
        {
        public:
          /**
           * @brief Internal flag constant values.
           */
          static unsigned char const CInteralFlagRegisterMessage;

          /**
           * @brief Destructor
           * @since 0.0.0
           */
          virtual ~AbsUccMessageRegister (void) = 0;

          /**
           * @brief validate the Message fields.
           *
           * @return True if parse process is successful. False otherwise.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual bool validate (void);

          /**
           * @brief Following method retrieves the current instance Message class type identification.
           *
           * @return The current Message class type identification.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.TypeId
           *
           * @since 0.0.0
           */
          static  TypeId GetTypeId (void);

        protected:
          /**
           * @brief Constructor.
           * 
           * @param Reference to the operation constant string.
           */
          AbsUccMessageRegister (void);
        };
      }
    }
  }

#endif
