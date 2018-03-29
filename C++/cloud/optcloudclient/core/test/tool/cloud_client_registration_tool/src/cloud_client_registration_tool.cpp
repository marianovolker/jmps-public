/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file cloud_client_registration_tool.cpp
 * @author gparis/mgrieco
 * @date Wednesday, November 10, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/cloud_client_registration_tool.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/NetSSL.h"
#include "Poco/Net/CertificateHandlerFactory.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/SharedPtr.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/Net/HTTPStream.h"

#include <algorithm>  // for copy
#include <iterator>
#include <string>
#include <iostream>   // for cout, cerr, clog, istream, etc.

#include "WrongCertificateChecker.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPCredentials;
using gvr::communication::ssl::WrongCertificateChecker;

const char* pcCappPayloadUrlEncoded = "application/x-www-form-urlencoded";
const char* pcCappPayloadJson = "application/json";
const char* pcCregistrationCodeInputTemplate = "\"registrationCode\": \"%10s\"";

enum EReturnCode
{
	eOK, ePreRegFailed, eRegFailed, eUnRegFailed, eBadParam
};

// First of all our certificate error handler must be registered to Poco::Net library.
// This macro creates a static global registration instance.
POCO_REGISTER_CHFACTORY(gvr, WrongCertificateChecker)

/**
 * doRequest
 * @brief Does the request to the server using a given session
 * @param [in] session, the http client session
 * @param [in] request, the http request to server
 * @param [in] sPayload, the payload request data
 * @param [in] contentType, the request content type
 * @param [out] response, the http response to server
 */
bool doRequest(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, const std::string& sPayload, const std::string& contentType, Poco::Net::HTTPResponse& response)
{
  request.setContentType(contentType);
  request.setContentLength( sPayload.length() );
  std::ostream& send = session.sendRequest(request);
  send << sPayload << std::flush;

  std::istream& rs = session.receiveResponse(response);
  if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
  {
    Poco::StreamCopier::copyStream(rs, std::cerr);
    std::cerr << std::endl << std::endl;
    return false;
  }
  else
  {
    Poco::StreamCopier::copyStream(rs, std::cout);
    std::cout << std::endl << std::endl;
    return true;
  }
}

/**
 * showUsage
 * @brief Shows the tool usage help text
 * */
static void showUsage()
{
  std::cerr << "Usage: cloud_client_registration_tool [option(s)] <ACTION> <URL> <DATA-FILENAME> [Registration Code]\n"
            << "Options:\n"
            << "\t-h,--help\t\tShow this help message\n"
            << "Parameters:\n"
            << "\tACTION should be one of the following: PRE, REG, UNREG.\n"
            << "\tURL the url or ip address for the corresponding cloud server.\n"
            << "\tDATA-FILENAME the file name containing the action data (payload text for the request).\n"
            << "\tRegistration Code is the optional supplied one, got from pre-registration response output.\n\n";
}

/**
 * readFileToString
 * @brief Reads a file into a string result from a given filename.
 * @param [in] filename, the file name to read.
 * @param [out] result, the resulting text.
 */
void readFileToString(const std::string& filename, std::string &  sResult)
{
  sResult.clear();

  try
  {
    Poco::FileInputStream  fis(filename,std::ios::in);
    if(!fis || !fis.good())
    {
      throw Poco::Exception("Error opening file input stream.");
    }

    std::string sLine;

    while( std::getline(fis, sLine) )
    {
      sResult += sLine;
      sResult.push_back('\n');
    }
  }
  catch (Poco::Exception& exc)
  {
    std::cerr << exc.displayText() << std::endl;
  }
}

/**
 * applyRegistrationCodeToPayload
 * @brief Apply the supplied registration code to a parametric registration payload.
 * @param [in] pcRegistrationCode, the supplied alphanumeric registration code. Only the first 10 chars
 *                                 are taken into account.
 * @param [in|out] sPayload, the parametric payload text to be "patched" with the correct registration code.
 * @return The legend "Registration Code: xxxxxxxxxx\n"
 */
std::string applyRegistrationCodeToPayload(const char* pcRegistrationCode, std::string &  sPayload)
{
  std::string sRegCodeLegend;
  int iActCodeParamIndex = sPayload.find(pcCregistrationCodeInputTemplate); // Searches for the specifier %10s

  if( iActCodeParamIndex >= 0 ) // format specifier found inside sPayload text
  {
    char aux[1024];
    memset(aux, 0, sizeof(aux));

    // Builds payload text replacing specifiers by values (currently activationCode only).
    snprintf(aux, sizeof(aux)-1, sPayload.c_str(), pcRegistrationCode);
    sPayload = aux;

    sRegCodeLegend = "Registration Code: ";
    sRegCodeLegend += pcRegistrationCode;
    sRegCodeLegend += '\n';
  }

  return sRegCodeLegend;
}

/**
 * doPreOrDeviceRegistration
 * @brief Creates, populates and sends the (pre)registration request, and waits and populates the answer.
 * @param blPreRegistration, @b true  if the request is a PRE-registration, @b false if it is a registration.
 * @param pcUri, the URI endpoint to be connected to perform the action requested.
 * @param pcPayloadFilename, the filename of the (pre)registration payload text.
 * @param sPayload, the (pre)registration payload text as a string.
 * @param sRegistrationCode, the supplied registration code as a string.
 *
 * @return @b true  if the request was successfully sent and the response was successfully received.
 *         @b false if exceptions or other errors occurred.
 *
 * @note The response content is not parsed. So it has nothing to do with the boolean returned.
 */
bool doPreOrDeviceRegistration( bool blPreRegistration, const char* pcUri,
                                const char* pcPayloadFilename, const std::string& sPayload ,
                                const std::string& sRegistrationCode )
{
    std::string sAction = blPreRegistration ? "PRE-" : "";
    std::clog << "cloud_client_registration_tool - " << sAction << "REGISTRATION  Site Address:" << pcUri
              << ", Data File:" << pcPayloadFilename << std::endl << std::endl
              << "Request Payload: \n" << sPayload << std::endl << sRegistrationCode << std::endl;

    std::string contentType( blPreRegistration ? pcCappPayloadUrlEncoded : pcCappPayloadJson );

    try
    {
      //CREATE HTTPS SESSION AND CONNECT
      Poco::Crypto::OpenSSLInitializer::initialize();
      Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> qAcceptCertHandler = new WrongCertificateChecker(false);
      Poco::SharedPtr<Poco::Net::PrivateKeyPassphraseHandler> qtrPassphraseHandler = new Poco::Net::KeyConsoleHandler(false);// ask the user via console for the pwd
      const Poco::Net::Context::Ptr qContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "certificates/not-revoked.pem", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

      Poco::Net::SSLManager::instance().initializeClient(qtrPassphraseHandler, qAcceptCertHandler, qContext);

      Poco::URI uri(pcUri);
      std::string path(uri.getPathAndQuery());
      if (path.empty())
      {
        path = "/";
      }

      HTTPSClientSession session(uri.getHost(), uri.getPort());

      //REQUEST/RESPONSE using the sPayload
      Poco::Net::HTTPRequest request(HTTPRequest::HTTP_POST, path);
      Poco::Net::HTTPResponse response;
      if( !doRequest(session, request, sPayload, contentType, response) )
      {
          std::cerr << "Invalid request." << std::endl;
          return false;
      }
    }
    catch (Poco::Exception& exc)
    {
      std::cerr <<  "exception found:" << exc.displayText() << std::endl;
      return false;
    }

	return true;
}

/**
 * doDeviceUnRegistration
 * @brief Creates, populates and sends the unregistration request, and waits and populates the answer.
 *
 * @param pcUri, the URI endpoint to be connected to perform the action requested.
 * @param pcPayloadFilename, the filename of the unregistration payload text.
 * @param sPayload, the unregistration payload text as a string.
 *
 * @return @b true  if the request was successfully sent and the response was successfully received.
 *         @b false if exceptions or other errors occurred.
 *
 * @note The response content is not parsed. So it has nothing to do with the boolean returned.
 */
bool doDeviceUnRegistration( const char* pcUri, const char* pcPayloadFilename, const std::string& sPayload )
{
  std::clog << "cloud_client_registration_tool - UN-REGISTRATION  Site Address:"
            << pcUri << ", Data File:" << pcPayloadFilename << std::endl << std::endl;

  Poco::URI uri(pcUri);
  std::string path(uri.getPathAndQuery());
  if( path.empty() )
  {
    path = "/";
  }

  try
  {
    // Initialize session
    Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
    Poco::Net::HTTPRequest request(HTTPRequest::HTTP_POST, path);
    Poco::Net::HTTPResponse response;
    // Prepare and send request, and get response
    if( !doRequest(session, request, sPayload, pcCappPayloadJson, response) )
    {
      std::cerr << "Invalid request." << std::endl;
      return false;
    }
  }
  catch (Poco::Exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    return false;
  }

  return true;
}


/**
 * main
 * @brief      Cloud Tool Main entry point.
 * @param [in] argc Amount of arguments passed to the cloud tool command line.
 * @param [in] argv List of arguments passed to the cloud tool command line.
 * @return     Command line expected result code. Returns 0, only if operation finished successfully.
 * @since 1.0.0
 */
int main (int argc, char* argv[])
{
  if (( argc < 4 ) || ( strcasecmp(argv[1],"-h") == 0 ) || ( strcasecmp(argv[1],"--help" ) == 0 ) || ( strcasecmp("PRE",argv[1]) != 0 && strcasecmp("REG",argv[1]) != 0  && strcasecmp("UNREG",argv[1]) != 0 ) )
  {
    showUsage();
    return eOK;
  }

  std::string sPayload;
  std::string sRegistrationCodeLegend; // must be empty if not externally supplied.

  readFileToString( std::string(argv[3]), sPayload ); // Reads the text file content into string sPayload.

  // case of optional fourth command line arg : Activation Code from the pre-registration stage.
  //                                            payloads/REG.Payload.txt must have "%10s" as registration code.
  if( argv[4] != NULL && strcasecmp("REG", argv[1]) == 0 ) // registration code was passed externally
  {
	  sRegistrationCodeLegend = applyRegistrationCodeToPayload( argv[4], sPayload );
  }

  //PRE and REGISTRATION
  if( strcasecmp("PRE",argv[1]) == 0 || strcasecmp("REG", argv[1]) == 0 )
  {
    bool blPreRegistration = ( strcasecmp("PRE",argv[1]) == 0 ? true : false );

    bool blRet = doPreOrDeviceRegistration( blPreRegistration, argv[2], argv[3], sPayload, sRegistrationCodeLegend );
    return blRet ? eOK : ( blPreRegistration ? ePreRegFailed : eRegFailed );
  }

  //UN-REGISTRATION
  if( strcasecmp("UNREG",argv[1]) == 0 )//un-registration action
  {
    bool blRet = doDeviceUnRegistration(argv[2], argv[3], sPayload);
    return blRet ? eOK : eUnRegFailed;
  }

  // incorrect parameters
  showUsage();
  return eBadParam;
}


