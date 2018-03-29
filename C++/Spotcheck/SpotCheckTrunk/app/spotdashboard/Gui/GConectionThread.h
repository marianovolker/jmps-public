/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GConectionThread.h
 * @author Mariano Volker
 * @date Dec 09 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GConectionThread_h
#define GConectionThread_h 1

// wx stuff
#include <wx/frame.h>
#include <wx/thread.h>
#include <wx/menu.h>
#include <wx/app.h>


class GConectionThread : public wxThread
{
    wxFrame* m_parent;

public:
    GConectionThread(wxFrame* parent):m_parent(parent){};
    virtual ~GConectionThread(){};

    virtual ExitCode Entry();


    // the ID we'll use to identify our event
    static const int CONNECTION_STATUS_EVENT_ID;
    static const int CONNECTION_STATUS_OK;
    static const int CONNECTION_STATUS_FAIL;
};

#endif
