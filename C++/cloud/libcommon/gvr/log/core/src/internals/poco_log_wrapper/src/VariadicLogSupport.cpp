/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file VariadicLogSupport.cpp
 * @author gparis
 * @date Friday, December 19, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstdio>
#include <iostream>

#include <Poco/Logger.h>

#include <gvrlog/VariadicLogSupport.h>

/// @brief Gilbarco Domain
namespace gvr
{
/// @brief log domain
namespace log
{

char* VariadicLogSupport::Format(unsigned int& nchars, char* pcBuffer, const char* pcFormat,...)
{
  va_list args;
  va_start(args, pcFormat);
  Format(nchars, pcBuffer, pcFormat, args);
  va_end(args);
  return pcBuffer;
}

char* VariadicLogSupport::Format(unsigned int& nchars, char* pcBuffer, const char* pcFormat, va_list vlist)
{
  int length;
  va_list vlistBackup;
  va_copy(vlistBackup, vlist);
  length = vsnprintf(NULL, 0, pcFormat, vlistBackup);
  va_end(vlistBackup);
  if(nchars == 0 || length <= 0) // length request or some error happened
  {
      pcBuffer[0] = '\0';
      nchars = length;
  }
  else if(nchars < length) // buffer too small
  {
      pcBuffer[0] = '\0';
      nchars = 0;
  }
  else // real operation without errors beforehand
  {
    nchars = vsnprintf(pcBuffer, nchars, pcFormat, vlist);
  }

  return pcBuffer;
}


bool VariadicLogSupport::LogFatal(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage)
{
  return Log(pLogger, eFatal, pcCppFile, line, pcMessage);
}

bool VariadicLogSupport::LogFatal(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...)
{
  bool blret = false;
  va_list args;

  va_start(args, pcFormat);
  blret = Log(pLogger, eFatal, pcCppFile, line, pcFormat, args);
  va_end(args);

  return blret;
}

bool VariadicLogSupport::LogCritical(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage)
{
  return Log(pLogger, eCritical, pcCppFile, line, pcMessage);
}

bool VariadicLogSupport::LogCritical(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...)
{
  bool blret = false;
  va_list args;

  va_start(args, pcFormat);
  blret = Log(pLogger, eCritical, pcCppFile, line, pcFormat, args);
  va_end(args);

  return blret;
}

bool VariadicLogSupport::LogError(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage)
{
  return Log(pLogger, eError, pcCppFile, line, pcMessage);
}

bool VariadicLogSupport::LogError(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...)
{
  bool blret = false;
  va_list args;

  va_start(args, pcFormat);
  blret = Log(pLogger, eError, pcCppFile, line, pcFormat, args);
  va_end(args);

  return blret;
}

bool VariadicLogSupport::LogWarn(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage)
{
  return Log(pLogger, eWarn, pcCppFile, line, pcMessage);
}

bool VariadicLogSupport::LogWarn(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...)
{
  bool blret = false;
  va_list args;

  va_start(args, pcFormat);
  blret = Log(pLogger, eWarn, pcCppFile, line, pcFormat, args);
  va_end(args);

  return blret;
}

bool VariadicLogSupport::LogNotice(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage)
{
  return Log(pLogger, eNotice, pcCppFile, line, pcMessage);
}

bool VariadicLogSupport::LogNotice(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...)
{
  bool blret = false;
  va_list args;

  va_start(args, pcFormat);
  blret = Log(pLogger, eNotice, pcCppFile, line, pcFormat, args);
  va_end(args);

  return blret;
}

bool VariadicLogSupport::LogInfo(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage)
{
  return Log(pLogger, eInfo, pcCppFile, line, pcMessage);
}

bool VariadicLogSupport::LogInfo(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...)
{
  bool blret = false;
  va_list args;

  va_start(args, pcFormat);
  blret = Log(pLogger, eInfo, pcCppFile, line, pcFormat, args);
  va_end(args);

  return blret;
}

bool VariadicLogSupport::LogDebug(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage)
{
  return Log(pLogger, eDebug, pcCppFile, line, pcMessage);
}

bool VariadicLogSupport::LogDebug(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...)
{
  bool blret = false;
  va_list args;

  va_start(args, pcFormat);
  blret = Log(pLogger, eDebug, pcCppFile, line, pcFormat, args);
  va_end(args);

  return blret;
}

bool VariadicLogSupport::LogTrace(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage)
{
  return Log(pLogger, eTrace, pcCppFile, line, pcMessage);
}

bool VariadicLogSupport::LogTrace(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...)
{
  bool blret = false;
  va_list args;

  va_start(args, pcFormat);
  blret = Log(pLogger, eTrace, pcCppFile, line, pcFormat, args);
  va_end(args);

  return blret;
}


bool VariadicLogSupport::Log(Poco::Logger* pLogger, ELogLevel e, const char* pcCppFile, int line, const char* pcFormat, va_list vlist)
{
  bool    blret = false;
  int     length = 0;
  char*   pcBuffer = NULL;
  va_list vlistBackup;

  va_copy(vlistBackup, vlist);
  length = vsnprintf(NULL, 0, pcFormat, vlistBackup); // length calculation
  va_end(vlistBackup);

  if(length <= 0) // some error happened calculating length
  {
    return false;
  }

  pcBuffer = new char[length + 1]; // buffer to hold the final message as a variadic printf result.
  length = vsnprintf(pcBuffer, length, pcFormat, vlist);

  if(length <= 0) // some error happened printing over buffer
  {
    delete[] pcBuffer;
    return false;
  }

  blret = Log(pLogger, e, pcCppFile, line, pcBuffer);
  delete[] pcBuffer;
  return blret;
}

bool VariadicLogSupport::Log(Poco::Logger* pLogger, ELogLevel e, const char* pcCppFile, int line, const char* pcMessage)
{
  bool blret = true;
  bool blBadParam = false;

  try
  {
    switch(e)
    {
      case eFatal:
        pLogger->fatal(pcMessage, pcCppFile, line);
      break;

      case eCritical:
        pLogger->critical(pcMessage, pcCppFile, line);
      break;

      case eError:
        pLogger->error(pcMessage, pcCppFile, line);
      break;

      case eWarn:
        pLogger->warning(pcMessage, pcCppFile, line);
      break;

      case eNotice:
        pLogger->notice(pcMessage, pcCppFile, line);
      break;

      case eInfo:
        pLogger->information(pcMessage, pcCppFile, line);
      break;

      case eDebug:
        pLogger->debug(pcMessage, pcCppFile, line);
      break;

      case eTrace:
        pLogger->trace(pcMessage, pcCppFile, line);
      break;

      default:
        blret = false; // should never happen: bad level parameter
        blBadParam = true;
      break;
    }
  }
  catch(Poco::FileNotFoundException& e)
  {
    std::cerr << "File not found exception: " << "Class: " << e.className()
              << " , Problem: " << e.displayText()  << std::endl;

    blret = false;
  }
  catch(Poco::NotFoundException& e)
  {
    std::cerr << "Not found exception: " << "Class: " << e.className()
              << " , Problem: " << e.displayText()  << std::endl;

    blret = false;
  }
  catch(Poco::RuntimeException& e)
  {
    std::cerr << "Runtime exception: " << "Class: " << e.className()
              << " , Problem: " << e.displayText()  << std::endl;

    blret = false;
  }
  catch(Poco::Exception& e)
  {
    std::cerr << "Library exception: " << "Class: " << e.className()
              << " , Problem: " << e.displayText()  << std::endl;

    blret = false;
  }
  catch(std::exception& e)
  {
    std::cerr << "Generic exception, problem: " << e.what() << std::endl;

    blret = false;
  }
  catch(...)
  {
    std::cerr << "Unknown exception while logging.\n";

    blret = false;
  }

  if( !blret && !blBadParam ) // exception happened while writing to log
  {
    std::cerr << "Please check the existence of log directory!\n\n";
  }

  return blret;
}

} // end namespace log
} // end namespace gvr

