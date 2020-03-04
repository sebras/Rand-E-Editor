#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

/*  if the C compiler doesn't support signed characters, you must
/*  #define NOSIGNEDCHAR /**/
/**/
#define NLLINE  7        /* 1 less than the number of bits per char */
#define LLINE   (~0x7f)  /* 1's in all but msb of a char */

/* fsd - file segment descriptor.  describes 1 to 127 contiguous lines
	on a file.  this is the atomic workspace component.
/**/
#define FSDMAXL 127		/* max nr lines in an fsd		*/
struct fsd
{
    struct fsd *backptr;	/* previous fsd in chain		*/
    struct fsd *fwdptr; 	/* next fsd in chain			*/
    long	seekpos;	/* location of first character		*/
    ASmall      fsdnlines;      /* number of lines in this fsd. 0 if    */
				/*  this is the last fsd in chain.	*/
    ASmall      fsdfile;        /* channel number of file containing    */
				/*  lines.				*/
				/*  0 if this is the last fsd in chain. */
    ASmall      fsdbytes[1];    /* a variable number of bytes - as many */
				/*  as are needed to describe		*/
				/*  fsdnlines-1 more lines.		*/
				/* byte values: 			*/
				/*  0	  sneak in a blank line here.	*/
				/*  1-127 next line starts this many	*/
				/*   chars after the start of this line.*/
				/*  -n	  next line starts		*/
				/*   n*128+(next byte) chars after the	*/
				/*  start of this line. 		*/
				/* There are at least fsdnlines-1	*/
				/*  bytes but may be more if lines	*/
				/*  are 128 or more characters long.	*/
				/* Note that lines in the		*/
				/*  file need not be contiguous to be	*/
				/*  describable in the same fsd but the */
				/*  output routine demands it.		*/
};
#define SFSD (sizeof (S_fsd))   /* byte size of fsd exclusive of fsdbytes */
#define FAKEFSD 127

