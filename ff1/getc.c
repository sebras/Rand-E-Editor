#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

ff_getc(ff)
register Ff_stream *ff;
{
    register Ff_buf *fb;
    register Ff_file *fp;
    int block, offset;
    extern int ldivr;

    if(FF_CHKF || !(ff->f_mode & F_READ)) {
	errno = EBADF;
	return -1;
    }
    if (ff->f_offset >= ff->f_file->fn_size) {
	errno = 0;
	return -1; /* EOF */
    }
    block  = ldiv(ff->f_offset, FF_BSIZE);
    offset = ldivr;
    fp = ff->f_file;
    if(CHKBLK)
	if(!(fb = ff_getblk(fp, block)))
	    return -1; /* errno will have been set by getblk */
#ifdef DEBUG
    if(offset >= fb->fb_count) {
	write(2, "ff:getc ptr past count\n", 23);
	abort();
    }
#endif
    ff->f_offset++;
    return fb->fb_buf[offset]&0377;
}

