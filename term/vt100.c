#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#define CTRL(x) ((x) & 31)
#define _BAD_ CCUNAS1


in_vt100 (lexp, count)
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
	if ((chr = *icp++ & 0177) >= 040) {
	    if (chr == 0177)         /* DEL KEY KLUDGE FOR NOW */
		*ocp++ = CCBACKSPACE;
	    else
		*ocp++ = chr;
	}
	else if (chr == 033) {
	    if (nr < 2) {
		icp--;
		goto nomore;
	    }
	    nr--;
	    chr = *icp++ & 0177;
	    if (chr == '[') {
		if (nr < 2) {
		    icp -= 2;
		    nr++;
		    goto nomore;
		}
		nr--;
		switch (chr = *icp++ & 0177) {
		case 'A':
		    *ocp++ = CCMOVEUP;
		    break;

		case 'B':
		    *ocp++ = CCMOVEDOWN;
		    break;

		case 'C':
		    *ocp++ = CCMOVERIGHT;
		    break;

		case 'D':
		    *ocp++ = CCMOVELEFT;
		    break;

		default:
		    *ocp++ = CCUNAS1;
		    break;
		}
	    }
	    else if (chr == 'O') {
		if (nr < 2) {
		    icp -= 2;
		    nr++;
		    goto nomore;
		}
		nr--;
		chr = *icp++ & 0177;
		if ('l' <= chr && chr <= 'y') Block {
		    static char xlt[] = {
			CCPICK         , /* l  , */
			CCOPEN         , /* m  - */
			CCDELCH        , /* n  . */
			CCUNAS1        , /* o    */
			CCINSMODE      , /* p  0 */
			CCMISRCH       , /* q  1 */
			CCPLSRCH       , /* r  2 */
			CCMARK         , /* s  3 */
			CCMIPAGE       , /* t  4 */
			CCPLPAGE       , /* u  5 */
			CCREPLACE      , /* v  6 */
			CCMILINE       , /* w  7 */
			CCPLLINE       , /* x  8 */
			CCSETFILE      , /* y  9 */
		    };
		    *ocp++ = xlt[chr - 'l'];
		}
		else if ('M' <= chr && chr <= 'S') Block {
		    static char xlt[] = {
			CCCMD          , /* M  ENTER */
			CCUNAS1        , /* N        */
			CCUNAS1        , /* O        */
			CCLWORD        , /* P  PF1   */
			CCRWORD        , /* Q  PF2   */
			CCBACKTAB      , /* R  PF3   */
			CCCLOSE        , /* S  PF4   */
		    };
		    *ocp++ = xlt[chr - 'M'];
		}
		else
		    *ocp++ = CCUNAS1;
	    }
	    else
		*ocp++ = CCUNAS1;
	}
	else if (chr == ('X' & 31)) {
	    if (nr < 2) {
		icp--;
		goto nomore;
	    }
	    nr--;
	    chr = *icp++ & 0177;
	    switch (chr) {
	    case CTRL ('a'):
		*ocp++ = CCSETFILE;
		break;
	    case CTRL ('c'):
		*ocp++ = CCCTRLQUOTE;
		break;
	    case CTRL ('e'):
		*ocp++ = CCERASE;
		break;
	    case CTRL ('h'):
		*ocp++ = CCLWINDOW;
		break;
	    case CTRL ('j'):
		*ocp++ = CCJOIN;
		break;
	    case CTRL ('l'):
		*ocp++ = CCRWINDOW;
		break;
	    case CTRL ('r'):
		*ocp++ = CCREPLACE;
		break;
	    case CTRL ('s'):
		*ocp++ = CCSPLIT;
		break;
	    case CTRL ('t'):
		*ocp++ = CCTABS;
		break;
	    case CTRL ('u'):
		*ocp++ = CCBACKTAB;
		break;
	    case CTRL ('w'):
		*ocp++ = CCCHWINDOW;
		break;
	    default:
		*ocp++ = CCUNAS1;
		break;
	    }
	}
	else
	    *ocp++ = lexstd[chr];
    }
 nomore:
    Block {
	int conv;
	*count = nr;     /* number left over - still raw */
	conv = ocp - lexp;
	while (nr-- > 0)
	    *ocp++ = *icp++;
	return conv;
    }
}


lt_vt100 () { P (CTRL('h')); delay (2); }
rt_vt100 () { fwrite ("\033[C", 3, 1, stdout); delay (2); }
dn_vt100 () { P (012); delay (2); }
up_vt100 () { fwrite ("\033[A", 3, 1, stdout); delay (2); }
cr_vt100 () { P (015); delay (2); }
nl_vt100 () { P (015); P (012); delay (4); }
clr_vt100 () { fwrite ("\033[;H\033[2J", 8, 1, stdout); delay (50); }
hm_vt100 () { fwrite ("\033[;H", 4, 1, stdout); delay (2); }
bsp_vt100 () { fwrite ("\010 \010", 3, 1, stdout); delay (5); }
addr_vt100 (lin, col) { printf ("\033[%d;%dH", lin + 1, col + 1); delay (5); }

xl_vt100 (chr)
#ifdef UNSCHAR
Uchar chr;
#else
Uint chr;
#endif
{
#ifndef UNSCHAR
    chr &= 0377;
#endif
    if (chr == ESCCHAR) Block {
	static char esc[] = {033,'(','0','\\',033,'(','B',0,0};
	fwrite (esc, sizeof esc, 1, stdout);
    }
    else if (chr == BULCHAR) Block {
	static char bul[] = {033,'(','0','a',033,'(','B',0,0};
	fwrite (bul, sizeof bul, 1, stdout);
    }
    else
	P (stdxlate[chr-FIRSTSPCL]);
}

kini_vt100 ()
{
    static char ini[] = {033,'='};
    fwrite (ini, sizeof ini, 1, stdout);
}
kend_vt100 ()
{
    static char end[] = {033,'>'};
    fwrite (end, sizeof end, 1, stdout);
}


S_kbd kb_vt100 = {
/* kb_inlex */  in_vt100,
/* kb_init  */  kini_vt100,
/* kb_end   */  kend_vt100,
};

S_term t_vt100 = {
/* tt_ini0    */    nop,
/* tt_ini1    */    nop,
/* tt_end     */    nop,
/* tt_left    */    lt_vt100,
/* tt_right   */    rt_vt100,
/* tt_dn      */    dn_vt100,
/* tt_up      */    up_vt100,
/* tt_cret    */    cr_vt100,
/* tt_nl      */    nl_vt100,
/* tt_clear   */    clr_vt100,
/* tt_home    */    hm_vt100,
/* tt_bsp     */    bsp_vt100,
/* tt_addr    */    addr_vt100,
/* tt_lad     */    bad,
/* tt_cad     */    bad,
/* tt_xlate   */    xl_vt100,
/* tt_insline */    (int (*) ()) 0,
/* tt_delline */    (int (*) ()) 0,
/* tt_inschar */    (int (*) ()) 0,
/* tt_delchar */    (int (*) ()) 0,
/* tt_clreol  */    (int (*) ()) 0,
/* tt_defwin  */    (int (*) ()) 0,
/* tt_deflwin */    (int (*) ()) 0,
/* tt_erase   */    (int (*) ()) 0,
/* tt_nleft   */    1,
/* tt_nright  */    3,
/* tt_ndn     */    1,
/* tt_nup     */    3,
/* tt_nnl     */    2,
/* tt_nbsp    */    3,
/* tt_naddr   */    8,
/* tt_nlad    */    0,
/* tt_ncad    */    0,
/* tt_wl      */    3,
/* tt_cwr     */    3,
/* tt_pwr     */    0,
/* tt_axis    */    0,
/* tt_bullets */    YES,
/* tt_prtok   */    YES,
/* tt_width   */    80,
/* tt_height  */    24,
};
