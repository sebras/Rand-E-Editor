#ifdef COMMENT
--------
file e.tt.c
    Terminal-dependent code and data declarations
#endif

#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

#ifdef  TERMSIM
#include "e.h"
#include "e.tt.h"

/* the following is to save code space */
#ifndef BIGADDR
#undef putchar
#define putchar(c) fputc(c,stdout)
#endif

char   *kname;                  /* name of keyboard type */
char   *tname;                  /* name of terminal type */
Small   kbdtype;                /* which kind of keyboard */
Small   termtype;               /* which kind of terminal */
Short   screensize;             /* term.tt_width * term.tt_height */
Flag    fast;                   /* running at a fast baud rate (>= 4800) */
char    ospeed;                 /* tty output baud rate */
#ifdef  BULLETS
Flag    borderbullets = YES;    /* Enable bullets on borders */
#endif  BULLETS
S_term  term;
S_kbd   kbd;
Flag tt_lt2;            /* 2 lefts take fewer characters than an addr */
Flag tt_lt3;            /* 3 lefts take fewer characters than an addr */
Flag tt_rt2;            /* 2 rights take fewer characters than an addr */
Flag tt_rt3;            /* 3 rights take fewer characters than an addr */

#ifdef COMMENT
   TO ADD A TERMINAL TYPE:
	 Increment NTERMINALS by 1
	 Define a T_xx manifest constant for the terminal
	 Add the names of the terminal to the termnames table.
	 Add an include for '../term/xx.c' in this file.
	 Add the TM_xx entry to the "tterm" structure initialization
	   at the end of this file.
	 Add the KB_xx entry to the "tkbd" structure initialization
	   at the end of this file.
	 See ../term/Doc for more instructions.
	 Write a "../term/xx.c" file for the terminal.
	 Add ../term/xx.c to Makefile dependencies

   Terminal #1 is the default if you are on a system without environment
   variables.

#endif

#ifdef TERMCAP
#define T_tcap      0   /* tcap.c       Termcap terminal -- must be 0 */
#endif
#define T_std       0   /* stdinlex.c   Standard keyboard */
#define T_aa        1   /* annarbor.c   Ann Arbor 4080 */
#define T_a1        2   /* ambas.c      Ann Arbor Ambassador */
#define T_3a        3   /* adm3a.c      Lear Siegler ADM3a */
/* fine T_31        4   /* adm31.c      Lear Siegler ADM31 */
/* fine T_dy        5   /* dy.c         Dave Yost terminal */
/* fine T_dm4000    6   /* dm4000.c     Datamedia DM4000 */
#define T_h19       7   /* h19.c        Heathkit H19 & H89 */
#define T_intext    8   /* intext.c     INTERACTIVE Systems Intext */
/* fine T_adm42     9   /* adm42.c      Lear Siegler ADM42 */
/* fine T_c100      10  /* c100.c       Concept 100 */
/* according to /etc/termcap there are 4 flavors.  I don't know what to do
   about that for now.  --dave yost
/**/
#define T_2intext   11  /* intext2.c    INTERACTIVE Systems Intext2 */
#define T_po        12  /* po.c         Perkin Elmer 1251 and Owl */
#define T_vt100     13  /* vt100.c      DEC VT100 -- needs termcap */

#define NTERMINALS  14

S_looktbl termnames[] = {
    /* must be sorted in ascending ascii order */
#ifdef T_31
    "31",           T_31,
#endif
#ifdef T_3a
    "3a",           T_3a,
#endif
#ifdef T_aa
    "aa",           T_aa,
#endif
#ifdef T_a1
    "aaa",          T_a1,
    "aaa18",        T_a1,
    "aaa20",        T_a1,
    "aaa22",        T_a1,
    "aaa24",        T_a1,
    "aaa26",        T_a1,
    "aaa28",        T_a1,
    "aaa30",        T_a1,
    "aaa36",        T_a1,
    "aaa40",        T_a1,
    "aaa48",        T_a1,
    "aaa60",        T_a1,
#endif
#ifdef T_31
    "adm31",        T_31,
#endif
#ifdef T_3a
    "adm3a",        T_3a,
#endif
#ifdef T_adm42
    "adm42",        T_adm42,
#endif
#ifdef T_a1
    "ambas",        T_a1,
    "ambassador",   T_a1,
#endif
#ifdef T_aa
    "annarbor",     T_aa,
#endif
#ifdef T_c100
    "c100",         T_c100,
    "co",           T_c100,
    "concept",      T_c100,
    "concept100",   T_c100,
#endif
    "default",      1,
#ifdef T_dm4000
    "dm4000",       T_dm4000,
#endif
#ifdef T_dy
    "dy",           T_dy,
#endif
#ifdef T_h19
    "h19",          T_h19,
#endif
#ifdef T_intext
    "in",           T_intext,
    "intext",       T_intext,
#endif
#ifdef T_2intext
    "intext2",      T_2intext,
#endif
#ifdef T_h19
    "k1",           T_h19,
#endif
#ifdef T_31
    "l1",           T_31,
#endif
#ifdef T_adm42
    "l4",           T_adm42,
#endif
#ifdef T_3a
    "la",           T_3a,
#endif
#ifdef T_po
    "po",           T_po,
#endif
    "standard",     T_std,
#ifdef T_vt100
    "vt100",        T_vt100,
#endif
    0,              0,
};

/*  This file and the following files have to work on both the current and
/*  the version, so I have to fix some stuff.
/**/
#include "../term/eoldfix.h"

#include "../term/standard.c"

/* I did it this way, including everything into one compile, so that
   the optimizer pass could strip out lots of duplication with its
   clever optimization of common code before jumps.
   Unfortunately, for now at least, the vax optimizer doesn't understand
   that it can optimize common code before 'ret' instructions, so
   it's not too red hot on the vax.
   I think the 11 should do better, but I haven't tried it yet.
   1/20/81 dave-yost
/**/

#include "../term/stdinlex.c"
#define KB_std &kb_std

#ifdef T_tcap
# include "../term/tcap.c"
# define TM_tcap &t_tcap
# define KB_tcap &kb_std
#else
# define TM_tcap ((S_term *) 0)
# define KB_tcap ((S_kbd *) 0)
#endif

#ifdef T_aa
# include "../term/annarbor.c"
# define TM_aa &t_aa
# define KB_aa &kb_aa
#else
# define TM_aa ((S_term *) 0)
# define KB_aa ((S_kbd *) 0)
#endif

#ifdef T_a1
# include "../term/ambas.c"
# define TM_a1 &t_a1
# define KB_a1 &kb_a1
#else
# define TM_a1 ((S_term *) 0)
# define KB_a1 ((S_kbd *) 0)
#endif

#ifdef T_3a
# include "../term/adm3a.c"
# define TM_3a &t_3a
# define KB_3a &kb_std
#else
# define TM_3a ((S_term *) 0)
# define KB_3a ((S_kbd *) 0)
#endif

#ifdef T_31
# include "../term/adm31.c"
# define TM_31 &t_31
# define KB_31 &kb_std
#else
# define TM_31 ((S_term *) 0)
# define KB_31 ((S_kbd *) 0)
#endif

#ifdef T_dy
# include "../term/dy.c"
# define TM_dy &t_dy
# define KB_dy &kb_std
#else
# define TM_dy ((S_term *) 0)
# define KB_dy ((S_kbd *) 0)
#endif

#ifdef T_dm4000
# include "../term/dm4000.c"
# define TM_dm4000 &t_dm4000
# define KB_dm4000 &kb_std
#else
# define TM_dm4000 ((S_term *) 0)
# define KB_dm4000 ((S_kbd *) 0)
#endif

#ifdef T_h19
# include "../term/h19.c"
# define TM_h19 &t_h19
# define KB_h19 &kb_h19
#else
# define TM_h19 ((S_term *) 0)
# define KB_h19 ((S_kbd *) 0)
#endif

#ifdef T_intext
# include "../term/intext.c"
# define TM_intext &t_intext
# define KB_intext &kb_intext
#else
# define TM_intext ((S_term *) 0)
# define KB_intext ((S_kbd *) 0)
#endif

#ifdef T_2intext
# include "../term/intext2.c"
# define TM_2intext &t_2intext
# define KB_2intext &kb_2intext
#else
# define TM_2intext ((S_term *) 0)
# define KB_2intext ((S_kbd *) 0)
#endif

#ifdef T_adm42
# include "../term/adm42.c"
# define TM_adm42 &t_adm42
# define KB_adm42 &kb_adm42
#else
# define TM_adm42 ((S_term *) 0)
# define KB_adm42 ((S_kbd *) 0)
#endif

#ifdef T_c100
# include "../term/c100.c"
# define TM_c100 &t_c100
# define KB_c100 &kb_c100
#else
# define TM_c100 ((S_term *) 0)
# define KB_c100 ((S_kbd *) 0)
#endif

#ifdef T_po
# include "../term/po.c"
# define TM_po &t_po
# define KB_po &kb_std
#else
# define TM_po ((S_term *) 0)
# define KB_po ((S_kbd *) 0)
#endif

#ifdef T_vt100
# include "../term/vt100.c"
# define TM_vt100 &t_tcap
# define KB_vt100 &kb_vt100
#else
# define TM_vt100 ((S_term *) 0)
# define KB_vt100 ((S_kbd *) 0)
#endif

/* the entries in this structure must be in the order of their
/* defined T_ values
/**/
S_term *tterm[NTERMINALS] = {
    TM_tcap     ,
    TM_aa       ,
    TM_a1       ,
    TM_3a       ,
    TM_31       ,
    TM_dy       ,
    TM_dm4000   ,
    TM_h19      ,
    TM_intext   ,
    TM_adm42    ,
    TM_c100     ,
    TM_2intext  ,
    TM_po       ,
    TM_vt100    ,
};

/* the entries in this structure must be in the order of their
/* defined T_ values
/**/
S_kbd *tkbd[NTERMINALS] = {
    KB_std      ,
    KB_aa       ,
    KB_a1       ,
    KB_3a       ,
    KB_31       ,
    KB_dy       ,
    KB_dm4000   ,
    KB_h19      ,
    KB_intext   ,
    KB_adm42    ,
    KB_c100     ,
    KB_2intext  ,
    KB_po       ,
    KB_vt100    ,
};

#endif  TERMSIM
