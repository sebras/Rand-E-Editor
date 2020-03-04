#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

ff_putc(chr, ff)
register Ff_stream *ff;
char chr;
{
    register Ff_buf *fb;
    register Ff_file *fp;
    int block, offset;
    extern int ldivr;

    if(FF_CHKF) {
	errno = EBADF;
	return -1;
    }
    if(!(ff->f_mode & F_WRITE)) {
	errno = EBADF;
	return -1;
    }
    block  = ldiv(ff->f_offset, FF_BSIZE);
    offset = ldivr;
    fp = ff->f_file;
    if(CHKBLK)
	if(!(fb = ff_getblk(fp, block)))
	    return -1;
    fb->fb_buf[offset] = chr;
    fb->fb_wflg = 1;
    if (fb->fb_count <= offset)     /* fb->fb_count MAX= offset + 1;    */
	fb->fb_count =  offset + 1;
    ff->f_offset++;
    if (fp->fn_size < ff->f_offset) /* fp->fn_size MAX= ff->f_offset;   */
	fp->fn_size = ff->f_offset;
    return (unsigned)chr & 0377;
}

