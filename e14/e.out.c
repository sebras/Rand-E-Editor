/*
/* file e.out.c: fsdtofil (..)
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

/* fsdtofil(f,nl,newf,allowint) - write a fsd chain out to a file.
	f is a pointer to the fsd chain.
	newf is the filedescriptor for the file to be written.
	The first nl lines are written, unless the file ends first.
	Returns total number lines written, or -1 if write error. */

fsdtofil (ff, nl, newf, allowint)
register S_fsd *ff;
Nlines  nl;
Fd      newf;
Flag    allowint;
{
    Ff_stream *pfile;
    Ff_buf    *pbuf;
    FILE       writbuf;
    long       totlft;
    Small      k;
    Nlines     tlines;
    Short      nlft,
	       offset,               /* offset to begin of block */
	       block;
    char      *c;
    Flag       all = NO;
    register Short  i;
    register Nlines j;

    if (nl == 0) {
	nl = 32767;
	all = YES;
    }
    fwrprep (&writbuf, newf);
    for (tlines = 0; ff->fsdfile && nl; ff = ff->fwdptr) {
	/* once per fsd */
	j = ff->fsdnlines;
	if (ff->fsdfile == FAKEFSD) {
	    if (j > nl)
		j = nl;
	    for (nl -= k = j; k--;) {
		putc ('\n', &writbuf);
		if (ferror(&writbuf))
		    goto wrerr;
	    }
	    tlines += j;
	}
	else {
	    for (i = Z, c = ff->fsdbytes; j && nl;
		    i += *c++, ++tlines, j--, nl--)
#ifndef NOSIGNEDCHAR
		if (*c < 0)       /* total num chars this fsd     */
		    i -= 128 * *c++;
#else
		if (*c & LLINE)
		    i += - (*c++ | LLINE) << NLLINE;
#endif
	    pfile = ffchans[ff->fsdfile];
	    pfile->f_offset = ff->seekpos;
				  /* fake a seek  */
	    for (block = ldiv (ff->seekpos, FF_BSIZE), offset = ldivr;
		    i; i -= j, offset = 0) {
				  /* need a block to work on          */
				  /* (instead of using ff_read)       */
		pbuf = ff_getblk (pfile->f_file, block++);
		totlft = pfile->f_file->fn_size - pfile->f_offset;
		if (!pbuf || (totlft <= 0)) {
				  /* eof                              */
		    if (totlft <= 0)
			goto endfsdtofil;
		    else {
			mesg (ERRALL + 1, "ERROR Reading disk. Seek help!");
			close (newf);
			return -1;
		    }

		}
		nlft = (totlft < (FF_BSIZE - offset))
		    ? totlft : (FF_BSIZE - offset);
		if (allowint && intrupnum++ > SRCHPOL && sintrup ()) {
		    /* interrupted during Exec      */
		    close (newf);
		    return -1;
		}
		fwrite ( &pbuf->fb_buf[offset],
			    (j = (i < nlft) ? i : nlft), 1, &writbuf);
		if (ferror(&writbuf))
		    goto wrerr;
		pfile->f_offset += j;
	    }
	}
	if (all)
	    nl = 32767;
    }
endfsdtofil:
    flushkeys ();
    fflush (&writbuf);
wrerr:
    close (newf);
    if (ferror(&writbuf)) {
	mesg (ERRALL + 1, "ERROR Writing to disk, seek help!");
	return -1;
    }
    return tlines;
}
