/*
/* file e.sb.c - subroutines
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.fn.h"
#include "e.sg.h"
#ifdef UNIXV7
#include <signal.h>
#endif
#ifdef UNIXV6
#include <sys/signals.h>
#endif

/* getpath (...) - finds where name is.
/*      Must be called before path is used, and if path is used
/*      in a child process, getpath should be been called in the PARENT
/*      so that it won't have to do any work on subsequent calls.
/**/
getpath (name, path, tryagain)
register char *name;
char **path;
Flag tryagain;
{
    static char usrbin[] = "/usr/bin/";
    register char *cp1;
    register char *cp2;

    if (*path && !tryagain)
	return;

    if (access (name, 1) == 0) {  /* try current directory */
	*path = append (name, "");
	return;
    }

    cp1 = append (getmypath (), usrbin + 4);  /* try user's bin directory */
    cp2 = append (cp1, name);
    sfree (cp1);
    if (access (cp2, 1) == 0) {
	*path  = cp2;
	return;
    }
    sfree (cp2);

    cp2 = append (usrbin + 4, name);    /* try /bin */
    if (access (cp2, 1) == 0) {
	*path  = cp2;
	return;
    }
    sfree (cp2);

    *path = append (usrbin, name);  /* try /usr/bin */
    return;
}

char *
getmypath ()
{
    register char *cp;
    register Short i;
    char    *path;
    char     pwbuf[132];

    if (mypath)
	return mypath;
    if (getpw (userid, pwbuf))
	fatal (FATALIO, "getpw failed!");

    for (cp = pwbuf; *cp != ':'; cp++)
	{}                        /* define my name             */
    *cp = '\0';                    /* terminate it               */
    myname = append (pwbuf, "");
    for (i = 4; i--;)		  /* skip some fields           */
	while (*cp++ != ':')
	    {}
    for (path = cp; *cp != ':'; cp++)
	{}                        /* and my path                */
    *cp = 0;                       /* make it asciz              */
    mypath = append (path, "");

    return mypath;
}

/*  fixtty() --  fix tty modes */
/**/
fixtty ()
{
    d_put (0);

    if (ostyflg) {
	stty (OUTSTREAM, &outstty);
	ostyflg = NO;
#ifdef MESG_NO
	if (ttynstr != NULL)
	    chmod (ttynstr, (int) (07777 & oldttmode));
#endif
    }
    if (istyflg) {
#ifdef V6XSET
	stty (INSTREAM | XSET, &inxstty);
#endif
	stty (INSTREAM, &instty);
	istyflg = NO;
    }
}

/*  cleanup() --  cleanup before getting out  */
/**/
cleanup (filclean, rmkeysflg)
Flag filclean,
     rmkeysflg;
{
    if (filclean && tmpname) {
	close (ff_fd (openffs[CHGFILE]));
	unlink (tmpname);
    }

    if (keyfile != NULL) {      /* cleanup may be called before it's open */
	if (rmkeysflg) {
	    unlink (keytmp);
	    unlink (bkeytmp);
	}
	else {
	    flushkeys ();       /* one last check for write errors */
	    fclose (keyfile);
	}
    }
}


/* fatal(...) -- routine to let user know of mishaps
/**/
/* VARARGS2 */
fatal (type, msg)
Flag    type;
char   *msg;
{
    if (ischild)
	exit (-1);
    fixtty ();
    if (windowsup) {
	clearscreen ();
	windowsup = 0;
    }
    d_put (0);

    fatalpr ("\007=*==*==*==*==*==*=\n");
    switch (type) {
    case -1:
	fatalpr ("You asked for a dump.\n");
	break;

    case FATALMEM:
	fatalpr ("\
%s just ran out of memory.\n", progname);
	break;

    case FATALIO:
	fatalpr ("\
%s just had fatal trouble with the disk.:\n*** %r", progname, &msg);
	fatalpr (" ***\n");
	break;

    case FATALSIG:
    case FATALBUG:
	fatalpr ("\
A bug in %s ", progname);
	fatalpr ("\
just caused it to crash.\n");
	fatalpr ("\
Please notify system administrators that the bug was:\n*** %r", &msg);
	fatalpr (" ***\n\
They can tell you if you can recover from that one.\n");
	break;
    }
    fatalpr ("\
If you want to recover your work, ");
    if (loginflg) {
	fatalpr ("log in again,\n");
	fatalpr ("\
wait for %s to finish recovering, and logoff.\n", progname);
    }
    else {
	fatalpr ("run %s again with no arguments,\n", progname);
	fatalpr ("\
and it will recover automatically.  Wait for %s to finish recovering,\n",
	    progname);
	fatalpr ("\
and exit immediately.\n");
    }
    fatalpr ("That should fix things up.\n");
    fatalpr ("\007\n\n");
    d_put (0);
    if (keyfile != NULL)
	fclose (keyfile);
    if (replaying && keysmoved && strcmp (inpfname, bkeytmp) == 0)
	mv (bkeytmp, keytmp);
    fflush (stdout);

    if (loginflg) {
	signal (SIGINT, SIG_DFL);
	signal (SIGQUIT, SIG_DFL);
	sleep (60);
    }

    switch (type) {
    case -1:
    case FATALSIG:
    case FATALBUG:
    case FATALMEM:
	_cleanup ();
#ifdef VAX
	signal (SIGILL, SIG_DFL); /* set IOT sig back to deflt for abort() */
#else
	signal (SIGIOT, SIG_DFL); /* set IOT sig back to deflt for abort() */
#endif
	abort();

    case FATALIO:
    default: ;
    }
    exit (-1);
}

#define NUMADRS 1500
#ifdef DEBUGDEF
char   *memaddrs[NUMADRS];
#endif

char *
gsalloc (oldp, ncopy, newsize, fatalflg)
char *oldp;
int ncopy;
int newsize;
Flag fatalflg;
{
    char *newp;

    newp = salloc (newsize, fatalflg);
    if ((ncopy = min (ncopy, newsize)) > 0)
	move (oldp, newp, (unsigned int) ncopy);
    sfree (oldp);
    return newp;
}

char *
salloc (n, fatalflg)
int n;
Flag fatalflg;
{
    register char  *cp;
#ifdef DEBUGDEF
    register Short  ind;
#endif
    extern char *calloc ();

    if ((cp = calloc ((unsigned) n, 1)) == NULL) {
	if (fatalflg)
	    fatal (FATALMEM, "");
	else
	    mesg (ERRALL + 1, "You have run out of memory. Get out NOW!");
    }

#ifdef DEBUGDEF
    for (ind = Z; ind < NUMADRS; ind++)
	if (memaddrs[ind] == 0) {
	    memaddrs[ind] = cp;
	    return cp;
	}
    fatal (FATALBUG, "NUMADRS is too small");
#endif

    return cp;
}

#ifdef DEBUGDEF
sfree (n)
char   *n;
{
    extern int end;
    register Short ind;

    if (n < &end)
	fatal (FATALBUG, "Illegal free in static storage");
    for (ind = Z; ind < NUMADRS; ind++)
	if (n == memaddrs[ind]) {
	    memaddrs[ind] = 0;
	    free (n);
	    return;
	}
    fatal (FATALBUG, "Illegal free in dynamic storage");

    free (n);
}
#endif

/* append (name,ext) - allocs enough space to hold the catenation
/*       of the strings name and ext.
/*       Returns pointer to new string.
/**/
char *
append (name, ext)
char   *name,
       *ext;
{
    Short   lname;
    register char  *c,
                   *d,
                   *newname;

    for (lname = 0, c = name; *c++; ++lname);
    for (c = ext; *c++; ++lname);
    for (newname = c = salloc (lname + 1, YES), d = name; *d; *c++ = *d++);
    for (d = ext; *c++ = *d++;);

    return newname;
}

/* s2i(s,*i) - converts string s to int and returns value in i.
/*          Returns pointer to the first char encountered that is not part
/*          of a valid decimal number.
/*          If the returned pointer == s, then no number was converted.
/**/
char *
s2i (s, i)
char   *s;
int    *i;
{
    register Char   lc;
    register Char   c;
    register Short  val;
    Flag    maxi;
    Short   sign;

    maxi = NO;
    sign = 1;
    val = 0;
    lc = Z;
    for (c = *s; ; lc = c, c = *++s) {
	if (c >= '0' && c <= '9') {
	    if (maxi)
		continue;
	    if ((val = 10 * val + c - '0') < 0 && sign > 0)
		maxi = YES;
	    continue;
	}
	else if (c == '-' && lc == 0) {
	    sign = -1;
	    continue;
	}
	else if (lc == '-')
	    s--;
	break;
    }
    if (maxi)
	*i = MAXINT;
    else
	*i = val * sign;
    return s;
}


mv (name1, name2)
char name1[],
     name2[];
{
    int ok;

    unlink (name2);
    if (ok = (link (name1, name2) != -1))
	unlink (name1);
    return ok;
}

/* VARARGS1 */
fatalpr (fmt)
char *fmt;
{
    dbgpr  ("%r", &fmt);
    printf ("%r", &fmt);
    return;
}


/* VARARGS1 */
dbgpr (fmt)
char *fmt;
{
    if (dbgfile != NULL) {
	fprintf (dbgfile, "%r", &fmt);
	fflush (dbgfile);
    }
    return;
}

flushkeys ()
{
    fflush (keyfile);
    if (ferror (keyfile))
	fatal (FATALIO, "ERROR WRITING keys FILE.");
}

okwrite ()
{
    return fileflags[curfile] & CANMODIFY;
}

