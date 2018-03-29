//! GDisplayManagerDaemon
/*!
  The main purpose of this class is to raise in order all the components of DisplayManager.
*/

#ifndef GDisplayManagerDaemon_H_
#define GDisplayManagerDaemon_H_

#include <QApplication>
#include <QSharedPointer>

#include "GControlChannelMgr.h"

class GSignalHandler;


class GDisplayManagerDaemon : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(GDisplayManagerDaemon)

public:
	GDisplayManagerDaemon(QApplication*, char **env);
	virtual ~GDisplayManagerDaemon();
	void Init();
	void Stop();
	int Run();

public slots:
    void goAway();

private:
	void QTInit();
	void LoggerInit();
	void ShowEnvironment();
	void ShowCurrentConfiguration();
	void PageLoadRequestManagerInit();
	void ImageConverterInit();
	void ImageMappingsInit();
	void PersonalityViewInit();
	void ControlChannelMgrInit();
	void SignalHandlerInit();
	void VideoManagerInit();

private:
	void PageLoadRequestManagerStop();
	void ImageConverterStop();
	void ControlChannelMgrStop();
	void LoggerStop();
	void SignalHandlerStop();

private:
    QSharedPointer<GControlChannelMgr> m_pControlChMgr;
    QSharedPointer<GSignalHandler> m_pSignalHandler;

    QApplication* m_pApp;
    char **m_env;
};

#endif
