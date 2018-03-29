#ifndef GProcess_H
#define	GProcess_H


class GProcess
{
public:
	static int StartDisplayManagerProcess(int argc, char *argv[], char *env[]);
	static int StartSignalHandlerProcess();

public:
    static void BlockAllSignalsInit();
    static void UnBlockChildSignal();
    static void UnBlockUsefulSignalsInit();
    static void SignalsInit();
    static void MainLoop();
    static void SignalsSocketNotifierInit();
    static void hupSignalHandler(int);
    static void termSignalHandler(int);
    static void intSignalHandler(int);
    static void chldSignalHandler(int);

    static void AddSigChildHandler();
    static void SetNoChildWait();

public:
	static int sighupFd[2];
	static int sigtermFd[2];
	static int sigintFd[2];
	static bool bContinue;

private:
	virtual void _init() = 0;
};

#endif
