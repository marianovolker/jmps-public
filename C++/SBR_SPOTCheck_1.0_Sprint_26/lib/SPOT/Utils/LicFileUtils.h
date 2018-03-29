//========================================================================
//
// File:		LicFileUtils.h
//
// Description:	Licence File utilities
//
// Created:		7 Jul 2007
//
// Author:		Albex srl
//
//========================================================================
#ifndef LICFILEUTILS_H
#define LICFILEUTILS_H

#define SERNUMLEN  8		// len of serial number field
#define SXCERTDIM	8		// len of certificate for additional fields ( KB RD DY )

enum LicFileStatus {
		eNoMem=-4,
		eBadSerNum=-3,
		eBadString=-2,
		eBadFile=-1,
		eGoodLic=0,
		eFileResource,
		eFileApplication,
		eMaxStatuses,

};

struct LicFileData {
	char szName[16-SXCERTDIM];
	unsigned char byArr[SXCERTDIM];
};



long WhichLen( FILE *pFile );
LicFileStatus LicFileFetch(const char *szFname, const char *szSpotSerNum, struct LicFileData * &pslfd, int * iNum );
unsigned char * LicFileGetMac(struct LicFileData *pslfd);
void LicFileFree( struct LicFileData *plfd );
LicFileStatus LicFileProcess(FILE *pFile, const char *szSpotSerNum, struct LicFileData * &pStruct, int *iNumElems);


#endif
