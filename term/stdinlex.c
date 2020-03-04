#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif


#define CTRL(x) ((x) & 31)

in_std (lexp, count)
char *lexp;
int *count;
{
    register int nr;
    register Uint chr;
    Reg3 char *icp;
    Reg4 char *ocp;

    icp = ocp = lexp;
    nr = *count;
    for (; nr > 0; nr--) {
	/* RAW MODE on V7 inputs all 8 bits so we and with 0177 */
	if ((chr = *icp++ & 0177) >= 32) {
	    if (chr == 0177)            /* DEL KEY KLUDGE FOR NOW */
		*ocp++ = CCBACKSPACE;
	    else
		*ocp++ = chr;
	}
	else if (chr == CTRL ('X')) {
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
	Reg1 int conv;
	*count = nr;     /* number left over - still raw */
	conv = ocp - lexp;
	while (nr-- > 0)
	    *ocp++ = *icp++;
	return conv;
    }
}

S_kbd kb_std = {
/* kb_inlex */  in_std,
/* kb_init  */  nop,
/* kb_end   */  nop,
};
