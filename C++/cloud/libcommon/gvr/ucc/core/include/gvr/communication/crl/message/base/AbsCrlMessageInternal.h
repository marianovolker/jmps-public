/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsCrlMessageInternal.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSCRLMESSAGEINTERNAL_H_
# define _ABSCRLMESSAGEINTERNAL_H_

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
         * @brief This class is the base class for all internal crl messages.
         * @see gvr.communication.crl.ICrlMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class AbsCrlMessageInternal : public AbsCrlMessage
        {
        public:
          static char const * COperationName; /** @brief Crl Operation field name. */
          static char const * CPayloadName;   /** @brief Crl Payload field name. */

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~AbsCrlMessageInternal (void) = 0;

          /**
           * @brief validate the Message fields.
           *
           * @return True if parse process is scrlessful. False otherwise.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual bool validate (void);

          /**
           * @brief Gets the expected message operation for this message.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getOperationExpected (void) const;

          /**
           * @brief Retrieves the Message Operation.
           *
           * @return The current Message operation.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getOperation (void) const;

          /**
           * @brief Resets message header to the expected values if missing.
           *
           * @param force boolean value which forces reseting messageId and deviceId. Default value is false.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void resetHeader (bool);

          /**
           * @brief Retrieves the payload field inside the  Json field.
           *
           * @since 0.0.0
           **/
          virtual JsonType const getPayload (void) const;

          /**
           * @brief Sets the payload field inside the Json field.
           *
           * @param Json field content.
           *
           * @since 0.0.0
           **/
          virtual void setPayload (AbsJsonType const &);

          /**
           * @brief Removes the payload field inside the Json field.
           *
           * @since 0.0.0
           **/
          virtual void removePayload (void);

        protected:
          /**
           * @brief Constructor.
           * 
           * @param Reference to the operation constant string.
           */
          AbsCrlMessageInternal (char const *);

        protected:
          std::string const m_kOperation;
        };
      }
    }
  }

#endif
