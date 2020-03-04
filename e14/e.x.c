/*
/* file e.x.c: external definitions
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.fsd.h"
#include "e.m.h"
#include "e.sg.h"

/* buffer for reading in keys */
char keybuf[NREAD];
S_inp keyinp = {0, keybuf, keybuf, 0};


Fd nopens;

S_fsd     *openfsds[MAXFILES];
Ff_stream *openffs[MAXFILES];   /* contains ff_ handle                */
ANlines    nlines[MAXFILES];    /* line number of last non-blank line */

char      *names[MAXFILES];
char      *oldnames[MAXFILES];
char      *savefnames[MAXFILES];/* name of temp file where save went  */
S_wksp     lastlook[MAXFILES];
short      fileflags[MAXFILES];

Ff_stream *ffchans[MAXSTREAMS];

FILE   *dbgfile;

S_wksp  *curwksp;


S_wksp  *pickwksp;


Fn        curfile;

S_window       *portlist[MAXPORTLIST],
	       *curport,	/* current editing port 		*/
		wholescreen,	/* whole screen 			*/
		infoport,	/* port for info			*/
		enterport;	/* port for CMD and ARG 		*/
Small   nportlist;

S_svbuf *pickbuf,
	*closebuf,
	*erasebuf;
S_svbuf pb,
	cb,
	eb;

Scols   cursorcol;              /* physical screen position of cursor   */
Slines  cursorline;             /*  from (0,0)=ulhc of text in port     */
Slines  newcurline = -1;        /* what cursorline will be when         */
				/* function gets done (= -1 if same     */
Scols   newcurcol  = -1;        /* similar to newcurline                */

Small   chgborders = 1;

Small   numtyp;                 /* number of text chars since last      */
				/* keyfile flush			*/

/* table of motions for control chars
	UP  1	up
	DN  2	down		 : 4 or less change cursorline
	RN  3	carriage return
	HO  4	home
	RT  5	right
	LT  6	left
	TB  7	tab
	BT  8	backtab
/**/
ASmall  cntlmotions[MAXMOTIONS] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    LT, TB, DN, HO, 0, RN, UP, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, BT, 0, RT,
};

char   *myname,
       *mypath,
       *progname;
Flag	inplace;		/* do in-place file updates?		*/
ANcols *tabs;                   /* array of tabstops */
Short   stabs = NTABS;          /* number of tabs we have alloced room for */
Short   ntabs = NTABS / 2;      /* number of set tabs */

char    *blanks;

Char    key;                /* last char read from tty */
Flag    keyused = YES;      /* last char read from tty has been used. */

/* parameters for line motion buttons */
Small   defplline = 10,         /* default plus a line         */
	defmiline = 10,         /* default minus a line        */
	defplpage = 1,          /* default plus a page         */
	defmipage = 1,          /* default minus a page        */
	deflport  = 16,         /* default port left           */
	defrport  = 16,         /* default port right          */
	definsert = 1,          /* default insert              */
	defdelete = 1,          /* default delete              */
	defpick   = 1;          /* default pick                */
char    deffile[] = "/etc/e/errmsg"; /* default filename            */
Fn      deffn     = -1;         /* file descriptor of default file */
Short   deflinewidth = 75;      /* default width for just, fill, center */


Short   linewidth    = 75;      /* used by just, fill, center */
Short   tmplinewidth;           /* used on return from parsing "width=xx" */


Flag    errsw;                  /* is YES if error; used to clear cmd line */
Flag    beepsw;                 /* YES to beep on clearing cmd line */

char   *paramv;                 /* * globals for param read routine     */
Small   paramtype;





/* initialize cline to be empty */
char   *cline;			/* array holding current line		*/

Short   lcline,                 /* length of cline buffer               */
	ncline;                 /* number of chars in current line      */
Short   icline = 100;           /* initial increment for expansion */

Flag    fcline,                 /* flag - has line been changed ?       */
	ecline,                 /* line contains DEL escapes            */
	xcline;                 /* cline was beyond end of file         */

Nlines  clineno = -2;           /* line number in workspace of cline    */

char    prebak[]  = ",",        /* text to precede/follow         */
	postbak[] = 0;          /* original name for backup file  */


char *searchkey;


#ifdef UNIXV7
short
#endif

#ifdef UNIXV6
char
#endif

    userid,
    groupid;

char   *tmpname;                /* name of change file                  */

FILE   *keyfile;		/* channel number of keystroke file	*/


Fd	inputfile;		/* channel number of command input file */

Small   intrupnum;              /* how often to call intrup             */



Flag	alarmed;



char    ffblock[NUMFFBUFS][sizeof (Ff_buf)];
				/* pre-allocated ff_ buffers		*/

char    putscbuf[10];

Flag windowsup = 0;

char saveftmp[] = ",esave";    /* first part of save temp names     */


FILE   *dbgfile = NULL;

Char evrsn;   /* the character used in the chng, strt, & keys filename   */
		/* '0', '1', ...	*/


Flag notracks = 0;
Flag norecover = 0;
Flag replaying = 0;
Flag recovering = 0;
Flag silent;     /* replaying silently */
long nreplaykeys;/* number of keys in replay file */
Flag keysmoved;  /* keys file has been moved to backup */

/************/
/* e.fn.h */
/* pathnames for standard files */
char   *tmppath   = "/tmp/etmp/"; /* the x will be replaced with */
#ifdef UNIXV7
char   *ttynstr;
#endif
#ifdef UNIXV6
char   *ttynstr   = "/dev/tty ";
#endif
char    scratch[] = "scratch";

char    tmpnstr[] = "c1";    /* the x will be replaced with */
char    keystr[]  = "k1";    /* a digit  */
char    bkeystr[] = "k1b";
char    rstr[]    = "s1";

char   *keytmp,
       *bkeytmp,
       *rfile,          /* strt file name and backup name */
       *inpfname;

/************/
/* e.sg.h */
struct sgttyb instty,
	      outstty;

#ifdef V6XSET
struct xsgttyb inxstty;
#endif

Flag istyflg,
     ostyflg;

unsigned Short oldttmode;

/************/

Flag cmdmode;

Flag imodesw;		/* is 1 when in insertmode			*/

Nlines parmlines;	   /* lines in numeric arg			   */

Short parmcols;           /* columns in numeric arg e.g. 8 in "45x8"      */

char *shargs[] = {"sh", "-c", 0, 0};

char *shpath;

long strttime;	/* time of start of session */

Flag loginflg;  /* = 1 if this is a login process */

Flag ischild;

Flag    entfstline = 0; /* says write out entire first line */

#ifdef SIGARG
char    *signames[] = {
	0,
	"Hangup",
	"Interrupt",
	"Quit",
	"Illegal instruction",
	"Trace/BPT trap",
	"IOT trap",
	"EMT trap",
	"Floating exception",
	0,
	"Bus error",
	"Memory fault",
	"Bad system call",
	"Pipe fault",
	0,
	0,
	0,
	0,
	0,
	0,
};
#endif

/************/
/* e.m.h */
char *cmdname;           /* the full name of the command given */
char *cmdopstr;
char *opstr;
char *nxtop;

S_looktbl cmdtable[] = {
    "-close"  , CMD_CLOSE   ,
    "-command", CMD_COMMAND ,
    "-erase"  , CMD_ERASE   ,
    "-join"   , CMDSPLIT    ,
    "-macro"  , CMD_MACRO   ,
    "-mark"   , CMD_MARK    ,
    "-pick"   , CMD_PICK    ,
    "-region" , CMD_REGION  ,
    "-replace", CMD_REPLACE ,
    "-search" , CMD_SEARCH  ,
    "-split"  , CMDJOIN     ,
    "-tab"    , CMD_TAB     ,
    "-tabfile", CMD_TABFILE ,
    "-tabs"   , CMD_TABS    ,
    "-update" , CMD_UPDATE  ,
    "-window" , CMD_WINDOW  ,
    "bye"     , CMDEXIT     ,
    "call"    , CMDCALL     ,
    "center"  , CMDCENTER   ,
    "clear"   , CMDCLEAR    ,
    "close"   , CMDCLOSE    ,
    "command" , CMDCOMMAND  ,
#ifdef  DELETE
    "delete"  , CMDDELETE   ,
    "delete"  , CMDDELETE   ,
#endif  DELETE
    "e"       , CMDEDIT     ,
    "edit"    , CMDEDIT     ,
    "endm"    , CMDENDM     ,
    "erase"   , CMDERASE    ,
    "exit"    , CMDEXIT     ,
    "feed"    , CMDFEED     ,
    "fill"    , CMDFILL     ,
    "goto"    , CMDGOTO     ,
    "help"    , CMDHELP     ,
    "help"    , CMDHELP     ,
    "join"    , CMDJOIN     ,
    "justify" , CMDJUST     ,
    "logoff"  , CMDEXIT     ,
    "macro"   , CMDMACRO    ,
    "mark"    , CMDMARK     ,
#ifdef  NAME
    "name"    , CMDNAME     ,
#endif  NAME
    "open"    , CMDOPEN     ,
    "pick"    , CMDPICK     ,
    "print"   , CMDPRINT    ,
    "redraw"  , CMDREDRAW   ,
    "replace" , CMDREPLACE  ,
    "region"  , CMDREGION   ,
    "run"     , CMDRUN      ,
    "save"    , CMDSAVE     ,
    "search"  , CMDSEARCH   ,
    "shell"   , CMDSHELL    ,
    "split"   , CMDSPLIT    ,
    "tab"     , CMDTAB      ,
    "tabfile" , CMDTABFILE  ,
    "tabs"    , CMDTABS     ,
    "update"  , CMDUPDATE   ,
    "window"  , CMDWINDOW   ,
    0, 0

};

Flag    clrsw,                    /* is 1 to clear paramport            */
	csrsw,                    /* is 1 if bullet is to temporarily   */
	bulsw;

struct markenv *curmark,
	       *prevmark;

Nlines  infoline;       /* line number displayed */
Fn      infofile;       /* file number of filename displayed */

Nlines  marklines;      /* display of num of lines marked */
Ncols   markcols;       /* display of num of columns marked */


char    mklinstr [6],   /* string of display of num of lines marked */
	mkcolstr [6];   /* string of display of num of lines marked */

Small   infoarealen;    /* len of string of marked area display */

/**************/
/* file e.ru.c */

char *filters[] = {
    "fill",
    "just",
    "center",
    "print"
};
char *filterpaths[] = {
    "/etc/e/fill",
    "/etc/e/just",
    "/etc/e/center",
    "/etc/e/print"
};
