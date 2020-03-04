#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

ff_close(afp)
Ff_stream *afp;
{
    register Ff_file *fp;
    register Ff_buf  *fb;
    register Ff_stream *ff;

    ff = afp;
    if(FF_CHKF) {
	errno = EBADF;
	return -1;
    }
    if(--ff->f_count == 0) {
	ff->f_mode = 0;
	fp = ff->f_file;
	ff->f_file = 0;
	if(--fp->fn_refs == 0) {
	    if(fp->fn_mode & 02)
		ff_sort(fp);
	    while(fb = fp->fn_qf)
		if(ff_putblk(fb, 1) == (Ff_buf *) 0)
		    return -1;
	    close(fp->fn_fd);
	    fp->fn_fd = -1;
	}
    }
    return 0;
}

