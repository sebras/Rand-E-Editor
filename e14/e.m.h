/*
/* file e.m.h - some specialized include stuff
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

extern char *cmdname;           /* the full name of the command given */
extern char *cmdopstr;  /* command operand string - points to the rest of
			/* paramv after the command */
extern char *opstr;     /* first word in cmdopstr from a call to getword () */
extern char *nxtop;     /* next word after opstr - use for further getwords */

extern S_looktbl cmdtable[];
extern S_looktbl exittable[];

/* commands: <arg> command <ret> */
#define CMD_MARK        0
#define CMDCHWINDOW     1
#define CMD_REGION      2
#define CMD_COMMAND     3
#define CMDEXIT         4
#define CMDCENTER       5
#define CMDMARK         6
#define CMDWINDOW       7
#define CMD_WINDOW      8
#define CMDRUN          9
#define CMDSAVE         10
#define CMDREGION       11
#define CMDFILL         12
#define CMDJUST         13
#define CMDEDIT         14
#define CMDREDRAW       15
#define CMDGOTO         16
#define CMDSPLIT        17
#define CMD_JOIN        17
#define CMDJOIN         18
#define CMD_SPLIT       18
#define CMDPRINT        19
#define CMDCOMMAND      20
#define CMDOPEN         21 /************************************/
#define CMDPICK         22 /*   these four must be consecutive */
#define CMDCLOSE        23 /*                                  */
#define CMDERASE        24 /************************************/
#define CMD_PICK        25 /************************************/
#define CMD_CLOSE       26 /*  these three must be consecutive */
#define CMD_ERASE       27 /************************************/
/* #define CMD next one 28 */
#define CMDTAB          29
#define CMD_TAB         30
#define CMDTABS         31
#define CMD_TABS        32
#define CMDTABFILE      33
#define CMD_TABFILE     34
#define CMDFEED         35
#define CMDHELP         36
#define CMDDELETE       37
#define CMDCLEAR        38
#define CMDNAME         39
#define CMDSHELL        40
#define CMDCALL         41
#define CMDMACRO        42
#define CMD_MACRO       43
#define CMDENDM         44
#define CMDSEARCH       45
#define CMD_SEARCH      46
#define CMDUSE          47
#define CMDREPLACE      48
#define CMD_REPLACE     49
#define CMDUPDATE       50
#define CMD_UPDATE      51

#define NOTSTRERR       1
#define NOWRITERR       2
#define NOARGERR        3
#define NOPIPERR        4
#define NOMARKERR       5
#define NORECTERR       6
#define NOBUFERR        7
/*                      8   use this next */
#define CONTIN          9
#define MARGERR         10

/* these came from the top of e.m.c */
extern
Flag    clrsw,                    /* is 1 to clear paramport            */
	csrsw,                    /* is 1 if bullet is to temporarily   */
	bulsw;
extern
char   *copy ();

/**/

struct markenv
{
    Nlines  mrkwinlin;
    ANcols  mrkwincol;
    ASlines mrklin;
    Scols   mrkcol;
};
extern
struct markenv *curmark,
	       *prevmark;

extern
Nlines  infoline;       /* line number displayed */
extern
Fn      infofile;       /* file number of filename displayed */

extern
Nlines  marklines;      /* display of num of lines marked */
extern
Ncols   markcols;       /* display of num of columns marked */

extern
char    mklinstr [],   /* string of display of num of lines marked */
	mkcolstr [];   /* string of display of num of lines marked */
extern
Small   infoarealen;    /* len of string of marked area display */

