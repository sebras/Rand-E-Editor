/*   C Standard defines.
/*   This file containes machine- and compiler-specific #defines
/**/

/*  In the #define's that follow, define the first n to 'register'
    and the rest to nothing, where 'n' is the number of registers
    supported by your compiler.
    For an explanation of this, see ../man/man5/c_env.5
/**/
#define Reg1  register
#define Reg2  register
#define Reg3  register
#define Reg4
#define Reg5
#define Reg6
#define Reg7
#define Reg8
#define Reg9
#define Reg10
#define Reg11
#define Reg12

#define CHARMASK   0xFF
#define CHARNBITS  8
#define MAXCHAR    0x7F

#define SHORTMASK  0xFFFF
#define SHORTNBITS 16
#define MAXSHORT   0x7FFF

#define LONGMASK  0xFFFFFFFF
#define LONGNBITS 32
#define MAXLONG   0x7FFFFFFF

#define INTMASK  0xFFFFFFFF
#define INTNBITS 32
#define MAXINT   0x7FFFFFFF

/* fine BIGADDR         /* text address space > 64K */
/* fine ADDR64K         /* text and data share 64K of memory (no split I&D */

/* fine INT4            /* sizeof (int) == 4 */
#define INT2            /* sizeof (int) == 2 */

/* fine PTR4            /* sizeof (char *) == 4 */
#define PTR2            /* sizeof (char *) == 2 */

			/* unsigned types supported by the compiler: */
/* fine UNSCHAR         /* unsigned char  */
/* fine UNSSHORT        /* unsigned short */
/* fine UNSLONG         /* unsigned long  */

/* fine NOSIGNEDCHAR    /* there is no signed char type */

#define STRUCTASSIGN	/* Compiler does struct assignments */

/* fine UNIONS_IN_REGISTERS     /* compiler allows unions in registers */
