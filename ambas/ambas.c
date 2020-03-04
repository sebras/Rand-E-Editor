

#include <stdio.h>
#include "ambas.h"

#define STUFF       0
#define PUSH        1
#define POP         2
#define NULLSTRINGS 3
int push = STUFF;       /* what we are to do */

char *file;     /* filename for terminal setup */

main (argc, argv)
int    argc;
char **argv;
{
    int level;
    char *state;
    static char statename[] = "/.ambas";

    while (--argc > 0) {
	if (!strcmp (*++argv, "-pop"))
	    push = POP;
	else if (!strcmp (*argv, "-push"))
	    push = PUSH;
	else if (!strcmp (*argv, "-file")) {
	    if (--argc <= 0)
		fprintf (stderr, "Must be a file name after '-file'\n");
	    file = *++argv;
	}
	else if (!strcmp (*argv, "-null"))
	    push = NULLSTRINGS;
    }

#ifdef NOTYET
    if (!(home = getenv ("HOME")))
	fprinf (stderr, "No HOME environment variable\n");
    state = malloc (strlen (home) + sizeof statename);
    strcpy (state, home);
    strcat (state, statename);

    level = 0;
    if (stiop = fopen (state, "r") {
	scanf (stiop, "%d", &level);
	fclose (stiop);
    }
    else
	level = 0;

    /* OH, GOD !! this is a big job! */
#endif

    /* the following is a temporary hack */
    switch (push) {
    case PUSH:
	break;

    case NULLSTRINGS:
	putnull ((struct string *) &ini_strings);
	putnull ((struct string *) &ini_keys);
	break;

    default:
    case STUFF:
    case POP:
	putstrtbl ((struct string *) &ini_strings);
	putstrtbl ((struct string *) &ini_keys);
	break;
    }
    exit (0);
}

putstrtbl (tbl)
struct string *tbl;
{
    for (; tbl->name; tbl++)
	printf ("\033P`%c%s\033\\", tbl->name, tbl->sbuf);
    return;
}

putnull (tbl)
struct string *tbl;
{
    for (; tbl->name; tbl++)
	printf ("\033P`%c\033\\", tbl->name);
    return;
}
