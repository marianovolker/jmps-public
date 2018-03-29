/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsCrlMessage.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSCRLMESSAGE_H_
# define _ABSCRLMESSAGE_H_

# include <gvr/communication/crl/message/base/ICrlMessage.h>
# include <gvr/utils/Json.h>

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
         * @brief This class is the base class for all crl messages.
         * @see gvr.communication.crl.ICrlMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class AbsCrlMessage : public ICrlMessage
        {
        public:
          static KeyType const CDefaultKey; /** @brief The dafault key. */

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~AbsCrlMessage (void) = 0;

          /**
           * @brief Retrieves the current Message key identification, which in many protocols is the sequence number.
           *
           * @return The current Message key identification.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.KeyType
           *
           * @since 0.0.0
           */
          virtual KeyType getKey (void) const;

          /**
           * @brief Retrieves the current size which could be obtained if Message would be serialized.
           *
           * @return The current Message size if serialized.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.SizeType
           *
           * @since 0.0.0
           */
          virtual SizeType size (void) const;

          /**
           * @brief Retrieves the serialized Message into a new buffer.
           *
           * @return The serialized Message to a buffer.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.BufferType
           *
           * @since 0.0.0
           */
          virtual BufferType serialize (void) const;

          /**
           * @brief Parse the given buffer and fill the Message fields with the obtained data.
           *
           * @param The given buffer to parse.
           *
           * @return True if parse process is scrlessful. False otherwise.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.BufferType
           *
           * @since 0.0.0
           */
          virtual bool parse (BufferType const &);

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
           * @brief Retrieves the Message Internal flag value.
           *
           * @return The current Message internal flag.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual unsigned char getInternalFlag (void) const;

          /**
           * @brief Check if message is an crl internal message.
           *
           * @return True if message is an crl internal message.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual bool isInternalMessage (void) const;

          /**
           * @brief Retrieves the Json field.
           *
           * @since 0.0.0
           **/
          virtual AbsJsonType const & getJson (void) const;

          /**
           * @brief Retrieves the Json field.
           *
           * @since 0.0.0
           **/
          virtual AbsJsonType & getJson (void);

          /**
           * @brief Sets the Json field.
           *
           * @param Json field content.
           *
           * @since 0.0.0
           **/
          virtual void setJson (AbsJsonType const &);

        protected:
          static unsigned int const CSizeFiledSize;    /** @brief Crl Message binary Size field size. */
          static unsigned int const CFlagFiledSize;    /** @brief Crl Message binary Flag field size. */
          static unsigned int const CBinaryFiledsSize; /** @brief Crl Message all binary fields size summarized. */

          static char const * CEmptyString; /** @brief A reference to an empty string. */

          /**
           * @brief Constructor.
           * 
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          AbsCrlMessage (unsigned char);

          /**
           * @brief Sets the internal flag value.
           *
           * @param The internal flag new value.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void private_setInternalFlag (unsigned char);

        protected:
          unsigned char m_flag; /** @brief The internal flag value. */
          JsonType      m_json; /** @brief The Json message field. */
        };
      }
    }
  }

#endif
