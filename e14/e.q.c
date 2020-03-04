/*
/* file e.q.c - quit stuff
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"
#include "e.cm.h"
#include "e.fn.h"
#include "e.tt.h"
#ifdef UNIXV7
#include <signal.h>
#endif
#ifdef UNIXV6
#include <sys/signals.h>
#endif

extern char *getenv ();

shell ()
{
    register char *cp;
    static char *args[] = {"-", (char *)0};

    for (cp = cmdopstr; *cp && *cp == ' '; cp++)
	{}

    if (*cp != '\0')
	return call ();

    if (endit () == 0)
	return CROK;

    docall (args);
    /* never returns */
    /* NOTREACHED */
}

call ()
{
    register char *cp;
    static char *args[] = {"sh", "-c", (char *)0, (char *)0};

    for (cp = cmdopstr; *cp && *cp == ' '; cp++)
	{}

    if (*cp == '\0')
	return CRNEEDARG;

    if (endit () == 0)
	return CROK;

    args[2] = cp;
    docall (args);
    /* never returns */
    /* NOTREACHED */
}

docall (args)
char *args[];
{
    char *ename;
    int child;
    int retstat;
    register Small j;

    d_put (0);
    fixtty ();
    clearscreen ();
    windowsup = NO;
    savestate ();               /* to work from where it  left off    */
    cleanup (YES, YES);           /* restore terminal modes; free tmp   */
    for (j = 1; j <= NSIG ; j++) {
	if (j == SIGINT || j == SIGQUIT)
	    signal (j, SIG_IGN);
	else
	    signal (j, SIG_DFL);
    }
    for (j = 2; j < 25 /*HIGHFD*/; )
	close (j++);
    dup (OUTSTREAM);

#ifdef UNIXV7
    shpath = append (getenv ("SHELL"), "");
#endif
#ifdef UNIXV6
    getpath ("sh", &shpath, NO);
#endif
    if ((child = fork ()) != -1) {
	if (child == 0) {
	    execv (shpath, args);
	    printf ("Can't exec shell\n");
	    fflush (stdout);
	    exit (-1);
	}
	else {
	    while (wait (&retstat) != child)
		{}
	}
    }
    else
	printf ("Can't fork a shell\n");

    printf ("Hit <RETURN> to go back to the editor. ");
    fflush (stdout);
    while ((j = getchar ()) != EOF  && (j & 0177) != '\n')
	{}

#ifdef UNIXV7
    ename = getenv ("PROGPATH");
    execl (ename, append (loginflg? "-": "", ename), 0);
#endif
#ifdef UNIXV6
    if (loginflg) {
	execl (ENAME, "-e", 0);
    }
    else {
	execl (ENAME, "e", 0);
    }
#endif
    printf ("Can't reenter editor \n");
    exit (-1);
}

#define EXABORT         0
#define EXDUMP          1
#define EXNORMAL        2
#define EXNOSAVE        3
#define EXQUIT          4

eexit ()
{
    Small extblind;
    static S_looktbl exittable[] = {
	"abort"   , EXABORT    ,
	"dump"    , EXDUMP     ,
	"nosave"  , EXNOSAVE   ,
	"quit"    , EXQUIT     ,
	0, 0
    };

    if (opstr[0] == '\0')
	extblind = EXNORMAL;
    else {
	if (*nxtop)
	    return CRTOOMANYARGS;
	extblind = lookup (opstr, exittable);
	if (extblind == -1  || extblind == -2) {
	    mesg (ERRSTRT + 1, opstr);
	    return extblind;
	}
	extblind = exittable[extblind].val;
    }

    switch (extblind) {
    case EXNORMAL:
	if (endit () == 0)
	    return CROK;
    case EXABORT:
    case EXDUMP:
    case EXNOSAVE:
    case EXQUIT:
	quit (extblind);
	/* quit never returns */
    }
    return CRBADARG;
}

/* endit() - finishes up
/**/
endit ()
{
    register Fn i;

    /* The strategy here is to stave off all permanent actions until as
    /* late as possible.  Deletes and renames are not done until necessary.
    /* On the other hand, according to this precept, all of the modified
    /* files should be saved to temp files first, and then linked or
    /* copied to backups, etc.  But this would take too much disk space.
    /* So the saves go one at a time, with some deleting and renaming along
    /* the way.  If we bomb during a save and any deletes or renames have
    /* happened, we're probably screwed if we want to replay
    /**/

    /* if any of the files are to be saved inplace, then it is possible
    /* that some fsds might point into them and so all files should go
    /* through the tmp save stage first before the final linkups are done,
    /* and in the case of inplace saves, the originals are copied over */
    for (i = FIRSTFILE; i < MAXFILES; i++) {
	if (   (fileflags[i] & (UPDATE | EDITED | INPLACE))
	       == (UPDATE | EDITED | INPLACE)
	    && fmultlinks (i)
	   ) {
	    for (i = FIRSTFILE; i < MAXFILES; i++) {
		if ((fileflags[i] & (UPDATE | EDITED)) == (UPDATE | EDITED)) {
		    if (savefile ((char *)0, i, NO) != 1)
			return 0;
		}
	    }
	    break;
	}
    }

    /* now the final saves */
    for (i = FIRSTFILE; i < MAXFILES; i++) {
	if (   (fileflags[i] & (EDITED | NEW | SAVED))
	    && (fileflags[i] & UPDATE)
	   ) {
	    if (savefile ((char *)0, i, YES) != 1)
		return 0;
	}
    }

#ifdef DELETE
    /* do any deleting as necessary
    /* if any deleted names conflicted with NEW names or RENAMED names,
    /* they were already deleted by the final save loop */
    for (i = FIRSTFILE; i < MAXFILES; i++) {
	if ((fileflags[i] & (UPDATE | DELETED)) == (UPDATE | DELETED)) {
	    mesg (TELALL + 2, "DELETE: ", names[i]);
	    unlink (names[i]);
	}
    }
#endif DELETE

#ifdef NAME
    /* do any renaming as necessary
    /* if any oldnames conflict with NEW names,
    /* or if any saved names wre RENAMED,
    /* they were already renamed by the final save loop */
    for (i = FIRSTFILE; i < MAXFILES; i++) {
	if ((fileflags[i] & (UPDATE | RENAMED)) == (UPDATE | RENAMED)) {
	    mesg (TELALL + 4, "RENAME: ", oldnames[i], " to ", names[i]);
	    mv (oldnames[i], names[i]);
	}
    }
#endif NAME

    return 1;
}

#ifdef COMMENT
Void
quit (qtype)
    Small qtype;
.
    Quit never returns.  From here on out its all the way to exit().
    Type of exit:
		  saves files   update state   rmv keys   rmv chg file
		  -----------   ------------   --------   ------------
    EXNORMAL           X             X             X          X
    EXNOSAVE           -             X             X          X
    EXQUIT             -             -             X          X
    EXABORT            -             -             -          X
    EXDUMP             -             -             -          -
#endif
Void
quit (qtype)
Small qtype;
{
    register Fn fn;
    Flag firsttime = 1;
    char *execargs[2];
    register Small i;

    d_put (0);
    fixtty ();
    clearscreen ();
    windowsup = NO;

    switch (qtype) {
    case EXDUMP:
	fatal (-1, "Aborted");

    case EXNORMAL:
    case EXNOSAVE:
	if (!notracks)
	    savestate ();
    case EXQUIT:
    case EXABORT:
	cleanup (YES, qtype != EXABORT);
	if (qtype == EXNORMAL)
	    exit (0);
    }
    exit (1);
    /* NOTREACHED */
}


/*  savestate () -- save it for later
/**/
savestate ()
{
    Short   nletters;
    register Short  i;
    register char  *f1;
    char   *fname;
    register S_window *port;
    FILE *sbuff;

    curwksp->ccol = cursorcol;
    curwksp->clin = cursorline;

    unlink (rfile);
    if ((sbuff = fopen (rfile, "w")) == NULL)
	return -1;
    chmod (rfile,0600);
    /* put out a flag word whitch won't match revision number  */
    /* later, if all is OK, we'll put out revision number */
    putshort (0, sbuff);

    putc (term.tt_height, sbuff);
    putc (term.tt_width, sbuff);

    putlong (strttime, sbuff);

    putshort (ntabs, sbuff);
    if (ntabs > 0) {
	i = 0;
	do
	    putshort (tabs[i++], sbuff);
	while (i < ntabs);
    }

    putshort (linewidth, sbuff);

    if (searchkey == 0)
	putshort (0, sbuff);
    else {
	for (f1 = searchkey; *f1++; )
	    {}
	putshort (f1 - searchkey, sbuff);
	f1 = searchkey;
	do
	    putc (*f1, sbuff);
	while (*f1++);
    }

    putc (imodesw, sbuff);

    putc (curmark != 0, sbuff);
    if (curmark) {
	putshort (curmark->mrkwinlin, sbuff);
	putshort (curmark->mrkwincol, sbuff);
	putc     (curmark->mrklin, sbuff);
	putshort (curmark->mrkcol, sbuff);
    }

    putc (nportlist, sbuff);
    for (i = Z; i < nportlist; i++)
	if (portlist[i] == curport)
	    break;
    putc (i, sbuff);
    for (i = Z; i < nportlist; i++) {
	port = portlist[i];
	putc (port->prevport, sbuff);
	putc     (port->tmarg, sbuff);
	putshort (port->lmarg, sbuff);
	putc     (port->bmarg, sbuff);
	putshort (port->rmarg, sbuff);
	if (f1 = fname = names[port->altwksp->wfile]) {
	    while (*f1++)
		{}
	    nletters = f1 - fname;
	    putshort (nletters, sbuff);
	    f1 = fname;
	    do
		putc (*f1, sbuff);
	    while (*f1++);
	    putshort (port->altwksp->wlin, sbuff);
	    putshort (port->altwksp->wcol, sbuff);
	    putc     (port->altwksp->clin, sbuff);
	    putshort (port->altwksp->ccol, sbuff);
	}
	else
	    putshort (0, sbuff);
	f1 = fname = names[port->wksp->wfile];
	while (*f1++)
	    {}
	nletters = f1 - fname;
	putshort (nletters, sbuff);
	f1 = fname;
	do
	    putc (*f1, sbuff);
	while (*f1++);
	putshort (port->wksp->wlin, sbuff);
	putshort (port->wksp->wcol, sbuff);
	putc     (port->wksp->clin, sbuff);
	putshort (port->wksp->ccol, sbuff);
    }
    if (ferror (sbuff)) {
	fclose (sbuff);
	return -1;
    }
    fseek (sbuff, 0, 0l);
    putshort ( revision, sbuff);   /* state file is OK */
    fclose (sbuff);
    return  0;
}
