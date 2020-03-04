#include	<stdio.h>

putlong (i, iop)
long i;
register FILE *iop;
{
    register tmp;
    register char *cp;

    cp = (char *) &i;
    tmp = sizeof (long);
    do
	putc (*cp++, iop);
    while (--tmp);
}
