/*
/* file e.u.c - use () and editfile (..)
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"
#include "e.cm.h"
#include <sys/stat.h>

use ()
{
    if (opstr[0] == '\0') {
	switchfile ();
	return CROK;
    }
    if (*nxtop)
	return CRTOOMANYARGS;

    savecwksp ();
    editfile (opstr, -1, -1, 1, YES);
    return CROK;
}

/* editfile (file, col, line, mkopt, puflg) -
/*   Installs file as edited file in current port, with window upper-left
/*     at col, line.  Use lastlook if -1.
/*   If file is not there
/*       if mkopt == 2, create the file.
/*       else if mkopt == 1, give the user the option of making it
/*       else return
/*   Returns
/*     -1 if user does not want to make one,
/*      0 if error condition,
/*      1 if successfull.
/*   Writes screen (calls putup) if puflg is 1. */
/**/
editfile (file, col, line, mkopt, puflg)
char   *file;
short   col;
Nlines  line;
Small   mkopt;
Flag    puflg;
{
    int retval;
    static Flag toomany = NO;
    Fn      i;
    Short   j;
    Flag    dwriteable,         /* directory is writeable */
	    fwriteable,         /* file is writeable */
	    isdirectory;        /* is a directory - don's set CANMODIFY */
    Flag    new = NO;
    Ff_stream *ffp;
    Fd      k;
    S_wksp *lwksp;
    char *dir = NULL;
    register Fn fn;

    if ((fn = hvname (file)) != -1) {
	/* we have it active */
	if (fn == curfile)
	    puflg = NO;
    }
    else if (   (fn = hvoldname (file)) != -1
	     || (fn = hvdelname (file)) != -1
	    ) {
	/* has been renamed or deleted, so its directory must be writebale */
	dwriteable = YES;
	goto asknew;
    }
    else {
	/* don't have it as an old name*/
	if (toomany || nopens >= MAXOPENS) {
	    mesg (ERRALL + 1, "Too many files -- Editor limit!");
	    Goret (0);
	}
	/* find the directory */
	if ((j = dircheck (file, &dir, (char **) 0, NO, YES)) == -1)
	    Goret (0);
	dwriteable = (j & S_IWRITE)? YES: NO;

	if ((j = chkpriv (file)) != -1) {  /* file already exists */
	    fwriteable = (j & S_IWRITE)? YES: NO;
	    if ( (j & S_IFMT) != S_IFREG &&  (j & S_IFMT) != S_IFDIR ) {
		mesg (ERRALL + 1, "Can only edit files");
		Goret (0);
	    }
	    if ( !(j & S_IREAD)) {
		mesg (ERRALL + 1, "File read protected.");
		Goret (0);
	    }
	    isdirectory = (j & S_IFMT) == S_IFDIR;
	    if ((fn = getnxfn ()) >= MAXFILES - 1)
		toomany = YES;      /* we'll get you next time */
	    errno = -2;
	    if (!(ffp = ff_open (file, 0, 0))) {
		mesg (ERRALL + 1, "BUG: ff_open failed!");
		Goret (0);
	    }
	    k = ff_fd (ffp);
	    /* is this another name for one we already have open? */
	    for (i = FIRSTFILE; i < MAXFILES; i++)
		if (   (fileflags[i] & (INUSE | NEW)) == INUSE
		    && openffs[i]
		    && k == ff_fd (openffs[i])
		   ) {
		    /* yes it is */
		    /* if it is DELETED, then we go and make a NEW one */
		    if (fileflags[i] & DELETED)
			break;
		    fileflags[fn] = 0;
		    fn = i;
		    ff_close (ffp);
		    toomany = NO;
		    mesg (TELALL + 3, "EDIT: ", names[i], " (linked)");
		    d_put (0);
		    errsw = YES;
		    goto editit;
		}
	    nopens++;
	    mesg (TELSTRT + 2, "EDIT: ", file);
	    d_put (0);
	    if (isdirectory) {
		mesg (1, "  (is a directory)");
		errsw = YES;
	    }
	    else if (fmultlinks (k)) {
		mesg (1, "  (has more than one link)");
		errsw = YES;
	    }
	    mesg (TELDONE);
	    d_put (0);
	    openffs[fn] = ffchans[ff_fd (ffp)] = ffp;
	    openfsds[fn] = filtofsd (fn, 0l, 0);
	    if (dwriteable)
		fileflags[fn] |= DWRITEABLE;
	    if (fwriteable)
		fileflags[fn] |= FWRITEABLE;
	    if (dwriteable && !isdirectory && (fwriteable || !inplace)) {
		fileflags[fn] |= CANMODIFY;
		if (inplace)
		    fileflags[fn] |= INPLACE;
	    }
	    fileflags[fn] |= UPDATE;
	}
	else
 asknew:
	    if (mkopt == 2)
		goto createit;
	else if (mkopt == 1) {
	    mesg (TELSTRT|TELCLR + 3, "Do you want to create ", file, "? ");
	    keyused = YES;
	    getkey (0);
	    keyused = YES;
	    mesg (TELDONE);
	    if (key != 'y' && key != 'Y')
		Goret (-1);
 createit:
	    if (!dwriteable) {
		/* tell 'em */
		dirncheck (dir, YES, YES);
		Goret (0);
	    }
	    /* ok to create file, so do it */
	    if ((fn = getnxfn ()) >= MAXFILES - 1)
		toomany = YES;  /* we'll get you next time */
	    fileflags[fn] |= UPDATE | DWRITEABLE | FWRITEABLE
			     | CANMODIFY | NEW;
	    /* a NEW file can NOT be INPLACE */
	    openffs[fn] = 0;    /* there is no ff stream for this file */
	    openfsds[fn] = blanklines(0);
	}
	else
	    Goret (-1);
	names[fn] = append (file, "");
	new = YES;
    }

editit:
    if (curfile == deffn)
	/* insure that the new altwksp will be null */
	curport->wksp->wfile = NULLFILE;
    switchwksp ();
    lwksp = &lastlook[fn];
    if (new) {  /* we don't have it already */
	lwksp->curfsd  = openfsds[fn]; /* ptr to current line's fsd */
	lwksp->curlno  = 0;            /* current line no. */
	lwksp->curflno = 0;            /* line no of 1st line in curfsd */
    }
    if (line != -1)
	lwksp->wlin = line;       /* line no of ulhc of screen */
    if (col != -1)
	lwksp->wcol = col;        /* col no of column 0 of screen */
    curwksp->curfsd  = lwksp->curfsd;
    curwksp->curlno  = lwksp->curlno;
    curwksp->curflno = lwksp->curflno;
    curwksp->wlin = lwksp->wlin;
    curwksp->wcol = lwksp->wcol;
    curwksp->ccol = lwksp->ccol;
    curwksp->clin = lwksp->clin;

    curwksp->wfile = curfile = fn;
    if (puflg) {
	putupwin ();
	poscursor (curwksp->ccol, curwksp->clin);
    }
    retval = 1;
ret:
    if (dir != NULL)
	sfree (dir);
    return retval;
}

getnxfn ()
{
    register Fn fn;

    for (fn = FIRSTFILE + NTMPFILES; fileflags[fn] & INUSE; )
	if (++fn >= MAXFILES)
	    fatal (FATALBUG, "Too many files");

    fileflags[fn] = INUSE;
    return fn;
}
