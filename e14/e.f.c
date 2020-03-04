/*
/* file e.f.c - file stuff for e
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include <sys/stat.h>

extern short   ldivr;
extern char    csrsw;           /* is 1 if bullet is to temporarily   */

fwrprep (file,fd)           /* prepare a FILE structure for writing on an  */
FILE *file;                 /* already open file descriptor                */
Fd fd;
{
    file->_cnt  = 0;
    file->_ptr  = NULL;
    file->_base = NULL;
    file->_flag = _IOWRT;
    file->_file = fd;
}


char *
copy (s1, s2)
char   *s1,
       *s2;
{
    register char  *c1,
                   *c2;

    for (c1 = s1, c2 = s2; *c2++ = *c1++;);
    return (--c2);
}

/*    chkpriv (name)   - do a stat,
/*   fchkpriv (fildes) - do an fstat,
/*                and return the access permission for us in the
/*                0700 rwx bits or -1 if [f]stat failed.
/**/

chkpriv (name)
char name[];
{
    struct stat statbuf;

    if (stat (name[0] ? name : ".", &statbuf) == -1)
	return (-1);
    return (checkpriv (&statbuf));
}


checkpriv (statbuf)
register struct stat *statbuf;
{
    register unsigned Short flags;
    register Short priv = 0;

    flags = statbuf->st_mode;

    if (userid == 0) {
	priv = S_IREAD | S_IWRITE;
	if (statbuf->st_uid == userid)
	    priv |= flags & S_IEXEC;
	if (statbuf->st_gid == groupid)
	    priv |= (flags & (S_IEXEC >> 3)) << 3;
	priv |= (flags & (S_IEXEC >> 6)) << 6;
    }
    else {
	if (statbuf->st_uid == userid)
	    priv |= flags & 0700;
	else if (statbuf->st_gid == groupid)
	    priv |= (flags & 070) << 3;
	else
	    priv |= (flags & 7) << 6;
    }
    return priv | (flags & S_IFMT);
}

fgetpriv (fildes)
Fd fildes;
{
    struct stat statbuf,
	       *buf;

    buf = &statbuf;
    fstat (fildes, buf);

    return (buf->st_mode & 0777);

}

/* dircheck - if dir != NULL, put a pointer to an alloced string containing
/*              the directory part into *dir.  If no slash in name, *dir
/*              will be a null string, else it will be the directory name
/*              with a '/' as the last character.
/*            if file != NULL, put a pointer to the file part into *file
/*            check the directory for access, insist on writeability if
/*              writecheck == YES
/*            complain with calls to mesg if errors == YES
/*            Truncate the file part of name to the appropriate size
/*
/*            return result of chkpriv or -1 if failure
/**/
dircheck (name, dir, file, writecheck, errors)
char  *name;
char **dir,
     **file;
Flag   writecheck,
       errors;
{
    Flag slashinname = NO;
    char *tdir;
    register int   j;
    register char *cp1,
		  *cp2;

    /* determine if there is a '/' in name. if so,
    /* cp2 will point to the last '/' in name */
    for (cp1 = cp2 = name; *cp1; cp1++) {
	if (*cp1 == '/') {      /* get directory name, if one         */
	    slashinname = YES;
	    cp2 = cp1;
	}
    }
    if (slashinname) {
	*cp2 = '\0';
	tdir = append (name, "/");
	*cp2++ = '/';
    }
    else {
	tdir = append ("", "");
	cp2 = name;
    }
    if (strlen (cp2) > FNSTRLEN)
	cp2[FNSTRLEN] = '\0';
    if (file != NULL)
	*file = cp2;

    j = dirncheck (tdir, writecheck, errors);
    if (dir == NULL)
	sfree (tdir);
    else
	*dir = tdir;
    return j;
}

dirncheck (tdir, writecheck, errors)
register char *tdir;
Flag   writecheck,
       errors;
{
    Flag putback = NO;
    register int j;
    register char *cp;

    /* temporarily remove a trailing '/' */
    if ((cp = tdir)[0] != '\0')
	for (; ; cp++)
	    if (*cp == '\0') {
		if (*--cp == '/' && cp != tdir) {
		    *cp = '\0';
		    putback = YES;
		}
		break;
	    }

    if ((j = chkpriv (tdir[0] != '\0' ? tdir : ".")) == -1) {
	if (errors) {
	    mesg (ERRSTRT + 1, "Can't find");
	    goto ret;
	}
    }
    else if ( (j & S_IFMT) != S_IFDIR) {
	if (errors)
	    mesg (ERRALL + 2, tdir, " is not a directory");
	j = -1;
    }
    else if (/* !(j & S_IREAD) || */ !(j & S_IEXEC) ) {
	if (errors) {
	    mesg (ERRSTRT + 1, "Can't work in");
	    goto ret;
	}
	j = -1;
    }
    else if (writecheck && !(j & S_IWRITE) ) {
	if (errors) {
	    mesg (ERRSTRT + 1, "Can't write in");
ret:
	    if (tdir[0] == '\0')
		mesg (ERRDONE + 1, " current directory");
	    else
		mesg (ERRDONE + 2, " dir: ", tdir);
	}
	j = -1;
    }
    if (putback)
	*cp = '/';
    return j;
}

multlinks (name)    /* return (are there multiple links to the file?) */
char name[];
{
    struct stat statbuf;

    if (stat (name[0] ? name : ".", &statbuf) == -1)
	return 0;
    return statbuf.st_nlink > 1;
}

fmultlinks (fildes)    /* return (are there multiple links to the fildes?) */
Fd fildes;
{
    struct stat statbuf;

    if (fstat (fildes, &statbuf) == -1)
	return 0;
    return statbuf.st_nlink > 1;
}

hvname (name)
char *name;
{
    register Fn i;

    for (i = FIRSTFILE; i < MAXFILES; ++i)
	if (   (fileflags[i] & (INUSE | DELETED)) == INUSE
	    && strcmp (name, names[i]) == 0
	   )
	    return i;
    return -1;
}

hvoldname (name)
char *name;
{
    register Fn i;

    for (i = FIRSTFILE; i < MAXFILES; ++i)
	if (   (fileflags[i] & (INUSE | RENAMED)) == (INUSE | RENAMED)
	    && strcmp (name, oldnames[i]) == 0
	   )
	    return i;
    return -1;
}

hvdelname (name)
char *name;
{
    register Fn i;

    for (i = FIRSTFILE; i < MAXFILES; ++i)
	if (   (fileflags[i] & (INUSE | DELETED)) == (INUSE | DELETED)
	    && strcmp (name, names[i]) == 0
	   )
	    return i;
    return -1;
}

eddeffile (puflg)
Flag puflg;
{
    if (editfile (deffile, 0, 0, 0, puflg) <= 0)
	mesg (ERRALL + 1, "Default file gone: notify sys admin.");
    else {
	deffn = curfile;
	fileflags[curfile] &= ~CANMODIFY;
    }
}
