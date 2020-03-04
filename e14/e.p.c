/*
/* file e.p.c - process a printing char for mainloop ()
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"

printchar ()
{
    Flag bkspflg = NO;
    register Ncols curcol;
    Nlines          ln;
    Ncols   thiscol;
    Slines  thislin;

    /* process a printing character */

    numtyp++;             /* have modified text   */

    if ( !okwrite () )
	return NOWRITERR;

    if (clineno != (ln = curwksp->wlin + cursorline))
	getline (ln);

    curcol = cursorcol + curwksp->wcol;
    if (key == CCDELCH || key == CCBACKSPACE) {
	if (key == CCBACKSPACE) {
	    if (cursorcol == 0)
		return CONTIN;
	    movecursor (LT, 1);
	    curcol--;
	    bkspflg = YES;
	}
	if (curcol >= ncline - 1)      /* assumes \n at end of cline */
	    return CONTIN;
	if (   key == CCDELCH
	    || imodesw
	   ) {
	    thislin = cursorline;

	    if (ncline - 2 - curcol > 0)
		move (&cline[curcol + 1], &cline[curcol],
		      (unsigned int) (ncline - 2 - curcol));
	    ncline--;
	    curcol -= curwksp->wcol;
	    putup (-1, cursorline, curcol, MAXWIDTH);
	    poscursor (curcol, thislin);
	    fcline = YES;
	    return CONTIN;
	}
	key = ' ';
    }
    else if (key == CCCTRLQUOTE)
	key = ESCCHAR;

    /* margin-stick feature */
    if (cursorcol > curport->rtext)
	return MARGERR;

    fcline = YES;

    if (curcol >= (lcline - 2))
	excline (curcol + 2);
    if (curcol >= ncline - 1) {  /* equiv to (curcol + 2 - ncline > 0) */
	fill (&cline[ncline - 1], (unsigned int) (curcol + 2 - ncline), ' ');
	cline[curcol + 1] = '\n';
	ncline = curcol + 2;
	cline[curcol] = key;
	if (xcline) {
	    thiscol = cursorcol;
	    xcline = 0;
	    putup (-1, cursorline, thiscol, MAXWIDTH);
	    poscursor (thiscol + 1, cursorline);
	}
	else
	    putch (key, 1);
    }
    else if (imodesw) {
	if (ncline >= lcline)
	    excline (0);
	/* (ncline - curcol > 1) at this point */
	move (&cline[curcol], &cline[curcol + 1],
	      (unsigned int) ncline - curcol);
	ncline++;
	cline[curcol] = key;
	curcol -= curwksp->wcol;
	putup (-1, cursorline, curcol, MAXWIDTH);
	poscursor (++curcol, cursorline);
    }
    else {
	cline[curcol] = key;
	putch (key, 1);
    }

    /* margin-stick feature */
    if (cursorcol >= curport->rtext)
	curport->redit = curport->rtext + 1;

    if (cursorcol == curport->rtext - 10)
	d_put (007);

    /* margin-stick feature */
    curport->redit = curport->rtext;

    if (bkspflg)
	movecursor (LT, 1);
    return CONTIN;
}
