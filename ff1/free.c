#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>

ff_free (nbuf, arena)
register int nbuf;
int arena;
{
    extern end;
    register Ff_buf *fb;

    if (arena != 0)
	goto done;
    if(nbuf >= ff_flist.fr_count)      /* leave at least one buf */
	nbuf = ff_flist.fr_count - 1;
    while(nbuf--) {
	fb = ff_flist.fr_back;
	while((int *) fb < &end) /* skip over bufs that weren't alloced */
	    if((fb = fb->fb_back) == ff_flist.fr_back)
		goto done;
	if((ff_putblk(fb, 1)) == (Ff_buf *) 0)
	    return -1;
	fb->fb_forw->fb_back = fb->fb_back;
	fb->fb_back->fb_forw = fb->fb_forw;
	free((char *)fb);
	ff_flist.fr_count--;
    }
done:
    return ff_flist.fr_count;
}

