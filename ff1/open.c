#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include <ff.h>
#include <errno.h>
#include <sys/stat.h>

Ff_stream *ff_gfil();
Ff_stream *_doffopen ();

Ff_stream *
ff_open (path, mode, arena)
char *path;
int   mode;
int arena;
{
    if (!path || *path == '\0')
	return 0;
    return _doffopen (path, 0, mode, arena);
}

Ff_stream *
ff_fdopen (chan, mode, arena)
int chan;
int mode;
int arena;
{
    return _doffopen ((char *) NULL, chan, mode, arena);
}

Ff_stream *
_doffopen (path, chan, mode, arena)
char *path;
int   chan;
int   mode;
int arena;
{
    register Ff_stream *ffi;
    register Ff_file *fp, *fp1;
    struct stat fstbf;
    Ff_stream *ffp;
    int md;

    if (arena != 0)     /* for now */
	return 0;
    md = (mode+1) | F_READ;         /* Always need this     */
    mode = md - 1;
    if (!ff_flist.fr_count && !ff_alloc (1, 0))
	return 0;
    if (path) {
	if (stat (path, &fstbf) < 0)
	    return 0;
    }
    else if (fstat (chan, &fstbf) < 0)
	return 0;
    fp1 = 0;
    for (fp = ff_files; fp < &ff_files[NOFILE]; fp++) {
	if(fp->fn_refs) {
	    if (
#ifdef UNIXV7
		   fp->fn_dev == fstbf.st_dev
		&& fp->fn_ino  == fstbf.st_ino
#endif
#ifdef UNIXV6
		   fp->fn_minor == fstbf.st_minor
		&& fp->fn_major == fstbf.st_major
		&& fp->fn_ino   == fstbf.st_inumber
#endif
	       )
		if (ffi = ff_gfil(fp, md)) {
		    fp->fn_refs++;
		    fp->fn_mode |= md;
		    return ffi;
		} else
		    return 0;
	}
	else if (!fp1)
	    fp1 = fp;
    }
    if (!fp1) {
	errno = EMFILE;
	return 0;
    }
    if (!(ffp = ff_gfil(fp1, md)))
	return 0;
    if (path && (chan = open (path, mode)) < 0) {
	ffp->f_count = 0;
	return 0;
    }
#ifdef UNIXV7
    fp1->fn_dev  = fstbf.st_dev;
    fp1->fn_ino  = fstbf.st_ino;
    fp1->fn_size = fstbf.st_size;
#endif
#ifdef UNIXV6
    fp1->fn_minor = fstbf.st_minor;
    fp1->fn_major = fstbf.st_major;
    fp1->fn_ino  = fstbf.st_inumber;
    fp1->fn_size = ((long)fstbf.st_size0) << 16;
    fp1->fn_size += (long) (unsigned) fstbf.st_size1;
#endif
    fp1->fn_fd   = chan;
    fp1->fn_mode = md;
    fp1->fn_refs = 1;
    fp1->fn_realblk = 0;
    fp1->fn_qf = (Ff_buf *) 0;
    return ffp;
}

Ff_stream *
ff_gfil (fnp, mode)
Ff_file *fnp;
int mode;
{
    register Ff_stream *fp;

    for (fp = ff_streams; fp->f_count; )
	if (++fp >= &ff_streams[NOFFFDS]){
	    errno = EMFILE;
	    return 0;
	}
    fp->f_mode   = mode;
    fp->f_file   = fnp;
    fp->f_count  = 1;
    fp->f_offset = 0;
    return fp;
}

