#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>

Ff_stream ff_streams[NOFFFDS];
Ff_file   ff_files[NOFILE];
Ff_rbuf   ff_flist =
{
    0,
    0,
    (Ff_buf *) &ff_flist,
    (Ff_buf *) &ff_flist,
};
Ff_stats ff_stats;

extern long lseek ();

Ff_buf *
ff_putblk(fb, flg)
register Ff_buf *fb;
int flg;
{
    register Ff_buf *fb1;
    register Ff_file *fp;
    int try;
    int cnt;

    if((fp = fb->fb_file) == (Ff_file *) 0)
	return fb;
    if(fb->fb_wflg) {
	if(fp->fn_realblk != fb->fb_bnum) {
	    ff_stats.fs_seek++;
	    lseek(fp->fn_fd, (long)fb->fb_bnum*FF_BSIZE, 0);
	}
	ff_stats.fs_write++;
#ifdef DEBUG
	if(fb->fb_count > FF_BSIZE) {
	    write(2, "ff:putblk write > %d\n", 22, FF_BSIZE);
	    abort();
	}
#endif
	for ( try = 1
	    ; (cnt = write(fp->fn_fd, fb->fb_buf, fb->fb_count))
	      != fb->fb_count
	    ; try++
	    ) {
	    if (try > 5 || cnt != -1 || errno != EINTR) {
		fp->fn_realblk = -1;
		return 0;
	    }
	}
	if(fb->fb_count == FF_BSIZE)
	    fp->fn_realblk = fb->fb_bnum + 1;
	else
	    fp->fn_realblk = -1;
	fb->fb_wflg = 0;
	fb->fb_count = 0;
    }
    if(flg) {                           /* take off the active list?  */
	fb->fb_qb->fb_qf = fb->fb_qf;   /*! should check against nblks? */
	if(fb1 = fb->fb_qf)
	    fb1->fb_qb = fb->fb_qb;
	fb->fb_file = (Ff_file *) 0;
	fb->fb_qf = (Ff_buf *) 0;
	fb->fb_qb = (Ff_buf *) 0;
    }
    return fb;
}

Ff_buf *
ff_getblk(afp, blk)
Ff_file *afp;
int blk;
{
    return ff_gblk(afp, blk, 1);
}

Ff_buf *
ff_gblk(fp, blk, rdflg)
register Ff_file *fp;
int blk;
int rdflg;
{
    register Ff_buf *fb, *fb1;

    for(fb = fp->fn_qf; fb; fb = fb->fb_qf)
	if(fb->fb_bnum == blk)
	    goto found;
    fb = ff_flist.fr_back;
    if(!ff_putblk(fb, 1))
	return 0;
    if (   (fp->fn_mode & F_READ)
	&& (   !(fp->fn_mode&F_WRITE)
	    || (blk*FF_BSIZE < fp->fn_size)
	   )
       ) {
	if(fp->fn_realblk != blk) {
	    ff_stats.fs_seek++;
	    lseek(fp->fn_fd, (long)blk*FF_BSIZE, 0);
	}
	ff_stats.fs_read++;
	if (rdflg){
	    if((fb->fb_count = read(fp->fn_fd,fb->fb_buf,FF_BSIZE)) == -1)
		return 0;
	}
	else
	    fb->fb_count = FF_BSIZE;
	if(fb->fb_count == FF_BSIZE)
	    fp->fn_realblk = blk + 1;
	else if(fb->fb_count)
	    fp->fn_realblk = -1;
	else
	    fp->fn_realblk = blk;
    } else
	fb->fb_count = 0;
    fb->fb_bnum  = blk;
    fb->fb_file = fp;
found:
    if(ff_flist.fr_forw != fb && ff_flist.fr_forw->fb_forw != fb) {
	fb->fb_back->fb_forw = fb->fb_forw;     /* Move block to*/
	fb->fb_forw->fb_back = fb->fb_back;     /*  head of free*/
	ff_flist.fr_forw->fb_back = fb;          /*  list to mark*/
	fb->fb_forw     = ff_flist.fr_forw;      /*  as recent   */
	ff_flist.fr_forw = fb;                   /*  reference   */
	fb->fb_back     = (Ff_buf *) &ff_flist;
    }
    if(!fp->fn_qf ||
      (fp->fn_qf != fb &&          /* If not one of first two      */
      (!fp->fn_qf->fb_qf ||       /*  queue entries, then move    */
      fp->fn_qf->fb_qf != fb))) { /*  block to head of the class  */
	if(fb1 = fb->fb_qf)
	    fb1->fb_qb = fb->fb_qb;
	if(fb1 = fb->fb_qb)
	    fb1->fb_qf = fb->fb_qf;
	fb1 = fp->fn_qf;
	fb->fb_qf = fb1;
	if(fb1)
	    fb1->fb_qb = fb;
	fp->fn_qf = fb;
	fb->fb_qb = (Ff_buf *) fp;
    }
    return fb;
}

