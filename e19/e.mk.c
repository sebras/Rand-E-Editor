#ifdef COMMENT
--------
file e.p.c
    cursor marking
#endif

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.inf.h"
#include "e.m.h"

extern void markprev ();

extern Flag fileticksflags[];
extern struct markenv fileticks[];

#ifdef COMMENT
void
mark ()
.
    Mark this spot.
#endif
void
mark ()
{
    if (prevmark == 0)
	rand_info (inf_mark, 4, "MARK");
    if (curmark)
	loopflags.flash = YES;
    markprev ();
    exchmark (YES);
    return;
}

#ifdef COMMENT
void
unmark ()
.
    Remove marks, if any.
#endif
void
unmark ()
{
    if (curmark) {
	rand_info (inf_mark, 4, "");
	rand_info (inf_area, infoarealen, "");
    }
    curmark = prevmark = 0;
    infoarealen = 0;
    marklines = 0;
    markcols = 0;
    mklinstr[0] = 0;
    mkcolstr[0] = 0;
    return;
}

#ifdef COMMENT
Nlines
topmark ()
.
    Return the top line of a marked area.
#endif
Nlines
topmark ()
{
    return min (curwksp->wlin + cursorline,
		curmark->mrkwinlin + curmark->mrklin);
}

#ifdef COMMENT
Ncols
leftmark ()
.
    Return the leftmost column of a marked area.
#endif
Ncols
leftmark ()
{
    return min (curwksp->wcol + cursorcol,
		curmark->mrkwincol + curmark->mrkcol);
}

#ifdef COMMENT
Flag
gtumark (leftflg)
    Flag leftflg;
.
    /*
    Go to upper mark.
    if leftflg is YES, go to upper left corner
    else if lines must be exchanged, then exchange columns also
    else don't exchange columns
    */
#endif
Flag
gtumark (leftflg)
Flag leftflg;
{
    Short tmp;
    Flag exchlines = NO;
    Flag exchcols  = NO;

    /* curmark is the OTHER corner */
    markprev ();

    if (    curmark->mrkwinlin + curmark ->mrklin
	 < prevmark->mrkwinlin + prevmark->mrklin
       )
	exchlines = YES;
    if (   (   leftflg
	    &&    curmark->mrkwincol + curmark ->mrkcol
	       < prevmark->mrkwincol + prevmark->mrkcol
	   )
	|| (!leftflg && exchlines)
       )
	exchcols = YES;
    if (exchlines || exchcols) {
	if (!exchcols) {
	    /* exchange the cols so mark will set them back */
	    tmp = curmark->mrkwincol;
	    curmark->mrkwincol = prevmark->mrkwincol;
	    prevmark->mrkwincol = tmp;
	    tmp = curmark->mrkcol;
	    curmark->mrkcol = prevmark->mrkcol;
	    prevmark->mrkcol = tmp;
	}
	else if (!exchlines) {
	    /* exchange the lines so mark will set them back */
	    tmp = curmark->mrkwinlin;
	    curmark->mrkwinlin = prevmark->mrkwinlin;
	    prevmark->mrkwinlin = tmp;
	    tmp = curmark->mrklin;
	    curmark->mrklin = prevmark->mrklin;
	    prevmark->mrklin = tmp;
	}
	return exchmark (NO) & WINMOVED;
    }
    return 0;
}

/* save current position into mark struct */

void savemark (struct markenv *mark_pt)
{
    if ( ! mark_pt ) return;
    mark_pt->mrkwincol = curwksp->wcol;
    mark_pt->mrkwinlin = curwksp->wlin;
    mark_pt->mrkcol = cursorcol;
    mark_pt->mrklin = cursorline;
}


/* copy mark if the current position is different from what to save */
void copymark (struct markenv *s_mark_pt, struct markenv *d_mark_pt)
{
    void infosetmark ();

    Nlines s_lin;
    Ncols  s_col;

    s_lin = s_mark_pt->mrkwinlin + s_mark_pt->mrklin;
    s_col = s_mark_pt->mrkwincol + s_mark_pt->mrkcol;
    if ( (s_lin) == (curwksp->wlin + cursorline) )
	/* do not save if the cursor was not moved outside the current line */
	return;

    if ( (s_lin <= 0)  || (s_lin >= la_lsize (curlas)) ) {
	/* do not save position at top or bottom of the file */
	if ( DebugVal > 0 ) {
	    mesg (TELALL + 1, (s_lin <= 0) ?
		    " At beginning of file : do not save the position"
		  : " At end of file : do not save the position");

	    loopflags.hold = YES;
	}
	return;
    }

    /* copy the saved position */
    *d_mark_pt = *s_mark_pt;
    infosetmark ();
}

/* move to the marked position */

Small movemark (struct markenv *mark_pt, Flag putflg)
{
    Nlines  winlin;
    Ncols   wincol;
    Slines  curslin;
    Scols   curscol;
    Small   retval;

    if ( ! mark_pt ) return;

    winlin = mark_pt->mrkwinlin;
    wincol = mark_pt->mrkwincol;
    curslin = mark_pt->mrklin;
    curscol = mark_pt->mrkcol;
    if ( curslin > curwin->btext ) {
	winlin += (curslin - curwin->btext);
	curslin = curwin->btext;
    }
    if ( curscol > curwin->rtext ) {
	wincol += (curscol - curwin->rtext);
	curscol = curwin->rtext;
    }
    retval = movewin (winlin, wincol, curslin, curscol, putflg);
    return retval;
}


#ifdef COMMENT
Small
exchmark (puflg)
    Flag puflg;
.
    Exchange the two marked positions.
#endif
Small
exchmark (puflg)
Flag puflg;
{
    struct markenv *tmp;
    Small retval = 0;

    if ( curmark ) retval = movemark (curmark, puflg);
    tmp = curmark;
    curmark = prevmark;
    prevmark = tmp;
    return retval;
}


#ifdef COMMENT
void
markprev ()
.
    Copy the current position into the prevmark structure.
#endif
void
markprev ()
{
    static struct markenv mk1, mk2;

    if (prevmark == 0)
	prevmark = curmark == &mk1 ? &mk2 : &mk1;
    savemark (prevmark);
    return;
}


/* file ticks : mark a position in a file, to be used by "goto tick" cmd */
/* --------------------------------------------------------------------- */

static Flag check_tick (AFn fnb, char * msgbuf, Flag msg_flg)
{
    static char notick_msg [] = " No \"tick\" set in the file";

    if ( !(fileflags[fnb] & INUSE) ) fileticksflags [fnb] = NO;
    if ( ! fileticksflags [fnb] ) {
	if ( msgbuf ) strcat (msgbuf, notick_msg);
	if ( msg_flg ) {
	    mesg (TELALL + 1, notick_msg);
	    loopflags.hold = YES;
	}
    }
    return fileticksflags [fnb];
}


static void msg_mark (struct markenv *markpt, char * prefix, char * msgbuf, Flag show_flg)
{
    int tkln, tkcl;
    char * buf;

    if ( !markpt || !msgbuf ) return;
    tkln = markpt->mrkwinlin + markpt->mrklin +1;
    tkcl = markpt->mrkwincol + markpt->mrkcol +1;
    buf = &msgbuf [strlen (msgbuf)];
    sprintf (buf, "%sline %d, column %d", prefix ? prefix : "", tkln, tkcl);
    if ( show_flg ) {
	mesg (TELALL + 1, msgbuf);
	loopflags.hold = YES;
    }
}

static void msg_tick (AFn fnb, char * prefix, char * msgbuf, Flag chk_flg)
{
    struct markenv *markpt;
    int tkln, tkcl;

    if ( ! check_tick (fnb, msgbuf, chk_flg) )
	return;

    markpt = & fileticks [fnb];
    msg_mark (markpt, prefix, msgbuf, chk_flg);
}

void infotick ()
{
    AFn fnb;
    struct markenv *markpt;
    char msgbuf [128];
    int tkln, tkcl;

    fnb = curwin->wksp->wfile;
    memset (msgbuf, 0, sizeof (msgbuf));
    msg_tick (fnb, " File \"tick\" at ", msgbuf, NO);
    strcat (msgbuf, ",");
    msg_mark (&curwksp->wkpos, " \"prev\" cursor at ", msgbuf, YES);
}


void infosetmark ()
{
    char msgbuf [128];
    Flag flg;

    memset (msgbuf, 0, sizeof (msgbuf));
    flg = ( DebugVal > 0 ); /* DebugVal is set with "set debug n" cmd */
    msg_mark (&curwksp->wkpos, " Save \"prev\" cursor position : ", msgbuf, flg);
}


void gototick ()
{
    extern void gotomvwin ();
    AFn fnb;
    struct markenv *markpt;

    fnb = curwin->wksp->wfile;
    if ( ! check_tick (fnb, NULL, YES) )
	return;

    markpt = & fileticks [fnb];
    (void) movemark (markpt, YES);
}

/* Set or reset the file tick mark at current position for the file */
/* ---------------------------------------------------------------- */

void marktickfile (AFn fnb, Flag set)
{
    struct markenv *markpt;
    char msgbuf [128];
    Flag flg;

    if ( (fnb < 0) || (fnb >= MAXFILES) )
	return;

    fileticksflags [fnb] = NO;
    if ( !(set && (fileflags[fnb] & INUSE)) )
	return;

    markpt = & fileticks [fnb];
    savemark (markpt);
    fileticksflags [fnb] = YES;
    flg = ( fnb == curwin->wksp->wfile );
    memset (msgbuf, 0, sizeof (msgbuf));
    msg_tick (fnb, " Set file \"tick\" at ", msgbuf, flg);
}

void marktick (Flag set)
{
    marktickfile (curwin->wksp->wfile, set);
}

