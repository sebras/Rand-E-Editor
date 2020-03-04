#ifdef COMMENT
--------
file e.cm.c
    command dispatching routine and some actual command-executing routines.
#endif

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <unistd.h>
#include "e.h"
#include "e.e.h"
#include "e.m.h"
#include "e.ru.h"
#include "e.cm.h"
#include "e.wi.h"
#ifdef LMCHELP
#include "e.tt.h"
#endif

extern Cmdret fileStatus ();

#include SIG_INCL

S_looktbl cmdtable[] = {
#ifdef CMDVERALLOC
    "#veralloc", CMDVERALLOC,
#endif
    "-blot"   , CMD_BLOT    ,
    "-close"  , CMD_CLOSE   ,
    "-command", CMD_COMMAND ,
#ifdef NOTYET
    "-diff"   , CMD_DIFF    ,
#endif
#ifdef LMCDWORD
    "-dword"  , CMD_DWORD   ,
#endif
    "-erase"  , CMD_ERASE   ,
    "-join"   , CMDSPLIT    ,
    "-macro"  , CMD_MACRO   ,
    "-mark"   , CMD_MARK    ,
    "-pick"   , CMD_PICK    ,
    "-range"  , CMD_RANGE   ,
    "-re"     , CMD_PATTERN ,   /* Added Purdue CS 10/18/82 MAB */
    "-regexp" , CMD_PATTERN ,   /* Added Purdue CS 10/18/82 MAB */
    "-replace", CMD_REPLACE ,
    "-search" , CMD_SEARCH  ,
    "-split"  , CMDJOIN     ,
    "-tab"    , CMD_TAB     ,
    "-tabfile", CMD_TABFILE ,
    "-tabs"   , CMD_TABS    ,
    "-track"  , CMD_TRACK   ,
    "-update" , CMD_UPDATE  ,
    "-w"      , CMD_WINDOW  ,
    "-window" , CMD_WINDOW  ,
#ifdef LMCAUTO
    "-wp"     , CMD_AUTO    ,
#endif
    "?file"   , CMDQFILE    ,
    "?range"  , CMDQRANGE   ,
    "?set"    , CMDQSET     ,
    "b"       , CMDEXIT     ,
    "blot"    , CMDBLOT     ,
    "box"     , CMDBOX      ,
    "bye"     , CMDEXIT     ,
    "call"    , CMDCALL     ,
#ifdef LMCCASE
    "caps"    , CMDCAPS     ,
    "ccase"   , CMDCCASE    ,
#endif
    "cd"      , CMDCD       , 
    "center"  , CMDCENTER   ,
    "checkscr", CMDCHECKSCR ,
    "clear"   , CMDCLEAR    ,
    "close"   , CMDCLOSE    ,
    "command" , CMDCOMMAND  ,
    "cover"   , CMDCOVER    ,
    "delete"  , CMDDELETE   ,
#ifdef NOTYET
    "diff"    , CMDDIFF     ,
#endif
    "dword"   , CMDDWORD    ,
    "e"       , CMDEDIT     ,
    "edit"    , CMDEDIT     ,
#ifdef FUTURCMD
    "endm"    , CMDENDM     ,
#endif
    "erase"   , CMDERASE    ,
    "exit"    , CMDEXIT     ,
    "feed"    , CMDFEED     ,
    "file"    , CMDFILE     ,
    "fill"    , CMDFILL     ,
    "goto"    , CMDGOTO     ,
    "help"    , CMDHELP     ,
    "insert"  , CMDINSERT   ,
    "join"    , CMDJOIN     ,
    "justify" , CMDJUST     ,
    "logoff"  , CMDLOGOUT   ,
    "logout"  , CMDLOGOUT   ,
#ifdef FUTURCMD
    "macro"   , CMDMACRO    ,
    "mark"    , CMDMARK     ,
#endif
    "name"    , CMDNAME     ,
    "open"    , CMDOPEN     ,
    "overlay" , CMDOVERLAY  ,
    "pick"    , CMDPICK     ,
#ifdef FUTURCMD
    "print"   , CMDPRINT    ,
#endif
    "pwd"     , CMDSTATS    ,
    "range"   , CMDRANGE    ,
    "re"      , CMDPATTERN  ,
    "redraw"  , CMDREDRAW   ,
    "regexp"  , CMDPATTERN  ,
    "replace" , CMDREPLACE  ,
    "run"     , CMDRUN      ,
    "save"    , CMDSAVE     ,
    "search"  , CMDSEARCH   ,
    "set"     , CMDSET      ,
    "shell"   , CMDSHELL    ,
    "split"   , CMDSPLIT    ,
    "status"  , CMDSTATUS   ,
#ifdef SIGTSTP  /* 4bsd vmunix */
    "stop"    , CMDSTOP     ,
#endif
    "tab"     , CMDTAB      ,
    "tabfile" , CMDTABFILE  ,
    "tabs"    , CMDTABS     ,
    "track"   , CMDTRACK    ,
    "underlay", CMDUNDERLAY ,
    "undo"    , CMDUNDO     ,
    "update"  , CMDUPDATE   ,
    "version" , CMDVERSION  ,
    "w"       , CMDWINDOW   ,
    "window"  , CMDWINDOW   ,
#ifdef LMCAUTO
    "wp"     ,  CMDAUTO     ,
#endif
    0, 0
};

/* table of command with multiple names */
static S_looktbl multi_cmd [] = {
    "exit",     CMDEXIT,
    "edit",     CMDEDIT,
    "window",   CMDWINDOW,
    "logout",   CMDLOGOUT,
    "-window",  CMD_WINDOW,
    "regexp",   CMDPATTERN,
    "-regexp",  CMD_PATTERN,
    "split",    CMDSPLIT,
    "join",     CMDJOIN,
    0,          0
    };

/* get_cmd_name : get the command major name string */
/* ------------------------------------------------ */
char * get_cmd_name (int idx) {
char *cmd_str;
int cmd, i;

    cmd = cmdtable[idx].val;
    cmd_str = cmdtable[idx].str;    /* default value */
    /* look for the major name */
    for ( i = 0 ; multi_cmd[i].str ; i++ ) {
        if ( multi_cmd[i].val != cmd ) continue;
        cmd_str = multi_cmd[i].str;
        break;
    }
    return (cmd_str);
}


extern void dostop ();

#ifdef LMCCMDS
#ifdef COMMENT
Cmdret
command (forcecmd, forceopt)
    int forcecmd;
    char *forceopt;
.
    Parses a command line and dispatches on the command.
    If a routine that was called to execute a command returns a negative
    value, then the error message is printed out here, and returns CROK.
    Else returns the completion status of the command.
#endif
Cmdret
command (forcecmd, forceopt)
    int forcecmd;
    char *forceopt;
{
    Short cmdtblind;
    Cmdret retval;
    Short cmdval;
    char *cmdstr;
    int clrcnt;                 /* Added 10/18/82 MAB */
#ifdef LMCHELP
    /* XXXXXXXXXXXXXXXXXXX */
    char helparg[256];
#endif


    if (forcecmd != 0) {
	cmdval = forcecmd;
	for (cmdtblind=0; cmdval != cmdtable[cmdtblind].val; cmdtblind++)
		;
	nxtop = forceopt;
    } else {
	nxtop = paramv;
	cmdstr = getword (&nxtop);
	if (cmdstr[0] == '\0')
	    return CROK;
	cmdtblind = lookup (cmdstr, cmdtable);
	if (cmdtblind == -1) {
#ifdef LMCGO
	    if (cmdstr[0] >= '1' && cmdstr[0] <= '9') {
		nxtop = paramv;
		cmdtblind = lookup ("g", cmdtable);
	    } else {
#endif
		mesg (ERRALL + 3, "Command \"", cmdstr, "\" not recognized");
		sfree (cmdstr);
		return CROK;
#ifdef LMCGO
	    }
#endif
	}
	else if (cmdtblind == -2) {
	    mesg (ERRALL + 3, "Command \"", cmdstr, "\" ambiguous");
	    sfree (cmdstr);
	    return CROK;
	}
	sfree (cmdstr);
	cmdval = cmdtable [cmdtblind].val;
    }
    cmdopstr = nxtop;
    opstr = getword (&nxtop);
    cmdname = cmdtable[cmdtblind].str;
    switch (cmdval) {
#else
#ifdef COMMENT
Cmdret
command ()
.
    Parses a command line and dispatches on the command.
    If a routine that was called to execute a command returns a negative
    value, then the error message is printed out here, and returns CROK.
    Else returns the completion status of the command.
#endif
Cmdret
command ()
{
    Short cmdtblind;
    Cmdret retval;
    Short cmdval;
    char *cmdstr;
    int clrcnt;                 /* Added 10/18/82 MAB */
#ifdef LMCHELP
    /* XXXXXXXXXXXXXXXXXXX */
    char helparg[256];
#endif

    nxtop = paramv;
    cmdstr = getword (&nxtop);
    if (cmdstr[0] == 0)
	return CROK;
    cmdtblind = lookup (cmdstr, cmdtable);
    if (cmdtblind == -1) {
	mesg (ERRALL + 3, "Command \"", cmdstr, "\" not recognized");
	sfree (cmdstr);
	return CROK;
    }
    else if (cmdtblind == -2) {
	mesg (ERRALL + 3, "Command \"", cmdstr, "\" ambiguous");
	sfree (cmdstr);
	return CROK;
    }
    sfree (cmdstr);

    cmdopstr = nxtop;
    opstr = getword (&nxtop);

    cmdname = cmdtable[cmdtblind].str;
    switch (cmdval = cmdtable[cmdtblind].val) {
#endif

#ifdef CMDVERALLOC
    case CMDVERALLOC:
	veralloc ();
	retval = CROK;
	break;
#endif

    case CMDRANGE:
    case CMD_RANGE:
    case CMDQRANGE:
	retval = rangecmd (cmdval);
	break;

    case CMDTRACK:
#ifdef LMCTRAK
	if ((curwin->winflgs & TRACKSET) == 0)
	    curwin->winflgs |= TRACKSET;
	else
	    curwin->winflgs &= ~TRACKSET;
	infotrack (TRACKSET & curwin->winflgs);
#else
	curwin->winflgs |= TRACKSET;
	infotrack (YES);
#endif
	retval = CROK;
	break;

    case CMD_TRACK:
	curwin->winflgs &= ~TRACKSET;
	infotrack (NO);
	retval = CROK;
	break;

#ifdef SIGTSTP  /* 4bsd vmunix */
    case CMDSTOP:
	dostop ();
	retval = CROK;
	break;
#endif

    case CMDUPDATE:
    case CMD_UPDATE:
	retval = doupdate (cmdval == CMDUPDATE);
	break;

    case CMDTAB:
	retval = dotab (YES);
	break;

    case CMD_TAB:
	retval = dotab (NO);
	break;

    case CMDTABS:
	retval = dotabs (YES);
	break;

    case CMD_TABS:
	retval = dotabs (NO);
	break;

    case CMDTABFILE:
	retval = tabfile (YES);
	break;

    case CMD_TABFILE:
	retval = tabfile (NO);
	break;

    case CMDREPLACE:
    case CMD_REPLACE:
	if (!okwrite ()) {
	    retval = NOWRITERR;
	    break;
	}
	retval = replace (cmdval == CMDREPLACE? 1: -1);
	break;

    case CMDPATTERN:            /* Added at CS/Purdue 10/3/82 MAB */
    case CMD_PATTERN:
	if (*cmdopstr)
	    retval = CRTOOMANYARGS;
	else if (patmode && cmdval == CMDPATTERN){
	    mesg(ERRALL+1, "You are in RE mode");
	    retval = CROK;
	}
	else if (!patmode && cmdval == CMD_PATTERN){
	    mesg(ERRALL+1, "You are not in RE mode");
	    retval = CROK;
	}
	else{
		tglpatmode();
		clrcnt = 0;
		if (searchkey){
		    sfree(searchkey);
		    searchkey = (char *) 0;
		    clrcnt++;
		}
		clrcnt += zaprpls();
		retval = CROK;
	}
	break;

    case CMDNAME:
	retval = name ();
	break;

    case CMDDELETE:
	retval = delete ();
	break;

    case CMDCOMMAND:
	mesg (TELALL + 1, "CMDS: ");
    case CMD_COMMAND:
	cmdmode = cmdval == CMDCOMMAND ? YES : NO;
	retval = CROK;
	break;

    case CMDPICK:
    case CMDCLOSE:
    case CMDERASE:
    case CMDOPEN:
    case CMDBOX:
#ifdef LMCCASE
    case CMDCAPS:
    case CMDCCASE:
#endif
	retval = areacmd (cmdval - CMDPICK);
	break;

    case CMDCOVER:
    case CMDOVERLAY:
    case CMDUNDERLAY:
    case CMDBLOT:
    case CMD_BLOT:
    case CMDINSERT:
	retval = insert (cmdval - CMDCOVER);
	break;

    case CMD_PICK:
    case CMD_CLOSE:
    case CMD_ERASE:
	retval = insbuf (cmdval - CMD_PICK + QPICK);
	break;

    case CMDCALL:
	retval = call ();
	/* if the syntax of the shell command was correct and all of the
	   saves went OK, forkshell will never return. */
	break;

    case CMDSHELL:
	retval = shell ();
	/* if the syntax of the shell command was correct and all of the
	   saves went OK, forkshell will never return. */
	break;

    case CMDLOGOUT:
	if (!loginflg) {
	    mesg (ERRALL + 1, "This is not your login program.  Use \"exit\".");
	    break;
	}
    case CMDEXIT:
	retval = eexit ();
	/* if the syntax of the exit command was correct and all of the
	   saves went OK, eexit will never return. */
	break;

#ifdef LMCHELP
    case CMDSTATUS:
	strncpy (helparg, "status", sizeof(helparg));
	helparg[sizeof (helparg) - 1] = '\0';
	if (*term.tt_help == NULL) retval = help_std (helparg);
	else retval = (*term.tt_help)(helparg);
	break;

    case CMDHELP:
	mesg (ERRALL + 1, "Calling help....");
	strncpy (helparg, opstr, sizeof(helparg));
	helparg[sizeof (helparg) - 1] = '\0';
	if (*term.tt_help == NULL) retval = help_std (helparg);
	else retval = (*term.tt_help)(helparg);
	break;

#endif

    case CMDCHECKSCR:
        {
	    extern void check_keyboard ();
	    extern int set_crlf ();
	    extern void reset_crlf ();
	    extern void getConsoleSize ();
	    int oflag;
	    int col, lin, x, y;
	    char * strg;

	    col = cursorcol; lin = cursorline;
	    savecurs ();
	    ( *term.tt_clear ) ();
	    ( *term.tt_home ) ();
	    poscursor (0, term.tt_height -1);
	    oflag = set_crlf ();

	    strg = termtype ? "" : " (use termcap or terminfo)";
	    printf ("Terminal : %s%s, keyboard : %s (%d)\n",
		    tname, strg, kname, kbdtype);
	    getConsoleSize (&x, &y);
	    printf ("Current screen size %d lines of %d columns\n", y, x);

	    check_keyboard ();

	    reset_crlf (oflag);
	    mesg (TELALL+1, " ");
	    fflush (stdout);
	    fresh ();
	    restcurs ();
            retval = CROK;
            break;
        }

    case CMDREDRAW:
	fresh ();
	retval = CROK;
	break;

    case CMDSPLIT:
    case CMDJOIN:
	if (*cmdopstr)
	    retval = CRTOOMANYARGS;
	else
	    retval = cmdval == CMDJOIN ? join () : split ();
	break;

    case CMDRUN:
    case CMDFEED:
	if (!okwrite ()) {
	    retval = NOWRITERR;
	    break;
	}
	retval = run (cmdopstr, cmdval);
	break;

    case CMDFILL:
	retval = filter (FILLNAMEINDEX, YES);
	break;

    case CMDJUST:
	retval = filter (JUSTNAMEINDEX, YES);
	break;

    case CMDCENTER:
	retval = filter (CENTERNAMEINDEX, YES);
	break;

    case CMDSAVE:
	if (curmark) {
	    retval = NOMARKERR;
	    break;
	}
	retval = save ();
	break;

    case CMDEDIT:
	if (curmark) {
	    retval = NOMARKERR;
	    break;
	}
	retval = edit ();
	break;

    case CMDWINDOW:
	if (curmark) {
	    retval = NOMARKERR;
	    break;
	}
	if (*opstr == '\0')
	    makewindow ((char *)0);
	else {
	    if (*nxtop) {
		retval = CRTOOMANYARGS;
		break;
	    }
	    makewindow (opstr);
	}
	loopflags.bullet = YES;
	retval = CROK;
	break;

    case CMD_WINDOW:
	if (curmark) {
	    retval = NOMARKERR;
	    break;
	}
	if (*opstr) {
	    retval = CRTOOMANYARGS;
	    break;
	}
	removewindow ();
	retval = CROK;
	break;

    case CMDGOTO:
	retval = gotocmd ();
	break;

#ifdef LMCAUTO
    case CMDAUTO:
	retval = parsauto (NO);
	break;

    case CMD_AUTO:
	retval = parsauto (YES);
	break;
#endif

#ifdef LMCDWORD
    case CMDDWORD:
	retval = dodword (YES);
	break;

    case CMD_DWORD:
	retval = dodword (NO);
	break;
#endif

    case CMDCD:
        {
            extern Cmdret dochangedirectory ();
	    retval = dochangedirectory ();
	    break;
        }

    case CMDVERSION:
	Block {
	    extern char verstr[];
	    char strg [128];
	    strncpy (strg, verstr, sizeof (strg));
	    strg [sizeof (strg) -1] = '\0';
	    if ( strg [strlen (strg) -1] == '\n' )
		strg [strlen (strg) -1] = '\0';
	    mesg (TELALL + 1, strg);
	    loopflags.hold = YES;
	    retval = CROK;
            break;
        }

    case CMDSTATS:          /* pwd command */
	Block {
            int sz;
	    char pwdname[PATH_MAX +16];
	    char *sp;

            strcpy (pwdname, "CWD = ");
            sz = strlen (pwdname);
	    sp = getcwd (&pwdname[sz], sizeof (pwdname) - sz);
	    if ( ! sp ) sp = "./";
	    sz = strlen (pwdname);
	    if ( sz >= term.tt_width -2 ) pwdname [term.tt_width -2] = '\0';
	    mesg (TELALL + 1, pwdname);
	    loopflags.hold = YES;
	    retval = CROK;
        }
	break;

    case CMDQFILE:      /* current file status query */
	retval = fileStatus (YES);
	break;
    case CMDFILE:       /* current file status set / query */
	retval = fileStatus (NO);
	break;

    case CMDSET:
	retval = setoption(NO);
	break;

    case CMDQSET:
	retval = setoption(YES);
	break;

#ifdef NOTYET
    case CMD_DIFF:
	retval = diff (-1);
	break;

    case CMDDIFF:
	retval = diff (1);
	break;
#endif

    case CMDSEARCH:
    case CMD_SEARCH:
	if ( *cmdopstr) {
	    if (patmode){
		extern Flag check_pattern ();
		if ( ! check_pattern (cmdopstr) ) {
		    retval = CROK;
		    break;
		}
	    }
	    if (searchkey) sfree (searchkey);
	    searchkey = append (cmdopstr, "");
	}
	dosearch (cmdval == CMDSEARCH ? 1 : -1);
	retval = CROK;
	break;

    default:
	mesg (ERRALL + 3, "Command \"", cmdtable[cmdtblind].str,
		"\" not implemented yet");
	retval = CROK;
	break;
    }
    if (opstr[0] != '\0')
	sfree (opstr);

    if (retval >= CROK)
	return retval;
    switch (retval) {
    case CRUNRECARG:
	mesg (1, " unrecognized argument to ");
	break;

    case CRAMBIGARG:
	mesg (1, " ambiguous argument to ");
	break;

    case CRTOOMANYARGS:
	mesg (ERRSTRT + 1, "Too many of arguments to ");
	break;

    case CRNEEDARG:
	mesg (ERRSTRT + 1, "Need an argument to ");
	break;

    case CRNOVALUE:
	mesg (ERRSTRT + 1, "No value for option to ");
	break;

    case CRMULTARG:
	mesg (ERRSTRT + 1, "Duplicate arguments to ");
	break;

    case CRMARKCNFL:
	return NOMARKERR;

    case CRBADARG:
    default:
	mesg (ERRSTRT + 1, "Bad argument(s) to ");
    }
    mesg (ERRDONE + 3, "\"", cmdname, "\"");

    return CROK;
}

#ifdef COMMENT
Cmdret
gotocmd ()
.
    Do the "goto" command.
#endif
Cmdret
gotocmd ()
{
    if (opstr[0] == '\0') {
	gotomvwin (0);
	return CROK;
    }
    if (*nxtop)
	return CRTOOMANYARGS;

    Block {
	Reg2 Short tmp;
	Reg1 char *cp;
	char *cp1;

	cp1 = opstr;
	tmp = getpartype (&cp1, 0, 0, 0);
	if (tmp == 1) {
	    for (cp = cp1; *cp && *cp == ' '; cp++)
		continue;
	    if (*cp == 0) {
		gotomvwin (parmlines - 1);
		return CROK;
	    }
	}
	else if (tmp == 2)
	    return CRBADARG;
    }

    Block {
	static S_looktbl gttbl[] = {
	    "b",            0,  /* guranteed abbreviation */
	    "beginning",    0,
	    "e",            1,  /* guranteed abbreviation */
	    "end",          1,
	    "rb",           2,  /* guranteed abbreviation */
	    "rbeginning",   2,
	    "re",           3,  /* guranteed abbreviation */
	    "rend",         3,
	     0,             0,
	};
	Reg1 Small ind;
	Reg2 Small val;
	if ((ind = lookup (opstr, gttbl)) < 0) {
	    mesg (ERRSTRT + 1, opstr);
	    return ind == -2 ? CRAMBIGARG : CRUNRECARG;
	}
	switch (val = gttbl[ind].val) {
	case 0:
	    gotomvwin ((Nlines) 0);
	    break;

	case 1:
	    gotomvwin (la_lsize (curlas));
	    break;

	case 2:
	case 3:
	    if (curwksp->brnglas)
		gotomvwin (la_lseek (val == 2
				     ? curwksp->brnglas : curwksp->ernglas,
				     0, 1));
	    else
		return NORANGERR;
	    break;
	}
    }
    return CROK;
}

#ifdef COMMENT
Cmdret
doupdate (on)
    Flag on;
.
    Do the "update" command.
    The 'on' argument is non-0 for "update" and 0 for "-update"
#endif
Cmdret
doupdate (on)
Flag on;
{
    Small ind;
    static S_looktbl updatetable[] = {
	"-inplace", 0,
	"inplace",  0,
	0        ,  0
    };

    if (*nxtop || !on && opstr[0] != '\0')
	return CRTOOMANYARGS;

    if (on && !(fileflags[curfile] & DWRITEABLE)) {
	mesg (ERRALL + 1, "Don't have directory write permission");
	return CROK;
    }
    if (opstr[0] != '\0') {
	ind = lookup (opstr, updatetable);
	if (ind == -1  || ind == -2) {
	    mesg (ERRSTRT + 1, opstr);
	    return ind;
	}
	/* at this point, ind can be 0 = -inplace or 1 = inplace */
	if (ind) { /* inplace */
	    if (fileflags[curfile] & NEW) {
		mesg (ERRALL + 1, "\"inplace\" is useless;  file is new");
		return CROK;
	    }
	    if (!(fileflags[curfile] & FWRITEABLE)) {
		mesg (ERRALL + 1, "Don't have file write permission");
		return CROK;
	    }
	    fileflags[curfile] |= INPLACE | CANMODIFY;
	}
	else /* -inplace */
	    fileflags[curfile] &= ~INPLACE;
    }
    if (on)
	fileflags[curfile] |= UPDATE;
    else
	fileflags[curfile] &= ~UPDATE;
    return CROK;
}


#define SET_PLLINE      1
#define SET_PLPAGE      2
#define SET_MILINE      3
#define SET_MIPAGE      4
#define SET_SHOW        5       /* very tacky usage below! */
#define SET_BELL        6
#define SET_WINLEFT     7
#define SET_LINE        8
#define SET_NOBELL      9
#define SET_PAGE        10
#define SET_WINRIGHT    11
#define SET_WIDTH       12
#define SET_WIN         13
#define SET_WORDDELIM   14
#define SET_DEBUG       15              /* internal debugging */
#ifdef LMCAUTO
#define SET_LMARG       16
#endif
#ifdef LMCVBELL
#define SET_VBELL       17
#endif
#ifdef LMCSRMFIX
#define SET_RMSTICK     18
#define SET_RMNOSTICK   19
#endif
#define SET_HY          20
#define SET_NOHY        21

/* setoption_msg : build current options state message */
/* --------------------------------------------------- */
void setoption_msg (char *buf)
	/* buf must be >= 80 char */
{
    extern Flag fill_hyphenate;

#ifndef LMCVBELL
    sprintf(buf, "+li %d, -li %d, +pg %d, -pg %d, wr %d, wl %d, wid %d, bell %s, hy %s",
	defplline, defmiline, defplpage, defmipage, defrwin, deflwin,
	linewidth, NoBell ? "off" : "on", fill_hyphenate ? "on" : "off" );
#else
    sprintf(buf, "+li %d, -li %d, +pg %d, -pg %d, wr %d, wl %d, wid %d, bell %s, vb %s, hy %s",
	defplline, defmiline, defplpage, defmipage, defrwin, deflwin,
	linewidth, NoBell ? "off" : "on", VBell ? "on" : "off",
	fill_hyphenate ? "on" : "off" );
#endif
}

#ifdef COMMENT
Cmdret
setoption( showflag )
.
	Set/show various options
#endif

Cmdret
setoption( showflag )
  int showflag;
{
	Reg1 char *arg;
	Reg2 Small ind;
	Reg3 Small value;
	Cmdret retval;
	extern Flag fill_hyphenate;
	static S_looktbl setopttable[] = {
	    "+line",        SET_PLLINE,    /* defplline */
	    "+page",        SET_PLPAGE,    /* defplpage */
	    "-line",        SET_MILINE,    /* defmiline */
	    "-page",        SET_MIPAGE,    /* defmipage */
	    "?",            SET_SHOW,      /* show options */
	    "bell",         SET_BELL,      /* echo \07 */
	    "debug",        SET_DEBUG,
	    "hy",           SET_HY,        /* fill: split hyphenated words */
	    "left",         SET_WINLEFT,   /* deflwin */
	    "line",         SET_LINE,      /* defplline and defmiline */
#ifdef LMCAUTO
	    "lmargin",      SET_LMARG,     /* left margin */
#endif
	    "nobell",       SET_NOBELL,    /* do not echo \07 */
	    "nohy",         SET_NOHY,      /* fill: don't split hy-words */
#ifdef LMCSRMFIX
	    "nostick",      SET_RMNOSTICK, /* auto scroll past rt edge */
#endif
	    "page",         SET_PAGE,      /* defmipage and defplpage */
	    "right",        SET_WINRIGHT,  /* defrwin */
#ifdef LMCAUTO
	    "rmargin",      SET_WIDTH,     /* same as width */
#endif
#ifdef LMCSRMFIX
	    "stick",        SET_RMSTICK,   /* no auto scroll past rt edge */
#endif
#ifdef LMCVBELL
	    "vbell",        SET_VBELL,     /* visible bell */
#endif
	    "width",        SET_WIDTH,     /* linewidth */
	    "window",       SET_WIN,       /* deflwin and defrwin */
	    "worddelimiter",SET_WORDDELIM  ,/* set word delimiter */
	    0        ,  0
	};

	if (showflag) {
	    ind = SET_SHOW - 1;         /* tricky, tricky */
	    arg = (char *)NULL;
	}
	else {
	    if (!opstr[0])
		return CRNEEDARG;

	    ind = lookup (opstr, setopttable);
	    if (ind == -1 || ind == -2) {
		mesg (ERRSTRT + 1, opstr);
		return ind;
	    }

	    arg = getword(&nxtop);
	    if (arg == NULL) {
		cmdname = cmdopstr;
		return CRNEEDARG;
	    }
	}

	switch( setopttable[ind].val ) {

	    case SET_SHOW:
		{       char buf[80];
		    setoption_msg (buf);
/*
#ifndef LMCVBELL
sprintf(buf, "+li %d, -li %d, +pg %d, -pg %d, wr %d, wl %d, wid %d, \
bell %s, hy %s",
    defplline, defmiline, defplpage, defmipage, defrwin, deflwin,
    linewidth, NoBell ? "off" : "on", fill_hyphenate ? "on" : "off" );
#else
sprintf(buf, "+li %d, -li %d, +pg %d, -pg %d, wr %d, wl %d, wid %d, \
bell %s, vb %s, hy %s",
    defplline, defmiline, defplpage, defmipage, defrwin, deflwin,
    linewidth, NoBell ? "off" : "on", VBell ? "on" : "off",
    fill_hyphenate ? "on" : "off" );
#endif
*/
		    mesg (TELALL + 1, buf);
		}
		loopflags.hold = YES;
		retval = CROK;
		break;

	    case SET_PLLINE:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
		defplline = value;
		curwin->plline = defplline;
		curwin->winflgs |= PLINESET;
		retval = CROK;
		break;

	    case SET_MILINE:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
		defmiline = value;
		curwin->miline = defmiline;
		curwin->winflgs |= MLINESET;
		retval = CROK;
		break;

	    case SET_LINE:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
		defmiline = value;
		defplline = defmiline;
		curwin->plline = defmiline;
		curwin->miline = defmiline;
		curwin->winflgs |= PLINESET;
		curwin->winflgs |= MLINESET;
		retval = CROK;
		break;

	    case SET_MIPAGE:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
		defmipage = value;
		curwin->mipage = defmipage;
		retval = CROK;
		break;

	    case SET_PLPAGE:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
		defplpage = value;
		curwin->plpage = defplpage;
		retval = CROK;
		break;

	    case SET_PAGE:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
		defplpage = value;
		defmipage = defplpage;
		curwin->plpage = defplpage;
		curwin->mipage = defplpage;
		retval = CROK;
		break;

	    case SET_WINLEFT:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
		deflwin = value;
		curwin->lwin = deflwin;
		retval = CROK;
		break;

	    case SET_WIN:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
		defrwin = value;
		deflwin = defrwin;
		curwin->rwin = defrwin;
		curwin->lwin = deflwin;
		retval = CROK;
		break;

	    case SET_WINRIGHT:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
		defrwin = value;
		curwin->rwin = defrwin;
		retval = CROK;
		break;

	    case SET_WIDTH:
		if ((value = abs( atoi( arg ))) == 0)
		    goto BadVal;
#ifdef LMCAUTO
		if (value <= autolmarg)
		    mesg (ERRALL+1, "rmar must be greater than lmar.");
		else
#endif
		setmarg (&linewidth, value);
		retval = CROK;
		break;
#ifdef LMCAUTO
	    case SET_LMARG:
		if ((value = abs( atoi( arg ))) < 0)
		    goto BadVal;
		if (value >= linewidth)
		    mesg (ERRALL+1, "lmar must be less than rmar.");
		else
		    setmarg (&autolmarg, value);
		retval = CROK;
		break;
#endif
	    case SET_BELL:
		NoBell = NO;
#ifdef LMCVBELL
		VBell = NO;
#endif
		retval = CROK;
		break;

	    case SET_NOBELL:
		NoBell = YES;
#ifdef LMCVBELL
		VBell = NO;
#endif
		retval = CROK;
		break;

#ifdef LMCVBELL
	    case SET_VBELL:
		if (*term.tt_vbell == NULL)
			mesg (ERRALL+1, "No vis-bell on this terminal.");
		else {
			NoBell = NO;
			VBell = YES;
		}
		retval = CROK;
		break;
#endif

	    case SET_WORDDELIM:
		retval = setwordmode (arg);
		break;

	    case SET_DEBUG:
		DebugVal =  atoi (arg);
		retval = CROK;
		break;

#ifdef LMCSRMFIX
	    case SET_RMSTICK:
		optstick = YES;
		break;

	    case SET_RMNOSTICK:
		optstick = NO;
		break;
#endif

	    case SET_HY:
		fill_hyphenate = YES;
		break;

	    case SET_NOHY:
		fill_hyphenate = NO;
		break;

	    default:
BadVal:         retval = CRBADARG;
		break;
	}

	if (arg)
	    sfree(arg);

	return (retval);
}
