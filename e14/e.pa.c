/*
/* file e.pa.c: command line parsing routines
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#ifdef UNIXV7
#include <ctype.h>
#endif
#include "e.h"
#include "e.m.h"
#include "e.cm.h"

extern Flag rplinteractive, rplshow;
extern
S_looktbl rpltable[],
	  filltable[];

/* getpartype (str, parflg, defparflg, parline)
/*       - parse for things like "3p", "3l", "45", "45x8" etc.
/*      When a number of paragraphs is specified, the "parline" argument
/*      is used as the first line of the paragraph parsing.
/*      If parflg != 0, then allow paragraph specs.
/*      If defparflg != 0, then default is paragraphs.
/*      str is the address of a pointer to the first character
/*      returns:
/*      0: *str is empty (== "")
/*      1: one number found, and its value is stored in parmlines
/*            parmlines will be < 0 if it means num of paragraphs.
/*      2: two numbers found - stored in paramlines and paramcols
/*      3: not recognizable as 0, 1, or 2
/*
/*      if 1 or 2, then pstline has starting line # and
/*          pstcol has starting col /* not implemented yet */
/*      str is set to next word if this word returns 1 or 2
/**/
getpartype (str, parflg, defparflg, parline)
char **str;
Flag    parflg,
	defparflg;
Nlines  parline;
{
    register char *cp,
		  *cp1;
    int tmp;    /* must be int for s2i */
    Nlines plines;

    cp = *str;
    if (*cp == 0)
	return 0;   /* no argument at all */
    for (; *cp && *cp == ' '; cp++)
	{}
    cp1 = cp;
    cp = s2i (cp, &tmp);
    if (cp == cp1)
	return 3;   /* string */
    if (parflg && (*cp == 'p' || *cp == 'P')) {
	cp++;
	plines = lincnt (parline, -tmp);
    }
    else if (*cp == 'l' || *cp == 'L') {
	cp++;
	plines = tmp;
    }
    else if (defparflg)
	plines = lincnt (parline, -tmp);
    else
	plines = tmp;
    if (*cp == ' ' || *cp == 0) {
	for (; *cp && *cp == ' '; cp++)
	    {}
	*str = cp;
	parmlines = plines;
	return 1;
    }
    if (*cp != 'x' && *cp != 'X')
	return 3;
    cp++;
    cp1 = cp;
    cp = s2i (cp, &tmp);
    if (cp == cp1)
	return 3;   /* string */
    if (*cp == ' ' || *cp == 0) {
	for (; *cp && *cp == ' '; cp++)
	    {}
	*str = cp;
	parmlines = plines;
	parmcols = tmp;
	return 2;
    }
    return 3;
}

/*  lookup (name, table) - lookup name in table.  Will take nonambiguous
/*      abbreviations.  If you want to insist that a certain table entry
/*      must be spelled out, enter it twice in the table.
/*      Table entries must be sorted by name, and a name which is a
/*      substring of a longer name comes earlier in the table.
/*      Accepts upper or lower case if table entry is lower case.
/*
/* returns:
/*   > 0 table entry index
/*    -1 not found
/*    -2 ambiguous
/**/
#define MAXLUPN 20
lookup (name, table)     /* table entries must be sorted by name */
char *name;
S_looktbl *table;
{
    register char  *namptr,
		   *tblptr;
    Small ind;
    Small value = 0;
    Small length;
    Small longest = 0;
    Flag ambig = NO;
    char lname[MAXLUPN];

    if (name == NULL)
	return -1;
    namptr = name;
    tblptr = lname;
    for (;;) {
	if ((*tblptr++ = isupper (*namptr)? tolower (*namptr++): *namptr++)
	    == '\0')
	    break;
	if (tblptr >= &lname[MAXLUPN])
	    return -1;
    }

    for (ind = 0; (tblptr = table->str) != 0; table++, ind++) {
	namptr = lname;
	for (; *tblptr == *namptr; tblptr++, namptr++) {
	    if (*tblptr == '\0')
		break;
	}
	if (*namptr == '\0') {  /* end of name or exact match */
	    length = namptr - lname;
	    if (longest < length) {
		longest = length;
		ambig = NO;
		value = ind;
		if (*tblptr == '\0')
		    break;          /* exact match */
	    }
	    else /* longest == length */
		ambig = YES;
	}
	else if ( *namptr < *tblptr )
	    break;
    }
    if (ambig)
	return -2;
    if (longest)
	return value;
    return -1;
}

/*  getword (str) - finds the first non-blank in *str then finds the first
/*                  blank or '\0' after that, then allocs enough space for
/*                  the 'word' thus delimited, copies it into the alloced
/*                  space, and null-terminates the new string.
/*                  If the returned string is the null string (not a null
/*                  pointer) then it was NOT alloced.
/*      returns a pointer to the new string and makes *str to point to the
/*      next 'word'.
/**/

char *
getword (str)
char **str;
{
    register char *cp1,
		  *cp2,
		  *cp3;
    char *newstr;

    if (*str == 0)
	return "";
    for (cp1 = *str; *cp1 == ' '; cp1++)
	{}
    for (cp2 = cp1; *cp2 && *cp2 != ' '; cp2++)
	{}
    for (cp3 = cp2; *cp3 == ' '; cp3++)
	{}
    *str = cp3;
    if (cp2 == cp1)
	return "";
    newstr = salloc (cp2 - cp1 + 1, YES);
    for (cp3 = newstr; cp1 < cp2; )
	*cp3++ = *cp1++;
    *cp3 = '\0';
    return newstr;
}

/* The following handfull of parsing routines are awful. Sorry.  DY */

/* scanopts scans for options from a table of options.
/*
/*  You may not be at end of string when this routine returns >= 0
/*
/*  returns:
/*       3 marked area        \
/*       2 rectangle           \
/*       1 number of lines      > may have stopped on an unknown option
/*       0 no area spec        /
/*      -2 ambiguous option     ) see parmlines, parmcols for type of area
/*   <= -3 other error
/**/
scanopts (cpp, table, pardefault)
char **cpp;
S_looktbl *table;
Flag pardefault;
{
    Small areatype;
    int tmp;

    if (curmark)
	areatype = 3;
    else
	areatype = 0;
    parmlines = 0;
    parmcols  = 0;
    for (;;) {
	tmp = getpartype (cpp, YES, pardefault, curwksp->wlin + cursorline);
	switch (tmp) {
	case 0: /* end of string */
	    return areatype;

	case 1:
	case 2:
	    if (areatype) {
		if (areatype == 3)
		    return CRMARKCNFL;
		else
		    return CRMULTARG;
	    }
	    areatype = tmp;
	    break;

	default:
	    tmp = getopteq (cpp, table);
	    if (tmp <= -2)
		return tmp;
	    if (tmp != 1 )
		return areatype;
	}
    }
}

/* getopteq (str, optiontable)
/*
/*  look for things like width=XXX
/*
/*  returns 1: valid option value stuffed in global
/*          0: end of string
/*         -1: option not found
/*         -2: ambiguous option
/*        < 0: other error
/**/
getopteq (str, table)
char **str;
S_looktbl *table;
{
    Small tmp;
    char *acp;
    register char *cp;
    register char *cp1;
    Char svchr;
    Flag equals;

    /* skip over blanks */
    for (cp = *str; *cp && *cp == ' '; cp++)
	{}
    cp1 = cp;
    equals = NO;
    /* delimit a word */
    for (; *cp && *cp != ' '; cp++)
	if (*cp == '=') {
	    equals = YES;
	    break;
	}
    if (cp != cp1) {
	svchr = *cp;
	*cp = '\0';
	tmp = lookup (cp1, table);
	*cp = svchr;
	if (tmp < 0)
	    return tmp;
	if (table == filltable) {
	    acp = cp;
	    if (tmp == 0) { /* "width" table entry */
		tmp = doeq (&acp, &tmplinewidth);
		if (tmp == 0) {
		    *str = acp;
		    return 1;
		}
		return tmp;
	    }
	    return -1;
	}
	else if (table == rpltable) {
	    if (equals)
		return CRBADARG;
	    if (tmp >= 2)
		return -1;
	    if (tmp == 0)
		rplinteractive = YES;
	    else if (tmp == 1)
		rplshow = YES;
	    for (; *cp && *cp == ' '; cp++)
		{}
	    *str = cp;
	    return 1;
	}
	else
	    return CRBADARG;
    }
    else if (equals)
	return -1;
    else
	return 0;
}

/* doeq
/*  Returns:    0 good value, and it was stuffed
/*            < 0 error type
/**/
doeq (cpp, value)
char **cpp;
int *value;
{
    char *cp;
    char *cp1;
    int tmp;

    cp = *cpp;
    if (*cp++ != '=')
	return CRNOVALUE;
    cp1 = cp;
    cp = s2i (cp1, &tmp);
    if (cp == cp1)
	return CRNOVALUE;
    if (*cp == ' ' || *cp == '\0') {
	for (; *cp && *cp == ' '; cp++)
	    {}
	*value = tmp;
	*cpp = cp;
	return 0;
    }
    return CRBADARG;
}


