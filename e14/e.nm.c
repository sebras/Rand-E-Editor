/*
/* file e.nm.c - name (), rename (), delete ()
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"
#include "e.cm.h"
#include <sys/stat.h>

#ifdef  NAME
name ()
{
    Flag renamed,
	 new;
    char *name;

    if (opstr[0] == '\0')
	return CRNEEDARG;       /* opstr not alloced */
    if (*nxtop)
	return CRTOOMANYARGS;

    name = names[curfile];

    /* are we naming it back to its original name? */
    if (   (renamed = fileflags[curfile] & RENAMED? YES: NO)
	&& strcmp (oldnames[curfile], opstr) == 0
       ) {
	sfree (oldnames[curfile]);
	oldnames[curfile] = NULL;
	fileflags[curfile] &= ~RENAMED;
	return CROK;
    }

    /* are we trying to rename ".", ".." or a TMPFILE? */
    if (   dotdot (name)
	|| curfile < FIRSTFILE + NTMPFILES
       ) {
	mesg (ERRALL + 2, "Can't rename ", name);
	return CROK;
    }

    /* do we have write permission in the file's directory? */
    if ((fileflags[curfile] & DWRITEABLE) == 0) {
	dircheck (name, (char **) 0, (char **) 0, YES, YES);
	return CROK;
    }

    /* do we have that name already internally? */
    /* note that hvname will pass over DELETED names */
    if (hvname (opstr) != -1)
	goto exists;

    /* is the proposed directory writeable? */
    if (dircheck (opstr, (char **) 0, (char **) 0, YES, YES) == -1)
	return CROK;

    /* does that name exist already? */
    /* don't look on disk if we hold it internally as deleted or as the
    /* old name before renaming */
    if (   hvdelname (opstr) == -1
	&& hvoldname (opstr) == -1
	&& chkpriv (opstr) != -1
       ) {
 exists:
	mesg (ERRALL + 1, "That name exists already");
	return CROK;
    }

    if (!(new = (fileflags[curfile] & NEW))) {
	/* can we rename, or will it take a copy operation? */
	char *d1, *d2;
	struct stat s1, s2;

	dircheck ((renamed? oldnames: names)[curfile],
		  &d1, (char **) 0, NO, NO);
	dircheck (opstr, &d2, (char **) 0, NO, NO);

	stat (d1, &s1);
	stat (d2, &s2);

#ifdef UNIXV7
	if (s1.st_dev != s2.st_dev) {
#else
#ifdef UNIXV6
	if (   s1.st_minor != s2.st_minor
	    || s1.st_major != s2.st_major
	   ) {
#else
}}}}}}} must define one or the other
#endif
#endif
	    mesg (ERRALL + 1, "Can't rename to there, have to copy");
	    return CROK;
	}
    }
    if (new || renamed)
	sfree (name);
    else {
	oldnames[curfile] = name;
	fileflags[curfile] |= RENAMED;
    }
    info (INFOFILE, strlen (names[infofile]), opstr);
    names[curfile] = append (opstr, "");
    return CROK;

}
#endif  NAME

#ifdef  NAME
delete ()
{
    register Short flags;
    char *name;
    int j;
    Small len;

    if (opstr[0] != '\0')
	return CRTOOMANYARGS;

    name = names[curfile];
    flags = fileflags[curfile];

    if (curfile < FIRSTFILE + NTMPFILES) {
	mesg (ERRALL + 2, "Can't delete ", name);
	return CROK;
    }

    /* do we have permission to delete this file? */
    if ((flags & DWRITEABLE) == 0) {
	dircheck (name, (char **) 0, (char **) 0, YES, YES);
	return CROK;
    }

    /* can't delete directories */
    if (   !(flags & NEW)
	&& (j = chkpriv (name)) != -1    /* file already exists         */
	&& (j & S_IFMT) == S_IFDIR /* can't modify directories */
       ) {
	mesg (ERRALL + 1, "Can't delete directories");
	return CROK;
    }

    if (flags & (NEW | RENAMED)) {
	len = strlen (name);
	sfree (name);
    }
    if (flags & NEW)
	/* throw the file away entirely */
	flags = 0;
    else {
	if (flags & RENAMED) {
	    names[curfile] = oldnames[curfile];
	    oldnames[curfile] = NULL;
	}
	else
	    len = strlen (name);
	flags &= ~(EDITED | SAVED | RENAMED | CANMODIFY);
	flags |= DELETED;
    }
    freefsd (&openfsds[curfile]);
    fileflags[curfile] = flags;
    dlfile (curfile);
    info (INFOFILE, len, names[curfile]);
    return CROK;
}

dotdot (name)
char *name;
{
    register char *cp;

    for (cp = name; *cp; ) {
	if (   (cp == name || *cp++ == '/')
	    && (*cp++ == '.')
	    && (   *cp == '\0'
		|| (*cp++ == '.' && *cp == '\0')
	       )
	   )
	    return YES;
    }
    return NO;
}

dlfile (fn)
Fn fn;
{
    register S_wksp *wk, *awk;
    register Small  i;
    S_window *oport;

    for (i = Z; i < nportlist; i++) {
	if ((awk = portlist[i]->altwksp)->wfile == fn)
	    awk->wfile = NULLFILE;
	if ((wk = portlist[i]->wksp)->wfile == fn) {
	    wk->wfile = NULLFILE;
	    oport = curport;
	    savecurs ();
	    newcurline = cursorline;
	    newcurcol  = cursorcol;
	    switchport (portlist[i]);
	    if (curport != oport)
		chgborders = 0;
	    if (awk->wfile != NULLFILE)
		swfile ();
	    else
		edscrfile (YES);
	    switchport (oport);
	    restcurs ();
	}
    }
}
#endif  DELETE

