//========================================================================
//
// File:		LicFileUtils.cpp
//
// Description:	Licence File utilities
//
// Created:		7 Jul 2007
//
// Author:		Albex srl
//
//========================================================================

#include <time.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LicFileUtils.h"

#ifndef byte
typedef unsigned char byte;
#endif

struct LicFileData lfd;

char szSx[8][SXCERTDIM];
int iSxFields;

/*

Lic file format

  <string>
  <record_1>

  <record_K>

where :
	<string>::= <SERNUM><\0>		file is resource
				or
				<SERNUM><blank><S1><blank><S2><blank>...<SM><\0> file is application


	<record_i>::=<CERT>	4 bytes len  file is resource
				 <CERT><CERT1><CERT2>....<CERTM> file is application

	<CERT>::= 4 bytes certificate
	<CERT1>::= 8 byte certificate one for each Si in <string>

  resource file .lic has lenght = [ SERNUMLEN+ K*4 ],    
  application file .lic has lenght = [ SERNUMLEN+ K*(4+M*8) ]

  where K is the number of valid certificates, I.e. for spot starting from SERNUM to SERNUM+K. 
  and M is the number of SM strings after SERNUM

  <record_1> is at position in file = to len of <string> field and MUST be calculated

  */


/*
	whichLen 

	returns the len of the file pointed by FILE *pFile

	-1 if some error occurs
	pFile must be obviously opened

*/
long WhichLen( FILE *pFile )
{
   struct stat buf;
   int fd, result;

   fd = fileno(pFile);
	if ( fd!=-1 )
	{
	   // Get data associated with "fd": 
	   result = fstat( fd, &buf );
	   if ( result!=-1 ) 
	   {
		   return buf.st_size;
	   }
	}
	return(-1L);
}

/*
	LicHeaderTokenCount 

	returns the number of valid tokens contained in buffer

	Return: 
		0 if no valid token
		1 if only first field present (sernum only in header)
		number of tokens found

*/
int LicHeaderTokenCount ( char * pBuf )
{
	int i=0;
	char* token;
	char *p=pBuf;
	while ( (token = strtok( p, " " ))!=NULL )
	{
		// after first call pointer param must be nullified
		p=NULL;
		i++;
	} 
	// if no valid token i=0, else if only first sernum i=1, else i=numb of tokens
	return( i );
}

/* 
	LicFileFetch 

	opens the .Lic file szFname and search for the szSpotSerNum certificates.

    Return: eBadFile	incorrect file
			eBadString	incorrect string format
			eBadSerNum	invalid serial number for this lic file
			eNoMem		can't return data buffer with certificates

			eFileApplication  lic file is for application
			eFileResource lic file is for Resource

	if return is eFileApplication or eFileResource AND pStruct is not NULL
   then the pStruct contains pointer to LicFileData struct where 

	 pStruct must be freed by the caller of this function invokibg LicFileFree

	pStruct[]->szName contains a string that identifies the type of certificate, for example "MAC"
	pStruct[]->byArr contains the certificate 

	iNumElems contains the number of LicFileData elements defined for pStruct

	pStruct[0]->szName="MAC"
	pStruct[0]->byArr=CERT

	pStruct[i]->szName="DY" | "KB" | "RD" | ...
	pStruct[i]->byArr=CERTx

   where i=1..iNumElems

*/
LicFileStatus LicFileFetch(const char *szFname, const char *szSpotSerNum, struct LicFileData * &pStruct, int *iNumElems)
{
	FILE *pFile;
	char buffer [256];
	LicFileStatus Ret = eBadFile;
	int i = 0;

	pStruct=NULL;
	memset(buffer, 0x00, sizeof(buffer));
	// Build lic file name
	snprintf((char *)buffer, sizeof(buffer)-strlen(".lic"), "%s", szFname);
	// add lic extension to file
	strcat((char *)buffer,".lic");
	
	if ( (pFile=fopen((char *)buffer,"rb")) != NULL )
	{
		Ret =  LicFileProcess(pFile, szSpotSerNum, pStruct, iNumElems);
		if (Ret == eFileResource || Ret == eFileApplication)
			return Ret;
	}
	
	while (i < 99 && Ret != eFileResource && Ret != eFileApplication)
	{
		// Build lic file name
		snprintf(buffer, sizeof(buffer), "%s%s%d", szFname, ".lic", i);
		if ( (pFile=fopen((char *)buffer,"rb")) != NULL )
		{
			Ret =  LicFileProcess(pFile, szSpotSerNum, pStruct, iNumElems);
			if (Ret == eFileResource || Ret == eFileApplication)
				return Ret;
		}
		++i;
	}

	// no existing .lic file
	return( eBadFile );
}


/*
	LicFileFree

   utility to free struct LicFileData *
*/
void LicFileFree( struct LicFileData *plfd )
{
	if ( plfd!=NULL )
		free(plfd);
	
	plfd=NULL;
}

/*
	LicFileGetMac

	Return pointer to Certificate field 
*/
byte * LicFileGetMac(struct LicFileData *pslfd)
{
	return(pslfd->byArr);
}

/*
	LicFileGetSwCert
	
	Return the sw certificate related to the ID string
*/
byte * LicFileGetSwCert(struct LicFileData *pslfd, char *id)
{
	for ( int i=1; i<8; i++, pslfd++)
	{
		if ( strncmp(pslfd->szName, id,8)!=0 )
			continue;
		return(pslfd->byArr);

	}
	return(NULL);
}

LicFileStatus LicFileProcess(FILE *pFile, const char *szSpotSerNum, struct LicFileData * &pStruct, int *iNumElems)
{
	unsigned char buffer1[128];
	unsigned long dwLen;
	long lSerNumBase, lSerNumSpot, lPos;
	int i, iDim;
	LicFileStatus eStat;

	//  get len
	dwLen=WhichLen( pFile );
	if ( dwLen!=0 )
	{
		// read a block and rewind to head
		int iRes;
		memset(buffer1,0,sizeof(buffer1));
		iRes = fread(buffer1,sizeof(char),sizeof(buffer1)-2,pFile);
		if ( iRes == 0 )
		{
			fclose(pFile);
			return(eBadFile);
		}
		else
		{
			// something got; search len of initial ascii string
			int iLen=strlen((const char *)buffer1);
			if ( iLen==(sizeof(buffer1)-2) )
			{
				// something wrong a <\0> must be present earlier in string
				fclose(pFile);
				return(eBadString);
			}
			// valid len found rewind and seek to end of initial string; +1 for the terminating \0
			iRes=fseek(pFile,(long)(iLen+1),SEEK_SET);
			if ( iRes!=0 )
			{
				// something wrong can't seek
				fclose(pFile);
				return(eBadFile);
			}
			// iLen is good at least SERNUM is present
			// token-ize the string
			char* token = strtok( (char *)buffer1, " " );
			if ( token == NULL )
			{
				// bad string
				fclose(pFile);
				return(eBadString);
			}
			// SERNUM is good store first SpotSerialNumber valid and requested
			lSerNumBase=atol(token);
			if ( lSerNumBase == 0 )
			{
				// bad SERNUM
				fclose(pFile);
				return(eBadSerNum);
			}
			lSerNumSpot = atol(szSpotSerNum);
			// invalid Serial Number for the file
			if ( lSerNumSpot<lSerNumBase )
			{
				fclose(pFile);
				return(eBadSerNum);
			}
			if ( iLen > SERNUMLEN )
			{
				// SERNUM and OTHERS separated by blank
				// how many Si are? Counts how many 
				for  ( i=0; i<8; i++)
				{
					memset(szSx[i],0,SXCERTDIM);
					token = strtok( NULL, " " );
					if ( token != NULL )
					{
						strncpy(szSx[i],token,SXCERTDIM);
					}
					else
						break;

				}
				iSxFields=i;
				// clean up Sx buffers
				for ( ; i<8; i++ )
					memset(szSx[i],0,SXCERTDIM);
				//  no additional fileds ? mmmmmmh! Should be an error
				if ( iSxFields==0 )
				{
					fclose(pFile);
					return(eBadString);
				}
				// all good save what we are doing
				eStat=eFileApplication;
				
			}
			else
			{
				// only SERNUM
				iSxFields=0;
				// all good save what we are doing
				eStat=eFileResource;
			}

			// now calculates where to seek into file relative to current position
			// to retrieve the certificate record for the requested SpotSerNum
			lPos = (lSerNumSpot-lSerNumBase);
			// lPos is the index to the <record_i> to retrieve, 0 based value 
			// ( same SN as SERNUM has lPos = 0 )
			if ( eStat==eFileResource)
			{
				iDim = 4;
			}
			else if ( eStat==eFileApplication )
			{
				iDim = 4 + iSxFields*SXCERTDIM;
			}
			lPos *= iDim;
			// and now seek to the relative position in file
			iRes=fseek(pFile,lPos,SEEK_CUR);
			if ( iRes!=0 )
			{
				// something wrong can't seek no valid value
				// may be there is no certificate for this SN
				fclose(pFile);
				return(eBadFile);
			}
			// position is good
			memset(buffer1,0,sizeof(buffer1));
			iRes = fread(buffer1,sizeof(char),iDim,pFile);
			if ( iRes == 0 )
			{
				// not read the record bye!
				fclose(pFile);
				return(eBadFile);
			}
			// got it ! close file
			fclose(pFile);

			// KB DY etc....
			// save in certificate structure
			struct LicFileData *p=(struct LicFileData *)malloc( (iSxFields+1)*(sizeof(struct LicFileData)) );
			if ( p==NULL)
			{
				// can't return data
				return(eNoMem);
			}
			//
			memset( p,0, (iSxFields+1) * (sizeof(struct LicFileData)) );
			pStruct = p;
			*iNumElems = iSxFields+1;
			// 1st elem is fw mac next are Dallas certs
			strcpy(p->szName,"MAC");
			memcpy(p->byArr,buffer1,4);
			unsigned char *p1 = &buffer1[4];
			for ( i=0; i<iSxFields; i++, p1+=SXCERTDIM )
			{
				p++;
				strncpy(p->szName, szSx[i], 4);
				memcpy(p->byArr, p1, SXCERTDIM);
			}

			// and return good status
			return(eStat);
		}
	}
	return eBadFile;
}
