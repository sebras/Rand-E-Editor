/*
/* file e.e.c: open, clse, pick, put, erase, etc.
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.fsd.h"
#include "e.m.h"
#include "e.cm.h"

extern Short ldivr;

struct fsd *ldelete (), *pick (), *writemp (), *blanklines ();

Void openmark (), openlines (), openrect ();
Void pickmark (), picklines (), pickrect ();
Void closemark (), closelines (), closerect ();
Void erasemark (), eraselines (), eraserect ();

int (*xarea[][3]) () = {
    openmark,  openlines,  openrect  ,
    pickmark,  picklines,  pickrect  ,
    closemark, closelines, closerect ,
    erasemark, eraselines, eraserect
};

areacmd (which)
Small which;
{
    char *cp;

    if (opstr[0] == '\0') {
	if (curmark)
	    (*xarea[which][0]) ();
	else
	    (*xarea[which][1]) (curwksp->wlin + cursorline, 1, YES);
	return CROK;
    }
    if (curmark)
	return NOMARKERR;
    if (*nxtop)
	return CRTOOMANYARGS;
    cp = opstr;
    switch (getpartype (&cp, YES, NO, curwksp->wlin + cursorline)) {
    case 1:
	for (; *cp && *cp == ' '; cp++)
	    {}
	if (*cp == 0) {
	    (*xarea[which][1]) (curwksp->wlin + cursorline,
				      parmlines, YES, YES);
	    return CROK;
	}
	break;

    case 2:
	(*xarea[which][2]) (curwksp->wlin + cursorline,
				  curwksp->wcol + cursorcol,
				   parmcols, parmlines, YES);
	return CROK;
	break;

    case 3:
	if (which != 0) {
	    /* check for "a" "al" or "all" */



	    break;
	}
    default:
	mesg (ERRSTRT + 1, opstr);
	return CRUNRECARG;
    }

    return CROK;
}

/* openlines, openrect and splitline */

Void
openmark ()
{
    register Flag moved;

    moved = gtumark (YES);
    if (markcols)
	openrect (topmark (), leftmark (), markcols, marklines, !moved);
    else
	openlines (topmark (), marklines, !moved);
    if (moved)
	putupwin ();
    unmark ();
    return;
}

Void
openlines (from, number, puflg)
Nlines  from,
        number;
Flag puflg;
{
    if (from >= nlines[curfile])
	return;

    nlines[curfile] += number;
    insert (curwksp, blanklines (number), from);
    fixfsds   (curfile, from);
    redisplay (curfile, from, 0, number, puflg);
    poscursor (cursorcol, from - curwksp->wlin);
    return;
}

Void
openrect (line, col, nc, nl, puflg)
Nlines  line,
        nl;
Ncols   col,
	nc;
Flag puflg;
{
    register Nlines i;
    register Nlines j;

    for (i = line; i < line + nl; i++) {
	getline (i);
	putbks (col, nc);
	if (   puflg
	    && (j = i - curwksp->wlin) >= 0
	    && j <= curport->btext
	   )
	    putup (-1, j, col - curwksp->wcol, MAXWIDTH);
	putline (0);
    }
    poscursor (col - curwksp->wcol, line - curwksp->wlin);
    return;
}

splitmark ()
{
    register Flag moved;

    moved = gtumark (NO);
    splitlines (topmark (),
		 curwksp->wcol + cursorcol,
		  marklines - 1,
		   curmark->mrkwincol + curmark->mrkcol,
		    !moved);
    if (moved)
	putupwin ();
    unmark ();
    return;
}

splitlines (line, col, nl, newc, puflg)
Nlines  line;
Ncols   col;
Nlines  nl;
Ncols   newc;
Flag    puflg;
{
    register Ncols  nsave;
    register Char   csave;

    if (line >= nlines[curfile])
	return;

    getline (line);
    if (col >= ncline - 1)
	openlines (line + 1, nl, puflg);
    else {
	csave = cline[col];
	cline[col] = '\n';
	nsave = ncline;
	ncline = col + 1;
	fcline = YES;
	nlines[curfile]++;
	putline (0);
	cline[col] = csave;
	insert (curwksp, writemp (&cline[col], nsave - col), line + 1);
	fixfsds (curfile, line);
	getline (line + 1);
	putbks (0, newc);
	putline (0);
	if (nl > 1)
	    openlines (line + 1, nl - 1, NO);
	redisplay (curfile, line, nl, nl, puflg);
    }
    poscursor (col - curwksp->wcol, line - curwksp->wlin);
    return;
}

/* closelines, closerect and joinlines */

Void
closemark ()
{
    register Flag moved;

    moved = gtumark (YES);
    if (markcols)
	closerect (topmark (), leftmark (), markcols, marklines, !moved);
    else
	closelines (topmark (), marklines, YES, !moved);
    if (moved)
	putupwin ();
    unmark ();
    return;
}

Void
closelines (from, number, redsplflg, puflg)
register Nlines from;
Nlines  number;
Flag redsplflg,
     puflg;
{
    register Nlines nn;
    register S_fsd *f;

    nn = nlines[PKCLFILE];
    if (from < nlines[curfile]) {
	nlines[curfile] -= number = min (number, nlines[curfile] - from);
	f = ldelete (curwksp, from, from + number - 1);
	fixfsds (curfile, from);
	if (redsplflg)
	    redisplay (curfile, from, number, -number, puflg);
	if (curfile != PKCLFILE)
	    pkcl (pickwksp, f, nn, number);
    }
    else
	number = 0;
    if (curfile != PKCLFILE) {
	closebuf->linenum = nn;
	closebuf->nlins = number;
	closebuf->ncolumns = 0;
    }
    if (puflg)
	poscursor (cursorcol, from - curwksp->wlin);
    return;
}

Void
closerect (line, col, nc, nl, puflg)
Nlines  line,
        nl;
Ncols   col,
	nc;
Flag puflg;
{
    pcerect (line, col, nc, nl, 1, puflg);
    return;
}

joinmark ()
{
    return NOMARKERR;
}

joinlines (line, col, nl, newc, puflg)
Nlines  line;
Ncols   col;
Nlines  nl;
Ncols   newc;
Flag    puflg;
{
    register char  *temp;
    register Ncols  nsave;

    if (line >= nlines[curfile])
	return;

    getline (line + 1);
    temp = salloc (nsave = ncline, YES);
    if (nsave > 0)
	move (cline, temp, (unsigned int) nsave);
    getline (line);
    if (col + nsave > lcline)
	excline ((col + nsave) - lcline);
    if (col - (ncline - 1) > 0)
	fill (&cline[ncline - 1], (unsigned int) (col - (ncline - 1)), ' ');
    if (nsave > 0)
	move (temp, &cline[col], (unsigned int) nsave);
    ncline = col + nsave;
    fcline = 1;
    putline (0);
    nlines[curfile]--;
    sfree (temp);
    ldelete (curwksp, line + 1, line + 1);
    fixfsds   (curfile, line);
    redisplay (curfile, line, 2, -1, YES);
    poscursor (col - curwksp->wcol, line - curwksp->wlin);
    return;
}

/* eraselines, eraserect */

Void
erasemark ()
{
    register Flag moved;

    moved = gtumark (YES);
    if (markcols)
	eraserect (topmark (), leftmark (), markcols, marklines, !moved);
    else
	eraselines (topmark (), marklines, !moved);
    if (moved)
	putupwin ();
    unmark ();
    return;
}

Void
eraselines (from, number, puflg)
register Nlines from;
Nlines   number;
Flag     puflg;
{
    register Nlines nn;
    register S_fsd *f;

    nn = nlines[PKCLFILE];
    if (from < nlines[curfile]) {
	number = min (number, nlines[curfile] - from);

	f = ldelete (curwksp, from, from + number - 1);
	fixfsds   (curfile, from);

	pkcl (pickwksp, f, nn, number);

	insert (curwksp, blanklines (number), from);
	fixfsds   (curfile, from);

	redisplay (curfile, from, 0, number, puflg);
	poscursor (cursorcol, from - curwksp->wlin);
    }
    else
	number = 0;

    erasebuf->linenum = nn;
    erasebuf->nlins = number;
    erasebuf->ncolumns = 0;
    return;
}

Void
eraserect (line, col, number, nl, puflg)
Nlines  line,
        nl;
Ncols   col,
	number;
Flag puflg;
{
    pcerect (line, col, number, nl, 2, puflg);
    return;
}

/* picklines, pickrect, put */

Void
pickmark ()
{
    if (gtumark (YES))
	putupwin ();
    if (markcols)
	pickrect (topmark (), leftmark (), markcols, marklines);
    else
	picklines (topmark (), marklines);
    unmark ();
    return;
}

Void
picklines (from, number)
register Nlines from;
Nlines  number;
{
    register Nlines n;
    register S_fsd *f;

    n = nlines[PKCLFILE];
    if (from < nlines[curfile]) {
	number = min (number, nlines[curfile] - from);
	if (curfile != PKCLFILE) {
	    f = pick (curwksp, from, from + number - 1);
	    fixfsds (curfile, from);      /* because of breakfsd */
	    pkcl (pickwksp, f, n, number);
	}
    }
    else
	number = 0;

    pickbuf->linenum = curfile == PKCLFILE ? from : n;
    pickbuf->nlins = number;
    pickbuf->ncolumns = 0;

    poscursor (cursorcol, from - curwksp->wlin);
    return;
}

Void
pickrect (line, col, number, nl)
Nlines  line,
        nl;
Ncols   col,
	number;
{
    pcerect (line, col, number, nl, 0, NO);
    return;
}

/* pcerect - pick/close/erase rectangle -- common routine
/*              flg == 0 for pick, 1 for close, 2 for erase
/**/
pcerect (line, col, nc, nl, flg, puflg)
Nlines  line,
        nl;
Ncols   col,
	nc;
Flag flg,
     puflg;
{
    S_svbuf *thebuf;
    long tempend;
    Nlines  j;
    Ncols   endcol;
    Ff_stream *tempff;
    register S_fsd *f1;
    register Nlines i;
    register char  *linebuf;

    if (nc <= 0 || nl <= 0)
	return;
    tempend = ff_size (tempff = openffs[CHGFILE]);
    linebuf = salloc (nc + 1, YES);
    for (j = line; j < line + nl; j++) {
	/* write the data onto the end of the change file */
	getline (j);
	if ((i = (ncline - col)) <= 0)
	    i = 0;
	else {
	    if ((i = min (i, nc)) > 0)
		move (&cline[col], linebuf, (unsigned int) i);
	}
	ff_seek (tempff, ff_size (tempff));
	try_ff_write (tempff, linebuf, dechars (linebuf, i));
    }
    sfree (linebuf);

    f1 = filtofsd (CHGFILE, tempend, 0);
    pkcl (pickwksp, f1, i = nlines[PKCLFILE], nl);
    (thebuf = flg ? (flg == 1 ? closebuf: erasebuf): pickbuf)->linenum = i;
    thebuf->ncolumns = nc;
    thebuf->nlins = nl;

    if (flg) {  /* close or erase */
	endcol = col + nc;
	for (i = line; i < line + nl; i++) {
	    getline (i);
	    if (col >= ncline)      /* begin after end of line?           */
		continue;           /*  => nothing to "close"             */
	    if (endcol < ncline) {  /* end before end of line?            */
				    /* yes => copy down rest of line      */
		if (flg == 1) {
		    if (ncline - endcol > 0)
			move (&cline[endcol], &cline[col],
			      (unsigned int) (ncline - endcol));
		    ncline -= nc;
		}
		else {
		    if (nc > 0)
			fill (&cline[col], (unsigned int) nc, ' ');
		}
	    }
	    else                  /* no => simply terminate line        */
		ncline = col + 1;
	    cline[ncline - 1] = '\n';
	    fcline = 1;
	    if (   puflg
		&& (j = i - curwksp->wlin) >= 0
		&& j <= curport->btext
	       )
		putup (-1, j, col - curwksp->wcol, flg == 1 ? MAXWIDTH: nc);
	    putline (0);
	}
    }
    poscursor (col - curwksp->wcol, line - curwksp->wlin);
    return;
}

/* put(buf,line,col) */

put (buf, line, col)
S_svbuf *buf;
Nlines  line;
Ncols   col;
{
    if (buf->ncolumns == 0)
	ptlines (buf, line);
    else
	putrect (buf, line, col);
    return;
}

ptlines (buf, line)
S_svbuf *buf;
Nlines  line;
{
    Nlines  lbuf;   /* no. of lines in the buf to be put */
    S_fsd  *w0,
	   *w1;
    register S_fsd *f,
		   *g;
    savecurs ();

    breakfsd (pickwksp, buf->linenum + buf->nlins);
    w1 = pickwksp->curfsd;
    breakfsd (pickwksp, buf->linenum);
    w0 = pickwksp->curfsd;

    for (f = g = copyfsd (w0, w1), lbuf = 0; g->fsdfile;
	    lbuf += g->fsdnlines, g = g->fwdptr);

    insert (curwksp, f, line);
    fixfsds   (curfile, line);
    redisplay (curfile, line, 0, lbuf, YES);
    restcurs ();
    if (nlines[curfile] < line)
	nlines[curfile] = line;
    nlines[curfile] += lbuf;
    return;
}

putrect (buf, line, col)
S_svbuf *buf;
Nlines  line;
Ncols   col;
{
    S_wksp *oldwksp;
    char   *linebuf;
    Ncols   nc;
    Nlines  i,
            j;

    linebuf = salloc (nc = buf->ncolumns, YES);
    oldwksp = curwksp;
    for (i = 0;  i < buf->nlins; i++) {
	curwksp = pickwksp;
	getline (buf->linenum + i);
	if (min (nc, ncline) > 0)
	    move (cline, linebuf, (unsigned int) min (nc, ncline));
	if (   (j = ncline - 1) < nc
	    && nc - j > 0
	   )
	    fill (&linebuf[j], (unsigned int) (nc - j), ' ');
	curwksp = oldwksp;

	getline (line + i);
	putbks (col, nc);       /* this also makes cline long enough */
	if (nc > 0)
	    move (linebuf, &cline[col], (unsigned int) nc);
	if (   (j = line + i - curwksp->wlin) >= 0
	    && j <= curport->btext
	   )
		putup (-1, j, col - curwksp->wcol, MAXWIDTH);
	putline (0);
    }
    sfree (linebuf);
    poscursor (col - curwksp->wcol, line - curwksp->wlin);
    return;
}

/* putbks(col,n) - inserts n blanks starting at col of cline */

putbks (col, n)
register Ncols col;
register Ncols n;
{
    fcline = YES;
    if (n <= 0)
	return;
    if (col >= ncline) {    /* remember that there is a '\n' at cline[ncline-1] */
	n += col - (ncline - 1);
	col = ncline - 1;
    }
    if (lcline <= (ncline += n))
	excline (n);

    if (ncline - col - n > 0)
	move (&cline[col], &cline[col + n],
	      (unsigned int) (ncline - col - n));
    if (n > 0)
	fill (&cline[col], (unsigned int) n, ' ');
    return;
}

pkcl (wk, f, nn, number)
S_wksp *wk;
S_fsd *f;
Nlines nn,
       number;
{
    insert (wk, f, nn);
    fixfsds   (PKCLFILE, nn);
    redisplay (PKCLFILE, nn, 0, number, YES);
    nlines[PKCLFILE] += number;
    return;
}
