//
//        File:    GStringMatch.h
//
//        Created: May 24,2007
//
//        Author:  Phil Robertson
//
//        Description:
//          Implements simple "borne shell" like pattern match algorithm to match strings. Each
//          "*" in the pattern results in one level of recursion but stack usage is relatively
//          minimal. For instance "[A-Z]*[cC]" matches all strings that start with an upper case
//          letter and end with upper or lower case "c".
//

#ifndef GStringMatch_h
#define GStringMatch_h

#include <string>

#define GS_ESCAPE '\\'       /* take the next character literally   */
#define GS_ANYSTR '*'        /* match any string (except empty)     */
#define GS_ANYCHR '?'        /* match any single character          */
#define GS_BEGIN  '['        /* begin match of enclosed characters  */
#define GS_END    ']'        /* end match of enclosed characters    */
#define GS_RANGE  '-'        /* match a range of characters in "[]" */
#define GS_NOT    '!'        /* if "!" after "[" not inclusive      */

using namespace std;

class GStringMatch
{
public:
	GStringMatch ();

	GStringMatch (const string pattern);

	~GStringMatch ();

	// sets current pattern to match string against
	void SetPattern (const string pattern);

	// gets current pattern to match string against
	string GetPattern ();

	// does the string match the current pattern?
	bool Matches (const string &stringToCompare);

	// static method to expose a simple one time string match request
	static bool StringMatches (const string stringToCompare, const string pattern);

protected:
	// static "C" method that exposes original "range match test"
	static const char * matchrng (int c, const char * p);

	// static "C" method that exposes original "string match test"
	static bool matchstr (const char * s, const char * p, int n);

private:
	// the pattern
	string m_sPattern;
};

#endif
