//! GSiteServerURLCommandsHelper 
/*!
  Helper for parsing URL Commands from SiteServer 
*/
#ifndef GSiteServerURLCommandsHelper_H_
#define	GSiteServerURLCommandsHelper_H_

#include <QUrl>
#include <QString>
#include "utils/base/src/GException.h"

class GSiteServerURLCommandsHelper
{

public:

    typedef enum
	{ eVideoTypeCommand, eAudioTypeCommand, eNoMediaTypeCommand } eMediaTypeCommand;

    GSiteServerURLCommandsHelper(const QString &strUrl) throw (GException);
    
    virtual ~GSiteServerURLCommandsHelper();

    void SetSiteServerURL(const QString &strUrl) throw (GException);

    QString GetSecondURL() const;
    

    void FirstLevelParse();

    void ProcessMediaCommands() throw(GException);

    void ObtainURL();

private:

    QUrl m_SiteServerURL;
    QString m_strSecondURL;
    QString m_strMediaAction;
    eMediaTypeCommand m_eMediaTypeCommand;

    static QString const CstrAudioMediaTypeURLScheme;
    static QString const CstrVideoMediaTypeURLScheme;

    static QChar const CcQueryValueDelim;
    static QChar const CcQueryPairDelim;

    static QString const CstrQueryItemKeyUrl;
    static QString const CstrQueryItemValueUrlDefault;

    static QString const CstrCommandVolumeMixer;
    static QString const CstrQueryItemKeyMute;

    static QString const CstrQueryItemKeyX;
    static QString const CstrQueryItemKeyY;
    static QString const CstrQueryItemKeyWidth;
    static QString const CstrQueryItemKeyHeight;

    static QString const CstrMuteTrue;
    static QString const CstrMuteFalse;

    void ProcessAudioCommand();
    void ProcessVideoCommand()  throw(GException);

};

#endif	/* GSiteServerURLCommandsHelper_H */

