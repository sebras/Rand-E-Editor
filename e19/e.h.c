#ifdef COMMENT
--------
file e.h.c
    support  standard help functions.
#endif

# include "e.h"
# include "e.cm.h"
# include "e.m.h"
# include "e.tt.h"
# include "e.h.h"
#include <string.h>

#ifdef SYSSELECT
#include <sys/time.h>
#else
#include SIG_INCL
#endif

/* these 2 values are primarily defined in e.it.h */
#define KBINIT  040     /* Must not conflict with CC codes */
#define KBEND   041


/* browse_keyboard : display the description of the pushed key assigned function */
/* ----------------------------------------------------------------------------- */
/*  return when :
	- CR is pushed
	- any non assigned key is pushed
*/

static void browse_keyboard (char *msg)
{
    static int help_description ();
    int go;
    Short qq;

    for ( go = YES ; go ; ) {
	qq = wait_keyboard (msg);
	if ( qq == CCRETURN ) break;
	go = help_description ('~', -1, key, NULL, NULL, NULL);
    }
}

/* ------------------------------------------------------------ */

/* open_help_file : local to open the help file */
/* -------------------------------------------- */

extern char * xdir_help;

static FILE * open_help_file () {
    FILE *helpin; 

    if ((helpin = fopen (xdir_help, "r")) == NULL) {
	printf ("\r\n\nCan't open %s\n\r", xdir_help);
    }
    return (helpin);
}

static void check_new_page (int *nbli, int nl)
{
    int sz;

    sz = term.tt_height -2;
    if ( nl ) sz++;
    if ( *nbli < sz ) return;

    wait_keyboard ("Push a key to complete the description");
    fputs ("\r                                          \r", stdout);
    *nbli = 0;
    (*term.tt_addr) (term.tt_height -2, 0);
}

/* help_description :
 *   Displays a message describing from the help file
 *   whose lexed value is passed in "val".
 *   The command major name is given in cmd_str.
 */

static int help_description (sep, nbli, val, cmd_str, str, abreviation)
char sep;   /* separator character : ~ (keyfunc) ` (coomand) # (info) */
int nbli;   /* number of line already displayed on the screen (-1 : nocheck) */
int val;    /* message value : command or key function value */
char *cmd_str;  /* if defined, the message is a command description */
char *str, *abreviation;    /* current command and mini abreviation */
{

    FILE *helpin, *fopen();
    int go, n, sz, nl;
    char buf [256];
    char *type;

    type = "???";

    helpin = open_help_file ();
    if ( helpin == NULL) {
	return (0);   /* no more message */
    }

    go = nl = NO;

    if ( nbli == 0 ) (*term.tt_home) ();

    if ( (sep != '#') && str ) {
	/* display the header for keyfunc and command */
	if ( sep == '`' ) {    /* message by string name */
	    type = "Command";
	    printf ("%s (%d) : Editor %s", str, val, type);
	    if ( strlen (abreviation) < strlen (str) )
		printf (", abreviated down to \"%s\"", abreviation);
	} else if ( sep == '~' ) {
	    type = ((val == KBINIT) || (val == KBEND))
		 ? "Defined String" : "Key Function";
	    printf ("%s (%d) : %s", str, val, type);
	}
	puts ("\n");    /* 2 nl ! */
	nbli +=2;
	nl = YES;
    }

    putchar ( (nbli >= 0 ) ? '\r' : '\n');
    while ( fgets (buf, sizeof buf, helpin) != NULL ) {
	if ( go ) {
	    if ( buf [0] == sep ) break;
	    if ( nl && (buf [0] == '\n') ) continue;
	    fputs (buf, stdout);
	    nl = (buf [0] == '\n');
	    if ( nbli >= 0 ) {
		nbli++;
		check_new_page (&nbli, nl);
	    }
	}
	else if ( buf [0] == sep ) {
            sz = strlen(buf);
            if ( buf[sz -1] == '\n' ) buf[sz -1] = '\0';
	    if ( cmd_str ) {    /* message by string name */
		if ( sep == '#' ) {
		    if ( strncmp (buf + 1, cmd_str, strlen (cmd_str)) == 0) go = YES;
		} else {
		    if ( strcmp (buf + 1, cmd_str) == 0) go = YES;
		}
	    }
	    else {      /* message by value */
		if ( (n = atoi (buf + 1)) == val ) go = YES;
	    }
        }
    }

    if ( !nl ) fputc ('\n', stdout);
    if ( (sep != '#') && !go ) {
	printf ("A description is not available for this %s in\n    %s\n\n",
		type, xdir_help);
    }
    if ( nl) go--;
    fclose (helpin);
    fflush (stdout);
    return (go);
}

/* help_cmd :
 *   Displays a message describing the effects of the command
 *   whose lexed value is passed in "cmd". 
 *   The command major name is given in cmd_str.
 */

static int help_cmd (cmd, cmd_str, str, abreviation)
int cmd;
char *cmd_str, *str, *abreviation;
{
    int go;

    go = help_description ('`', 0, cmd, cmd_str, str, abreviation);
    return (go);
}

int help_info (char *str, int *nbli)
{
    int go;
    int nb;

    nb = ( nbli ) ? *nbli : -1;
    go = help_description ('#', nb, 0, str, NULL, NULL);
    if ( nbli ) *nbli += go;
    return (go);
}

/* ------------------------------------------------------------------------ */

static void not_yet ()
{
    printf ("Not yet implemented\r\n");
}

static void keymap_term ()
{
    extern void display_keymap ();
    int cc;

    display_keymap (NO);
}

/* key_assigned : local to edit the string of key assigned to the given key function */
/* --------------------------------------------------------------------------------- */

static void key_assigned (msg, msg_sz, fcmd, fstr, funcval_flg)
char *msg;
int msg_sz;
int fcmd;
char *fstr;
Flag funcval_flg;
{
    extern Flag verbose_helpflg;
    extern char * escstrg ();
    extern int it_value_to_string ();
    extern void all_string_to_key ();
    int i, nb, sz;
    char ch, buf[256];
    char *escp[16];
    char *sp, *sp1;
    char *shift_key[4];
    char keys_label [4*128];
    int shift_key_nb;

    memset (escp, 0, sizeof (escp));
    memset (buf, 0, sizeof (buf));
    nb = it_value_to_string (fcmd, NO, buf, sizeof (buf));
    if ( nb <= 0 ) return;

    if ( nb > (sizeof (escp) / sizeof (escp[0])) )
	nb = sizeof (escp) / sizeof (escp[0]);
    sp = buf;

    if ( verbose_helpflg || funcval_flg ) printf ("%s\n", escstrg (buf));
    if ( funcval_flg ) return;

    for ( i = 0 ; i < nb ; i++ ) {
	sp1 = strchr (sp, ' ');
	escp [i] = sp;
	if ( !sp1 ) break;
	*sp1 = '\0';
	sp = sp1 +1;
    }
    if ( i < nb ) nb = i+1;

    /* control characters */
    for ( i = 0 ; i < nb ; i++ ) {
	/* control characters */
	ch = escp[i][0];
	if ( (ch == '\033') || (ch == '\177') ) continue;
	if ( ch >= '\040' ) continue;   /* ??? not a Ctrl or escape seq */
	sprintf (&msg[strlen (msg)], "<Ctrl %c> ", ch + '@');
    }

    /* assigned keys */
    memset (shift_key, 0, sizeof (shift_key));
    memset (keys_label, 0, sizeof (keys_label));
    shift_key_nb = sizeof (shift_key) / sizeof (shift_key[0]);
    for ( i = 0 ; i < nb ; i++ ) {
	ch = escp[i][0];
	if ( ch >= '\040' && (ch != '\177') ) continue;
	all_string_to_key (escp[i], shift_key, &shift_key_nb,
			   keys_label, sizeof (keys_label));
    }
    for ( i = 0 ; i < shift_key_nb ; i++ ) {
	if ( !shift_key[i] || !shift_key[i][0] ) continue;
	sz = msg_sz - strlen (msg);
	if ( sz < 1 ) break;
	strncat (msg, shift_key[i], sz);
	sz = strlen (msg);
	if ( sz < (msg_sz -2) ) {
	    msg [sz-1] =';';
	    msg [sz] =' ';
	}
    }
    return;
}
/* ------------------------------------------------------------------------ */

/* keyfunc_ibmpc : find the key for a given key command */
/* ---------------------------------------------------- */

static Cmdret keyfunc_ibmpc (helparg)
char *helparg;
{
    extern Flag verbose_helpflg;
    extern S_looktbl itsyms[];
    int idx;
    char msg [256];
    char *hlparg;
    int fcmd;
    char *str;
    Flag tflg;

    if ( ! *nxtop )
        return CRNEEDARG;

    str ="";
    hlparg = getword (&nxtop);
    idx = lookup (hlparg, itsyms);
    fcmd = itsyms[idx].val;
    str  = itsyms[idx].str;
    if ( *nxtop && (fcmd == CCCMD) ) {
	hlparg = getword (&nxtop);
	idx = lookup (hlparg, itsyms);
	if ( idx  >= 0 ) {
	    fcmd = itsyms[idx].val;
	    str  = itsyms[idx].str;
	}
    }

    (void) getword (&nxtop);
    if ( *nxtop ) 
    return CRTOOMANYARGS;

    if ( idx  < 0 ) {
	mesg (TELALL + 2, hlparg, " : Unknown Key Function");
	loopflags.hold = YES;
        return CROK;
    }
    memset (msg, 0, sizeof (msg));
    sprintf (msg, "%s: ", str);
    tflg = verbose_helpflg;
    verbose_helpflg = NO;
    key_assigned (msg, sizeof (msg), fcmd, str, NO);
    verbose_helpflg = tflg;
    msg [term.tt_width -3] = '\0';
    mesg (TELALL + 1, msg);
    loopflags.hold = YES;
    return CROK;
}

/* ======================================================================== */

/* New help for Unix console */
/* ------------------------- */
/*
    The argument provided by a <cmd><help> is defined by the value
	char *boardstg = "status\0";
	defined in the file : e.m.c in "mainloop" routine
*/

Cmdret help_std (helparg)
char *helparg;

{
void keymap_term ();
extern char *nxtop;
extern S_looktbl cmdtable[];
extern char * get_cmd_name ();
static void browse_cmdhelp ();
static void browse_keyfhelp ();

static char stmsg [] = "\n\
\"help status\"         Display the actual editor parameters value\n\
\"help NewFeatures\" or \"help new\"      Display the new features of the editor\n\
\"help key <function>\" Info on the keys assigned to the given function\n\
\"help keymap\"         Global keyboard mapping info\n\
\"help <command>\"      Info on the given Editor Command\n\
\"help cmd\"   or \"help commands\"       Navigate in Editor Commands description\n\
\"help keyf\"  or \"help keyfunctions\"   Navigate in Key Functions description\n\
\"help kbchk\" or \"help keyboard_check\" Interractively check keyboard mapping\n\
For info on a particular key action, press that key combination.\n\
\n";

static char stmsg1 [] = "\
Now press a key (or keys combination) for description of the assigned action.\
\n";

static int stmsg_nbln = 0;  /* number of lines in stmsg string */

static char retmsg[] = "--- Press ENTER (CR or RETURN) to return to the Edit session ---";
Short qq;
int i, idx;
char ch;
int cmd;
char *cmd_str, *str;
int oflag;
int col, lin, nbli;
extern void showstatus ();
extern int set_crlf ();
extern void reset_crlf ();
extern void check_keyboard ();
extern char verstr[];

    for ( i = 0 ; (ch = helparg[i]) ; i++ ) helparg[i] = tolower (ch);

    if ( helparg ) {
        if ( (strcmp (helparg, "key") == 0) || (strcmp (helparg, "?") == 0) ) {
	    return (keyfunc_ibmpc () );
	    }
    }

    if ( *nxtop )
	return CRTOOMANYARGS;

    col = cursorcol; lin = cursorline;
    savecurs ();
    ( *term.tt_clear ) ();
    ( *term.tt_home ) ();
    poscursor (0, term.tt_height -1);
    oflag = set_crlf ();

    if (   (helparg == NULL) || (*helparg == '\0')
        || ( strcmp (helparg, "keyhelp") == 0) ) {
	if ( ! stmsg_nbln ) {
	    char ch, *cp;
	    for ( cp = stmsg ; (ch = *cp) ; cp++ ) if ( ch == '\n' ) stmsg_nbln++;
	    }

	nbli = stmsg_nbln;
	fputs (stmsg, stdout);
	browse_keyboard (retmsg);
        }

    else if ( strncasecmp (helparg, "NewFeatures", strlen(helparg)) == 0 ) {
	fprintf (stdout, "%s\n\n", verstr);
	nbli = 2;
	(void) help_info ("New_features_Linux", &nbli);
	check_new_page (&nbli, 1);
	wait_keyboard (retmsg);
	}

    else if ( strcmp (helparg, "status") == 0 ) {
        showstatus ();
	wait_keyboard (retmsg);
	}

    else if ( strcmp (helparg, "keymap") == 0 ) {
	keymap_term ();
	}

    else if (   (strcmp (helparg, "commands") == 0)
             || (strcmp (helparg, "cmd") == 0) ) {
        browse_cmdhelp ();
	}

    else if (   (strcmp (helparg, "keyfunctions") == 0)
             || (strcmp (helparg, "keyf") == 0) ) {
        browse_keyfhelp ();
	}

    else if (   (strcmp (helparg, "keyboard_check") == 0)
	     || (strcmp (helparg, "kbchk") == 0) ) {
	check_keyboard ();
	}

    else if ( (idx = lookup (helparg, cmdtable)) >= 0 ) {
        cmd_str = get_cmd_name (idx);
        cmd = cmdtable[idx].val;
        str = cmdtable[idx].str;
        (void) help_cmd (cmd, cmd_str, str, helparg);
	wait_keyboard (retmsg);
	}

    else if ( idx == -2 ) {
	printf ("\n\r\"%s\" : Ambiguous Editor command\r\n", helparg);
	wait_keyboard (retmsg);
	}

    else {
	printf ("\n\r\"%s\" : Unknow Editor Command or HELP parameter\r\n", helparg);
	wait_keyboard (retmsg);
	}

    reset_crlf (oflag);
    mesg (TELALL+1, " ");
    fflush (stdout);
    fresh ();
    restcurs ();
    return CROK ;
}

/* ----------------------------------------------------------------------- */

S_looktbl *keyftable = NULL;
int keyftable_sz = 0;   /* nb of entry in keyftable */

static FILE *outst = NULL;  /* stdout or the file stream */

/* these flags are not yet used by the UNIX version */
static int Esc_flg = 0;     /* Escape was just pushed flag */
static int CtrlC_flg = 0;   /* Ctrl C was just pushed flag */



static int sort_looktb (obj1, obj2)
S_looktbl *obj1;
S_looktbl *obj2;

{
char *p1, *p2;
int cmp;
    p1 = ( isalpha (obj1->str[0]) ) ? obj1->str : &obj1->str[1];
    p2 = ( isalpha (obj2->str[0]) ) ? obj2->str : &obj2->str[1];
    cmp = strcmp (p1, p2);
    if ( cmp == 0 ) cmp = strcmp (obj1->str, obj2->str);
    return (cmp);
}


/* print_sorted : routine to print in columns a list of strings */
/* ------------------------------------------------------------ */

static void print_sorted (tbl, print_func, item_nb, nb_clm, nb_chr, nb_spc)
void *tbl;
void (*print_func)();
int item_nb;    /* number of items in sorted array */
int nb_clm;     /* number of columns */
int nb_chr;     /* nb of charcaters printed for each item */
int nb_spc;     /* number of spaces between each item */

{
char line[81];
int lnb, lastln;
int itemidx[16];
int i, j, k, l, idx;
int psiz;

    psiz = nb_chr + nb_spc;
    if ( (nb_spc < 1) || (nb_clm > 16)
	 || ((nb_clm * psiz) >= sizeof (line)) )
	return;

    lnb = item_nb / nb_clm;
    lastln = item_nb % nb_clm;
    for ( i = 0, k = 0 ; i < nb_clm ; i++ ) {
	itemidx[i] = k;
	k += ( i < lastln ) ? lnb +1 : lnb;
	}
    for ( i = 0 ; i <= lnb ; i ++ ) {
	memset (line, ' ', sizeof (line));
	l = (i == lnb) ? lastln : nb_clm;
	if ( l == 0 ) continue;
	for ( j = 0 ; j < l ; j ++ ) {
	    (* print_func) (tbl, &line[psiz*j], itemidx[j]);
	    itemidx[j] +=1;
	    }
	for ( idx = 0, k = sizeof (line) -1 ; k >= 0 ; k-- ) {
	    if ( line[k] == '\0' ) line[k] = ' ';
	    if ( (idx == 0) && (line[k] != ' ') ) idx = k+1;
	    }
	if ( idx == 0 ) idx = sizeof (line) -2;
	line[idx++] = '\n'; line[idx] = '\0';
	if ( (line [0] == ' ') && (line [1] == ' ') ) line [0] = '#';
	if ( outst == NULL ) outst = stdout;
	fprintf (outst, line);
	}
    return;
}


/* looktbl_print : local to print 1 item from key function table */
/* ------------------------------------------------------------- */

static void looktbl_print (tbl, line_pt, idx)
S_looktbl  *tbl;
char *line_pt;
int idx;

{
    (void) sprintf (line_pt + 2, "%3d %-9s ",
	     tbl[idx].val, tbl[idx].str);
}


/* sort_cmdt : local to sort the command table */
/* ------------------------------------------- */

static int sort_cmdt (cmdt_pt) 
S_looktbl **cmdt_pt;

{
    extern S_looktbl cmdtable[];
    int nb;

    for (nb = 1 ; ; nb++ ) if ( cmdtable[nb].str == NULL ) break;
    *cmdt_pt = (S_looktbl *) calloc (nb, sizeof (cmdtable[0]));
    if ( ! *cmdt_pt ) return (0);

    memcpy (*cmdt_pt, cmdtable, nb * sizeof (cmdtable[0]));
    qsort (*cmdt_pt, nb, sizeof (cmdtable[0]), (int (*)(const void *,const void *)) sort_looktb);
    return (nb);
}

/* browse_cmdhelp : local to browse all editor commands ("help cmd" processing) */
/* ---------------------------------------------------------------------------- */

static void browse_cmdhelp ()
{
    extern char * get_cmd_name ();
    extern S_looktbl cmdtable[];
    static int waitkb ();
    int *abv_tbl;

    int nb, i, di, j, idx, cmd, sz, cc;
    char *str, *str_abv, *cmd_str;
    S_looktbl *cmdt;

    nb = sort_cmdt (&cmdt);
    if ( nb <= 0 ) return;

    abv_tbl = (int *) calloc (nb, sizeof (int));
    if ( ! abv_tbl ) return;

    for ( i = 0, di = 1 ;  ; i += di ) {
        if ( i < 0 ) i = nb -1;
        if ( i >= nb ) i = 0;
        if ( abv_tbl[i] < 0 ) continue;

        str_abv = str = cmdt[i].str;
        idx = lookup (str, cmdtable);
        cmd_str = get_cmd_name (idx);
        cmd = cmdtable[idx].val;
        str = cmdtable[idx].str;

        /* check for abreviation or alias */
        if ( strcmp (str_abv, cmd_str) != 0 ) {
            sz = strlen (str_abv);
            if ( strncmp (str_abv, cmd_str, sz) == 0 ) str = cmd_str;
            else {
                for ( j = 0 ; j < nb ; j ++ ) {
                    if ( cmdt[j].val != cmd ) continue;
                    if ( strncmp (str_abv, cmdt[j].str, sz) != 0 ) continue;
                    if ( j == i ) continue;
                    str = cmdt[j].str;
                    break;
                }
            }
            /* a real abreviation */
            if ( strncmp (str_abv, str, sz) == 0 ) {
                for ( j = nb -1 ; j >= 0 ; j-- ) {
                    if ( j == i ) continue;
                    if ( strcmp (cmdt[j].str, str) == 0 ) break;
                }
                if ( j >= 0 ) {
                    abv_tbl [j] = idx;
                    abv_tbl [i] = -j;
                    continue;
                }
            }
        }

        if ( idx = abv_tbl[i] ) 
            str_abv = cmdtable[idx].str;
        (void) help_cmd (cmd, cmd_str, str, str_abv);
	cc = waitkb (NO);
        if ( cc > 0 ) break;
        di = ( cc < 0 ) ? -1 : 1;
    }
    free (cmdt);
    free (abv_tbl);
}


/* sort_keyft : local to sort the key functions table */
/* -------------------------------------------------- */

static int sort_keyft ()
{
    extern S_looktbl itsyms[];
    S_looktbl *keyft_pt;
    int nb;

    if ( keyftable ) return (keyftable_sz);    /* already init */

    keyftable_sz = 0;
    for (nb = 0 ; ; nb++ ) if ( itsyms[nb].str == NULL ) break;
    keyft_pt = (S_looktbl *) calloc (nb +1, sizeof (S_looktbl));
    if ( ! keyft_pt ) return (0);

    memcpy (keyft_pt, itsyms, nb * sizeof (S_looktbl));
    qsort (keyft_pt, nb, sizeof (S_looktbl), (int (*)(const void *,const void *)) sort_looktb);
    keyftable = keyft_pt;
    keyftable_sz = nb;
    return (nb);
}

/* help_keyf :
 *   Displays a message describing the effects of the key function
 *   whose lexed value is passed in "fcnt". 
 *   The command major name is given in fcnt_str.
 */

static int help_keyf (fcnt, fcnt_str)
int fcnt;
char *fcnt_str;
{
    int go;

    go = help_description ('~', 0, fcnt, NULL, fcnt_str, NULL);
    return (go);
}

/* waitkb : local to wait on keyboard input */
/* ---------------------------------------- */
/*
    return : -1 = UP or PAGE-UP
	      0 = any other key
	      1 = CCCMD or CCINT
*/

static void wait_msg (short nl) {
    extern int GetCccmd ();
    char ch;

    (*term.tt_addr)  (term.tt_height -1, 0);
    ch = '@' + GetCccmd ();
    if ( nl ) putchar ('\n');
    printf ("Push a key to continue, <Ctrl %c> or <cmd>Key to end, UP and DOWN to navigate\r", ch);
    fflush (stdout);
}

static int waitkb (short nl)

{
unsigned Short qq;

    wait_msg (nl);
    keyused = YES;
    qq = getkey (WAIT_KEY);
    (*term.tt_clear) ();
    if ( (qq == CCINT) || (qq == CCCMD) || CtrlC_flg || Esc_flg ) {
	if ( keyfile != NULL ) {
	    putc (CCINT, keyfile);
	    if ( numtyp > MAXTYP ) {
		flushkeys ();
		numtyp = 0;
		}
	    }
	return (1);
	}
    if ( (qq == CCMIPAGE) || (qq == CCMOVEUP) ) return (-1);
    return (0);
}

/* wait_key : local to wait on keyboard input, never wait if output is on file */
/* --------------------------------------------------------------------------- */
/*
    return : -1 = UP or PAGE-UP
	      0 = any other key (continue)
	      1 = CCCMD or INT
*/

static int wait_key ()
{
    if ( outst == NULL ) outst = stdout;
    if ( outst != stdout ) return (0);
    return ( waitkb (YES) );
}

/* wait_keyboard : wait for any key pushed on the keyboard */
/* ------------------------------------------------------- */
/*
    return the result of getkey
*/

int wait_keyboard (char *msg)
{
    Short qq;

    if ( msg ) {
	(*term.tt_addr)  (term.tt_height -1, 0);
	fputs (msg, stdout); fputc ('\r', stdout);
    }
    keyused = YES;
    qq = getkey (WAIT_KEY);
    return (qq);
}

/* wait_continue : wait for a key pushed on the keyboard */
/* ----------------------------------------------------- */
/*
    return :  0 = any other key
	      1 = CCCMD or CCINT
*/

int wait_continue (any_flg)
int any_flg;    /* ! 0 : wait in any case */
		/*   0 : wait only if the screen size < 43 rows */
{
    if ( !any_flg && (term.tt_height >= 43) ) return (0);
    (*term.tt_addr)  (term.tt_height -1, 0);
    return ( waitkb (NO) > 0 );
}

/* browse_keyfhelp : local to browse all key functions ("help keyf" processing) */
/* ---------------------------------------------------------------------------- */

static void browse_keyfhelp ()
{
    extern Flag verbose_helpflg;
    static int sort_keyftable ();

    int nb, i, di, fcnt, cc;
    Flag funcval_flg;
    char *fcnt_str, *sp;
    char msg[256];

    nb = sort_keyftable ();
    for ( i = 0, di = 1 ;  ; i += di ) {
        if ( i < 0 ) i = nb -1;
        if ( i >= nb ) i = 0;
        fcnt = keyftable[i].val;
        fcnt_str = keyftable[i].str;
	if ( fcnt == 0177 ) fcnt = CCBACKSPACE; /* special case for del char */
	funcval_flg = ( (fcnt == KBINIT) || (fcnt == KBEND) );
        (void) help_keyf (fcnt, fcnt_str);
	for ( sp = fcnt_str ; *sp ; sp++ ) putchar (toupper (*sp));
	funcval_flg = ( (fcnt == KBINIT) || (fcnt == KBEND) );
	sp = ( funcval_flg ) ? " Current value: " :
			       ( verbose_helpflg )
				    ? " Current Key Assignement: "
				    : " Current Key Assignement:\n";
	fputs (sp, stdout);
	memset (msg, 0, sizeof (msg));
	key_assigned (msg, sizeof (msg), fcnt, fcnt_str, funcval_flg);
	puts (msg);
	cc = waitkb (NO);
        if ( cc > 0 ) break;
        di = ( cc < 0 ) ? -1 : 1;
    }
}


/* print_cmdtable : local to print in column the command names */
/* ----------------------------------------------------------- */

static void print_cmdtable ()
{
    extern S_looktbl cmdtable[];
    int nb;
    S_looktbl *cmdt;

    nb = sort_cmdt (&cmdt);
    if ( nb <= 0 ) return;

    print_sorted (cmdt, looktbl_print, nb, 5, 14, 1);
    free (cmdt);

    puts ("\r\n\
Help on a specific command can be displayed with \"help <command name>\"\r\n\
To navigate in the Editor Commands description : \"help cmd\" or \"help commands\"\r\n");

}


/* sort_keyftable : local to sort the key function names */
/* ----------------------------------------------------- */

static int sort_keyftable ()
{
    return (sort_keyft ());
}


/* print_keyftable : local to print in column the key function names */
/* ----------------------------------------------------------------- */

static void print_keyftable ()
{
    int sz;

    sz = sort_keyftable ();
    print_sorted (keyftable, looktbl_print, sz, 5, 14, 1);
#ifdef WIN32
    mouse_info ();
#endif

}


/* display_keyftable : local to display in column the key assignement function names */
/* --------------------------------------------------------------------------------- */

static void display_keyftable ()
{
    print_keyftable ();

    puts ("\r\n\
Help on a specific key action can be displayed with \"help\"\r\n");
    puts ("\
Keys assigned to a given function can be displayed with\r\n\
    \"help key <key function name>\" or \"help ? <key function name>\"\r\n\
To navigate in the Key Functions description :\r\n\
    \"help keyf\" or \"help keyfunctions\"\r\n");

}
/* ======================================================================== */
