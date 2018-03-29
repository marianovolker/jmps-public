#ifndef GDMQTAPLLICATION_H_
#define GDMQTAPLLICATION_H_

#include <iostream>
#include <cstdlib>
#include <QApplication>

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "utils/base/src/GException.h"
#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

using std::cout;
using std::endl;

class GDMQtApplication : public QApplication
{
        Q_OBJECT;

public:
        GDMQtApplication(int argc, char *argv[]) : QApplication(argc, argv)
        {
        }

        bool notify( QObject *receiver, QEvent *e )
        {
                try
                {
                        return QApplication::notify( receiver, e );
                }
                catch (std::exception *e)
                {
                        cout << "Unhandled exception from GDMQtApplication" << e->what() << endl;
                        LOG_LEVEL3("Unhandled exception from GDMQtApplication");

                        kill(getppid(), 9);     //Killing parent process
                        abort();
                        return false;
                }
                catch(...)
                {
                        cout << "Unhandled exception from GDMQtApplication" << endl;
                        LOG_LEVEL3("Unhandled exception from GDMQtApplication");

                        kill(getppid(), 9);     //Killing parent process
                        abort();
                        return false;
                }
        }
};

#endif


