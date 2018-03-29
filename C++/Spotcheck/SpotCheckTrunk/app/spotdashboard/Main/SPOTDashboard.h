#ifndef SPOTDashboard_h
#define SPOTDashboard_h

/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file SPOTDashboard.h
 * @author Volker Mariano
 * @date Aug 26 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


class GSPOTSession;


int  SPOTDashboard_init();
int  SPOTDashboard_exit(GSPOTSession *pSession);
int  SPOTDashboard_Reconnect();

#endif
