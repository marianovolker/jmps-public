#ifndef _GIMAGEHELPER_H
#define	_GIMAGEHELPER_H

#include <QString>
#include <QDir>

#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"

class GImageHelper 
{
public:
    GImageHelper();
    virtual ~GImageHelper();

    static const QString CurrentResolutionColorImagesPath();

    static const QString CurrentResolutionColorImagesAbsolutePath();

    static const QString CurrentResolutionMonochromeImagesPath();

    static const QString CurrentResolutionMonochromeImagesAbsolutePath();

    static const QString CurrentResolutionString();

    static const QString RawImagesPath();

    static  const QString ImagesMappingsFile();

    

    static const QString CSTRCustomGraphicPropertyNamePrefix;
    
    static const QString CSTRMonochromeImagesRelativeDir;

    static const QString CSTRRawImagesRelativeDir;

    static const QString CSTRColorImagesRelativeDir;

private:

};


inline const QString GImageHelper::CurrentResolutionColorImagesPath()
{
    LOG_LEVEL4("CurrentResolutionColorImagesPath()");
    return ( GETCONF_STR(eDisplayManagerImagesDir) + "/" + CSTRColorImagesRelativeDir + CurrentResolutionString() );
}

inline const QString GImageHelper::CurrentResolutionColorImagesAbsolutePath()
{
    LOG_LEVEL4("CurrentResolutionMonochromeImagesPath()");
    return ( QDir(CurrentResolutionColorImagesPath()).absolutePath() + "/" );
}

inline const QString GImageHelper::CurrentResolutionMonochromeImagesPath()
{
    LOG_LEVEL4("CurrentResolutionMonochromeImagesPath()");
    return ( GETCONF_STR(eDisplayManagerImagesDir) + "/" + CSTRMonochromeImagesRelativeDir + CurrentResolutionString() );
}

inline const QString GImageHelper::CurrentResolutionMonochromeImagesAbsolutePath()
{
    LOG_LEVEL4("CurrentResolutionMonochromeImagesPath()");
    return ( QDir(CurrentResolutionMonochromeImagesPath()).absolutePath() + "/" );
}


inline const QString GImageHelper::CurrentResolutionString()
{
    LOG_LEVEL4("CurrentResolutionString()");
    return ( GETCONF_STR(eDisplayManagerViewWidth) + "x" + GETCONF_STR(eDisplayManagerViewHeight) + "/" );
}


inline const QString GImageHelper::RawImagesPath()
{
    LOG_LEVEL4("RawImagesPath()");
    return ( GETCONF_STR(eDisplayManagerImagesDir) + "/" + CSTRRawImagesRelativeDir );
}


inline const QString GImageHelper::ImagesMappingsFile()
{
    LOG_LEVEL4("ImagesMappingsFile()");
    return GETCONF_STR(eDisplayManagerImageMappingsFileName);
}



#endif
