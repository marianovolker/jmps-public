#include "GClockThread.h"
#include "utils/logger/src/GLogger.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GTextField.h"

#include <QSharedPointer>

GClockThread * GClockThread::m_pInstance = NULL;
QMutex GClockThread::m_mutex;
const char GClockThread::m_phasesTable[][3] = {"18", "19", "1A", "1B", "1C", "1D", "1E", "1F"};
unsigned char GClockThread::m_iPhaseIndex = static_cast<unsigned char>(GClockThread::ePhaseMin);


GClockThread::GClockThread() :
	m_bRunning(true)
{
	LOG_LEVEL4("GClockThread()");
}

GClockThread::~GClockThread()
{
	LOG_LEVEL4("~GClockThread()");
}

GClockThread * GClockThread::Instance()
{
	return m_pInstance;
}

GClockThread * GClockThread::CreateSoleInstance()
{
	LOG_LEVEL4("CreateSoleInstance()");

	m_mutex.lock();

	if (!m_pInstance)
	{
		m_pInstance = new (std::nothrow) GClockThread();
	}

	m_mutex.unlock();

	return m_pInstance;
}

void GClockThread::RemoveSoleInstance()
{
	LOG_LEVEL4("RemoveSoleInstance()");

	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void GClockThread::SetCoordinates(int x_pos, int y_pos)
{
	m_iXpos = x_pos;
	m_iYpos = y_pos;
}

void GClockThread::Stop()
{
	LOG_LEVEL4("Stop()");

	m_bRunning = false;

	// It will block until the thread has finished. 
	// When the thread returns from run().
	QThread::wait();
}

void GClockThread::run()
{
	LOG_LEVEL3("run()");

	while (m_bRunning)
	{
		QThread::msleep(GClockThread::ePhaseTime);

		QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

		QMutexLocker mtxScope(pModel->GetMutex());

		GTextField *poField;
		poField = new (std::nothrow) GTextField(0, NULL);
		if (poField)
		{
			poField->SetXpos(m_iXpos);
			poField->SetYpos(m_iYpos);
			poField->SetAttribute(0);
			poField->SetFont(0);
			poField->SetText(rotatePhase());

			pModel->UpdateViewSection(poField);
		}
		else
		{
			LOG_LEVEL4("Error allocating memory for GTextField.");
		}
	}

	// Tells the thread to exit succefully.
	QThread::quit();
}

QString GClockThread::rotatePhase()
{
	if (m_iPhaseIndex > static_cast<unsigned char>(GClockThread::ePhaseMax))
	{
		m_iPhaseIndex = static_cast<unsigned char>(GClockThread::ePhaseMin);
	}

	QByteArray oText = QByteArray::fromHex(m_phasesTable[m_iPhaseIndex]);

	m_iPhaseIndex++;

	return QString(oText);
}

