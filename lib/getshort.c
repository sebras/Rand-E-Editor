#include	<stdio.h>

short
getshort (iop)
register FILE *iop;
{
    short i;
    register tmp;
    register char *cp;

    cp = (char *) &i;
    tmp = sizeof (short);
    *cp++ = getc (iop);
    if (iop->_flag&_IOEOF)
	return -1;
    --tmp;
    do
	*cp++ = getc (iop);
    while (--tmp);

    return i;
}
