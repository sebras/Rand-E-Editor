#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#ifdef COMMENT
--------
file term/standard.c
    Standard stuff in support of
    terminal-dependent code and data declarations
#endif

char lexstd[32] = {
    CCCMD       , /* <BREAK > */
    CCCMD       , /* <cntr A> */
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
/* support separate row and column cursor addressing
/**/
bad ()
{
    fatal (FATALBUG, "Impossible cursor address");
}

nop () {}

/* Special characters */
char stdxlate[] = {
    0177,    /* BULCHAR bullet character */
    0177,    /* ESCCHAR escape character */
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
    P (stdxlate[chr - FIRSTSPCL]);
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
    else
	P (stdxlate[chr - FIRSTSPCL]);
}
