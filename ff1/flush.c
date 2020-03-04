#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

ff_flush(ff)
register Ff_stream *ff;
{
    register Ff_file *fp;
    register Ff_buf  *fb;

    if(FF_CHKF) {
	errno = EBADF;
	return -1;
    }
    if(ff->f_mode&F_WRITE) {
	fp = ff->f_file;
	ff_sort(fp);
	if(fb = fp->fn_qf) do
	    if(ff_putblk(fb, 0) == (Ff_buf *) 0)
		return -1;
	while(fb = fb->fb_qf);
    }
    return 0;
}

