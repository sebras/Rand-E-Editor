/*
/* file e.c - main program and startup stuff
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.tt.h"
#include "e.fn.h"
#include "e.sg.h"
#include <sys/stat.h>
#ifdef UNIXV7
#include <signal.h>
char *ttyname ();
#endif
#ifdef UNIXV6
#include <sys/signals.h>
#endif

#define OPTREPLAY    0
#define OPTRECOVER   1
#define OPTDEBUG     2
#define OPTSILENT    3
#define OPTHELP      4
#define OPTNOTRACKS  5
#define OPTINPLACE   6
#define OPTNORECOVER 7
#define OPTTERMINAL  8
#define OPTKEYBOARD     9
#define OPTBULLETS     10
#define OPTNOBULLETS   11

/* entries of which there are two in this table must be spelled out. */
S_looktbl opttable[] = {
    "bullets"  , OPTBULLETS  ,
    "debug"    , OPTDEBUG    ,
    "help"     , OPTHELP     ,
    "help"     , OPTHELP     ,
    "inplace"  , OPTINPLACE  ,
    "keyboard" , OPTKEYBOARD ,
    "nobullets", OPTNOBULLETS  ,
    "norecover", OPTNORECOVER,
    "notracks" , OPTNOTRACKS ,
    "replay"   , OPTREPLAY   ,
    "silent"   , OPTSILENT   ,
    "terminal" , OPTTERMINAL ,
    0          , 0
};

extern char _sobuf[];
extern char *signames[];
Small numargs;
Small curarg;
char *curargchar;
char **argarray;
char *dbgname;
Flag helpflg,
     dbgflg;
Flag crashed;
Flag chosereplay;
char *opttname;
char *optkname;
Flag optbullets = -1;   /* YES = -bullets, NO = -nobullets */

main (argc, argv)
int     argc;
char   *argv[];
{
    main1 (argc, argv);

    mainloop ();

    /* mainloop never returns */
}

main1 (argc, argv)
int     argc;
char   *argv[];
{
    short   tmp;        /* must be a short */
    char    ichar;      /* must be a char */
    Short   tmpi,
	    tmpj;
    char   *cp;
#ifdef UNIXV7
    extern char *getenv ();
#endif

    setbuf (stdout, _sobuf);
    fclose (stderr);

    numargs = argc;
    argarray = argv;
    getprogname (argv[0]);

    checkargs ();

    startup ();

    if (!chosereplay)
	checkargs ();

    if (helpflg) {
	fixtty ();
	printf ("\n\
syntax: %s [options]... [files]...\n\
options are:\n", progname);
	printf ("\
%c -bullets\n", optbullets == YES ? '*' : ' ');
	if (dbgflg)
	    printf ("\
* -debug=%s\n", dbgname);
	printf ("\
* -help\n");
	printf ("\
%c -inplace\n", inplace ? '*' : ' ');
	printf ("\
%c -keyboard=%s (keyboard type)\n", optkname ? '*' : ' ',
		    kname ? kname : tname);
	printf ("\
%c -nobullets\n", optbullets == NO ? '*' : ' ');
	printf ("\
%c -norecover\n", norecover ? '*' : ' ');
	printf ("\
%c -notracks\n", notracks ? '*' : ' ');
	printf ("\
%c -replay=%s\n", replaying ? '*' : ' ', replaying ? inpfname : "filename");
	printf ("\
%c -silent\n", silent ? '*' : ' ');
	printf ("\
%c -terminal=%s (terminal type)\n", opttname ? '*' : ' ', tname);
	printf ("\
\"*\" means option was in effect.");
	getout (YES, "");
    }

    if (dbgflg && (dbgfile = fopen (dbgname, "w")) == NULL)
	getout (YES, "Can't open debug file: %s", dbgname);

    if (replaying) {
	short replterm;
	struct stat statbuf;
	if ((inputfile = open (inpfname, 0)) < 0)
	    getout (YES, "Can't open replay file %s.", inpfname);
	if (read (inputfile, (char *) &tmp, 2) == 0)
	    getout (YES, "Replay file is empty.");
	if (tmp != revision)
	    getout (YES, "Replay file \"%s\" was made by revision %d of %s.",
		 inpfname, -tmp, progname);
	if (   read (inputfile, (char *) &ichar, 1) == 0
	    || read (inputfile, (char *) &replterm, sizeof (short)) == 0
	   )
	    getout (YES, "Replay file is too short.");
	if (replterm != termtype)
	    getout (YES, "\
Replay file \"%s\" was made by a different type of terminal.",
		 inpfname, -tmp, progname);
	if (fstat (inputfile, &statbuf) >= 0) {
#ifdef UNIXV7
		long lseek ();
		nreplaykeys = statbuf.st_size - lseek (inputfile, 0L, 1);
#endif
#ifdef UNIXV6
		long tell ();
		nreplaykeys = (long) statbuf.st_size0 << 16;
		nreplaykeys += (long) (unsigned) statbuf.st_size1;
		nreplaykeys -= tell (inputfile);
#endif
	}
    }
    else if (curarg < argc)
	ichar = ' ';        /* file args follow */
    else
	ichar = '!';        /* put up all old windows and files */

    if (recovering)
	printf ("\r\n\r\rRecovering from crash...");
    fflush (stdout);

    makescrfile (); /* must come before any calls to editfile () */

    if (!silent) {
	(*term.tt_home) ();
	(*term.tt_clear) ();
    }

    getstate (ichar);
    putshort (revision, keyfile);
    putc (ichar, keyfile);          /* for next time */
    putshort (termtype, keyfile);
    mesg (TELALL);

    if (!replaying && curarg < argc && *argv[curarg] != '\0') {
#ifdef UNIXV7
	if (   (cp = getenv ("editalt"))
	    || (   curarg + 1 < argc
#else
	if (   (   curarg + 1 < argc
#endif
		&& *(cp = argv[curarg + 1]) != '\0'
	       )
	   ) {
	    editfile (cp, 0, 0, 0, NO);
	    keyedit (cp);
	    /* do this so that editfile won't be cute about suppressing
	    /* a putup on a file that is the same as the altfile
	    /**/
	    curfile = NULLFILE;
	}
	if (editfile (argv[curarg], 0, 0, 1, YES) <= 0)
	    eddeffile (YES);
	keyedit (cp);
    }
    else if (!replaying || ichar != ' ') {
	putupwin ();
	poscursor (curwksp->ccol, curwksp->clin);
    }
}

static
keyedit (file)
char *file;
{
    Reg1 char *cp;

    cp = append ("edit ", file);
    writekeys (CCCMD, cp, CCRETURN);
    sfree (cp);
}

getprogname (arg0)
char arg0[];
{
    register char *cp;
    register Char lastc;

    lastc = Z;
    progname = cp = arg0;
    for (; *cp; cp++) {
	if (lastc == '/')
	    progname = cp;
	lastc = *cp;
    }
    cp = progname;

    if (loginflg = *progname == '-')
	progname = LOGINNAME;
}

checkargs ()
{
    Short   tmp;
    char   *cp,
	   *opterr;
    Flag opteqflg;

    dbgflg = 0;
    for (curarg = 1; curarg < numargs; curarg++) {
	curargchar = argarray[curarg];
	if (*curargchar != '-')
	    break;
	curargchar++;
	opteqflg = NO;
	for (cp = curargchar; *cp; cp++)
	    if (*cp == '=') {
		opteqflg = YES;
		*cp = 0;
		break;
	    }
	if (cp == curargchar)
	    goto unrecog;
	tmp = lookup (curargchar, opttable);
	if (opteqflg)
	    *cp++ = '=';
	if (tmp == -1)
	    goto unrecog;
	if (tmp == -2) {
	    opterr = "ambiguous";
	    goto error;
	}
	switch (opttable[tmp].val) {
	case OPTBULLETS:
	    optbullets = YES;
	    break;

	case OPTNOBULLETS:
	    optbullets = NO;
	    break;

	case OPTHELP:
	    helpflg = 1;
	    break;

	case OPTDEBUG:
	    /* file for debug info */
	    if (!opteqflg || *cp == 0)
		getout (YES, "Must give debug file name");
	    if (dbgflg) {
		opterr = "repeated";
		goto error;
	    }
	    dbgflg = YES;
	    dbgname = cp;
	    break;

	case OPTINPLACE:
	    inplace = YES;
	    break;

	case OPTSILENT:
	    silent = 1;
	    break;

	case OPTNORECOVER:
	    norecover = 1;
	    break;

	case OPTNOTRACKS:
	    notracks = 1;
	    break;

	case OPTREPLAY:
	    replaying = YES;
	    if (opteqflg && *cp)
		/* this is only useful on the second call to checkargs */
		inpfname = cp;
	    break;

	case OPTKEYBOARD:
	    if (opteqflg && *cp)
		optkname = cp;
	    break;

	case OPTTERMINAL:
	    if (opteqflg && *cp)
		opttname = cp;
	    break;

	default:
	unrecog:
	    opterr = "not recognized";
	error:
	    getout (YES, "%s option %s", argarray[curarg], opterr);
	}
    }
}

/*  startup() - the initializing routine
/**/
startup ()
{
    struct stat tbuf;
    Short indv;
    register Short  i;
    register char  *c,
		   *name;

    for (i = 1; i <= NSIG; ++i)
	switch (i) {
	case SIGINT:    /* until setitty can take effect */
	case SIGQUIT:   /* until setitty can take effect */
#ifdef SIGCHLD  /* 4bsd vmunix */
	case SIGCHLD:
#endif
#ifdef SIGTSTP  /* 4bsd vmunix */
	    case SIGTSTP:
	    case SIGCONT:
#endif
	/*      (void) signal (i, SIG_IGN);    /* ignored by default */
	    break;

	default:
	    signal (i, sig);
	}

    /* stty on input to RAW and ~ECHO ASAP to allow typeahead */
    setitty ();

    /*  since we are now in raw mode, the following 2 calls to signal will
    /*  not affect us, but if we leave them set to SIG_IGN, then the 'who'
    /*  command won't treat us like we're normal.
    /**/
    signal (SIGINT, sig);
    signal (SIGQUIT, sig);

    Block {
	extern time_t time ();
	time (&strttime);
    }

    gettermtype ();

    /* stty on output for ~CRMOD and ~XTABS */
    setotty ();

    /* initialize terminal.  must be done before "starting..." message */
    /* so keys are initialized. */
    if (!silent) {
	d_put (VCCICL);
	windowsup = YES;
    }
    if (loginflg)
	printf ("%s is starting...", progname);
    else
	printf ("%s rev %d.%d is starting...", progname, -revision, subrev);

#ifdef UNIXV7
    userid = getuid ();
    groupid = getgid ();
#endif
#ifdef UNIXV6
    userid  = getuid () & 0377;
    groupid = getgid () & 0377;
#endif

    blanks = salloc (term.tt_width, YES);
    fill (blanks, (unsigned int) term.tt_width, ' ');

    for (i = Z; i < NUMFFBUFS; i++)
	ff_use (ffblock[i], 0);


    cline = salloc ((lcline = icline) + 1, YES);
				  /* start with size of first increment */
#define PRIV (S_IREAD | S_IWRITE | S_IEXEC)
    if (   stat (".", &tbuf) != -1
	&& (checkpriv (&tbuf) & PRIV) == (S_IREAD | S_IWRITE | S_IEXEC)
	&& (   userid != 0
	    || tbuf.st_uid == 0
	   )
       ) {
	tmppath = ".e"; /* use current directory for e files */
			/* and make them invisible */
	if (tbuf.st_uid != userid)
	    goto appendname;
	name = append ("", "");     /* so that name is alloced */
    }
    else {
	/* maybe we should make tmppath "logindir/etmp/" here */
appendname:
	getmypath ();   /* also gets myname */
	name = append (".", myname); /* we will append user name */
    }

    c = append (tmpnstr, name);
    tmpname = append (tmppath, c);
    sfree (c);
    c = append (rstr, name);
    rfile = append (tmppath, c);
    sfree (c);
    indv = strlen (tmppath) + VRSCHAR;
    for (evrsn = '1'; ; evrsn++) {
	if (evrsn > '9')
	    getout (NO, "\n%s: No free temp file names left\n", progname);
	tmpname[indv] = evrsn;
	rfile[indv] = evrsn;
	if ((i = open (tmpname, 04)) >= 0) {
	    /* exclusive => exists and no one has it exclusively open */
	    close (i);
	    dorecov (0);
	    crashed = YES;
	    break;
	}
	/* if we get here, it's because
	/*  (tmpname exists and someone is using it) or (it doesn't exist)
	/**/
	if (stat (tmpname, &tbuf) >= 0)
	    /* exists and someone is using it */
	    continue;
	if (notracks && stat (rfile, &tbuf) >= 0)
	    continue;
	break;
    }

    if ((i = creat (tmpname, 0600)) == -1)
	getout (YES, "Can't open temporary file (%s) to hold changes.",
		tmpname);
    close (i);			  /* make it both read&write-able */
    {
    Ff_stream *ff;

    ff = ff_open (tmpname, 6, 0);     /* and use buffer pool          */
    ffchans[ff_fd (ff)] = ff;
    openffs[CHGFILE] = ff;
    names[CHGFILE] = tmpname;
    fileflags[CHGFILE] = INUSE | FWRITEABLE | DWRITEABLE | CANMODIFY;
    }

    fileflags[NULLFILE] = INUSE;

    /* make the rest of the file names */
    bkeystr[VRSCHAR] = keystr[VRSCHAR] = evrsn;
    c = append (keystr, name);
     keytmp  = append (tmppath, c);
     sfree (c);
    c = append (bkeystr, name);
     inpfname = bkeytmp = append (tmppath, c);
     sfree (c);
    sfree (name);

    if (!crashed && stat (keytmp, &tbuf) != -1)
	dorecov (1);
    mv (keytmp, bkeytmp);
    keysmoved = YES;
    if ((keyfile = fopen (keytmp, "w")) == NULL)
	getout (YES, "Can't create keystroke file.");
    chmod (keytmp, 0600);

 /* set up port for whole screen.  no margins, and it is not on portlist. */
    setupviewport (&wholescreen, 0, 0,
		   term.tt_width - 1, term.tt_height - 1, 0);

 /* set up port for parameter entry.  no margins and it is not on portlist  */
    setupviewport (&enterport, 0,
			       term.tt_height - NENTERLINES - NINFOLINES,
			       term.tt_width - 1,
			       term.tt_height - 1 - NINFOLINES, 0);
    enterport.redit = term.tt_width - 1;
 /* set up port for info display.  no margins and it is not on portlist  */
    setupviewport (&infoport, 0,
			      term.tt_height - NINFOLINES,
			      term.tt_width - 1,
			      term.tt_height - 1, 0);

    curport = &wholescreen;

    return;
}

dorecov (type)
int type;
{
	    if (norecover)
		return;
	    else {
		fixtty ();
		/* the following is broken up into multiple strings so as */
		/* not to overflow the C compiler's buffers */
		printf ("\n\
The last time you used the editor in this directory, ");
		if (type)
		    printf ("you aborted.\n");
		else
		    printf ("it crashed.\n");
		for (;;) {
		    char line[132];
		    printf ("\
You have these choices:\n");
		    printf ("\
  1. E will recover silently then update screen; then you should exit.\n");
		    if (type)
			printf ("\
     (This is normally what you should do.)\n");
		    printf ("\
  2. E will replay the last session on the screen; then you may exit\n");
		    printf ("\
     or continue editing.\n");
		    printf ("\
  3. E will ignore the ");
		    if (type)
			printf ("aborted");
		    else
			printf ("crashed");
		    printf ("\
 session and go ahead with a fresh session\n");
		    printf ("\
     as per the commmand you typed.\n");
		    printf ("\
  4. Abort now.\n");
		    printf ("\
Type the number of the option you want then hit <RETURN>: ");
		    fflush (stdout);
		    gets (line);
		    if (feof (stdin))
			getout (type, "");
		    switch (*line) {
		    case '1':
			recovering = YES;
			silent = YES;
			goto restore;
		    case '2':
			silent = NO;
		    restore:
			replaying = YES;
			helpflg = NO;
			notracks = NO;
			chosereplay = YES;
			break;
		    case '3':
			break;
		    case '4':
			getout (type, "");
		    default:
			continue;
		    }
		    break;
		}
		setitty ();
		setotty ();
	    }
	    return;
}

gettermtype ()
{
#ifdef ENVIRON
    extern char *getenv ();
#endif

    /* Get the selected terminal type */
    if (   !(tname = opttname)
#ifdef ENVIRON
	&& !(tname = getenv ("TERM"))
       )
	getout (YES, "No TERM environment variable or -terminal argument");
#else   ENVIRON
       )
	getout (YES, "No -terminal argument");
#endif  ENVIRON
    Block {
	Reg1 int ind;
	if ((ind = lookup (tname, termnames)) >= 0)
	    kbdtype = termnames[ind].val; /* assume same as term for now */
	else
	    kbdtype = 0;
	if (   ind >= 0 /* we have the name */
#ifdef  TERMCAP
	    && tterm[termnames[ind].val] != tterm[0] /* not a termcap type */
#endif  TERMCAP
	    && tterm[termnames[ind].val] /* we have compiled-in code */
	   )
	    termtype = termnames[ind].val;
#ifdef  TERMCAP
	else {
	    char *str;

	    switch (getcap (tname)) {
	    default:
		termtype = 0; /* termcap type is 0 */
		break;
	    case -1:
		str = "known";
		goto badterm;
	    case -2:
		str = "usable";
	    badterm:
		getout (YES, "Un%s terminal type: \"%s\"", str, tname);
	    }
	}
#else   TERMCAP
	else
	    getout (YES, "Unknown terminal type: \"%s\"", tname);
#endif  TERMCAP
    }

    /* Get the selected keyboard type */
    if (   (kname = optkname)
#ifdef ENVIRON
	|| (kname = getenv ("EKBD"))
#endif
       ) Block {
	Reg1 int ind;
	if (   (ind = lookup (kname, termnames)) >= 0
	    && tkbd[termnames[ind].val] /* we have compiled-in code for it */
	   )
	    kbdtype = termnames[ind].val;
	else
	    getout (YES, "Unknown keyboard type: \"%s\"", kname);
    }
    else if (kbdtype)
	kname = tname;
    else
	kname = "standard";

    /* select the routines for the terminal type and keyboard type */
    move ((char *) tterm[termtype], (char *) &term,
	(unsigned int) sizeof (S_term));
    move ((char *) tkbd[kbdtype],   (char *) &kbd,
	(unsigned int) sizeof (S_kbd));

    d_put (VCCINI);     /* initializes display image for d_write */
			/* and selects tt_height if selectable */

    /* initialize the keyboard */
    (*kbd.kb_init) ();

    Block {
	Reg1 int tmp;
	tmp = term.tt_naddr;
	tt_lt2 = 2 * term.tt_nleft  < tmp;
	tt_lt3 = 3 * term.tt_nleft  < tmp;
	tt_rt2 = 2 * term.tt_nright < tmp;
	tt_rt3 = 3 * term.tt_nright < tmp;
    }

    if (optbullets >= 0)
	borderbullets = optbullets;
    else if (borderbullets)
	borderbullets = term.tt_bullets;
    return;
}

setitty ()
{
    int i;

    if (   gtty (INSTREAM, &instty) == -1
#ifdef V6XSET
	|| gtty (INSTREAM | XSET, &inxstty) == -1
#endif
       )
	return;
    i = instty.sg_flags;
    instty.sg_flags = RAW | (instty.sg_flags & ~(ECHO | CRMOD));
    stty (INSTREAM, &instty);        /* set tty raw mode */
    instty.sg_flags = i;             /* all set up for cleanup */
#ifdef V6XSET
    i = inxstty.xs_xflags;
    inxstty.xs_xflags |= IN8BIT;
    stty (INSTREAM | XSET, &inxstty);        /* set tty 8-bit input */
    inxstty.xs_xflags = i;
#endif
    istyflg = YES;
}

setotty ()
{
    if (gtty (OUTSTREAM, &outstty) == -1)
	fast = YES;
    else {
	int i;
	struct stat tbuf;
	i = outstty.sg_flags;
	outstty.sg_flags &= ~(XTABS | CRMOD | ALLDELAY);
	stty (OUTSTREAM, &outstty);
	outstty.sg_flags = i;             /* all set up for cleanup */
	ostyflg = YES;

#ifdef MESG_NO
	fstat (OUTSTREAM, &tbuf);        /* save old tty message status and */
	oldttmode = tbuf.st_mode;
#ifdef UNIXV7
	if ((ttynstr = ttyname (OUTSTREAM)) != NULL)
#endif
#ifdef UNIXV6
	if ((ttynstr[strlen (ttynstr) - 1] = ttyn (OUTSTREAM)) != NULL)
#endif
	    chmod (ttynstr, 0600);        /* turn off messages */
#endif
	fast = (ospeed = outstty.sg_ospeed) >= B4800;
    }
    /* no border bullets if speed is slow */
    if (!term.tt_bullets || !fast)
	borderbullets = NO;
    return;
}

/* makescrfile () - make the pick/close file "#"
/**/
makescrfile ()
{
    /* setup file '#' -- used to save deletebuffer and pickbuffer */
    fileflags[PKCLFILE] = INUSE;
    names[PKCLFILE] = append ("#", ""); /* must be freeable */
    nlines[PKCLFILE] = 0;
    pickwksp = &lastlook[PKCLFILE];
    pickwksp->curfsd = openfsds[PKCLFILE] = blanklines (0);
    pickwksp->curlno  = 0;
    pickwksp->curflno = 0;
    pickwksp->wfile = PKCLFILE;
    pickwksp->wlin    = 0;
    pickwksp->wcol    = 0;
    pickwksp->ccol    = 0;
    pickwksp->clin    = 0;
    pickbuf = &pb;
    closebuf = &cb;
    erasebuf = &eb;
    return;
}

/*
/*  See State.fmt for a description of the state file format.
/**/

/* getstate (ichar) -- get  the state
/*      ichar == ' ' means use only port portnum, no files
/*      ichar == '+' means use old file, fg, port->tmarg);
/*      ichar == '!' means use all ports and all old files
/**/
getstate (ichar)
Char    ichar;
{
    Small   nletters;
    Scols   lmarg,
	    rmarg;
    Slines  tmarg,
	    bmarg;
    Nlines  lin;
    Ncols   col;
    Flag    gf;
    Small   portnum;
    register Short  i,
                    n;
    register char  *f1;
    char   *fname;
    FILE  *gbuf;
    S_window *port;
    Small ttype;

    if ( ichar == ' ' || notracks || (gbuf = fopen (rfile, "r")) == NULL) {
	makestate (ichar != ' ');
	return;
    }
    if ( (i = getshort (gbuf)) != revision) {
	if (i >= 0 || feoferr (gbuf))
	    goto badstart;
	if (i != -1)            /* always accept -1 */
	    getout (YES,
		    "Startup file: \"%s\" was made by revision %d of %s.\n\
Delete it or give a filename after the %s command.",
		    rfile, -i, progname, progname);
    }

    Block {
	Slines nlin;
	Scols ncol;
	nlin = getc (gbuf) & 0377;
	ncol = getc (gbuf) & 0377;
	if (   nlin != term.tt_height
	    || ncol != term.tt_width
	   )
	    getout (YES, "\
Startup file: \"%s\" was made for a terminal with a different screen size. \n\
(%d x %d).  Delete it or give a filename after the %s command.",
		    rfile, nlin, ncol, progname);
    }

    /* sttime     = */ getlong (gbuf);
    if (n = getshort (gbuf)) {
	ntabs = n;
	stabs = max (n, NTABS);
	tabs = (ANcols *) salloc (stabs * sizeof *tabs, YES);
	i = 0;
	do {
	    n = getshort (gbuf);
	    if (feoferr (gbuf))
		goto badstart;
	    tabs[i++] = n;
	} while (i < ntabs);
    }
    linewidth = getshort (gbuf);
    if (i = getshort (gbuf)) {
	f1 = searchkey = salloc (i, YES);
	do
	    *f1++ = getc (gbuf);
	while (--i);
    }
    imodesw = getc (gbuf);
    if (getc (gbuf)) {  /* curmark */
	i = sizeof (short)
	  + sizeof (short)
	  + sizeof (char)
	  + sizeof (short);
	do
	    getc (gbuf);
	while (--i);
    }
    nportlist = getc (gbuf);
    if (ferror(gbuf) || nportlist > MAXPORTLIST)
	goto badstart;
    portnum = getc (gbuf);
    if (ichar != '!') {
	/* skip over ports until we get to the right port */
	for (n = Z; n < portnum; n++) {
	    i = sizeof (char)
	      + sizeof (char)
	      + sizeof (short)
	      + sizeof (char)
	      + sizeof (short);
	    do
		getc (gbuf);
	    while (--i);
	    if (i = getshort (gbuf)) {
		do
		    getc (gbuf);
		while (--i);
		i = sizeof (short)
		  + sizeof (short)
		  + sizeof (char)
		  + sizeof (short);
		do
		    getc (gbuf);
		while (--i);
	    }
	    i = getshort (gbuf);
	    do
		getc (gbuf);
	    while (--i);
	    i = sizeof (short)
	      + sizeof (short)
	      + sizeof (char)
	      + sizeof (short);
	    do
		getc (gbuf);
	    while (--i);
	    if (feoferr (gbuf))
		goto badstart;
	}
	portnum = 0;
	nportlist = 1;
    }

    for (n = Z; n < nportlist; n++) {
	port = portlist[n] = (S_window *) salloc (SVIEWPORT, YES);
	port->prevport = getc (gbuf);
	if (ichar != '!')
	    port->prevport = 0;
	tmarg = getc (gbuf);
	lmarg = getshort (gbuf);
	bmarg = getc (gbuf);
	rmarg = getshort (gbuf);
	if (ichar != '!') {
	    tmarg = 0;
	    lmarg = 0;
	    bmarg = term.tt_height - 1 - NPARAMLINES;
	    rmarg = term.tt_width - 1;
	}
	setupviewport (port, lmarg, tmarg, rmarg, bmarg, 1);
	if (n != portnum)
	    drawborders (portlist[n], 1);
	switchport (port);
	gf = NO;
	if (nletters = getshort (gbuf)) {   /* there is an alternate file */
	    if (feoferr (gbuf))
		goto badstart;
	    f1 = fname = salloc (nletters, YES);
	    do
		*f1++ = getc (gbuf);
	    while (--nletters);
	    lin = getshort (gbuf);
	    col = getshort (gbuf);
	    {
	    Slines tmplin;
	    Scols tmpcol;

	    tmplin = getc (gbuf);
	    tmpcol = getshort (gbuf);
	    if (ichar == '!') {
		if (editfile (fname, col, lin, 0, NO) == 1)
		    gf = YES;
		/* this sets them up to get copied into curwksp->ccol & clin */
		poscursor (tmpcol, tmplin);
	    }
	    else
		poscursor (0, 0);
	    }
	    /* do this so that editfile won't be cute about suppressing a
	    /* putup on a file that is the same as the altfile
	    /**/
	    curfile = NULLFILE;
	}
	nletters = getshort (gbuf);
	if (feoferr (gbuf))
	    goto badstart;
	f1 = fname = salloc (nletters, YES);
	do
	    *f1++ = getc (gbuf);
	while (--nletters);
	lin = getshort (gbuf);
	col = getshort (gbuf);
	if (feoferr (gbuf))
	    goto badstart;
	if (ichar == ' ')
	    gf = YES;
	else {
	    if (n != portnum && nportlist > 1)
		chgborders = 2;
	    if (editfile (fname, col, lin, 0, (n == portnum ? NO : YES)) == 1)
		gf = YES;
	}
	curwksp->clin = getc (gbuf);
	curwksp->ccol = getshort (gbuf);
	if (gf == NO) {
	    if (n != portnum && nportlist > 1)
		chgborders = 2;
	    eddeffile (n == portnum ? NO : YES);
	    curwksp->ccol = curwksp->clin = 0;
	}
    }
    if ( feoferr (gbuf))
badstart:
	getout (YES, "Bad startup file: \"%s\"\nDelete it and try again.",
		 rfile);

    drawborders (portlist[portnum], 3);
    switchport (portlist[portnum]);
    fclose (gbuf);
    return;
}

/* makestate() -- routine to create an initial state
/*                      when there is no strt file
/**/
makestate (nofileflg)
Flag nofileflg;
{
    register S_window *port;

    stabs = NTABS;
    tabs = (ANcols *) salloc (stabs * sizeof *tabs,YES);
    ntabs = 0;
    tabevery (TABCOL, 0, ((NTABS / 2) - 1) * TABCOL, YES);

    nportlist = 1;
    port = portlist[0] = (S_window *) salloc (SVIEWPORT, YES);
    setupviewport (portlist[0], 0, 0,
		   term.tt_width - 1, term.tt_height - 1 - NPARAMLINES, 1);
    drawborders (port, 3);
    poscursor (port->lmarg, port->tmarg + 1);
    switchport (port);
    poscursor (0, 0);
    if (nofileflg)
	edscrfile (NO);
}

/* sig() - interrupt routine to trap errors */
#ifdef SIGARG
sig (num)
unsigned Small num;
{
    char errstr[100];

    if (num > NSIG)  /* hedge against old signal routine */
	num = 0;
    if (signames[num] == 0)
	sprintf (errstr, "Signal %d.", num);
    else
	sprintf (errstr, "%s %s", signames[num], "trap.");
    fatal (FATALSIG, errstr);
#else
sig ()
{
    fatal (FATALSIG, "Fatal Trap");
#endif
}

/* VARARGS2 */
getout (filclean, str)
Flag filclean;
char   *str;
{
    fixtty ();
    if (windowsup)
	clearscreen ();
    if (filclean && !crashed)
	cleanup (YES, NO);
    if (keysmoved)
	mv (bkeytmp, keytmp);
    d_put (0);
    if (*str)
	printf ("\n%r",&str);
    printf ("\nThis is %s revision %d.%d\n", progname, -revision, subrev);
    exit (-1);
}

edscrfile (puflg)
Flag puflg;
{
    if (editfile (scratch, -1, -1, 2, puflg) != 1)
	eddeffile (puflg);
}
