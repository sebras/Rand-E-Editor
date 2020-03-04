#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>

ff_sort(fp)
register Ff_file *fp;
{
    register Ff_buf  *fb, *fb1;
    char change;

    do {
	change = 0;
	for(fb = fp->fn_qf; fb; fb = fb->fb_qf)
	    for(fb1 = fb->fb_qf; fb1; fb1 = fb1->fb_qf)
		if(fb->fb_bnum > fb1->fb_bnum) {
		    change++;
		    if(fb->fb_qf)
			fb->fb_qf->fb_qb = fb->fb_qb;
		    fb->fb_qb->fb_qf = fb->fb_qf;
		    if(fb1->fb_qf)
			fb1->fb_qf->fb_qb = fb;
		    fb->fb_qf = fb1->fb_qf;
		    fb1->fb_qf = fb;
		    fb->fb_qb = fb1;
		}
    } while(change);
}

