#ifndef GGstDaemonMediaManager_H_
#define GGstDaemonMediaManager_H_

#include "GAbsMediaManager.h"
#include <QThread>
#include <QSharedPointer>
#include <vector>
#include "utils/base/src/GException.h"

class QMutex;
class QString;
class QWaitCondition;


class GGstDaemonMediaManager : public GAbsMediaManager
{
	Q_OBJECT

public:

	static QSharedPointer<GGstDaemonMediaManager> GetInstance();


public:

	~GGstDaemonMediaManager();


public:

    	void Initialize(char **env);

	void StartVideo(const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile) throw(GException);
	void StopMedia();
	void SynchroStopMedia();
	void StopChildProcesses();



private:
        enum VideoState {
            null,
            pause,
            ready,
            play
        };
        bool m_bIsDisplayHidden;
        VideoState m_eCurrentVideoState;
        QString m_strVideoDaemonCommand;
        QString m_strVideoDaemonClientCommand;
        QString m_strVideoDaemonClientCreatePipelineCommand;
        QString m_strVideoDaemonClientFilenameCommand;
        QString m_strVideoDaemonClientShowCommand;
        QString m_strVideoDaemonClientHideCommand;
        QString m_strVideoDaemonClientSetStateCommand;
        QString m_strVideoDaemonProcessName;
	int m_iVideoDaemonClientCommandTimeout;
        int m_iDaemonProcessID;

	static QMutex _mtxSynchMutexVideoTermination;
	static QWaitCondition _cndSynchConditionalVideoTermination;

	
	static QSharedPointer<GGstDaemonMediaManager> st_poGGstDaemonMediaManager;
	static QMutex st_oMutex;

	GGstDaemonMediaManager(); // Private constructor for Singleton
	void BuildCreatePipelineCommand(QList<QByteArray> &slVideoCmdLine, const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile);

	void ExecuteCreatePipelineCommand(char **cppArgsLaunchVideoCmd,  int iArgQtty);
	void ExecuteSetStateCommand(QString newState);
        void ExecuteSetVideoFilenameCommand(const QString &strVideoFile);
        void ExecuteShowDisplayCommand();
        void ExecuteHideDisplayCommand();

	void EndVideoExecution();
        void GetGstDaemonProcessID(std::vector<std::string>& vProcessID) const;
        void RestartGstDaemon();
        void StartGstDaemon();
        void StopGstDaemon();
        bool KillGstDaemon(int signal); // returns true on successfully killing gstd process, false otherwise

        // Define a private copy constructor and assignment operator to prevent their use
        GGstDaemonMediaManager( const GGstDaemonMediaManager& gGstDaemonMediaManager );
        GGstDaemonMediaManager& operator=( const GGstDaemonMediaManager& gGstDaemonMediaManager );

private slots:

	void SlotStartVideo(const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile);
	void SlotStopMedia();
	void SlotSynchroStopMedia();

signals:

	void SignalStartVideo(const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile);
	void SignalStopMedia();
	void SignalSynchroStopMedia();

};



#endif
