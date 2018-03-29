/* 
 * File:   MediaManagerFactory.cpp
 * Author: sorgius
 * 
 * Created on March 30, 2011, 2:14 PM
 */

#include "GMediaManagerFactory.h"
#include "GGstLaunchMediaManager.h"
#include "GGstDaemonMediaManager.h"
#include "config/GDisplayManagerConfig.h"

const char *CDaemon = "daemon";
const char *CGstlaunch = "gstlaunch";
const char *CEnabled = "Enabled";
const char *CDisabled = "Disabled";

GMediaManagerFactory::GMediaManagerFactory() {
}

/*
 * Factory method to create a singleton of a concrete MediaManager type.
 *   The type returned depends on what is set in the configuration file.
 */
QSharedPointer<GAbsMediaManager> GMediaManagerFactory::CreateMediaManager()
{
    // Read the correct MediaManager type to create from the config file
    QString type = GETCONF_STR_FROM_FILESYSTEM(eDisplayManagerMediaManagerType);


    if (QString(CGstlaunch) == type) {
        return GGstLaunchMediaManager::GetInstance();
    } else if (QString(CDaemon) == type) {
        return GGstDaemonMediaManager::GetInstance();
    } else {
        // We will make the Daemon version the default if not specified
        // correctly in the config file
        return GGstDaemonMediaManager::GetInstance();
    }

}


bool GMediaManagerFactory::IsAValidMediaManagerType(QString type)
{
    if (QString(CGstlaunch) == type) {
        return(true);
    } else if (QString(CDaemon) == type) {
        return(true);
    } else {
        return(false);
    }
}


bool GMediaManagerFactory::IsAValidForkFailureOption(QString option)
{
    if (QString(CEnabled) == option) {
        return(true);
    } else if (QString(CDisabled) == option) {
        return(true);
    } else {
        return(false);
    }
}

// This is the list of available Media Managers.  It is passed to 
// CRIND Diagnostics so the user can choose which one will be used.
// Make sure to update this if we ever add another media manager!
QStringList GMediaManagerFactory::GetMediaManagerList()
{
	QStringList mediaManagerList;
        mediaManagerList << CGstlaunch << CDaemon;
        return mediaManagerList;
}

// This is the list of available Fork Failure Options.  It is passed to
// CRIND Diagnostics so the user can choose which one will be used.
// Make sure to update this if we ever add another fork failure option!
QStringList GMediaManagerFactory::GetForkFailureOptionList()
{
	QStringList forkFailureOptionList;
	forkFailureOptionList << CEnabled << CDisabled;
    return forkFailureOptionList;
}

GMediaManagerFactory::~GMediaManagerFactory() {
}

