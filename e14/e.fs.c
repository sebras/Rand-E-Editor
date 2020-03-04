/*
/* file e.fs.c: fsd stuff - to be replaced by LA package
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.fsd.h"
#include "e.m.h"
#ifdef UNIXV7
#include <signal.h>
#endif
#ifdef UNIXV6
#include <sys/signals.h>
#endif

extern Short ldivr;

/* dechars(line,n) - performs in-place character conversion from internal
/*  to external format of n characters starting at line.  May
/*  destroy contents of line.  CAUTION - will always insert a
/*  newline at line[n] - so line must be n+1 long.
/*
/*  note: replaces initial spaces with tabs; deletes trailing spaces
/*  returns number of characters in the converted, external representation
/**/
Short
dechars (line, nchars)
char   *line;
Ncols   nchars;
{
    Char    cc;                   /* current character          */
    Ncols   cn;                   /* col number                 */
    register char  *fm,
                   *to;		  /* pointers for move          */
    register Ncols  lnb;          /* 1 + last non-blank col     */

    line[nchars] = '\n';
    fm = to = line;
    cn = -1;
    lnb = Z;
    for (; (cc = *fm++) != '\n';) {
	if (cc == ESCCHAR && *fm != '\n')
	    cc = (*fm == ESCCHAR) ? *fm++ : (*fm++ & 037);
	++cn;
	if (cc != ' ') {
	    if (lnb == 0)
		for (; 8 + (lnb & ~7) <= cn;
			*to++ = (lnb & 7) == 7 ? ' ' : '\t',
			lnb &= ~7, lnb += 8);
	    for (; ++lnb <= cn; *to++ = ' ');
	    *to++ = cc;
	}
    }
    *to++ = '\n';
    return (to - line);
}

/* excline(n) - expand cline to max of length or lcline + icline
/**/
excline (length)
Ncols length;
{
    register Ncols  j;
    register char  *tmp;

    j = lcline + icline;
    icline += icline / 2;
    if (j < length)
	j = length;

    tmp = salloc (j + 1, YES);
    if ((lcline = j) > 0)
	move (cline, tmp, (unsigned int) j);
    sfree (cline);
    cline = tmp;
}

/*   wposit (wk, lno) - get wk->curfsd to point to the fsd
/*      containing current line. */
/**/
wposit (wk, lno)
register S_wksp *wk;
Nlines  lno;
{
    register Nlines wkcurflno;
    register Small  curfsdnlines;

    if (lno < 0)
	fatal (FATALBUG, "WPOSIT NEG ARG");

    for (wkcurflno = wk->curflno;
	  lno >= wkcurflno + (curfsdnlines = wk->curfsd->fsdnlines);
	   wkcurflno += curfsdnlines,
	   wk->curfsd = wk->curfsd->fwdptr
	)
	if (wk->curfsd->fsdfile == 0) {
	    wk->curlno = wk->curflno = wkcurflno;
	    return (1);
	}

    for (; lno < wkcurflno; wkcurflno -= wk->curfsd->fsdnlines)
	if ((wk->curfsd = wk->curfsd->backptr) == 0)
	    fatal (FATALBUG, "WPOSIT 0 BACKPTR");

    if ((wk->curflno = wkcurflno) < 0)
	fatal (FATALBUG, "WPOSIT LINE CT LOST");

    wk->curlno = lno;
    return 0;
}

/* insert(wksp,f,at) inserts fsd f into workspace wksp before line at.
		Calling program MUST call fixfsds with proper arguments. */

insert (wksp, f, at)
S_wksp *wksp;
S_fsd  *f;
Nlines  at;
{
    register S_fsd *w0,
		   *wf,
		   *ff;
				  /* determine length of insert */
    for (ff = f; ff->fwdptr->fsdfile; ff = ff->fwdptr)
	{}
    breakfsd (wksp, at);
    wf = wksp->curfsd;
    w0 = wf->backptr;
    sfree ((char *) (ff->fwdptr));    /* free the terminating fsd */

    ff->fwdptr = wf;            /* attach links at end of f */
    wf->backptr = ff;
    f->backptr = w0;            /* attach links at beginning */
    wksp->curfsd = f;

    wksp->curlno = wksp->curflno = at;
    if (fileflags[wksp->wfile] & CANMODIFY)   /* for # file - dy */
	fileflags[wksp->wfile] |= EDITED;
    if (w0)
	w0->fwdptr = f;
    else
	openfsds[wksp->wfile] = f;
}

/* ldelete(wksp,from,to) - deletes indicated lines from workspace.
	Returns fsd chain that was deleted, with appended 0 block.
	Calling program MUST call fixfsds with proper arguments. */

S_fsd *
ldelete (wksp, from, to)
S_wksp *wksp;
Nlines  from,
        to;
{
    S_fsd      *w0;
    register S_fsd *wf,
		   *f0,
		   *ff;

    breakfsd (wksp, to + 1);
    wf = wksp->curfsd;
    breakfsd (wksp, from);
    f0 = wksp->curfsd;
    ff = wf->backptr;
    w0 = f0->backptr;

    wksp->curfsd = wf;
    wf->backptr = w0;
    f0->backptr = 0;
    (ff->fwdptr = (S_fsd *) salloc (SFSD, YES))->backptr = ff;
				  /* do both in one line */
    if (w0)
	w0->fwdptr = wf;
    else
	openfsds[wksp->wfile] = wf;
    fileflags[wksp->wfile] |= EDITED;

    return (f0);
}

/* pick(wksp,from,to) - returns a copy of fsds for lines
	    from to to in wksp with the appended 0 block.
	    Calling program MUST call fixfsds with proper arguments. */

S_fsd *
pick (wksp, from, to)
S_wksp *wksp;
Nlines  from,
        to;
{
    S_fsd *wf;

    breakfsd (wksp, to + 1);
    wf = wksp->curfsd;
    breakfsd (wksp, from);
    return (copyfsd (wksp->curfsd, wf));
}

/* breakfsd(w,n) - breaks the fsd at line n of workspace w. curlno =
        curflno on return, and curfsd is left pointing to the first line
        after the break (which may be an end-of-file block).  the original
        fsd block will probably be left with unused and unrecoverable space
        at the end.  If called to break at the line past the end of file,
        will leave current position at the end-of-file block.  If called to
        break past that, FSD's with channel -1 (blank lines) will be
        inserted to provide sufficient lines.

CAUTION: breakfsd mucks about with pointers, and may invalidate the fsd
	pointer field of a workspace.  fixfsds must be called before
        attempting to mess with workspaces other than those specifically
        fixed up in routines that call breakfsd. */

Flag
breakfsd (w, n)
S_wksp *w;
Nlines  n;
{
    Short   nby,    /*? what types do these really have to be? */
            i,
            j,
            jj,
            k,
            offs;
    register S_fsd *f,
		   *ff;
    S_fsd          *fn;
    register char  *c;
    char   *cc;

    if (wposit (w, n)) {
	f = w->curfsd;
	ff = f->backptr;
	sfree ((char *) f);
	fn = blanklines (n - w->curlno);
	w->curfsd = fn;
	fn->backptr = ff;
	if (ff)
	    ff->fwdptr = fn;
	else
	    openfsds[w->wfile] = fn;
	wposit (w, n);
	return (1);
    }
    f = w->curfsd;
    c = f->fsdbytes;
    offs = 0;
    ff = f;

    if ((nby = n - w->curflno) != 0) {
	for (i = 0; i < nby; offs += j, i++)
#ifndef NOSIGNEDCHAR
	    if ((j = *c++) < 0) { /* get down to the nth line */
		offs -= 128 * j;
		j = *c++;
	    }
#else
	    if ((j = *c++) & LLINE) { /* get down to the nth line */
		offs += - (j | LLINE) << NLLINE;
		j = *c++;
	    }
#endif
	for (i = j = jj = f->fsdnlines - nby, cc = c; --i >= 0;)
#ifndef NOSIGNEDCHAR
	    if (*cc++ < 0) {      /* make new fsd from the remainder of f */
		j++;
		cc++;
	    }
#else
	    if (*cc++ & LLINE) {  /* make new fsd from the remainder of f */
		j++;
		cc++;
	    }
#endif
	ff = (S_fsd *) salloc (SFSD + j, YES);
	ff->fsdnlines = jj;
	ff->fsdfile = f->fsdfile;
	ff->seekpos = f->seekpos + offs;
	for (cc = ff->fsdbytes, k = 0; k < jj; k++)
#ifndef NOSIGNEDCHAR
	    if ((*cc++ = *c++) < 0)
		*cc++ = *c++;
#else
	    if ((*cc++ = *c++) & LLINE)
		*cc++ = *c++;
#endif
	if ((ff->fwdptr = f->fwdptr))
	    ff->fwdptr->backptr = ff;
	ff->backptr = f;
	f->fwdptr = ff;
	f->fsdnlines = nby;
    }
    w->curfsd = ff;
    w->curflno = n;
    return (0);
}

/* writemp(buf,n) - writes a line n long from buf onto temp file.
	returns an fsd pointer describing the line. */

S_fsd *
writemp (buf, n)
char   *buf;
Ncols   n;
{
    long tempend;
    register S_fsd *f1,
		   *f2;
    register char  *p;

    tempend = ff_size (openffs[CHGFILE]);
    ff_seek (openffs[CHGFILE], tempend);
    try_ff_write (openffs[CHGFILE], buf, n = dechars (buf, n - 1));

 /* now make fsd */
    f1 = (S_fsd *) salloc (2 + SFSD, YES);
    f2 = (S_fsd *) salloc (SFSD, YES);
    f2->backptr = f1;
    f1->fwdptr = f2;
    f1->fsdnlines = 1;
    f1->fsdfile = ff_fd (openffs[CHGFILE]);
    f1->seekpos = tempend;
    p = f1->fsdbytes;
#ifndef NOSIGNEDCHAR
    if (n <= 127)
	*p = n;
    else {
	*p++ = -(n / 128);
	*p = n % 128;
    }
#else
    if (n > ~LLINE) {
	*p++ = -(n >> NLLINE);
	n &= ~LLINE;
    }
    *p = n
#endif
    return (f1);
}

/* getline (ln) - gets line ln of the current workspace into cline.
/*      returns
/*          1: ln is past last line
/*          0: got it OK
/*         -1: EOF
/**/
getline (ln)
Nlines  ln;
{
    static S_wksp *clinewksp;
    Ff_buf      *pbuf;
    S_fsd      *thecfsd;
    Ff_file    *theffile;
    long        pos,
                totlft;
    Short   offset,
            block;
    char   *endbuf,		  /* last available from this block     */
	   *endcl;
    Char    anewline,             /* to fake it on an eof               */
	    ch;
    register int   i1;
    int            i2;
    char          *cp1;       /* explained as used, below           */
    register char *cp2,
		  *cp3;       /* often more volativel than 1 or 2   */

    if (clineno == ln && curwksp == clinewksp) {
	/* already have the line */
	if (clineno >= nlines[curwksp->wfile]) /* note if past end */
	    xcline = 1;
	else
	    xcline = 0;
	return xcline;
    }
				  /* note if past end of real text      */
    ecline = 0;
    ncline = 0;
    fcline = 0;
    clineno = ln;
    anewline = '\n';

    /* get curfsd to contain current line */
    if (   (i1 = wposit ((clinewksp = curwksp), ln))
	|| ((i2 = (thecfsd = curwksp->curfsd)->fsdfile) == FAKEFSD)
       ) {
	/* eof/blank line->fake contents    */
	ncline = 1;
	cline[0] = '\n';
	if (i1)
	    xcline = 1;
	else
	    xcline = 0;
	return xcline;
    }
    theffile = ffchans[i2]->f_file;

/* get exact pos of line in file      */
				  /* i1  = length of a line */
				  /* i2  = lines into fsd   */
				  /* cp3 = ptr to fsd line bytes     */
    offset = 0;
    i2 = ln - curwksp->curflno;
    cp3 = thecfsd->fsdbytes;
    for (; i2-- != 0; offset += i1)
#ifndef NOSIGNEDCHAR
	if ((i1 = *cp3++) < 0)
	    i1 = -128 * i1 + *cp3++;
#else
	if ((i1 = *c++) & LLINE) { /* get down to the nth line */
	    i1 = - (i1 | LLINE) << NLLINE;
	    i1 += *c++;
	}
#endif

    pos = thecfsd->seekpos + offset;
    totlft = theffile->fn_size - pos;
    block = ldiv (pos, FF_BSIZE);
    offset = ldivr;
				  /* cp1 = ptr into source line    */
				  /* cp2 = ptr into dest (cline)   */

    endbuf = cp1 = 0;
    cp2 = cline;
    endcl = &cline[lcline - 9];
    for (;;) {
	if (cp1 == endbuf) {    /* need a block to work on          */
				/* (instead of using ff_read)       */
	    pbuf = ff_getblk (theffile, block++);
	    if (!pbuf || (totlft <= 0))
		cp1 = (char *)&anewline;
	    else {
		/* i1 = left in block */
		cp1 = &pbuf->fb_buf[offset];
		if (totlft < (i1 = FF_BSIZE - offset))
		    i1 = totlft;
		totlft -= i1;
		endbuf = &cp1[i1];
		offset = 0;
	    }
	}
	if (cp2 > endcl) {     /* check if room in cline           */
	    i1 = cp2 - cline;
	    excline (0);	  /* 8 is max needed (for tab)        */
	    cp2 = &cline[i1];
	    endcl = &cline[lcline - 9];
	}
	switch (ch = *cp1++ & 0177) {
	    case '\011':
		fill (cp2, (unsigned int) (i1 = 8 - (07 & (cp2 - cline))),
		      ' ');
		cp2 += i1;
		break;
	    case -1: 		  /* same as newline                  */
				  /* except prevent more reading      */
	    case '\n':
		for (cp3 = &cp2[-1];
			cp3 >= cline && *cp3 == ' '; cp3--);
				  /* take out trailing spaces         */
		if (cp3 < &cp2[-1])
		    cp2 = &cp3[1];
		*cp2 = '\n';
		ncline = cp2 - cline + 1;
		xcline = 0;
		return (0);       /* ((avail < 0) ? -1 : '\n'); */
	    default:
		if (   ch == ESCCHAR
		    || ch < 040
		   ) {
		    if (ch < 040)
			ch |= 0100;
		    ecline = 1;   /* line has control char        */
		    *cp2++ = ESCCHAR;
		}
		*cp2++ = ch;
	}
    }
}

/* putline() - inserts the line in cline in place of the current one */

putline (n)
Nlines  n;
{
    struct fsd *w0,
               *cl;
    Nlines  ln;
    Flag    flg;
    register S_fsd *wf,
		   *wg;
    register S_wksp *w;

    if (fcline == NO)
	return;
    fcline = NO;
    if (nlines[curfile] <= clineno)
	nlines[curfile] = clineno + 1;
    cline[ncline - 1] = '\n';
    cl = writemp (cline + n, ncline - n);
    w = curport->wksp;	  /* w s can be replaced by curwksp */
    ln = clineno;
    flg = breakfsd (w, ln);
    wg = w->curfsd;
    w0 = wg->backptr;
    if (flg == 0) {
	breakfsd (w, ln + 1);
	wf = w->curfsd;
	sfree ((char *) (cl->fwdptr));
	cl->fwdptr = wf;
	wf->backptr = cl;
    }
    sfree ((char *) wg);
    cl->backptr = w0;
    w->curfsd = cl;
    w->curlno = w->curflno = ln;
    fileflags[w->wfile] |= EDITED;
    clineno = -1;
    if (w0)
	w0->fwdptr = cl;
    else
	openfsds[w->wfile] = cl;
    fixfsds   (w->wfile, ln);
    redisplay (w->wfile, ln, 1, 0, NO);
}

/* copyfsd(f,end) - returns a copy of fsd f up to but not including
		end, or until its final block. */
S_fsd *
copyfsd (f, end)
S_fsd *f,
      *end;
{
    S_fsd *res,
	  *ff,
	  *rend;
    register Short  i;
    register char  *c1,
                   *c2;

#ifdef lint
    rend = f; /* to keep lint quiet */
#endif
    for (res = 0; f->fsdfile && f != end; f = f->fwdptr) {
	for (c1 = f->fsdbytes, i = f->fsdnlines; i; i--)
#ifndef NOSIGNEDCHAR
	    if (*c1++ < 0)
#else
	    if (*c1++ & LLINE)
#endif
		c1++;
	c2 = (char *) f;
	i = c1 - c2;
	ff = (S_fsd *) salloc (i, YES);
	move ((char *) f, (char *) ff, (unsigned int) i);
	if (res) {
	    rend->fwdptr = ff;
	    ff->backptr = rend;
	    rend = ff;
	}
	else
	    res = rend = ff;
    }
    if (res) {
	(rend->fwdptr = (S_fsd *) salloc (SFSD, YES))->backptr = rend;
	rend = rend->fwdptr;
    }
    else
	res = rend = (S_fsd *) salloc (SFSD, YES);
    if (f->fsdfile == 0)
	rend->seekpos = f->seekpos;
    return res;
}

/* freefsd(f) - frees all blocks in fsd */

freefsd (ff)
S_fsd **ff;
{
    register S_fsd *f;
    register S_fsd *g;

    f = *ff;
    while (g = f) {
	f = f->fwdptr;
	sfree ((char *) g);
    }
    *ff = 0;
}

/* blanklines (n) - returns an fsd chain of n blank lines */

S_fsd *
blanklines (n)
Nlines  n;
{
    Nlines  i;
    register S_fsd *f,
		   *g;

    for (f = (S_fsd *) salloc (SFSD, YES); n > 0; f = g) {
	i = min (n, FSDMAXL);
	g = (S_fsd *) salloc (SFSD + i, YES);
	g->fwdptr = f;
	f->backptr = g;
	g->fsdnlines = i;
	g->fsdfile = FAKEFSD;

	n -= i;
	fill (g->fsdbytes, (unsigned int) i, 1);
    }
    return (f);
}

try_ff_write (ffd, from, count)
Ff_stream *ffd;
char *from;
Short count;
{
    register Short i, j;

    for (i = 0; (j = ff_write (ffd, from, count)) != count; i++) {
	if ( j == -1)
	    fatal (FATALIO, "bad write on temp file");
	if ( i > 5)
	    fatal (FATALIO, "can't write on temp file");
	sleep(1);
	count -= j;
	from += j;
    }
}

/* fixfsds (fn, from) -
/*      fixfsds is called after a change has been made in file fn,
/*      starting at line from.
/*      We are supposed to adjust the position in the fsd chain of any
/*      workspace which may have had its curfsd pointer moved
/*      out from under it.
/*
/*      This stuff used to be done in redisplay (..) which now only
/*      does redisplaying.
/*      This routine will go away when the la_ package is installed.
/**/
fixfsds (fn, from)
Fn      fn;
Nlines  from;
{
    register S_wksp *tw;
    register Small  i;
    register Nlines wtop;

    clineno = -1;

    for (i = Z; i < nportlist; i++) {
	if ((tw = portlist[i]->altwksp)->wfile == fn) {
	    wtop = tw->wlin;
	    if (wtop + portlist[i]->btext >= from) {
		/* repoint file to avoid pointer muckup */
		tw->curlno = tw->curflno = 0;
		tw->curfsd = openfsds[fn];
	    }
	}
	if ((tw = portlist[i]->wksp)->wfile == fn) {
	    wtop = tw->wlin;
	    if (wtop + portlist[i]->btext >= from) {
		/* repoint file to avoid pointer muckup */
		tw->curlno = tw->curflno = 0;
		tw->curfsd = openfsds[fn];
	    }
	}
    }
}

/* lincnt (stline, nl) - if nl >= 0, count lines, else paragraphs
/*      return the actual number of lines.  End of file can cause
/*      the number to be less than nl.
/*      BUG: should consider a line with only a form-feed on it as blank.
/**/
lincnt (stline, nl)
Nlines  stline,
	nl;     /* nl < 0 => blocks; otherwise lines    */
{
    register S_fsd *ff;
    register char  *cp;
    Nlines  j;
    Nlines  tlines;
    Flag    bflag;

    breakfsd (curwksp, stline);

    bflag = 1;
    tlines = 0;
    for (ff = curwksp->curfsd; ff->fsdfile && nl; ff = ff->fwdptr) {
	j = ff->fsdnlines;
	if (ff->fsdfile == FAKEFSD) {
	    if (nl < 0) {
		if (bflag == 0 && ++nl == 0)
		    j = 0;
		bflag = 1;
	    }
	    else {
		if (j > nl)
		    j = nl;
		nl -= j;
	    }
	    tlines += j;
	}
	else {
	    for (cp = ff->fsdbytes; j; j--, cp++) {
		if (nl < 0) {     /* check blank line count */
		    if (bflag && *cp != 1)
			bflag = 0;
		    else if (bflag == 0 && *cp == 1) {
			bflag = 1;
			if (++nl == 0)
			    break;
		    }
		}
#ifndef NOSIGNEDCHAR
		if (*cp < 0)
#else
		if (*cp & LLINE)
#endif
		    cp++;
		++tlines;
		if (nl > 0 && --nl == 0)
		    break;	  /* check line count */
	    }
	}
    }
    return tlines;
}
