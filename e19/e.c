#ifdef COMMENT
--------
file e.c
    main program and startup code.
    All code in this file is used only prior to calling mainloop.
	(not any more, some service routines can be called during execution)

    This code assume that not initialised global and static variables
	are by default initialized to 0
#endif

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.e.h"
#ifdef  KBFILE
#include "e.it.h"
#endif  /* KBFILE */
#include "e.tt.h"
#include "e.wi.h"
#include "e.fn.h"
#include "e.sg.h"
#include "e.inf.h"
#include <sys/stat.h>
#include <string.h>
#ifdef SYSIII
#include <fcntl.h>
#endif /* SYSIII */
#include <unistd.h>

extern char *getenv ();

#include SIG_INCL
#ifdef TTYNAME
extern char *ttyname ();
#endif

#define DEFAULT_TABS 4      /* default tabs setting to 4 */

/*  After the two-byte revision number in the keystroke file
 *  comes a character telling what to use from the state file.
 *  As of this writing, only ALL_WINDOWS and NO_WINDOWS are usable.
 **/
#define NO_WINDOWS  ' ' /* don't use any files from state file */
#define ONE_FILE    '-' /* use only one file from current window */
#define ONE_WINDOW  '+' /* use current and alt file from current window */
#define ALL_WINDOWS '!' /* use all windows and files */

#define OPTREPLAY       0
#define OPTRECOVER      1
#define OPTDEBUG        2
#define OPTSILENT       3
#define OPTHELP         4
#define OPTNOTRACKS     5
#define OPTINPLACE      6
#define OPTNORECOVER    7
#define OPTTERMINAL     8
#define OPTKEYBOARD     9
#ifdef  KBFILE
#define OPTKBFILE      10
#endif
#define OPTBULLETS     11
#define OPTNOBULLETS   12
#ifdef TERMCAP
#define OPTDTERMCAP    13
#endif
#define OPTPATTERN     15
#ifdef LMCSTATE
#define OPTSTATE       16
#endif
#ifdef LMCOPT
#define OPTSTDKBD      17
#endif
#ifdef LMCSRMFIX
#define OPTSTICK       18
#define OPTNOSTICK     19   /* no pun intended */
#endif
#ifdef NOCMDCMD
#define OPTNOCMDCMD    20
#endif
/* XXXXXXXXXXXXXXXXX */
#define OPTDIRPACKG    21
#define OPTVHELP       22
#define OPTVERSION     23

/* Entries must be alphabetized. */
/* Entries of which there are two in this table must be spelled out. */
S_looktbl opttable[] = {
    "bullets"  , OPTBULLETS  ,
    "debug"    , OPTDEBUG    ,
#ifdef LMCOPT
    "dkeyboard", OPTSTDKBD   ,
#endif
#ifdef TERMCAP
    "dtermcap" , OPTDTERMCAP ,
#endif
    "help"     , OPTHELP     ,
    "inplace"  , OPTINPLACE  ,
#ifdef  KBFILE
    "kbfile"   , OPTKBFILE   ,
#endif
    "keyboard" , OPTKEYBOARD ,
    "nobullets", OPTNOBULLETS,
#ifdef NOCMDCMD
    "nocmdcmd" , OPTNOCMDCMD ,
#endif
    "norecover", OPTNORECOVER,
#ifdef LMCSRMFIX
    "nostick"  , OPTNOSTICK  ,
#endif
    "notracks" , OPTNOTRACKS ,
    /* XXXXXXXXXXXXXXXXXXXXXXXXXXXX */
    "package"  , OPTDIRPACKG ,
    "regexp"   , OPTPATTERN  ,  /* Added Purdue CS 2/8/83 */
    "replay"   , OPTREPLAY   ,
    "silent"   , OPTSILENT   ,
#ifdef LMCSTATE
    "state"    , OPTSTATE    ,
#endif
#ifdef LMCSRMFIX
    "stick"    , OPTSTICK    ,
#endif
    "terminal" , OPTTERMINAL ,
    "verbose"  , OPTVHELP    ,
    "version"  , OPTVERSION  ,
    0          , 0
};

#ifdef  NOCMDCMD
Flag optnocmd;          /* YES = -nocmdcmd; <cmd><cmd> functions disabled */
#endif

/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */

/* Assumed Rand editor directory tree structure :
   ==============================================
   The following structure is assumed in order to found automaticaly
	the various pathes to services files (help, keyboard definition ...)

/* The Rand pacakge directory is the directory which include :
	- the Rand Editor installed executable file (which cannot be called a.out)
	- the service programs (center, fill, run) executable files
	- the help files : errmsg, helpkey, recovermsg, Crashdoc
	- the kbfiles directory (directory of keyboard definition files)
    Special case : building a new version (compiling and linking)
	- the new executable file name must be : a.out
	- the help files and kbfiles directory must be at the relative
	    position defined by helpdir constant string : ../help
*/

static char *dirpackg = NULL;   /* Rand editor package directory */
static Flag buildflg = NO;      /* newly build executable (a.out) */

/* Rand package directory tree structure related names */
/* --------------------------------------------------- */
    /* default default package dir (used to extract the package sub dir) */
#ifdef __linux__
extern char def_xdir_dir [];    /* build by Nerversion */
#else
static char def_xdir_dir [] = XDIR_DIR;     /* defined in ../include/linux_localenv.h */
#endif
    /* system wide configuration directory */
static char syst_cnfg []  = "/etc";     /* computer configuration directory */
static char alt_local_cnfg []  = "/../etc";  /* local relative to pacakage */
static char local_cnfg [] = "/usr/local/etc";   /* cluster configuration dir */
    /* all-purpose keyboard definition file name */
static char universal_kbf_name [] = "universalkb";
    /* kbfiles directory relative to rand package and configurations directory */
static char kbfilesdir [] = "/kbfiles";
    /* help directory relative to the directory including a.out (just build editor) */
static char helpdir [] = "/../help";
static char buildexec [] = "a.out";
    /* postfix added to the keyboard name (or terminal name) to generate the kbfile name */
static char kbfile_postfix [] = "kb";

static char *full_prgname = NULL;   /* full Rand program name */
static char *prog_fname;        /* actual Rand editor file name */
static char *pwdname = NULL;    /* current working directory */

static char *optdirpackg = NULL;    /* option dir package */
static Flag optusedflg = NO;        /* some program options in use */

char default_workingdirectory [PATH_MAX] = "./";

#ifdef VBSTDIO
extern unsigned char _sobuf[];
#endif

Small numargs;
Small curarg;
char *curargchar;
char **argarray;
char *dbgname;      /* argument to -debug=xxx option */
char *replfname;    /* argument to -replay=xxx option */
#ifdef LMCSTATE
Flag state=NO;
char *statefile;    /* argument to -state=xxx option */
#endif

char stdk[] = "standard";   /* standard comipled keyboard name */
Flag stdkbdflg;     /* use the standard compiled keyboard definition */
static Flag Xterm_flg;  /* YES for xterm terminal afmilly */

Flag helpflg,
     verbose_helpflg,
     dbgflg;
Flag crashed;       /* prev session crashed */
Flag chosereplay;   /* user chose replay or recovery */
char *opttname;
char *optkname;     /* Keyboard name */

char *kbfcase;          /* who define kbfile */
char *kbfile = NULL;    /* name of input (keyboard) table file */
char *optkbfile;        /* Keyboard translation file name option */

Flag optbullets = -1;   /* YES = -bullets, NO = -nobullets */
#ifdef TERMCAP
Flag optdtermcap;       /* YES = force use of termcap */
#endif

/* ++XXXXXXXXXXXXXXXXXXXXXXXXX */
/* Rand package service & configuration files */

static char *packg_subdir = NULL;   /* extracted form program path or def_xdir_dir */

struct cnfg_dir_rec {
	char *path;         /* directory path name */
	Flag existing;      /* existing and read access */
    };

static struct cnfg_dir_rec user_cnfg_dir =      { NULL, NO }; /* $HOME/.Rand/kbfiles */
static struct cnfg_dir_rec local_cnfg_dir =     { NULL, NO }; /* /usr/local/etc/Rand/kbfiles */
static struct cnfg_dir_rec alt_local_cnfg_dir = { NULL, NO }; /* <package>/../etc/Rand/kbfiles */
static struct cnfg_dir_rec syst_cnfg_dir =      { NULL, NO }; /* /etc/Rand/kbfiles */
static struct cnfg_dir_rec def_cnfg_dir  =      { NULL, NO }; /* in the package directory <pakage> */

static struct cnfg_dir_rec *cnfg_dir_rec_pt [] = {        /* in order of priority */
	    &user_cnfg_dir,         /* user config */
	    &syst_cnfg_dir,         /* computer config */
	    &alt_local_cnfg_dir,    /* cluster config */
	    &local_cnfg_dir,        /* default cluster config */
	    &def_cnfg_dir           /* default package config */
	};
#define cnfg_dir_rec_pt_sz (sizeof (cnfg_dir_rec_pt) / sizeof (cnfg_dir_rec_pt[0]))

static char no_exist_msg [] = " (not existing or no read access)";

static int max_cnfg_dir_sz = 0;     /* max size of config dir path name */

char * xdir_dir = NULL;     /* package directory */

char * recovermsg;
char * xdir_kr;
char * xdir_help;
char * xdir_crash;
char * xdir_err;
char * xdir_run;
char * xdir_fill;
char * xdir_just;
char * xdir_center;
char * xdir_print;

char * filterpaths[4];
/*
    xdir_fill ,
    xdir_just ,
    xdir_center ,
    xdir_print
*/
/* --XXXXXXXXXXXXXXXXXXXXXXXXX */

extern void main1 ();
       void getprogname ();
extern void checkargs ();
       void startup ();
       void showhelp ();
extern void dorecov ();
       void gettermtype ();
       void setitty ();
       void setotty ();
extern void makescrfile ();
extern void getstate ();
extern void getskip ();
extern void makestate ();
extern void infoinit ();
#ifdef LMCAUTO
extern void infoint0 ();
#endif
#ifdef  KBFILE
extern Flag getkbfile ();
#endif

void getout ();

/* XXXXXXXXXXXXXXXXXXXXXXX */
static void keyedit ();


#ifdef COMMENT
void
main1 (argc, argv)
    int     argc;
    char   *argv[];
.
    All of the code prior to entering the main loop of the editor is
    either in or called by main1.
#endif
void
main1 (argc, argv)
Reg3 int argc;
Reg2 char *argv[];
{
    char    ichar;      /* must be a char and can't be a register */

#ifdef VBSTDIO
    setbuf (stdout, (char *) _sobuf);
#endif
    fclose (stderr);

    numargs = argc;
    argarray = argv;
    getprogname (argv[0]);
    (void) getcwd (default_workingdirectory, sizeof (default_workingdirectory));

    checkargs ();

    if (dbgflg && (dbgfile = fopen (dbgname, "w")) == NULL)
	getout (YES, "Can't open debug file: %s", dbgname);

#ifdef LMCAUTO
    infoint0 ();
#endif
    startup ();

    if (helpflg) {
	static void clean_all ();
	static void exit_now ();
	showhelp ();
	helpflg = NO;
	clean_all (NO);
	exit_now (-1);
    }

    if (replaying) Block {
	short tmp;      /* must be a short, can't be a register */
	short replterm; /* must be a short, can't be a register */
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
Replay file \"%s\" was made by a different type of terminal.", inpfname);
    }
    else if (curarg < argc)
	ichar = NO_WINDOWS;         /* file args follow */
    else
	ichar = ALL_WINDOWS;        /* put up all old windows and files */

    if (recovering)
	printf ("\r\n\r\rRecovering from crash...");
    fflush (stdout);

    makescrfile (); /* must come before any calls to editfile () */

    if (!silent) {
	(*term.tt_home) ();
	(*term.tt_clear) ();
    }
#ifdef LMCVBELL
    if (getenv ("VBELL"))
	if (*term.tt_vbell != NULL) {
	    NoBell = NO;
	    VBell = YES;
	}
#endif
    getstate (ichar);
    putshort (revision, keyfile);
    putc (ichar, keyfile);          /* for next time */
    putshort ((short) termtype, keyfile);

    infoinit ();

    if (!replaying && curarg < argc && *argv[curarg] != '\0') Block {
	extern void keyedit ();
	Block {
	    Reg1 char *cp;
	    extern char *getenv ();
	    if (   (cp = getenv ("editalt"))
		|| (   curarg + 1 < argc
		    && *(cp = argv[curarg + 1]) != '\0'
		   )
	       ) {
		editfile (cp, (Ncols) 0, 0, 0, NO);
		keyedit (cp);
		/* do this so that editfile won't be cute about suppressing
		 * a putup on a file that is the same as the altfile
		 **/
		curfile = NULLFILE;
	    }
	}
	if (editfile (argv[curarg], (Ncols) 0, 0, 1, YES) <= 0)
	    eddeffile (YES);
	keyedit (argv[curarg]);
    }
    else if (!replaying || ichar != NO_WINDOWS)
	putupwin ();
    return;
}

#ifdef COMMENT
static void
keyedit (file)
    char   *file;
.
    Write out an edit command to the keys file.
#endif
static void
keyedit (file)
char *file;
{
    Reg1 char *cp;

    cp = append ("edit ", file);
    writekeys (CCCMD, cp, CCRETURN);
    sfree (cp);
}

#ifdef COMMENT
void
getprogname (arg0)
    char arg0[];
.
    Set the global character pointer 'progname' to point to the actual name
    that the editor was invoked with.
#endif
void
getprogname (arg0)
char arg0[];
{
    Reg1 char *cp;
    Reg2 Char lastc;

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
    return;
}

#ifdef COMMENT
void
checkargs ()
.
    Scan the arguments to the editor.  Set flags and options and check for
    errors in the option arguments.
#endif
void
checkargs ()
{
    optusedflg = dbgflg = NO;
    for (curarg = 1; curarg < numargs; curarg++) Block {
	char *cp;
	Flag opteqflg;
	Short tmp;
	char *opterr;

	curargchar = argarray[curarg];
	if (*curargchar != '-')
	    break;
	curargchar++;
	if (*curargchar == '-') curargchar++;   /* allow --help ... */
	optusedflg = YES;
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
	    helpflg = YES;
	    break;

	case OPTVHELP:  /* verbose help (for debugging) */
	    verbose_helpflg = YES;
	    break;

#ifdef LMCSRMFIX
	case OPTSTICK:
	    optstick = YES;
	    break;

	case OPTNOSTICK:
	    optstick = NO;
	    break;
#endif

#ifdef TERMCAP
	case OPTDTERMCAP:
	    optdtermcap = YES;
	    break;
#endif

#ifdef LMCSTATE
	case OPTSTATE:
	    if (opteqflg && *cp) {
		statefile = cp;
		state = YES;
	    }
	    break;
#endif

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
	    silent = YES;
	    break;

	case OPTNORECOVER:
	    norecover = YES;
	    break;

	case OPTNOTRACKS:
	    notracks = YES;
	    break;

	case OPTREPLAY:
	    replaying = YES;
	    if (opteqflg && *cp)
		replfname = cp;
	    break;

	case OPTKEYBOARD:
	    if (opteqflg && *cp)
		optkname = cp;
	    break;

#ifdef  KBFILE
	case OPTKBFILE:
	    if (opteqflg && *cp)
		optkbfile = cp;
	    break;
#endif

	case OPTTERMINAL:
	    if (opteqflg && *cp)
		opttname = cp;
	    break;

	case OPTPATTERN:
	    patmode = YES;
	    break;

#ifdef LMCOPT
	case OPTSTDKBD:
	    stdkbdflg = YES;
	    break;
#endif

#ifdef  NOCMDCMD
	case OPTNOCMDCMD:
	    optnocmd = YES;
	    break;
#endif
	case OPTDIRPACKG:
	    if (opteqflg && *cp)
		optdirpackg = cp;
	    break;

	case OPTVERSION:
	    getout (YES, "");

	default:
	unrecog:
	    opterr = "not recognized";
	error:
	    {
		S_looktbl *slpt;
		printf ("\n%s option %s\n", argarray[curarg], opterr);
		printf ("\nDefined options :");
		for ( slpt = &opttable[0] ; slpt->str ; slpt ++ )
		    printf (" -%s,", slpt->str);
		printf ("\n\nUse : \"%s --help\" for a short description\n", argarray[0]);
		getout (YES, "");
	    }
	}
    }
    return;
}

/* ++XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */

/* check_access : check the access right of a file or directory */
/* ------------------------------------------------------------ */

static int check_access (char *fname, int mode, char **str)
    /* return : 0 = ok, 1 = no access right, 2 = not existing */
{
    int cc, err;

    cc = access (fname, mode);
    if ( cc == 0 ) {
	*str = " access ok";
	return (0);
    }
    err = errno;
    cc = access (fname, F_OK);
    if ( cc < 0 ) {
	err = errno;
	*str = " not existing";
	return (2);
    }
    if      ( mode & X_OK ) *str = " no execute access";
    else if ( mode & W_OK ) *str = " no write access";
    else if ( mode & R_OK ) *str = " no read access";
    else                    *str = " no access";
    return (1);
}

#ifdef COMMENT
static char *
packg_file ()
.
    Build the full file name for a Rand package file
#endif
static char *
packg_file (fname)
char *fname;
{
    extern int dircheck ();
    char *file, *fulln;
    int sz;

    if ( dirpackg ) {
	file = strrchr (fname, '/');
	if ( ! file ) file = fname;
	sz = strlen(dirpackg) + strlen(file) +2;
	fulln = (char *) malloc (sz);
	(void) memset (fulln, 0, sz);
	if ( fulln ) {
	    (void) strcpy (fulln, dirpackg);
	    if ( file[0] != '/' ) (void) strcat (fulln, "/");
	    (void) strcat (fulln, file);
	    return fulln;
	}
    }
    return fname;
}
/* --XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */

/* get_cwd : get the current working directory */
static char * get_cwd ()
{
    char *sp;
    char cwd_buf [512];

    if ( pwdname ) return (pwdname);

    (void) memset (cwd_buf, 0, sizeof (cwd_buf));
    sp = getcwd (cwd_buf, sizeof (cwd_buf) -1);
    if ( ! sp ) cwd_buf[0] = '.';
    sp = &cwd_buf[strlen (cwd_buf) -1];
    if ( *sp == '/' ) *sp = '\0';
    pwdname = (char *) malloc (strlen (cwd_buf) +1);
    if ( pwdname ) strcpy (pwdname, cwd_buf);
    return (pwdname);
}

/* get_exec_name : try to find the exec file, using the $PATH */
char * get_exec_name (char *fname)
{
    char *sp0, *sp1, *path, *fnm;
    char str[2048];
    int sz, msz;

    if ( ! fname ) return (NULL);

    if ( strchr (fname, '/') ) fnm = fname;
    else {
	path = getenv ("PATH");
	if ( !path ) path = ":/bin:/usr/bin";   /* default PATH value */

	fnm = str;
	msz = sizeof (str);
	for ( sp0 = path ; ; sp0 = sp1 +1 ) {
	    memset (fnm, 0, msz);
	    sp1 = strchr (sp0, ':');    /* next token */
	    if ( !sp1 ) sp1 = &path[strlen (path)];
	    sz = sp1 - sp0;
	    if ( sz == 0 ) break;   /* no more token */
	    if ( sz <= 1 ) continue;
	    if ( sz >= (msz -1) ) break;    /* too large path token */

	    strncpy (fnm, sp0, sz);
	    if ( fnm[sz-1] != '/' ) fnm[sz] = '/';
	    strncat (fnm, fname, msz - sz -1);
	    if ( access (fnm, X_OK) == 0 ) break;
	}
    }
    if ( access (fnm, X_OK) < 0 ) return (NULL);

    /* executable file found */
    sz = strlen (fnm);
    sp0 = (char *) malloc (sz +1);
    if ( sp0 ) strcpy (sp0, fnm);
    return (sp0);
}

/* get_flname : get actual name (follow link) */
char * get_flname (char *fname)
{
#ifdef __linux__
    /* try to found the actual directory of the program */
    int cc, nb;
    struct stat lstbuf;
    char pkgnm [1024];
    char lbuf [512];
    char *sp;

    (void) memset (pkgnm, 0, sizeof (pkgnm));
    if ( fname[0] != '/' ) {    /* relative path file name */
	sp = get_cwd ();
	if ( sp ) strncpy (pkgnm, sp, sizeof (pkgnm) -2);
	else pkgnm[0] = '.';
	pkgnm[strlen (pkgnm)] = '/';
    }
    strncat (pkgnm, fname, sizeof (pkgnm));
    pkgnm [sizeof (pkgnm) -1] = '\0';
    for ( ; ; ) {
	cc = lstat (pkgnm, &lstbuf);
	if ( cc ) break;
	if ( ! S_ISLNK (lstbuf.st_mode) ) break;

	nb = readlink(pkgnm, lbuf, sizeof (lbuf));
	if ( nb < 0 ) return (NULL);    /* some error */

	lbuf[nb] = '\0';
	if ( lbuf[0] == '/' ) pkgnm[0] = '\0';    /* absolu path */
	else {      /* relative path, remove the file name */
	    sp = strrchr (pkgnm, '/');
	    if ( sp ) *(sp +1) = '\0';
	    else pkgnm[0] = '\0';
	}
	if ( (strlen (pkgnm) + nb) >= sizeof (pkgnm) ) return (NULL); /* too large name */
	strcat (pkgnm, lbuf);
    }
    if ( ! pkgnm[0] ) return (NULL);    /* not found */
    sp = (char *) malloc (strlen (pkgnm) +1);
    if ( sp ) strcpy (sp, pkgnm);
    return (sp);
#else
    return (NULL);
#endif
}


static void get_dirpackage ()
{
    char *fnm, *dir, *cp, *cp1, *home;
    int i, sz;

    sz = strlen (def_xdir_dir);
    if ( (sz > 1) && (def_xdir_dir[sz-1] == '/') ) def_xdir_dir[sz-1] = '\0';

    /* get the Rand program directory */
    fnm = get_exec_name (argarray[0]);
    full_prgname = dir = get_flname (fnm);
    if ( full_prgname ) {
	cp = strrchr (full_prgname, '/');
	if ( cp ) {
	    prog_fname = cp+1;
	    sz = cp - full_prgname;
	    dir = (char *) malloc (sz +1);
	    if ( dir ) {
		strncpy (dir, full_prgname, sz);
		dir[sz] = '\0';
	    }
	    else dir = full_prgname;

	    if ( strcmp (prog_fname, buildexec) == 0 ) {
		/* special case for debugging a not yet installed executable */
		buildflg = YES;
		cp = "/../help";
		sz = strlen (dir) + strlen(cp) +1;
		cp1 = (char *) malloc (sz);
		if ( cp1 ) {
		    strcpy (cp1, dir);
		    strcat (cp1, cp);
		}
		dir = cp1;
	    }
	}
    }
    dirpackg = ( optdirpackg ) ? optdirpackg : getenv ("RAND_PACKG");
    if ( dirpackg && !*dirpackg) dirpackg = NULL;
    if ( ! dirpackg ) {
	dirpackg = dir;     /* from the Rand prog directory */
    }
    if ( dirpackg ) {
	for ( cp = &dirpackg[strlen (dirpackg) -1] ; *cp == ' ' ; cp-- ) ;
	if ( *cp == '/' ) *cp = '\0';
    }
    /* If directory package is not defined, take the compiled default */
    xdir_dir = dirpackg ? dirpackg : def_xdir_dir;

    /* package subdirectory */
    cp = strrchr (def_xdir_dir, '/');
    packg_subdir = ( cp ) ? cp : def_xdir_dir;

    /* build the configuration directory pathes */
    home = getenv ("HOME");

    sz = (4 * strlen (packg_subdir)) + (5 * strlen (kbfilesdir))
	 + strlen (local_cnfg) +1
	 + strlen (syst_cnfg) +1
	 + ( (home) ? strlen (home) : 0 ) +2
	 + (2 * strlen (xdir_dir)) + strlen (alt_local_cnfg) +2;
    cp = cp1 = (char *) malloc (sz);

    if ( cp1 ) {
	struct stat stat_local, stat_alt_local;

	(void) memset (cp1, 0, sz);
	(void) memset (&stat_local, 0, sizeof(struct stat));
	(void) memset (&stat_alt_local, 0, sizeof(struct stat));

	def_cnfg_dir.path = cp1;
	sprintf (def_cnfg_dir.path, "%s%s", xdir_dir, kbfilesdir);
	cp1 += (strlen (def_cnfg_dir.path) +1);
	if ( access (def_cnfg_dir.path, R_OK) == 0 ) def_cnfg_dir.existing = YES;

	syst_cnfg_dir.path = cp1;
	sprintf (syst_cnfg_dir.path, "%s%s%s", syst_cnfg, packg_subdir, kbfilesdir);
	cp1 += (strlen (syst_cnfg_dir.path) +1);
	if ( access (syst_cnfg_dir.path, R_OK) == 0 ) syst_cnfg_dir.existing = YES;

	local_cnfg_dir.path = cp1;
	sprintf (local_cnfg_dir.path, "%s%s%s", local_cnfg, packg_subdir, kbfilesdir);
	cp1 += (strlen (local_cnfg_dir.path) +1);
	if ( access (local_cnfg_dir.path, R_OK) == 0 ) {
	    local_cnfg_dir.existing = YES;
	    (void) stat (local_cnfg_dir.path, &stat_local);
	    }

	if ( ! buildflg ) {   /* not debugging a new release of the editor */
	    alt_local_cnfg_dir.path = cp1;
	    sprintf (alt_local_cnfg_dir.path, "%s%s%s%s", xdir_dir, alt_local_cnfg, packg_subdir, kbfilesdir);
	    cp1 += (strlen (alt_local_cnfg_dir.path) +1);
	    if ( access (alt_local_cnfg_dir.path, R_OK) == 0 ) {
		alt_local_cnfg_dir.existing = YES;
		(void) stat (alt_local_cnfg_dir.path, &stat_alt_local);
		if ( (stat_alt_local.st_dev = stat_local.st_dev)
		     && (stat_alt_local.st_ino = stat_local.st_ino) ) {
		    alt_local_cnfg_dir.existing = NO;
		    alt_local_cnfg_dir.path = NULL;
		}
	    }
	}

	if ( home ) {
	    user_cnfg_dir.path = cp1;
	    sprintf (user_cnfg_dir.path, "%s/.%s%s", home, packg_subdir+1, kbfilesdir);
	    if ( access (user_cnfg_dir.path, R_OK) == 0 ) user_cnfg_dir.existing = YES;
	}
    }

    /* get the max lenth of the config dir path name */
    max_cnfg_dir_sz = 0;
    for ( i = cnfg_dir_rec_pt_sz -1 ; i >= 0 ; i-- ) {
	if ( cnfg_dir_rec_pt[i] && cnfg_dir_rec_pt[i]->path )
	    if ( strlen (cnfg_dir_rec_pt[i]->path) > max_cnfg_dir_sz )
		max_cnfg_dir_sz = strlen (cnfg_dir_rec_pt[i]->path);
    }
}

#ifdef COMMENT
void
startup ()
.
    Various initializations.
#endif
void
startup ()
{
    extern Flag get_keyboard_map ();
    char  *name;

    /* ++XXXXXXXXXXXXXXXXXXXXXXXXXXXXX */

#ifdef __linux__
    extern char def_xdir_dir    [];
    extern char def_recovermsg  [];
    extern char def_xdir_kr     [];
    extern char def_xdir_help   [];
    extern char def_xdir_crash  [];
    extern char def_xdir_err    [];
    extern char def_xdir_run    [];
    extern char def_xdir_fill   [];
    extern char def_xdir_just   [];
    extern char def_xdir_center [];
    extern char def_xdir_print  [];
#endif

#ifdef  SHORTUID
    userid = getuid ();
    groupid = getgid ();
#else   /* uid is a char (old unix version) */
    userid  = getuid () & 0377;
    groupid = getgid () & 0377;
#endif
    getmypath ();   /* gets myname & mypath */

    keytmp = bkeytmp = rfile = inpfname = NULL;

    get_dirpackage ();

#ifdef __linux__
    recovermsg  = packg_file (def_recovermsg);
    xdir_kr     = packg_file (def_xdir_kr);
    xdir_help   = packg_file (def_xdir_help);
    xdir_crash  = packg_file (def_xdir_crash);
    xdir_err    = packg_file (def_xdir_err);
    xdir_run    = packg_file (def_xdir_run);
    xdir_fill   = packg_file (def_xdir_fill);
    xdir_just   = packg_file (def_xdir_just);
    xdir_center = packg_file (def_xdir_center);
    xdir_print  = packg_file (def_xdir_print);

#else
    recovermsg  = packg_file (RECOVERMSG);
    xdir_kr     = packg_file (XDIR_KR);
    xdir_help   = packg_file (XDIR_HELP);
    xdir_crash  = packg_file (XDIR_CRASH);
    xdir_err    = packg_file (XDIR_ERR);
    xdir_run    = packg_file (XDIR_RUN);
    xdir_fill   = packg_file (XDIR_FILL);
    xdir_just   = packg_file (XDIR_JUST);
    xdir_center = packg_file (XDIR_CENTER);
    xdir_print  = packg_file (XDIR_PRINT);
#endif /* __linux__ */

    filterpaths[0] = xdir_fill;
    filterpaths[1] = xdir_just;
    filterpaths[2] = xdir_center;
    filterpaths[3] = xdir_print;

    /* --XXXXXXXXXXXXXXXXXXXXXXXXXXXXX */

    Block {
	Short i;
	for (i=0; i<MAXFILES ; i++) {
		names[i] = "";
/*
		oldnames[i] = "";
*/
	}
     }

#ifdef  SIGNALS
    Block {
	Reg1 Short  i;
	for (i = 1; i <= NSIG; ++i)
	    switch (i) {
	    case SIGINT:
		/* ignore so that replay questionnaire and exiting are
		 * not vulnerable
		 **/
		signal (i, SIG_IGN);
		break;

#ifdef SIGCHLD  /* 4bsd vmunix */
	    case SIGCHLD:
#endif
#ifdef SIGTSTP  /* 4bsd vmunix */
	    case SIGTSTP:
	    case SIGCONT:
#endif
#ifdef CERN     /* while we are debugging */
	    case SIGBUS:
	    case SIGSEGV:
#endif
#ifdef SIGWINCH /* cannot handle window size change (yet?) */
	    case SIGWINCH:
#endif
		/* leave at SIG_DFL */
		break;

	    default:
		if (signal (i, SIG_DFL) != SIG_IGN)
		    (void) signal (i, sig);
	    }
    }
#endif  /* SIGNALS */

#ifdef PROFILE
    Block {
	extern Flag profiling;
	/* sample only 1/8 of all editor runs */
	if (strttime & 7)
	    profil ((char *) 0, 0, 0, 0);
	else
	    profiling = YES;
    }
#endif

    /*
     *  Do this before setitty so that cbreakflg can be cleared if
     *  it is inappropriate for the type of terminal.
     */
    gettermtype ();
    /* stty on input to CBREAK or RAW and ~ECHO ASAP to allow typeahead */
    setitty ();

    Block {
	extern time_t time ();
	time (&strttime);
    }

    /* stty on output for ~CRMOD and ~XTABS */
    setotty ();

    /* XXXXXXXXXXXXXX : do not open files in -help option */
    if (helpflg) {
	extern char *TI, *KS, *VS;
	Xterm_flg = get_keyboard_map (tname, 4, TI, KS, VS, kbinistr);
	return;
    }

    /* initialize terminal.  must be done before "starting..." message */
    /* so keys are initialized. */
    if (!silent) {
	extern char *TI, *KS, *VS;
	d_put (VCCICL);
	windowsup = YES;
	Xterm_flg = get_keyboard_map (tname, 4, TI, KS, VS, kbinistr);
    }
    if (loginflg)
	printf ("%s is starting...", progname);
#ifdef  DEBUGDEF
    else
	printf ("terminal = %s,  keyboard = %s.\n\r", tname, kname);
    sleep (1);
#endif  /* DEBUGDEF */

#ifdef VBSTDIO
    if (outbuf = salloc (screensize+1, YES)) {
	setbuf (stdout, outbuf);
	stdout->_bufsiz = screensize;
    }
    else
	setbuf (stdout, (char *) _sobuf);
#endif

    blanks = salloc (term.tt_width+1, YES);
    fill (blanks, (Uint) term.tt_width, ' ');

#ifdef NMN
    Block {
	Reg1 Short  i;
	static Ff_buf ffbufhdrs[NUMFFBUFS];
	static char ffbufs[NUMFFBUFS * FF_BSIZE];

	for (i = Z; i < NUMFFBUFS; i++) {
	    ffbufhdrs[i].fb_buf = &ffbufs[i * FF_BSIZE];
	    ff_use ((char *)&ffbufhdrs[i], 0);
	}
    }
#else
    /* Donot use static buffers for OSF/1 ... */
    ff_alloc(NUMFFBUFS,0);
#endif

    la_maxchans = MAXSTREAMS;   /* Essentially no limit.  Limit is dealt */
				/* with in editfile () */

    /* set up cline
     * start with size of first increment
     **/
    cline = salloc ((lcline = icline) + 1, YES);

    /* build the names of the working files */
    Block {
	struct stat statbuf;

	/* check current directory */
	/* #define PRIV (S_IREAD | S_IWRITE | S_IEXEC) */
	if (   stat (".", &statbuf) != -1
	    && access (".", 7) >= 0
	    && (   userid != 0
		|| statbuf.st_uid == 0
	       )
	   ) {
	    tmppath = "./.e"; /* use current directory for e files */
			      /* and make them invisible */
	    if (statbuf.st_uid != userid)
		name = append ("", "");     /* so that name is allocated */
	    else
		name = append (".", myname); /* we will append user name */
	}
	else {
	    /* maybe we should make tmppath "logindir/etmp/" here */
	    name = append (".", myname); /* we will append user name */
	}
    }

#ifdef BIGADDR
    la_nbufs = 20;
#else
    la_nbufs = 10;
#endif
    Block {
	Reg1 char *c;
	c = append (tmpnstr, name);
	la_cfile = append (tmppath, c);
	sfree (c);
	c = append (rstr, name);
	rfile = append (tmppath, c);
	sfree (c);
    }
    Block {
	Reg1 Short indv;
	indv = strlen (tmppath) + VRSCHAR;
	for (evrsn = '1'; ; evrsn++) {
	    if (evrsn > '9')
		getout (NO, "\n%s: No free temp file names left\n", progname);
	    la_cfile[indv] = evrsn;
	    rfile[indv] = evrsn;
	    Block {
		Reg3 int i;
		if ((i = open (la_cfile, 0)) >= 0) {
		    /* Should use exclusive open here, but we don't have it */
		    /* exclusive => exists and no one has it exclusively open */
		    close (i);
		    dorecov (0);
		    crashed = YES;
		    break;
		}
	    }
	    /* if we get here, it's because either
	     *  (la_cfile exists and someone is using it)
	     *  or (it doesn't exist)
	     **/
	    if (access (la_cfile, 0) >= 0)
		/* exists and someone is using it */
		continue;
	    if (notracks && access (rfile, 0) >= 0)
		continue;
	    break;
	}
    }

    names[CHGFILE] = la_cfile;
    fileflags[CHGFILE] = INUSE;
    fileflags[NULLFILE] = INUSE;

    /* make the rest of the file names */
    bkeystr[VRSCHAR] = keystr[VRSCHAR] = evrsn;
    Block {
	Reg1 char *c;
	c = append (keystr, name);
	keytmp  = append (tmppath, c);
	sfree (c);
	c = append (bkeystr, name);
	inpfname = bkeytmp = append (tmppath, c);
	sfree (c);
    }
    sfree (name);

    Block {
	struct stat statbuf;
	if (!crashed && stat (keytmp, &statbuf) != -1)
	    dorecov (1);
    }
    if (!chosereplay)
	inpfname = replfname;

    mv (keytmp, bkeytmp);
    keysmoved = YES;
    if ((keyfile = fopen (keytmp, "w")) == NULL)
	getout (YES, "Can't create keystroke file (%s).", keytmp);
    chmod (keytmp, 0600);

    /* Set up utility windows.
     * They have no margins, and they are not on winlist.
     **/
    /* wholescreen window.  general utility */
    setupwindow (&wholescreen, 0, 0,
#ifdef LMCSTATE
		 term.tt_width - 1, term.tt_height - 1, 0, NO);
#else
		 term.tt_width - 1, term.tt_height - 1, NO);
#endif
    /* parameter entry window */
    setupwindow (&enterwin, 0,
		 term.tt_height - NENTERLINES - NINFOLINES,
		 term.tt_width - 1,
#ifdef LMCSTATE
		 term.tt_height - 1 - NINFOLINES, 0, NO);
#else
		 term.tt_height - 1 - NINFOLINES, NO);
#endif
    enterwin.redit = term.tt_width - 1;
    /* info display window. */
    setupwindow (&infowin, 0,
		 term.tt_height - NINFOLINES,
		 term.tt_width - 1,
#ifdef LMCSTATE
		 term.tt_height - 1, 0, NO);
#else
		 term.tt_height - 1, NO);
#endif

    curwin = &wholescreen;
    return;
}


static void display_env_var (char *buf, char *var_name, char *strg) {
    char *tmp;
    tmp = getenv (var_name);
    if ( !tmp || !tmp[0] ) tmp = strg;
    if ( tmp ) sprintf (buf + strlen (buf),
			"    %-7s = %s\n", var_name, tmp);
}

static int wait_cont ()
{
    extern int wait_continue ();
    char ch;
    int cc;

    if ( ! helpflg ) {
	cc = wait_continue (0);
	return (cc);
    }

    printf ("\nPush a key to continue, <Ctrl C> or Escape to end\n");
    ch = getchar ();
    if ( (ch == '\003') || (ch == '\033') ) return (1);
    (*term.tt_clear) ();
    return (0);
}

static void display_fn (char *buf, char *msg, char *fnm)
{
    if ( (strlen (msg) + strlen (fnm)) >= term.tt_width )
	sprintf (buf + strlen (buf), "%s\n  %s\n", msg, fnm);
    else
	sprintf (buf + strlen (buf), "%s %s\n", msg, fnm);
}

/* Show Help processing */
static void do_showhelp (full_flg)
Flag full_flg;    /* ful dispaly */
{
    extern char * filestatusString ();
    extern char kbmap_fn[];
    extern char verstr[];
    static void display_bigbuf ();
    char *tmpstrg;
    int i, nbli;
    char bigbuf [4096]; /* must be large enough for the message */
    char strg [256];

    nbli = 0;
    memset (bigbuf, 0, sizeof (bigbuf));

    /* -------
    fixtty ();
    if (windowsup) screenexit (YES);
    ------- */

    fputc ('\n', stdout);
    if ( full_flg ) 
	sprintf (bigbuf + strlen (bigbuf), "\
Synopsis: %s [options] file [alternate file]\n\
 options are:\n", progname);
    else {
	sprintf (bigbuf + strlen (bigbuf), "\
Overall status of the editor parameters\n\
=======================================\
\n");
	sprintf (bigbuf + strlen (bigbuf),
		 "  %s\n\n", verstr);
	if ( optusedflg ) sprintf (bigbuf + strlen (bigbuf),
			"In use command line program options :\n");
	else sprintf (bigbuf + strlen (bigbuf),
			"No in use command line program option\n");
    }

    if ( full_flg || (optbullets >= 0) )
    sprintf (bigbuf + strlen (bigbuf), "\
%c -bullets\n", optbullets == YES ? '*' : ' ');

    if (dbgflg)
	sprintf (bigbuf + strlen (bigbuf), "\
* -debug=%s\n", dbgname);

#ifdef LMCOPT
    if ( full_flg || stdkbdflg )
    sprintf (bigbuf + strlen (bigbuf), "\
%c -dkeyboard : force the mini keyboard definition (only control charaters)\n", stdkbdflg ? '*' : ' ');
#endif

    if ( full_flg || helpflg )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -help : short help and some checks\n", helpflg ? '*' : ' ');

    if ( full_flg || inplace )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -inplace\n", inplace ? '*' : ' ');

#ifdef  KBFILE
    if ( full_flg || optkbfile ) {
	if ( ! optkbfile ) tmpstrg = "<file name>";
	else tmpstrg = optkbfile;
	sprintf (bigbuf + strlen (bigbuf), "\
%c -kbfile=%s : (keyboard file, cf EKBFILE)\n", optkbfile ? '*' : ' ', tmpstrg);
    }
#endif

    if ( full_flg || optkname )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -keyboard=%s : (use compiled internal kbd definition, cf EKBD)\n", optkname ? '*' : ' ',
	    optkname ? optkname : "<keyboard type>");

    if ( full_flg || (optbullets >= 0) )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -nobullets\n", optbullets == NO ? '*' : ' ');

#ifdef  NOCMDCMD
    if ( full_flg || optnocmd )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -nocmdcmd\n", optnocmd ? '*' : ' ');
#endif

    if ( full_flg || norecover )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -norecover\n", norecover ? '*' : ' ');

    if ( full_flg || notracks )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -notracks\n", notracks ? '*' : ' ');

    if ( full_flg || patmode )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -package=%s : (package directory, cf RAND_PACKG)\n", optdirpackg ? '*' : ' ',
	    optdirpackg ? optdirpackg : "<directory>");

    if ( full_flg || patmode )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -regexp\n", patmode ? '*' : ' ');

    if ( full_flg || replaying )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -replay=%s\n", replaying ? '*' : ' ', replaying ? inpfname : "<filename>");

    if ( full_flg || silent )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -silent\n", silent ? '*' : ' ');

#ifdef LMCSTATE
    if ( full_flg )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -state=%s\n", state ? '*' : ' ', state ? statefile : "<filename>");
#endif

    if ( full_flg || opttname )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -terminal=%s : (terminal type, default : %s, cf TERM)\n",
	opttname ? '*' : ' ', opttname ? opttname : "<terminal type>", tname);

    if ( full_flg || verbose_helpflg )
	sprintf (bigbuf + strlen (bigbuf), "\
%c -verbose : verbose help for kbfiles debugging\n",
	verbose_helpflg ? '*' : ' ');

    if ( full_flg )
	sprintf (bigbuf + strlen (bigbuf), "\
  -version : display the version and revision info\n");

    if ( optusedflg )
	sprintf (bigbuf + strlen (bigbuf),
		 "\"*\" means this option is in effect.\n");

    if ( dbgflg )
	sprintf (bigbuf + strlen (bigbuf), "\
NB : -debug=debug_file_name option cannot be use by an indirect call\n\
     using a batch file. Only the default debug output can be used !\n");

    sprintf (bigbuf + strlen (bigbuf), "\n\
Environment variables known by Rand editor:\n\
  EKBD, EKBFILE, RAND_PACKG, SHELL, TERM, VBELL and editalt\n");
    display_env_var (bigbuf, "EKBD", NULL);
    display_env_var (bigbuf, "EKBFILE", NULL);
    display_env_var (bigbuf, "RAND_PACKG", NULL);
    display_env_var (bigbuf, "SHELL", NULL);
    display_env_var (bigbuf, "TERM", "(mandatory if \"-terminal\" not used)");
    display_env_var (bigbuf, "VBELL", NULL);
    display_env_var (bigbuf, "editalt", NULL);
    bigbuf [strlen (bigbuf)] = '\n';

    display_bigbuf (bigbuf, &nbli, sizeof (bigbuf));

    tmpstrg = termtype ? "" : " (use termcap or terminfo)";
    sprintf (bigbuf + strlen (bigbuf), "%sTerminal : \"%s\"%s, keyboard : %s (%d)\n",
	    (Xterm_flg)? "X-" : "", tname, tmpstrg, kname, kbdtype);

    if ( kbfile ) {     /* use termcap or terminfo and key board definition file */
	extern char * get_keyboard_mode_strg ();
	extern int kbfile_wline;    /* > 0 : duplicated string in kbfile */
	int cc;

	if ( kbfcase )
	    sprintf (strg, "Keyboard definition file (defined by %s):", kbfcase);
	else
	    strcpy (strg, "Keyboard definition file:");
	display_fn (bigbuf, strg, kbfile);
	cc = check_access (kbfile, R_OK, &tmpstrg);
	if ( cc ) {
	    Flag ok;
	    ok = NO;    /* warning not printed */
	    if ( cc == 2 ) {    /* not exiting, check directory */
		char *fpt, *msg;
		fpt = strrchr (kbfile, '/');
		if ( fpt ) {    /* directory specified */
		    *fpt = '\0';
		    if ( check_access (kbfile, R_OK, &msg) ) {
			char *dpt;
			dpt = strrchr (kbfile, '/');
			dpt = ( dpt ) ? dpt+1 : kbfilesdir;
			sprintf (bigbuf + strlen (bigbuf),
				 "    WARNING %s directory :%s\n", dpt, msg);
			ok = YES;
		    }
		    *fpt = '/';     /* rebuild the kfile name */
		}
	    }
	    if ( !ok ) sprintf (bigbuf + strlen (bigbuf),
				"    WARNING kbfile :%s\n", tmpstrg);
	} else if ( kbfile_wline > 0 ) {
	    sprintf (bigbuf + strlen (bigbuf),
		     "    WARNING kbfile : duplicated string at line %d\n", kbfile_wline);
	}
	sprintf (bigbuf + strlen (bigbuf), "Keyboard expected mode : %s\n",
		 get_keyboard_mode_strg ());
    }
    else {      /* use build in compiled keyboard definition */
	if ( stdkbdflg )
	    sprintf (bigbuf + strlen (bigbuf), "Use internal mini definition for %s (forced by \"-dkeyboard\" option)\n", kname);
	else
	    if ( kbfcase )
		sprintf (bigbuf + strlen (bigbuf), "Use internal definition for %s (%s)\n", kname, kbfcase);
	    else
		sprintf (bigbuf + strlen (bigbuf), "No keyboard definition file, use internal definition for %s\n", kname);
    }

    if ( helpflg ) {
	static int get_kbfile_dname (char *, char *, int, char **);
	S_looktbl *slpt;

	sprintf (bigbuf + strlen (bigbuf), " Build in terminals & keyboards :");
	for ( slpt = &termnames[0] ; slpt->str ; slpt ++ )
	    sprintf (bigbuf + strlen (bigbuf), " \"%s\",", slpt->str);
	bigbuf [strlen (bigbuf)] = '\n';
	sprintf (bigbuf + strlen (bigbuf), " Expect \"%s%s\" (term family) or \"%s\" keyboard definition file in :\n",
		 tname, kbfile_postfix, universal_kbf_name);
	if ( buildflg ) {   /* debugging a new release of the editor */
	    for ( i = cnfg_dir_rec_pt_sz -1 ; i >= 0 ; i-- ) {
		if ( cnfg_dir_rec_pt[i] == NULL ) continue;
		if ( cnfg_dir_rec_pt[i]->path )
		    sprintf (bigbuf + strlen (bigbuf), "  %s%s\n",
			     cnfg_dir_rec_pt[i]->path,
			     cnfg_dir_rec_pt[i]->existing ? "" : no_exist_msg);
	    }
	} else {
	    for ( i = 0 ; i < cnfg_dir_rec_pt_sz ; i++ ) {
		if ( cnfg_dir_rec_pt[i] == NULL ) continue;
		if ( cnfg_dir_rec_pt[i]->path )
		    sprintf (bigbuf + strlen (bigbuf), "    %s%s\n",
			     cnfg_dir_rec_pt[i]->path,
			     cnfg_dir_rec_pt[i]->existing ? "" : no_exist_msg);
	    }
	}
    }
    bigbuf [strlen (bigbuf)] = '\n';

    tmpstrg = buildflg ? " (build mode)" : "";
    sprintf (strg, "%sRand package and HELP directory path%s:",
	    dirpackg ? "" : "Default buid in ", tmpstrg);
    display_fn (bigbuf, strg, xdir_dir);
    if ( check_access (xdir_dir, R_OK, &tmpstrg) )
	sprintf (bigbuf + strlen (bigbuf), "    WARNING : %s\n", tmpstrg);
    else {
	static void check_message_file ();
	check_message_file (recovermsg, bigbuf);
	/* check_message_file (xdir_kr   , bigbuf);  -- no more in use */
	check_message_file (xdir_help , bigbuf);
	check_message_file (xdir_crash, bigbuf);
	check_message_file (xdir_err  , bigbuf);
    }

    if ( ! helpflg ) {
	display_fn (bigbuf, "Default directory:",
		    default_workingdirectory);
	tmpstrg = get_cwd ();
	if ( tmpstrg )
	    display_fn (bigbuf, "Current working directory:", tmpstrg);
    }

    /* working files */
    if ( keytmp && rfile && names[CHGFILE] ) {
	sprintf (bigbuf + strlen (bigbuf), "Current Editor status, keystroke, changes files :\n  %s\n  %s\n  %s\n",
		rfile, keytmp, names[CHGFILE]);
    }
    strcat (bigbuf + strlen (bigbuf), "Edited files :\n");
    for ( i = FIRSTFILE + NTMPFILES ; i < MAXFILES ; i++) {
	char *fname;
	if ( !(fileflags[i] & INUSE) ) continue;
	tmpstrg = filestatusString (i, &fname);
	if ( tmpstrg )
	    sprintf (bigbuf + strlen (bigbuf), "  %s%s\n", tmpstrg, fname);
    }

    if ( full_prgname )
	display_fn (bigbuf, "Rand program:", full_prgname);

    sprintf (bigbuf + strlen (bigbuf), "Current user : %s (uid = %d, gid = %d)\n",
	myname, userid, groupid);

    if ( helpflg ) {
	sprintf (bigbuf + strlen (bigbuf),
		 "\nThis is %s : Rand editor version %d release %d\n%s\n",
		 prog_fname ? prog_fname : progname, -revision, subrev, verstr);
    }
    else {
	extern void setoption_msg ();
	void getConsoleSize ();

	int x, y;
	char buf[128];

	getConsoleSize (&x, &y);
	sprintf (bigbuf + strlen (bigbuf), "\nCurrent screen size %d lines of %d columns\n", y, x);
	setoption_msg (buf);
	sprintf (bigbuf + strlen (bigbuf), "Current value of SET command paramters (use 'help set' command for details) \n%s\n\n", buf);
    }

    display_bigbuf (bigbuf, &nbli, sizeof (bigbuf));
    return;
}

static void check_message_file (char *fname, char *buf)
{
    char *strg, *fn;
    int cc;

    cc = check_access (fname, R_OK, &strg);
    if ( cc == 0 ) return;
    fn = strrchr (fname, '/');
    fn = ( fn ) ? fn+1 : fname;
    sprintf (buf + strlen (buf), "    WARNING : Rand package file \"%s\" :%s\n",
	     fn, strg);
}

static void display_bigbuf (char *buf, int *nbli, int bufsz)
{
    int nb, dispsz;
    char ch, *sp0, *sp1;

    dispsz = term.tt_height;
    if ( verbose_helpflg  ) dispsz = 100000;     /* very very large : do not hold the screen */
    for ( sp0 = sp1 = buf ; (ch = *sp1) ; sp1++ ) {
	if ( ch != '\n' ) continue;
	(*nbli)++;
	if ( *nbli < (dispsz -2) ) continue;
	*sp1 = '\0';
	fputs (sp0, stdout);
	fputc ('\n', stdout);
	fputc ('\n', stdout);
	sp0 = sp1 +1;
	wait_keyboard ("Push a key to continue");
	fputs ("                                         \r", stdout);
	*nbli = 2;  /* overlap size */
	(*term.tt_addr) (dispsz -2, 0);
    }
    if ( *sp0 ) fputs (sp0, stdout);
    if ( dbgflg )
	printf ("-- debug -- message size %d, nb of lines %d\n", strlen (buf), *nbli);
    memset (buf, 0, bufsz);
}

#ifdef COMMENT
void
showhelp ()
.
    Do the help option.
#endif
void
showhelp ()
{
    extern void print_it_escp ();
    extern void print_all_ccmd ();
    extern void display_keymap ();

    void reset_crlf (int);
    int oflag;

    oflag = set_crlf ();
    do_showhelp (YES);
    if ( verbose_helpflg ) {
	extern int help_info (char *, int *);
	print_it_escp ();   /* print the it table tree (escape seq to ccmd) */
	print_all_ccmd ();  /* print ccmd assignement (escape seq) */
	display_keymap (verbose_helpflg);
	(void) help_info ("Linux_key_mapping", NULL);
    }
    reset_crlf (oflag);
}

void getConsoleSize (int *x, int *y) {
    *x = term.tt_width;
    *y = term.tt_height;
}

void showstatus ()
{
    extern void setoption_msg ();
    int x, y;
    char buf[128];

    do_showhelp (NO);
}


#ifdef COMMENT
void
dorecov (type)
    int type;
.
    Ask the user how he wants to deal with a crashed or aborted session.
    Type is 0 for crashed, 1 for aborted.
    If the user opts for a recovery or replay, anything which was set by
    checkargs() must be set here to the way we want it for the replay.
#endif
void
dorecov (type)
Reg3 int type;
{
    /* XXXXXXXXXXXXXXXXXXXXXXX */
    if (helpflg)
	return;
    if (norecover)
	return;
    fixtty ();
    printf("\n"); fflush(stdout);
    for (;;) Block {
	char line[132];
	Block {
	    int tmp;
	    /* XXXXXXXXXXXXXXXXX */
	    tmp = dup (1);      /* so that filecopy can close it */
	    if (   (tmp = filecopy (recovermsg, 0, (char *) 0, tmp, NO)) == -1
		|| tmp == -3
	       )
		fatalpr ("Please notify the system administrators\n\
that the editor couldn't read file \"%s\".\n", recovermsg);
	}
	printf("\n\
Type the number of the option you want then hit <RETURN>: ");
	fflush (stdout);
	fgets (line, sizeof (line), stdin);
	if (feof (stdin))
	    getout (type, "");
	switch (*line) {
	case '1':
	    recovering = YES;
	    silent = YES;
	    if (0) {
	case '2':
		recovering = NO;
		silent = NO;
	    }
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
    return;
}

/* set, reset the terminal character set */
/* ------------------------------------- */
void (*reset_term_proc) () = NULL;

void reset_term ()
{
    if ( reset_term_proc ) (*reset_term_proc) ();
    reset_term_proc = NULL;
}

void set_term ()
{
    extern void (*alt_specialchar ()) ();

    reset_term_proc = alt_specialchar (tname);
}

/* build_kbfile : build the default keyboard definition file name */
/* -------------------------------------------------------------- */

static char * get_kbfile_buf (char *term_family, int *buf_sz)
{
    char * kbfname;
    int sz;

    *buf_sz = 0;
    if ( ! xdir_dir ) return (NULL);

    sz = strlen (term_family) + strlen (kbfile_postfix);
    if ( sz < sizeof (universal_kbf_name) ) sz = sizeof (universal_kbf_name);

    sz += max_cnfg_dir_sz +2;
    kbfname = (char *) malloc (sz);
    if ( kbfname ) memset (kbfname, 0, sz);
    *buf_sz = sz;
    return (kbfname);
}

static int get_kbfile_dname (char *term_family,
			     char * kbfile_buf, int kbfile_buf_sz,
			     char **kbfcase_pt)
{
    /* kbfile_buf must be large enough */
    char * kbfname;
    int sz;

    if ( ! xdir_dir ) return (0);

    memset (kbfile_buf, kbfile_buf_sz, 0);
    if ( term_family ) {    /* terminal family default */
	sprintf (kbfile_buf, "%s%s/%s%s",
		 xdir_dir, kbfilesdir, term_family, kbfile_postfix);
	if ( kbfcase_pt )
	    *kbfcase_pt = "package directory and terminal name";
    } else {    /* general purpose default */
	sprintf (kbfile_buf, "%s%s/%s",
		 xdir_dir, kbfilesdir, universal_kbf_name);
	if ( kbfcase_pt )
	    *kbfcase_pt = "package directory and all purpose kb file";
    }
    return (strlen (kbfile_buf));
}

static char * find_kbfile (int buf_sz, char * tname, char * cnfg_dir, char ** cnfg)
{
    memset (kbfile, 0, buf_sz);
    if ( ! cnfg_dir ) return (NULL);

    if ( ! *cnfg ) *cnfg = cnfg_dir;
    sprintf (kbfile, "%s/%s%s", cnfg_dir, tname, kbfile_postfix);
    if ( access (kbfile, R_OK) >= 0 ) return ("terminal name kb file");

    memset (kbfile, 0, buf_sz);
    sprintf (kbfile, "%s/%s", cnfg_dir, universal_kbf_name);
    if ( access (kbfile, R_OK) >= 0 ) return ("all purpose kb file");

    memset (kbfile, 0, buf_sz);
    return (NULL);
}


static void build_kbfile ()
{
    int i, buf_sz, sz;
    char *cnfg;

    /* get the terminal type default kbfile name */
    buf_sz = 0;
    kbfile = get_kbfile_buf (tname, &buf_sz);
    if ( ! kbfile ) return;

    cnfg = NULL;
    if ( buildflg ) {   /* debugging a new release of the editor : reversed priority */
	for ( i = cnfg_dir_rec_pt_sz -1 ; i >= 0 ; i-- ) {
	    if ( cnfg_dir_rec_pt[i] == NULL ) continue;
	    if ( ! cnfg_dir_rec_pt[i]->existing ) continue;
	    kbfcase = find_kbfile (buf_sz, tname, cnfg_dir_rec_pt[i]->path, &cnfg);
	    if ( kbfcase ) return;
	}
    } else {
	for ( i = 0 ; i < cnfg_dir_rec_pt_sz ; i++ ) {
	    if ( cnfg_dir_rec_pt[i] == NULL ) continue;
	    if ( ! cnfg_dir_rec_pt[i]->existing ) continue;
	    kbfcase = find_kbfile (buf_sz, tname, cnfg_dir_rec_pt[i]->path, &cnfg);
	    if ( kbfcase ) return;
	}
    }
    sprintf (kbfile, "%s/%s%s", cnfg, tname, kbfile_postfix);
    kbfcase = "file not existing or no read access";
}

/* get_kbfile_name : get or build the kbfile name */
/* ---------------------------------------------- */
static void get_kbfile_name () {

    kbfile = NULL;
    if ( stdkbdflg ) kbfcase = "-dkeyboard";    /* force use of the compiled standard */
    else if ( (kbfile = optkbfile) )
	kbfcase = ( access (kbfile, R_OK) < 0 ) ?
		    "-kbfile option (file not existing or no read access)"
		  : "-kbfile option";
    else if ( (kbfile = getenv ("EKBFILE")) )
	kbfcase = ( access (kbfile, R_OK) < 0 ) ?
		    "EKBFILE (file not existing or no read access)"
		  : "EKBFILE";
    else {
	/* build the default keyboard definition file name */
	build_kbfile ();
    }
}


#ifdef COMMENT
void
gettermtype ()
.
    Get the terminal type.  If Version 7, get the TERM environment variable.
    Copy the appropriate terminal and keyboard structures from the tables
    in e.tt.c into the terminal structure to be used for the session.
#endif
void
gettermtype ()
{
    extern int GetCccmd ();

    /* Get the selected terminal type */
    if (   !(tname = opttname)
	&& !(tname = getenv ("TERM"))
       )
	getout (YES, "No TERM environment variable or -terminal argument");
    Block {
	int ind;
	if ((ind = lookup (tname, termnames)) >= 0) {
	    kbdtype = termnames[ind].val; /* assume same as term for now */
	    kbfcase = "known internal terminal";
	}
	else
	    kbdtype = 0;

	if (   ind >= 0 /* we have the name */
#ifdef  TERMCAP
	    && !optdtermcap /* we aren't forcing the use of termcap */
	    && tterm[termnames[ind].val] != tterm[0] /* not a termcap type */
#endif
	    && tterm[termnames[ind].val] /* we have compiled-in code */
	   )
	    termtype = termnames[ind].val;
#ifdef  TERMCAP
	else {      /* use termcap or terminfo */
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
#else
	else
	    getout (YES, "Unknown terminal type: \"%s\"", tname);
#endif
    }

    /* Get the selected keyboard type */
    if ( stdkbdflg ) kname = stdk;  /* use the standard compiled */
    else if ( (kname = optkname) ) kbfcase = "\"-keyboard\" option";
    else if ( (kname = getenv ("EKBD")) ) kbfcase = "EKBD variable";
    if ( kname ) {
	Reg1 int ind;
	if (   (ind = lookup (kname, termnames)) >= 0
	    && tkbd[termnames[ind].val] /* we have compiled-in code for it */
	   )
	    kbdtype = termnames[ind].val;
	else
	    getout (YES, "Unknown keyboard type: \"%s\"", kname);
    }
    else if ( kbdtype )
	kname = tname;
    else
	kname = stdk;

    /* select the routines for the terminal type and keyboard type */
    move ((char *) tterm[termtype], (char *) &term, (Uint) sizeof (S_term));
    move ((char *) tkbd[kbdtype],   (char *) &kbd,  (Uint) sizeof (S_kbd));

    if (term.tt_width > MAXWIDTH)
	term.tt_width = MAXWIDTH;

#ifdef  KBFILE
    /** should integrate this stuff as a keyboard type */
    /** Don't document it until it is done that way */
    /* Get the keyboard file if specified */

    kbfile = NULL;  /* DEFAULT : use internal definition */
    if ( kbdtype == 0 ) get_kbfile_name ();

    if ( kbfile ) {
	extern int in_file();
	extern int nop ();

	if ( getkbfile (kbfile) ) kbd.kb_inlex = in_file;
	else kname = stdk;
	/* -------
	kbd.kb_init  = nop;
	kbd.kb_end   = nop;
	----- */
    }
#endif

    if ( GetCccmd () < 0 )
	getout (YES, "A control character (Ctrl A ... Ctrl Z) must be assigned to <cmd> key function");

    d_put (VCCINI);     /* initializes display image for d_write */
			/* and selects tt_height if selectable */

    /* initialize the keyboard */
    (*kbd.kb_init) ();

    Block {
	Reg1 int tmp;
	tmp = term.tt_naddr;
	tt_lt2 = term.tt_nleft  && 2 * term.tt_nleft  < tmp;
	tt_lt3 = term.tt_nleft  && 3 * term.tt_nleft  < tmp;
	tt_rt2 = term.tt_nright && 2 * term.tt_nright < tmp;
	tt_rt3 = term.tt_nright && 3 * term.tt_nright < tmp;
    }

    if (optbullets >= 0)
	borderbullets = optbullets;
    else if (borderbullets)
	borderbullets = term.tt_bullets;
    return;
}

#ifdef COMMENT
void
setitty ()
.
    Set the tty modes for the input tty.
#endif
void
setitty ()
{
#define BITS(start,yes,no) start  = ( (start| (yes) )&( ~(no) )  )
#ifdef SYSIII
    struct termio temp_termio;

#   ifdef CBREAK
	char ixon = cbreakflg;
#   else
	char ixon = NO;
#   endif
    if (ioctl(STDIN, TCGETA, &in_termio) < 0)
	return;
    temp_termio = in_termio;
    temp_termio.c_cc[VMIN]=1;
    temp_termio.c_cc[VTIME]=0;
    BITS(temp_termio.c_iflag,
	 IGNPAR|ISTRIP,
	 IGNBRK|BRKINT|PARMRK|INPCK|INLCR|IGNCR|ICRNL|IUCLC|IXOFF
	);
    if(ixon) temp_termio.c_iflag  |= IXON;
    else     temp_termio.c_iflag  &= ~IXON;
    BITS(temp_termio.c_lflag,NOFLSH,ISIG|ICANON|ECHO);
    if(ioctl(STDIN,TCSETAW,&temp_termio) >= 0) istyflg=YES;
    fcntlsave = fcntl(STDIN,F_GETFL,0);
    return;
#undef BITS
#else


    if (ioctl (STDIN, TIOCGETP, &instty) < 0)
	return;

#ifdef  CBREAK
#ifdef  TIOCGETC
    if (cbreakflg) Block {
	static struct tchars tchars = {
	    0xff,       /* char t_intrc;    * interrupt */
	    0xff,       /* char t_quitc;    * quit */
	    'Q' & 0x1f, /* char t_startc;   * start output */
	    'S' & 0x1f, /* char t_stopc;    * stop output */
	    0xff,       /* char t_eofc;     * end-of-file */
	    0xff,       /* char t_brkc;     * input delimiter (like nl) */
	};
	if (   ioctl (STDIN, TIOCGETC, &spchars) < 0
	    || tchars.t_startc != spchars.t_startc
	    || tchars.t_stopc  != spchars.t_stopc
	    || ioctl (STDIN, TIOCSETC, &tchars) < 0
	   )
	    cbreakflg = NO;
    }
#endif

#ifdef  TIOCGLTC
    if (cbreakflg) Block {
	static struct ltchars ltchars = {
	    0xff,       /* char t_suspc;    * stop process signal */
	    0xff,       /* char t_dsuspc;   * delayed stop process signal */
	    0xff,       /* char t_rprntc;   * reprint line */
	    0xff,       /* char t_flushc;   * flush output (toggles) */
	    0xff,       /* char t_werasc;   * word erase */
	    0xff,       /* char t_lnextc;   * literal next character */
	};
	if (   ioctl (STDIN, TIOCGLTC, &lspchars) < 0
	    || ioctl (STDIN, TIOCSLTC, &ltchars) < 0
	   ) {
	    (void) ioctl (STDIN, TIOCSETC, &spchars);
	    cbreakflg = NO;
	}
    }
#endif
#endif  /* CBREAK */

    Block {
	Reg1 int tmpflags;
	tmpflags = instty.sg_flags;
#ifdef  CBREAK
	if (cbreakflg)
	    instty.sg_flags = CBREAK | (instty.sg_flags & ~(ECHO | CRMOD));
	else
#endif
	    instty.sg_flags = RAW | (instty.sg_flags & ~(ECHO | CRMOD));
	if (ioctl (STDIN, TIOCSETP, &instty) >= 0)
	    istyflg = YES;
	instty.sg_flags = tmpflags;             /* all set up for cleanup */
    }
    return;
#endif /* SYSIII */
}

#ifdef COMMENT
void
setotty ()
.
    Set the tty modes for the output tty.
#endif
void
setotty ()
{
    set_term ();        /* set terminal character set */

#ifdef SYSIII
    if (ioctl (STDOUT, TCGETA, &out_termio) < 0)
#else
    if (ioctl (STDOUT, TIOCGETP, &outstty) < 0)
#endif /* SYSIII */
	fast = YES;
    else Block {
	Reg1 int i;
#ifdef MESG_NO
	struct stat statbuf;
#endif /* MESG_NO */

#ifdef SYSIII

#ifdef COMMENT
/*
The #define of speed below and the comparison, further below, of the
result with B4800 assume a monotinicity of the B's in the speeds they
represent which is true but undocumented for System 3.
In fact the system 3 B's are identical to the version 7 B's.
*/
#endif
#define SPEED ((out_termio.c_cflag)&CBAUD)
	i = out_termio.c_oflag;
	out_termio.c_oflag &= ~(OLCUC|ONLCR|OCRNL|ONOCR|ONLRET);
	if( (out_termio.c_oflag & TABDLY) == TAB3)
	    out_termio.c_oflag &= ~TABDLY;
	if(ioctl(STDOUT,TCSETA,&out_termio) >= 0) {
	    ostyflg = YES;
	    out_termio.c_oflag = i;  /* all set up for cleanup */
	}
#else /* - SYSIII */
#define SPEED (outstty.sg_ospeed)
	i = outstty.sg_flags;
	outstty.sg_flags &= ~CRMOD;
	if ((outstty.sg_flags & TBDELAY) == XTABS)
	    outstty.sg_flags &= ~TBDELAY;
	if (ioctl (STDOUT, TIOCSETP, &outstty) >= 0) {
	    ostyflg = YES;
	    outstty.sg_flags = i;             /* all set up for cleanup */
	}
#endif /* SYSIII */

#ifdef MESG_NO
	fstat (STDOUT, &statbuf);        /* save old tty message status and */
	oldttmode = statbuf.st_mode;
#ifdef TTYNAME
	if ((ttynstr = ttyname (STDOUT)) != NULL)
#endif
#ifdef TTYN
	if ((ttynstr[strlen (ttynstr) - 1] = ttyn (STDOUT)) != NULL)
#endif
	    if (ttynstr != NULL) chmod (ttynstr, 0600); /* turn off messages */
#endif
	fast = (ospeed = SPEED) >= B4800;
    }
    /* no border bullets if speed is slow */
    if (!fast && optbullets == -1)
	borderbullets = NO;
    return;
#undef SPEED
}

/* set_crlf : set nl -> nl cr on output to console */
/* ----------------------------------------------- */

int set_crlf ()
{
    int oflag;
#ifdef SYSIII
    struct termio termiobf;

    (void) ioctl (STDOUT, TCGETA, &termiobf);
    oflag = termiobf.c_oflag;
    termiobf.c_oflag |= (ONLCR);    /* map nl into cr nl */
    (void) ioctl (STDOUT, TCSETA, &termiobf);
#endif
    return (oflag);
}

/* reset_crlf : back to initial mode */
/* --------------------------------- */

void reset_crlf (int oflag)
{
#ifdef SYSIII
    struct termio termiobf;

    (void) ioctl (STDOUT, TCGETA, &termiobf);
    termiobf.c_oflag = oflag;
    (void) ioctl (STDOUT, TCSETA, &termiobf);
#endif
}


#ifdef COMMENT
void
makescrfile ()
.
    Initialize the #o and #c pseudo-files.
    Set up the qbuffers (pick, close, etc.).
#endif
void
makescrfile ()
{
    extern char *cwdfiledir [];
    int j;

    for (j = FIRSTFILE; j < FIRSTFILE + NTMPFILES; j++) {
	names[j] = append (tmpnames[j - FIRSTFILE], ""); /* must be freeable*/
	/*  the firstlas is used for appending lines, and a separate stream
	 *  is cloned for the lastlook wksp so that seeks on it won't disturb
	 *  the seek position of firstlas[j]
	 **/
	cwdfiledir [curfile] = NULL;
	if (!la_open ("", "nt", &fnlas[j]))
	    getout (YES, "can't open %s", names[j]);
	(void) la_clone (&fnlas[j], &lastlook[j].las);
	lastlook[j].wfile = j;
	fileflags[j] = INUSE | CANMODIFY;
    }

    for (j = 0; j < NQBUFS; j++)
	(void) la_clone (&fnlas[qtmpfn[j]], &qbuf[j].buflas);
    return;
}

#ifdef COMMENT
void
getstate (ichar)
    Char ichar;
.
    /*
    Set up things as per the state file.
    See State.fmt for a description of the state file format.
    The 'ichar' argument is still not totally cleaned up from the way
    this was originally written.
    If ichar ==
      ALL_WINDOWS use all windows and files from state file.
      ONE_WINDOW  set up one full-size window and don't put up any files.
    */
#endif
void
getstate (ichar)
Char ichar;
{
    Small winnum;
    Reg1 FILE *gbuf;
#ifdef LMCSTATE
    char *x;

    x = state ? statefile : rfile;
#endif
    if (   ichar == NO_WINDOWS
	|| notracks
#ifdef LMCSTATE
	|| (gbuf = fopen (x, "r")) == NULL
#else
	|| (gbuf = fopen (rfile, "r")) == NULL
#endif
       ) {
	makestate (gbuf == NULL);
	return;
    }

    Block {
	Reg2 Short i;
	if ( (i = getshort (gbuf)) != revision) {
	    if (i >= 0 || feoferr (gbuf))
		goto badstart;
	    if (i != -1)            /* always accept -1 */
		getout (YES,
			"Startup file: \"%s\" was made by revision %d of %s.\n\
Delete it or give a filename after the %s command.",
			rfile, -i, progname, progname);
	}
    }

    /* terminal type */
    Block {
	Reg2 Small nletters;
	nletters = getshort (gbuf);
	if (feoferr (gbuf))
	    goto badstart;
	getskip (nletters, gbuf);
    }

    Block {
	Reg2 Slines nlin;
	Reg3 Scols ncol;
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

    if (ntabs = getshort (gbuf)) Block {
	Reg2 int ind;
	if (feoferr (gbuf))
	    goto badstart;
	stabs = max (ntabs, NTABS);
	tabs = (ANcols *) salloc (stabs * sizeof *tabs, YES);
	for (ind = 0; ind < ntabs; ind++)
	    tabs[ind] = (ANcols) getshort (gbuf);
	if (feoferr (gbuf))
	    goto badstart;
    }

    linewidth = getshort (gbuf);

    Block {
	Reg2 Short i;
	if (i = getshort (gbuf)) {
	    if (feoferr (gbuf))
		goto badstart;
	    searchkey = salloc (i, YES);
	    fread (searchkey, 1, i, gbuf);
	    if (feoferr (gbuf))
		goto badstart;
	}
    }

    insmode = getc (gbuf);

    if (getc(gbuf) || patmode)	patmode	= YES;	/* Added Purdue	CS 10/8/82 MAB */

    if (getc (gbuf)) {  /* curmark */
	getskip (sizeof (long)
		 + sizeof (short)
		 + sizeof (char)
		 + sizeof (short),
		 gbuf);
    }

#ifdef LMCV19
#ifdef LMCAUTO
    autofill = getc (gbuf);
    autolmarg = getshort (gbuf);
    info (inf_auto, 2, autofill ? "WP" : "  ");
#else
    (void) getc (gbuf);
    (void) getshort (gbuf);
#endif
#endif

    nwinlist = getc (gbuf);
    if (ferror(gbuf) || nwinlist > MAXWINLIST)
	goto badstart;
    winnum = getc (gbuf);
    if (ichar != ALL_WINDOWS) Block {
	Reg3 Short  n;
	/* skip over windows until we get to the right window */
	for (n = Z; n < winnum; n++) {
	    getskip (sizeof (char)
		     + sizeof (char)
		     + sizeof (short)
		     + sizeof (char)
		     + sizeof (short)
#ifdef LMCV19
		     + sizeof (char)
#endif
		     , gbuf);
	    Block {
		Reg2 Short i;
		if (i = getshort (gbuf)) {
		    getskip (i, gbuf);
		    getskip (sizeof (long)
			     + sizeof (short)
			     + sizeof (char)
			     + sizeof (short),
			     gbuf);
		}
	    }
	    getskip (getshort (gbuf), gbuf);
	    getskip (sizeof (long)
		     + sizeof (short)
		     + sizeof (char)
		     + sizeof (short),
		     gbuf);
	    if (feoferr (gbuf))
		goto badstart;
	}
	winnum = 0;
	nwinlist = 1;
    }

    /* set up the windows */
    Block {
	Reg7 Small n;
	for (n = Z; n < nwinlist; n++) Block {
	    Reg6 Small gf;
	    Reg5 S_window *window;
	    window = winlist[n] = (S_window *) salloc (SWINDOW, YES);
	    window->prevwin = getc (gbuf);
	    if (ichar != ALL_WINDOWS)
		window->prevwin = 0;
	    Block {
		Reg2 Scols   lmarg;
		Reg3 Scols   rmarg;
		Reg4 Slines  tmarg;
		Slines  bmarg;
#ifdef LMCV19
		AFlag   winflgs;
#endif

		tmarg = getc (gbuf);
		lmarg = getshort (gbuf);
		bmarg = getc (gbuf);
		rmarg = getshort (gbuf);
#ifdef LMCV19
		winflgs = getc (gbuf);
#endif
		if (ichar != ALL_WINDOWS) {
		    tmarg = 0;
		    lmarg = 0;
		    bmarg = term.tt_height - 1 - NPARAMLINES;
		    rmarg = term.tt_width - 1;
#ifdef LMCSTATE
		    winflgs = 0;
#endif
		}
		defplline = getshort (gbuf);
		defmiline = getshort (gbuf);
		defplpage = getshort (gbuf);
		defmipage = getshort (gbuf);
		deflwin = getshort (gbuf);
		defrwin = getshort (gbuf);
#ifdef LMCSTATE
		setupwindow (window, lmarg, tmarg, rmarg, bmarg, winflgs, 1);
#else
		setupwindow (window, lmarg, tmarg, rmarg, bmarg, 1);
#endif
	    }
	    if (n != winnum)
		drawborders (winlist[n], WIN_INACTIVE);
	    switchwindow (window);
	    gf = 0;
	    Block {
		Reg3 Small nletters;
		if (nletters = getshort (gbuf)) {   /* there is an alternate file */
		    char *fname;
		    if (feoferr (gbuf))
			goto badstart;
		    fname = salloc (nletters, YES);
		    fread (fname, 1, nletters, gbuf);
		    Block {
			Reg2 Nlines  lin;
			Reg4 Ncols   col;
			Slines tmplin;
			Scols tmpcol;
			lin = getlong  (gbuf);
			col = getshort (gbuf);
			tmplin = getc (gbuf);
			tmpcol = getshort (gbuf);
			if (ichar != ONE_FILE) {
			    if (editfile (fname, col, lin, 0, NO) == 1)
				gf = 1;
			    /* this sets them up to get copied into
			       curwksp->ccol & clin */
			    poscursor (curwksp->ccol = tmpcol,
				       curwksp->clin = tmplin);
			}
			else
			    poscursor (0, 0);
		    }
		    /*  do this so that editfile won't be cute about
		     *  suppressing a putup on a file that is the same
		     *  as the altfile
		     **/
		    curfile = NULLFILE;
		}
	    }
	    Block {
		Reg4 char *fname;
		Block {
		    Reg2 Small nletters;
		    nletters = getshort (gbuf);
		    if (feoferr (gbuf))
			goto badstart;
		    fname = salloc (nletters, YES);
		    fread (fname, 1, nletters, gbuf);
		}
		Block {
		    Reg2 Nlines  lin;
		    Reg3 Ncols   col;
		    Slines tmplin;
		    Scols tmpcol;
		    lin = getlong  (gbuf);
		    col = getshort (gbuf);
		    tmplin = getc (gbuf);
		    tmpcol = getshort (gbuf);
		    if (feoferr (gbuf))
			goto badstart;
		    if (n != winnum)
			chgborders = 2;
		    if (editfile (fname, col, lin, 0, n == winnum ? NO : YES)
			== 1) {
			gf = 2;
			poscursor (curwksp->ccol = tmpcol,
				   curwksp->clin = tmplin);
		    }
		    chgborders = 1;
		}
	    }
	    if (gf < 2) {
		if (n != winnum)
		    chgborders = 2;
		if (gf == 0) {
		    eddeffile (n == winnum ? NO : YES);
		    curwksp->ccol = curwksp->clin = 0;
		}
		else if (n != winnum)
		    putupwin ();
		chgborders = 1;
	    }
	}
    }

    if ( feoferr (gbuf))
badstart:
	getout (YES, "Bad state file: \"%s\"\nDelete it and try again.",
		 rfile);

    drawborders (winlist[winnum], WIN_ACTIVE);
    switchwindow (winlist[winnum]);
    infotrack (winlist[winnum]->winflgs & TRACKSET);
    poscursor (curwksp->ccol, curwksp->clin);
    fclose (gbuf);
    return;
}

#ifdef COMMENT
void
getskip (num, file)
    Reg2 int num;
    Reg1 FILE *file;
.
    Skip 'num' bytes in 'file'.  'Num' must be > 0.
#endif
void
getskip (num, file)
Reg1 int num;
Reg2 FILE *file;
{
    do {
	getc (file);
    } while (--num);
    return;
}

/* ------ default tabs setting ------ */
static int default_tabs = DEFAULT_TABS;


#ifdef COMMENT
void
makestate (nofileflg)
    Flag nofileflg;
.
    Make an initial state without reference to the state file.
    If 'nofileflg' != 0, edit the 'scratch' file.
#endif
void
makestate (nofileflg)
Flag nofileflg;
{
    Reg1 S_window *window;

    nwinlist = 1;
    window = winlist[0] = (S_window *) salloc (SWINDOW, YES);

    stabs = NTABS;
    tabs = (ANcols *) salloc (stabs * sizeof *tabs,YES);
    ntabs = 0;

    setupwindow (winlist[0], 0, 0,
#ifdef LMCSTATE
		 term.tt_width - 1, term.tt_height - 1 - NPARAMLINES, 0, 1);
#else
		 term.tt_width - 1, term.tt_height - 1 - NPARAMLINES, 1);
#endif
    switchwindow (window);              /* switched this and next line */
    tabevery ((Ncols) default_tabs, (Ncols) 0, (Ncols) (((NTABS / 2) - 1) * default_tabs),
	      YES);
    drawborders (window, WIN_ACTIVE);
    poscursor (0, 0);
    if (nofileflg) {
	edscrfile (NO);
	poscursor (0, 0);
    }
    return;
}

#ifdef LMCAUTO
#ifdef COMMENT
void
infoint0 ()
.
    Set up initial parameters for the info line.
#endif
void
infoint0 ()
{
    Reg1 Scols col;

    inf_insert = 0; col = 7;    /* "INSERT" */

    inf_track = col; col += 6;  /* "TRACK" */

    inf_range = col; col += 7;  /* "=RANGE" */

    inf_pat = col; col += 3;    /* "RE" */

    inf_auto = col; col += 3;   /* "WP" */

    inf_mark = col; col += 5;   /* "MARK" */

    inf_area = col; col += 7;   /* "30x16" etc. */

    inf_at = col; col += 3;     /* "At"         */
    inf_line = col; col += 6;   /* line         */
    inf_in = col; col += 3;     /* "in"         */
    inf_file = col;             /* filename     */
}

#ifdef COMMENT
void
infoinit ()
.
    Set up the displays on the info line.
#endif
void
infoinit ()
{
#else
#ifdef COMMENT
void
infoinit ()
.
    Set up the displays on the info line.
#endif
void
infoinit ()
{
    Reg1 Scols col;

    inf_insert = 0; col = 8;    /* "INSERT" */

    inf_track = col; col += 7;  /* "TRACK" */

    inf_range = col; col += 8;  /* "=RANGE" */

    inf_pat = col; col += 3;    /* "RE" Added 10/18/82 MAB */

    inf_mark = col; col += 5;   /* "MARK" */

    inf_area = col;             /* "30x16" etc. */

    col = 42;
    inf_at = col; col += 3;     /* "At"         */
    inf_line = col; col += 6;   /* line         */
    inf_in = col; col += 3;     /* "in"         */
    inf_file = col;             /* filename     */
#endif

    info (inf_at, 2, "At");
    info (inf_in, 2, "in");
    infoline = -1;
    infofile = NULLFILE;
    if (insmode) {
	insmode = NO;
	tglinsmode ();
    }
    if (patmode) {
	patmode = NO;
	tglpatmode ();
    }
}

static void clean_all (Flag filclean)
{
    fixtty ();
    if (windowsup)
	screenexit (YES);
    if (filclean && !crashed)
	cleanup (YES, NO);
    if (keysmoved)
	mv (bkeytmp, keytmp);
    d_put (0);
}

static void exit_now (int status)
{
#ifdef PROFILE
    monexit (status);
#else
    exit (status);
#endif
    /* NOTREACHED */
}


#ifdef COMMENT
void
getout (filclean, str, a1,a2,a3,a4,a5,a6)
    Flag filclean;
    char *str;
.
    This is called to exit from the editor for conditions that arise before
    entering mainloop().
#endif
/* VARARGS2 */
void
getout (filclean, str, a1,a2,a3,a4,a5,a6)
Flag filclean;
char   *str;
long a1,a2,a3,a4,a5,a6;
{
    extern char verstr[];

    clean_all (filclean);
    if (*str)
	fprintf (stdout, str, a1,a2,a3,a4,a5,a6);

    if ( helpflg ) {
	/* do not exit during a -help option : check all options */
	printf ("\n  --- %s : error during initialization ---\n", progname);
	return;
    }
    printf ("\nThis is %s : Rand editor version %d release %d\n%s\n",
	    prog_fname ? prog_fname : progname, -revision, subrev, verstr);

    exit_now (-1);
    /* NOTREACHED */
}
