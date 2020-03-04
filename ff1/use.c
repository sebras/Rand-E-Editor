#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>

ff_use (cp, arena)
register char *cp;
int arena;
{
#define fb ((Ff_buf *)cp)

    if (arena != 0)
	goto done;
    ff_flist.fr_forw->fb_back = fb;
    fb->fb_forw = ff_flist.fr_forw;
    fb->fb_back = (Ff_buf *) &ff_flist,
    ff_flist.fr_forw = fb;

    fb->fb_file  = (Ff_file *) 0;
    fb->fb_bnum  = -1;
    fb->fb_count = -1;
    fb->fb_qf    = (Ff_buf *) 0;
    fb->fb_qb    = (Ff_buf *) 0;
    fb->fb_wflg  = 0;
    ff_flist.fr_count++;
done:
    return ff_flist.fr_count;
}
