#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif


#define CTRL(x) ((x) & 31)
#define _BAD_ CCUNAS1

static
char lexa1[32] = {
    CCCMD       , /* <BREAK > enter parameter         */
    CCLWINDOW   , /* <cntr A> window left             */
    CCSETFILE   , /* <cntr B> set file                */
    CCINT       , /* <cntr C> interrupt               */
    CCDELCH     , /* <cntr D> character delete        */
    CCMIPAGE    , /* <cntr E> minus a page            */
    CCREPLACE   , /* <cntr F> replace                 */
    CCMARK      , /* <cntr G> mark a spot             */
    CCMOVELEFT  , /* <cntr H> move left               */
    CCTAB       , /* <cntr I> tab                     */
    CCMOVEDOWN  , /* <cntr J> move down 1 line        */
    CCMOVEUP    , /* <cntr K> move up 1 lin           */
    CCMOVERIGHT , /* <cntr L> forward move            */
    CCRETURN    , /* <cntr M> return                  */
    CCCLOSE     , /* <cntr N> delete                  */
    CCOPEN      , /* <cntr O> insert                  */
    CCPICK      , /* <cntr P> pick                    */
    CCMILINE    , /* <cntr Q> minus a line            */
    CCPLPAGE    , /* <cntr R> plus a page             */
    CCRWINDOW   , /* <cntr S> window right            */
    CCMISRCH    , /* <cntr T> minus search            */
    CCBACKTAB   , /* <cntr U> tab left                */
    CCUNAS1     , /* <cntr V> -- not assigned --      */
    CCPLLINE    , /* <cntr W> plus a line             */
    CCUNAS1     , /* <cntr X> -- not assigned --      */
    CCPLSRCH    , /* <cntr Y> plus search             */
    CCCHWINDOW  , /* <cntr Z> change window           */
    CCINSMODE   , /* <escape> insert mode             */
    CCCTRLQUOTE , /* <cntr \> knockdown next char     */
    CCTABS      , /* <cntr ]> set tabs                */
    CCHOME      , /* <cntr ^> home cursor             */
    CCBACKSPACE , /* <cntr _> backspace and erase     */
};

in_a1 (lexp, count)
char *lexp;
int *count;
{
    Reg1 int chr;
    Reg2 int nr;
    Reg3 char *icp;
    Reg4 char *ocp;

    icp = ocp = lexp;
    nr = *count;
    for (; nr > 0; nr--) {
	/* RAW MODE on V7 inputs all 8 bits so we and with 0177 */
	if ((chr = *icp++ & 0177) != CTRL('V')) {
	    if (   chr == CTRL('H')     /* BS KEY KLUDGE FOR NOW */
		|| chr == 0177          /* DEL KEY KLUDGE FOR NOW */
	       )
		*ocp++ = CCBACKSPACE;
	    else
		*ocp++ = chr >= ' ' ? chr : lexa1[chr];
	}
	else {
	    if (nr < 2) {
		icp--;
		goto nomore;
	    }
	    nr--;
	    chr = *icp++ & 0177;
	    if ('%' <= chr && chr <= '_') Block {
		static char xlt[] = {
		    CCPLLINE    , /* %   send      */
		    _BAD_       , /* &   reset     */
		    _BAD_       , /* '   --        */
		    _BAD_       , /* (   break     */
		    _BAD_       , /* ) S break     */
		    _BAD_       , /* *   pause     */
		    _BAD_       , /* +   return    */
		    CCCMD       , /* ,   move up   */
		    CCCMD       , /* - S move up   */
		    CCMARK      , /* .   move down */
		    CCMARK      , /* / S move down */
		    CCINSMODE   , /* 0   0         */
#ifndef  RANDold
		    CCMISRCH    , /* 1   4         */
		    CCMOVEDOWN  , /* 2   2         */
		    CCPLSRCH    , /* 3   6         */
		    CCMOVELEFT  , /* 4   1         */
		    CCHOME      , /* 5   8         */
		    CCMOVERIGHT , /* 6   3         */
		    CCMIPAGE    , /* 7   7         */
		    CCMOVEUP    , /* 8   5         */
		    CCPLPAGE    , /* 9   9         */
#else   RANDold
		    CCMOVELEFT  , /* 1   1         */
		    CCMOVEDOWN  , /* 2   2         */
		    CCMOVERIGHT , /* 3   3         */
		    CCMISRCH    , /* 4   4         */
		    CCMOVEUP    , /* 5   5         */
		    CCPLSRCH    , /* 6   6         */
		    CCMIPAGE    , /* 7   7         */
		    CCHOME      , /* 8   8         */
		    CCPLPAGE    , /* 9   9         */
#endif  RANDold
		    CCINSMODE   , /* :   .         */
		    _BAD_       , /* ;   tab       */
		    CCDELCH     , /* <   enter     */
		    CCBACKTAB   , /* = S tab       */
		    CCCLOSE     , /* >   erase     */
		    CCCLOSE     , /* ? S erase     */
		    CCOPEN      , /* @   edit      */
		    CCMILINE    , /* A   delete    */
		    CCMILINE    , /* B S delete    */
		    CCREPLACE   , /* C   insert    */
		    CCREPLACE   , /* D S insert    */
		    CCPLLINE    , /* E   print     */
		    CCPLLINE    , /* F S print     */
		    CCMIPAGE    , /* G   7 ctrl-sh */
		    _BAD_       , /* H   pf1       */
		    CCCTRLQUOTE , /* I   pf2       */
		    CCINT       , /* J   pf3       */
		    CCTABS      , /* K   pf4       */
		    CCCHWINDOW  , /* L   pf5       */
		    CCSETFILE   , /* M   pf6       */
		    CCLWINDOW   , /* N   pf7       */
		    CCRWINDOW   , /* O   pf8       */
		    CCLWORD     , /* P   pf9       */
		    CCRWORD     , /* Q   pf10      */
		    CCPICK      , /* R   pf11      */
		    CCERASE     , /* S   pf12      */
		    _BAD_       , /* T S pf1       */
		    _BAD_       , /* U S pf2       */
		    CCINT       , /* V S pf3       */
		    CCTABS      , /* W S pf4       */
		    CCCHWINDOW  , /* X S pf5       */
		    CCSETFILE   , /* Y S pf6       */
		    CCLWINDOW   , /* Z S pf7       */
		    CCRWINDOW   , /* [ S pf8       */
		    CCLWORD     , /* \ S pf9       */
		    CCRWORD     , /* ] S pf10      */
		    CCPICK      , /* ^ S pf11      */
		    CCERASE     , /* _ S pf12      */
		};
		*ocp++ = xlt[chr - '%'];
	    }
	    else
		*ocp++ = _BAD_;
       }
    }
nomore:
    Block {
	Reg1 int conv;
	*count = nr;     /* number left over - still raw */
	conv = ocp - lexp;
	while (nr-- > 0)
	    *ocp++ = *icp++;
	return conv;
    }
}

lt_a1 () { P ('h' & 31); }
rt_a1 () { fwrite ("\033[C", 3, 1, stdout); }
dn_a1 () { P (012); }
up_a1 () { fwrite ("\033[A", 3, 1, stdout); }
cr_a1 () { P (015); }
nl_a1 () { P (015); P (012); }
clr_a1 () { fwrite ("\033[;H\033[2J", 8, 1, stdout); delay (500); }
hm_a1 () { fwrite ("\033[;H", 4, 1, stdout); }
bsp_a1 () { P ('h' & 31); P (' '); P ('h' & 31); }
addr_a1 (lin, col) { printf ("\033[%d;%dH", lin + 1, col + 1); }

erase_a1 (num)
Scols num;
{
    printf ("\033[%dX", num);
}

#include <ctype.h>
ini0_a1 ()
{
#   define LINES_DEFAULT 48
    Reg1 char *cp;
    Reg2 char *numstr;
    int number;

    /* Decide on the number of lines */
    for (cp = tname; *cp && !isdigit (*cp); cp++)
	continue;
    if (   (cp = s2i (numstr = cp, &number))
	&& cp != numstr
       )
	term.tt_height = number;
    else
	term.tt_height = LINES_DEFAULT;
    return;
}

ini1_a1 ()
{
    /* Initialize the characteristics */
    Block {
	Reg1 char **cpp;
	static char *ini[] = {
			      /* A: xxx0 1011 xxxx xxxx */
	"\033[>27;h",         /*  SM ZKPCM */
	"\033[>25;29l",       /*  RM ZMBM,ZRLM */
			      /* B: xxxx xxxx 100x xxxx xxxx 0xxx xxxx xx xx*/
	"\033[>12h",          /*  SM SRM */
	"\033[>40;2;37l",     /*  RM ZHDM,KAM,ZAXM */
			      /* C: xx xx xx x xxxx xxxx xxxx */
			      /* D: 0110 10xx */
	"\033[>33;34;35h",    /*  SM ZWFM,ZWBM,ZDDM */
	"\033[>20;30;36l",    /*  RM LNM,ZDBM,ZSPM */
	NULL
	};

	for (cpp = ini; *cpp; cpp++)
	    fwrite (*cpp, strlen (*cpp), 1, stdout);
    }
    return;
}

kini_a1 ()
{
#ifndef RANDold
    static char nonrepeat[] =
    ",-./013579(@?>=:/.-,ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_";
    static char repeat[] =
    "2468<";
#else   RANDold
    static char nonrepeat[] =
    ",-./046789(@?>=:/.-,ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_";
    static char repeat[] =
    "1235<";
#endif  RANDold

    /* Initialize the keys */
    kinix_a1 (nonrepeat, NO);
    kinix_a1 (repeat, YES);

    return;
}

kinix_a1 (str, repeat)
char *str;
{
    Reg1 char *cp;

    for (cp = str; *cp; cp++) {
	fputs ("\033P`", stdout);
	putchar (*cp);
	if (repeat)
	    putchar ('x');
	fputs ("~V", stdout);
	putchar (*cp);
	fputs ("\033\\", stdout);
    }
    return;
}

kend_a1()
{
    if (system ("ambas -pop"))
	fatalpr ("\nCan't exec 'ambas' to reset the terminal.\n");
    return;
}

S_kbd kb_a1 = {
/* kb_inlex */  in_a1,
/* kb_init  */  kini_a1,
/* kb_end   */  kend_a1,
};

S_term t_a1 = {
/* tt_ini0    */    ini0_a1,
/* tt_ini1    */    ini1_a1,
/* tt_end     */    nop,
/* tt_left    */    lt_a1,
/* tt_right   */    rt_a1,
/* tt_dn      */    dn_a1,
/* tt_up      */    up_a1,
/* tt_cret    */    cr_a1,
/* tt_nl      */    nl_a1,
/* tt_clear   */    clr_a1,
/* tt_home    */    hm_a1,
/* tt_bsp     */    bsp_a1,
/* tt_addr    */    addr_a1,
/* tt_lad     */    bad,
/* tt_cad     */    bad,
/* tt_xlate   */    xlate0,
/* tt_insline */    (int (*) ()) 0,
/* tt_delline */    (int (*) ()) 0,
/* tt_inschar */    (int (*) ()) 0,
/* tt_delchar */    (int (*) ()) 0,
/* tt_clreol  */    (int (*) ()) 0,
/* tt_defwin  */    (int (*) ()) 0,
/* tt_deflwin */    (int (*) ()) 0,
/* tt_erase   */    erase_a1,
/* tt_nleft   */    1,
/* tt_nright  */    3,
/* tt_ndn     */    1,
/* tt_nup     */    3,
/* tt_nnl     */    2,
/* tt_nbsp    */    3,
/* tt_naddr   */    8,
/* tt_nlad    */    0,
/* tt_ncad    */    0,
/* tt_wl      */    1,
/* tt_cwr     */    1,
/* tt_pwr     */    1,
/* tt_axis    */    0,
/* tt_bullets */    YES,
/* tt_prtok   */    YES,
/* tt_width   */    80,
/* tt_height  */    48,
};


