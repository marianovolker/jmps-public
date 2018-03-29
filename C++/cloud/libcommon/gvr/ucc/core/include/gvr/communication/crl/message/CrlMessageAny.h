/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageAny.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLMESSAGEANY_H_
# define _CRLMESSAGEANY_H_

#include <gvr/communication/crl/message/base/AbsCrlMessage.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Crl Domain
      namespace crl
      {
        /**
         * @brief This class is the crl message any.
         * @see gvr.communication.crl.ICrlMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class CrlMessageAny : public AbsCrlMessage
        {
        public:
          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~CrlMessageAny (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          CrlMessageAny (void);

          /**
           * @brief Sets the internal flag value.
           *
           * @param The internal flag new value.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void setInternalFlag (unsigned char);

          /**
           * @brief Following methods Retrieves the current instance Message class type identification.
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
          virtual TypeId getTypeId (void) const;
        };
      }
    }
  }

#endif
