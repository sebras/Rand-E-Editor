/*
/* e.wk.c - workspace stuff
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"

/* switchfile()  go to alternate wksp if possible */

switchfile ()
{
    if (curport->altwksp->wfile == NULLFILE) {
	mesg (ERRALL + 1, "No alternate file");
	return;
    }
    swfile ();
}

swfile ()
{
    savecwksp ();
    switchwksp ();
    putupwin ();
    limitcursor ();
    poscursor (curwksp->ccol, curwksp->clin);
}

switchwksp ()
{
    register S_wksp *cwksp;

    cwksp = curwksp;

    cwksp->ccol = cursorcol;
    cwksp->clin = cursorline;

    curport->wksp    = curport->altwksp;
    curport->altwksp = cwksp;

    curwksp = curport->wksp;
    curfile = curwksp->wfile;
}

savecwksp ()
{
    register S_wksp *lwksp,
		    *cwksp;

    cwksp = curwksp;
    lwksp = &lastlook[curfile];

    /* save where we are in current worksp */
    lwksp->curfsd  = cwksp->curfsd;  /* ptr to current line's fsd */
    lwksp->curlno  = cwksp->curlno;  /* current line no. */
    lwksp->curflno = cwksp->curflno; /* line no of 1st line in curfsd */
    lwksp->wcol = cwksp->wcol;
    lwksp->wlin = cwksp->wlin;
    lwksp->ccol = cwksp->ccol = cursorcol;
    lwksp->clin = cwksp->clin = cursorline;
}

