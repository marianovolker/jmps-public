/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ISpotSessionListener.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ISPOTSESSIONLISTENER_H_
# define _ISPOTSESSIONLISTENER_H_

# include <gvr/communication/ISessionListener.h>
# include <gvr/communication/spot/SpotMessageSpec.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class ISpotSessionListener : public ISessionListener<SpotMessageSpec>
        {
          public:

            /**
             * @brief redefine the inherited types.
             * @see ISessionListener types
             */
            typedef ISessionListener<SpotMessageSpec>::Spec         Spec;
            typedef ISessionListener<SpotMessageSpec>::KeyType      KeyType;
            typedef ISessionListener<SpotMessageSpec>::BufferType   BufferType;
            typedef ISessionListener<SpotMessageSpec>::SizeType     SizeType;
            typedef ISessionListener<SpotMessageSpec>::TypeId       TypeId;
            typedef ISessionListener<SpotMessageSpec>::ISessionType ISessionType;
            typedef ISessionListener<SpotMessageSpec>::StatusType   StatusType;
            typedef ISessionListener<SpotMessageSpec>::ErrorType    ErrorType;

            /**
             * @brief default destructor.
             */
            virtual ~ISpotSessionListener (void) = 0;
        };
      }
    }
  }

#endif
