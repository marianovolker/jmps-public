/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IUccClassifyMessage.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _IUCCCLASSIFYMESSAGE_H_
# define _IUCCCLASSIFYMESSAGE_H_

# include <gvr/communication/IClassifyMessage.h>
# include <gvr/communication/ucc/UccMessageSpec.h>
# include <gvr/utils/Json.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Ucc Domain
      namespace ucc
      {

        /*
         * @brief This class is the base class for all ucc message classifiers.
         * @see gvr.communication.ucc.IUccClassifyMessage
         * @since 0.0.0
         * @author pcantarini
         */
        class IUccClassifyMessage : public IClassifyMessage<UccMessageSpec>
        {
        public:
          /**
           * @brief redefine the inherited type. Spec.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<UccMessageSpec>::Spec          Spec;

          /**
           * @brief redefine the inherited type. KeyType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<UccMessageSpec>::KeyType       KeyType;

          /**
           * @brief redefine the inherited type. BufferType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<UccMessageSpec>::BufferType    BufferType;

          /**
           * @brief redefine the inherited type. SizeType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<UccMessageSpec>::SizeType      SizeType;

          /**
           * @brief redefine the inherited type. TypeId.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<UccMessageSpec>::TypeId        TypeId;

          /**
           * @brief redefine the inherited type. IntegrityType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<UccMessageSpec>::IntegrityType IntegrityType;

          typedef gvr::utils::Json                     JsonType; /// @brief The Message Json field;

          static unsigned int const CSizeFiledSize;    /** @brief Ucc Message binary Size field size. */
          static unsigned int const CFlagFiledSize;    /** @brief Ucc Message binary Flag field size. */
          static unsigned int const CBinaryFiledsSize; /** @brief Ucc Message all binary fields size summarized. */

          /**
           * @brief Parse the given buffer and fill the UccStructure fields with the obtained data.
           *
           * @param The given buffer to parse.
           * @param The message parsed size including the binary fields.
           * @param The message structure size besides the binary fields.
           * @param The message internal flag.
           * @param The UccStructure to hold the parsed data.
           *
           * @return True if parse process is successful. False otherwise.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.BufferType
           * @see gvr.communication.ucc.AbsUccStructureHeaderBase
           *
           * @since 0.0.0
           */
          static IntegrityType parseToStructure (BufferType const &, SizeType &, SizeType &, unsigned char &, JsonType &);

          /**
           * @brief default destructor.
           */
          virtual ~IUccClassifyMessage (void) = 0;
        };
      }
    }
  }

#endif
