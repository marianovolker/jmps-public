#ifndef SPOTCheck_h
#define SPOTCheck_h

/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file SPOTCheck.h
 * @author Vance Tate
 * @date Nov 10 2012
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


class GSPOTSession;

int  SPOTCheck_init();
int  SPOTCheck_exit(GSPOTSession *pSession, int iShortWay = 0);

#endif
