#include	<stdio.h>

long
getlong (iop)
register FILE *iop;
{
    long i;
    register tmp;
    register char *cp;

    cp = (char *) &i;
    tmp = sizeof (long);
    *cp++ = getc (iop);
    if (iop->_flag&_IOEOF)
	return -1;
    --tmp;
    do
	*cp++ = getc (iop);
    while (--tmp);

    return i;
}
