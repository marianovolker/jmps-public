/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file log.cpp
 * @author gparis
 * @date Tuesday, September 30, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvrlog/log.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include <Poco/AutoPtr.h>
#include <Poco/Util/LoggingConfigurator.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/Util/MapConfiguration.h>
#include <Poco/Path.h>
#include <Poco/DateTime.h>
#include <Poco/Mutex.h>
#include <Poco/ScopedLock.h>


using Poco::Util::LoggingConfigurator;
using Poco::Util::PropertyFileConfiguration;
using Poco::Util::MapConfiguration;
using Poco::Logger;
using Poco::Path;
using Poco::DateTime;
using Poco::AutoPtr;

namespace gvr
{
  namespace log
  {
    typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;

    static LoggingConfigurator*  gst_pLogConfigurator = NULL;

    static Poco::FastMutex gst_mxLog;

    /* These values are to store the current day of the log.
     * The idea is to store the current day when the log is created and then
     * when each line is logged check if there is a change of day, in that case
     * recreate all channels of logs and change the date in these variables.
     * To maintain this feature we need to create the following variables.
     */
    static int gst_date_day   = 0;
    static int gst_date_month = 0;
    static int gst_date_year  = 0;

    static const char* gst_pcFullPathNameToIniFile = "libgvrlog.ini";
    static bool gst_bAppendDateToFilenamesOfLogs = true;
    /*************************/

    /**
     * @brief Class helper to modify the internal map of properties loaded
     *        from properties file used by the application.
     */
    class GvrMapConfiguration: public Poco::Util::MapConfiguration
    {
      public:
        GvrMapConfiguration() {};
        virtual ~GvrMapConfiguration() {};

        void appendDateAsYYYYMMDDToFilenamesOfLogs()
        {
          GvrMapConfiguration::iterator it = begin();
          GvrMapConfiguration::iterator ed = end();
          while (it != ed)
          {
            std::string key (it->first);
            std::string value;

            for (std::string::const_iterator its = it->second.begin(); its != it->second.end(); ++its)
            {
              switch (*its)
              {
                case '\t': value.append("\\t");   break;
                case '\r': value.append("\\r");   break;
                case '\n': value.append("\\n");   break;
                case '\f': value.append("\\f");   break;
                case '\\': value.append("\\\\");  break;
                default:   value.push_back(*its); break;
              }
            }

            if( key.find(".path") != std::string::npos )
            {
              Poco::Path path(value, Path::PATH_UNIX);
              Poco::DateTime date;

              std::stringstream newFileName;

              gst_date_day   = date.day();
              gst_date_month = date.month();
              gst_date_year  = date.year();

              newFileName << path.parent().toString();
              newFileName << path.getBaseName() << ".";
              newFileName << gst_date_year;
              newFileName << std::setfill('0') << std::setw(2) << gst_date_month;
              newFileName << std::setfill('0') << std::setw(2) << gst_date_day << ".";
              newFileName << path.getExtension();

              setRaw(key, newFileName.str());
            }

            ++it;
          }
        }
    };

    bool init()
    {
      return init(gst_pcFullPathNameToIniFile, gst_bAppendDateToFilenamesOfLogs);
    }

    bool init(const char* pcFullPathNameToIniFile, bool bAppendDateToFilenamesOfLogs)
    {
      ScopedLock lock(gst_mxLog);

      if(gst_pLogConfigurator != NULL)
      {
        std::cerr << "WARNING: init() - Log instance already created!\n";
        return false; // configuration object already instantiated.
      }

      std::cout << "Log library initializing....\n";

      if( pcFullPathNameToIniFile == NULL )
      {
        pcFullPathNameToIniFile = gst_pcFullPathNameToIniFile;
      }

      gst_pcFullPathNameToIniFile = pcFullPathNameToIniFile;
      gst_bAppendDateToFilenamesOfLogs = bAppendDateToFilenamesOfLogs;

      try
      {
        gst_pLogConfigurator = new LoggingConfigurator;
        AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(pcFullPathNameToIniFile);

        if( bAppendDateToFilenamesOfLogs )
        {
          GvrMapConfiguration map;
          pConfig->copyTo(map);
          map.appendDateAsYYYYMMDDToFilenamesOfLogs();
          gst_pLogConfigurator->configure(&map);
        }
        else
        {
          gst_pLogConfigurator->configure(pConfig);
        }
      }
      catch(Poco::FileNotFoundException& e)
      {
        std::cerr << "File not found exception: " << "Class: " << e.className()
                  << " , Problem: " << e.displayText()  << std::endl << std::endl;

        return false;
      }
      catch(Poco::NotFoundException& e)
      {
        std::cerr << "Not found exception: " << "Class: " << e.className()
                  << " , Problem: " << e.displayText()  << std::endl << std::endl;

        return false;
      }
      catch(Poco::RuntimeException& e)
      {
        std::cerr << "Runtime exception: " << "Class: " << e.className()
                  << " , Problem: " << e.displayText()  << std::endl << std::endl;

        return false;
      }
      catch(Poco::Exception& e)
      {
        std::cerr << "Library exception: " << "Class: " << e.className()
                  << " , Problem: " << e.displayText()  << std::endl << std::endl;

        return false;
      }
      catch(std::exception& e)
      {
        std::cerr << "Generic exception, problem: " << e.what() << std::endl << std::endl;

        return false;
      }
      catch(...)
      {
        std::cerr << "Unknown exception while initializing up the log.\n";

        return false;
      }

      return true;
    }

    void cleanUp(void)
    {
      ScopedLock lock(gst_mxLog);

      try
      {
        Poco::Logger::shutdown();

        if( gst_pLogConfigurator )
        {
          delete gst_pLogConfigurator;
          gst_pLogConfigurator = NULL;
        }
      }
      catch(Poco::FileNotFoundException& e)
      {
        std::cerr << "File not found exception: " << "Class: " << e.className()
                  << " , Problem: " << e.displayText()  << std::endl << std::endl;

        return;
      }
      catch(Poco::NotFoundException& e)
      {
        std::cerr << "Not found exception: " << "Class: " << e.className()
                  << " , Problem: " << e.displayText()  << std::endl << std::endl;

        return;
      }
      catch(Poco::RuntimeException& e)
      {
        std::cerr << "Runtime exception: " << "Class: " << e.className()
                  << " , Problem: " << e.displayText()  << std::endl << std::endl;

        return;
      }
      catch(Poco::Exception& e)
      {
        std::cerr << "Library exception: " << "Class: " << e.className()
                  << " , Problem: " << e.displayText()  << std::endl << std::endl;

        return;
      }
      catch(std::exception& e)
      {
        std::cerr << "Generic exception, problem: " << e.what() << std::endl << std::endl;

        return;
      }
      catch(...)
      {
        std::cerr << "Unknown exception while cleaning up the log.\n";

        return;
      }

      std::cout << "Log library successfully cleaned up to exit the application.\n";
    }

    bool setModuleLevel(const char* pcModule, int iLevel)
    {
      if( pcModule == NULL )
      {
        std::cerr << "WARNING: setModuleLevel() - NULL logger name!\n";
        return false;
      }
      else if( iLevel < eHighestLogLevel || iLevel > eLowestLogLevel )
      {
        std::cerr << "WARNING: setModuleLevel() - logger level number out of range!\n";
        return false;
      }

      Poco::Logger::setLevel(pcModule, iLevel);
      return true;
    }

    bool setModuleLevel(Poco::Logger* pLogger, int iLevel)
    {
      if( pLogger == NULL )
      {
        std::cerr << "WARNING: setModuleLevel() - Logger object not instantiated!\n";
        return false;
      }
      else if( iLevel < eHighestLogLevel || iLevel > eLowestLogLevel )
      {
        std::cerr << "WARNING: setModuleLevel() - logger level number out of range!\n";
        return false;
      }

      pLogger->setLevel(iLevel);
      return true;
    }

    bool ifAnotherDay()
    {
      Poco::DateTime date;

      return !( (gst_date_day == date.day()) && (gst_date_month == date.month()) && (gst_date_year == date.year()) );
    }

  } // end namespace log
} // end namespace gvr

