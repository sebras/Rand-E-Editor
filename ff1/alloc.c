#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>

char *malloc();

ff_alloc (nbuf, arena)
register int nbuf;
int arena;
{
    register char *cp;

    if (arena != 0)
	return 0;
    while (nbuf--) {
	if ((cp = malloc (sizeof (Ff_buf))) == NULL)
	    break;
	ff_use (cp, 0);
    }

    return ff_flist.fr_count;
}


