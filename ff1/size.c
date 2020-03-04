#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

long ff_size(ff)
register Ff_stream *ff;
{
    if(FF_CHKF) {
	errno = EBADF;
	return(-1);
    }
    return(ff->f_file->fn_size);
}

