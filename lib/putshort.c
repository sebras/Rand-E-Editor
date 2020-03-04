#include	<stdio.h>

putshort (i, iop)
short i;
register FILE *iop;
{
    register tmp;
    register char *cp;

    cp = (char *) &i;
    tmp = sizeof (short);
    do
	putc (*cp++, iop);
    while (--tmp);
}
