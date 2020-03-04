/*
/* file e.d.c - screen manipulation for e
/*
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.tt.h"

#define MINIMIZE     /* define to minimize output    */
#define REALLYDUMB
#define DUMB    /* define this until e is smart about insert/delete and  */
		/* windowing, etc.                                         */

/**********
/*
/*  WARNING: VCCINI must be the first character sent to d_write
/*
/***********

/* these are the functions expected of the physical terminal */
#define MINI0()         (*term.tt_ini0)  ()
#define MINI1()         (*term.tt_ini1)  ()
#define MEND()          (*term.tt_end)   ()
#define MLEFT()         (*term.tt_left)  ()
#define MRIGHT()        (*term.tt_right) ()
#define MDN()           (*term.tt_dn)    ()
#define MUP()           (*term.tt_up)    ()
#define MCRET()         (*term.tt_cret)  ()
#define MNL()           (*term.tt_nl)    ()
#define MCLEAR()        (*term.tt_clear) ()
#define MHOME()         (*term.tt_home)  ()
#define MBSP()          (*term.tt_bsp)   ()
#define MADDR(l,c)      (*term.tt_addr)  (l,c)
#define MLAD(l)         (*term.tt_lad)   (l)
#define MCAD(c)         (*term.tt_cad)   (c)
#define MXLATE(c)       (*term.tt_xlate) (c)
#define MWL()           term.tt_wl
#define MCWR()          term.tt_cwr
#define MPWR()          term.tt_pwr
#define MAXIS()         term.tt_axis
#define MPRTOK()        term.tt_prtok

extern Scols tabright ();
extern Scols tableft ();

AFlag   dtabs[MAXWIDTH];        /* array of tabstops */

UC char    *image;
Short   icursor;                /* used as index into image */
Short   ocursor;                /* where the terminal cursor is */

Short   lincurs;                /* pos of beginning of current line*/

Scols   winl;                   /* left column of window */
Scols   winr;                   /* right column of window */
Slines  wint;                   /* top line of window */
Slines  winb;                   /* bottom line of window */
Short   winul;                  /* upper-left corner of window */
Scols   width;                  /* width of current window */
Slines  height;                 /* height of current window */
Scols   argcol;                 /* marked column */
Slines  argline;                /* marked line */

Flag    redraw;                 /* we're redrawing screen */
Small   arg;                    /* repeat count for repeatable commands */
Small   state;                  /* state the char is to be analyzed in */
Flag    noclear;                /* do not clear out opened area in next move */
Flag    rollmode;               /* roll at bottom of window */
Flag    wrapmode;               /* wrap at end of line */
Flag    cwrapmode;              /* cursor wraparound */

Slines  ilin;                   /* current line */
Scols   icol;                   /* current column */
Scols   ocol;                   /* column position of the terminal */
Slines  olin;                   /* line   position of the terminal */

/*  d_init()
/*      display initialize
/**/
d_init (clearmem, clearscr)
Flag clearmem,
     clearscr;
{
    register Short i;

    if (image == NULL) {
	screensize = term.tt_width * term.tt_height;
	image = (UC char *) salloc (screensize, YES);
    }
    ocursor = 0;
    icursor = 0;
    if (clearmem)
	dbfill (' ', 0, screensize, NO, YES);
    if (clearscr) {
	MCLEAR ();
	MHOME ();
    }
    fflush (stdout);
    lincurs = 0;
    ilin = 0;
    icol = 0;
    if (redraw)
	return;

    winl = 0;
    winr = term.tt_width - 1;
    wint = 0;
    winb = term.tt_height - 1;
    winul = 0;
    width = term.tt_width;
    height = term.tt_height;
    argcol = argline = 0;

    arg = 1;
    state = 0;
    rollmode = 1;
    wrapmode = 1;
    cwrapmode = 1;
    noclear = 0;

    i = (sizeof dtabs / sizeof dtabs[0]) - 1;
    do  dtabs[i] = ((i&7)? 0:1);
	while (i--);
    return;
}

/*  d_write(bufaddr, count)
/*
/*      put characters to the display
/*
/**/
d_write (chp, count)
UC char *chp;
Short count;
{
    register Short j;
    register unsigned int chr;

    for (; count > 0; chp++,count--) {
#ifdef UNSCHAR
	chr = *chp;
#else
	chr = *chp & 0377;
#endif
	for (;;) {
	    icol = icursor - lincurs;
	    if (chr == VCCNUL) {
		putscr (0);
		goto nextchar;
	    }
	    switch (state) {
	    case 0:
		if ( chr >= 040 ) {
		    if (arg == 1) {
#ifdef MINIMIZE
			if ((chr != image[icursor]) && !redraw)
#endif
			{
			    putscr (chr);
			    image[icursor] = chr;
			}
			if(redraw && chr != ' ')
			    putscr (chr);
			icursor++;
			if ( icol >= winr) {
			    if (wrapmode) {
				icursor = lincurs + winl;
				state = 1;
			    }
			    else
				state = 10;
			}
		    }
		    else  putmult((Char)chr);
		}
		else switch (chr) {
		case VCCARG:
		    state = 2;
		    break;

		case VCCBEL:
		    if (!silent)
			putchar(07);
		    goto nextchar;

		case VCCAAD:
		    state = 3;
		    break;

		case VCCINI:
		    MINI0 ();
		    d_init (YES, NO);
		    goto nextchar;

		case VCCEND:
		    MEND ();
		    goto nextchar;

		case VCCICL:
		    MINI1 ();
		    d_init (NO, YES);
		    goto nextchar;

#ifndef DUMB
		case VCCCLR:
		    clwindow ();
		    break;

		case VCCRES:
		    d_init (NO, NO);
		    goto nextchar;

		case VCCRAD:
		    state = 17;
		    break;

		case VCCNCL:
		    noclear = 1;
		    goto nextchar;

		case VCCESC:
		    state = 6;
		    goto nextchar;     /* save arg where it is */

#endif
		case VCCRET:
		    icursor = lincurs + winl;
		    break;

#ifndef DUMB
		case VCCNWL:
		    icursor = lincurs + winl;
		    rollifnec(arg);
		    break;

		case VCCSTB:
		case VCCCTB:
		    chr = (chr == VCCSTB? 1: 0);
		    dtabs[icol-winl] = chr;
		    if ( icol < winr )
			icursor++;
		    break;

		case VCCFTB:
		    icursor = tabright(arg) + lincurs + winl;
		    break;

		case VCCBTB:
		    icursor = tableft(arg) + lincurs + winl;
		    break;
#endif

		case VCCLEF:
		    for (;;) {
			j = icol - winl;
			if (j > 0) {
			    j = min (j, arg);
			    icursor -= j;
			    arg -= j;
			}
			if (!cwrapmode)
			    break;
			if (arg <= 0)
			    break;
			icursor += width;
			icol = winr + 1;
		    }
		    break;

		case VCCRIT:
		    for (;;) {
			j = winr - icol;
			if (j > 0) {
			    j = min (j, arg);
			    icursor += j;
			    arg -= j;
			}
			if (!cwrapmode)
			    break;
			if (arg <= 0)
			    break;
			icursor -= width;
			icol = winl - 1;
		    }
		    break;

		case VCCUP:
		    for (;;) {
			j = ilin - wint;
			if (j > 0) {
			    j = min (arg, j);
			    ilin -= j;
			    arg -= j;
			    icursor -= (j *= term.tt_width);
			    lincurs -= j;
			}
			if (!cwrapmode)
			    break;
			if (arg <= 0)
			    break;
			lincurs += (Short)(height - 1) * (Short) term.tt_width;
			icursor = lincurs + icol;
			ilin = winb;
			arg--;
		    }
		    break;

		case VCCDWN:
		    for (;;) {
			j = winb - ilin;
			if (j > 0) {
			    j = min (arg, j);
			    ilin += j;
			    arg -= j;
			    icursor += (j *= term.tt_width);
			    lincurs += j;
			}
			if (!cwrapmode)
			    break;
			if (arg <= 0)
			    break;
			lincurs -= (Short)(height - 1) * (Short) term.tt_width;
			icursor = lincurs + icol;
			ilin = wint;
			arg--;
		    }
		    break;

		case VCCBKS:
		    if ( ( j = icol - winl ) > 0 ) {
			arg = j = min (arg, j);
			putscr (0); /* update terminal cursor */
			icursor -= j;
			do {
			    MBSP ();
			    ocursor--;
			} while (--j);
			dbfill (' ', icursor, arg, NO, YES);
		    }
		    break;

#ifndef DUMB
		case VCCEOL:
		    dbfill (' ', icursor, winr - icol + 1, 1, 1);
		    break;
#endif

		case VCCHOM:
		    icursor = winul;
		    lincurs = icursor - winl;
		    ilin = wint;
		    break;

#ifndef DUMB
		case VCCWIN:
		    if ( ( icol >= argcol ) && ( ilin >= argline ) ) {
			winl = argcol;
			winr = icol;
			wint = argline;
			winb = ilin;
			width = winr - winl + 1;
			height = winb - wint + 1;
			winul = wint * term.tt_width + winl;
		    }
		    break;

		case VCCDLL:
		    scroll(ilin,arg, 1);
		    break;

		case VCCINL:
		    scroll(ilin,arg, 0);
		    break;

		case VCCDLC:
		    i = winr - icol + 1;
		    j = min (arg, i);
		    dbmove(icursor+j,icursor,i-j,1,1);
		    if (noclear)
			noclear = 0;
		    else
			dbfill (' ',icursor+i-j,j,1,1);
		    break;

		case VCCINM:
		    state = 8;
		    break;

		case VCCINC:
		    i = winr - icol + 1;
		    j = min (arg, i);
		    dbmove(icursor,icursor+j,i-j,1,1);
		    if (noclear)
			noclear = 0;
		    else
			dbfill (' ',icursor,j,1,1);
		    break;

		case VCCMVL:
		    j = min (arg, width);
		    i = winb - wint + 1;
		    to = winul;
		    do {
			dbmove(to+j,to,width-j,1,1);
			if (!noclear)
			    dbfill (' ',to+width-j,j,1,1);
			to += term.tt_width;
		    }while (--i);
		    noclear = 0;
		    icursor -= min (j, icol-winl);
		    break;

		case VCCMVR:
		    j = min (arg, width);
		    i = winb - wint + 1;
		    to = winul;
		    do {
			dbmove(to,to+j,width-j,1,1);
			if (!noclear)
			    dbfill(' ',to,j,1,1);
			to += term.tt_width;
		    } while (--i);
		    noclear = 0;
		    icursor += min (j, winr-icol);
		    break;
#endif

		default:
		    fatal (FATALBUG, "Illegal command to terminal simulator");
		}

		arg = 1;
		goto nextchar;

	    case 1:                         /* prev char spilled over */
		if ( chr >= 040 ) {
		    rollifnec(1);
		    if ( state == 13 ) goto nextchar;
		}
#ifndef DUMB
		else if ( chr == VCCESC ) {
		    state = 7;
		    goto nextchar;
		}
#endif
		else if ( chr == VCCARG ) {
		    state = 16;
		    goto nextchar;
		}
		state = 0;
		break;

	    case 2:                         /* get argument */
		state = 0;
		if ( chr == 040 ) {
		    argcol = icol;
		    argline = ilin;
		    arg = 1;
		    goto nextchar;
		}
		else if ( chr > 040) {
		    arg = chr - 040;
		    goto nextchar;
		}
		arg = 1;
		break;

	    case 3:                         /* get column addr */
		if ( chr > 040 ) {
		    icursor = lincurs + min (chr - 040, term.tt_width) -1;
		    state = 5;
		}
		else if (chr == 040)
		    state = 5;
		else if ( chr == 127 )
		    state = 4;
		else {
		    state = 0;
		    break;
		}
		goto nextchar;

	    case 4:                         /* get oversize column addr */
		icursor = lincurs + min (127 - 040 + chr - 040, term.tt_width) -1;
		state = 5;
		goto nextchar;

	    case 5:                         /* get line addr */
		state = 0;
		if ( chr > 040 ) {
		    ilin = min (chr - 040, term.tt_height) - 1;
		    lincurs = (Short)ilin * (Short) term.tt_width;
		    icursor = lincurs + icol;
		}
		else if (chr < 040)
		    break;
		if (   ( icol < winl ) || ( icol > winr )
		    || ( ilin < wint ) || ( ilin > winb )
		   ) {
		    winl = wint = 0;
		    winr = term.tt_width - 1;
		    winb = term.tt_height - 1;
		    winul = 0;
		    width = term.tt_width;
		    height = term.tt_height;
		}
		goto nextchar;

#ifndef DUMB
	    case 6:                        /* prev char was esc */
		state = 0;
/*              if ( chr >= 040 && chr < '@' ) {
/*                  if (arg == 1) {
/*                      if (chr != image[icursor]) {
/*                          putscr (chr);
/*                          image[icursor] = chr;
/*                      }
/*                      if ( icol >= winr) {
/*                          if (wrapmode) {
/*                              icursor = lincurs + winl;
/*                              state = 1;
/*                          }
/*                          else
/*                              state = 10;
/*                      }
/*                      else
/*                          icursor++;
/*                  }
/*                  else  putmult(c);
/*              }
/*              else
/**/
		switch (chr) {
		case VECSWR:
		    wrapmode = 1;
		    break;

		case VECCWR:
		    wrapmode = 0;
		    break;

		case VECSRL:
		    rollmode = 1;
		    break;

		case VECCRL:
		    rollmode = 0;
		    break;

		case VECSWC:
		    cwrapmode = 1;
		    break;

		case VECCWC:
		    cwrapmode = 0;
		    break;
		}
		arg = 1;
		goto nextchar;

	    case 7:                         /* prev char was esc after */
		state = 6;                  /*  line wrapped around    */
/*              if ( chr >= 040 && chr < '@' ) {
/*                  rollifnec(1);
/*                  if ( state == 13 )
/*                      goto nextchar;
/*              }
/**/            break;

	    case 8:                         /* insert mode in effect */
		if ( chr >= 040 ) {         /*! no multichar inserts as yet */
		    j = winr - icol;
		    dbmove(icursor,icursor+1,j,1,1);
#ifdef MINIMIZE
		    if (chr != image[icursor])
#endif
		    {
			putscr (chr);
			image[icursor] = chr;
		    }
		    if ( icol >= winr) {
			if (wrapmode) {
			    icursor = lincurs + winl;
			    state = 1;
			}
			else
			    state = 10;
		    }
		    else
			icursor++;

		    goto nextchar;
		}
		else if ( chr == VCCESC ) {
		    state = 9;
		    goto nextchar;
		}
		else
		    state = 0;
		break;

	    case 9:                         /* prev char was esc while */
/*              if ( chr >= 040 && chr < '@') { /*  in insert mode         */
/*                  j = winr - icol;
/*                  dbmove(icursor,icursor+1,j,1,1);
/*                  state = 8;
/*                  image[icursor] = chr;
/*                  if ( icol >= winr) {
/*                      if (wrapmode) {
/*                          icursor = lincurs + winl;
/*                          state = 1;
/*                      }
/*                      else
/*                          state = 10;
/*                  }
/*                  else
/*                      icursor++;
/*                  goto nextchar;
/*              }
/**/            state = 6;
		break;

	    case 10:                        /* throwing away characters */
		if (   chr == VCCAAD            /*  off end of line         */
		    || chr == VCCRAD
		    || chr == VCCRET
		    || chr == VCCNWL
		    || chr == VCCHOM
		   ) {
		    state = 0;
		    break;
		}
		if ( chr == VCCESC ) {
		    state = 11;
		    arg = 1;
		}
		else if ( chr == VCCARG )
		    state = 12;
		goto nextchar;

	    case 11:                        /* esc from state 10 */
		state = 10;
		goto nextchar;

	    case 12:                        /* arg from state 10 */
		if ( chr > 040 )
		    arg = chr - 040;
		state = 10;
		goto nextchar;

	    case 13:                        /* throw away everything */
		if (   chr == VCCHOM             /*  after an attempted */
		    || chr == VCCAAD             /*  newline at winb */
		    || chr == VCCRAD
		   ) {
		    state = 0;
		    break;
		}
		else if ( chr == VCCESC )
		    state = 14;
		else if ( chr == VCCARG )
		    state = 15;
		goto nextchar;

	    case 14:                        /* esc from state 13 */
		state = 13;
		goto nextchar;

	    case 15:                        /* arg from state 13 */
		state = 13;
		goto nextchar;
#endif

	    case 16:                        /* arg from state 1 */
		if ( chr > 040 )
		    arg = chr - 040;
		state = 1;
		goto nextchar;

#ifndef DUMB
	    case 17:                        /* get rel column */
		if ( chr <= 040 )
		    state = 19;
		else if ( chr == 127 )
		    state = 18;
		else {
		    icursor = lincurs + winl + min (chr - 040, width) - 1;
		    state = 19;
		}
		goto nextchar;

	    case 18:                        /* get oversize rel col */
		icursor = lincurs + winl + min (127 - 040 + chr - 040, width) - 1;
		state = 19;
		goto nextchar;

	    case 19:                        /* get rel line addr */
		if ( chr != 0 ) {
		    ilin = wint + min (chr - 040, height) - 1;
		    lincurs = ilin * term.tt_width;
		    icursor = lincurs + icol;
		}
		state = 0;
		goto nextchar;
#endif

	    default:
		fatal (FATALBUG, "Illegal state in terminal simulator");
	    }
	}
    nextchar:
	{}
    }
    return;
}



#ifndef DUMB
tabright(n)
register Short n;
{
    register Short wincol, i;

    wincol = icol - winl;
    if ( n == 0 )
	return(wincol);
    do {
	if ( wincol == width-1 )
	    return(wincol);
	for (i=wincol+1; i<width; i++)
	    if ( dtabs[i] ) {
		wincol = i;
		break;
	    }
    }  while (--n);
    return(wincol);
}


tableft(n)
register Short n;
{
    register Short wincol, i;

    wincol = icol - winl;
    if ( n == 0 )
	return(wincol);
    do {
	if ( wincol == 0 )
	    return(wincol);
	for (i=wincol-1; i>=0; i--)
	    if ( dtabs[i] ) {
		wincol = i;
		break;
	    }
    }  while (--n);
    return(wincol);
}
#endif

putmult(chr)
register Char chr;
{
    register Short i,j;

    for (;;) {
	i = winr - (icursor - lincurs) + 1;
	j = min (i, arg);
	arg -= j;
	dbfill(chr,icursor,j,1,1);
	if (j < i ) {
	    icursor += j;
	    arg = 1;
	    return;
	}
	if ( wrapmode == 0 ) {
	    icursor = lincurs + winr;
	    state = 10;
	    arg = 1;
	    return;
	}
	icursor = lincurs + winl;
	if (arg == 0 || ilin == winb) {
	    state = 1;
	    arg = 1;
	    break;
	}
	icursor += term.tt_width;
	lincurs += term.tt_width;
	ilin++;
    }
    return;
}


rollifnec (nn)
register Slines nn;
{
    register Short i;

    if ( ilin < winb ) {
	i = min (nn, winb-ilin);
	nn -= i;
	ilin += i;
	icursor += (i *= term.tt_width);
	lincurs += i;
    }
#ifndef REALLYDUMB
    if ( nn > 0 ) {
	if (rollmode)
	    scroll(wint,nn, 1);
	else
	    state = 13;
    }
#endif
    return;
}

#ifndef REALLYDUMB

scroll(lin, nn, upflg)
Slines lin, nn;
Flag upflg;
{
    register Short to;
    register Short i, n;

    if ( nn == 0 )
	return;
    n = min (nn, winb-lin+1);
    nn = n;
    n *= term.tt_width;
    if (upflg) {
	to = (Short)lin * (Short)term.tt_width + winl;
	if (i = winb - lin + 1 - nn) {
	    do {
		dbmove(to+n,to,width,1,1);
		to += term.tt_width;
	    } while (--i);
	}
	i = nn;
	if (noclear)
	    noclear = 0;
	else
	    do {
		dbfill(' ',to,width,1,1);
		to += term.tt_width;
	    } while (--i);
    }
#ifndef DUMB
    else {
	/* first display the change from top to bottom */
	to = lin * term.tt_width + winl;
	i = nn;
	if (noclear)
	    to += term.tt_width * i;
	else
	    do {
		dbfill(' ',to,width,1,0);
		to += term.tt_width;
	    } while (--i);
	i = winb - lin + 1 - nn;
	if ( i )
	    do {
		dbmove(to-n,to,width,1,0);
		to += term.tt_width;
	    } while (--i);

	/* then update image from bottom to top */
	to = winb * term.tt_width + winl;
	i = winb - lin + 1 - nn;
	if ( i )
	    do {
		dbmove(to-n,to,width,0,1);
		to -= term.tt_width;
	    } while (--i);
	i = nn;
	if (noclear)
	    noclear = 0;
	else
	    do {
		dbfill(' ',to,width,0,1);
		to -= term.tt_width;
	    } while (--i);
    }
    return;
#endif
}

clwindow()
{
    register Short i;
    register Short to;

    icursor = winul;
    lincurs = winul - winl;
    ilin = wint;
    to = icursor;
    i = winb - wint + 1;
    do {
	dbfill (' ', to, width, 1, 1);
	to += term.tt_width;
    } while (--i);
    return;
}
#endif

dbfill (chr, to, nchars, displflg, wrtflg)
register UC chr;
Short to;
Short nchars;
Flag displflg,
     wrtflg;
{
    Short savicursor;
    Short savicol;
    register UC char *rto;
    register Short rnchars;

#ifndef UNSCHAR
    chr &= 0377;
#endif
    if (nchars > 0) {
	if (displflg) {
	    rto = &image[to];
	    rnchars = nchars;
	    savicursor = icursor;
	    icursor = to;
	    savicol = icol;
	    icol = to % term.tt_width;
	    do {
#ifdef MINIMIZE
		if (*rto++ != chr)
#endif
#ifndef MINIMIZE
		rto++;
#endif
		    putscr (chr);
		icursor++;
		icol++;
	    } while (--rnchars);
	    icursor = savicursor;
	    icol = savicol;
	}
	if (wrtflg)
	    fill (&image[to], (unsigned int) nchars, chr);
    }
    return;
}

#ifndef REALLYDUMB
dbmove (from, to, nchars, displflg, wrtflg) /* move backwards if necessary */
Short from, to; /* indexes into the image[] array */
Short nchars;
Flag displflg,
     wrtflg;
{
    register UC char *rfrom, *rto;
    register Short rnchars;
    Short savicursor;

    if (( nchars > 0 ) && ( from != to )) {
	if (displflg) {
	    rfrom = &image[from];
	    rto = &image[to];
	    rnchars = nchars;
	    savicursor = icursor;
	    icursor = to;
	    do {
#ifdef MINIMIZE
		if (*rto++ != *rfrom)
#endif
#ifndef MINIMIZE
		rto++;
#endif
		    putscr (*rfrom);
		rfrom++;
		icursor++;
	    } while (--rnchars);
	    icursor = savicursor;
	}
	if (wrtflg)
	    move (&image[from], &image[to], (unsigned int) nchars);
    }
    return;
}
#endif

#define putx(c) {if ((c) < FIRSTSPCL && MPRTOK ()) putchar (c); else MXLATE (c);}

/*  putscr  Should only be called with printing characters or 0 to force
/*            the cursor to be where it is supposed to be.
/**/
putscr (chr)
unsigned int chr;
{
    static Flag wrapflg;
    register Slines lin;
    register Scols col;

    if (silent)
	return;
    col = icol;
    lin = ilin;
    if (ocursor == icursor)
	wrapflg = NO;
    else {
	if (wrapflg) {
	    wrapflg = NO;
	    olin--;
	    if (icursor == (ocursor -= term.tt_width)) {
		MCRET ();   /* if we got this far, then MCRET will work */
		goto there;
	    }
	}
	if (col == 0) {
	    switch (lin - olin) {
	    case -1:
		if (!term.tt_cret)
		    goto addr;
		MCRET ();
		MUP ();
		break;

	    case 0:
		if (!term.tt_cret)
		    goto addr;
		MCRET ();
		break;

	    case 1:
		if (MCWR () == 1 && icursor - ocursor == 1)
		    goto wrap;
		if (!term.tt_nl)
		    goto addr;
		MNL ();
		break;

	    default:
		if (icursor == 0) {
		    MHOME ();
		    break;
		}
		goto try1;
	    }
	}
	else {
 try1:      switch (icursor - ocursor) {
	    case -3:
		if (   !tt_lt3
		    || (col >= term.tt_width - 3 && MWL () != 1)
		   )
		    goto addr;
		MLEFT ();
		MLEFT ();
		MLEFT ();
		break;

	    case -2:
		if (   !tt_lt2
		    || (col >= term.tt_width - 2 && MWL () != 1)
		   )
		    goto addr;
		MLEFT ();
		MLEFT ();
		break;

	    case -1:
		if (col >= term.tt_width - 1 && MWL () != 1)
		    goto addr;
		MLEFT ();
		break;

	    case 3:
		if (   !tt_rt3
		    || (col <= 2 && MCWR () != 1)
		   )
		    goto addr;
		MRIGHT ();
		MRIGHT ();
		MRIGHT ();
		ocursor += 3;
		break;

	    case 2:
		if (   !tt_rt2
		    || (col <= 1 && MCWR () != 1)
		   )
		    goto addr;
		MRIGHT ();
		MRIGHT ();
		ocursor += 2;
		break;

	    case 1:
		if (col <= 0 && MCWR () != 1)
		    goto addr;
 wrap:          MRIGHT ();
		ocursor++;
		break;

	    default:
	    addr:
		if (MAXIS ()) {
		    if (lin == olin) {
			if (MAXIS () & 2)
			    MCAD (col);
			else
			    goto coordinate;
		    }
		    else if (col == ocol) {
			if (-2 <= lin - olin && lin - olin <= 2) {
			    switch (lin - olin) {
			    case -2:
				MUP ();
			    case -1:
				MUP ();
				break;

			    case 0:
				/* imossible */
				break;

			    case  2:
				MDN ();
			    case  1:
				MDN ();
				break;
			    }
			}
			else if (MAXIS () & 1)
			    MLAD (lin);
			else
			    goto coordinate;
		    }
		    else
			goto coordinate;
		}
		else
 coordinate:        MADDR (lin, col);
		break;
	    }
	}
	ocursor = icursor;
	olin = lin;
	ocol = col;
    }

 there:
#ifdef UNSCHAR
    if (chr < 040) {
#else
    if ((chr & 0377) < 040) {
#endif
	if (wrapflg) {
	    wrapflg = NO;
	    MNL ();     /* if we got this far, then MNL will work */
	}
	fflush (stdout);
	return;
    }

    if (col != term.tt_width - 1) {
	ocursor++;
	ocol++;
	putx (chr);
    }
    else {
	switch (MPWR ()) {
	case 0:
	    if (ocursor != screensize - 1) {
		olin = term.tt_height + 10;
		ocol = term.tt_width + 10;
		ocursor = olin * ocol;
		putx (chr);
	    }
	    break;

	case 1:
	    if (ocursor != screensize - 1) {
		ocursor++;
		olin++;
		ocol = 0;
		putx (chr);
	    }
	    break;

	case 2:
	    ocursor -= term.tt_width - 1;
	    ocol = 0;
	case 3:
	    putx (chr);
	    break;

	case 4:
	    wrapflg = YES;
	    ocursor++;
	    olin++;
	    ocol = 0;
	    putx (chr);
	    break;
	}
    }
    return;
}

fresh()
{
    savecurs ();
    redraw = YES;
    d_init (NO, YES);
    d_write(image, screensize);
    redraw = NO;
    restcurs ();
    return;
}
