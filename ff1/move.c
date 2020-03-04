/*
/*   file move.c   memory move
/*
/*   D. Yost
/*
/*   VAX version 3/21/80  D. Yost
/*   PDP-11 version 4/3/80 D. Yost
/*
/**/
#include <c_env.h>

#ifdef vax

char *
move (source, dest, count)
char *source, *dest;
unsigned int count;
{                                       /* MUST BE:     */
    register char         *rsource;     /* r11          */
    register char         *rdest;       /* r10          */
    register unsigned int  rcount;      /* r9           */
    unsigned int dist;
    unsigned int nsave;

    if ((rcount = count) == 0)
	return dest;
    if (rcount <= 65535) {
	asm ("        movc3   r9,*4(ap),*8(ap)");
    }
    else if (   source > dest
	     || &source[rcount] <= dest
	    ) {
	rdest = dest;
	rsource = source;
	for (;;) {
	    if (rcount <= 65535) {
		asm ("        movc3   r9,(r11),(r10)");
		break;
	    }
	    else {
		asm ("        movc3   $65535,(r11),(r10)");
		rcount -= 65535;
		rsource += 65535;
		rdest += 65535;
	    }
	}
    }
    else {
	rdest = &dest[rcount];
	rsource = &source[rcount];
	if ((dist = rdest - rsource) > 16) {
	    unsigned int nmove, resid;

	    nmove = rcount / dist;
	    resid = rcount % dist;
	    while (nmove--) {
		move (rsource -= dist, rdest -= dist, dist);
	    }
	    if (resid)
		move (rsource - resid, rdest - resid, resid);
	}
	else {
	    if ((dist & 3) == 0) {
		if ((long) rsource & 1) {
		    *--rdest = *--rsource;
		    --rcount;
		}
		if ((long) rsource & 2) {
		/*  *--((short *) rdest) = *--((short *) rsource);  */
		    asm ("        movw    -(r11),-(r10)");
		    rcount -= 2;
		}
		nsave = rcount;
		rcount >>= 2;
		do {
		/*  *--((long *) rdest) = *--((long *) rsource);    */
		    asm ("        movl    -(r11),-(r10)");
		} while (--rcount);
		if (nsave & 2) {
		/*  *--((short *) rdest) = *--((short *) rsource);  */
		    asm ("        movw    -(r11),-(r10)");
		}
		if (nsave & 1)
		    *--rdest = *--rsource;
	    }
	    else if ((dist & 1) == 0) {
		if ((long) rsource & 1) {
		    *--rdest = *--rsource;
		    --rcount;
		}
		nsave = rcount;
		rcount >>= 1;
		do {
		/*  *--((short *) rdest) = *--((short *) rsource);  */
		    asm ("        movw    -(r11),-(r10)");
		} while (--rcount);
		if (nsave & 1)
		    *--rdest = *--rsource;
	    }
	    else
		do {
		    *--rdest = *--rsource;
		} while (--rcount);
	}
    }
    return &dest[count];
}

#else


/*  Source and dest should really be declared as unions, but the compilers
/*  either totally reject the idea of a union as an argument or won't put
/*  a union into a register even if it is a union of single items.
/*  Ritchie's Phototypesetter Version 7 compiler accepts the args as (char *)
/*  and generates the desired optimal code.
/**/
union bw {
    char         *b;
    short        *w;
    unsigned int  i;
};

char *
move (source, dest, count)
# ifdef UNIONS_IN_REGISTERS
register union bw source, dest;
# else
register char *source, *dest;
# endif
unsigned int count;
{
    register unsigned int rcount;
    unsigned int nsave;

    if ((rcount = count) == 0)
	return dest.b;
    if ( source.b > dest.b || &source.b[rcount] <= dest.b ) {
	if ((rcount < 10) || ((source.i ^ dest.i) & 1))
	    do {
		*dest.b++ = *source.b++;
	    } while (--rcount);
	else {
	    if (source.i & 1) {
		*dest.b++ = *source.b++;
		--rcount;
	    }
	    nsave = rcount;  rcount >>= 1;
	    do {
		*dest.w++ = *source.w++;
	    } while (--rcount);
	    if (nsave & 1)
		*dest.b++ = *source.b++;
	}
	return dest.b;
    } else {
	dest.b = &dest.b[rcount];
	source.b = &source.b[rcount];
	if ((rcount < 10) || ((source.i ^ dest.i) & 1))
	    do {
		*--dest.b = *--source.b;
	    } while (--rcount);
	else {
	    if (source.i & 1) {
		*--dest.b = *--source.b;
		--rcount;
	    }
	    nsave = rcount; rcount >>= 1;
	    do {
		*--dest.w = *--source.w;
	    } while (--rcount);
	    if (nsave & 1)
		*--dest.b = *--source.b;
	}
	return &dest.b[count];
    }
}

#endif

#ifdef _MOVETEST

#include <stdio.h>

#ifdef vax
#define BOUND 65536
#else
#define BOUND 100
#endif

char alph[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char test[BOUND+500] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

main (argc, argv)
int argc;
char *argv[];
{
    char *source;
    char *dest;
    unsigned int cnt;

    if (argc != 4){
	    printf ("%s: source dest count\n", argv[0]);
	    exit (1);
    }
    move (&alph[36], &test[BOUND-10-26], 26);
    move (alph, &test[BOUND-10], 62);

    printf ("%39.39s %39.39s\n", test, &test[BOUND-10]);
    source = &test[atoi (argv[1])];
    dest =   &test[atoi (argv[2])];
    cnt = atoi (argv[3]);
    printf ("%d\n", move (source, dest, cnt) - dest);
    printf ("%39.39s %39.39s\n", test, &test[BOUND-10]);
    return 0;
}
#endif
