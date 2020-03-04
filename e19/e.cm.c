#ifdef COMMENT
--------
file e.cm.c
    command dispatching routine and some actual command-executing routines.
#endif

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#ifdef __linux__
#define _FILE_OFFSET_BITS 64
#endif

#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
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
extern void resize_screen ();

#include SIG_INCL

/* string for <Del> character */
char del_strg [] ="\177";

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
#ifdef FUTURCMD
    "-macro"  , CMD_MACRO   ,
#endif
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
    "-tick"   , CMD_TICK    ,
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
    "?tick"   , CMDQTICK    ,
    "b"       , CMDEXIT     ,
    "blot"    , CMDBLOT     ,
    "box"     , CMDBOX      ,
    "build-kbmap", CMDBKBFILE  ,
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
    "files"   , CMDSHFILES  ,
    "fill"    , CMDFILL     ,
    "flipbkar", CMDFLIPBKAR ,
    "flipbkarrow", CMDFLIPBKAR ,
    "goto"    , CMDGOTO     ,
    "help"    , CMDHELP     ,
    "insert"  , CMDINSERT   ,
    "join"    , CMDJOIN     ,
    "justify" , CMDJUST     ,
    "logoff"  , CMDLOGOUT   ,
    "logout"  , CMDLOGOUT   ,
#ifdef FUTURCMD
    "macro"   , CMDMACRO    ,
#endif
    "mark"    , CMDMARK     ,
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
/*  "resize"  , CMDRESIZE   ,   used purely internaly */
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
    "tick"    , CMDTICK     ,
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
    "flipbkarrow", CMDFLIPBKAR ,
    0,          0
    };

/* table of command which request a file name parameter */
/* ---------------------------------------------------- */
static struct _file_cmd {
    Short cmdv;
    Short dirflg;
    } file_cmds [] = {
    CMDEDIT,    0,
    CMDNAME,    0,
    CMDTABFILE, 0,
    CMD_TABFILE,0,
    CMDWINDOW,  0,
    CMDSAVE,    0,
    CMDCD,      1,
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

/* expand the file name */
/* -------------------- */

static struct dirent **namelist = NULL;
static int nb_namelist = 0;
static int dir_cmd_flg;
static int namelist_idx;
static char *dir_name = NULL;
static char *dir_name_fname = NULL;
static int dir_name_fname_sz = 0;
static int fname_para_sz = 0;
static char name_exp[PATH_MAX+2];


void clear_namelist () {
    int i;

    if ( namelist ) {
	if ( nb_namelist > 0 ) {
	    for ( i = nb_namelist-1 ; i >= 0 ; i-- )
		if ( namelist[i] ) free (namelist[i]);
	}
	free (namelist);
    }
    if ( dir_name ) free (dir_name);
    namelist = (struct dirent **) NULL;
    dir_name = dir_name_fname = NULL;
    nb_namelist = fname_para_sz = dir_name_fname_sz = 0;
    namelist_idx = -2;
}

/* dir_entry_mode : return the entry mode (type) */
/* --------------------------------------------- */

static mode_t dir_entry_mode (char *fname, struct dirent *dent)
{
    int cc;
    mode_t dirmode;
    struct stat stat_buf;


    if ( !(dir_name && dir_name_fname) ) return 0;

#ifdef _DIRENT_HAVE_D_TYPE
    if ( dent && dent->d_type )
	return (DTTOIF (dent->d_type));
#endif

    strncpy (dir_name_fname, fname, dir_name_fname_sz);
    cc = stat (dir_name, &stat_buf);
    memset (dir_name_fname, 0, dir_name_fname_sz);
    dirmode = (cc == 0) ? stat_buf.st_mode : 0;
#ifdef _DIRENT_HAVE_D_TYPE
    if ( dent ) dent->d_type = IFTODT (dirmode);
#endif
    return dirmode;
}

/* dir_entry test for directory name entry */
/* --------------------------------------- */
/* name_exp must have enough free space to be expanded with a '/'
    return 1 if the entry is a directory
*/

static int dir_entry (char *name_ex, struct dirent *dent)
{
    mode_t mode;

    mode = dir_entry_mode (name_ex, dent);
    if ( ! S_ISDIR (mode) ) return 0;
    strcat (name_ex, "/");
    return 1;
}

/* get_next_name : get the file name expantion from the selected list */
/* ------------------------------------------------------------------ */
/*  Return : >= 0 size of the epantion string
	       -1 at the top of the list
	       -2 at the bottom of the list
	       -3 only one name in the list
	       -4 no name in the list
	       -5 nothing selected
*/

int get_next_name (int delta, char ** name_expantion_pt)
{
    int idx, sz, flg;
    struct dirent *dent;

    if ( namelist == NULL ) return -5;  /* nothing selected */
    if ( nb_namelist < 2 ) return ((nb_namelist == 0) ? -4 : -3);

    idx = namelist_idx + delta;
    if ( idx < 0 ) return -1;               /* top of the list */
    if ( idx >= nb_namelist ) return -2;    /* bottom of the list */

    namelist_idx = idx;
    dent = namelist[namelist_idx];
    memset (name_exp, 0, sizeof(name_exp));
    strncpy (name_exp, dent->d_name, sizeof(name_exp)-2);
    flg = dir_entry (name_exp, dent);
    *name_expantion_pt = &(name_exp[fname_para_sz]);
    sz = strlen (*name_expantion_pt);
    return sz;
}

/* expand_file_para : extend the file name */
/* --------------------------------------- */

/* static data for direntry_cmp routine */
static char *fname_para;    /* could point to volatile (stack) data ! */

int direntry_cmp (struct dirent *dent)
{
    char *str, chr;
    mode_t mode;

    str = dent->d_name;
    if ( *str == '.' ) {
	chr = str[1];
	if ( (chr == '\0') || (chr == '.' && str[2] == '\0') )
	    return 0;   /* allways remove the . and .. directory entry */
    }

    if ( fname_para_sz > 0 ) {
	if ( strncmp (str, fname_para, fname_para_sz) != 0 )
	return 0;
    }

#ifdef _DIRENT_HAVE_D_TYPE
    dent->d_type = 0;
#endif
    mode = dir_entry_mode (str, dent);

    if ( dir_cmd_flg ) return S_ISDIR (mode);
    return S_ISDIR (mode) || S_ISREG (mode) || S_ISLNK (mode);
}


int expand_file_para (char *file_para, char **name_expantion_pt,
		      int *wlen_pt, int *dir_flg_pt)
{
    extern int alphasort ();
    extern int scandir ();

    int i, j, sz, flg;
    char fp_strg[512];
    char *dir;
    struct dirent *dent;
    char path[PATH_MAX+2];
    struct stat stat_buf;
    int (*dir_select)();

    *name_expantion_pt = NULL;
    *wlen_pt = *dir_flg_pt = flg = 0;
    if ( !file_para ) return 0;

    clear_namelist ();

    memset (fp_strg, '\0', sizeof(fp_strg));
    strncpy (fp_strg, file_para, sizeof(fp_strg)-1);
    dir = fname_para = fp_strg;
    fname_para = strrchr (fp_strg, '/');
    if ( fname_para ) {
	*fname_para = '\0';
	fname_para++;
    } else {
	dir = ".";
	fname_para = fp_strg;
    }

    /* save the directory path name */
    sz = strlen(dir)+2;
    dir_name_fname_sz = PATH_MAX;
    /* alloc enough space for a full file name path */
    dir_name = (char *) calloc (sz + dir_name_fname_sz +1, sizeof(char));
    if ( dir_name ) {
	strcpy (dir_name, dir);
	strcat (dir_name, "/");
	/* save the position of file name */
	dir_name_fname = &(dir_name[strlen (dir_name)]);
    } else dir_name_fname_sz = 0;

    if ( dir[0] == '\0' ) dir = "/";

    fname_para_sz = strlen (fname_para);
    /*
    dir_select = ( dir_cmd_flg || (fname_para_sz != 0) ) ? direntry_cmp : (int(*)()) NULL;
    */
    dir_select = direntry_cmp;
    nb_namelist = scandir (dir, &namelist, dir_select, alphasort);
    namelist_idx = -1;
    /* to debug
    {
	char info_str [16];
	sprintf (info_str, "nb %d", nb_namelist);
	rand_info (7, 5, info_str);
	printf (" '%s / %s' : dflg %d %d %d", dir, fname_para, dir_cmd_flg, fname_para_sz, nb_namelist); fflush(stdout); sleep(5);
    }
    */
    if ( nb_namelist > 0 ) {
	memset (name_exp, 0, sizeof(name_exp));
	dent = namelist[0];
	strncpy (name_exp, dent->d_name, sizeof(name_exp)-2);
	if ( nb_namelist == 1 ) {
	    /* a single file name, check for directory */
	    flg = dir_entry (name_exp, dent);
	    sz = 0;
	} else {
	    /* more than one file name, get the larger common substring */
	    sz = strlen (name_exp);
	    for ( i = 1 ; i < nb_namelist ; i++ ) {
		dent = namelist[i];
		for ( j = fname_para_sz ; j < sz ; j++ ) {
		    if ( dent->d_name[j] == name_exp[j] ) continue;
		    name_exp[j] = '\0';
		    sz = strlen (name_exp);
		    break;
		}
		if ( sz == 0 ) break;
	    }
	}
	*name_expantion_pt = &(name_exp[fname_para_sz]);
	*wlen_pt = strlen (*name_expantion_pt);
	*dir_flg_pt = (fname_para_sz == 0); /* evry thing in the directory */
    }
    return nb_namelist;
}

void incr_fname_para_sz (int nb)
{
    fname_para_sz += nb;    /* what was displayed */
}


/* commande_file : check for command which request a file name as parameter */
/* ------------------------------------------------------------------------ */
/*
    This class of command could have automatic file name extension
	by the tab character (like in many shell program)
    Return : 1 (TRUE) if it a file command
	     0 (FALSE) if not
    If return TRUE, the file_param must be sfree if it is not null.
*/

int command_file (char *param, char **file_para_pt, Short *cmdval_pt)
{
    Short cmdtblind;
    Short cmdval;
    char *cmdstr, *nxt;
    int i;


    dir_cmd_flg = 0;
    if ( file_para_pt ) *file_para_pt = NULL;
    nxt = (param) ? param : paramv;
    cmdstr = getword (&nxt);
    if (cmdstr[0] == 0) return 0;

    cmdtblind = lookup (cmdstr, cmdtable);
    sfree (cmdstr);
    if ( cmdtblind < 0 ) return 0;

    cmdval = cmdtable[cmdtblind].val;
    if ( cmdval_pt ) *cmdval_pt = cmdval;
    for ( i = 0 ; file_cmds[i].cmdv != 0 ; i++ ) {
	if ( file_cmds[i].cmdv != cmdval ) continue;
	if ( file_para_pt != NULL ) *file_para_pt = getword (&nxt);
	dir_cmd_flg = file_cmds[i].dirflg;
	return 1;
    }
    return 0;
}


extern void dostop ();

#ifdef LMCHELP
static Cmdret call_help (char * helpcmd)
{
    Cmdret retval;
    char helparg [256];

    strncpy (helparg, helpcmd, sizeof(helparg));
    helparg[sizeof (helparg) - 1] = '\0';
    if (*term.tt_help == NULL) retval = help_std (helparg);
    else retval = (*term.tt_help) (helparg);
    return retval;
}
#endif


#ifdef LMCCMDS      /* function key extensions */
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
    extern S_looktbl helptable [];
    int idx;
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
#ifdef LMCGO    /* implicite goto command */
	    if (cmdstr[0] >= '1' && cmdstr[0] <= '9') {
		nxtop = paramv;
		cmdtblind = lookup ("g", cmdtable);
	    } else {
#endif
#ifdef LMCHELP
		idx = lookup (cmdstr, helptable);
		if ( idx >= 0 ) {
		    nxtop = paramv;
		    cmdtblind = lookup ("help", cmdtable);
		} else {
#endif
		    mesg (ERRALL + 3, "Command \"", cmdstr, "\" not recognized");
		    sfree (cmdstr);
		    return CROK;
#ifdef LMCHELP
		}
#endif
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

#else /* -LMCCMDS   no function key extensions */

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
    extern void cmds_prompt_mesg ();
    extern Cmdret displayfileslist ();
    extern Cmdret buildkbfile ();
    extern void itswapdeldchar (char *);
    extern void infotick ();
    extern void marktick (Flag set);

    Short cmdtblind;
    Cmdret retval;
    Short cmdval;
    char *cmdstr;
    int clrcnt;                 /* Added 10/18/82 MAB */

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

#endif /* -LMCCMDS   no function key extensions */

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
	cmds_prompt_mesg ();
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
	retval = call_help ("status");
	break;

    case CMDHELP:
	mesg (ERRALL + 1, "Calling help....");
	retval = call_help (opstr);
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

    case CMDRESIZE:
	resize_screen ();
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

    case CMDSHFILES:    /* display currently edited files list */
	retval = displayfileslist ();
	break;

    case CMDBKBFILE:    /* interactively build a mapping file */
	retval = buildkbfile ();
	break;

    case CMDSET:
	retval = setoption (NO);
	break;

    case CMDQSET:
	retval = setoption (YES);
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

    case CMDFLIPBKAR:
	/* flip the function of BackArrow key between "del" and "dchar"
	 *  assumed the BackArrow key generate the <del> (0177) ASCII char
	 */
	itswapdeldchar (del_strg);
	retval = CROK;
	break;

    case CMDMARK:
	mark ();
	retval = CROK;
	break;

    case CMD_MARK:
	unmark ();
	retval = CROK;
	break;

    case CMDTICK:
	marktick (YES);
	retval = CROK;
	break;

    case CMD_TICK:
	marktick (NO);
	retval = CROK;
	break;

    case CMDQTICK:
	infotick ();
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
    extern void gototick ();
    extern void savemark (struct markenv *);
    extern Small movemark (struct markenv *, Flag);
    extern void copymark (struct markenv *, struct markenv *);

    struct markenv tmpmk;
    Nlines lnb, dlnb;

    savemark (&tmpmk);

    if (opstr[0] == '\0') {
	gotomvwin (0);
	copymark (&tmpmk, &curwksp->wkpos);
	return CROK;
    }
    if (*nxtop)
	return CRTOOMANYARGS;

    Block {
	Short tmp;
	char ch;
	char *cp, *cp1;

	for (cp1 = opstr; *cp1 && *cp1 == ' '; cp1++) continue;
	ch = (cp1) ? *cp1 : '\0';
	if ( (ch == '+') || (ch == '-') ) cp1++;
	tmp = getpartype (&cp1, 0, 0, 0);
	if (tmp == 1) {
	    for (cp = cp1; *cp && *cp == ' '; cp++)
		continue;
	    if (*cp == 0) {
		lnb = parmlines - 1;
		if ( ch == '-' ) lnb = curwksp->wlin + cursorline - parmlines;
		if ( ch == '+' ) lnb = curwksp->wlin + cursorline + parmlines;
		dlnb = lnb - curwksp->wlin;
		if ( (dlnb < 0) || (dlnb > curwin->btext) ) gotomvwin (lnb);
		else poscursor (cursorcol, dlnb);
		copymark (&tmpmk, &curwksp->wkpos);
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
	    "p",            5,
	    "prev",         5,
	    "rb",           2,  /* guranteed abbreviation */
	    "rbeginning",   2,
	    "re",           3,  /* guranteed abbreviation */
	    "rend",         3,
	    "t",            4,
	    "tick",         4,
	     0,             0,
	};
	Small ind;
	Small val;

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
	case 4:
	    gototick ();
	    break;
	case 5:
	    (void) movemark (&curwksp->wkpos, YES);
	    break;
	}
	copymark (&tmpmk, &curwksp->wkpos);
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
#define SET_CHARSET     22

/* setoption_msg : build current options state message */
/* --------------------------------------------------- */
void setoption_msg (char *buf)
	/* buf must be >= 128 char */
{
    extern Flag fill_hyphenate;
    char * buf_pt;

#ifndef LMCVBELL
    sprintf (buf, "+li %d, -li %d, +pg %d, -pg %d, wr %d, wl %d, wid %d, bell %s, hy %s",
	     defplline, defmiline, defplpage, defmipage, defrwin, deflwin,
	     linewidth, NoBell ? "off" : "on", fill_hyphenate ? "on" : "off" );
#else
    sprintf (buf, "+li %d, -li %d, +pg %d, -pg %d, wr %d, wl %d, wid %d, bell %s, vb %s, hy %s",
	     defplline, defmiline, defplpage, defmipage, defrwin, deflwin,
	     linewidth, NoBell ? "off" : "on", VBell ? "on" : "off",
	     fill_hyphenate ? "on" : "off" );
#endif
    buf_pt = & buf [strlen (buf)];
    if ( DebugVal == 0 ) strcpy (buf_pt, ", debug off");
    else sprintf (buf_pt, ", debug %d", DebugVal);
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
	extern void alt_set_term ();
	extern void fresh ();

	char *ternm;
	char *arg;
	Small ind;
	Small value;
	Cmdret retval;
	extern Flag fill_hyphenate;
	static S_looktbl setopttable[] = {
	    "+line",        SET_PLLINE,    /* defplline */
	    "+page",        SET_PLPAGE,    /* defplpage */
	    "-line",        SET_MILINE,    /* defmiline */
	    "-page",        SET_MIPAGE,    /* defmipage */
	    "?",            SET_SHOW,      /* show options */
	    "bell",         SET_BELL,      /* echo \07 */
#ifdef __linux__
	    "charset",      SET_CHARSET,   /* set reset the IBM PC character set */
#endif
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
	    "worddelimiter",SET_WORDDELIM, /* set word delimiter */
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

	    arg = getword (&nxtop);
	    if (arg == NULL) {
		cmdname = cmdopstr;
		return CRNEEDARG;
	    }
	}

	switch( setopttable[ind].val ) {

	    case SET_SHOW:
		{   char buf[128];
		    setoption_msg (buf);
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

	    case SET_CHARSET:
		ternm = ((value = abs( atoi( arg ))) == 0) ? NULL : tname;
		alt_set_term (ternm, YES);
		fresh ();
		retval = CROK;
		break;

	    default:
BadVal:         retval = CRBADARG;
		break;
	}

	if ( arg && *arg )
	    sfree (arg);

	return (retval);
}
