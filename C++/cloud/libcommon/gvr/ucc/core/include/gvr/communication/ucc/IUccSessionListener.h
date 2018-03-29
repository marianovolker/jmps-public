/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IUccSessionListener.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _IUCCSESSIONLISTENER_H_
# define _IUCCSESSIONLISTENER_H_

# include <gvr/communication/ISessionListener.h>
# include <gvr/communication/ucc/UccMessageSpec.h>

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
         * @brief This class is the interface class for all ucc session listeners.
         * @see gvr.communication.ISessionListener
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class IUccSessionListener : public ISessionListener<UccMessageSpec>
        {
        public:
          /**
           * @brief redefine the inherited types.
           * @see ISessionListener types
           */
          typedef ISessionListener<UccMessageSpec>::Spec         Spec;
          typedef ISessionListener<UccMessageSpec>::KeyType      KeyType;
          typedef ISessionListener<UccMessageSpec>::BufferType   BufferType;
          typedef ISessionListener<UccMessageSpec>::SizeType     SizeType;
          typedef ISessionListener<UccMessageSpec>::TypeId       TypeId;
          typedef ISessionListener<UccMessageSpec>::ISessionType ISessionType;
          typedef ISessionListener<UccMessageSpec>::StatusType   StatusType;
          typedef ISessionListener<UccMessageSpec>::ErrorType    ErrorType;

          /**
           * @brief default destructor.
           */
          virtual ~IUccSessionListener (void) = 0;
        };
      }
    }
  }

#endif
