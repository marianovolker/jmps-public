#ifndef COPYRIGHT_H
#define COPYRIGHT_H
// ===================================================
// This copyright file is made in accordance with the
// ISO 9000 Standard GIL-ENG-S022
//
//
// ===================================================

// Note - One and only one module in the entire program should
// define MAKE_COPYRIGHT_CODE. 
// This will produce a string of ASCII characters in case someone
// is scanning the binary file distrubuted (as required by the
// copyright standard).

#ifdef MAKE_COPYRIGHT_CODE
char gszCopyRight[] = " Copyright Gilbarco Inc 2012 ";
#else
extern char gszCopyRight[];
#endif


#endif
