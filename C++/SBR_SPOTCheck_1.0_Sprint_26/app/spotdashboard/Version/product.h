#ifndef PRODUCT_H
#define PRODUCT_H

// Note - One and only one module in the entire program should
// define MAKE_COPYRIGHT_CODE. 
// This will produce a string of ASCII characters in case someone
// is scanning the binary file distrubuted (as required by the
// copyright standard).

#ifdef MAKE_PRODUCT_CODE
char gszProduct[] = " SPOTDashboard Diagnostics ";
#else
extern char gszProduct[];
#endif

#endif
