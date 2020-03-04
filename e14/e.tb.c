/*
/* file e.tb.c: tab stuff
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"
#include "e.cm.h"

ANcols *ptabs;          /* new tabstops requested (each is +1 from actual) */
Short   pntabs;         /* num of new tabstops requested */
Short   pstabs;         /* num of new tabstops alloced */

dotab (setclr)
Flag setclr;
{
    if ((pntabs = getptabs ()) == -1)
	return CRBADARG;
    if (pntabs == 0) {
	ptabs[0] = curwksp->wcol + cursorcol + 1;
	pntabs = 1;
    }

    stclptabs (setclr);
    if (ptabs)
	sfree ((char *) ptabs);
    return CROK;
}

dotabs (setclr)
Flag setclr;
{
    int retval;

    if ((pntabs = getptabs ()) == -1)
	return CRBADARG;
    if (pntabs == 0 && setclr)
	Goret (CRNEEDARG);
    if (pntabs > 1)
	Goret (CRTOOMANYARGS);

    if (curmark) {
	if (marklines > 1 || markcols == 0) {
	    mesg (ERRALL + 1,
		  "\"tabs\" only works with a marked rectangle 1x...");
	    Goret (CROK);
	}
	if (gtumark (YES))
	    putupwin ();
	tabevery (pntabs == 0? 1: ptabs[0], curwksp->wcol + cursorcol,
		      curwksp->wcol + cursorcol + markcols, setclr);
	unmark ();
    }
    else {
	if (pntabs == 0)
	    ntabs = 0;
	else
	    tabevery (ptabs[0], 0, (NTABS / 2 - 1) * ptabs[0], setclr);
    }
    retval = CROK;
ret:
    if (ptabs)
	sfree ((char *) ptabs);
    return retval;
}

getptabs ()
{
    int tmpi;
    char *cp;
    char *cp1;

    ptabs = 0;
    pntabs = 0;
    cp = cmdopstr;
    alcptabs ();

    for (;;) {
	for (; *cp && *cp == ' '; cp++)
	    {}
	cp1 = cp;
	cp = s2i (cp, &tmpi);
	if (cp == cp1 || tmpi <= 0)
	    break;
	if (pntabs >= pstabs) {
	    ptabs = (ANcols *)
		    gsalloc ((char *) ptabs,
			     pntabs * sizeof *ptabs,
			     (pstabs = ((3 * pntabs) / 2)) * sizeof *ptabs,
			     YES);
	}
	ptabs[pntabs++] = tmpi;
    }
    if (*cp != '\0') {
	sfree ((char *) ptabs);
	return -1;
    }
    return pntabs;
}

tabfile (setclr)
Flag setclr;
{
    if (*opstr == '\0')
	return CRNEEDARG;
    if (*nxtop != '\0')
	return CRTOOMANYARGS;

    gettabs (opstr, setclr);
    return CROK;
}

/* gettabs - sets or clears tabs as specified in a file.  The spec in the
/*   file is just a list of decimal numbers separated by spaces.
/*   All existing tabs are left set, and new tabstops are set according
/*   to the tabfile.
/**/
#define TNDIG 6
gettabs (filenam, setclr)
char   *filenam;
Flag setclr;
{
    int retval;
    register char  *cp;
    register Short  gc;
    char    ts[TNDIG],
            nambuf[128];
    Short   i;
    FILE  *iob;

    if (   (iob = fopen (filenam, "r")) == NULL
	&& (   *filenam == '/'
	    || (   (copy (filenam, copy ("/etc/",copy (getmypath (),nambuf))),
		    (iob = fopen (nambuf, "r"))) == NULL
		|| (copy (filenam, copy ("/etc/e/", nambuf)),
		    (iob = fopen (nambuf, "r"))) == NULL
	       )
	   )
       ) {
	mesg (ERRALL + 1, "Can't open file");
	return (0);
    }
    alcptabs ();
    for (gc = getc (iob); ; gc = getc (iob)) {  /* once for each tab stop */
	if (gc == EOF)
	    break;
	for (cp = ts; ;gc = getc (iob)) {
	    if (gc >= '0' && gc <= '9') {
		if (cp >= &ts[TNDIG])
		    goto bad;
		*cp++ = gc;
	    }
	    else
		break;
	}
	if (gc == ' ' || gc == '\n' || gc == EOF) {
	    if (cp == ts) {
		if (gc == EOF)
		    break;
		continue;
	    }
	}
	else {
 bad:       mesg (ERRALL + 1, "Bad tabstop file format");
	    Goret (0);
	}
	*cp = 0;
	s2i (ts, &i);
	if (i <= 0)
	    goto bad;
	if (pntabs >= pstabs) {
	    ptabs = (ANcols *)
		    gsalloc ((char *) ptabs,
			     pntabs * sizeof *ptabs,
			     (pstabs = ((3 * pntabs) / 2)) * sizeof *ptabs,
			     YES);
	}
	ptabs[pntabs++] = i;
    }
    stclptabs (setclr);

 ret:
    if (ptabs)
	sfree ((char *) ptabs);
    fclose (iob);
    return retval;
}

alcptabs ()
{
    ptabs = (ANcols *) salloc (NTABS * sizeof *ptabs, YES);
    pntabs = 0;
    pstabs = NTABS;
}

tabevery (interval, stcol, endcol, setclr)
register Ncols interval;
Ncols stcol;
register Ncols endcol;
Flag setclr;
{
    register Ncols col;

    for (col = stcol; col <= endcol; col += interval)
	sctab (col,setclr);
}

stclptabs (setclr)
Flag setclr;
{
    register Ncols i;

    for (i = Z; i < pntabs; )
	sctab (ptabs[i++] - 1, setclr);
}

sctab (col, setclr)
Ncols col;
Flag setclr;
{
    register Ncols i1;
    register Ncols i2;

    if (setclr) {
	if (ntabs + 1 > stabs)
	    tabs = (ANcols *) gsalloc ((char *) tabs,
				       ntabs * sizeof *tabs,
				       (stabs += NTABS / 2) * sizeof *tabs,
				       YES);

	for (i1 = Z; ; i1++) {
	    if (i1 >= ntabs) {
 setit:         ntabs++;
		tabs[i1] = col;
		break;
	    }
	    if ((i2 = col - tabs[i1]) == 0)
		break;
	    if (i2 < 0) {
		if ((i2 = (ntabs - i1) * sizeof *tabs) > 0)
		    move ((char *) &tabs[i1], (char *) &tabs[i1 + 1],
			  (unsigned int) i2);
		goto setit;
	    }
	}
    }
    else
	for (i1 = Z; i1 < ntabs; i1++) {
	    if ((i2 = col - tabs[i1]) < 0)
		break;
	    if (i2 == 0) {
		if ((i2 = (--ntabs - i1) * sizeof *tabs) > 0)
		    move ((char *) &tabs[i1 + 1], (char *) &tabs[i1],
			  (unsigned int) i2);
		break;
	    }
	}
}
