//
//        File:    GStringMatch.cpp
//
//        Created: May 24,2007
//
//        Author:  Phil Robertson
//
//        Description:
//            Implementation of GStringMatch class
//

#include "GStringMatch.h"


//
// constructor, sets pattern to match anything
//
GStringMatch::GStringMatch()
{
	SetPattern("*");
}


//
// constructor, setting pattern to given string
//
GStringMatch::GStringMatch(const string pattern)
{
	SetPattern(pattern);
}


//
// destructor
//
GStringMatch::~GStringMatch()
{
}


//
// sets current pattern to match string against
//
void GStringMatch::SetPattern(const string pattern)
{
	m_sPattern = pattern;
}


//
// gets current pattern to match string against
//
string GStringMatch::GetPattern()
{
	return m_sPattern;
}


//
// see if given string matches the current pattern
//
bool GStringMatch::Matches (const string &stringToCompare)
{
	const char * compare = stringToCompare.c_str();
	const char * pattern = m_sPattern.c_str();
	int          len     = stringToCompare.length();

/** check for empty string **/
	if ( len < 1 )
		return (false);

/** leading "." must be explicit **/
	if (*compare == '.')
	{
		if (*pattern != '.')
			return (false);

		pattern++;
		compare++;
		len--;
	}

	return ( matchstr (compare, pattern, len) );
}


//
// static method to expose a simple one time string match request
//
bool GStringMatch::StringMatches (const string stringToCompare, const string pattern)
{
	const char * compare = stringToCompare.c_str();
	const char * pat     = pattern.c_str();
	int          len     = stringToCompare.length();

/** check for empty string **/
	if ( len < 1 )
		return (false);

/** leading "." must be explicit **/
	if (*compare == '.')
	{
		if (*pat != '.')
			return (false);

		pat++;
		compare++;
		len--;
	}

	return ( matchstr (compare, pat, len) );
}


//
// static "C" method that exposes original "range match test"
//
const char * GStringMatch::matchrng (int c, const char * p)
{
	bool escaped = false;
	bool matched = false;
	bool notflag = false;

	switch (*++p)
	{
		case GS_RANGE:
			escaped = true;
			break;

		case GS_NOT  :
			notflag = true;
			p++;
			break;
	}

	while (*p)
	{
		if (*p == GS_END)
		{
			if (notflag ^ matched)
				return (p);
			else
				return (NULL);
		}

		if (! matched )
		{
			if (escaped)
			{
				matched = (c == *p);
				escaped = false;
			}
			else
			{
				switch (*p)
				{
					case GS_ESCAPE:
						escaped = true;
						break;

					case GS_RANGE :
						if ( (*(p - 1) < c) && (c < *(p + 1)) )
							matched = true;
						break;

					default       :
						matched = (c == *p);
						break;
				}
			}
		}

		p++;
	}

	/** invalid pattern **/
	return (NULL);
}


//
// static "C" method that exposes original "string match test"
//
bool GStringMatch::matchstr (const char * s, const char * p, int n)
{
	bool escaped = false;
	bool anystr  = false;

	while ((n > 0) && *s)
	{
	/** is it the end of the pattern? **/
		if (*p == 0)
			return (anystr);

		if (! anystr)
		{
			if (escaped)
			{
				if (*s != *p)
					return (false);
				escaped = false;
			}
			else
			{
				switch (*p)
				{
					case GS_ESCAPE:
						escaped = true;
						break;

					case GS_ANYSTR:
						anystr = true;
						break;

					case GS_ANYCHR:
						break;

					case GS_BEGIN :
						if (!(p = matchrng(*s, p)))
							return (false);
						break;

					default       :
						if (*s != *p)
							return (false);
				}
			}
			p++;
		}

		if (anystr)
		{
	/** after finding "*" try to match the rest of the string **/
			if (matchstr (s, p, n))
				return (true);
		}

		s++;
		n--;
	}

	/** allow trailing "*" 's **/
	while (*p == GS_ANYSTR)
		p++;

	return (*p == 0);
}
