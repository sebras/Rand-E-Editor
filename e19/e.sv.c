#ifdef COMMENT
--------
file e.sv.c
    file saving routines

    F. Perriollat : version for Linux / Dec 1998

	In the version for Linux, Window 95 / Window NT, a facility to define the
        file style (MicroSoft style or Unix style) has been added.
        The current file style is sample on the first read line, by
        cheking if the line is ended by cr/lf (MicroSoft) or just lf (Unix).
	By default a file is created as Unix style (Linux)
	and Microsoft style (Window).
        The style in which the file will be saved can be redefined by the
        user. This is done with the "file" command.
        The curent file status can be displayed with the "?file" or
        "file ?" command.
        Most of the handling for this facility is provided by the e_sv.c file

	For MSDOS file system the end of line is CR LF.
	On Window system :
	    On getline the CR is removed, and inserted in the file by putline
	    The file are assumed to be opened in BINARY mode when it is read
		( no CR-LF <=> LF conversion on file I/O ).
	On Linux (Unix) system :
	    For Microsoft file style (cr/lf),
	    on getline the CR is removed, and inserted in the file by
	    la_lflush routine (lalflush.c file).

    F. Perriollat : version for Window 95 / NT / Dec 1997
		    version for Linux / Dec 1998
#endif

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"
#include "e.cm.h"
#include <sys/stat.h>
#include <unistd.h>

#define DEFAULT_STYLE UNIX_FILE

/* file_mode : get the file mode (text or binary) */
/* ---------------------------------------------- */
/*  return the cr/lf flag to be use to write the file */


static Flag file_mode (fn, style_pt, user_style_pt)
Fn fn;
char *style_pt, *user_style_pt;
{ 
    Flag crlf;          /* crlf to be used for saving the file */
    char style;         /* save file style */
    Ff_file *ffile_pt;

    /* set to default (or not in use) */
    crlf = (DEFAULT_STYLE == MS_FILE);
    if ( style_pt ) *style_pt = 0;
    if ( user_style_pt ) *user_style_pt = 0;
    if ( fn < (FIRSTFILE + NTMPFILES) ) return (crlf);
    if ( ! (fileflags[fn] & INUSE) ) return (crlf);    /* entry not in used */

    ffile_pt = fnlas[fn].la_file->la_ffs->f_file;

    /* default for a newly created file */
    if ( (fileflags[fn] & NEW) && (ffile_pt->user_style == 0) ) 
	ffile_pt->style = DEFAULT_STYLE;

    style = ( ffile_pt->user_style ) ? ffile_pt->user_style : ffile_pt->style;
    if ( style == 0 ) style = DEFAULT_STYLE;
    crlf = ( style == MS_FILE );

    if ( style_pt ) *style_pt = ffile_pt->style;
    if ( user_style_pt ) *user_style_pt = ffile_pt->user_style;

    return (crlf);
}



/* file_style_string : get file style string */
/* ----------------------------------------- */

static char *style_string (style)
char style;
{
    static char unix_style[] = "Unix";
    static char ms_style[]   = "MS";

    if ( style == MS_FILE ) return (ms_style);
    if ( style == UNIX_FILE ) return (unix_style);
    return (NULL);
}

static char * file_style_string (fn, ms_flg, stylestrg_pt, user_stylestrg_pt) 
Fn fn;
int ms_flg;     /* YES if the MS style string must be returned */
char **user_stylestrg_pt, **stylestrg_pt;
{ 
    char style, fstyle, ustyle;     /* file style */
    char *strg, *fstrg, *ustrg;

    (void) file_mode (fn, &fstyle, &ustyle);
    fstrg = style_string (fstyle);
    ustrg = style_string (ustyle);
    if ( stylestrg_pt ) *stylestrg_pt = fstrg;
    if ( user_stylestrg_pt ) *user_stylestrg_pt = ustrg;
    
    strg = ( ustyle ) ? ustrg : fstrg;
    if ( ! ms_flg ) {
        style = ( ustyle ) ? ustyle : fstyle;
	if ( style == DEFAULT_STYLE ) strg = NULL;
    }
    return ( strg );
}


/* save_msg : save file message */
/* ---------------------------- */

static void save_msg (fn, fname_pt, extra) 
Fn fn;
char *fname_pt, *extra;
{
    char *stl;
    char str[32];

    stl = file_style_string (fn, NO, NULL, NULL);
    if ( stl ) sprintf (str, "SAVE (%s file) as: ", stl);
    else strcpy (str, "SAVE as: ");
    if ( extra ) mesg (TELALL + 3, str, fname_pt, extra);
    else mesg (TELALL + 2, str, fname_pt);
}


/* filestatusString, fileslistString  : build a string of the file status */
/* ---------------------------------------------------------------------- */

/* file flags string : ref to e.h INUSE ... SYMLINK defined value */
static char fileflags_string [] = "LURDNS.PCFDI";

char * fileStatusString (Short fflag, char *ffstrg)
{
    static char fileflagstrg [sizeof(fileflags_string)];
    int i;
    short ffmask;
    char * strg;

    strg = ( ffstrg ) ? ffstrg : fileflagstrg;
    strcpy (strg, fileflags_string);
    for ( i = strlen(strg) -1 , ffmask = 1 ; i >= 0 ; i--, ffmask <<= 1 ) {
	if ( strg[i] == '.' ) continue;
	if ( ! (fflag & ffmask) ) strg[i] = '-';
    }
    return strg;
}

static char * get_filestatusString (Fn fn, char **fname_pt, Flag file_sz_flg)
{
    static char strg[128];
    /*
    extern int is_aCdRom ();
    */
    char file_style;
    Ff_file *ffile_pt;
    char *sv_stylestrg, *fstylestrg;
    char ffstrg [sizeof(fileflags_string)];
    int i, flnb;
    short ffmask, fflag;
    int cdrom_flg;
    Flag modified_flg;

    *fname_pt = NULL;
    if ( (fn < FIRSTFILE + NTMPFILES) || (fn >= MAXFILES) ) return (NULL);
    if ( !fnlas[fn].la_file || !fnlas[fn].la_file->la_ffs ) return (NULL);
    ffile_pt = fnlas[fn].la_file->la_ffs->f_file;

    fflag = fileflags[fn];
    if ( ! (fflag & INUSE) ) return (NULL);

    memset (strg, 0, sizeof (strg));

    /* build the file status string */
    modified_flg = la_modified (&fnlas[fn]);
    fflag = fileflags[fn];
    (void) fileStatusString (fflag, ffstrg);

    /* build file state string */
    if ( file_sz_flg ) {
	flnb = ( fnlas[fn].la_file ) ? la_lsize (&fnlas[fn]) : 0;
	sprintf (strg, "%6d lines, (%s) %c : ",
		 flnb, ffstrg, modified_flg ? 'M' : '-');
    } else {
	/*
	cdrom_flg = is_aCdRom (names[fn]);
	*/
	cdrom_flg = NO;
	sv_stylestrg = file_style_string (fn, YES, &fstylestrg, NULL);
	if ( !sv_stylestrg || (sv_stylestrg == fstylestrg) ) {
	    if ( ! fstylestrg ) fstylestrg = "";    /* To Be assersed */
	    sprintf (strg, "%-4s%s file #%d (%s) %c : ",
		     fstylestrg, cdrom_flg ? " CD-ROM" : "", fn, ffstrg,
		     modified_flg ? 'M' : '-');
	}
	else {
	    if ( ! fstylestrg ) fstylestrg = "Tbd";
	    sprintf (strg, "%s%s file #%d (%s) saved as %s: ",
		fstylestrg, cdrom_flg ? " CD-ROM" : "", fn, ffstrg, sv_stylestrg);
	}
    }

    if ( fname_pt )
	*fname_pt = ( fileflags[fn] & NEW ) ? names[fn] : ffile_pt->fl_path;
    return (strg);
}

char * filestatusString (Fn fn, char **fname_pt)
{
    return get_filestatusString (fn, fname_pt, NO);
}

char * fileslistString (Fn fn, char **fname_pt)
{
    return get_filestatusString (fn, fname_pt, YES);
}

/* fileSatus : file query or set style */
/* ----------------------------------- */

Cmdret fileStatus (show_flg)
int show_flg;
{

    static S_looktbl fileStatusTable[] = {
        "?",            127,
	"linux",        UNIX_FILE,
        "microsoft",    MS_FILE, 
        "ms",           MS_FILE,
        "ms-dos",       MS_FILE,
        "msdos",        MS_FILE,
        "unix",         UNIX_FILE,
        0,              0
    };

    int shw_flg;
    Cmdret retval;
    char *arg;
    char file_style;
    Ff_file *ffile_pt;
    char *strg, *fname;

    arg = (char *) NULL;
    shw_flg = show_flg;
    if ( ! shw_flg ) {
        Small ind;

	if (!opstr[0])
	    return CRNEEDARG;

	ind = lookup (opstr, fileStatusTable);
	if (ind == -1 || ind == -2) {
	    mesg (ERRSTRT + 1, opstr);
	    return ind;
	}

	arg = getword(&nxtop);
	if (arg == NULL) {
	    cmdname = cmdopstr;
	    return CRNEEDARG;
        }
        file_style = (char) fileStatusTable[ind].val;
        shw_flg = (file_style == 127);
    }

    ffile_pt = fnlas[curfile].la_file->la_ffs->f_file;

    if ( shw_flg ) {
        /* display the file status */
	strg = filestatusString (curfile, &fname);
	if ( !strg ) {
            mesg (TELALL + 1, "This file entry in not in use !");
	    loopflags.hold = YES;
            return (CROK);
        }
	mesg (TELALL + 2, strg, fname);
	loopflags.hold = YES;
        return (CROK);
    }

    /* set user file style (to be used for saving the file */
    switch ( file_style ) {
        case MS_FILE:
            ffile_pt->user_style = MS_FILE;
	    loopflags.hold = NO;
            retval = CROK;
            break;

        case UNIX_FILE:
            ffile_pt->user_style = UNIX_FILE;
	    loopflags.hold = NO;
            retval = CROK;
            break;
    }
    return (retval);
}




#ifdef COMMENT
Cmdret
save ()
.
    Do the "save" command.
#endif
Cmdret
save ()
{
    if (curmark)
	return NOMARKERR;
    if (opstr[0] == '\0')
	return CRNEEDARG;
    if (*nxtop)
	return CRTOOMANYARGS;

    save_msg (curfile, opstr, NULL);
    savefile (opstr, curfile, NO, NO, NO);
    sleep (1);
    return CROK;
}

#ifdef COMMENT
Flag
savefile (filename, fn, svinplace, rmbak)
    char *filename;
    Fn    fn;
    Flag  svinplace;         /* enable inplace save */
    Flag  rmbak;
    Flag  filedir_flg;       /* YES : use the initial directory for this file */
.
  There are 2 cases to deal with.
    1. Doing a save as part of exit sequence
        NEW feature for file style :
            if la_modified is true, or if the file style was chnaged (by the user)
                a full save will be done
       a. Only remove the backup file
       b. Do the saving
    2. Doing a "save" command of fn to filename
  If filename is a null pointer, then we are doing case 1, else case 2
.
    savefile (filename, fn, svinplace, rmbak)
	if filename == NULL && rmbak != 0
	    remove backup file if any
	    return
	if filename != NULL
	    if filename is open for editing (this includes the case where
	      the filename is the name of the file)
		disallow the save
		return NO;
	else filename == NULL, meaning save to this file,
	    if not EDITED
		return YES
	if svinplace and file has multiple links
	    Copy original to backup
	    if filename == NULL
		Do an la_freplace to the backup filename
	    la_lflush the file to the original file
	else
	    write out file number fn to a temp file called ,esvXXXXXX
		where XXXXX is unique.
	    rename the original file to the backup name
	    rename the temp file to the original name
	if filename == 0
	    clear EDITED and CANMODIFY
	return YES;
.
	normal return is YES; error is NO;
#endif
/* VARARGS 3 */
Flag
savefile (filename, fn, svinplace, rmbak, filedir_flg)
char   *filename;       /* NULL for exit case */
Fn      fn;
Flag svinplace;         /* enable inplace save */
Flag rmbak;
Flag filedir_flg;       /* YES : use the initial directory for this file */
{
    extern char *cwdfiledir [];
    extern char default_workingdirectory[];
    char   *origfile,       /* origfile to be saved */
	   *dirname,        /* the directory for the save */
	   *filepart,       /* filename part of pathname */
	   *bakfile;
    register Short  tmp;
    register Short  j;
    Flag hasmultlinks;
    char cwdbuf [PATH_MAX], *cwd, *fdir;

    struct stat stbuf;
    int crlf_flg;       /* file new line style (UNIX (no) or MS-DOS (yes)); */

    crlf_flg = NO;      /* assume UNIX style file */

    /* new test for the file style feature */
    if (filename == NULL) {
        /* exit case */
        if ( ! la_modified (&fnlas[fn]) ) {
            char fstyle, ustyle;
            /* do not create an empty file */
            if ( fileflags[fn] & NEW ) 
                return (YES);
            (void) file_mode (fn, &fstyle, &ustyle);
            if ( !ustyle || (ustyle == fstyle) ) 
                return  (YES);  /* nothing to save */
        }
    }

    memset ((char *)&stbuf, 0, sizeof (stbuf));
    if ( fstat (la_chan (&fnlas[fn]), &stbuf) != 0 ) {
	stbuf.st_mode = 0600;
	}

    putline (YES);
    bakfile = NULL;
    if (filename == NULL)
	origfile = names[fn];
    else {
	if (   hvname (filename) != -1
	    || hvdelname (filename) != -1
	    || hvoldname (filename) != -1
	   ) {
	    mesg (ERRALL + 1, "Can't save to a file we are using");
	    return NO;
	}
	else if (   (j = filetype (origfile = filename)) != -1
		 && j != S_IFREG
		) {
	    mesg (ERRALL + 1, "Can only save to files");
	    return NO;
	}
    }

    /* get the directory name and the file part of the name */
    if (dircheck (origfile, &dirname, &filepart, YES, YES) == -1) {
	sfree (dirname);
	return NO;
    }

    /* make the backup name */
    if (prebak[0] != '\0') Block {
	char *cp;
	cp = append (prebak, filepart);
	bakfile = append (dirname, cp);
	sfree (cp);
    }
    if (postbak[0] != '\0')
	bakfile = append (origfile, postbak);

    if (filename == NULL && rmbak) {
	unlink (bakfile);
	sfree (dirname);
	sfree (bakfile);
	return YES;
    }

    /* if this is a saveall save, and the file was renamed,
     * or the filename is the same as the old name of another renamed file,
     * then we must do the renaming now
     **/
    if (filename == NULL) Block {
	Fn tmp;
	if (   (tmp = hvoldname (origfile)) != -1
	    && !svrename (tmp)
	   ) {
 err2:      sfree (dirname);
	    sfree (bakfile);
	    return NO;
	}
	if (   (fileflags[tmp = fn] & RENAMED)
	    && !svrename (tmp)
	   )
	    goto err2;
    }

    /* return to the file directory if needed */
    cwd = getcwd (cwdbuf, sizeof (cwdbuf));
    fdir = (cwdfiledir [fn]) ? cwdfiledir [fn] : default_workingdirectory;
    if ( filedir_flg ) (void) chdir (fdir);

    hasmultlinks = multlinks (origfile) > 1;
    if (svinplace) Block {
	Fd origfd;
	int origpriv;
	save_msg (fn, origfile,
	      hasmultlinks ? " (preserving link)" : " (inplace)");
	d_put (0);
	/* copy orig to backup */
	sfree (dirname);
	unlink (bakfile);            /* get rid of any old backup    */
	if ((origfd = open (origfile, 2)) == -1) {
 err1:      mesg (ERRALL + 3, "Unable to copy ", origfile, " to backup");
	    sfree (bakfile);
	    if ( cwd ) chdir (cwd);
	    return NO;
	}
	origpriv = fgetpriv (origfd);
	if ((j = filecopy ((char *) 0, origfd, bakfile, 0,
		     YES, fgetpriv (la_chan (&fnlas[fn]))))
	    == -2) {
	    mesg (ERRALL + 1, "Unable to create Backup");
	    goto err1;
	}
	else if (j < 0)
	    goto err1;

	if (   filename == NULL
	    && !la_freplace (bakfile, &fnlas[fn])
	   )
	    goto err1;
	sfree (bakfile);

	/* I wish that I could write to the file and then truncate it to the
	 * written length, rather than doing a creat on it and releasing
	 * all those blocks to the free list possibly to be gobbled up
	 * by someone else, leaving me stranded.
	 * But current unix can't truncate except by doing a creat.
	 **/
	if ((origfd = creat (origfile, origpriv)) == -1) {
	    mesg (ERRALL + 2, "Unable to create ", origfile);
	    if ( cwd ) chdir (cwd);
	    return NO;
	}
	crlf_flg = file_mode (fn, NULL, NULL);
	if (la_lflush (&fnlas[fn], (La_linepos) 0,
		       la_lsize (&fnlas[fn]), origfd, NO, NO, crlf_flg)
	    != la_lsize (&fnlas[fn])
	   ) {
	    mesg (ERRALL + 2, "Error to writing to ", origfile);
	    close (origfd);
	    if ( cwd ) chdir (cwd);
	    return NO;
	}
	close (origfd);
    }
    else

    Block {  /* not inplace */
	Fd tempfd;
	struct stat stbuf;
	char *mktemp ();
	char *tempfile;
	Nlines ltmp;

	save_msg (fn, origfile, hasmultlinks ? "( breaking link)" : "");
	d_put (0);
	tempfile = mktemp (append (dirname, ",esvXXXXXX"));
	sfree (dirname);
	if ((tempfd = creat (tempfile, groupid == 1 ? 0644 : 0664)) == -1) {
	    mesg (ERRALL + 1, "Unable to create tmp file!");
 retno:     sfree (tempfile);
	    if ( cwd ) chdir (cwd);
	    return NO;               /* error */
	}
	crlf_flg = file_mode (fn, NULL, NULL);
	if ((ltmp = la_lflush (&fnlas[fn], (La_linepos) 0,
		       la_lsize (&fnlas[fn]), tempfd, NO, NO, crlf_flg))
	    != la_lsize (&fnlas[fn])
	   ) {
	    mesg (ERRALL + 3, "Error saving ", origfile, " to disk");
	    unlink (tempfile);
	    close (tempfd);
	    goto retno;              /* error */
	}
	close (tempfd);

	/* if the name we just saved to is also in use as a deleted name,
	 * we have to clear the INUSE and DELETED flags for that fn so our
	 * file won't get deleted later as part of the saveall sequence
	 **/
	if ((tmp = hvdelname (origfile)) != -1) {
	    extern void marktickfile ();
	    unlink (origfile);
	    fileflags[tmp] &= ~(INUSE | DELETED);
	    marktickfile (tmp, NO);
	}

	tmp = mv (origfile, bakfile);
	sfree (bakfile);
	tmp = mv (tempfile, origfile);

	if (!(fileflags[fn] & NEW)) {
	    if (userid == 0) {
		fstat (la_chan (&fnlas[fn]), &stbuf);
		chown (origfile, stbuf.st_uid, stbuf.st_gid);
		chmod (origfile, (int) stbuf.st_mode & 0777);
	    }
	    else
		chmod (origfile, fgetpriv (la_chan (&fnlas[fn])));
	}
	sfree (tempfile);
    }

    if (filename == NULL) {
	/* see to it that nothing more will happen to this file */
	fileflags[fn] &= ~(CANMODIFY | NEW | DWRITEABLE);
	la_unmodify (&fnlas[fn]);
    }
    if ( cwd ) chdir (cwd);
    return YES;
}

#ifdef COMMENT
Flag
svrename (fn)
    Fn fn;
.
    Do the actual renaming of the file on disk.
    Called as part of the exit saving sequence.
    If all went OK return YES, else NO.
#endif
Flag
svrename (fn)
Reg1 Fn fn;
{
    Block {
	Reg2 char *old;
	Reg3 char *new;
	old = oldnames[fn];
	new = names[fn];
	mesg (TELALL + 4, "RENAME: ", old, " to ", new);
	if (!mv (old, new)) {
	    mesg (ERRALL + 4, "Can't rename ", old, " to ", new);
	    return NO;
	}
    }
    fileflags[fn] &= ~RENAMED;

    /* if the name we just renamed to is also in use as a deleted name,
     * we have to clear the INUSE and DELETED flags for that fn so our
     * file won't get deleted later as part of the saveall sequence
     **/
    Block {
	extern void marktickfile ();
	Reg2 Fn tmp;
	if ((tmp = hvdelname (names[fn])) != -1) {
	    fileflags[tmp] &= ~(INUSE | DELETED);
	    marktickfile (tmp, NO);
	}
    }
    return YES;
}
