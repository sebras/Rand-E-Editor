/*
/* file e.h - "include" file used by all
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#define BULLETS         /* border bullets */

#ifdef UNIXV7
#include <sys/types.h>
#endif

#include <c_env.h>
#include <localenv.h>

#include <ff.h>     /* stdio.h is included by ff.h */

/* IMPORTANT: NOFILE is defined in ff.h -- BE SURE is the true number of
/* file descriptors you are allowed to have open! */

extern char *sprintf ();

#define Block

/* phototypesetter v7 c compiler has a bug with items declared as chars
/* If the variable is used as an index i.e. inside [], then it is likely
/* to get movb'ed to a register, which is then used as the index,
/* including the undefined upper byte.  Also true of things declared
/* "register char"
/* By assigning "Z" to the register, a sign extension will occur
/**/

/* Char and Short are used to point out the minimum storage required for
/*   the item.  It may be that for a given compiler, int will take up no
/*   more storage and actually execute faster for one or both of these,
/*   so they should be defined to be ints for that case.
/*   Especially, note that defining these to be ints gets around bugs in
/*   both the Ritchie PDP-11 compiler and the Johnson VAX compiler regarding
/*   declaring types smaller than int in registers.
/**/
#define Char  int
#define Short int

#define Z 0  /* define to "zero" for the VAX compiler bug if Char and Short */
	     /* above aren't both defined to int */
#include "e.t.h"

/*#define Void int              /* function with no returned value */
#define Void short
/* For each type there is Type and Atype.
/* use AType for array and structure member declarations,
/* and use Type everywhere else
/**/
typedef Char  Flag;             /* YES or NO */
typedef char  AFlag;            /* YES or NO */
typedef Char  Small;            /* small integer that will fit into char */
typedef char  ASmall;           /* small integer that will fit into char */
typedef Short Nlines;           /* number of lines in a file */
typedef short ANlines;          /* number of lines in a file */
typedef Short Ncols;            /* number of columns in a line */
typedef short ANcols;           /* number of columns in a line */
typedef Char  Fd;               /* unix file descriptor */
typedef char  AFd;              /* unix file descriptor */
typedef Char  Fn;               /* index into files we are editing */
typedef char  AFn;              /* index into files we are editing */
#ifdef UNSCHAR
# define UC unsigned
#else
# define UC
#endif

/*
#ifdef UNIXV7
#define SIGARG
#endif
*/
#ifdef UNIXV6
#define wait(a) waita (a)
#define sgttyb      sgtty
#define xsgttyb     xsgtty
#define sg_ispeed   sg_ispd
#define sg_ospeed   sg_ospd
#define sg_flags    sg_flag
#define ALLDELAY (NLDELAY | TBDELAY | CRDELAY | VTDELAY)
#define B4800       12
#define st_mode  st_flags
#define S_IREAD  IREAD
#define S_IWRITE IWRITE
#define S_IEXEC  IEXEC
#define S_IFMT   IFMT
#define S_IFDIR  IFDIR
#define S_IFCHR  IFCHR
#define S_IFBLK  IFBLK
#define S_IFREG  0
#define SIGKILL  SIGKIL
#define SIGALRM  SIGCLK
#define SIGQUIT  SIGQIT
#endif

#define FNSTRLEN 14             /* max allowable filename string length */

# ifdef UNIXV6
# define ENAME "/bin/e" /* this will get exec'ed after forked shell */
# endif

#define abs(a) ((a)<0?(-(a)):(a))
#define min(a,b) (((a) < (b))? (a): (b))
#define max(a,b) (((a) > (b))? (a): (b))
#define Goret(a) {retval= a;goto ret;}
#ifndef YES
#define YES 1
#define NO  0
#endif
#define feoferr(p)	   (((p)->_flag&(_IOERR|_IOEOF))!=0)

/* fine MESG_NO                 /* if messages to be disallowed during edit */
/* #define SIGARG               /* if signal# passed as arg to signal(SIG) */
#define LOGINNAME "E"
/* #define CLEARONEXIT          /* if defined, clears screen on exit    */
/* #define DEBUGDEF             /* misc debugging stuff                 */
#ifndef DEBUGDEF
# define sfree(c) free (c)
#endif
#define CATCHSIGS		/* ignore signals */
#define DUMPONFATAL		/* dump always on call to fatal (..)	*/
#define FATALMEM 0              /* out of memory */
#define FATALIO  1              /* fatal I/O err */
#define FATALSIG 2              /* signal caught */
#define FATALBUG 3              /* bug */
#define DEBUGFILE   "e.dbg"
#define CTRLCHAR   (key < 040 || 0177 <= key)
#define PGMOTION (key==CCPLPAGE||key==CCPLLINE||key==CCMIPAGE||key==CCMILINE)

#define NHORIZBORDERS 2 	/* number of horiz borders per window	*/
#define SMOOTHOUT		/* if defined, try to smooth output	*/
				/* to terminal by doing flushing at	*/
				/* line boundaries near end of buffer	*/
#define TABCOL 8                /* num of cols per tab character        */

/* infoport columns */
#define INFOINSERT 0                /* "INSERT"     */
#define INFOREGION (INFOINSERT + 8) /* "REGION"   */
#define INFOMARK (INFOREGION + 7)   /* "MARK"       */
#define INFOAREA (INFOMARK + 5)     /* e.g. "30" or "30x16" */

#define INFOAT 42		/* "At" 	*/
#define INFOLINE (INFOAT + 3)   /* line         */
#define INFOIN (INFOLINE + 6)   /* "in"         */
#define INFOFILE (INFOIN + 3)   /* filename     */

#define NENTERLINES 1		/* number of ENTER lines on screen	*/
#define NINFOLINES 1		/* number of ENTER lines on screen	*/
#define NPARAMLINES (NENTERLINES + NINFOLINES)

#define FILEMODE 0644		/* mode of editor-created files 	*/
#define MAXTYP 25		/* # of modifying chars typed before	*/
				/* keyfile buffer is flushed		*/
#define DPLYPOL 4		/* how often to poll dintrup		*/
#define SRCHPOL 50		/* how often to poll sintrup		*/
#define EXECTIM 3		/* alarm time limit for Exec function	*/

#define NTABS 80

#define UP 1			/* Up		*/
#define DN 2			/* Down 	*/
#define RN 3			/* Return	*/
#define HO 4			/* Home 	*/
#define RT 5			/* Right	*/
#define LT 6			/* Left 	*/
#define TB 7			/* Tab		*/
#define BT 8			/* Backtab	*/

/* switches for mesg() note: rightmost 3 bits are reserved for arg count */
#define TELSTRT 0010
#define TELSTOP 0020            /* no more to write     */
#define TELCLR  0040            /* clear rest of line   */
#define TELDONE 0060
#define TELALL  0070
#define TELERR  0100
#define ERRSTRT 0110
#define ERRSTOP 0120            /* no more to write     */
#define ERRCLR  0140            /* clear rest of line   */
#define ERRDONE 0160
#define ERRALL  0170

/* fsd - file segment descriptor.  describes 1 to 127 contiguous lines
	on a file.  this is the atomic workspace component.
/**/
typedef struct fsd S_fsd;

/* workspace - two per window: wksp and altwksp
/**/
typedef struct workspace
{
    S_fsd      *curfsd;         /* ptr to current line's fsd            */
    ANlines curlno;             /* current line no.                     */
    ANlines curflno;            /* line no of 1st line in curfsd        */
    AFn      wfile;             /* channel number of file - 0 if none   */
    ASlines clin;               /* cursorline when inactive             */
    AScols   ccol;              /* curorcol when inactive               */
    ANlines wlin;               /* line no of ulhc of screen            */
    ANcols  wcol;               /* col no of column 0 of screen         */
} S_wksp;

extern
S_wksp  *curwksp;


extern
S_wksp  *pickwksp;


/* The routine "read2" in e.t.c that gets characters for the editor
/* reads NREAD characters from the keyboard into a char array and then
/* takes them out one at a time.  If a macro is being executed, then
/* read2 is pointed at a char array that contains the keystrokes of the
/* macro.  When the macro buffer is exhausted, then read2 goes back to
/* reading the previous buffer; thus macros can be nested.
/**/
#define NREAD 32

typedef struct inp
{   int         icnt;   /* no. of chars left in the buffer */
    char       *iptr;   /* pointer into the current position in the buffer */
    char       *ibase;  /* where the base of the buffer is */
    struct inp *iprevinp;   /* pointer to previous structure for nesting */
} S_inp;

extern char  keybuf[NREAD];
extern S_inp keyinp;   /* key input structure - always around */

#define INSTREAM    0
#define OUTSTREAM   1
#define ERRSTREAM   2
/*#define MAXSTREAMS NOFILE */
#define MAXSTREAMS 10
/* We need some channels for overhead stuff:
/*  0 - keyboard input
/*  1 - screen output
/*      change file
/*      fsd file (future feature of la package)
/*      keystroke file
/*      replay input file
/*      pipe[0]
/*      pipe[1]
/**/
#define WORKFILES 9
#define MAXOPENS (MAXSTREAMS - WORKFILES)
#define MAXFILES (MAXOPENS + 10)


extern Fd nopens;

/* There is an entry in each of these arrays for every file we have open
/* for editing.  The correct type for an array index is an Fn.
/**/
#define NULLFILE  0     /* This is handy since workspaces are calloc-ed */
#define CHGFILE   1
#define PICKFILE  2     /* file where picks go. Gets special */
			/* consideration: can't be renamed, etc.        */
#define PKCLFILE  2     /* file where picks and closes go. Gets special */
			/* consideration: can't be renamed, etc.        */
#define NTMPFILES 1

#define FIRSTFILE PICKFILE

extern S_fsd        *openfsds[];
extern Ff_stream    *openffs[];     /* contains ff_ handle                 */
extern ANlines       nlines[];      /* line number of last non-blank line  */
extern char         *names[];       /* current name of the file            */
extern char         *oldnames[];    /* if != 0, orig name before renamed   */
extern char         *savefnames[];  /* name of temp file where save went   */
extern S_wksp        lastlook[];
extern short         fileflags[];
#define INUSE        1              /* this array element is in use        */
#define DWRITEABLE   2              /* directory is writeable              */
#define FWRITEABLE   4              /* file is writeable                   */
#define CANMODIFY  010              /* ok to modify the file               */
#define INPLACE    020              /* to be saved in place                */
#define EDITED     040              /* edited                              */
#define SAVED     0100              /* was saved during the session        */
			/* A file can have no more than one of the
			/* following three bits set.
			/* The same name can appear in more than one fn,
			/*  but only in the following combinations:
			/*  names[i] (DELETED)    == names[j] (NEW)
			/*  names[i] (DELETED)    == names[j] (RENAMED)
			/*  oldnames[j] (RENAMED) == names[j] (NEW)
			/* If (NEW | DELETED | RENAMED) == 0
			/*   file exists and we are using it               */
#define NEW       0200  /* doesn't exist yet, we want to create it         */
#define DELETED   0400  /* exists, and we'll delete it on exit             */
#define RENAMED  01000  /* exists and we will rename it on exit            */
#define UPDATE   02000  /* update this file on exit */

extern
Fn      curfile;

/* This array is indexed by unix file descriptor */
extern Ff_stream *ffchans[];

/*
 viewport - describes a viewing window with file
   all marg values, and ltext and ttext, are limits relative
	to (0,0) = ulhc of screen.  the other six limits are
	relative to ltext and ttext.
/**/
typedef struct viewport
{
    S_wksp *wksp;               /* workspace window is showing          */
    S_wksp *altwksp;            /* alternate workspace                  */
    ASmall  prevport;           /* number of the ancester port          */
				/* boundaries of text within viewport	*/
				/*  may be same as or one inside margins*/
    ASlines ttext;              /* rel to top of full screen            */
    AScols   ltext;             /* rel to left of full screen           */
    AScols   rtext;             /*  = width - 1                         */
    ASlines btext;              /*  = height - 1                        */
    ASlines tmarg;              /*  rel to upper left of full screen    */
    AScols   lmarg;             /* margins                              */
    AScols   rmarg;
    ASlines bmarg;
    ASlines tedit;
    AScols   ledit;             /* edit window limits on screen         */
    AScols   redit;
    ASlines bedit;
    AScols  *firstcol;          /* first col containing nonblank        */
    AScols  *lastcol;           /* last col containing nonblank         */
    char   *lmchars;		/* left margin characters		*/
    char   *rmchars;		/* right margin characters		*/
} S_window;

#define SVIEWPORT (sizeof (S_window)) /* size in bytes of viewport */

#define MAXPORTLIST 10
extern
S_window       *portlist[MAXPORTLIST],
	       *curport,	/* current editing port 		*/
		wholescreen,	/* whole screen 			*/
		infoport,	/* port for info			*/
		enterport;	/* port for CMD and ARG 		*/
extern
Small   nportlist;

#define COLMOVED    8
#define LINMOVED    4
#define WCOLMOVED   2
#define WLINMOVED   1
#define WINMOVED    (WCOLMOVED | WLINMOVED)
#define CURSMOVED   (COLMOVED | LINMOVED)

/*	savebuf - structure that describes a pick or delete buffer	*/

typedef struct savebuf
{
    ANlines linenum;           /* the first line number in pickfile    */
    ANcols  ncolumns;
    ANlines nlins;
} S_svbuf;

extern
S_svbuf *pickbuf,
	*closebuf,
	*erasebuf;
extern
S_svbuf pb,
	cb,
	eb;

/* input control character assignments */

#define CCCMD		000 /* <BREAK > enter parameter 	*/
#define CCLPORT 	001 /* <cntr A> port left		*/
#define CCSETFILE	002 /* <cntr B> set file		*/
#define CCINT		003 /* <cntr C> interrupt	    *** was chgwin */
#define CCOPEN		004 /* <cntr D> insert			*/
#define CCMISRCH	005 /* <cntr E> minus search		*/
#define CCCLOSE 	006 /* <cntr F> delete			*/
#define CCMARK		007 /* <cntr G> mark a spot	    *** was PUT */
#define CCMOVELEFT	010 /* <cntr H> backspace		*/
#define CCTAB		011 /* <cntr I> tab			*/
#define CCMOVEDOWN      012 /* <cntr J> move down 1 line        */
#define CCHOME		013 /* <cntr K> home cursor		*/
#define CCPICK		014 /* <cntr L> pick			*/
#define CCRETURN        015 /* <cntr M> return                  */
#define CCMOVEUP	016 /* <cntr N> move up 1 lin		*/
#define CCINSMODE	017 /* <cntr O> insert mode		*/
#define CCREPLACE       020 /* <cntr P> replace             *** was GOTO */
#define CCMIPAGE        021 /* <cntr Q> minus a page            */
#define CCPLSRCH	022 /* <cntr R> plus search		*/
#define CCRPORT 	023 /* <cntr S> port right		*/
#define CCPLLINE	024 /* <cntr T> minus a line		*/
#define CCDELCH 	025 /* <cntr U> character delete	*/
#define CCUNAS2 	026 /* <cntr V> -- not assigned --  *** was SAVE */
#define CCMILINE        027 /* <cntr W> plus a line             */
#define CCUNAS1 	030 /* <cntr X> -- not assigned --  *** was EXEC */
#define CCPLPAGE        031 /* <cntr Y> plus a page             */
#define CCCHPORT	032 /* <cntr Z> change port	    *** was WINDOW */
#define CCTABS		033 /* <cntr [> set tabs		*/
#define CCCTRLQUOTE	034 /* <cntr \> knockdown next char	*/
#define CCBACKTAB	035 /* <cntr ]> tab left		*/
#define CCBACKSPACE	036 /* <cntr ^> backspace and erase	*/
#define CCMOVERIGHT	037 /* <cntr _> forward move		*/
#define CCDEL	       0177 /* <DEL>	-- not assigned --  *** was EXIT */
#define CCERASE        0201 /* *A erase                 */
#define CCLWORD        0202 /* *B move left one word    */
#define CCRWORD        0203 /* *C move right one word   */

extern
Scols   cursorcol;              /* physical screen position of cursor   */
extern
Slines  cursorline;             /*  from (0,0)=ulhc of text in port     */
extern Slines newcurline;       /* what cursorline will be when         */
				/* function gets done (= -1 if same)    */
extern Scols  newcurcol;        /* similar to newcurline                */

/* chgborders is set outside putup (), then looked at by putup ()
/*                              then putup resets it to 1 */
extern Small chgborders;        /* 0: don't change the borders          */
				/* 1: update them */
				/* 2: set them to inactive (dots) */

extern
Small   numtyp;                 /* number of text chars since last      */
				/* keyfile flush			*/

#define MAXMOTIONS 32
extern ASmall cntlmotions[MAXMOTIONS];

extern
char   *myname,
       *mypath,
       *progname;
extern
Flag	inplace;		/* do in-place file updates?		*/
extern ANcols *tabs;            /* array of tabstops */
extern Short   stabs;           /* number of tabs we have alloced room for */
extern Short   ntabs;           /* number of tabs set */
extern
char    *blanks;

#define NOCHAR 0400
extern unsigned Short getkey ();
extern Char key;             /* last char read from tty */
extern Flag keyused;         /* last char read from tty has been used */

/* default parameters */
extern Small defplline,         /* default plus a line          */
	     defplpage, 	/* default minus a line 	*/
	     defmiline, 	/* default plus a page		*/
	     defmipage, 	/* default minus a page 	*/
	     deflport,		/* default port left		*/
	     defrport,		/* default port right		*/
	     definsert, 	/* default insert		*/
	     defdelete, 	/* default delete		*/
	     defpick;		/* default pick 		*/
extern char  deffile[]; 	/* default filename		*/
extern Fn    deffn;             /* file descriptor of default file      */
extern Short deflinewidth;      /* default width for just, fill, center */

extern
Short   linewidth,              /* used by just, fill, center           */
	tmplinewidth;		/* used on return from parsing "width=xx" */

extern
Flag    errsw;                  /* is YES if error; used to clear cmd line */
extern
Flag    beepsw;                 /* is YES if bell on clearing cmd line */
extern
char   *paramv;                 /* * globals for param read routine     */
extern
Small   paramtype;

/* array to hold current line being edited */
/* tabs are expanded on input. */
extern
char   *cline;			/* array holding current line		*/
extern
Short   lcline,                 /* length of cline buffer               */
	ncline,                 /* number of chars in current line      */
	icline;                 /* increment for next expansion         */
extern
Flag    fcline,                 /* flag - has line been changed ?       */
	ecline,                 /* line contains DEL escapes            */
	xcline;                 /* cline was beyond end of file         */
extern
Nlines	clineno;		/* line number in workspace of cline	*/

extern char
	prebak[],		/* text to precede/follow		*/
	postbak[];		/* original name for backup file	*/

extern
char *searchkey;

#ifdef UNIXV7
extern short
#endif
#ifdef UNIXV6
extern char
#endif
    userid,
    groupid;

extern
char   *tmpname;                /* name of change file                  */

extern
FILE   *keyfile;		/* channel number of keystroke file	*/

extern
Fd	inputfile;		/* channel number of command input file */
extern
Small   intrupnum;              /* how often to call intrup             */
extern
Void    alarmproc ();
extern
Flag	alarmed;

#define NUMFFBUFS	10
extern
char    ffblock[][sizeof (Ff_buf)];     /* pre-allocated ff_ buffers */

extern char putscbuf[];

extern Flag windowsup;	 /* screen is in use for windows */

#define d_put(c) (putscbuf[0]=(c),d_write(putscbuf,1))

extern char saveftmp[]; 	/* first part of save temp names	*/

extern
FILE   *dbgfile;

extern short revision;  /* revision number of this e */
extern short subrev;    /* sub-revision number of this e */

extern
Char evrsn;   /* the character used in the chng, strt, & keys filename   */
		/* '0', '1', ...	*/

extern Flag notracks,   /* don't use or leave any strt file */
	    norecover,
	    replaying,
	    recovering,
	    silent;     /* replaying silently */
extern long nreplaykeys;/* number of keys in replay file */
extern Flag keysmoved;  /* keys file has been moved to backup */

/* these used to be in e.c only */

Flag breakfsd ();
S_fsd   *ldelete (),
	*pick (),
	*copyfsd (),
	*filtofsd (),
	*writemp (),
	*blanklines ();
extern
Void    sig (), igsig ();
extern char
	*gsalloc (),
	*salloc (),
	*append (),
	*copy (),
	*s2i (),
	*getword (),
	*getmypath ();
extern Short
	dechars ();

extern
Flag cmdmode;
extern
Flag imodesw;           /* is YES when in insertmode                      */
extern
Nlines parmlines;	   /* lines in numeric arg			   */
extern
Short parmcols;           /* columns in numeric arg e.g. 8 in "45x8"      */

extern
char *shargs[];
extern
char *shpath;

typedef struct lookuptbl
{   char *str;
    short val;
} S_looktbl;

extern
long strttime;	/* time of start of session */

extern
Flag loginflg;  /* = 1 if this is a login process */

extern Flag ischild;    /* YES if this is a child process */

extern int zero;
