#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

char *move ();

ff_read(afp, buf, count, brkcnt, brklst)
Ff_stream *afp;
char *buf, *brklst;
int brkcnt;
{
    int                 block,
			offset,
			n,
			nn,
			cnt;
    register int        c;
    register Ff_stream *ff;
    Ff_buf             *fb;
    char               *from,
		       *to;
    register char      *sp;
    long                dn;
    extern int          ldivr;

    ff = afp;
    if(FF_CHKF || !(ff->f_mode & F_READ)) {
	errno = EBADF;
	return -1;
    }
    cnt = count;
    to = buf;
    do {
	block  = ldiv(ff->f_offset, FF_BSIZE);
	offset = ldivr;
	n = FF_BSIZE - offset;
	if (n > cnt)
	    n = cnt;
	if(!(fb = ff_getblk(ff->f_file, block)))
	    return -1;
	dn = ff->f_file->fn_size - ff->f_offset;
	if(dn <= 0)
	    break;
	if(dn < n)
	    n = dn;
	from = &fb->fb_buf[offset];
	ff->f_offset += n;
	if (brkcnt) {
	    while(n--) {
		c = *from++;
		if(to)
		    *to++ = c;
		--cnt;
		sp = brklst;
		if (nn = brkcnt)
		    do
			if(c == *sp++) {
			    ff->f_offset -= n;
			    goto leave;
			}
		    while(--nn);
#ifdef DEBUG
		if(from > &fb->fb_buf[FF_BSIZE]) {
		    write(2, "ff:read ptr past blk\n", 21);
		    abort();
		}
#endif
	    }
	}
	else {
	    if (to)
		to = move (from, to, n);
	    cnt -= n;
#ifdef DEBUG
	    if(from + n > &fb->fb_buf[FF_BSIZE]) {
		write(2, "ff:read ptr past blk\n", 21);
		abort();
	    }
#endif
	}
    } while(cnt);
 leave:
    return count - cnt;
}

