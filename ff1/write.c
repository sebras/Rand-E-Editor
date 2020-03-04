#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

char *move ();

ff_write(ff, buf, count)
register Ff_stream *ff;
char *buf;
int count;
{
    int block, offset, n, cnt;
    Ff_buf *fb;
    register char *from, *to;
    extern int   ldivr;

    if(FF_CHKF || !(ff->f_mode & F_WRITE)) {
	errno = EBADF;
	return -1;
    }
    cnt = count;
    from = buf;
    do {
	block  = ldiv(ff->f_offset, FF_BSIZE);
	offset = ldivr;
	n = FF_BSIZE - offset;
	if (n > cnt)
	    n = cnt;
	if(!(fb = ff_gblk(ff->f_file, block, n < FF_BSIZE)))
	    return count - cnt;    /* Return written count */
	to = &fb->fb_buf[offset];
	ff->f_offset += n;
	cnt -= n;
	if((offset += n) >= fb->fb_count)
	    fb->fb_count = offset;
#ifdef DEBUG
	if(fb->fb_count > FF_BSIZE) {
	    write(2, "ff:write cnt past blk\n", 22);
	    abort();
	}
#endif
	to = move (from, to, n);
	from += n;
#ifdef DEBUG
	if(to > &fb->fb_buf[FF_BSIZE]) {
	    write(2, "ff:write ptr past blk\n", 22);
	    abort();
	}
#endif

	fb->fb_wflg = 1;

    } while(cnt);

    if(ff->f_offset > ff->f_file->fn_size)
	ff->f_file->fn_size = ff->f_offset;

    return count;
}

