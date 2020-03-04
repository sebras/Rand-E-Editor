/*
/* file e.sv.c: save stuff
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"
#include "e.cm.h"
#include <sys/stat.h>

save ()
{
    if (curmark)
	return NOMARKERR;
    if (opstr[0] == '\0')
	return CRNEEDARG;
    if (*nxtop)
	return CRTOOMANYARGS;

#ifdef NEVER /* DON'T do this; it is not replayable! */
    if (chkpriv (opstr) != -1) {
	mesg (TELSTRT + 3, "\"", opstr,
		"\" already exists.  OK to overwrite it? ");
	keyused = YES;
	getkey (0);
	j = key;
	keyused = YES;
	mesg (TELDONE);
	if (j != 'y' && j != 'Y')
	    return CROK;
    }
#endif

    savefile (opstr, curfile, NO);
    flushkeys ();
    return CROK;
}

/* savefile (filenam, filefn, finalflg) -
/*      if filenam is given (filenam != 0)
/*          if filenam is open already
/*              disallow the save
/*              return NO;
/*          else if filenam is same as names[filefn]
/*              set filenam to 0
/*      if filenam is 0, meaning save to this file,
/*          if finalflg is 0
/*              if not EDITED, return
/*          else
/*              if not EDITED or NEW or SAVED
/*                  return
/*      if filenam is was given or file is EDITED or NEW
/*          write out file number filefn
/*          to a temp file called ,esaveXXXXX
/*          where XXXXX is unique.
/*          if filenam is 0
/*              stash the name of the tempfile in
/*              savefnames[filefn] and flag the file as SAVED and not EDITED.
/*      if finalflg or filnam
/*          if file is marked NEW,
/*              everything works OK even though the file really doesn't exist
/*          save the appropriate tempfile, the one just made or the one
/*            whose name was stashed, with backup.
/*          clear SAVED and EDITED and CANMODIFY
/*      return YES;
/*
/*      normal return is YES; error is NO;
/*
/*      see finalsave (filefn)
/**/
savefile (filenam, filefn, finalflg)
char   *filenam;
Fn      filefn;
Flag    finalflg;
{
    Fd      tempfd,
	    origfd,
	    bakfd;
    char   *origfile,       /* origfile to be saved                      */
	   *f3,             /* gen purpose char ptr */
	   *tempfile,
	   *dirname,        /* the directory for the save */
	   *filepart,       /* filename part of pathname */
           *bakf,
	    crnamo[15];     /* largest name is 14, plus 1 for '\0' at end */
    char    buf[BUFSIZ];
    register Short  i;
    register Short  j;
    Small ntries;

    if ( !(fileflags[filefn] & UPDATE) )
	return YES;
    bakf = 0;
    if (   filenam != NULL
	&& (   (i = hvname (filenam)) != -1
	    || (i = hvdelname (filenam)) != -1
	    || (i = hvoldname (filenam)) != -1
	   )
       ) {
	mesg (ERRALL + 1, "Can't save to a file we are using");
	return NO;
    }
    if (filenam == NULL) {
	if ( !finalflg ) {
	    if ( !(fileflags[filefn] & EDITED) )
		return YES;
	}
	else {
	    if ( !(fileflags[filefn] & (EDITED | NEW | SAVED)) )
		return YES;
	}
	origfile = names[filefn];
    }
    else {
	origfile = filenam;
	if ( (j = chkpriv (origfile)) != -1 && (j & S_IFMT) != S_IFREG) {
	    mesg (ERRALL + 1, "Can only save to files");
	    return NO;
	}
    }

    /* get the directory name and the file part of the name */
    if (dircheck (origfile, &dirname, &filepart, YES, YES) == -1)
	goto badsave;

    mesg (TELALL + 2, "SAVE: ", origfile);
    d_put (0);

    /* the for (;;) loop is here to keep trying until */
    /* we find a temp name  we can use */
    ntries = 0;
    for (;; sfree (tempfile)) {
	/* get us a temp file name */
	if (fileflags[filefn] & SAVED)
	    tempfile = savefnames[filefn];
	else {
	    strcpy (crnamo, saveftmp);  /* (to, from) basic tempname */
	    crname (crnamo + strlen (saveftmp));/* put unique name in rest */
	    tempfile = append (dirname, crnamo);
	}
	if (   filenam != NULL
	    || (fileflags[filefn] & (EDITED | NEW))
	   ) {
	    /* create temp 0644 to lock out other users from using this */
	    /* temp name  */
	    if ((tempfd = creat (tempfile, groupid == 1? 0644: 0664)) == -1) {
		if (   (fileflags[filefn] & SAVED)
		    || chkpriv (tempfile) == -1
		    || ++ntries > 20
		   ) {
		    mesg (ERRALL + 1, "Unable to create tmp file!");
		    goto badsave;
		}
		continue;   /* try another temp name */
	    }
	    /* GOT ONE */
	    if (fsdtofil (openfsds[filefn], 0, tempfd, NO) == -1) {
		unlink (tempfile);
	badsave:
		sfree (dirname);
		return NO;               /* error */
	    }
	    if (filenam == NULL) {
		fileflags[filefn] &= ~EDITED;
		fileflags[filefn] |= SAVED;
		savefnames[filefn] = tempfile;
	    }
	}
	break;
    }
    if ( finalflg  || filenam != NULL) {
	if (prebak[0] != 0) {     /* choose the backup name             */
	    f3 = append (prebak, filepart);
	    bakf = append (dirname, f3);
	    sfree (f3);
	}
	if (postbak[0] != 0)
	    bakf = append (origfile, postbak);
	sfree (dirname);

	i = filefn;
	if (   (fileflags[filefn] & RENAMED)
	    || (i = hvoldname (origfile)) != -1
	   ) {
	    /* got to rename it now */
	    if (!mv (oldnames[i], names[i])) {
		mesg (ERRALL + 4, "Can't rename ", oldnames[i], " to ",
		       names[i]);
		if (bakf)
		    sfree (bakf);
		return NO;
	    }
	    fileflags[i] &= ~RENAMED;
	    /* in doing the rename, we may have deleted another one */
	    if ((j = hvdelname (names[i])) != -1)
		fileflags[j] &= ~DELETED;
	}
	if (   filenam == NULL
	    && (fileflags[filefn] & INPLACE)
	    && multlinks (origfile)
	   ) {
	    /* save inplace */
	    /* copy orig to backup */
	    if (bakf) {                   /* do we even use backups?    */
		unlink (bakf);            /* get rid of any old backup    */
		if ((origfd = open (origfile, 0)) != -1) {
		    if ( (bakfd = creat (bakf,
					 fgetpriv (ff_fd (openffs[filefn])))
			 ) == -1
		       ) {
			mesg (ERRALL + 1, "Unable to create Backup");
			sfree (bakf);
			close (origfd);
			return NO;
		    }
		    do {
			j = read (origfd, buf, sizeof buf);
			if (write (bakfd, buf, j) != j) {
			    unlink (bakf);
			    close (bakfd);
			    mesg (ERRALL + 1, "Error writing to backup file");
			    sfree (bakf);
			    close (origfd);
			    return NO;
			}
		    } while (j == sizeof buf);
		    close (origfd);
		    close (bakfd);
		}
		sfree (bakf);
	    }
	    /* copy temp to original */
	    if ((tempfd = open (tempfile, 0)) == -1)
		goto saverr;
	    if ( (origfd = creat (origfile,
				   fgetpriv (ff_fd (openffs[filefn])))
		 ) == -1
	       ) {
		close (tempfd);
   saverr:      mesg (ERRALL + 1, "Unable to (inplace) Save after Backup");
		return NO;
	    }
	    do {
		j = read (tempfd, buf, sizeof buf);
		if (write (origfd, buf, j) != j) {
		    close (origfd);
		    close (tempfd);
		    mesg (ERRALL + 1, "Write error saving after Backup");
		    return NO;
		}
	    } while (j == sizeof buf);
	    close (origfd);               /* original saved.             */
	    unlink (tempfile);                  /* get rid of tmp-name link     */
	}
	else {  /* not inplace */
	    if ((i = hvdelname (origfile)) != -1) {
		unlink (origfile);
		fileflags[i] &= ~DELETED;
	    }
	    if (bakf) {                   /* do we even use backups?    */
		unlink (bakf);            /* get rid of any old backup    */
		link (origfile, bakf);     /* create new bakup file     */
		sfree (bakf);
	    }
	    i = fgetpriv (ff_fd (openffs[filefn]));
	    unlink (origfile);          /* get rid of old link to it    */
	    link (tempfile, origfile);  /* link to tmp                  */
	    unlink (tempfile);          /* get rid of tmp-name link     */
	    if (!(fileflags[filefn] & NEW))
		chmod (origfile, i);
	}
	if (!filenam)
	    /* see to it that nothing more will happen to this file */
	    fileflags[filefn]
		&= ~(CANMODIFY | SAVED | EDITED | NEW | DWRITEABLE);
    }
    else
	sfree (dirname);
    return YES;
}

crname (str)                       /* create unique file name */
char   *str;                       /* this is where to put the name */
{
    static Char latest = 0;

    *str++ = evrsn;
    *str++ = '.';
    sprintf (str, "%d", latest);
    latest++;
}

