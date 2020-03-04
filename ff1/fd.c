#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

ff_fd(ff)
register Ff_stream *ff;
{
    if(FF_CHKF) {
	errno = EBADF;
	return(-1);
    }
    return ff->f_file->fn_fd;
}

