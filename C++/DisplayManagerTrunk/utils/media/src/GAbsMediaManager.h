/* 
 * File:   GAbsMediaManager.h
 * Author: sorgius
 *
 * Created on March 25, 2011, 11:41 AM
 */

#ifndef GABSMEDIAMANAGER_H
#define	GABSMEDIAMANAGER_H

#include <QThread>
#include <QSharedPointer>
#include <errno.h>
#include <stdlib.h>
#include "utils/base/src/GException.h"
#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//class QMutex;
//class QString;
//class QWaitCondition;


class GAbsMediaManager : public QThread
{

    Q_OBJECT

public:
    void run();
    void Stop();

    virtual void Initialize(char **env);
    virtual void StartVideo(const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile) throw(GException) = 0;
    virtual void StopMedia() = 0;
    virtual void SynchroStopMedia() = 0;
    virtual void StopChildProcesses() = 0; // Stop any child processes created by the MediaManager here

    void MuteVideo();
    void UnMuteVideo();

    virtual ~GAbsMediaManager();

protected:
    char **m_env;
    bool m_bSystemRestartNeeded;
    QString m_strVideoMuteCommand;
    QString m_strVideoFilePath;
    QString m_strVideoMaxBufs;
    QString m_strVideoLateness;
    QString m_strVideoGstPipeline;
    QString m_strVideoVideoDecoderGstElement;
    QString m_strVideoAudioDecoderGstElement;
    QString m_strVideoPlayCommandFilePath;
    QString m_strCurrentVideoFilePath;
    int m_iVideoRotation;
    int m_iVideoStopEndSleep;

    GAbsMediaManager();

    void VideoMuteUnMute(const QString &strMuteParameter);
    void UnBlockAllSignals();
    void ExecuteMuteUnMuteVideoCommand(const QString &strMuteParameter);
    QByteArray JoinLaunchVideoCmd(const QList<QByteArray> &balVideoCmdLine, const QByteArray &baSep) const;
    char **CreateVariableArgument(QList<QByteArray> &balVideoCmdLine, int iArgQtty) const;
    void ProcessFailedFork();

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GAbsMediaManager( const GAbsMediaManager& gAbsMediaManager );
    GAbsMediaManager& operator=( const GAbsMediaManager& gAbsMediaManager );

protected slots:

    void SlotMuteVideo();
    void SlotUnMuteVideo();

signals:

    void SignalMuteVideo();
    void SignalUnMuteVideo();


};

#endif	/* GABSMEDIAMANAGER_H */

