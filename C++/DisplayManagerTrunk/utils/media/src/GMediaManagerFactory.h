/* 
 * File:   MediaManagerFactory.h
 * Author: sorgius
 *
 * Created on March 30, 2011, 2:14 PM
 */

#ifndef GMEDIAMANAGERFACTORY_H
#define	GMEDIAMANAGERFACTORY_H

#include <QStringList>
#include <QSharedPointer>
#include "GAbsMediaManager.h"


class GMediaManagerFactory {
public:
    GMediaManagerFactory();
    QSharedPointer<GAbsMediaManager> CreateMediaManager();
    static bool IsAValidMediaManagerType(QString type);
    static bool IsAValidForkFailureOption(QString option);
    static QStringList GetMediaManagerList();
    static QStringList GetForkFailureOptionList();

    virtual ~GMediaManagerFactory();

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GMediaManagerFactory( const GMediaManagerFactory& gMediaManagerFactory );
    GMediaManagerFactory& operator=( const GMediaManagerFactory& gMediaManagerFactory );

};

#endif	/* GMEDIAMANAGERFACTORY_H */

