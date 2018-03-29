#ifndef GURLView_h
#define GURLView_h

#include <QMutex>
#include <QSharedPointer>

#include "view/src/base/GAbsView.h"
#include "utils/base/src/GException.h"

/**
 * GURLView
 *
 * View Singleton- It gets an Url.
 * This inherits from GAbsView
 * This is the suscriber from suscriber-publisher pattern.
 */

class GURLView : public GAbsView
{
public:

	/**
	 * Gets an instance from Singleton-
	 * Calls constructor.
	 */
	static QSharedPointer<GURLView> GetInstance() throw (GException);

	/**
	 *
	 * Destructor
	 *
	 */
	virtual ~GURLView();

protected:

	/**
	 * Constructor
	 *
	 */
	GURLView();

	/**
	 * Init()
	 *
	 */
	void Init();

public:

	/**
	 * Update()
	 *
	 * @param pModel GAbsModel pointer
	 *
	 */
	void Update(bool bSynchronize = false);

private:

	/**
	 * Instance pointer;
	 *
	 */
	static QSharedPointer<GURLView> m_pGURLView;

	/**
	 * mutex singleton
	 *
	 */
	static QMutex m_mutex;
};

#endif
