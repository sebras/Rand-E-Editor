/*
/* file e.m.c - mainloop ()
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"
#include "e.tt.h"

#define VMOTCODE 4		  /* codes 1 through 4 imply the cursor has
				     left the line                  */

mainloop ()
{
    Small cm;
    Nlines lns;
    register Nlines j;
    register Ncols  k;
    register Nlines l;
    char    ich[8],
	   *cp;
    Small   donetype;

/***** main loop for editor - read and echo 1 char ***/

    info (INFOAT, 2, "At");
    info (INFOIN, 2, "in");
    infoline = -1;
    infofile = NULLFILE;
    if (imodesw) {
	imodesw = NO;
	tglinsmode ();
    }

/**/
    for (;;) {
funcdone:
	clrsw = YES;
newnumber:
	{
	Slines curline;
	Scols curcol;

	curline = cursorline;
	curcol = cursorcol;

	if (clrsw && !errsw) {
	    mesg (TELALL + 1, beepsw? "\007": "");
	    beepsw = NO;
	}
	l = curwksp->wlin + curline + 1;
	if (infoline != l) {
	    sprintf (ich, "%-5d", l);
	    info (INFOLINE, 5, ich);
	    infoline = l;
	}

	if (curfile != infofile && (fileflags[curfile] & INUSE)) {
	    info (INFOFILE, strlen (names[infofile]), names[curfile]);
	    infofile = curfile;
	}

	if (csrsw)
	    setbul (YES);

/**/
contin:
	keyused = YES;
	if (curmark) {
	    curline = cursorline;
	    curcol = cursorcol;
	    l = curwksp->wlin + curline;
	    if ((j = l - (curmark->mrkwinlin + curmark->mrklin)) < 0)
		j = -j;
	    j++;
	    if ((k = curwksp->wcol + curcol
		- (curmark->mrkwincol + curmark->mrkcol)) < 0)
		k = -k;
	    if (marklines != j) {
		sprintf (mklinstr, "%d", j);
		marklines = j;
	    }
	    if (markcols != k) {
		if (k)
		    sprintf (mkcolstr, "x%d", k);
		else
		    mkcolstr[0] = '\0';
		markcols = k;
	    }
	    cp = append (mklinstr, mkcolstr);
	    j = strlen (cp);
	    info (INFOAREA, max (j, infoarealen), cp);
	    infoarealen = j;
	    sfree (cp);
	}
	}

	dobullets ();

/* old top of forever loop */
	csrsw = NO;
	clrsw = NO;
	if (cmdmode) {
	    errsw = NO;
	    goto gotcmd;
	}
	getkey (0);
	if (errsw) {
	    errsw = NO;
	    mesg (TELALL);
	}

	if (   !CTRLCHAR
	    || key == CCCTRLQUOTE
	    || key == CCBACKSPACE
	    || key == CCDELCH
	   ) {
	    donetype = printchar ();
	    goto doneswitch;
	}
	if (key < MAXMOTIONS && (cm = cntlmotions[key])) {
	    /* in the next several lines, a putline is always done, EXCEPT
	    /* +TAB, -TAB, and <- and->arrows within the screen
	    /*
	    /* This is not the right way to do this.
	    /* The reason it's messed up is basically because putline
	    /* depends heavily on all sorts of "current" things like
	    /* curfsd, etc.  Really, putline should happen automatically
	    /* whenever there is a getline of a different line than the
	    /* current line.  But that is impossible with the present
	    /* mess.        3/13/80 D. Yost
	    /**/
	    if (   cm <= VMOTCODE
		|| (cm == RT && cursorcol + 1 > curport->redit)
		|| (cm == LT && cursorcol - 1 < curport->ledit)
	       )
		putline (0);    /* must come before movecursor or movew */
	    if (key == CCRETURN && cursorline == curport->btext)
		/* Return on bottom line => +lines    */
		movew (defplline);
	    movecursor (cm, 1);
	    if (cm <= VMOTCODE)
		goto newnumber;
	    goto contin;
	}
	putline (0);

	/* margin-stick feature */
	if (cursorcol > curport->rtext)
	    poscursor (curport->rtext, cursorline);

	switch (key) {
	    case CCCMD: 	  /* <CTRL @> */
		flushkeys ();
		keyused = YES;
		goto gotcmd;

	    case CCLPORT:
		movep (-deflport);/* <CTRL A> */
		goto funcdone;

	    case CCSETFILE:
		if (curmark)
		    goto nomarkerr;
		switchfile ();    /* <CTRL B> */
		goto funcdone;

	    case CCCHPORT:
		if (curmark)
		    goto nomarkerr;
		if (nportlist > 1) {
		    chgport (-1);     /* <CTRL Z> */
		    bulsw = YES;
		}
		goto funcdone;

	    case CCOPEN: 	  /* <CTRL D> */
		if ( !okwrite () )
		    goto nowriterr;
		if (curmark)
		    openmark ();
		else
		    openlines (curwksp->wlin + cursorline,
			definsert, YES);
		flushkeys ();
		goto funcdone;

	    case CCMISRCH:              /* <CTRL E> */
	    case CCPLSRCH:              /* <CTRL R> */
		flushkeys ();
		dosearch (key == CCPLSRCH? 1: -1);
		goto funcdone;

	    case CCCLOSE: 	  /* <CTRL F> */
		if ( !okwrite ())
		    goto nowriterr;
		if (curmark)
		    closemark ();
		else
		    closelines (curwksp->wlin + cursorline,
			defdelete, YES, YES);
		flushkeys ();
		goto funcdone;

       /*   case CCPUT:           /* <CTRL G> */
       /*       if (curmark)
       /*           goto nomarkerr;
       /*       if ( !okwrite () )
       /*           goto nowriterr;
       /*       if (pickbuf->nlins == 0)
       /*           goto nopickerr;
       /*       put (pickbuf, curwksp->wlin + cursorline,
       /*               curwksp->wcol + cursorcol);
       /*       flushkeys ();
       /*       goto funcdone;
       /**/
	    case CCPICK: 	  /* <CTRL L> */
		if (curmark)
		    pickmark ();
		else
		    picklines (curwksp->wlin + cursorline, defpick);
		goto funcdone;

	    case CCINSMODE:       /* <CTRL O> */
		tglinsmode ();
		goto funcdone;

	    case CCMIPAGE:        /* <CTRL Q> */
		movew (-defmipage * (1 + curport->btext));
		goto funcdone;

	    case CCRPORT:
		movep (defrport); /* <CTRL S> */
		goto funcdone;

	    case CCPLLINE:
		movew (defplline);/* <CTRL T> */
		goto funcdone;

	    case CCMILINE:        /* <CTRL W> */
		movew (-defmiline);
		goto funcdone;

	    case CCPLPAGE:        /* <CTRL Y> */
		movew (defplpage * (1 + curport->btext));
		goto funcdone;

	    case CCINT:        /* <CTRL C> */
		goto nointerr;

	    case CCTABS: 	  /* <CTRL [> */
		sctab (curwksp->wcol + cursorcol, YES);
		goto funcdone;

	    case CCMARK:          /* <CTRL G> -- old PUT key */
		mark ();
		goto funcdone;

	    case CCREPLACE:         /* <CTRL P> -- old GOTO key   */
		dodoit ();
		goto funcdone;

	    case CCUNAS1:         /* <CTRL X> */
	    case CCUNAS2:         /* <CTRL V> */
	    case CCERASE:
	    case CCLWORD:
	    case CCRWORD:
	    case CCDEL:
		goto notimperr;

	    case CCDELCH:
	    case CCMOVELEFT:      /* <CTRL H> */
	    case CCTAB: 	  /* <CTRL I> */
	    case CCMOVEDOWN:      /* <CTRL J> */
	    case CCHOME: 	  /* <CTRL K> */
	    case CCRETURN:        /* <CTRL M> */
	    case CCMOVEUP:        /* <CTRL N> */
	    default:
		goto badkeyerr;
	}
/**/
gotcmd:
	param ();

	if (cmdmode && key != CCRETURN)
	    goto notcmderr;
	switch (key) {
	    case CCCMD:         /* <CTRL @> */
		goto funcdone;

	    case CCLPORT:       /* <CTRL A> */
	    case CCRPORT: 	  /* <CTRL S> */
		switch (paramtype) {
		case 0:
		    movep (key == CCRPORT? cursorcol: - curwksp->wcol);
		    break;

		case 1:
		    movep (key == CCRPORT? parmlines: -parmlines);
		    break;

		case 2:
		    goto norecterr;

		default:
		    goto notinterr;
		}
		goto funcdone;

	    case CCSETFILE:     /* <CTRL B> */
		goto notimperr;

	    case CCINT:         /* <CTRL C> */
		goto funcdone;

	    case CCOPEN:        /* <CTRL D> */
		if (curmark)
		    goto nomarkerr;
		if ( !okwrite () )
		    goto nowriterr;
		flushkeys ();
		switch (paramtype) {
		case 0:
		    goto notimperr;
		    break;

		case 1:
		    if (parmlines <= 0)
			goto notposerr;
		    openlines (curwksp->wlin + cursorline,
			parmlines, YES);
		    break;

		case 2:
		    if (parmlines <= 0 || parmcols <= 0)
			goto notposerr;
		    openrect (curwksp->wlin + cursorline,
			       curwksp->wcol + cursorcol,
				parmcols, parmlines, YES);
		    break;

		default:
		    goto notinterr;
		}
		goto funcdone;

	    case CCMISRCH:      /* <CTRL E> */
	    case CCPLSRCH:        /* <CTRL R> */
		if (paramtype == 0)
		    getarg ();
		if (*paramv == 0)
		    goto noargerr;
		if (searchkey)
		    sfree (searchkey);
		searchkey = append (paramv, "");
		dosearch (key == CCPLSRCH? 1: -1);
		goto funcdone;

	    case CCBACKSPACE:
		k = curwksp->wcol;
		if (imodesw)
		    closerect
			(curwksp->wlin + cursorline, k, cursorcol, 1, YES);
		else {
		    savecurs ();
		    eraserect
			(curwksp->wlin + cursorline, k, cursorcol, 1, YES);
		    restcurs ();
		}
		goto funcdone;

	    case CCDELCH:         /* <CTRL U> */
		if (clineno != (lns = curwksp->wlin + cursorline))
		    getline (lns);
		if (ncline > (k = curwksp->wcol + cursorcol))
		    eraserect (curwksp->wlin + cursorline, k, ncline - k,
				1, YES);
		goto funcdone;

	    case CCCLOSE:       /* <CTRL F> */
		if (curmark)
		    goto nomarkerr;
		flushkeys ();
		switch (paramtype) {
		case 0:
		    if (!okwrite ())
			goto nowriterr;
		    if (closebuf->nlins == 0)
			goto nobuferr;
		    put (closebuf, curwksp->wlin + cursorline,
			    curwksp->wcol + cursorcol);
		    break;

		case 1:
		    if (!okwrite ())
			goto nowriterr;
		    if (parmlines <= 0)
			goto notposerr;
		    closelines (curwksp->wlin + cursorline,
			parmlines, YES, YES);
		    break;

		case 2:
		    if (!okwrite ())
			goto nowriterr;
		    if (parmlines <= 0 || parmcols <= 0)
			goto notposerr;
		    closerect (curwksp->wlin + cursorline,
			       curwksp->wcol + cursorcol,
				parmcols, parmlines, YES);
		    break;

		default:
		    goto notinterr;
		}
		goto funcdone;

	    case CCMARK:        /* <CTRL G> -- old PUT key */
		if (paramtype != 0)
		    goto notimperr;
		unmark ();
		goto funcdone;

	    case CCMOVELEFT:      /* <CTRL H> */
	    case CCMOVEDOWN:      /* <CTRL J> */
	    case CCMOVEUP:        /* <CTRL N> */
	    case CCMOVERIGHT:     /* <CTRL _> */

	    case CCTAB: 	  /* <CTRL I> */
	    case CCHOME:          /* <CTRL K> */
	    case CCBACKTAB:       /* <CTRL ]> */
		switch (paramtype) {
		case 0:
		    switch (key) {
		    case CCHOME:
			movecursor (cntlmotions[CCHOME], 1);
			key = CCMOVEDOWN;
			goto llcor;

		    case CCMOVEDOWN:
			if ((lns = nlines[curfile] -
				   (curwksp->wlin + cursorline)) > 0)
			    lns = min (lns, curport->btext - cursorline);
			else
		llcor:      lns = curport->btext - cursorline;
			break;

		    case CCMOVEUP:
			lns = cursorline;
			break;

		    case CCBACKTAB:
		    case CCTAB:
			goto notimperr;

		    case CCMOVELEFT:
			lns = cursorcol;
			break;

		    case CCMOVERIGHT:
			if (clineno != (lns = curwksp->wlin + cursorline))
			    getline (lns);
			if ((lns = ncline - 1 - (curwksp->wcol + cursorcol))
			      > 0)
			    lns = min (lns, curport->rtext - cursorcol);
			else
			    lns = curport->rtext - cursorcol;
			break;
		    }
		    if (lns <= 0)
			goto funcdone;
		    goto multmove;

		case 1:
		    if (parmlines <= 0)
			goto notposerr;
		    lns = parmlines;
 multmove:          cm = cntlmotions[key];
		    movecursor (cm, lns);
		    break;

		case 2:
		    goto norecterr;

		default:
		    goto notinterr;
		}
		goto funcdone;

	    case CCPICK: 	  /* <CTRL L> */
		if (curmark)
		    goto nomarkerr;
		switch (paramtype) {
		case 0:
		    if ( !okwrite () )
			goto nowriterr;
		    if (pickbuf->nlins == 0)
			goto nobuferr;
		    put (pickbuf, curwksp->wlin + cursorline,
			    curwksp->wcol + cursorcol);
		    flushkeys ();
		    break;

		case 1:
		    if (parmlines <= 0)
			goto notposerr;
		    picklines (curwksp->wlin + cursorline, parmlines);
		    break;

		case 2:
		    if (parmlines <= 0 || parmcols <= 0)
			goto notposerr;
		    pickrect (curwksp->wlin + cursorline,
			       curwksp->wcol + cursorcol,
				parmcols, parmlines);
		    break;

		default:
		    goto notinterr;
		}
		goto funcdone;

	    case CCRETURN:      /* <CTRL M> */
		donetype = command ();
		goto doneswitch;

	    case CCMIPAGE:        /* <CTRL Q> */
	    case CCPLPAGE:        /* <CTRL Y> */
		switch (paramtype) {
		case 0:
		    gtfcn (key == CCPLPAGE ? nlines[curfile] : 0);
		    break;

		case 1:
		    movew ((key == CCPLPAGE? parmlines: -parmlines )
				* (1 + curport->btext));
		    break;

		case 2:
		    goto norecterr;

		default:
		    goto notinterr;
		}
		goto funcdone;

	    case CCMILINE:        /* <CTRL W> */
	    case CCPLLINE:        /* <CTRL T> */
		switch (paramtype) {
		case 0:
		    movew (cursorline - (key == CCPLLINE? 0:
					curport->btext));
		    break;

		case 1:
		    movew (key == CCPLLINE? parmlines: -parmlines);
		    break;

		case 2:
		    goto norecterr;

		default:
		    goto notinterr;
		}
		goto funcdone;

	    case CCCHPORT:        /* <CTRL Z> */
		if (curmark)
		    goto nomarkerr;
		switch (paramtype) {
		case 0:
		    goto notimperr; /* should be backwards move */

		case 1:
		    if (parmlines <= 0)
			goto notposerr;
		    chgport (parmlines - 1);
		    bulsw = YES;
		    break;

		case 2:
		    goto norecterr;

		default:
		    goto notinterr;
		}
		goto funcdone;

	    case CCTABS: 	  /* <CTRL [> */
		if (paramtype == 0)
		    sctab (curwksp->wcol + cursorcol, NO);
		else
		    goto notimperr;
		goto funcdone;

	    case CCINSMODE:       /* <CTRL O> */
		goto badkeyerr;

	    case CCUNAS1:         /* <CTRL X> */
	    case CCUNAS2:         /* <CTRL V> */
	    case CCREPLACE:         /* <CTRL P> -- old GOTO key   */
	    case CCERASE:
	    case CCLWORD:
	    case CCRWORD:
	    case CCDEL:
	    default:
		goto notimperr;
	}

doneswitch:
	switch (donetype) {
	case NOTSTRERR:
	    mesg (ERRALL + 1, "Argument must be a string.");
	    break;

	case NOWRITERR:
nowriterr:
	    mesg (ERRALL + 1, "You cannot modify this file!");
	    break;

	case NOARGERR:
noargerr:
	    mesg (ERRALL + 1, "Invalid argument.");
	    break;

	case NOPIPERR:
	    mesg (ERRALL + 1, "Can't fork or write pipe.");
	    break;

	case NOMARKERR:
nomarkerr:
	    mesg (ERRALL + 1, "Can't do that with marks set");
	    break;

	case NORECTERR:
norecterr:
	    mesg (ERRALL + 1, "Can't do that to a rectangle");
	    break;

	case NOBUFERR:
nobuferr:
	    mesg (ERRALL + 1, "Nothing in that buffer.");
	    break;

	case CONTIN:
	    goto contin;

	case MARGERR:
	    mesg (ERRALL + 1, "Cursor stuck on right margin.");
	    break;
	}
	continue;

notcmderr:
	mesg (ERRALL + 1, "Can't do that in Command Mode");
	continue;

nointerr:
	mesg (ERRALL + 1, "No operation to interrupt");
	continue;

badkeyerr:
	mesg (ERRALL + 1, "Bad key error - editor error");
	continue;

notinterr:
	mesg (ERRALL + 1, "Argument must be numeric.");
	continue;

notposerr:
	mesg (ERRALL + 1, "Argument must be positive.");
	continue;

notimperr:
	mesg (ERRALL + 1, "That key sequence is not implemented.");
	continue;
    }
}

dobullets ()
{
    static Scols  oldccol;
    static Slines oldcline;
    Scols   occ;
    Slines  ocl;

    if (!borderbullets)
	return;
    occ = oldccol;
    oldccol = cursorcol;
    ocl = oldcline;
    oldcline = cursorline;
    if (clrsw || (oldccol != occ)) {
	/* do top */
	if (!bulsw && occ < oldccol) {
	    poscursor (occ, -1);
	    putch (TMCH, 0);
	}
	poscursor (oldccol, -1);
	putch (BULCHAR, 0);
	if (!bulsw && occ > oldccol) {
	    poscursor (occ, -1);
	    putch (TMCH, 0);
	}
	/* do bottom */
	if (!bulsw && occ < oldccol) {
	    poscursor (occ, curport->btext + 1);
	    putch (BMCH, 0);
	}
	poscursor (oldccol, curport->btext + 1);
	putch (BULCHAR, 0);
	if (!bulsw && occ > oldccol) {
	    poscursor (occ, curport->btext + 1);
	    putch (BMCH, 0);
	}
    }

    if (clrsw || (oldcline != ocl)) {
	/* do right side */
	if (!bulsw) {
	    poscursor (curport->rtext + 1, ocl);
	    putch (curport->rmchars[ocl], 0);
	}
	poscursor (curport->rtext + 1, oldcline);
	putch (BULCHAR, 0);
	/* do left side */
	if (!bulsw) {
	    poscursor (-1, ocl);
	    putch (curport->lmchars[ocl], 0);
	}
	poscursor (-1, oldcline);
	putch (BULCHAR, 0);
    }
    bulsw = NO;
    poscursor (oldccol, oldcline);
    d_put (0);
    return;
}
