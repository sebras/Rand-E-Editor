#ifdef COMMENT

    pres - print E state file

    See State.fmt for a description of the latest state file format.

    David Yost 3/79 and later

    pres [file]...

    An argument consisting of '-' by itself means read the original
      standard input, and is treated as a file argument, not an option.

    If no files are specified, use stdin as input.
    If files are specified, use them as input.

    Default is to check access to all files and report those which can't
      read, then to go ahead with those that can.  This behavior can
      be changed by setting the appropriate global flags below.
    Buffers output and input.

    Exits 0 if done OK.
    Exits -1 if error encountered and nothing touched.
    Exits -2 if error encountered after doing some output.

#endif
/******************* standard filter declarations ************************/
#include "e.h"
/*  #include <stdio.h>  is already included by ned.h above */
extern char _sobuf[];
FILE *tfopen ();

#define STATSIZE 100     /* max num of characters returned by stat call */
#define BELL 07
/* #define RAWMODE      /* define this if raw mode is needed               */
			/* NOT FIXED FOR UNIXV7 yet */
#ifdef UNIXV7
#include <sgtty.h>
#else
#include <sys/sgtty.h>
#endif
#ifdef RAWMODE
struct sgttyb instty;
#endif
int rawflg = 0;         /* input is set to raw mode                        */
int flushflg = 0;       /* means a user is sitting there waiting to see    */
			/*   the output, so give it to him as soon as it   */
			/* is ready                                        */

int numargs,            /* global copy of argc                             */
    curarg;             /* current arg to look at                          */
char **argarray,        /* global copy of argv                             */
     *curargchar,       /* current arg character to look at                */
     *progname;         /* global copy of argv[0] which is program name    */

int opterrflg = 0,      /* option error encountered                          */
    badfileflg = 0;     /* bad file encountered                            */

int opt_abort_flg = 1,  /* abort entirely if any option errors encountered */
    opt_stop_flg = 0;   /* stop processing at first option error           */
			/* do not turn this on if opt_abort_flg is on      */

int fil_abort_flg = 0,  /* abort entirely if any bad files encountered     */
    fil_stop_flg = 0;   /* stop processing at first bad file               */
			/* do not turn this on if fil_abort_flg is on      */

int show_errs_flg;      /* print error diagnostics to stderr               */
int output_done = 0;    /* set to one if any output done                   */

FILE *input;
/********************* end of standard filter declarations *****************/


/**/
main(argc, argv)
int argc;
char **argv;
{
    numargs = argc;
    curarg = 0;
    argarray = argv;
    curargchar = argarray[curarg];

    input = stdin;
    getprogname();

    opterrflg = badfileflg = 0;
    show_errs_flg = 0;

    do
    {   getoptions(1);          /* check for option errors      */
	filterfiles(1);         /* check for bad files          */
    } while (curarg < numargs);
    if ( (opterrflg && !opt_stop_flg) || (badfileflg && !fil_stop_flg) )
    {   curarg = 1;
	opterrflg = badfileflg = 0;
	show_errs_flg = 1;
	do
	{   getoptions(1);          /* check for option errors      */
	    filterfiles(1);         /* check for bad files          */
	} while (curarg < numargs);
	if ( (opterrflg && opt_abort_flg) || (badfileflg && fil_abort_flg) )
	{   fprintf(stderr,"%s: not performed.\n",progname);
	    getout (-2);
	}
    }

    curarg = 1;
    opterrflg = badfileflg = 0;

    if (!intss())
	setbuf(stdout, _sobuf);
    do
    {   opterrflg = 0;
	show_errs_flg = opt_stop_flg;
	getoptions(0);
	if (opterrflg && opt_stop_flg)
	    stop ();
	badfileflg = 0;
	show_errs_flg = fil_stop_flg;
	filterfiles(0);
	if (badfileflg && fil_stop_flg)
	    stop ();
    } while (curarg < numargs);
    getout (0);
}


getprogname()
{   register char *cp;
    register char lastc = '\0';

    progname = cp = argarray[0];
    for (; *cp; cp++)
    {   if (lastc == '/')
	    progname = cp;
	lastc = *cp;
    }
    curarg++;

    /******************************************************************/
    /**/
    /**/    /* determine what to do depending on prog name here */
    /**/
    /******************************************************************/
}


getoptions(check_only)
{   register char *p, *c;

    for (; curarg<numargs; curarg++)
    {   curargchar = argarray[curarg];
	if (curargchar[0] != '-')           /* not an option arg */
	    return;
	if (curargchar[1] == '\0')          /* arg was '-' by itself */
	    return;

	p = ++curargchar;
	for (;; p++,curargchar++)
	{   switch (*p)
	    {
	    case 'x':
		if (!check_only)
		{   /******************************************************/
		    /**/
		    /**/    /* process single char option, such as "-x" */
		    /**/
		    /******************************************************/
		}
		continue;

	    case 'l':
		/******************************************************/
		/**/
		/**/    /* process option with modifier, such as "-l65" */
		/**/    /*   checking for syntax errors               */
		/**/
		/******************************************************/
		if (!check_only)
		/******************************************************/
		/**/
		/**/    /* process option with modifier, such as "-l65" */
		/**/    /*   and actually set the appropriate         */
		/**/    /*   global variables                         */
		/**/
		/******************************************************/
		break;

	    case '\0':                    /* done */
		break;

	    default:                      /* unknown option or other errors */
	    error:
		opterrflg = 1;
		if (show_errs_flg)
		{   fprintf(stderr,"%s: option error: -%s\n",
			progname,curargchar);
		    fflush(stderr);
		}
	    }
	    break;
	}
    }
}

filterfiles(check_only)
{   register FILE *f;

    if (curarg >= numargs && !check_only)
    {   input = stdin;
	filter();
	return;
    }

    for (; curarg<numargs; curarg++)
    {   curargchar = argarray[curarg];
	if (curargchar[0] == '-')
	{   if (curargchar[1] == '\0')    /* "-" arg */
		f = stdin;
	    else
		return;
	}
	else
	{   if (check_only)
		f = tfopen(curargchar,"r");
	    else
		f = fopen(curargchar,"r");
	    if (f == NULL)
	    {   char scratch[STATSIZE];

		if (stat(curargchar,scratch) == -1)
		{   if (show_errs_flg)
		    {   fprintf(stderr,"%s: can't find %s\n",
			    progname,curargchar);
			fflush(stderr);
		    }
		}
		else
		{   if (show_errs_flg)
		    {   fprintf(stderr,"%s: not allowed to read %s\n",
			    progname,curargchar);
			fflush(stderr);
		    }
		}
		badfileflg = 1;
	    }
	}
	if (!check_only && f != NULL)
	{   input = f;
	    filter();
	}
    }
}

filter ()
{
    if ( input == stdin && intss() )
	fprintf(stderr,"%c%s: start typing.\n",BELL,progname);

    if (xintss ())
	setraw ();
    doit();
    fixtty ();

    if (input != stdin)
	fclose(input);
    else
	rewind(stdin);
    fflush(stdout);
}


doit()
{   int nportlist,
	portnum,
	n,
	revision,
	nletters,
	lmarg,
	tmarg,
	rmarg,
	bmarg,
	winlin,
	wincol,
	lin,
	col;
    Char chr,
	 majdev,
	 mindev;
    long tmpl;

    printf ("Revision %d\n", revision = - getshort (input));
    output_done = 1;
    if (revision <= 0)
	goto badstart;

    switch (revision)
    {
#define BIGTAB 32767
#ifndef UNIXV7
    case 4:
    case 5:
    case 6:
	nportlist = getshort (input);
	if (ferror(input) || nportlist > MAXPORTLIST)
	    goto badstart;
	printf ("Number of ports: %d\n", nportlist);
	output_done = 1;
	portnum = getshort (input);
	printf ("Current port: %d\n", portnum);

	fputs ("Tabstops: ", stdout);
	for (;;)
	{   n = getshort (input);
	    if (feoferr (input))
		goto badstart;
	    if (n == BIGTAB)
		break;
	    printf ("%d, ", n);
	}
	putchar ('\n');

	for (n = 0; n < nportlist; n++)
	{   fputs ("=================\n", stdout);
	    printf ("Port %d:\n", n);
	    printf ("  Previous port: %d\n", getshort (input));
	    lmarg = getshort (input);
	    tmarg = getshort (input);
	    rmarg = getshort (input);
	    bmarg = getshort (input);
	    printf ("  (%d, %d, %d, %d) = (l, t, r, b) port margins\n",
		lmarg, tmarg, rmarg, bmarg);
	    if (nletters = getshort (input))
	    {   if (feoferr (input))
		    goto badstart;
		fputs ("  Alternate file: ", stdout);
		while (chr = getc (input))
			putchar (chr);
		putchar ('\n');
		wincol = getshort (input);
		winlin = getshort (input);
		printf ("    (%d, %d) = (col, lin) window upper left\n",
		    wincol, winlin);
		printf ("    (%d, ", getshort (input));
		printf ("%d) = (col, lin) cursor position\n", getshort (input));
	    }
	    else
		fputs ("  No alt wksp\n", stdout);
	    if (feoferr (input))
		goto badstart;

	    fputs ("  File: ", stdout);
	    nletters = getshort (input);
	    while (chr = getc (input))
		    putchar (chr);
	    putchar ('\n');
	    wincol = getshort (input);
	    winlin = getshort (input);
	    printf ("    (%d, %d) = (col, lin) window upper left\n",
		wincol, winlin);
	    printf ("    (%d, ", getshort (input));
	    printf ("%d) = (col, lin) cursor position\n", getshort (input));
	}
	break;
#endif
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
	if (revision >= 10)
	    printf ("Terminal type: %d\n", getshort (input));
	majdev = getc (input);
	mindev = getc (input);
	printf ("Working Directory device: %d, %d; inode: %d\n",
	    majdev, mindev, getshort (input));

	tmpl = getlong (input);
	printf ("Time of start of session: %s", ctime (&tmpl) );

	fputs ("Tabstops: ", stdout);
	if (revision < 9)
	{   for (;;)
	    {   n = getshort (input);
		if (feoferr (input))
		    goto badstart;
		if (n == BIGTAB)
		    break;
		printf ("%d, ", n);
	    }
	    putchar ('\n');
	}
	else
	{   n = getshort (input);
	    if (n > 0)
		do
		{   printf ("%d, ", getshort (input));
		    if (feoferr (input))
			goto badstart;
		} while (--n);
	    else
		fputs ("none.", stdout);
	    putchar ('\n');
	}

	printf ("Width for fill, etc. = %d\n", getshort (input));

	if (nletters = getshort (input))
	{   printf ("Search string is \"");
	    while (--nletters)
		putchar (getc (input));
	    if (getc (input))
		goto badstart;
	    printf ("\"\n");
	}
	else
	    printf ("No search string.\n");

	printf ("INSERT mode ");
	if (getc (input))
	    printf ("on\n");
	else
	    printf ("off\n");

	if (getc (input))
	{   printf ("MARK in effect:\n");
	    winlin = getshort (input);
	    wincol = getshort (input);
	    lin    = getc     (input);
	    col    = getshort (input);
	    printf ("  window at (%d, %d); cursor at (%d, %d)\n",
		     winlin, wincol, lin, col);
	}
	else
	    printf ("MARK not in effect\n");

	nportlist = getc (input);
	if (ferror(input) || nportlist > MAXPORTLIST)
	    goto badstart;
	printf ("Number of ports: %d\n", nportlist);
	portnum = getc (input);
	printf ("Current port: %d\n", portnum);

	for (n = 0; n < nportlist; n++)
	{   fputs ("=================\n", stdout);
	    printf ("Port %d:\n", n);
	    printf ("  Previous port: %d\n", getc (input));
	    tmarg = getc     (input);
	    lmarg = getshort (input);
	    bmarg = getc     (input);
	    rmarg = getshort (input);
	    printf ("  (%d, %d, %d, %d) = (t, l, b, r) port margins\n",
		tmarg, lmarg, bmarg, rmarg);
	    if (nletters = getshort (input))
	    {   if (feoferr (input))
		    goto badstart;
		fputs ("  Alternate file: ", stdout);
		while (chr = getc (input))
			putchar (chr);
		putchar ('\n');
		winlin = getshort (input);
		wincol = getshort (input);
		printf ("    (%d, %d) = (lin, col) window upper left\n",
		    winlin, wincol);
		printf ("    (%d, ", getc     (input));
		printf ("%d) = (lin, col) cursor position\n", getshort (input));
	    }
	    else
		fputs ("  No alt wksp\n", stdout);
	    if (feoferr (input))
		goto badstart;

	    fputs ("  File: ", stdout);
	    nletters = getshort (input);
	    while (chr = getc (input))
		    putchar (chr);
	    putchar ('\n');
	    winlin = getshort (input);
	    wincol = getshort (input);
	    printf ("    (%d, %d) = (lin, col) window upper left\n",
		winlin, wincol);
	    printf ("    (%d, ", getc     (input));
	    printf ("%d) = (lin, col) cursor position\n", getshort (input));
	}
	break;

    default:
	fputs ("Don't know how to interpret that version.\n", stdout);
	return;
    }
    if (ferror (input))
	fputs ("\nBad startup file.  Read error\n", stdout);
    else if (feof (input))
	fputs ("\nBad startup file.  Premature EOF\n", stdout);
    return;

badstart:
    if (ferror (input))
	fputs ("\nBad startup file.  Read error\n", stdout);
    else if (feof (input))
	fputs ("\nBad startup file.  Premature EOF\n", stdout);
    else
	fputs ("\nBad startup file.\n", stdout);
}


setraw ()
{   register regi;

#ifdef RAWMODE
    if (gtty (INSTREAM, &instty) != -1)
    {   regi = instty.sg_flags;
	instty.sg_flags = RAW | (instty.sg_flags & ~(ECHO | CRMOD));
	stty (INSTREAM, &instty);        /* set tty raw mode */
	instty.sg_flags = regi;             /* all set up for cleanup */
	rawflg = 1;
    }
#endif
    flushflg = 1;
}

stop ()
{
    fprintf(stderr,"%s: stopped.\n",progname);
    getout(-1 - output_done);
}

getout (status)
{
    fixtty ();
    exit (status);
}

fixtty ()
{
#ifdef RAWMODE
    if (rawflg)
	stty (0, &instty);
#endif
    flushflg = 0;
}

xintss()
{   struct sgttyb buf;

    if (gtty (fileno (input), &buf) != -1)
	return 1;
    return gtty (fileno (stdin), &buf) != -1;
}

