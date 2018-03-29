#ifndef GSignalHandler_H
#define	GSignalHandler_H

#include <QThread>

class GDisplayManagerDaemon;

class GSignalHandler : public QThread
{
    Q_OBJECT

public:
    GSignalHandler(GDisplayManagerDaemon *pDMObserver);
    virtual ~GSignalHandler();

    void run();
    void Stop();

private:
    void MainLoop();
    void handleSigHup();
    void handleSigTerm();
    void handleSigInt();

private:
    GDisplayManagerDaemon *m_pDMObserver;
    bool m_bContinue;
    
signals:
    void StopDM();
};

#endif
