#ifndef GGstLaunchMediaManager_H_
#define GGstLaunchMediaManager_H_

#include "GAbsMediaManager.h"
#include <QThread>
#include <QSharedPointer>
#include "utils/base/src/GException.h"

class QMutex;
class QString;
class QWaitCondition;


class GGstLaunchMediaManager : public GAbsMediaManager
{
	Q_OBJECT

public:

	static QSharedPointer<GGstLaunchMediaManager> GetInstance();


public:

	~GGstLaunchMediaManager();
    	void Initialize(char **env);
	void StartVideo(const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile) throw(GException);
	void StopMedia();
	void SynchroStopMedia();
	void StopChildProcesses();
	int GetVideoStopEndDelay();


private:

        QString m_strVideoAppCommand;
	int m_iVideoProcessid;
	static QMutex _mtxSynchMutexVideoTermination;
	static QWaitCondition _cndSynchConditionalVideoTermination;
	static QSharedPointer<GGstLaunchMediaManager> st_poGGstLaunchMediaManager;
	static QMutex st_oMutex;

	void BuildLaunchVideoCommand(QList<QByteArray> &slVideoCmdLine, const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile);
	void ExecuteLaunchVideoCommand(char **cppArgsLaunchVideoCmd,  int iArgQtty);
	void EndVideoExecution();
	GGstLaunchMediaManager(); // Private constructor for singleton
        // Define a private copy constructor and assignment operator to prevent their use
        GGstLaunchMediaManager( const GGstLaunchMediaManager& gGstLaunchMediaManager );
        GGstLaunchMediaManager& operator=( const GGstLaunchMediaManager& gGstLaunchMediaManager );

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
