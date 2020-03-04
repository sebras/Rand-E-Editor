/*
/* file e.t.c: terminal interface stuff
/*
/*  The terminal-dependent output stuff is in e.d.c
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.tt.h"

#ifdef UNIXV7
# include <signal.h>
#endif

#ifdef UNIXV6
# include <sys/signals.h>
#endif

#ifndef EMPTY
    /* this is especially bad news, because you can't abort replays */
    empty (fd) { return 1; }
#endif


#define EINTR   4       /* should be #included but for problems ... */

static Flag upbullet;

    /* set outside putup (), looked at by putup () */
Flag    entfstline;     /* says write out entire first line */

Flag needputup; /* because the last one was aborted */
Flag atoldpos;

putupwin ()
{
    savecurs ();
    putup (0, curport->btext, 0, MAXWIDTH);
    restcurs ();
}

/* putup (ls, lf, stcol, ncols) - puts up lines from ls to lf.
/*      ls and lf are window line numbers; first line is 0.
/*      Special feature - if ls is negative,
/*          only puts up line lf and takes it from cline instead of disk.
/*      Start at column stcol (rel. to ltext) on each line.
/*      If ncols != MAXWIDTH then it is assumed that the line has not gotten
/*      shorter or longer since last putup, and you know what you are doing,
/*      and you only want to putup from stcol to stcol + ncols.
/*      Caller does his own poscursor after calling putup.
/*
/*      N.B. There was some attempt in the old days to have putup deal with
/*      ports that had no left & right borders.  No more.  It is now assumed
/*      that borders are always there.  If the caller doesn't want the
/*      display of the borders to change, he clears the global "chgborders"
/*      before calling putup (..);  If he wants dot borders for this putup,
/*      he sets chgborders to 2.
/**/
putup (ls, lf, stcol, ncols)
Nlines  ls;
Nlines  lf;
Scols   stcol;      /* starting col for partial line redraw */
Scols   ncols;      /* number of columns for partial line redraw */
{
    register Ncols  j;
    register Ncols  k;
    register Nlines ln; /* the line worked on within the for loop */
    Ncols   fc;
    Slines  lt;         /* temp line num for writing left border upwards */
    Slines  lb;         /* limit for going backwards to fix the left border */
    Ncols   lcol,       /* = curwksp->wcol */
	    rlimit,     /* = curport->rtext + 1 */
	    ricol,      /* rightmost col of text to put up            */
	    lstcol,     /* rightmost col of text on line              */
	    col;        /* variable col, reset to stcol for each line */
    int     lmc,        /* left  margin char that should be on screen */
	    rmc;        /* right margin char that should be on screen */
    Flag    usecline,   /* ls was < 0, so use cline */
	    offendflg;  /* off the end of the file */

    /* if the caller knows that after the putup he will move the cursor
    /* to someplace other than where it is, then he will set newcurline
    /* and/or newcurcol to the new position
    /* newcurline is always set to -1 on return from putup.
    /**/
    if (newcurline == -1)
	newcurline = cursorline;
    if (newcurcol == -1)
	newcurcol = cursorcol;
    atoldpos = NO;
    if (dintrup ()) {
	needputup = YES;
	goto ret;
    }
    else
	needputup = NO;

    lcol = curwksp->wcol;
    rlimit = curport->rtext + 1;
    if (usecline = ls < 0)
	ls = lf;
    lmc = lcol == 0 ? LMCH : MLMCH;

    for (ln = ls;  ln <= lf; ln++) {
	rmc = RMCH;
	offendflg = NO;

	if (!usecline) {        /* get the line into cline */
	    if (   ln != lf
		&& !entfstline
		&& ( replaying || (intrupnum++ > DPLYPOL) )
		&& dintrup ()
	       ) {
		needputup = YES;
		break;
	    }
	    if (getline (curwksp->wlin + ln) == 1) {
		offendflg = YES;
		lmc = ELMCH;
	    }
	    if (ln >= newcurline)
		atoldpos = YES;
	}

	/* deal with the left border */
	/* if this is the first line of the putup, back up and fix left    */
	/*   border as far up as necessary. */
	if (ln == ls && lmc != ELMCH)
	    lb = 0;
	else
	    lb = ln;
	for (lt = ln; lt >= lb; lt--) {
	    if ((curport->lmchars[lt] & 0377) == lmc)
		break;
	    if (chgborders && !(borderbullets && lt == newcurline)) {
		poscursor (-1, lt);
		putch (chgborders == 1? lmc: INMCH, NO);
	    }
	    curport->lmchars[lt] = lmc;
	}

	/*  determine fc: first non-blank column in cline
	/*  if portion of line to be displayed is blank, then set fc
	/*  to rlimit
	/**/
	if (   offendflg
	    || (fc = fnbcol (cline, lcol, min (ncline, lcol + rlimit))) < lcol
	   )
	    fc = rlimit;
	else
	    fc -= lcol;

	/* how many leading blanks do we need to put out */
	if (entfstline) {
	    entfstline = NO;
	    col = 0;
	}
	else {
	    col = min (fc, (curport->firstcol)[ln]);
	    if (col < stcol)
		col = stcol;
	}
	(curport->firstcol)[ln] = fc;
	poscursor (col, ln);
#ifdef SMOOTHOUT
	if (stdout->_cnt < 100)   /* smooth any outputting breaks  */
	    d_put (0);
#endif
	if ((k = fc - col) > 0) {
	    multchar (' ', k);
	    cursorcol += k;
	    col = fc;
	}

	/* determine the rightmost col of the new text to put up */
	/* this should do better on lines that go past right border */
	if (offendflg)
	    lstcol = 0;
	else {
	    j = (ncline - 1) - lcol;
	    if (j < 0)
		j = Z;
	    else if (j > rlimit) {
		j = rlimit;
		rmc = MRMCH;
	    }
	    lstcol = j;
	}
	if (lstcol < col)
	    lstcol = col;
	ricol = min (lstcol, min (rlimit, stcol + ncols));

	/* put out the text */
	if (ricol - col > 0) {
	    j = lcol + col;
	    d_write (cline + j, ricol - col);
	    cursorcol += (ricol - col);
	}

	/* determine how many trailing blanks we need to put out */
	if ((k = curport->lastcol[ln] - lstcol) > 0) {
	    multchar (' ', k);
	    cursorcol += k;
	}
	curport->lastcol[ln] = (lstcol == fc)? 0: lstcol;

	/* what to do about the right border */
	if ((curport->rmchars[ln] & 0377) != rmc) {
	    if (chgborders && !(borderbullets && ln == newcurline)) {
		poscursor (curport->rmarg - curport->ltext, ln);
		putch (chgborders == 1? rmc: INMCH, NO);
	    }
	    curport->rmchars[ln] = rmc;
	}
    }
ret:
    chgborders = 1;
    newcurline = -1;
}

/*  look for a non-blank character in line between line[left] and line[right]
/*  If right < left, or a newline is encountered, or no non-blank is
/*  encountered then return left - 1
/*  else return index into array line of the non-blank char.
/**/
fnbcol (line, left, right)
char *line;
int left;
int right;
{
    register char *cp;
    register char *cpl;
    register char *cpr;

    if (right < left)
	return left - 1;

    /* this code is a little tricky because it looks for a premature '\n' */
    cpl = &line[left];
    cpr = &line[right];
    for (cp = line; ; cp++) {
	if (   *cp == '\n'
	    || cp >= cpr
	   )
	    return left - 1;
	if (   *cp != ' '
	    && cp >= cpl
	   )
	    return cp - line;
    }
}

multchar (ch, num)
Char ch;
register Short num;
{
    register Small cnt;
    register char *buf;
    char cbuf[3];

    buf = cbuf;
    while (num > 0) {
	cnt = Z;
	if (num > 3) {
	    buf[cnt++] = VCCARG;
	    if (num > 95) {
		buf[cnt++] = 95 + 040;
		num -= 95;
	    }
	    else {
		buf[cnt++] = num + 040;
		num = Z;
	    }
	}
	else {
	    if (num > 2) {
		buf[cnt++] = ch;
		num--;
	    }
	    if (num > 1) {
		buf[cnt++] = ch;
		num--;
	    }
	    num--;
	}
	buf[cnt++] = ch;
	d_write (buf, cnt);
    }
}

bulalarm ()
{
}

setbul (wt)
Flag wt;
{
    if (!term.tt_bullets)
	return;
    putch (BULCHAR, YES);
    movecursor (LT, 1);
    d_put (0);
    if (wt) {
	if (getkey (1) == NOCHAR) {
	    upbullet = YES;
	    signal (SIGALRM, bulalarm);
	    alarm (1);
	    if (getkey (2) != NOCHAR)
		alarm (0);
	    /* either this read will be interrupted by the alarm,
	    /* or a key will have come in, in which case we want to clear
	    /* bullet right away and reset the alarm.
	    /**/
	}
	clrbul ();
    }
}

clrbul ()
{
    Ncols charpos;

    if (!term.tt_bullets)
	return;
    if (curfile != NULLFILE) {
	getline (cursorline + curwksp->wlin);
	putch ((charpos = cursorcol + curwksp->wcol) < ncline - 1
	       ? cline[charpos]
	       : ' ', NO);
    }
    else
	putch (' ', NO);
    movecursor (LT, 1);
}

/* All three of the following functions: gtfcn, movew, and movep must not
/* return until they have called movewin, so that movewin can see if a
/* putup needs to be done after one was aborted.
/**/
gtfcn (number)
Nlines  number;
{
    register Scols  cc;
    register Nlines cl;
    register Small  defpl;
    Ncols wincol;

    number = max (0, number);
    wincol = curwksp->wcol;
    if (curport->btext > 1)
	defpl = defplline;
    else
	defpl = Z;
    if (number == 0) {
	wincol = 0;
	cc = Z;
    }
    else
	cc = cursorcol;
    cl = min (number, defpl);
    movewin (number - defpl, wincol, cl, cc, 1);
}

/* movew(nl) - moves current port n lines up/down.
/**/
movew (nl)
Nlines nl;
{
    register Nlines winlin;
    register Nlines cl;

    winlin = curwksp->wlin;
    if (winlin == 0 && nl <= 0)
	nl = 0;
    if ((winlin += nl) < 0) {
	nl -= winlin;
	winlin = Z;
    }
    cl = cursorline - nl;

    if ( abs (nl) > curport->btext )
	cl = cursorline;
    else if (cl < 0)
	cl = Z;
    else if (cl > curport->btext)
	cl = curport->btext;

    return movewin (winlin, curwksp->wcol, cl, cursorcol, 1);
}

/* movep(nc) - moves port nc columns right. */

movep (nc)
Ncols nc;
{
    register Ncols  cc;

    cc = cursorcol;
    if ((curwksp->wcol + nc) < 0)
	nc = -curwksp->wcol;
    cc -= nc;
    cc = max (cc, curport->ledit);
    cc = min (cc, curport->redit);
    return movewin (curwksp->wlin, curwksp->wcol + nc, cursorline, cc,
			YES);
}

movewin (winlin, wincol, curslin, curscol, puflg)
Nlines  winlin;
Ncols   wincol;
Slines  curslin;
Scols   curscol;
Flag    puflg;
{
    register Small newdisplay;

    winlin = max (0, winlin);
    wincol = max (0, wincol);
    newdisplay = Z;
    if (curwksp->wlin != winlin) {
	curwksp->wlin = winlin;
	newdisplay |= WLINMOVED;
    }
    if (curwksp->wcol != wincol) {
	curwksp->wcol = wincol;
	newdisplay |= WCOLMOVED;
    }
    if (newdisplay || needputup) {
	newcurline = curslin;
	newcurcol  = curscol;
	if (puflg)
	    putupwin ();
    }
    curslin = max (0, curslin);
    curslin = min (curslin, curport->btext);
    curscol = max (0, curscol);
    curscol = min (curscol, curport->rtext);
    if (cursorline != curslin)
	newdisplay |= LINMOVED;
    if (cursorcol != curscol)
	newdisplay |= COLMOVED;
    poscursor (curscol, curslin);
    return newdisplay;
}

struct svcs {
    struct svcs *sv_lastsv;
    AScols  sv_curscol;
    ASlines sv_cursline;
} *sv_curs;

savecurs ()
{
    register struct svcs *lastsv;

    lastsv = sv_curs;
    sv_curs = (struct svcs *) salloc (sizeof *sv_curs, YES);
    sv_curs->sv_lastsv = lastsv;
    sv_curs->sv_curscol = cursorcol;
    sv_curs->sv_cursline = cursorline;
}

restcurs ()
{
    register struct svcs *lastsv;

    poscursor (sv_curs->sv_curscol, sv_curs->sv_cursline);
    lastsv = sv_curs->sv_lastsv;
    sfree ((char *) sv_curs);
    sv_curs = lastsv;
}

/* poscursor (col, lin) - routine to position cursor */
/*      col is relative to curport->ltext
/*      lin is relative to curport->ttext
/*
/**/
poscursor (col, lin)
register Scols  col;
register Slines lin;
{
    if (cursorline == lin) {
	/* only need to change column?  */
	switch (col - cursorcol) {  /* fortran arithmetic if!!      */
	case -1:
	    d_put (VCCLEF);
	    --cursorcol;
	case 0:               /* already in the right place   */
	    return;
	case 1:
	    d_put (VCCRIT);
	    ++cursorcol;
	    return;
	}
    }
    else if (cursorcol == col) {   /* only need to change line?    */
	switch (lin - cursorline) {
	    /* fortran arithmetic if!!  zero taken care of above     */
	case -1:
	    d_put (VCCUP);
	    --cursorline;
	    return;
	case 1:
	    d_put (VCCDWN);
	    ++cursorline;
	    return;
	}
    }
    cursorcol = col;
    cursorline = lin;

    /* the 041 below is for the terminal simulator cursor addressing */
    putscbuf[0] = VCCAAD;
    putscbuf[1] = 041 + curport->ltext + col;
    putscbuf[2] = 041 + curport->ttext + lin;
    d_write (putscbuf, 3);
}

/* move cursor within boundaries of viewport w
/**/
movecursor (arg, cnt)
Small   arg;
Nlines cnt;
{
    register Slines lin;
    register Ncols  col;
    register Short  i;
    Nlines ldif;
    Ncols  cdif;

    lin = cursorline;
    col = cursorcol;
    switch (arg) {
    case 0:                   /* noop                             */
	break;
    case HO:                  /* upper left-hand corner of screen */
	col = Z;
	lin = Z;
	break;
    case UP:                  /* up one line                      */
	lin -= cnt;
	break;
    case RN:
	col = curport->ledit;
			      /* break not needed                 */
    case DN:                  /* down one line                    */
	lin += cnt;
	break;
    case RT:                  /* forward one space                */
	col += cnt;
	break;
    case LT:                  /* back space (non-destructive)     */
	col -= cnt;
	break;
    case TB:                  /* tab forward to next stop         */
	for (i = Z, col += curwksp->wcol; i < ntabs; i++)
	    if (tabs[i] > col) {
		if (tabs[i] <= curwksp->wcol + curport->rtext)
		    col = tabs[min (ntabs - 1, i + cnt - 1)];
		break;
	    }
	col -= curwksp->wcol;
	break;
    case BT:                  /* tab back to previous stop        */
	for (i = ntabs - 1, col += curwksp->wcol; i >= 0; i--)
	    if (tabs[i] < col) {
		if (tabs[i] >= curwksp->wcol)
		    col = tabs[max (0, i - (cnt - 1))];
		break;
	    }
	col -= curwksp->wcol;
	break;
    }
    if ((cdif = col - curport->ledit) < 0)
	col = curport->ledit;
    else if ((cdif = col - curport->redit) > 0)
	col = curport->redit;
    else
	cdif = NO;

    if ((ldif = lin - curport->tedit) < 0)
	lin = curport->tedit;
    else if ((ldif = lin - curport->bedit) > 0)
	lin = curport->bedit;
    else
	ldif = NO;

    if ((ldif || cdif) && arg != 0)
	movewin (curwksp->wlin + ldif,
		 curwksp->wcol + cdif,
		 lin, col, YES);
    else
	poscursor (col, lin);
}

extern unsigned Short read2 ();

/* Read another character from the input stream.  If the last character
/*      wasn't used up (keyused == NO) don't read after all.
/*
/*  peekfl: 0 wait for a character, ignore interrupted read calls.
/*          1 peek for a character
/*          2 wait for a character, then peek at it;
/*              if read is interrupted, return NOCHAR.
/**/
unsigned Short
getkey (peekfl)
Flag peekfl;
{
    register Char rkey;
    static Flag knockdown  = NO;

    if (!peekfl && keyused == NO)
	return key; /* then getkey is really a no-op */
    rkey = read2 (peekfl);
    if (knockdown && ((unsigned) rkey) < 040)
	rkey |= 0100;
    if (peekfl)
	return rkey;
    knockdown = rkey == CCCTRLQUOTE;
    keyused = NO;
    return key = rkey;
}

Flag entering = NO;     /* set if e is in the param () routine. */
			/* looked at by read2 () */
/* read2 (peekfl) - return the next character from the input stream
/*                  if CCINT is encountered, it will NOT be written
/*                  to the keyfile by this routine.  The caller will write
/*                  it there if and only if it actually interrupted something.
/**/
static unsigned Short
read2 (peekfl)
Small peekfl;
{
    register unsigned Short rchar;
    register char *cp;
    int nread;
    static Small replaydone = 0;
    static Short lcnt;
    static int   lexrem;        /* portion of lcnt that is still raw */
    static char chbuf[NREAD];
    static char *lp;

    if (replaying) {
	if (replaydone) {
 finishreplay:
	    close (inputfile);
	    inputfile = INSTREAM;
	    if (silent) {
		silent = NO;
		(*term.tt_ini1) (); /* not d_put(VCCICL) because fresh() */
				    /* follows */
		windowsup = YES;
		fresh ();
	    }
	    mesg (ERRALL + 1,
		  recovering
		  ? "Recovery completed."
		  : replaydone == 1
		    ? "Replay completed."
		    : "Replay aborted."
		 );
	    d_put (0);
	    if (!empty (INSTREAM))
		sleep (2);
	    replaying = NO;
	    recovering = NO;
	    replaydone = 0;
	    goto nonreplay;
	}
	if (   !recovering
	    && !empty (INSTREAM) /* any key stops replay */
	   ) {
	    lcnt = 0;
	    replaydone = 2;
	    goto endreplay;
	}
	if (   lcnt <= 0
	    && nreplaykeys > 1
	   ) {
	    d_put (0);
	    for (;;) {
		/* note that keystroke file is in canonical format and
		/* doesn't have to go through inlex */
		if ((lcnt = read (inputfile, lp = chbuf, NREAD)) >= 0) {
		    if ((nreplaykeys -= lcnt) <= 0)
			lcnt--;     /* don't take the last character */
		    break;
		}
		if (errno != EINTR)
		    fatal (FATALIO, "Error reading input.");
	    }
	}
	if (lcnt == 0) {
	    replaydone = 1;
 endreplay:
	    if (!entering)
		goto finishreplay;
	    else {
		*chbuf = CCINT;
		lp = chbuf;
		lcnt = 1;
	    }
	}
    }
    else
 nonreplay:
	if (lcnt < 0)
	    fatal (FATALBUG, "lcnt < 0");
	if (   (lcnt - lexrem == 0 && peekfl != 1)
	    || (lcnt < NREAD && !empty (INSTREAM))
	   ) {
	    /* read some more */
	    if (lcnt == 0)
		lp = chbuf;
	    else if (lp > chbuf) {
		if (lcnt == 1)
		    /* handle frequent case more efficiently */
		    *chbuf = *lp;
		else
		    move (lp, chbuf, (unsigned int) lcnt);
		lp = chbuf;
	    }
	    d_put (0);
	    do {
		nread = NREAD - lcnt;
		if ((nread = read (inputfile, &chbuf[lcnt], nread)) > 0) {
		    char *stcp;
		    stcp = &chbuf[lcnt -= lexrem];
		    lexrem += nread;
		    if ((nread = (*kbd.kb_inlex) (stcp, &lexrem)) > 0) {
			register int nr;
			cp = &stcp[nread];
			/* look ahead for interrupts */
			nr = nread;
			do {
			    if (*--cp == CCINT)
				break;
			} while (--nr);
			if ((nr = cp - stcp) > 0) {
			    /* found CCINT character in chars just read */
			    /* and some characters in front of it are to be */
			    /* thrown away */
			    lp += lcnt + nr;
			    lcnt = nread - nr + lexrem;
			}
			else
			    lcnt += nread + lexrem;
		    }
		    else
			lcnt += lexrem;
		}
		else if (nread < 0) {
		    if (errno != EINTR)
			fatal (FATALIO, "Error reading input.");
		    if (peekfl == 2)
			break;
		}
		else
		    fatal (FATALIO, "Unexpected EOF in key input.");
	    } while (lcnt - lexrem == 0);
	}

    if (   lcnt - lexrem <= 0
	&& peekfl
       )
	return NOCHAR;

    if (peekfl)
	return *lp & 0377;

    rchar = *lp++ & 0377;
    lcnt--;
    if (keyfile != NULL && rchar != CCINT) {
	putc (rchar, keyfile);
	if (numtyp > MAXTYP) {
	    flushkeys ();
	    numtyp = 0;
	}
    }
    return rchar;
}

writekeys (code1, str, code2)     /* writefile -- put it out on keyfile   */
Char    code1,
        code2;
char   *str;
{
    putc (code1, keyfile);
    fputs (str, keyfile);
    putc (code2, keyfile);
}

dintrup ()			  /* interrupt a display function on      */
{                                 /* any cntl except motion, INS/ARG/SRCH */
    register unsigned Short ichar;

    intrupnum = 0;

    ichar = getkey (1);
    switch (ichar) {
	case CCOPEN:
	case CCCLOSE: 
	    return atoldpos;

	/* NOTE: so far, all functions that appear here call movep(), movew()
	/*       when it is their turn to actually be performed, and
	/*       movewin does a putup if the last one was aborted.
	/**/
	case CCLPORT:
	case CCRPORT:
	case CCMIPAGE:
	case CCPLPAGE:
	case CCMILINE:
	case CCPLLINE:
	    return YES;

	case CCMOVEUP:
	    if (newcurline == curport->tedit)
		return YES;
	    break;

	case CCMOVEDOWN:
	    if (newcurline == curport->bedit)
		return YES;
	    break;

	case CCMOVELEFT:
	    if (newcurcol == curport->ledit)
		return YES;
	    break;

	case CCMOVERIGHT:
	    if (newcurcol == curport->redit)
		return YES;
	    break;

	case NOCHAR:
	case CCDEL:     /* should this be here? */
	case CCDELCH: 
	case CCCMD: 
	case CCINSMODE: 
	case CCMISRCH: 
	case CCPLSRCH: 
	case CCSETFILE:
	case CCCHPORT:
	case CCPICK:
	case CCUNAS1:
	case CCUNAS2:
	case CCREPLACE:
	case CCMARK:
	case CCINT:
	case CCTABS:
	case CCCTRLQUOTE:
	case CCBACKSPACE:
	default:
	    break;
    }
    return NO;
}


sintrup ()			  /* Whether to intrup search/exec      */
{
    intrupnum = 0;		  /* reset the counter                  */
    if (getkey (1) == CCINT) {
	putc (CCINT, keyfile);
	keyused = YES;
	getkey (0);
	keyused = YES;
	return YES;
    }
    return NO;
}

/* put a character up at current cursor position
/* The character had better be a printing char or a bell or an 0177
/**/
putch (j, flg)
#ifdef UNSCHAR
unsigned char j;
#else
unsigned int j;
#endif
Flag    flg;
{
#ifndef UNSCHAR
    j &= 0377;
#endif
    if (j < 040) {
	d_put (j);
	return;
    }
    if (flg && key != ' ') {
	if ((curport->firstcol)[cursorline] > cursorcol)
	    (curport->firstcol)[cursorline] = cursorcol;
	if ((curport->lastcol)[cursorline] <= cursorcol)
	    (curport->lastcol)[cursorline] = cursorcol + 1;
    }

    /*  adjust cursorcol, cursorline for edge effects of screen
	Sets cursorcol, cursorline to correct values if they were positioned
	past right margin.
	If cursor was incremented
	from brhc of screen do not put out a character
	since screen would scroll, but home cursor.
    /**/
    if (curport->ltext + ++cursorcol > term.tt_width) {
	cursorcol = -curport->ltext;  /* left edge of screen */
	if (curport->ttext + ++cursorline >= term.tt_height) {
	    cursorline = -curport->ttext;
	    d_put (VCCHOM);
	}
	else
	    d_put (j);
	poscursor (cursorcol < curport->ledit
		   ? curport->ledit
		   : cursorcol > curport->redit
		     ? curport->redit
		     : cursorcol,
		   cursorline < curport->tedit
		   ? curport->tedit
		   : cursorline > curport->bedit
		     ? curport->bedit
		     : cursorline);
    }
    else
	d_put (j);
}

/* param() - gets a parameter
/*      There are three types of parameters:
/*              paramtype = -1 -- cursor defined
/*              paramtype = 0  -- just <arg> <function>
/*              paramtype = 1  -- string, either integer or text:
/*      returns pointer to the text string entered.
/*      the pointer is left in the global variable paramv, and
/*      its alloced length in bytes (not string length) in ccmdl.
/**/

static S_window *msoport;
static Scols  msocol;
static Slines msolin;

static char *ccmdp;
static Short ccmdl;
static Short ccmdlen;
static char *lcmdp;
static Short lcmdl;
static Short lcmdlen;
#define  LPARAM 20       /* length of free increment */

param ()
{
    register char  *c1;
    register char  *cp;
    char   *c2;
    register Short  i;
    Short ppos;
    Slines lin;
    Scols  col;
    char *wcp;
    Scols  wlen;
    Flag cmdflg;
    Flag uselast;
    char *pkarg ();

    uselast = NO;
    entering = YES;   /* set this flag so that read2 knows you are in this
		    /* routine. */
    savecurs ();
    setbul (NO);

    if (ccmdl == 0)  /* first time only */
	ccmdp = salloc (ccmdl = LPARAM, YES);
    if (lcmdl == 0)
	lcmdp = salloc (lcmdl = LPARAM, YES);

    exchgcmds ();
 parmstrt:
    mesg (TELSTRT|TELCLR + 1, cmdmode? "": "CMD: ");
    if (uselast) {
	uselast = NO;
	if (lcmdlen >= ccmdl)
	    ccmdp = gsalloc (ccmdp, 0, ccmdl = lcmdlen + LPARAM, YES);
	for (c1 = ccmdp, c2 = lcmdp, i = lcmdlen; i--; )
	    putch (*c1++ = *c2++, NO);
	*c1 = '\0';
	ppos = ccmdlen = lcmdlen;
	getkey (0);
    }
    else {
	ccmdp[0] = '\0';
	ppos = Z;
	ccmdlen = Z;
	getkey (0);
	switch (key) {
	case CCDELCH:
	case CCMOVELEFT:
	case CCMOVERIGHT:
	case CCBACKSPACE:
	    goto done;
	}
    }

    cmdflg = NO;
    for (; ; cmdflg = key == CCCMD, keyused = YES, getkey (0)) {
	if (ccmdlen >= ccmdl)
	    ccmdp = gsalloc (ccmdp, ccmdlen, ccmdl += LPARAM, YES);
	if (CTRLCHAR)
	    /* make sure that all codes for which <CMD><key> is undefined
	    /* are ignored */
	    switch ((unsigned) key) {
	    case CCBACKSPACE:
		if (ppos) {
		    if (cmdflg) {
			if (imodesw) {
			    poscursor (cursorcol - ppos, cursorline);
			    if ((i = ccmdlen - ppos) > 0) {
				c2 = &(c1 = ccmdp)[ppos];
				do {
				    putch (*c1++ = *c2++, NO);
				} while (--i);
			    }
			    i = ppos;
			    do {
				putch (' ', NO);
			    } while (--i);
			    poscursor (cursorcol - ccmdlen, cursorline);
			    ccmdlen -= ppos;
			    ppos = 0;
			}
			else {
			    i = ppos;
			    c1 = ccmdp;
			    poscursor (cursorcol - ppos, cursorline);
			    do {
				putch (*c1++ = ' ', NO);
			    } while (--i);
			}
		    }
		    else {
			if ((i = ccmdlen - ppos--) > 0 && imodesw) {
			    movecursor (LT, 1);
			    goto delchr;
			}
			else {
			    if (i == 0)
				ccmdlen--;
			    cursorcol--;
			    d_put (VCCBKS);
			    ccmdp[ppos] = ' ';
			}
		    }
		}
		break;

	    case CCDELCH:
		if ((i = ccmdlen - ppos) > 0) {
		    if (cmdflg) {
			ccmdp[ppos] = '\0';
			savecurs ();
			do {
			    putch (' ', NO);
			} while (--i);
			restcurs ();
			ccmdlen = ppos;
		    }
		    else {
 delchr:                if (i > 0)
			    move (&ccmdp[ppos + 1], &ccmdp[ppos],
				  (unsigned int) i);
			ccmdlen--;
			savecurs ();
			for (i = ppos; i < ccmdlen; )
			    putch (ccmdp[i++], NO);
			putch (' ', NO);
			restcurs ();
		    }
		}
		break;

	    case CCINSMODE:
		/* do this whether or not preceded by <CMD> key */
		tglinsmode ();
		break;

	    case CCMOVELEFT:
		if (ppos) {
		    i = cmdflg? ppos: 1;
		    movecursor (LT, i);
		    do {
			if (ppos-- == ccmdlen && ccmdp[ppos] == ' ')
			    ccmdp[--ccmdlen] = 0;
		    } while (--i);
		}
		break;

	    case CCMOVERIGHT:
		for (;;) {
		    if (ppos < ccmdlen) {
			ppos++;
			movecursor (RT, 1);
			if (!cmdflg)
			    break;
		    }
		    else if (cmdflg)
			break;
		    else {
			ccmdp[ppos++] = ' ';
			ccmdlen++;
			movecursor (RT, 1);
			break;
		    }
		}
		break;

	    case CCPICK:
		if (!cmdflg)
		    goto done;
		if ((wcp = pkarg (msoport->wksp,
				   msoport->wksp->wlin + msolin,
				    msoport->wksp->wcol + msocol,
				     &wlen)) == NULL)
		    break;
		/* make sure alloced space is big enough */
		if (ccmdlen + wlen >= ccmdl)
		    ccmdp = gsalloc (ccmdp, ccmdlen,
				     ccmdl = ccmdlen + wlen + LPARAM, YES);
		/* make room for insertion */
		if ((i = ccmdlen - ppos) > 0)
		    move (&ccmdp[ppos], &ccmdp[ppos + wlen],
			  (unsigned int) i);
		/* insert the word */
		if (wlen > 0)
		    move (wcp, &ccmdp[ppos], (unsigned int) wlen);
		ccmdlen += wlen;
		/* update screen */
		col = cursorcol + wlen;
		lin = cursorline;
		c1 = &ccmdp[ppos];
		for (i = ccmdlen - ppos; i-- > 0; )
		    putch (*c1++, NO);
		poscursor (col, lin);
		ppos += wlen;
		break;

	    case CCSETFILE:
		if (!cmdflg)
		    goto done;

		mesg (TELSTOP);
		uselast = YES;
		keyused = YES;
		goto parmstrt;

	    case CCCMD:
		break;

	    case CCCTRLQUOTE:
		/* do this whether or not preceded by <CMD> key */
		key = ESCCHAR;
		goto prchar;

	    case CCINT:
		/* do this whether or not preceded by <CMD> key */
		if (ccmdlen > 0)
		    /* we have generated something we may want to call back */
		    exchgcmds ();
		putc (CCINT, keyfile);
		keyused = YES;
		if (cmdmode) {
		    mesg (TELSTOP);
		    goto parmstrt;
		}
		ccmdlen = 0;
	    case CCRETURN:
		goto done;

	    default:
		if (!cmdflg)
		    goto done;
	    }
	else
 prchar:    if (imodesw && ppos < ccmdlen) {
	    move (&ccmdp[ppos], &ccmdp[ppos + 1], ccmdlen++ - ppos);
	    ccmdp[ppos] = key;
	    col = cursorcol + 1;
	    lin = cursorline;
	    for (i = ppos++; i < ccmdlen; )
		putch (ccmdp[i++], NO);
	    poscursor (col, lin);
	}
	else {
	    if (ppos == ccmdlen)
		ccmdlen++;
	    ccmdp[ppos++] = key;
	    putch (key, NO);
	}
    }
 done:
    if ((i = (key == CCINT? lcmdlen: ccmdlen) - ppos) > 0)
	movecursor (RT, i);
    mesg (TELSTOP);
    ccmdp[ccmdlen] = '\0';

    if (ccmdlen == 0)
	paramtype = 0;
    else {
	/* in most cases, the interest is whether the arg string
	/*  0: was empty
	/*  1: contained only a number
	/*  2: contained only a rectangle spec (e.g. "12x16")
	/*  3: contained a single-word string
	/*  4: contained a multiple-word string
	/**/
	c2 = ccmdp;
	paramtype = getpartype (&c2, YES, NO, curwksp->wlin + cursorline);

	switch (paramtype) {
	case 1:
	case 2:
	    for (cp = c2; *cp && *cp == ' '; cp++)
		{}
	    if (*cp)
		paramtype = 4; /* more than one string */
	    break;

	case 3:
	    for (cp = ccmdp; *cp && *cp == ' '; cp++)
		{}
	    for (; *cp && *cp != ' '; cp++)
		{}
	    for (; *cp && *cp == ' '; cp++)
		{}
	    if (*cp)
		paramtype = 4;
	}
    }
    paramv = ccmdp;
    if (ccmdlen == 0)
	/* exchange back so that we don't have empty string as last cmd */
	exchgcmds ();
    restcurs ();
    clrbul ();
    entering = NO;
    return;
}

exchgcmds ()
{
    register int i;
    register char *cp;

    cp = lcmdp;
    lcmdp = ccmdp;
    ccmdp = cp;
    i = lcmdl;
    lcmdl = ccmdl;
    ccmdl = i;
    i = lcmdlen;
    lcmdlen = ccmdlen;
    ccmdlen = i;
}

/* getarg() -- get the argument from the file  all characters up to space
/**/
getarg ()
{
    register Short  i;
    register char  *cp;
    int len;
    char *pkarg ();

    if (ccmdl == 0)
	ccmdp = salloc (ccmdl = LPARAM, YES);

    if ((cp = pkarg (curwksp, curwksp->wlin + cursorline,
		      curwksp->wcol + cursorcol, &len)) == NULL) {
	ccmdp[0] = '\0';
	return;
    }

    if (ccmdl < len + 1)
	ccmdp = gsalloc (ccmdp, 0, ccmdl = len + 1, YES);
    /* arg = rest of "word" */
    for (i = Z; i < len; i++)
	ccmdp[i] = *cp++;
    ccmdp[len] = '\0';
    ccmdlen = len;
    paramv = ccmdp;
}

char *
pkarg (wksp, line, col, len)
S_wksp *wksp;
Nlines line;
Ncols col;
int *len;
{
    S_wksp *tmpwksp;
    register int rlen;
    register char *ch;

    tmpwksp = curwksp;
    curwksp = wksp;
    getline (line);
    curwksp = tmpwksp;
    if (col >= ncline - 1 || cline[col] == ' ')
	return NULL;
    rlen = 0;
    for (ch = cline + col; *ch != ' ' && *ch != '\n'; rlen++, *ch++)
	{}
    *len = rlen;
    return cline + col;
}

limitcursor ()
{
    curport->wksp->ccol = min (curport->wksp->ccol, curport->rtext);
    curport->wksp->clin = min (curport->wksp->clin, curport->btext);
}

info (column, ncols, msg)
Scols   column,
	ncols;
register char *msg;
{
    S_window *oldport;
    Scols  oldcol;
    Slines oldlin;

    oldport = curport;        /* save old port info   */
    oldcol = cursorcol;
    oldlin = cursorline;
    switchport (&infoport);
    poscursor (column, 0);

    for (; cursorcol < infoport.redit && *msg; putch (*msg++, NO), ncols--)
	{}
    for (; ncols > 0; ncols--)
	putch (' ', NO);

    switchport (oldport);
    poscursor (oldcol, oldlin);
}

/* VARARGS1 */
mesg (parm, msgs)
register Small parm;
char *msgs;
{
    static Scols lparamdirt,       /* for edit part        */
		 rparamdirt;       /* for info part        */
    Scols   lastcol;
    Small nmsg;
    char **msgp;
    register Scols   tmp;
    register char *cp;

    if (parm & TELSTRT) {
	msoport = curport;        /* save old port info   */
	msocol = cursorcol;
	msolin = cursorline;
	switchport (&enterport);
	if (cmdmode)
	    poscursor (sizeof "CMDS: " - 1, 0);
	else
	    poscursor (0, 0);
    }
    else
	if (curport != &enterport)
	    return;

    lastcol = cursorcol;
    if ((parm & ERRSTRT) == ERRSTRT) {
	cp = "\007 *** ";
	for (; cursorcol < enterport.redit && *cp; )
	    putch (*cp++, NO);
    }
    msgp = &msgs;
    for (nmsg = parm & 7; nmsg-- > 0; ) {
	cp = *msgp++;
	while (cursorcol < enterport.redit && *cp)
	    putch (*cp++, NO);
    }
    if ((parm & ERRSTOP) == ERRSTOP) {
	putch ('\007', NO);
	errsw = YES;
    }
    lastcol = (cursorcol < lastcol) ? enterport.redit : cursorcol;
				  /* Did we wrap around?          */
    tmp = (lastcol <= term.tt_width - 1) ? lparamdirt : rparamdirt;
				  /* how much dirty from before?     */
    if (lastcol > tmp || parm & TELCLR) {

	if (lastcol <= term.tt_width - 1)
	    lparamdirt = lastcol;
	else
	    rparamdirt = lastcol;
    }
    if ((parm & TELCLR) && (lastcol < enterport.redit)) {
				  /* wipe out rest of what was there */
	while (cursorcol < enterport.redit && cursorcol < tmp)
	    putch (' ', NO);
	if (!(parm & TELSTOP))
	    poscursor (lastcol, cursorline);
    }
    if (parm & TELSTOP) {         /* remember last position and.. */
	if (curport != msoport)
	    switchport (msoport); /* go back to original pos      */
	poscursor (msocol, msolin);
    }
}


/* redisplay (fn, from, num, delta, cwkspflg) -
/*      redisplay is called after a change has been made in file fn,
/*      starting at line from, and changing
/*      num lines of the file,
/*      with a total change of delta in the length of the file.
/*      We are supposed to:
/*  1. Redisplay any workspaces which are actually changed by this
/*      tampering, including curwksp if cwkspflg is non-0
/*  2. Adjust the current line number of any workspace which may be pointing
/*      further down in the file than the change, and doesn't want
/*      to suffer any apparent motion.
/*
/*  The stuff involving fsds that used to be in here is in a new routine
/*    called fixfsds (..) in e.fs.c
/**/
redisplay (fn, from, num, delta, cwkspflg)
Fn      fn;
Nlines  from,
	num,
	delta;
Flag    cwkspflg;
{
    register S_wksp *tw;
    register Small  i;
    register Nlines j;
    Nlines  k,
	    wtop;
    S_window *oport;

    clineno = -2;       /* what's this here for? -dy 8/20/79 */
    for (i = Z; i < nportlist; i++) {
	if ((tw = portlist[i]->altwksp)->wfile == fn)
	    readjtop (tw, from, num, delta);
	if ((tw = portlist[i]->wksp)->wfile == fn) {
	    readjtop (tw, from, num, delta);
	    wtop = tw->wlin;
	    /* do we have to redisplay anything? */
	    j = max (from, wtop);
	    k = wtop + portlist[i]->btext;
	    if (delta == 0)
		k = min (k, from + num - 1);
	    if (j <= k && (tw != curwksp || cwkspflg)) {
		oport = curport;
		savecurs ();
		newcurline = cursorline;
		newcurcol  = cursorcol;
		switchport (portlist[i]);
		if (curport != oport)
		    chgborders = NO;
		putup (j - wtop, k - wtop, 0, MAXWIDTH);
		switchport (oport);
		restcurs ();
	    }
	}
    }
}

readjtop (pw, from, num, delta)
register S_wksp *pw;
register Nlines  from;
Nlines  num,
	delta;
{
    /* adjust line num of top of wksp, if necessary  */

    if (delta != 0 && pw->wlin >= from) {
	if (pw->wlin >= from + max (num, max (0, delta)))
	    pw->wlin += delta;
    /*  else {
	    if (delta > 0)
		pw->wlin = from + num;
	    else
		pw->wlin = from;
	}  */
    }
}

/* clearscreen() -- used to do that, but now puts cursor at lower left and
/*  scrolls screen
/**/
clearscreen ()
{
#ifdef CLEARONEXIT
    /*  When a terminal simulator is used that keeps its own duplicate image
    /*  of the screen, clearscreen is only used upon exiting, so it doesn't
    /*  need to reinitialize that image.
    /**/
    d_put (VCCCLR);
    d_put (VCCEND);
#else
    putscbuf[0] = VCCAAD;
    putscbuf[1] = 041 + 0;
    putscbuf[2] = 041 + term.tt_height - 1;
    putscbuf[3] = 0;
    d_write (putscbuf, 4);
    d_put (VCCEND);
    putchar ('\n');
    fflush (stdout);
#endif
}

tglinsmode ()
{
    imodesw = !imodesw;          /* toggle it */
    if (imodesw)
	info (INFOINSERT, 6, "INSERT");
    else
	info (INFOINSERT, 6 , "");
}
