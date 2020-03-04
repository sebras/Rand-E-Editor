/*
/* file e.sg.h: sgtty stuff
/**/

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#ifdef UNIXV7
#include <sgtty.h>
#endif
#ifdef UNIXV6
#include <sys/sgtty.h>
#endif

extern
struct sgttyb instty,
	      outstty;

#ifdef V6XSET
extern
struct xsgttyb inxstty;
#endif

Flag istyflg,
     ostyflg;

extern
unsigned Short oldttmode;
