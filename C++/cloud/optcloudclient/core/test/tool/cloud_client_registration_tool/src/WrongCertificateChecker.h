/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file WrongCertificateChecker.h
 * @author gparis
 * @date Wednesday, December 10, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef WRONGCERTIFICATECHECKER_H_
#define WRONGCERTIFICATECHECKER_H_

#include "Poco/Net/InvalidCertificateHandler.h"

namespace gvr
{

namespace communication
{

namespace ssl
{

/**
 * @class WrongCertificateChecker
 *
 * @brief  The main responsibility of this class is to check the received server certificate in the case
 *         the default error handler in Poco::Net framework, that is X509Certificate::verify(),
 *         marks this certificate as "wrong". Then this class can be used as a helper to debug and
 *         determine the exact cause of the error.
 *
 * @author Guillermo Paris
 * @date   2014-12-10
 * @since  0.0.0
 */
class WrongCertificateChecker : public Poco::Net::InvalidCertificateHandler
{
  public:
   /*
    * @brief Constructor
    *
    * @param blHandleErrorsOnServerSide @b true on server side verification, @b false in client side.
    */
    WrongCertificateChecker(bool blHandleErrorsOnServerSide);

   /*
    * @brief Destructor
    */
    ~WrongCertificateChecker();

   /*
    * @brief certificate validator routine.
    *
    * @note Currently it is just a debugging routine, always validating OK, but printing certificate
    *       details in case of erroneous validation done by the underlying openSSL routines.
    *
    * @param pSender  unused and undocumented.
    * @param [in|out] errorCert The X509 certificate, plus its error details.
    */
    virtual void onInvalidCertificate(const void* pSender, Poco::Net::VerificationErrorArgs& errorAndCert);
};


} /* namespace ssl */
} /* namespace communication */
} /* namespace gvr */

#endif /* WRONGCERTIFICATECHECKER_H_ */
