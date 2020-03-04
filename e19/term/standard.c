#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#define MY_CTRL(x) ((x) & 31)

#ifdef COMMENT
--------
file term/standard.c
    Standard stuff in support of
    terminal-dependent code and data declarations
#endif

unsigned char lexstd[32] = {
    CCCMD       , /* <BREAK > */
    CCCMD       , /* <cntr A> */    /* must be CCCMD */
    CCLWORD     , /* <cntr B> */
    CCBACKSPACE , /* <cntr C> */
    CCMILINE    , /* <cntr D> */
    CCMIPAGE    , /* <cntr E> */
    CCPLLINE    , /* <cntr F> */
    CCHOME      , /* <cntr G> */
    CCMOVELEFT  , /* <cntr H> */
    CCTAB       , /* <cntr I> */
    CCMOVEDOWN  , /* <cntr J> */
    CCMOVEUP    , /* <cntr K> */
    CCMOVERIGHT , /* <cntr L> */
    CCRETURN    , /* <cntr M> */
    CCRWORD     , /* <cntr N> */
    CCOPEN      , /* <cntr O> */
    CCPICK      , /* <cntr P> */
    CCUNAS1     , /* <cntr Q> */
    CCPLPAGE    , /* <cntr R> */
    CCUNAS1     , /* <cntr S> */
    CCMISRCH    , /* <cntr T> */
    CCMARK      , /* <cntr U> */
    CCCLOSE     , /* <cntr V> */
    CCDELCH     , /* <cntr W> */
    CCUNAS1     , /* <cntr X> */
    CCPLSRCH    , /* <cntr Y> */
    CCINSMODE   , /* <cntr Z> */
    CCINSMODE   , /* <escape> */
    CCINT       , /* <cntr \> */
    CCREPLACE   , /* <cntr ]> */
    CCERASE     , /* <cntr ^> */
    CCSETFILE   , /* <cntr _> */
};

/* use this for column or row cursor addressing if the terminal doesn't
 * support separate row and column cursor addressing
 **/
bad ()
{
    fatal (FATALBUG, "Impossible cursor address");
}

nop () {}

/* Special characters */

/* look up table for kb file */
S_looktbl bordersyms [] = {
    "blcmch",  BLCMCH  ,    /* bottom left corner border */
    "bmch",    BMCH    ,    /* bottom border */
    "brcmch",  BRCMCH  ,    /* bottom right corner border */
    "btmch",   BTMCH   ,    /* bottom tab border */
    "elmch",   ELMCH   ,    /* empty left border */
    "inmch",   INMCH   ,    /* inactive border */
    "lmch",    LMCH    ,    /* left border */
    "mlmch",   MLMCH   ,    /* more left border */
    "mrmch",   MRMCH   ,    /* more right border */
    "rmch",    RMCH    ,    /* right border */
    "tlcmch",  TLCMCH  ,    /* top left corner border */
    "tmch",    TMCH    ,    /* top border */
    "trcmch",  TRCMCH  ,    /* top right corner border */
    "ttmch",   TTMCH   ,    /* top tab border */
    "userchar",LASTSPCL +1, /* flag for user (1) or default (0) border char set */
    0,0        /* end of key functions */
    };

static struct _bsymbs_com {
    short val;
    char *str;
} bsymbs_com [] = {     /* comments for the border char definition */
    BLCMCH  ,   "bottom left corner border",
    BMCH    ,   "bottom border",
    BRCMCH  ,   "bottom right corner border",
    BTMCH   ,   "bottom tab border",
    ELMCH   ,   "empty left border",
    INMCH   ,   "inactive border",
    LMCH    ,   "left border",
    MLMCH   ,   "more left border",
    MRMCH   ,   "more right border",
    RMCH    ,   "right border",
    TLCMCH  ,   "top left corner border",
    TMCH    ,   "top border",
    TRCMCH  ,   "top right corner border",
    TTMCH   ,   "top tab border",
    LASTSPCL +1,"flag to use the user defined char set",
    0,0
    };

static unsigned char defxlate [NSPCHR] = {  /* default setting */
    0177,    /* ESCCHAR escape character */
	/*
	 * For terminals that don't support any BLOT character,
	 * their kbd_init() routine should reset BULCHAR to something
	 * else like '*'.
	 */
    0177,    /* BULCHAR bullet character */
    '|',     /* LMCH    left border */
    '|',     /* RMCH    right border */
    '<',     /* MLMCH   more left border */
    '>',     /* MRMCH   more right border */
    '-',     /* TMCH    top border */
    '-',     /* BMCH    bottom border */
    '+',     /* TLCMCH  top left corner border */
    '+',     /* TRCMCH  top right corner border */
    '+',     /* BLCMCH  bottom left corner border */
    '+',     /* BRCMCH  bottom right corner border */
    '+',     /* BTMCH   bottom tab border */
    '+',     /* TTMCH   top tab border */
    ';',     /* ELMCH   empty left border */
    '.'      /* INMCH   inactive border */
};

static unsigned char ibmpcxlate [NSPCHR] = {
    0177,   /* ESCCHAR escape character */
	/*
	 * For terminals that don't support any BLOT character,
	 * their kbd_init() routine should reset BULCHAR to something
	 * else like '*'.
	 */
    0177,   /* 0x7F BULCHAR bullet character */
    186,    /* 0xBA LMCH    left border */
    186,    /* 0xBA RMCH    right border */
    185,    /* 0xCC MLMCH   more left border */
    204,    /* 0xB9 MRMCH   more right border */
    205,    /* 0xCD TMCH    top border */
    205,    /* 0xCD BMCH    bottom border */
    201,    /* 0xC9 TLCMCH  top left corner border */
    187,    /* 0xBB TRCMCH  top right corner border */
    200,    /* 0xC8 BLCMCH  bottom left corner border */
    188,    /* 0xBC BRCMCH  bottom right corner border */
    209,    /* 0xD1 BTMCH   top tab border */
    207,    /* 0xCF TTMCH   bottom tab border */
    179,    /* 0xB3 ELMCH   empty left border */
    '.'     /* 0x2E INMCH   inactive border */
    };

static unsigned char vt200xlate [NSPCHR] = {
    0177,   /* ESCCHAR escape character */
	/*
	 * For terminals that don't support any BLOT character,
	 * their kbd_init() routine should reset BULCHAR to something
	 * else like '*'.
	 */
    0177,    /* 0x7F BULCHAR bullet character */
    0x19,    /*      LMCH    left border */
    0x19,    /*      RMCH    right border */
    0x16,    /*      MLMCH   more left border */
    0x15,    /*      MRMCH   more right border */
    0x12,    /*      TMCH    top border */
    0x12,    /*      BMCH    bottom border */
    0x0D,    /*      TLCMCH  top left corner border */
    0x0C,    /*      TRCMCH  top right corner border */
    0x0E,    /*      BLCMCH  bottom left corner border */
    0x0B,    /*      BRCMCH  bottom right corner border */
    0x18,    /*      BTMCH   top tab border */
    0x17,    /*      TTMCH   bottom tab border */
    0x1F,    /*      ELMCH   empty left border */
    '.'      /* 0x2E INMCH   inactive border */
    };

/* On linux system execute 'showcfont' to see the current set of chararcters */

/* xlate char set, can be dynamicaly updated from kbfile at startup */
static unsigned char userxlate [NSPCHR] = {0,0};    /* must be initialized to '\0'\ */

/* border character set pointer */
unsigned char *stdxlate = defxlate; /* current border character set pointer */
unsigned char *altxlate = defxlate; /* alternate border character set pointer */
static Flag usercharset_flg = YES;  /* start with the user char set */

/* strings to set and reset the char set table to be used during editing session */
/* ref"console_codes" man page for Linux console */
static unsigned char IBMPC_tc [] ="\033(U";    /* G0 -> translation table c */
static unsigned char IBMPC_ta [] ="\033(B";    /* G0 -> translation table a */

static unsigned char *   set_charset_strg = NULL;
static unsigned char * reset_charset_strg = NULL;

static char enter_unicode_mode [] = "\033%G";
static char leave_unicode_mode [] = "\033%@";

/* my_ch_to_utf8 : look into console-tools-0.3.3/lib/generic/unicode.c
    and console-tools-0.3.3/screenfonttools/showcfont.c
    for the magic way to display any graphic char with utf8 encoding !!
    To be check if it is working on other OS than Linux */

static char * my_ch_to_utf8 (unsigned char ch)
{
    static char utf [4];
    int c;

    /* Ref to UCS2 special value in range 0xF000..0xF1FF
	and in 'PRIVATE AREA' of the unicode man page on linux */
    c = ch + 0xF000;
    utf[0] = 0xe0 | (c >> 12);        /*  1110**** 10****** 10******  */
    utf[1] = 0x80 | ((c >> 6) & 0x3f);
    utf[2] = 0x80 | (c & 0x3f);
    utf[3] = 0;
    return utf;
}

void display_xlate (char *term_name)
{
    int i, j, idx;
    unsigned char c;
    char *symb, *cmt;
    char tstrg [80];

    puts (enter_unicode_mode);
    printf ("\nBorder character set for terminal type \'%s\'\n", term_name);
    for ( i = 0 ; idx = bordersyms[i].val ; i++ ) {
	if ( (idx < FIRSTSPCL) || (idx > LASTSPCL) ) continue;
	symb = bordersyms[i].str;
	c = userxlate [idx - FIRSTSPCL];
	cmt = "???";
	for ( j = 0 ; j < sizeof (bsymbs_com) ; j++ ) {
	    if ( idx == bsymbs_com[j].val ) {
		cmt = bsymbs_com[j].str;
		break;
	    }
	}
	memset (tstrg, 0, sizeof (tstrg));
	sprintf (tstrg, "<%s>:<0x%02X>", symb, c);
	printf ("%-20s # \'%s\' = %s\n", tstrg, my_ch_to_utf8 (c), cmt);
    }
    puts (leave_unicode_mode);
    putchar ('\n');
}

/* terminal character set routines */
/* ------------------------------- */
static void reset_charset ()
{
    if ( reset_charset_strg )
	(void) fputs ((char *) reset_charset_strg, stdout);
}

void (*alt_specialchar (char *term_name, Flag force)) ()
    /* function returning the pointer to associated reset function */
    /* if term_name is NULL, use the default set */
    /* force : define the alt set according to the term_name value */
{
    unsigned char *tmpxlate, *charset_strg;

    if ( userxlate[0] == '\0' ) {
	/* init the user xlate array */
	tmpxlate = defxlate;
	usercharset_flg = NO;
#ifdef __linux__
	if ( term_name ) {
	    if ( strncmp (term_name,"xterm", 5) == 0 ) {
		tmpxlate = vt200xlate;
		reset_charset_strg = NULL;
		set_charset_strg   = NULL;
	    }
#if defined ( __i386 ) + defined ( __i486 ) + defined ( __i586 ) + defined ( __i686 )
	    else if ( strncmp (term_name,"linux", 5) == 0 ) {
		usercharset_flg = YES;
		tmpxlate = ibmpcxlate;
#if 0   /* not any more in use with the unicode utf8 encoding */
		reset_charset_strg = IBMPC_ta;
		set_charset_strg   = IBMPC_tc;
#endif
	    }
#endif /* __i386 ... */
	}
#endif /* __linux__ */
	memcpy (userxlate, tmpxlate, sizeof (userxlate));
	altxlate = (usercharset_flg) ? userxlate : defxlate;
    }
    if ( force ) altxlate = (term_name) ? userxlate : defxlate;
    stdxlate = ( term_name ) ? altxlate : defxlate;     /* xlate char set */
    charset_strg = ( term_name ) ? set_charset_strg : reset_charset_strg;   /* char set */
    if ( charset_strg ) (void) fputs ((char *) charset_strg, stdout);
    return (reset_charset);
}

/* set_charset_val : set the border character from kbfile specification */
/*  kb file must provide in info in this syntax :
	<border symbol>:<display character code>
	eg : <lmch>:<0xBA>   IBM PC charter set
	  or <lmch>:"|"      default
*/
void set_charset_val (unsigned char idx, unsigned char val)
{
    if ( (idx > FIRSTMCH) && (idx <= LASTSPCL) )
	userxlate [idx - FIRSTSPCL] = val;
    else if ( idx == LASTSPCL +1 ) {
	usercharset_flg = ! ( (val == 0) || (val == '0') );
	altxlate = (usercharset_flg) ? userxlate : defxlate;
    }
}

#define P putchar

/* standard character translate, using stdxlate[] array */
xlate0 (chr)
#ifdef UNSCHAR
Uchar chr;
#else
int chr;
#endif
{
    unsigned char c;

#ifndef UNSCHAR
    chr &= 0377;
#endif
    if ( chr >= FIRSTSPCL ) {
	c = (stdxlate[chr - FIRSTSPCL]);
	if ( (c < '\040') || (c > '\177') ) {
	    /* to be check if it is working on other OS than Linux */
	    fputs (enter_unicode_mode, stdout);
	    fputs (my_ch_to_utf8 (c), stdout);
	    fputs (leave_unicode_mode, stdout);
	}
	else putchar (c);
    }
    else if (chr) putchar (chr);
}

/* standard character translate, using stdxlate[] array */
/* except using '^' for ESCCHAR */
xlate1 (chr)
#ifdef UNSCHAR
Uchar chr;
#else
int chr;
#endif
{
#ifndef UNSCHAR
    chr &= 0377;
#endif
    if ( chr == ESCCHAR ) putchar ('^');
    else xlate0 (chr);
/*
    else if (chr >= FIRSTSPCL)
	P (stdxlate[chr - FIRSTSPCL]);
    else
	if (chr) P (chr);
*/
}

kini_nocbreak()
{
#ifdef  CBREAK
    cbreakflg = NO;
#endif
}


Flag in_std_p ();

in_std (lexp, count)
char *lexp;
int *count;
{
    int nr;
    Uint chr;
    char *icp;
    char *ocp;

    icp = ocp = lexp;
    nr = *count;
    for (; nr > 0; nr--) {
		if (in_std_p (&icp, &ocp, &nr)) break;
    }
    Block {
	Reg1 int conv;
	*count = nr;     /* number left over - still raw */
	conv = ocp - lexp;
	while (nr-- > 0)
	    *ocp++ = *icp++;
	return conv;
    }
}

Flag in_std_p (ic, oc, n)
	char **ic, **oc;
	int (*n);
{
	char chr;

	/* RAW MODE on V7 inputs all 8 bits so we and with 0177 */
	if ((chr = *(*ic)++ & 0177) >= 32) {
	    if (chr == 0177)            /* DEL KEY KLUDGE FOR NOW */
		*(*oc)++ = CCBACKSPACE;
	    else
		*(*oc)++ = chr;
	}
	else if (chr == MY_CTRL ('X')) {
	    if (*n < 2) {
		(*ic)--;
		return (YES);
	    }
	    (*n)--;
	    chr = *(*ic)++ & 0177;
	    switch (chr) {
	    case MY_CTRL ('a'):
		*(*oc)++ = CCSETFILE;
		break;
	    case MY_CTRL ('b'):
		*(*oc)++ = CCSPLIT;
		break;
	    case MY_CTRL ('c'):
		*(*oc)++ = CCCTRLQUOTE;
		break;
	    case MY_CTRL ('e'):
		*(*oc)++ = CCERASE;
		break;
	    case MY_CTRL ('h'):
		*(*oc)++ = CCLWINDOW;
		break;
	    case MY_CTRL ('j'):
		*(*oc)++ = CCJOIN;
		break;
	    case MY_CTRL ('l'):
		*(*oc)++ = CCRWINDOW;
		break;
	    case MY_CTRL ('n'):
		*(*oc)++ = CCDWORD;
		break;
	    case MY_CTRL ('r'):
		*(*oc)++ = CCREPLACE;
		break;
	    case MY_CTRL ('t'):
		*(*oc)++ = CCTABS;
		break;
	    case MY_CTRL ('u'):
		*(*oc)++ = CCBACKTAB;
		break;
	    case MY_CTRL ('w'):
		*(*oc)++ = CCCHWINDOW;
		break;
	    default:
		*(*oc)++ = CCUNAS1;
		break;
	    }
	}
	else
	    *(*oc)++ = lexstd[chr];
	return (NO);
}

S_kbd kb_std = {
/* kb_inlex */  in_std,
/* kb_init  */  nop,
/* kb_end   */  nop,
};

