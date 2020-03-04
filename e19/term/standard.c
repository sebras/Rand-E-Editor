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

static unsigned char defxlate[] = {     /* default setting */
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
    '+',     /* BTMCH   top tab border */
    '+',     /* TTMCH   bottom tab border */
    ';',     /* ELMCH   empty left border */
    '.'      /* INMCH   inactive border */
};

static unsigned char ibmpcxlate[] = {
    0177,    /* ESCCHAR escape character */
	/*
	 * For terminals that don't support any BLOT character,
	 * their kbd_init() routine should reset BULCHAR to something
	 * else like '*'.
	 */
    0177,       /* BULCHAR bullet character */
    186,        /* LMCH  left border */
    186,        /* RMCH  right border */
    204,        /* MLMCH         more left border */
    185,        /* MRMCH         more right border */
    205,        /* TMCH  top border */
    205,        /* BMCH  bottom border */
    201,        /* TLCMCH  top left corner border */
    187,        /* TRCMCH  top right corner border */
    200,        /* BLCMCH       bottom left corner border */
    188,        /* BRCMCH  bottom right corner border */
    209,        /* BTMCH   top tab border */
    207,        /* TTMCH   bottom tab border */
    179,        /* ELMCH        empty left border */
    '.'         /* INMCH         inactive border */
    };

unsigned char *stdxlate = defxlate;  /* current special character set */

/* terminal character set routines */
/* ------------------------------- */
static void reset_ibmpcchar ()
{
    if ( stdxlate == ibmpcxlate )
	printf ("\033(B");  /* switch to default character set */
    stdxlate = defxlate;
}

void (*alt_specialchar (char *term_name)) ()
    /* function returning the pointer to associated reset function */
{
#ifdef __linux__
#ifdef __i386__
    if ( strncmp (term_name, "linux", 5) == 0 ) {
	stdxlate = ibmpcxlate;
	printf ("\033(U");  /* switch to PC character set */
	return (reset_ibmpcchar);
	}
#endif /* __i386__ */
#endif /* __linux__ */

    stdxlate = defxlate;    /* default char set */
    return (NULL);
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
#ifndef UNSCHAR
    chr &= 0377;
#endif
    if (chr >= FIRSTSPCL)
	P (stdxlate[chr - FIRSTSPCL]);
    else
	if (chr) P (chr);
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
    if (chr == ESCCHAR)
	P ('^');
    else if (chr >= FIRSTSPCL)
	P (stdxlate[chr - FIRSTSPCL]);
    else
	if (chr) P (chr);
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

