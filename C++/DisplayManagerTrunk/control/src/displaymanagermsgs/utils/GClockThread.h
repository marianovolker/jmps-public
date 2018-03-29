#ifndef CLOCK_THREAD_H
#define CLOCK_THREAD_H

#include <QThread>
#include <QMutex>

class GClockThread : public QThread
{
public:
	~GClockThread();

	/* 
	 * Returns the instance.
	 */
	static GClockThread * Instance();

	/*
	 * Creates the thread instance if it wasn't created
	 * previously. 
	 * The instance will be returned if it is created.
	 */
	static GClockThread * CreateSoleInstance();

	/*
	 * If the instance is created, it will be removed.
	 */
	static void RemoveSoleInstance();

	/*
	 * x and y position are saved by this method.
	 * They will be used to locate the clock in the screen.
	 */
	void SetCoordinates(int x_pos, int y_pos);

	/*
	 * It stops the thread. It will be blocked
	 * until it returns from run().
   */
	void Stop();
protected:

	void run();

private:
	/*
	 * The time between phases in milliseconds.
	 */
	enum {ePhaseTime = 500};
	
	enum {ePhaseMin = 0, ePhaseMax = 7};

	GClockThread();

	/*
	 * Returns the next phase of the clock.
	 */
	QString rotatePhase();
	
	/*
	 * Table to hold the clock phases represented as strings.
	 */
	static const char m_phasesTable[][3];
	static unsigned char m_iPhaseIndex;

	static GClockThread *m_pInstance;
	static QMutex m_mutex;

	volatile bool m_bRunning;
	int m_iXpos;
	int m_iYpos;
};

#endif
