/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageSpec.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGESPEC_H_
# define _UCCMESSAGESPEC_H_

# include <gvr/communication/MessageSpec.h>
# include <string>

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
       * @brief This class is the ucc message specification.
       * @see gvr.communication.MessageSpec
       * @since 0.0.0, 2015-03-02
       * @version 0.0.0, 2015-03-02
       * @author pcantarini
       */
      class UccMessageSpec : public MessageSpec<std::string>
      {
        public:

          typedef MessageSpec<std::string> MessageSpecType;

          /**
           * @brief redefine the inherited type. KeyType.
           * @see MessageSpec
           */
          typedef MessageSpecType::KeyType    KeyType;
          /**
           * @brief redefine the inherited type. KeyType.
           * @see MessageSpec
           */
          typedef MessageSpecType::BufferType BufferType;
          /**
           * @brief redefine the inherited type. KeyType.
           * @see MessageSpec
           */
          typedef MessageSpecType::SizeType   SizeType;
          /**
           * @brief redefine the inherited type. KeyType.
           * @see MessageSpec
           */
          typedef MessageSpecType::TypeId     TypeId;

          /**
           * @brief default destructor.
           */
          virtual ~UccMessageSpec (void);
      };
    }
  }
}

#endif

