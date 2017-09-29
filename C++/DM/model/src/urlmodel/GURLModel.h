/* 
 * File:   GURLModel.h
 * Author: gtomasini
 *
 * Created on 23 de julio de 2009, 12:01
 */

#if !defined(GURLModel_h)
#define	GURLModel_h

#include <QString>
#include <QMutex>
#include "model/src/base/GAbsModel.h"

/**
 *
 * URL Model.
 * This is the publisher from the suscriber-publisher model.
 * This class instance should be connected to the suscriber instance
 * by means of qt connect.
 */
class GURLModel : public GAbsModel
{

public:
	/*
	 * Constructor
	 * Moves to the main thread.
	 */
	GURLModel();

	/*
	 * Destructor
	 *
	 */
	virtual ~GURLModel();

	/*
	 * Suscribers notifiers
	 *
	 */
	void UpdateView(bool bSynchronize = false);

	/*
	 * Url setter.
	 *
	 * @param qUrl QUrl reference.
	 *
	 */
	void SetUrl( QString const & sUrl );


	/*
	 * Url() getter.
	 *
	 * @return QUrl reference.
	 */
	QString const & GetUrl() const;
        
	QMutex *GetMutex();

private:
	/*
	 * Url string
	 *
	 */
	QString m_sUrl;
	QMutex m_mutex;

};


#endif

