/*
/* file e.wi.c: window routines
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.tt.h"

/* defined in e.t.c, set outside putup (), looked at by putup () */
extern Flag entfstline;     /* says write out entire first line */

/* Routine to handle screen functions for switching to
/*      a new viewport.  changes cursorline, cursorcol to
/*      be relative to new upper lefthand corner.
/**/
switchport (pv)
register S_window *pv;
{
    register S_window *cv;

    cv = curport;
    cursorcol  += cv->ltext - pv->ltext;
    cursorline += cv->ttext - pv->ttext;
    if (curwksp = (curport = pv)->wksp) {
	curfile = curwksp->wfile;
    }
    if (!curport || !curwksp)
	fatal (FATALBUG, "switchport %o %o", curport, curwksp);
    defplline = defmiline = pv->btext / 4 + 1;
}

/* setupviewport - initialize the viewport .
/*      flg = 1 if editing window -- i.e. borders, etc.
/**/
setupviewport (w, cl, lt, cr, lb, flg)
register S_window *w;
Scols   cl,
	cr;
Slines  lt,
	lb;
Flag    flg;
{
    register Slines i;
    register Slines size;

    w->lmarg = cl;
    w->tmarg = lt;
    w->rmarg = cr;
    w->bmarg = lb;
    if (flg) {
	w->ltext = cl + 1;
	w->ttext = lt + 1;
	w->rtext = cr - cl - 2;
	w->btext = lb - lt - 2;
    }
    else {
	w->ltext = cl;
	w->ttext = lt;
	w->rtext = cr - cl;
	w->btext = lb - lt;
    }
    w->ledit = 0;
    w->tedit = 0;
    w->redit = w->rtext;
    w->bedit = w->btext;
	/* evenually this extra space may not be needed */

    w->wksp = (S_wksp *) salloc (sizeof (S_wksp), YES);
    w->altwksp = (S_wksp *) salloc (sizeof (S_wksp), YES);
    size = term.tt_height - NINFOLINES - NENTERLINES - NHORIZBORDERS;
    w->firstcol = (AScols *) salloc (size * (sizeof *w->firstcol), YES);
    /* can't use bfill here because firstcol may not be a char */
    for (i = Z; i < size; i++)
	(w->firstcol)[i] = w->rtext + 1;
    w->lastcol = (AScols *) salloc (size * (sizeof *w->lastcol), YES);
    for (i = Z; i < size; i++)
	(w->lastcol)[i] = 0;
    w->lmchars = salloc (size * (sizeof *w->lmchars), YES);
    w->rmchars = salloc (size * (sizeof *w->rmchars), YES);
}

/* makeport (file) - make a new viewport
/**/
makeport (file)
char   *file;
{
    register S_window *oldport;
    register S_window *newport;
    register Slines i;
    Fn      ocurfile, oaltfile;
    Flag    horiz;                /* 1 if margin horiz, 0 if vert */
    Small   portnum;

    if (nportlist >= MAXPORTLIST) {
	mesg (ERRALL + 1, "Can't make any more windows.");
	return;
    }
    if (   cursorcol == 0
	&& cursorline > 0
	&& cursorline < curport->btext
       )
	horiz = YES;
    else if (   cursorline == 0
	     && cursorcol > 0
	     && cursorcol < curport->rtext - 1
	    )
	horiz = NO;
    else {
	mesg (ERRALL + 1, "Can't put a window there.");
	return;
    }

    savecwksp ();

    ocurfile = curwksp->wfile;
    oaltfile = curport->altwksp->wfile;

    oldport = curport;
    newport = portlist[nportlist++] = (S_window *) salloc (SVIEWPORT, YES);

    /* the number of curport is new prevport */
    for (portnum = 0; portlist[portnum] != curport; portnum++)
	{}
    newport->prevport = portnum;

    if (horiz) {
	/* newport is below oldport on the screen  */
	setupviewport (newport, oldport->lmarg,
				oldport->tmarg + cursorline + 1,
				oldport->rmarg,
				oldport->bmarg, 1);
	oldport->bmarg = oldport->tmarg + cursorline + 1;
	oldport->btext = oldport->bedit = cursorline - 1;
	if ((i = (newport->btext + 1) * sizeof oldport->firstcol[0]) > 0){
	    move (&oldport->firstcol[cursorline + 1],
		   newport->firstcol, (unsigned int) i);
	    move (&oldport->lastcol[cursorline + 1],
		   newport->lastcol, (unsigned int) i);
	}
    }
    else {
	/* newport is to the right of oldport on the screen  */
	setupviewport (newport, oldport->lmarg + cursorcol + 1,
				oldport->tmarg,
				oldport->rmarg,
				oldport->bmarg, 1);
	oldport->rmarg = oldport->lmarg + cursorcol + 1;
	oldport->rtext = oldport->redit = cursorcol - 1;
	for (i = newport->btext; i >= 0; i--) {
	    if (oldport->lastcol[i] > oldport->rtext + 1) {
		newport->firstcol[i] = 0;
		newport->lastcol[i] = oldport->lastcol[i] -
		    cursorcol - 1;
		oldport->lastcol[i] = oldport->rtext + 1;
		oldport->rmchars[i] = MRMCH;
	    }
	}
    }
    drawborders (oldport, 0);   /* inactive, draw sides */
    drawborders (newport, 3);   /* active, don't draw sides */
    switchport (newport);

    if (file) {
	if (strcmp (file, names[ocurfile]) != 0)
	    editfile (names[ocurfile], -1, -1, 0, NO);
	if (editfile (file, -1, -1, 1, YES) <= 0)
	    eddeffile (1);
    }
    else {
	if (oaltfile) {
	    editfile (names[oaltfile], -1, -1, 0, oaltfile == ocurfile);
	    cursorline = curwksp->clin;
	    cursorcol  = curwksp->ccol;
	}
	editfile (names[ocurfile], -1, -1, 0, YES);
    }

    poscursor (0, 0);
}

/* removeport() --
/*      eliminates the last made port by expanding its ancestor */
/**/
removeport ()
{
    Slines  j;
    Scols   stcol;                          /* start col for putup         */
    Slines  stlin;                          /* start lin for putup         */
    Small   ppnum;                          /* prev port number            */
    register Slines i;
    register S_window *theport;    /* port to be removed          */
    register S_window *pport;      /* previous port               */

    if (nportlist == 1) {
	mesg (ERRALL + 1, "Can't remove remaining port.");
	return;
    }
    savecwksp ();
    theport = portlist[--nportlist];
    ppnum = theport->prevport;
    pport = portlist[ppnum];

    if (pport->bmarg != theport->bmarg) {
	/* theport is below pport on the screen  */
	pport->firstcol[j = pport->btext + 1] = 0;
	pport->lastcol[j++] = pport->rtext + 1;
	if ((i = (theport->btext + 1) * sizeof *theport->firstcol) > 0) {
	    move (&theport->firstcol[0],
		  &pport->firstcol[j], (unsigned int) i);
	    move (&theport->lastcol[0],
		  &pport->lastcol[j], (unsigned int) i);
	}
	stcol = 0;
	stlin = pport->btext + 1;
	pport->bmarg = theport->bmarg;
	pport->btext = pport->bmarg - pport->tmarg - 2;
	pport->bedit = pport->btext;
    }
    else {
	/* theport is to the right of pport on the screen  */
	for (i = Z; i <= pport->btext; i++) {
	    pport->lastcol[i] = theport->lastcol[i] +
		theport->lmarg - pport->lmarg;
	    if (pport->firstcol[i] > pport->rtext)
		pport->firstcol[i] = pport->rtext;
	}
	stcol = pport->rtext + 1;
	stlin = 0;
	pport->rmarg = theport->rmarg;
	pport->rtext = pport->rmarg - pport->lmarg - 2;
	pport->redit = pport->rtext;
    }
    chgport (ppnum);
    putup (stlin, curport->btext, stcol, MAXWIDTH);
    poscursor (pport->wksp->ccol, pport->wksp->clin);
    sfree ((char *) theport->firstcol);
    sfree ((char *) theport->lastcol);
    sfree (theport->lmchars);
    sfree (theport->rmchars);
    sfree ((char *) theport->wksp);
    sfree ((char *) theport->altwksp);
    sfree ((char *) theport);
}

/*  chgport(portnum) - changes the current port */
/*      if portnum < 0 means go to next higher port
/**/
chgport (portnum)
Small portnum;
{
    register S_window *newport;
    register S_window *oldport;

    oldport = curport;
    if (portnum < 0) {
	portnum = 0;
	while (portnum < nportlist && oldport != portlist[portnum++])
	    {}
    }
    oldport->wksp->ccol = cursorcol;
    oldport->wksp->clin = cursorline;
    newport = portlist[portnum % nportlist];    /* wrap back to port 0 */
    if (newport == oldport)     /* ALWAYS rewrite first line */
	entfstline = YES;       /* don't skip over blanks */
    drawborders (oldport, 0);
    drawborders (newport, 2);
    switchport (newport);
    limitcursor ();
    poscursor (curwksp->ccol, curwksp->clin);
}

/* drawborders (port, how)
/*     how: 0 - inactive, draw sides
/*          1 - inactive, don't draw sides
/*          2 - active, draw sides
/*          3 - active, don't draw sides
/**/
drawborders (port, how)
S_window *port;
Small how;
{
    S_window *oport;
    register Char bchr;
    register Short i;
    register Short j;

    oport = curport;
    switchport (&wholescreen);

    j = port->rmarg;
    poscursor (i = port->lmarg, port->tmarg);
    if (how <= 1)
	for (; i <= j; i++)
	    putch (INMCH, NO);
    else {
	putch (TLCMCH, NO);
	for (i++; i < j; i++)
	    putch (TMCH, NO);
	putch (TRCMCH, NO);
    }

    poscursor (i = port->lmarg, port->bmarg);
    if (how <= 1)
	for (; i <= j; i++)
	    putch (INMCH, NO);
    else {
	putch (BLCMCH, NO);
	for (i++; i < j; i++)
	    putch (BMCH, NO);
	putch (BRCMCH, NO);
    }

    drawside (port, port->lmarg, how);
    drawside (port, port->rmarg, how);

    switchport (oport);
}

drawside (port, border, how)
S_window *port;
register Scols border;
Small how;
{
    register Slines j;
    register char *bchrp;
    Slines bottom;

    bottom = port->bmarg - 1;
    j = port->tmarg + 1;
    switch (how) {
    case 0:
	for (; j <= bottom; j++) {
	    poscursor (border, j);
	    putch (INMCH, NO);
	}
	break;

    case 2:
	bchrp = border == port->lmarg ? port->lmchars: port->rmchars;
	for (; j <= bottom; j++) {
	    poscursor (border, j);
	    putch (*bchrp++, NO);
	}
	break;

    default:
	/* do nothing */
	break;
    }
}
