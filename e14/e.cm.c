/*
/* file e.cm.c - command stuff
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#include "e.h"
#include "e.m.h"
#include "e.ru.h"
#include "e.cm.h"

command ()
{
    Short cmdtblind;
    Small retval;
    Short cmdval;
    char *cmdstr;

    nxtop = paramv;
    cmdstr = getword (&nxtop);
    if (cmdstr[0] == 0)
	return CROK;
    cmdtblind = lookup (cmdstr, cmdtable);
    if (cmdtblind == -1) {
	mesg (ERRALL + 3, "Command \"", cmdstr, "\" not recognized");
	sfree (cmdstr);
	return CROK;
    }
    else if (cmdtblind == -2) {
	mesg (ERRALL + 3, "Command \"", cmdstr, "\" ambiguous");
	sfree (cmdstr);
	return CROK;
    }
    sfree (cmdstr);

    cmdopstr = nxtop;
    opstr = getword (&nxtop);

    cmdname = cmdtable[cmdtblind].str;
    switch (cmdval = cmdtable[cmdtblind].val) {
    case CMDUPDATE:
    case CMD_UPDATE:
	retval = doupdate (cmdval == CMDUPDATE);
	break;

    case CMDTAB:
	retval = dotab (YES);
	break;

    case CMD_TAB:
	retval = dotab (NO);
	break;

    case CMDTABS:
	retval = dotabs (YES);
	break;

    case CMD_TABS:
	retval = dotabs (NO);
	break;

    case CMDTABFILE:
	retval = tabfile (YES);
	break;

    case CMD_TABFILE:
	retval = tabfile (NO);
	break;

    case CMDREPLACE:
    case CMD_REPLACE:
	retval = replace (cmdval == CMDREPLACE? 1: -1);
	break;

#ifdef NAME
    case CMDNAME:
	retval = name ();
	break;
#endif NAME

#ifdef DELETE
    case CMDDELETE:
	retval = delete ();
	break;
#endif DELETE

    case CMDCOMMAND:
	mesg (TELALL + 1, "CMDS: ");
	cmdmode = YES;
	return CROK;

    case CMD_COMMAND:
	cmdmode = NO;
	return CROK;

    case CMDOPEN:
    case CMDPICK:
    case CMDCLOSE:
    case CMDERASE:
	if (!okwrite ())
	    return NOWRITERR;
	retval = areacmd (cmdval - CMDOPEN);
	break;

    case CMD_PICK:
    case CMD_CLOSE:
    case CMD_ERASE:
	if (curmark)
	    return NOMARKERR;
	if (!okwrite ())
	    return NOWRITERR;
	if (*cmdopstr) {
	    retval = CRTOOMANYARGS;
	    break;
	}
	flushkeys ();
	{
	S_svbuf *buf;

	buf =  cmdval == CMD_PICK
	       ? pickbuf
	       : (cmdval == CMD_CLOSE
		  ? closebuf
		  : erasebuf);
	if (buf->nlins == 0)
	    return NOBUFERR;
	}
	put (cmdval == CMD_PICK? pickbuf
	       : (cmdval == CMD_CLOSE? closebuf: erasebuf),
	     curwksp->wlin + cursorline,
	     curwksp->wcol + cursorcol);
	return CROK;

    case CMDCALL:
	retval = call ();
	/* if the syntax of the shell command was correct and all of the
	/* saves went OK, forkshell will never return. */
	break;

    case CMDSHELL:
	retval = shell ();
	/* if the syntax of the shell command was correct and all of the
	/* saves went OK, forkshell will never return. */
	break;

    case CMDEXIT:
	retval = eexit ();
	/* if the syntax of the exit command was correct and all of the
	/* saves went OK, eexit will never return. */
	break;

    case CMDREDRAW:
	fresh ();
	return CROK;

    case CMDJOIN:
	if (!okwrite ())
	    return NOWRITERR;
	if (*cmdopstr) {
	    retval = CRTOOMANYARGS;
	    break;
	}
	if (curmark)
	    return joinmark ();
	joinlines (curwksp->wlin + cursorline,
		      curwksp->wcol + cursorcol,
		      1, 0, YES);
	return CROK;

    case CMDSPLIT:
	if (!okwrite ())
	    return NOWRITERR;
	if (*cmdopstr) {
	    retval = CRTOOMANYARGS;
	    break;
	}
	if (curmark)
	    return splitmark ();
	splitlines (curwksp->wlin + cursorline,
		     curwksp->wcol + cursorcol,
		      1, 0, YES);
	return CROK;

    case CMDRUN:
    case CMDFEED:
	if (!okwrite ())
	    return NOWRITERR;
	retval = run (cmdopstr, cmdval == CMDRUN);
	break;

    case CMDFILL:
	retval = filter (FILLNAMEINDEX, YES);
	break;

    case CMDJUST:
	retval = filter (JUSTNAMEINDEX, YES);
	break;

    case CMDCENTER:
	retval = filter (CENTERNAMEINDEX, YES);
	break;


    case CMDSAVE:
	if (curmark)
	    return NOMARKERR;
	retval = save ();
	break;

    case CMDUSE:
	mesg (ERRALL + 1, "type \"e\" or \"edit\" instead of \"use\"");
	return CROK;

    case CMDEDIT:
	if (curmark)
	    return NOMARKERR;
	retval = use ();
	break;

    case CMDWINDOW:
	if (curmark)
	    return NOMARKERR;
	if (*opstr == '\0')
	    makeport ((char *)0);
	else {
	    if (*nxtop) {
		retval = CRTOOMANYARGS;
		break;
	    }
	    makeport (opstr);
	}
	bulsw = YES;
	return CROK;

    case CMD_WINDOW:
	if (curmark)
	    return NOMARKERR;
	if (*opstr) {
	    retval = CRTOOMANYARGS;
	    break;
	}
	removeport ();
	return CROK;

    case CMDGOTO:
	retval = gotocmd ();
	break;

    default:
	mesg (ERRALL + 3, "Command \"", cmdtable[cmdtblind].str,
		"\" not implemented yet");
	return CROK;
    }
    if (opstr[0] != '\0')
	sfree (opstr);

    if (retval >= CROK)
	return retval;
    switch (retval) {
    case CRUNRECARG:
	mesg (1, " unrecoginzed argument to ");
	break;

    case CRAMBIGARG:
	mesg (1, " ambiguous argument to ");
	break;

    case CRTOOMANYARGS:
	mesg (ERRSTRT + 1, "Too many of arguments to ");
	break;

    case CRNEEDARG:
	mesg (ERRSTRT + 1, "Need an argument to ");
	break;

    case CRNOVALUE:
	mesg (ERRSTRT + 1, "No value for option to ");
	break;

    case CRMULTARG:
	mesg (ERRSTRT + 1, "Duplicate arguments to ");
	break;

    case CRMARKCNFL:
	return NOMARKERR;

    case CRBADARG:
    default:
	mesg (ERRSTRT + 1, "Bad argument(s) to ");
    }
    mesg (ERRDONE + 3, "\"", cmdname, "\"");

    return CROK;
}


gotocmd ()
{
    Short tmp;
    char *cp;

    if (opstr[0] == '\0') {
	gtfcn (0);
	return CROK;
    }
    if (*nxtop)
	return CRTOOMANYARGS;

    cp = opstr;
    tmp = getpartype (&cp, 0, 0, 0);
    if (tmp == 1) {
	for (; *cp && *cp == ' '; cp++)
	    {}
	if (*cp == 0) {
	    gtfcn (parmlines - 1);
	    return CROK;
	}
    }
    else if (tmp == 2)
	return CRBADARG;
    if (opstr[1] != '\0')
	goto unrec;

    switch (opstr[0]) {
    case 'b':
	gtfcn (0);
	break;

    case 'e':
	gtfcn (nlines[curfile]);
	break;

    default:
    unrec:
	mesg (ERRSTRT + 1, opstr);
	return CRUNRECARG;
    }

    return CROK;
}

S_looktbl updatetable[] = {
    "-inplace", 0,
    "inplace",  0,
    0        ,  0
};

doupdate (on)
Flag on;
{
    Small ind;

    if (*nxtop || !on && opstr[0] != '\0')
	return CRTOOMANYARGS;

    if (on && !(fileflags[curfile] & DWRITEABLE)) {
	mesg (ERRALL + 1, "Don't have directory write permission");
	return CROK;
    }
    if (opstr[0] != '\0') {
	ind = lookup (opstr, updatetable);
	if (ind == -1  || ind == -2) {
	    mesg (ERRSTRT + 1, opstr);
	    return ind;
	}
	/* at this point, ind can be 0 = -inplace or 1 = inplace */
	if (ind) { /* inplace */
	    if (fileflags[curfile] & NEW) {
		mesg (ERRALL + 1, "\"inplace\" is useless on a new file");
		return CROK;
	    }
	    if (!(fileflags[curfile] & FWRITEABLE)) {
		mesg (ERRALL + 1, "Don't have file write permission");
		return CROK;
	    }
	    fileflags[curfile] |= INPLACE;
	}
	else /* -inplace */
	    fileflags[curfile] &= ~INPLACE;
    }
    if (on)
	fileflags[curfile] |= UPDATE;
    else
	fileflags[curfile] &= ~UPDATE;
    return CROK;
}
