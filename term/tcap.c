#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

/****************************************/
/**** 0 = terminal from termcap ****/

extern char *tgoto ();

#undef UP
#undef HO

char *BC;
char *ND;
char *DO;
char *UP;
char *CR;
char *NC;
char *CL;
char *CD;
char *HO;
char *CM;
char *TI;
char *TE;
char PC;

pch(ch) { putchar(ch); }

lt_tcap () { tputs(BC, 1, pch); }
rt_tcap () { tputs(ND, 1, pch); }
dn_tcap () { tputs(DO, 1, pch); }
up_tcap () { tputs(UP, 1, pch); }
cr_tcap () { tputs(CR, 1, pch); }
nl_tcap () { tputs(CR, 1, pch); tputs(DO, 1, pch); }
clr_tcap () { tputs(CL, 1, pch); }
clr1_tcap () { punt_tcap (); tputs(CD, 1, pch); }
hm_tcap () { tputs(HO, 1, pch); }
bsp_tcap () { tputs(BC, 1, pch); putchar(' '); tputs(BC, 1, pch); }
addr_tcap (lin, col) { tputs(tgoto(CM, col, lin), 1, pch); }
ini1_tcap () { tputs(TI, 1, pch); }
unit_tcap () { tputs(TE, 1, pch); }
punt_tcap () { tputs(tgoto (CM, icol, ilin), 1, pch); }

S_term t_tcap = {
/* tt_ini0    */    nop,
/* tt_ini1    */    ini1_tcap,
/* tt_end     */    unit_tcap,
/* tt_left    */    lt_tcap,
/* tt_right   */    rt_tcap,
/* tt_dn      */    dn_tcap,
/* tt_up      */    up_tcap,
/* tt_cret    */    cr_tcap,
/* tt_nl      */    nl_tcap,
/* tt_clear   */    clr_tcap,
/* tt_home    */    hm_tcap,
/* tt_bsp     */    bsp_tcap,
/* tt_addr    */    addr_tcap,
/* tt_lad     */    bad,
/* tt_cad     */    bad,
/* tt_xlate   */    xlate1,
/* tt_insline */    (int (*) ()) 0,
/* tt_delline */    (int (*) ()) 0,
/* tt_inschar */    (int (*) ()) 0,
/* tt_delchar */    (int (*) ()) 0,
/* tt_clreol  */    (int (*) ()) 0,
/* tt_defwin  */    (int (*) ()) 0,
/* tt_deflwin */    (int (*) ()) 0,
/* tt_erase   */    (int (*) ()) 0,
/* tt_nleft   */    1,
/* tt_nright  */    1,
/* tt_ndn     */    1,
/* tt_nup     */    1,
/* tt_nnl     */    1,
/* tt_nbsp    */    1,
/* tt_naddr   */    1,
/* tt_nlad    */    0,
/* tt_ncad    */    0,
/* tt_wl      */    1,
/* tt_cwr     */    1,
/* tt_pwr     */    1,
/* tt_axis    */    0,
/* tt_bullets */    NO,
/* tt_prtok   */    YES,
/* tt_width   */    80,
/* tt_height  */    24,
};

#define NG  -2
#define UNKNOWN -1
#define OK  0

Small
getcap(term)
char *term;
{
    char tcbuf[1024];
    char *cp;
    extern char *tgetstr();

    switch (tgetent(tcbuf, term)) {
    case -1:
    case 0:
	return UNKNOWN;
    }
    cp = salloc(256, YES);

    if (   (t_tcap.tt_width = tgetnum("co")) < 0
	|| (t_tcap.tt_height = tgetnum("li")) < 0
       )
	return NG;

    if ((HO = tgetstr("ho", &cp)) == NULL)
	t_tcap.tt_home = punt_tcap;

    if ((CL = tgetstr("cl", &cp)) == NULL) {
	if ((CD = tgetstr("cd", &cp)) == NULL)
	    return NG;
	t_tcap.tt_clear = clr1_tcap;
    }

    if ((CM = tgetstr("cm", &cp)) == NULL)
	    return NG;
    t_tcap.tt_naddr = strlen(tgoto(CM, 10, 10));

    if ((BC = tgetstr("bc", &cp)) == NULL)
	if (tgetflag("bs"))
	    BC = "\b";
	else {
	    t_tcap.tt_left = punt_tcap;
	    t_tcap.tt_nleft = t_tcap.tt_naddr;
	    t_tcap.tt_nbsp  = 2 * t_tcap.tt_naddr + 1;
	    goto endbc;
	}
    t_tcap.tt_nleft = strlen(BC);
    t_tcap.tt_nbsp = 2*strlen(BC)+1;
 endbc:

    if ((ND = tgetstr("nd", &cp)) == NULL) {
	t_tcap.tt_right = punt_tcap;
	t_tcap.tt_nright = t_tcap.tt_naddr;
    }
    else
	t_tcap.tt_nright = strlen(ND);

    if ((UP = tgetstr("up", &cp)) == NULL) {
	t_tcap.tt_up = punt_tcap;
	t_tcap.tt_nup = t_tcap.tt_naddr;
    }
    else
	t_tcap.tt_nup = strlen(UP);

    if (tgetflag("nc")) {
	t_tcap.tt_cret = punt_tcap;
	t_tcap.tt_nl = punt_tcap;
	t_tcap.tt_nnl = t_tcap.tt_naddr;
    }
    else if ((CR = tgetstr("cr", &cp)) == NULL)
	CR = "\r";

    if (   (DO = tgetstr("do", &cp)) == NULL
	&& (DO = tgetstr("nl", &cp)) == NULL
       ) {
	t_tcap.tt_dn = punt_tcap;
	t_tcap.tt_ndn = t_tcap.tt_naddr;
	t_tcap.tt_nl = punt_tcap;
	t_tcap.tt_nnl = t_tcap.tt_naddr;
    }
    else {
	t_tcap.tt_ndn = strlen(DO);
	t_tcap.tt_nnl = strlen(CR)+strlen(DO);
    }

    TI = tgetstr("ti", &cp);
    TE = tgetstr("te", &cp);
    Block {
	Reg1 char *pc;
	if (pc = tgetstr("pc", &cp))
		PC = *pc;
    }

    t_tcap.tt_wl = 0;   /* termcap doesn't guarantee what it will do */
 /* t_tcap.tt_pwr = tgetflag("am") ? (tgetflag("xn") ? 4 : 1) : 3; /**/
    t_tcap.tt_pwr = 0;  /* might need delay, so we punt */
    t_tcap.tt_cwr = 0;  /* termcap doesn't guarantee what it will do */
    t_tcap.tt_axis = 0;
    t_tcap.tt_nlad = 0;
    t_tcap.tt_ncad = 0;

    return OK;
}
