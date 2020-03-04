#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

/* * * * * * * * * * * * * * * * * * * * * */
/*** 2 =  Ann Arbor Ambassador  40 lines ***/
/* * * * * * * * * * * * * * * * * * * * * */

#define ESC 033

inlex_a1(lexp, count)
char *lexp;
int *count;
{
	register char *ip, *op;
	register int chr;
	char *tbl;
	int nr;
	static char tfunc[] = {     /* functions preceded by ESC O */
	    CCUNAS1,        /* @                           */
	    CCUNAS1,        /* a   (EXIT)                  */
	    CCUNAS1,        /* b   (REFRESH)               */
	    CCUNAS1,        /* c   (WIN)                   */
	    CCCHWINDOW,     /* d   change port             */
	    CCSETFILE,      /* e   set file                */
	    CCUNAS1,        /* f   (DO)                    */
	    CCOPEN,         /* g   insert                  */
	    CCCLOSE,        /* h   delete                  */
	    CCUNAS1,        /* i   (RESTORE)               */
	    CCPICK,         /* j   pick                    */
	    CCMARK,         /* k   mark                    */
	    CCINSMODE,      /* l   insert mode             */
	    CCUNAS1,        /* m   (EXIT)                  */
	    CCUNAS1,        /* n   (REFRESH)               */
	    CCUNAS1,        /* o   (WIN)                   */
	    CCCHWINDOW,     /* p   change port             */
	    CCSETFILE,      /* q   set file                */
	    CCUNAS1,        /* r   (DO)                    */
	    CCOPEN,         /* s   insert                  */
	    CCCLOSE,        /* t   delete                  */
	    CCUNAS1,        /* u   (RESTORE)               */
	    CCPICK,         /* v   pick                    */
	    CCMARK,         /* w   mark                    */
	    CCINSMODE,      /* x   insert mode             */
	    CCUNAS1,        /* y                           */
	    CCUNAS1,        /* z                           */
	};
	static char bfunc[] = {     /* functions preceded by ESC [ */
	    CCHOME,         /* @   home                    */
	    CCPLLINE,       /* a   +line                   */
	    CCMOVEUP,       /* b   up arrow                */
	    CCMISRCH,       /* c   -srch                   */
	    CCMIPAGE,       /* d   -page                   */
	    CCPLSRCH,       /* e   +srch                   */
	    CCUNAS1,        /* f                           */
	    CCPLPAGE,       /* g   +page                   */
	    CCMILINE,       /* h   -line                   */
	    CCREPLACE,      /* i   goto                    */
	    CCDELCH,        /* j   del ch (shift)          */
	    CCDELCH,        /* k   del ch                  */
	    CCHOME,         /* l   home (shift)            */
	    CCTABS,         /* m   s/r tab (shift)         */
	    CCMOVEDOWN,     /* n   down arrow              */
	    CCMOVERIGHT,    /* o   right arrow             */
	    CCTABS,         /* p   s/r tab                 */
	    CCLWINDOW,      /* q   left                    */
	    CCRWINDOW,      /* r   right                   */
	    CCMOVELEFT,     /* s   left arrow              */
	    CCUNAS1,        /* t                           */
	    CCUNAS1,        /* u                           */
	    CCUNAS1,        /* v                           */
	    CCUNAS1,        /* w                           */
	    CCUNAS1,        /* x                           */
	    CCUNAS1,        /* y                           */
	    CCBACKTAB,      /* z   backtab                 */
	};

	ip = op = lexp;
	for (nr = *count; --nr >= 0; ) {
	    if ((chr = *ip++ & 0177) != ESC) {
		if (chr == 03)
		    *op++ = CCINT;
		else if (chr == '\b')
		    *op++ = CCBACKSPACE;
		else if (chr == '\t')
		    *op++ = CCTAB;
		else if (chr == '\r')
		    *op++ = CCRETURN;
		else if (chr == 0177)
		    *op++ = CCCMD;
		else
		    *op++ = chr;
		continue;
	    }
	    if (--nr < 0) {
		*count = 1;
		*op = chr;
		return(op - lexp);
	    }
	    if ((chr = *ip++ & 0177) == 'O') {
		tbl = tfunc;
	    } else if (chr == '[') {
		tbl = bfunc;
	    } else if (chr == '6') {
		*op++ = CCCTRLQUOTE;
		continue;
	    } else {
		*op++ = CCUNAS1;
		continue;
	    }
	    if (--nr < 0) {
		*count = 2;
		op[0] = ESC;
		op[1] = chr;
		return(op - lexp);
	    }
	    *op++ = tbl[(*ip++ & 0137) - '@'];     /* ignore upper/lower case */
	}
	*count = 0;
	return(op - lexp);
}

ini_a1() {
	printf("%c[40;;;40p", ESC);
}
end_a1() {}
lt_a1() { P(ESC); P('['); P('D'); }
rt_a1() { P(ESC); P('['); P('C'); }
dn_a1() { P(ESC); P('['); P('B'); }
up_a1() { P(ESC); P('['); P('A'); }
cr_a1() { P (015); }
nl_a1() { P (015); P (012); }
clr_a1() {
	printf("%c[H%c[J%c", ESC, ESC, 014);
}
hm_a1() { P(ESC); P('['); P('H'); }
bsp_a1() { P (010); }
addr_a1(lin, col) {
	lin++;
	col++;
	P(ESC); P('[');
	P('0' + lin / 10); P('0' + lin % 10);
	P(';');
	P('0' + col / 10); P('0' + col % 10);
	P('H');
}
lad_a1(lin)
{
	lin++;
	P(ESC); P('[');
	P('0' + lin / 10); P('0' + lin % 10);
	P('d');
}
cad_a1(col)
{
	col++;
	P(ESC); P('[');
	P('0' + col / 10); P('0' + col % 10);
	P('`');
}
xlate_a1 (chr)
unsigned int chr;
{
	chr &= 0377;
	P(stdxlate[chr - FIRSTSPCL]);
}
S_term t_a1   = {
/* tt_ini0    */    nop,
/* tt_ini1    */    ini_a1,
/* tt_end     */    end_a1,
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
/* tt_lad     */    lad_a1,
/* tt_cad     */    cad_a1,
/* tt_xlate   */    xlate_a1,
/* tt_insline */    (int (*) ()) 0,
/* tt_delline */    (int (*) ()) 0,
/* tt_inschar */    (int (*) ()) 0,
/* tt_delchar */    (int (*) ()) 0,
/* tt_clreol  */    (int (*) ()) 0,
/* tt_defwin  */    (int (*) ()) 0,
/* tt_deflwin */    (int (*) ()) 0,
/* tt_erase   */    (int (*) ()) 0,
/* tt_nleft   */    3,
/* tt_nright  */    3,
/* tt_ndn     */    3,
/* tt_nup     */    3,
/* tt_nnl     */    2,
/* tt_nbsp    */    1,
/* tt_naddr   */    8,
/* tt_nlad    */    5,
/* tt_ncad    */    5,
/* tt_wl      */    1,
/* tt_cwr     */    1,
/* tt_pwr     */    1,
/* tt_axis    */    3,
/* tt_bullets */    YES,
/* tt_prtok   */    YES,
/* tt_width   */    80,
/* tt_height  */    40,
};

