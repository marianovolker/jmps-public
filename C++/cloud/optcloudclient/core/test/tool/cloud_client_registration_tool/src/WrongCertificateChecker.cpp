/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file WrongCertificateChecker.cpp
 * @author gparis
 * @date Wednesday, December 10, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include "Poco/DateTimeFormatter.h"
#include "WrongCertificateChecker.h"

namespace gvr
{

namespace communication
{

namespace ssl
{

WrongCertificateChecker::WrongCertificateChecker(bool blHandleErrorsOnServerSide)
  : Poco::Net::InvalidCertificateHandler(blHandleErrorsOnServerSide)
{
}

WrongCertificateChecker::~WrongCertificateChecker()
{
}

void WrongCertificateChecker::onInvalidCertificate(const void* pSender, Poco::Net::VerificationErrorArgs& errorAndCert)
{
	if( errorAndCert.errorNumber() != 0 )
	{
		const Poco::Net::X509Certificate& aCert = errorAndCert.certificate();
		std::clog << std::endl;
		std::clog << "WARNING: Certificate verification failed\n";
		std::clog << "----------------------------------------\n";
		std::clog << "Issuer  Name: " << aCert.issuerName() << std::endl;
		std::clog << "Subject Name: " << aCert.subjectName() << std::endl;
		std::clog << "Common  Name: " << aCert.commonName() << "\n\n";

		std::clog << "Valid from: " << Poco::DateTimeFormatter::format(aCert.validFrom(), "%Y-%m-%d  %H:%M:%S  UTC")
				  << std::endl;
		std::clog << "Expires on: " << Poco::DateTimeFormatter::format(aCert.expiresOn(), "%Y-%m-%d  %H:%M:%S  UTC")
				  << std::endl;

		std::clog << "The error occurred in the certificate chain at depth " << errorAndCert.errorDepth() << ".\n";
		std::clog << "Error code: " << errorAndCert.errorNumber() << ".\n";
		std::clog << "The certificate yielded the error: " << errorAndCert.errorMessage() << "\n\n";

		std::clog << std::endl;

		aCert.save("certificates/SSL_received_certificate.pem");
	}

	// Always validate the certificate and allows the application to go on.
	errorAndCert.setIgnoreError(true);
}


} /* namespace ssl */
} /* namespace communication */
} /* namespace gvr */

