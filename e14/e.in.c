/*
/* file e.in.c: filtofsd (..)
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

#define NBYMAX 150		  /* max bytes +1 for fsdbytes */

/* filtofsd(fname) - parse file into fsd chain, starting at pos      */

S_fsd *
filtofsd (filenum, pos, allowint)
Fn   filenum;
long pos;
Flag allowint;
{
    S_fsd      *firstfsd,
               *lastfsd;
    Ff_stream  *pfile;  /* pointer to ffile structure    */
    Ff_buf     *pbuf;      /* pointer to fbuf structure returned by getblk*/
    long       fsdpos,  /* seek pos in openffs[chan] of start of fsd       */
	       linpos,  /* seek pos in openffs[chan] of start of line      */
	       totlft;  /* total bytes left to parse in openffs[filenum]   */
    Short   nlft;       /* num of bytes left to work on in current block   */
    Small   nby;        /* index into fby                                  */
    Short   offset,               /* offset to begin of block */
	    block;
    Nlines  nl;
    char    fby[NBYMAX + 1],/* the fsdbytes to be put into the fsd struct  */
	   *obpt;           /* old bpt                                     */
    register S_fsd *thisfsd;
    register Short  regi;
    register char  *bpt;

    pfile = openffs[filenum];
    fsdpos = linpos = pos;
    totlft = pfile->f_file->fn_size - linpos;
    if ( (obpt = salloc ((int)(totlft/20), YES)) != NULL)
	sfree (obpt);
    block = ldiv (linpos, FF_BSIZE);
    offset = ldivr;
    firstfsd = 0;
    thisfsd = 0;
    lastfsd = 0;
    nl = 0;
    nby = 0;
    nlft = 0;

    for (;;) {  /* once per line */
	if (allowint && intrupnum++ > SRCHPOL && sintrup ()) {
	    /* interrupted during Exec */
	    if (firstfsd)
		freefsd (&firstfsd); /* free up all the fsds just made  */
	    return 0;
	}

	for (regi = Z; ; bpt++) {
				  /* parse a line.   NOTE:  count is  */
				  /* of REAL chars, not expansions    */
	    if (--nlft < 0) {     /* need a block to work on          */
				  /* (instead of using ff_read)       */
		if (totlft <= 0)
		    break;
		if ( !(pbuf = ff_getblk (pfile->f_file, block++)))
		/*! WHAT about errors from getblk */
		/* also, since this is inherently a sequential access     */
		/* operation, getblk probably shouldn't be used here      */
		    break;
		bpt = &pbuf->fb_buf[offset];
		if ((nlft = (FF_BSIZE - offset)) > totlft)
		    nlft = totlft;
		totlft -= nlft--;
		offset = 0;
	    }
	    if (++regi > (128*128) + 127) /* overflow -- too many chars    */
		/*! there are still linelengths > 12000 that bomb */
		fatal (FATALBUG, "Line too long to handle");
	    if (   *bpt == '\n'
	       )
		break;
	}
	bpt++;
	if (regi > 0) {
	    linpos += regi;
#ifndef NOSIGNEDCHAR
	    if (regi > 127) {
		fby[nby++] = -(regi / 128);
		regi %= 128;
	    }
	    fby[nby++] = regi;
#else
	    if (regi > ~LLINE) {
		fby[nby++] = -(regi >> NLLINE);
		regi &= ~LLINE;
	    }
	    fby[nby] = regi
#endif
	    nl++;
	}
	if ((nlft < 0) || (nby >= NBYMAX) || (nl >= FSDMAXL)) {
	    /* finish off this fsd and make the next one                   */
	    lastfsd = thisfsd;
	    thisfsd = (S_fsd *) salloc (SFSD + nby, YES);
	    if (firstfsd == 0)
		firstfsd = thisfsd;
	    else
		lastfsd->fwdptr = thisfsd;
	    thisfsd->backptr = lastfsd;
	    thisfsd->fwdptr = 0;
	    thisfsd->fsdnlines = nl;
	    nlines[filenum] += nl;
	    thisfsd->fsdfile = ff_fd (openffs[filenum]);
	    thisfsd->seekpos = fsdpos;
	    obpt = bpt;
	    for (regi = Z, bpt = thisfsd->fsdbytes; regi < nby;)
		*(bpt++) = fby[regi++];
	    bpt = obpt;
	    if (nlft < 0)
		break;
	    fsdpos = linpos;
	    nl = nby = 0;
	}
    }
    /* put eof block and return */
    thisfsd->fwdptr = lastfsd = (S_fsd *) salloc (SFSD, YES);
    lastfsd->backptr = thisfsd;
    return firstfsd;
}
