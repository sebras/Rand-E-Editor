#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

long
ff_seek(ff, pos)
register Ff_stream *ff;
long pos;
{
    if(FF_CHKF) {
	errno = EBADF;
	return -1;
    }
    return ff->f_offset = pos;
}

