/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ICrlSessionListener.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ICRLSESSIONLISTENER_H_
# define _ICRLSESSIONLISTENER_H_

# include <gvr/communication/ISessionListener.h>
# include <gvr/communication/crl/CrlMessageSpec.h>

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
         * @brief This class is the interface class for all Crl session listeners.
         * @see gvr.communication.ISessionListener
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class ICrlSessionListener : public ISessionListener<CrlMessageSpec>
        {
        public:
          /**
           * @brief redefine the inherited types.
           * @see ISessionListener types
           */
          typedef ISessionListener<CrlMessageSpec>::Spec         Spec;
          typedef ISessionListener<CrlMessageSpec>::KeyType      KeyType;
          typedef ISessionListener<CrlMessageSpec>::BufferType   BufferType;
          typedef ISessionListener<CrlMessageSpec>::SizeType     SizeType;
          typedef ISessionListener<CrlMessageSpec>::TypeId       TypeId;
          typedef ISessionListener<CrlMessageSpec>::ISessionType ISessionType;
          typedef ISessionListener<CrlMessageSpec>::StatusType   StatusType;
          typedef ISessionListener<CrlMessageSpec>::ErrorType    ErrorType;

          /**
           * @brief default destructor.
           */
          virtual ~ICrlSessionListener (void) = 0;
        };
      }
    }
  }

#endif
