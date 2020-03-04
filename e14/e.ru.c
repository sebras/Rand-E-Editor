/*
/* file e.ru.c: functions requiring forking
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.fsd.h"
#include "e.m.h"
#include "e.cm.h"
#include "e.ru.h"

#ifdef UNIXV7
#include <signal.h>
#endif

#ifdef UNIXV6
#include <sys/signals.h>
#endif

S_looktbl filltable[] = {
    "width"   , 0       ,
    0         , 0
};

print ()
{
    return filter (PRINTNAMEINDEX, NO);
}

filter (whichfilter, closeflg)
Small   whichfilter;
Flag    closeflg;
{
    Nlines stline;
    register Small tmp;
    register Small retval = 0;
    char *cp;

    if (!okwrite ())
	return NOWRITERR;

    cp = cmdopstr;
    tmplinewidth = linewidth;
    if ((tmp = scanopts (&cp, filltable, whichfilter != CENTERNAMEINDEX)) < 0)
	return tmp;
    if (*cp != '\0')
	return CRBADARG;
    switch (tmp) {
    case 0:
	linewidth = tmplinewidth;
	if (whichfilter == PRINTNAMEINDEX) {
	    stline = 0;
	    parmlines = nlines[curfile];
	}
	else {
	    parmlines = whichfilter == CENTERNAMEINDEX? 1
			: lincnt (curwksp->wlin + cursorline, -1);
	    stline = curwksp->wlin + cursorline;
	}
	retval = filtlines (whichfilter,
			     stline,
			      parmlines, YES, closeflg);
	break;

    case 1:
	linewidth = tmplinewidth;
	retval = filtlines (whichfilter, curwksp->wlin + cursorline,
			    parmlines, YES, closeflg);
	break;

    case 2:
	return NORECTERR;
    case 3:
	if (markcols)
	    return NORECTERR;
	linewidth = tmplinewidth;
	retval = filtmark (whichfilter, closeflg);
	break;

    default:
	return tmp;
    }
    return retval;
}

filtmark (whichfilter, closeflg)
Small   whichfilter;
Flag    closeflg;
{
    register Flag moved;
    register Small retval = 0;

    if (markcols)
	return NORECTERR;
    moved = gtumark (YES);
/*      retval = filtrect (whichfilter, topmark (), leftmark (),
			    markcols, marklines, !moved, closeflg);
    else
*/      retval = filtlines (whichfilter, topmark (), marklines,
			     !moved, closeflg);
    if (moved)
	putupwin ();
    unmark ();
    return retval;
}

extern char *filters[], *filterpaths[];

filtlines (whichfilter, from, number, puflg, closeflg)
register Small whichfilter;
Nlines  from,
	number;
Flag    puflg,
	closeflg;
{
    char *args[4];
    char buf[10];
    register Small ix;

    ix = Z;
    args[ix] = filters[whichfilter];
    switch (whichfilter) {
    case FILLNAMEINDEX:
    case JUSTNAMEINDEX:
    case CENTERNAMEINDEX:
	args[++ix] = "-x";
	sprintf (buf, "-l%d", linewidth);
	args[++ix] = buf;
	break;

    case PRINTNAMEINDEX:
	break;

    default:
	mesg (TELALL + 1, "BUG!");
	return CRBADARG;
    }
    args[++ix] = 0;

    mesg (TELALL + 2, cmdname, "ing.  Please wait.");
    d_put (0);
    beepsw = YES;
    return runlines (filterpaths[whichfilter],
		      args, from, number, puflg, closeflg, YES);
}

/*  run () - does the exec function.
/*
/*    Returns:
/*      0 = OK
/*      s= nostrerr
/*      2 = nowriterr
/*      3 = noargerr
/*      4 = nopiperr
/*
/**/

run (cmdstr, closeflg)
char *cmdstr;
Flag closeflg;
{
    register Small j;
    Short   retval;
    Nlines  tlines;
    Nlines  stline,     /* start line of exec */
	    nmlines;    /* num of lines to send to program */
    Flag   moved = 0;
    char *cp;
#ifdef UNIXV7
    extern char *getenv ();
#endif

    cp = cmdstr;
    j = getpartype (&cp, YES, NO, curwksp->wlin + cursorline);
    if (j == 1) {
	if (curmark)
	    return NOMARKERR;
	tlines = parmlines;
    }
    else if (j == 2)
	return NORECTERR;
    else if (curmark) {
	if (markcols)
	    return NORECTERR;
	moved = gtumark (YES);
	tlines = marklines;
	unmark ();
    }
    else
	tlines = 0;

    for (; *cp && *cp == ' '; cp++)
	{}

    stline = curwksp->wlin + cursorline;
    nmlines = lincnt (stline, tlines);

    mesg (TELALL + 2, "RUN: ", cp);
    d_put (0);
    beepsw = YES;

#ifdef UNIXV7
    if (!(shpath = getenv ("SHELL")))
#endif
	getpath ("sh", &shpath, NO);
    shargs[2] = cp;
    retval = runlines (shpath, shargs, stline, nmlines, !moved, closeflg,
			YES);
    if (moved)
	putupwin ();
    return retval;
}

runlines (progpath, args, from, number, puflg, closeflg, insflg)
char   *progpath;
char   *args[];
Nlines  from,
	number;
Flag    puflg,
	closeflg,
	insflg;
{
    register Small j;
    int     pipef[2],
	    pipeg[2],
	    childid,
	    childe;

    if (pipe (pipef) == -1)
	return NOPIPERR;

    breakfsd (curwksp, from);
    /* flushing must be done before the fork or the */
    /* child e will flush the stuff also          */
    d_put (0);
    flushkeys ();
    {
    FILE *iop;
    for (iop = _iob; iop < _iob + _NFILE; iop++)
	fflush (iop);
    }
    ff_flush (openffs[CHGFILE]);
		      /* closed and "re-opened" by child   */
		      /* separately; see below             */

    /* It'll take some work to be able to use vfork here */
    if ((childe = fork ()) == -1)
	return NOPIPERR;
    if (childe == 0) {
	/* child e process */
	ischild = YES;          /* looked at by fatal (..) */
	if (pipe (pipeg) == -1)
	    _exit (-1);
#ifdef VMUNIX
	if ((childid = vfork ()) == -1)
#else
	if ((childid = fork ()) == -1)
#endif
	    exit (-1);          /* dy */
	if (childid == 0) {
	    /* grandchild (application pgm run by sh)  */
	    close (0);          /* pipe is standard input             */
	    dup (pipeg[0]);     /* read from child e */
	    close (1);          /* write to parent e */
	    dup (pipef[1]);
	    /* Pass on virgin fd state */
	    for (j = 2; j < NOFILE; )
		close (j++);
	    execv (progpath, args);
	    _exit (-1);
	}
	for (j = 1; j <= NSIG ; j++)
	    signal (j, SIG_DFL);
	close (pipef[1]);
	close (pipef[0]);
	close (pipeg[0]);
	xf_fdchng (openffs[CHGFILE], tmpname, 0);
	feedexec (childid, from, number, pipeg[1]);
	/* feedexec never returns */
	_exit (-1);    /* shouldn't get here. */

/*  In a sane world, these various shenanigans with CHGFILE wouldn't be
/*  needed, but we have these two processes sharing an fd and they can
/*  screw each other up, so the child simply won't use the same handle;
/*  note that this mucks a lot with the innards of ff data structures.  It
/*  really is a lot more efficient to do it this filthy a way, otherwise...
/**/
    }

    /* parent process here after fork     */
    close (pipef[1]);
    suckexec (from, number, childe, pipef[0], puflg, closeflg, insflg);
    return CROK;
}

Fd      feedout;
int     feedid;

feedexec (i, start, nmlines, outchan)
int     i;
Nlines  start;
Nlines  nmlines;
Fd      outchan;
{
    Void feedabort ();
    Void feedend ();

    feedid = i;
    feedout = outchan;

    signal (SIGINT, feedabort);       /* signal from parent          */
    signal (SIGPIPE, feedend);        /* in case application dies    */

    if (nmlines == 0)
	close (outchan);
    else
	fsdtofil (curwksp->curfsd, nmlines, outchan, NO);

    feedend ();
}

Void
feedabort ()
{
    signal (SIGINT, SIG_IGN);
    close (feedout);
    kill (feedid, SIGKILL);
    feedend ();
}

Void
feedend ()
{
    int     retstat;

    signal (SIGINT, SIG_IGN);
    while (wait (&retstat) != feedid);
				  /* wait for completion        */
    exit (retstat >> 8);	  /* pass status to daddy       */
}

suckexec (stline, lines, childe, inchan, puflg, closeflg, insflg)
Nlines  stline,
	lines;
int     childe;
Fd      inchan;
Flag    puflg,          /* putup when done */
	closeflg,       /* close the lines sent to the command */
	insflg;         /* insert the result into the current file */
{
    long    tempend;
    int     retstat;
    Nlines  cllines;
    char    suckbuf[BUFSIZ];    /* for inhaling text through a pipe   */
    Ff_stream *chgffs;
    S_fsd   *ee;
    register S_fsd *e;
    register Nlines l;

    /* suck from the pipe and put it on the end of the chng file    */
    chgffs = openffs[CHGFILE];
    tempend = ff_size (chgffs);
    ff_seek (chgffs, tempend);
				  /* get to end of temp                 */

    for (;;) {
	alarmed = 0;
	signal (SIGALRM, alarmproc);
	alarm (EXECTIM);
	while ((l = read (inchan, suckbuf, sizeof suckbuf)) > 0)
	    try_ff_write (chgffs, suckbuf, l);
	if ( !alarmed ) {
	    alarm (0);
	    alarm (0);
	    alarmed = 0;
	    break;
	}
	if (sintrup ())
	    break;
    }
#ifdef CATCHSIGS
    signal (SIGALRM, sig);
    signal (SIGALRM, sig);
#endif

    close (inchan);

    /* Is this necessary ? */
    for (l = Z; l < NOFILE; l++)  /* child e mucked w/fd's           */
	if (ff_files[l].fn_fd != 0)
	    lseek (ff_files[l].fn_fd,
		    (long)ff_files[l].fn_realblk*FF_BSIZE, 0);

    if (alarmed)
	kill (childe, SIGINT);              /* eliminate child e    */
    while (wait (&retstat) != childe)
	{}
    if ( !alarmed ) {
	if ((retstat & ~0377) == ~0377) {
	    mesg (ERRALL + 1, "Can't find program to execute.");
	    return;
	}
	if ((retstat & ~0377) == ~0777 || (retstat & 0377)) {
	    mesg (ERRALL + 1, "Abnormal termination of program.");
	    return;
	}

	/* ask filtofsd to allow "interruption" */
	if (insflg)
	    ee = e = filtofsd (CHGFILE, tempend, YES);
	if (e) {
	    if (closeflg && lines) {
		closelines (stline, lines, NO, NO);
		cllines = lines;
	    }
	    else
		cllines = 0;
	    for (l = Z; e->fsdfile; l += e->fsdnlines, e = e->fwdptr)
		{}
	    if (l)
		if (insflg) {
		    /* did we have any? */
		    if (nlines[curfile] < stline)
			nlines[curfile] = stline;
		    nlines[curfile] += l;
		    insert (curwksp, ee, stline);
		}
		else
		    llreturned (ee);
	    if (closeflg || (insflg)) {
		fixfsds   (curfile, stline);
		redisplay (curfile, stline, cllines, l - cllines, puflg);
	    }
	    if (closeflg && lines > 0 && l == 0)  /* so what did we do???           */
		mesg (ERRALL + 1, "No lines returned.");
	    return;
	}
    }

    mesg (ERRALL + 1, "Exec aborted.");
    d_put (0);
    sleep (1);
}


/* xf_fdchng - handles editing functions for "Execute filter" function
/**/
xf_fdchng (afp, file, mode)
Ff_stream *afp;
char   *file;
int     mode;
{
    int     md;
    register Ff_stream *fp;
    register Ff_file   *fn;

    md = (mode + 1) | F_READ;
    mode = md - 1;
    close ((fn = ((fp = afp)->f_file))->fn_fd);
    fn->fn_fd = open (file, mode);
    fn->fn_mode = fp->f_mode = md;
    fn->fn_realblk = 0;
}

Void
alarmproc ()
{
    alarmed = 1;
}

llreturned (afs) /* do a mesg () of last line of fsd chain */
S_fsd *afs;
{

}
