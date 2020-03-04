/*
/* file e.t.h - header file that is terminal dependent
/*
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

/* MAXWIDTH is the maximum of all defined widths in e.tt.h */
#define MAXWIDTH 80             /* length of longest possible screen line */
/* if (MAXWIDTH > 127)  then you must define WIDE
/**/
/* #define WIDE /**/

#ifdef WIDE
typedef Short Scols;            /* number of columns on the screen */
typedef short AScols;           /* number of columns on the screen */
#else
typedef Char Scols;
typedef char AScols;
#endif
typedef Char Slines;            /* number of lines on the screen */
typedef char ASlines;           /* number of lines on the screen */

/* margin characters and others */
#define FIRSTSPCL  127
#define ESCCHAR 127     /* escape character */
#define BULCHAR 128     /* bullet character */
#define FIRSTMCH   129
#define LMCH    129     /* left */
#define RMCH    130     /* right */
#define MLMCH   131     /* more left */
#define MRMCH   132     /* more right */
#define TMCH    133     /* top */
#define BMCH    134     /* bottom */
#define TLCMCH  135     /* top left corner */
#define TRCMCH  136     /* top right corner */
#define BLCMCH  137     /* bottom left corner */
#define BRCMCH  138     /* bottom right corner */
#define TTMCH   139     /* top tab */
#define BTMCH   140     /* bottom tab */
#define ELMCH   141     /* empty left */
#define INMCH   142     /* inactive */
#define NSPCHR      16  /* number of special characters */

#define NMCH        14  /* number of margin characters */
