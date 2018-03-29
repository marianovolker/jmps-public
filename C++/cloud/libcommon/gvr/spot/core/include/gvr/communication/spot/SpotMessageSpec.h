/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageSpec.h
 * @author mgrieco
 * @date Friday, December 22, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGESPEC_H_
# define _SPOTMESSAGESPEC_H_

# include <gvr/communication/MessageSpec.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      /**
       * @brief This class is the base class for all spot messages that share the same spot message specification.
       * @see gvr.communication.MessageSpec
       * @since 0.0.0, 2014-12-16
       * @version 0.0.0, 2014-12-16
       * @author mgrieco
       */
      class SpotMessageSpec : public MessageSpec<std::size_t>
      {
        public:

          typedef MessageSpec<std::size_t> MessageSpecType;

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
          virtual ~SpotMessageSpec (void);
      };
    }
  }
}

#endif

