/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ICrlClassifyMessage.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ICRLCLASSIFYMESSAGE_H_
# define _ICRLCLASSIFYMESSAGE_H_

# include <gvr/communication/IClassifyMessage.h>
# include <gvr/communication/crl/CrlMessageSpec.h>
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

        /*
         * @brief This class is the base class for all crl message classifiers.
         * @see gvr.communication.crl.ICrlClassifyMessage
         * @since 0.0.0
         * @author pcantarini
         */
        class ICrlClassifyMessage : public IClassifyMessage<CrlMessageSpec>
        {
        public:
          /**
           * @brief redefine the inherited type. Spec.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<CrlMessageSpec>::Spec          Spec;

          /**
           * @brief redefine the inherited type. KeyType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<CrlMessageSpec>::KeyType       KeyType;

          /**
           * @brief redefine the inherited type. BufferType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<CrlMessageSpec>::BufferType    BufferType;

          /**
           * @brief redefine the inherited type. SizeType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<CrlMessageSpec>::SizeType      SizeType;

          /**
           * @brief redefine the inherited type. TypeId.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<CrlMessageSpec>::TypeId        TypeId;

          /**
           * @brief redefine the inherited type. IntegrityType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<CrlMessageSpec>::IntegrityType IntegrityType;

          typedef gvr::utils::Json                     JsonType; /// @brief The Message Json field;

          static unsigned int const CSizeFiledSize;    /** @brief Crl Message binary Size field size. */
          static unsigned int const CFlagFiledSize;    /** @brief Crl Message binary Flag field size. */
          static unsigned int const CBinaryFiledsSize; /** @brief Crl Message all binary fields size summarized. */

          /**
           * @brief Parse the given buffer and fill the CrlStructure fields with the obtained data.
           *
           * @param The given buffer to parse.
           * @param The message parsed size including the binary fields.
           * @param The message structure size besides the binary fields.
           * @param The message internal flag.
           * @param The CrlStructure to hold the parsed data.
           *
           * @return True if parse process is scrlessful. False otherwise.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.BufferType
           * @see gvr.communication.crl.AbsCrlStructureHeaderBase
           *
           * @since 0.0.0
           */
          static IntegrityType parseToStructure (BufferType const &, SizeType &, SizeType &, unsigned char &, JsonType &);

          /**
           * @brief default destructor.
           */
          virtual ~ICrlClassifyMessage (void) = 0;
        };
      }
    }
  }

#endif
