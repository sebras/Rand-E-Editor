/*
/* file e.se.c: search (..) and replace ()
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

#ifdef UNIXV6
#define ispunct(c) (040 < c && c < 0177 && !(isdigit(c) || isalpha(c)))
#endif

#define FOUND    0
#define NOTFOUND 1
#define ABORTED  2

Flag rplinteractive, rplshow;
static char *rpls1;
static char *rpls2;
static Ncols rpls1len;
static Ncols rpls2len;
static Flag starthere;
S_looktbl rpltable[] = {
    "interactive" , 0,
    "show"        , 0,
    0             , 0
};

Slines srchline;
Scols srchcol;

replace (delta)
Small delta;
{
    register char *cp;
    char *s1;
    char *s2;
    char *fcp;
    Char delim;
    int nl;
    int tmp;
    Ncols s1len;
    Ncols s2len;
    Flag moved;

    if (*(fcp = cmdopstr) == '\0')
	return CRNEEDARG;
    moved = NO;
    nl = 1;
    rplshow = NO;
    rplinteractive = NO;
    tmp = scanopts (&fcp, rpltable, NO);
/*       3 marked area        \
/*       2 rectangle           \
/*       1 number of lines      > may have stopped on an unknown option
/*       0 no area spec        /
/*      -2 ambiguous option     ) see parmlines, parmcols for type of area
/*   <= -3 other error
/**/
    if (tmp <= -2)
	return tmp;
    if (*(cp = fcp) == '\0') {
	mesg (ERRALL + 1, "no search string");
	return CROK;
    }
    switch (tmp) {
    case 0:
	if (delta > 0)
	    nl = max (0, nlines[curfile] - (curwksp->wlin + cursorline))+ 1;
	else
	    nl = curwksp->wlin + cursorline + 1;
	break;

    case 1:
	nl = parmlines;
	break;

    case 2:
	return NORECTERR;
	break;

    case 3:
	if (markcols)
	    return NORECTERR;
	nl = marklines;
	break;
    }
    if (!ispunct (*cp)) {
	mesg (ERRALL + 1, "invalid string delimiter");
	return CROK;
    }
    delim = *cp++;
    s1 = cp;
    for (; *cp && *cp != delim; cp++)
	{}
    if (*cp == '\0') {
	mesg (ERRALL + 1, "unterminated search string");
	return CROK;
    }
    if (cp++ - s1 == 0) {
	mesg (ERRALL + 1, "null search string");
	return CROK;
    }
    s2 = cp;
    for (; *cp && *cp != delim; cp++)
	{}
    if (*cp == '\0') {
	mesg (ERRALL + 1, "unterminated replacement string");
	return CROK;
    }
    fcp = cp++;
    for (; *cp && *cp == ' '; cp++)
	{}
    if (*cp != '\0') {
	mesg (ERRALL + 1, "extraneous stuff after replacement string");
	return CROK;
    }
    s2[-1] = '\0';
    if ((s1len = skeylen (s1, YES, NO, NO)) == -1) {
	mesg (ERRALL + 1, "imbedded newline in search string");
	if (moved)
	    putupwin ();
	goto r2;
    }
    /* By the way, it's OK if s1len == 0 at this point */
    *fcp = '\0';
    if ((s2len = skeylen (s2, NO, NO, NO)) == -1) {
	mesg (ERRALL + 1, "newline in replacement string");
	if (moved)
	    putupwin ();
	goto r1;
    }

    if (rpls1) {
	sfree (rpls1);
	sfree (rpls2);
    }
    rpls1 = append (s1, "");
    rpls2 = append (s2, "");
    rpls1len = s1len;
    rpls2len = s2len;

    if (curmark) {
	moved = gtumark (YES);
	unmark ();
    }
    if (moved && rplshow || rplinteractive)
	putupwin ();
    if (rplinteractive) {
	if (searchkey)
	    sfree (searchkey);
	searchkey = append (rpls1, "");
	starthere = YES;
	dosearch (delta);
	starthere = NO;
    }
    else {
	doreplace (nl, delta, !moved || rplshow);
	if (moved && !rplshow)
	    putupwin ();
    }
r1: *fcp = delim;
r2: s2[-1] = delim;
    return CROK;
}

/* any newlines in s1 must be at the beginning and/or end of the string */
/* s1len is exclusive of newlines
/* s2 must not have any newlines in it.
/*
/*
/**/
doreplace (nl, delta, puflg)
Nlines nl;
Small delta;
Flag puflg;
{
    Nlines blimit;
    Nlines flimit;
    int srchret;
    Slines svline;
    Scols svcol;

    srchline = curwksp->wlin + (svline = cursorline);
    if (delta > 0) {
	blimit = srchline;
	flimit = min (nlines[curfile] - 1, srchline + nl - 1);
	starthere = YES;
    }
    else {
	blimit = max (0, srchline - nl - 1);
	flimit = srchline;
    }
    srchcol = curwksp->wcol + (svcol = cursorcol);

    for (;;) {
	if (delta > 0) {
	    if (srchline > flimit)
		break;
	}
	else {
	    if (srchline < blimit)
		break;
	}
	if (sintrup ())
	    goto abort;
	if (rplshow) {
	    if ((srchret = dsplsearch (rpls1, srchline, srchcol,
				       delta > 0? flimit: blimit,
				       delta, NO, YES))
		!= FOUND
	       )
		break;
	    svline = cursorline;
	    svcol  = cursorcol;
	    dobullets ();
	    setbul (YES);
	}
	else {
	    if ((srchret = strsearch (rpls1, srchline, srchcol,
				       delta > 0? flimit: blimit,
				      delta, YES)) == NOTFOUND)
		break;
	    else if (srchret == ABORTED) {
 abort:         mesg (ERRALL + 1, "Search aborted");
		csrsw = NO;
		d_put (0);
		sleep (1);
		break;
	    }
	}
	rplit (srchline, srchcol, puflg);
	if (delta > 0)
	    srchcol += rpls2len + (rpls1len == 0);
    }
    poscursor (svcol, svline);
    starthere = NO;
}

dodoit ()
{
    Nlines slin;
    Ncols  scol;

    if (!rpls1)
	mesg (ERRALL + 1, "no replacement to do");
    slin = curwksp->wlin + cursorline;
    scol = curwksp->wcol + cursorcol;
    starthere = YES;
    if (strsearch (rpls1, slin, scol, slin, 1, NO)
	 == FOUND) {
	newcurcol = cursorcol;
	rplit (slin, scol, YES);
	poscursor (newcurcol, cursorline);
    }
    else
	mesg (ERRALL + 1, "cursor is not on the string to be replaced");
    starthere = NO;
}

rplit (slin, scol, puflg)
Nlines slin;
Ncols scol;
Flag puflg;
{
    Ncols wdelta;
    Nlines lin;

    getline (slin); /* we DO need this */
    /* adjust cline to accept the replacement string */
    if ((wdelta = rpls2len - rpls1len) > 0)
	putbks (scol, wdelta);
    else if (wdelta < 0) {
	if ((ncline += wdelta) - scol > 0)
	    move (&cline[scol - wdelta], &cline[scol],
		    (unsigned int) (ncline - scol));
    }
    if (rpls2len > 0)
	move (rpls2, &cline[scol], (unsigned int) rpls2len);
    fcline = YES;
    if (   puflg
	&& (lin = slin - curwksp->wlin) >= 0
	&& lin <= curport->btext
       )
	putup (-1, lin, max (0, scol - curwksp->wcol),
	       wdelta == 0? rpls2len: MAXWIDTH);
    putline (0);
}

dosearch (delta)
Small delta;
{
    dsplsearch (searchkey,
		 curwksp->wlin + cursorline,
		  curwksp->wcol + cursorcol,
		   delta > 0? nlines[curfile] - 1: 0,
		    delta, YES, YES);
}

/* search(delta) - searches up/down current file for str, according
	as delta is -1 or 1.  If key is not on current page, positions
	port with key on top line.  Leaves cursor under key. */

dsplsearch (str, ln, stcol, limit, delta, delay, puflg)
char   *str;
Nlines  ln;
Ncols   stcol;
Nlines  limit;
Small   delta;
Flag    delay;
Flag    puflg;
{
    int     srchret;
    Nlines  lin;
    Ncols   col;
    Flag    newputup;
    Nlines  winlin;
    Ncols   wincol;
    Ncols   lkey;

    if (str == 0 || *str == 0) {
	mesg (ERRALL + 1, "Nothing to search for.");
	return NOTFOUND;
    }

    if (puflg) {
	setbul (NO);
	mesg (TELALL + 3, delta > 0 ? "+": "-", "SEARCH: ", str);
	d_put (0);
    }

    switch (srchret = strsearch (str, ln, stcol, limit,
				 delta, YES)) {
    case FOUND:
	if (puflg) {
	    winlin = curwksp->wlin;
	    wincol = curwksp->wcol;

	    newputup = NO;
	    lin = srchline - winlin;
	    if (lin < 0 || lin > curport->btext) {
		newputup = YES;
		if (curport->btext > 1)
		    lin = defplline;
		else
		    lin = 0;
		if ((winlin = srchline - lin) < 0) {
		    lin += winlin;
		    winlin = 0;
		}
	    }
	    if ((lkey = skeylen (str, YES, YES, YES)) == 0)
		lkey = 1;
	    col = srchcol;
	    if (   col < wincol
		|| col > wincol + curport->rtext + 1 - lkey
	       ) {
		newputup = YES;
		if (col < curport->rtext + 1 - lkey)
		    wincol = 0;
		else {
		    wincol = col - (curport->rtext + 1 - lkey);
		    col = curport->rtext - lkey + 1;
		}
	    }
	    else
		col -= wincol;
	    if (newputup)
		movewin (winlin, wincol, lin, col, YES);
	    else {
		clrbul ();
		poscursor (col, lin);
	    }
	    if (delay)
		setbul (YES);
	}
	break;

    case NOTFOUND:
    case ABORTED:
	if (puflg) {
	    clrbul ();
	    mesg (ERRALL + 2, "Search ", srchret == NOTFOUND?
					  "key not found.":
					   "aborted");
	    csrsw = NO;
	    if (srchret == ABORTED) {
		d_put (0);
		sleep (1);
	    }
	}
	break;
    }
    return srchret;
}

/* strsearch - searches curfile for str starting at line ln, column stcol,
/*      and giving up when line limit is reached.
/*      delta is 1 or -1, and is the search direction.
/*      Assumes curwksp is set to look at curfile.
/**/
strsearch (str, ln, stcol, limit, delta, srch)
char   *str;
Nlines  ln;
Ncols   stcol;
Nlines  limit;
Small   delta;
Flag srch;      /* if NO, then only look at current position */
{
    register char  *at;
    register char  *sk;
    register char  *fk;
    char   *atcol;
    Nlines  continln;
    Ncols   lkey;
    Flag    nextline;
    Flag    mustmatch;
    Flag    sabort;

    sabort = NO;
    intrupnum = 0;
    lkey = skeylen (str, YES, YES, YES);

    if (ln >= nlines[curfile]) {
	if (delta > 0)
	    goto srchabrt;
	ln = nlines[curfile];
	at = cline;
	getline (ln);
    }
    else {
	getline (ln);
	at = &cline[min (ncline - lkey, stcol)];
    }
    if (starthere)
	at -= delta;

    nextline = NO;
    mustmatch = NO;
    sk = str;
    for (;;) {
	/* get a line that is long enough that it could contain str */
#ifdef lint
	continln = 0;
#endif
	for (at += delta;
	       nextline || at < cline || at >= cline + ncline - lkey;
	    ) {
	    if (!srch)
		goto srchabrt;
	    if (nextline)
		continln++;
	    else
		continln = ln += delta;
	    if (   (delta < 0 && continln < limit)
		|| (delta > 0 && continln > limit && !nextline)
		|| (   intrupnum++ > SRCHPOL
		    && (sabort = sintrup ())
		   )
	       )
		goto srchabrt;
	    getline (continln);
	    at = cline;
	    if (!nextline) {
		if (delta < 0)
		    at += ncline - 1 - lkey;
		mustmatch = NO;
		sk = str;
	    }
	    else
		break;
	}
	fk = at;
	do {
	    if (    sk[0] == ESCCHAR
		&& (sk[1] | 0140) == 'j'
	       ) {
		if (sk == str && sk[2]) {
		    if (fk == cline) {
			sk += 2;
			mustmatch = NO;
			continue;
		    }
		    else
			break;
		}
		else if (*fk == '\n') {
		    sk += 2;
		    if (*sk == 0)
			break;
		    else {
			mustmatch = YES;
			if ( !nextline ) {
			    atcol = at;
			    nextline = YES;
			}
			    goto contin_search;
		    }
		}
	    }
	    else if (*sk == *fk) {
		sk++;
		fk++;
		mustmatch = NO;
		continue;
	    }
	    if (mustmatch) {
		if (delta > 0)
		    at = cline + ncline - lkey;
		else
		    at = cline;
	    }
	    break;
	} while (*sk);
	if (nextline) {
	    nextline = NO;
	    at = atcol;
	}

	if (*sk == 0) { /* found it */
	    srchline = ln;
	    srchcol = at - cline;
	    return FOUND;
	}
	mustmatch = NO;
	sk = str;
    contin_search :
	if (!srch)
	    break;
    }
srchabrt: 
    return sabort? ABORTED: NOTFOUND;
}

/* get length of non-newline characters in str
/**/
skeylen (str, nlok, imbed, stopatnl)
char *str;
Flag nlok;      /* ok for newlines in string */
Flag imbed;     /* imbedded newlines are OK */
Flag stopatnl;  /* stop counting at first newline after other text */
{
    register char *cp;
    register Ncols lkey;

    /* get length of searchkey */
    /* if there are imbedded newlines, length is first part */

    /* skip over initial newlines */
    for (cp = str; ; cp += 2)
	if (    cp[0] == ESCCHAR
	    && (   cp[1] == 'j'
		|| tolower (cp[1]) == 'j'
	       )
	   ) {
	    if (!nlok)
		return -1;
	}
	else
	    break;

    /* stop at imbedded newline */
    for (lkey = Z; *cp; cp++, lkey++)
	if (   cp[0] == ESCCHAR
	    && (   cp[1] == 'j'
		|| tolower (cp[1]) == 'j'
	       )
	   ) {
	    if (!nlok)
		return -1;
	    if (!imbed) {
		if (cp[2] != '\0')
		    return -1;
		else
		    return lkey;
	    }
	    if (stopatnl)
		break;
	    cp++;
	    lkey--;
	}
    return lkey;
}
