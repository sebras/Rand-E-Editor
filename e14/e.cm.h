/*
/* file e.cm.h - return values for functions called by command ()
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#define CROK            0
#define CRUNRECARG      -1      /* matches lookup return */
#define CRAMBIGARG      -2      /* matches loup return */
#define CRTOOMANYARGS   -3
#define CRNEEDARG       -4
#define CRBADARG        -5
#define CRNOVALUE       -6
#define CRMULTARG       -7
#define CRMARKCNFL      -8
