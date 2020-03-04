/*
/* file e.mk.c - stuff for cursor marking
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"


mark ()
{
    if (prevmark == 0)
	info (INFOMARK, 4, "MARK");
    if (curmark)
	csrsw = 1;
    markprev ();
    exchmark (1);
}

unmark ()
{
    if (curmark) {
	info (INFOMARK, 4, "");
	info (INFOAREA, infoarealen, "");
    }
    curmark = prevmark = 0;
    infoarealen = 0;
    marklines = 0;
    markcols = 0;
    mklinstr[0] = 0;
    mkcolstr[0] = 0;
}

topmark ()
{
    return min (curwksp->wlin + cursorline,
		curmark->mrkwinlin + curmark->mrklin);
}

leftmark ()
{
    return min (curwksp->wcol + cursorcol,
		curmark->mrkwincol + curmark->mrkcol);
}

/* go to upper mark -
/*   if leftflg is YES, go to upper left corner
/*   else if lines must be exchanged, then exchange columns also
/*   else don't exchange columns
/**/
gtumark (leftflg)
Flag leftflg;
{
    register Short tmp;
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
	return exchmark (0) & WINMOVED;
    }
    return 0;
}

exchmark (puflg)
Flag puflg;
{
    register struct markenv *tmp;
    Short retval = 0;

    if (curmark)
	retval = movewin (curmark->mrkwinlin,
			   curmark->mrkwincol,
			    curmark->mrklin,
			     curmark->mrkcol, puflg);
    tmp = curmark;
    curmark = prevmark;
    prevmark = tmp;
    return retval;
}

markprev ()
{
    static struct markenv mk1, mk2;

    if (prevmark == 0)
	prevmark = curmark == &mk1? &mk2: &mk1;
    prevmark->mrkwincol = curwksp->wcol;
    prevmark->mrkwinlin = curwksp->wlin;
    prevmark->mrkcol = cursorcol;
    prevmark->mrklin = cursorline;
}
