#ifdef COMMENT
--------
file e.iit.c
    initialize input (keyboard) table parsing and lookup
#endif

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif


#include <string.h>
#include "e.h"
#ifdef  KBFILE
#include "e.it.h"

extern Flag helpflg, verbose_helpflg, dbgflg;
extern char *salloc();
static void itadd ();

int kbfile_wline = 0;   /* line number of a duplicated string */

struct itable *ithead = NULLIT;

S_looktbl itsyms [] = {
    "+line",   CCPLLINE,
    "+page",   CCPLPAGE,
    "+sch",    CCPLSRCH,
    "+tab",    CCTAB,
    "+word",   CCRWORD,
    "-line",   CCMILINE,
    "-page",   CCMIPAGE,
    "-sch",    CCMISRCH,
    "-tab",    CCBACKTAB,
    "-word",   CCLWORD,
#ifdef LMCCMDS
    "abort",   CCABORT,
#endif
    "alt",     CCSETFILE,
    "bksp",    CCBACKSPACE,
#ifdef LMCCMDS
    "blot",    CCBLOT,
    "box",     CCBOX,
#ifdef LMCCASE
    "caps",    CCCAPS,
    "ccase",   CCCCASE,
#endif
#endif
    "cchar",   CCCTRLQUOTE,
#ifdef LMCCMDS
    "center",  CCCENTER,
#endif
    "chwin",   CCCHWINDOW,
#ifdef LMCCMDS
    "clear",   CCCLEAR,
#endif
    "close",   CCCLOSE,
#ifdef LMCCMDS
    "cltabs",  CCCLRTABS,
#endif
    "cmd",     CCCMD,
#ifdef LMCCMDS
    "cover",   CCCOVER,
#endif
    "dchar",   CCDELCH,
    "del",     0177,
    "down",    CCMOVEDOWN,
#ifdef LMCDWORD
    "dword",   CCDWORD,
#endif
    "edit",    CCSETFILE,
    "erase",   CCERASE,
#ifdef LMCCMDS
    "exit",    CCEXIT,
    "fill",    CCFILL,
    "fnavigate", CCFNAVIG,
#endif
#ifdef LMCCMDS
#ifdef LMCHELP
    "help",    CCHELP,
#endif
#endif
    "home",    CCHOME,
    "insmd",   CCINSMODE,
    "int",     CCINT,
    "join",    CCJOIN,
#ifdef LMCCMDS
    "justify", CCJUSTIFY,
#endif
    "kbend",   KBEND,
    "kbinit",  KBINIT,
    "left",    CCMOVELEFT,
    "mark",    CCMARK,
#ifdef LMCCMDS
    "null",    CCNULL,
#endif
    "open",    CCOPEN,
#ifdef LMCCMDS
    "overlay", CCOVERLAY,
#endif
    "pick",    CCPICK,
#ifdef LMCCMDS
    "quit",    CCQUIT,
#endif
#ifdef LMCCMDS
    "range",   CCRANGE,
    "redraw",  CCREDRAW,
#endif
    "replace", CCREPLACE,
    "ret",     CCRETURN,
    "right",   CCMOVERIGHT,
    "split",   CCSPLIT,
    "srtab",   CCTABS,
#ifdef LMCCMDS
    "stopx",   CCSTOPX,
#endif
    "tab",     CCTAB,
#ifdef LMCCMDS
    "track",   CCTRACK,
#endif
    "undef",   CCUNAS1,
    "up",      CCMOVEUP,
    "wleft",   CCLWINDOW,
#ifdef LMCAUTO
    "wp",      CCAUTOFILL,
#endif
    "wright",  CCRWINDOW,
    0,0,            /* end of key functions */

    "esc",     033,
    "del",     0177,
    "space",   040,
    "quote",   042,
    0,0
 };

char *kbinistr;
char *kbendstr;
int  kbinilen;
int  kbendlen;

static struct itable *it_leave_ctrlc = NULL;
static struct itable  it_leave_ctrlc_ref;
static int ctrlc_is_ret;
static char ccreturn_val [2] = { CCRETURN, 0 };

/* Get the <Ctrl C> value */
int get_ctrlc_fkey ()
{
    if ( ctrlc_is_ret || !it_leave_ctrlc ) return CCRETURN;
    return (int) *(it_leave_ctrlc->it_val);
}

/* switch <Ctrl C> to <ret> and back (for help output) */
void switch_ctrlc (ret)
Flag ret;
{
    if ( ctrlc_is_ret || !it_leave_ctrlc ) return;
    it_leave_ctrlc->it_len = ( ret ) ? 1 : it_leave_ctrlc_ref.it_len;
    it_leave_ctrlc->it_val = ( ret ) ? ccreturn_val : it_leave_ctrlc_ref.it_val;
}

/* include_cccmd : force <Ctrl M> to be <ret>,
 * if <Ctrl C> is not allocated, set to <ret> (used in help output),
 * and try to add a Control char for <cmd> function
 */
static void include_cccmd ()
{
    extern int itget ();
    extern int itgetleave ();
    extern struct itable *ithead;

    char ch, ch0, cmd[256], *ch_pt;
    int cc, nb;

    ch = ch0 = ('M' & '\037');
    ch_pt = &ch; nb = 1;
    cc = itget (&ch_pt, &nb, ithead, cmd);
    if ( (cc != 1) || (cmd[0] != CCRETURN) ) {
	/* force <Ctrl M> to be CCRETURN */
	cmd[0] = CCRETURN ; cmd[1] = '\0';
	itadd (&ch0, 1, &ithead, cmd, 1, "-- Force setting of 'Ctrl M' to <ret> --", -1);
    }
    ch = ch0 = ('C' & '\037');
    ch_pt = &ch; nb = 1;
    cc = itget (&ch_pt, &nb, ithead, cmd);
    if ( cc == IT_NOPE ) {
	/* <Ctrl C> not allocated, force to CCRETURN */
	cmd[0] = CCRETURN ; cmd[1] = '\0';
	itadd (&ch0, 1, &ithead, cmd, 1, "-- Default setting of 'Ctrl C' to <ret> --", -1);
    }
    /* setup data to be use for temporary overwrite <Crtl C> with <ret> */
    cmd[0] = ('C' & '\037'); cmd[1] = '\0';
    cc = itgetleave (cmd, &it_leave_ctrlc, ithead);
    ctrlc_is_ret = (   (it_leave_ctrlc->it_len == 1)
		    && (*(it_leave_ctrlc->it_val) == CCRETURN) );
    it_leave_ctrlc_ref = *it_leave_ctrlc;

    /* found an empty slot in Ctrl A ... Ctrl Z */
    ch0 = 0;
    for ( ch = ('Z' & '\037') ; ch >= ('A' & '\037') ; ch-- ) {
	ch_pt = &ch; nb = 1;
	cc = itget (&ch_pt, &nb, ithead, cmd);
	if ( (cc == 1) && (cmd[0] == CCCMD) ) return;
	if ( cc == IT_NOPE ) ch0 = ch;     /* empty slot */
    }
    if ( ch0 >= ('A' & '\037') ) {
	cmd[0] = CCCMD; cmd[1] = '\0';
	itadd (&ch0, 1, &ithead, cmd, 1, "-- Default internal setting of <cmd> --", -1);
    }
}

static char * check_include (line, path)
char *line, *path;
{
    static const char incl[] = "include ";
    char *fname, *str;
    char c;
    int sz;

    fname = line;
    sz = strlen (incl);
    if ( strncmp (fname, incl, sz) != 0 ) return NULL;
    fname += sz;
    for ( ; c = *fname++ ; ) if ( c != ' ') break;
    if ( ! c ) return NULL;
    if ( (c != '<') && (c != '"') ) return NULL;
    if ( c == '<' ) c = '>';
    str = strchr (fname, c);
    if ( ! str ) return NULL;

    *str = '\0';    /* ignore the rest of the line */
    if ( *fname != '/' ) {
	/* this include file path name is relative to the including file */
	str = strrchr (path, '/');
	if ( str ) {
	    str++;
	    sz = str - path;
	    /* make room for path */
	    memmove (fname + sz, fname, strlen (fname) +1);
	    memcpy (fname, path, sz);
	}
    }
    return fname;
}


static Flag process_kbfile (filename, level)
char *filename;
int level;
{
    char line[TMPSTRLEN], string[TMPSTRLEN], value[TMPSTRLEN];
    FILE *f;
    int lnb, str_len, val_len;
    char *incl_fname;
    char c;
    Flag cc;

    if ( verbose_helpflg ) printf ("Process kbfile : \"%s\"\n", filename);
    if ( level > 32 ) {
	if ( verbose_helpflg )
	    printf ("  Too large included depth %d : this is probably a loop of include\n", level);
	return NO;
    }
    if ((f = fopen (filename, "r")) == NULL) {
	/* ---- old version
	getout (YES, "Can't open keyboard file \"%s\"", filename);
	---- */
	if ( verbose_helpflg ) printf ("ERROR fopen error %d : %s\n", errno, strerror (errno));
	else if ( helpflg || dbgflg )
	    printf ("\n\"%s\"ERROR  fopen error %d :\n\n  %s\n", filename, errno, strerror (errno));
	return NO;
    }

    for ( lnb = 1 ; fgets (line, sizeof line, f) != NULL; lnb++ ) {
	line[strlen (line)-1] = '\0';   /* remove newline */
	if ( verbose_helpflg ) printf ("%3d  %s\n", lnb, line);
	if ( line[0] == '#' ) {         /* FP 14 Jan 2000 */
	    incl_fname = line+1;
	    for ( ; c = *incl_fname ; incl_fname++ ) if ( c != ' ') break;
	    incl_fname = check_include (incl_fname, filename);
	    if ( incl_fname ) {
		cc = process_kbfile (incl_fname, level +1);
		if ( verbose_helpflg ) printf ("---Back in kbfile : \"%s\"\n", filename);
	    }
	    continue;
	}
	if ( line[0] == '!'             /* add FP 14 Jan 2000 */
	     || line[0] == '\0' )       /* gww 21 Feb 82 */
	    continue;                   /* gww 21 Feb 82 */
	itparse (line, string, &str_len, value, &val_len);
	switch (string[0]) {
	case KBINIT:
	    kbinilen = val_len;
	    kbinistr = salloc (kbinilen, YES);
	    move (value, kbinistr, kbinilen);
	    break;
	case KBEND:
	    kbendlen = val_len;
	    kbendstr = salloc (kbinilen, YES);
	    move (value, kbendstr, kbendlen);
	    break;
	default:
	    itadd (string, str_len, &ithead, value, val_len, line, lnb);
	}
    }
    fclose (f);
    return (YES);
}

Flag
getkbfile (filename)
char *filename;
{
    /* use the Rand options "-verbose -help" to debug the kbfile */
    extern void overwrite_PF1PF4 ();
    Flag cc;

    cc = process_kbfile (filename, 1);
    include_cccmd ();
    overwrite_PF1PF4 ();
#ifdef  DEBUG_KBFILE
    itprint (ithead, 0);
#endif
    if ( verbose_helpflg ) printf ("\nEnd of Processing kbfile(s)\n");
    return (cc);
}

static void itadd (str, str_len, headp, val, val_len, line, line_nb)
char *str;              /* Character string */
struct itable **headp;  /* Pointer to head (where to start) */
char *val;              /* Value */
int str_len, val_len;
char *line;             /* For debugging */
int line_nb;
{
    char * itsyms_by_val ();
    struct itable *it;         /* Current input table entry */
    struct itable *pt;         /* Previous table entry */

    if (str_len == 0)
	getout (YES, "kbfile invalid prefix in %s\n", line);
    for (it = *headp; it != NULLIT; pt = it, it = it->it_next) {
	if (it->it_c == *str) {         /* Character match? */
	    if (it->it_leaf) {          /* Can't add this */
		/*
		getout (YES, "kbfile duplicate string in %s\n", line);
		*/
		if ( verbose_helpflg ) {
		    printf ("WARNING : kbfile overwrite previously defined value : <%s>\n",
			    itsyms_by_val ((short) *it->it_val));
		    kbfile_wline = line_nb;
		}
		/* overwrite the previously defined value */
		if ( val_len != it->it_len ) {
		    sfree (it->it_val);
		    it->it_val = salloc (val_len, YES);
		    it->it_len = val_len;
		}
		move (val, it->it_val, val_len);
	    } else      /* Go down the tree */
		itadd (str+1, str_len-1, &it->it_link, val, val_len, line, line_nb);
	    return;
	}
    }
    it = (struct itable *) salloc (sizeof *it, YES);           /* Get new node */
    if (*headp == 0)                    /* Change head if tree was empty */
	*headp = it;
    else
	pt->it_next = it;               /* Otherwise update prev node */
    it->it_c = *str++;                  /* Save current character */
    it->it_next = 0;
    if (--str_len > 0) {                /* Is this a leaf? */
	it->it_leaf = 0;                /* No */
	it->it_link = 0;
	itadd (str, str_len, &it->it_link, val, val_len, line, line_nb);
    } else {
	it->it_leaf = 1;
	it->it_val = salloc (val_len, YES);
	it->it_len = val_len;
	move (val, it->it_val, val_len);
    }
    return;
}

itparse (inp, strp, str_lenp, valp, val_lenp)
register char *inp;
char *strp, *valp;      /* Pointers to string to match and value to return */
int *str_lenp, *val_lenp; /* Where to put the respective lengths */
{
    register char c;
    unsigned int n;
    int i;
    int gotval = 0;
    register char *outp = strp;
    char tmpstr[50], *tp;
    char *line = inp;            /* Save for error messages */

    while ((c = *inp++) != '\0') {
	switch (c) {
	    case '"':       /* String "foo bar" (with no quotes) */
		while ((c = *inp++) != '"')
		    *outp++ = c;
		break;
	    case '<':
		if ((c = *inp) >= '0' && c <= '7') {
		    for (n = 0; (c = *inp++) != '>'; ) {
			if (c < '0' || c > '7')
			    getout (YES, "kbfile bad digit in %s\n", line);
			n = n*8 + (c-'0');
		    }
		    if (n > 0377)
			getout (YES, "Number %d too big in kbfile\n", n);
		    *outp++ = (char) n;
		} else {
		    for (tp = tmpstr; (c = *inp++) != '>'; ) {
			if (c == '\0')
			    getout (YES, "kbfile mismatched < in %s\n", line);
			*tp++ = c;
		    }
		    *tp = '\0';
		    i = lookup (tmpstr, itsyms);
		    if (i < 0)
			getout (YES, "Bad symbol %s in kbfile\n", tmpstr);
		    *outp++ = (char) itsyms[i].val;
		}
		break;
	    case '^':
		c = *inp++;
		if (c != '?' && (c < '@' || c > '_'))
		    getout (YES, "kbfile bad char ^<%o> in %s\n", c, line);
		*outp++ = c^0100;
		break;
	    case ':':
		*str_lenp = outp - strp;
		if (gotval++)
		    getout (YES, "kbfile too many colons in %s\n", line);
		if (*str_lenp > TMPSTRLEN)
		    goto toolong;
		outp = valp;
		break;
	    default:
		getout (YES, "kbfile bad char <%o> in %s\n", c, line);
	}
    }
    *val_lenp = outp - valp;
    if (*str_lenp > TMPSTRLEN)
toolong:
	getout (YES, "kbfile line too long %s\n", line);
    return;
}

/* escstrg : return a printable string from a string with control char */

char * escstrg (char *escst)
{
    static char st[128];
    char *sp, ch;

    memset (st, 0, sizeof (st));
    for ( sp = escst; (ch = *sp) ; sp++ ) {
	if ( ch == '\033' ) strcat (st, "<Esc>");
	else if ( ch == '\177' ) strcat (st, "<Bksp>");
	else if ( ch == '\t' ) strcat (st, "<Tab>");
	else if ( ch < ' ' ) {
	    st[strlen(st)] = '^';
	    st[strlen(st)] = ch + '@';
	}
	else st[strlen(st)] = ch;
    }
    return (st);
}

char * itsyms_by_val (short val)
{
typedef struct lookuptbl
{   char *str;
    short val;
} S_looktbl;

    int i;
    char *sp;

    for ( i = 0 ; (sp = itsyms [i].str) ; i++ ) {
	if ( itsyms [i].val == val ) break;
    }
    return (sp);
}

/* -------------------------------------- */

/* it_walk : get entry in the it tree */
/* ---------------------------------- */
/*
    On the first call head must point to the head of the it list
	and n must be 0 (current level call in the recursion).
	strg must be large enough to receive the longer it string
*/


static struct itable *it_walk (head, n, strg)   /* n should be 0 the first time */
struct itable *head;
int n;
char *strg;
{
    struct itable *it, *it1;
    int i;
    char c, c1;
    unsigned char *cp, *sp;

    for (it = head; it != NULLIT; it = it->it_next) {
	c = it->it_c;
	c1 = strg [n];
	if ( c1 && (c != c1) )
	    continue;    /* continue to found the last processed leaf */
	strg [n] = c;
	if ( c1 == '\0' ) strg [n+1] = '\0';
	if (it->it_leaf) {
	    if ( ! c1 )
		return (it);    /* a new leaf is found : process it */
	    strg [n] = '\0';    /* this leaf is already processed */
	}
	else {
	    /* go to child leaf */
	    it1 = it_walk (it->it_link, n+1, strg);
	    if ( it1 )
		return (it1);   /* a new leaf must be processed */
	}
    }
    if (n > 0 ) strg [n -1] = '\0';  /* continue on the previous level */
    return (NULL);    /* no more leaves on this level */
}

/* it_listall : display all the elements of the it tree */
/* ---------------------------------------------------- */

static int it_listall ()
{
    int i, nb, nb_leaves;
    struct itable *it;
    unsigned char *cp, *sp;
    char strg [80];

    memset (strg, 0, sizeof (strg));
    nb_leaves = 0;
    for ( nb = 0 ; nb < 10000 ; nb ++ ) {
	it = it_walk (ithead, 0, strg);
	if ( ! it ) break;  /* no more leaves */

	nb_leaves++;
	printf ("%3d : %-10s = ", nb_leaves, escstrg (strg));
	for (cp = it->it_val, i = it->it_len ; i-- > 0 ; ) {
	    sp = itsyms_by_val ((short) *cp);
	    printf (" (%#4o)", *cp++);
	    if ( sp ) printf ("<%s>", sp);
	}
	putchar ('\n');
    }
    if ( it ) {
	printf ("======= error in it structure ==========\n");
	return -1;
    }
    return (nb_leaves);
}

/* itwalk : get a string for a given value */
/* --------------------------------------- */
/*
    On the first call head must point to the head of the it list
	and n must be 0 (index of the beegining of strg)
*/

static void itwalk (int val, struct itable **it_pt, int *n, char *strg)
{
    struct itable *it, *it_child;
    int n_child;
    int i;
    char c;
    unsigned char *cp;

    for (it = *it_pt; it != NULLIT; it = it->it_next) {
	*it_pt = it;
	c = it->it_c;
	strg [*n] = c;
	strg [*n+1] = '\0';
	if (it->it_leaf) {
	    for (cp = it->it_val, i = it->it_len; i-- > 0; ) {
		if ( *cp = val ) return;
	    }
	}
	else {
	    /* go to child */
	    it_child = it->it_link;
	    n_child = *n +1;
	    itwalk (val, &it_child, &n_child, strg);
	}
    }
    return;
}




#ifdef DEBUG_KBFILE
#include <ctype.h>

itprint (head, n)        /* n should be 0 the first time */
struct itable *head;
int n;
{
    register struct itable *it;
    int i;
    char c;
    char *cp;

    for (it = head; it != NULLIT; it = it->it_next) {
	for (i = 0; i < n; i++)
	    putchar (' ');
	c = it->it_c;
	if (isalnum (c))
	    printf ("%c  ", c);
	else
	    printf ("<%3o>", c);
	if (it->it_leaf) {
	    printf ("=");
	    for (cp = it->it_val, i = it->it_len; i-- > 0; )
		printf ("<%o>", *cp++);
	    printf (" (len %d)\n", it->it_len);
	}
	else {
	    printf ("\n");
	    itprint (it->it_link, n+2);
	}
    }
    return;
}
#endif

/* print all the elements of the it table tree (escape seq to key function) */
/* ------------------------------------------------------------------------ */

int print_it_escp ()
{
    int nb;

    printf ("\n---- Escape Sequence to key function -----\n", nb);
    nb = it_listall ();
    printf ("---- defined entries in Escape Seq table : %d -----\n", nb);
    return nb;
}

/* it_value_to_string : return all the string for a given it value */
/* --------------------------------------------------------------- */
/*  if keyflg : return the keys which are assigned to the value */

int it_value_to_string (int value,
		    Flag key_flg,   /* to get the keyboard assignement */
		    char *buff,     /* buffer to receive the result */
		    int buff_sz)    /* buffer size */
{
    int i, sz, nb_leaves;
    struct itable *it;
    unsigned char *cp, *sp;
    char strg [80];

    nb_leaves = 0;
    memset (strg, 0, sizeof (strg));
    for ( ; ; ) {
	it = it_walk (ithead, 0, strg);
	if ( ! it ) break;  /* no more leaves */
	cp = it->it_val;    /* if more than 1 value, it must be <cmd><xxx> */
	i = it->it_len;
	if ( cp[i-1] != value ) continue;

	/* TO BE COMPLETED
	if ( key_flg ) {
	} else {
	}
	*/

	sz = strlen (strg);
	if ( strlen (buff) + sz +2 < buff_sz ) {
	    nb_leaves++;
	    strcat (buff, strg);
	    strcat (buff, " ");
	}
    }
    return (nb_leaves);
}

void print_all_ccmd ()
{
    int i, nb, nb_it, sz, val;
    char *strg, *val_strg;
    char ccmd [256];

    printf ("\n---- Key Function to Escape Sequence assignement -----\n");
    nb = 0;
    for ( i = 0 ; itsyms[i].str ; i++ ) {
	memset (ccmd, 0, sizeof (ccmd));
	val = itsyms[i].val;
	val_strg = itsyms[i].str;
	memset (ccmd, 0, sizeof (ccmd));
	nb_it = it_value_to_string (val, NO, ccmd, sizeof (ccmd));
	if ( ! nb_it ) continue;
	if ( !ccmd[0] ) continue;
	sz = printf ("(%#4o) <%s>", val, val_strg);
	for ( ; sz < 17 ; sz++ ) putchar (' ');
	printf (": %s\n", escstrg (ccmd));
	nb++;
    }
    printf ("---- assigned entries : %d (defined key functions : %d) -----\n", nb, i);
}

#endif
