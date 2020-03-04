#ifdef COMMENT
--------
file e.it.c
    input (keyboard) table parsing and lookup
#endif

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif


#include "e.h"
#ifdef  KBFILE
#include "e.it.h"

struct itable *ithead;

/****************************************/
/* file-driven input (no associated terminal number) */

int
in_file (lexp, count)
Uchar *lexp;
int *count;
{
    extern int CtrlC_flg;
    int code;
    Uchar *inp, *outp, chr;
    int i;

    for (inp = lexp, i = *count; i-- > 0;)
	*inp++ &= 0177;           /* Mask off high bit of all chars */

    /* outp should be different so a string can be replaced by a longer one */
    inp = outp = lexp;  /* !!!! what append if output is longer than input ! */
    while (*count > 0) {
	chr = *inp;
	CtrlC_flg = (chr == ('C' & '\037'));
	if (chr >= ' ' && chr <= '~') {
	    *outp++ = *inp++;
	    --*count;
	    continue;
	}

	code = itget (&inp, count, ithead, outp);
	if (code >= 0) {    /* Number of characters resulting */
	    outp += code;
	    continue;
	}
	if (code == IT_MORE)    /* Stop here if in the middle of a seq */
	    break;
				/* Otherwise not in table */
#if 0
	mesg (ERRALL + 1, "Bad control key");
#endif
	inp++;       /* Skip over this key */
	--*count;
    }
    move (inp, outp, *count);
    return outp - lexp;
}


static int itget_addr (cpp, countp, head, valp, it_pt)
char **cpp;
int *countp;
struct itable *head;
char *valp;
struct itable **it_pt;

{
    register struct itable *it;
    register char *cp;
    int count;
    int len;

    if ( it_pt) *it_pt = NULL;
    cp = *cpp;
    count = *countp;
next:
    for (it = head; it != NULLIT; it = it->it_next) {
	if (count <= 0)
	    return IT_MORE;             /* Need more input */
	if (it->it_c == *cp) {          /* Does character match? */
	    cp++;
	    --count;
	    if (it->it_leaf) {
		    if ( it_pt) *it_pt = it;
		    *cpp = cp;
		    len = it->it_len;
		    if ( valp ) move (it->it_val, valp, len);
		    *countp = count;
		    return len;
	    }
	    else {
		head = it->it_link;
		goto next;
	    }
	}
    }
    return IT_NOPE;
}

#endif


#ifdef COMMENT
itget (..) matches input (at *cpp) against input table
If some prefix of the input matches the table, returns the number of
   characters in the value corresponding to the matched input, stores
   the value in valp, points cpp past the matching input,
   and decrements *countp by the number of characters matched.

If no match, returns IT_NOPE.

If the input matches some proper previx of an entry in the input table,
   returns IT_MORE.

cpp and countp are not changed in the last two cases.
#endif

int
itget (cpp, countp, head, valp)
char **cpp;
int *countp;
struct itable *head;
char *valp;
{
    int cc;

    cc = itget_addr (cpp, countp, head, valp, NULL);
    return cc;
}


/* itoverwrite : overwrite the value of the leave define by dsetstrg
 *               by value of the leave defined by srcstrg
 * -----------------------------------------------------------------
 */
int itoverwrite (srcstrg, deststrg, head)
char *srcstrg, *deststrg;
struct itable *head;

{
    int cc;
    char *cp;
    int count;
    struct itable *its, *itd;

    cp = srcstrg;
    count = strlen (cp);
    cc = itget_addr (&cp, &count, head, NULL, &its);
    if ( !its || (cc < 0) ) return cc;

    cp = deststrg;
    count = strlen (cp);
    cc = itget_addr (&cp, &count, head, NULL, &itd);
    if ( !itd || (cc < 0) ) return cc;

    itd->it_len = its->it_len;
    itd->it_val = its->it_val;
    return (cc);
}


/* itgetleave : get the leave for the given string
 * -----------------------------------------------
 */
int itgetleave (strg, it_pt, head)
char *strg;
struct itable **it_pt;
struct itable *head;

{
    int cc;
    char *cp;
    int count;

    cp = strg;
    count = strlen (cp);
    cc = itget_addr (&cp, &count, head, NULL, it_pt);
    return (cc);
}
