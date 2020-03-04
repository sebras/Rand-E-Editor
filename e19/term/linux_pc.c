/* ====================================================================== */
/*
    keyboard_map.c : keyboard mapping package :
	Assess and test the keyboard mapping for IBM PC 101 keys
	keyboard style.
	v1 : linux version only
    by : Fabien Perriollat : Fabien.Perriollat@cern.ch
    version 1.0 Nov 1998
*/
/* ---------------------------------------------------------------------- */
/*  Exported routines :
	Flag get_keyboard_map (char *terminal, int strg_nb, char *strg1, ...
	char * get_keyboard_mode_strg ()
	void all_string_to_key (char *escp_seq, ...
	void check_keyboard ()
	void display_keymap (Flag verbose)
*/
/* ====================================================================== */
static const char version[] = "version 1.0 Nov 1998 by Fabien.Perriollat@cern.ch";

/* #define TEST_PROGRAM */
/* build an autonomous program to test of this code */

/* ---------------------------------------------------------------------- */

#ifdef TEST_PROGRAM
#define CCUNAS1 0202 /* defined in e.h */
typedef char Flag;
#define NO 0
#define YES 1
#include <signal.h>
#else  /* -TEST_PROGRAM */
#include "e.h"
#include "e.it.h"
#include "e.tt.h"
#endif /* TEST_PROGRAM */

#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include <X11/keysym.h>

struct _keyboard_struct {
    int idx;    /* index in keyboard_desc of 1st key of the class */
    int nb;     /* nb of key in this class */
    char *label;    /* class label */
    }
    pc101_keyboard_struct [] = {
	{ 0, 12, "Function keys" },
	{ 0,  4, "Cursor key" },
	{ 0,  6, "Extended key pad" },
	{ 0, 17, "Numeric Key Pad" },
	{ 0,  2, "Miscellaneous key" }
    };
#define pc101_keyboard_struct_nb (sizeof (pc101_keyboard_struct) / sizeof (pc101_keyboard_struct[0]))

/* --------------------------------------------------------------------
    It is assumed that the modifier keys use the standard mapping :
	man keytables(5) :
	    (By default Shift, AltGr, Control
	    and Alt are bound to the keys that bear a similar  label;
	    AltGr may denote the right Alt key.)
	    Note that you should be very careful when binding the mod-
	    ifier keys, otherwise you can end up with an unusable key-
	    board  mapping.
*/

static Flag get_map_flg = NO;   /* package init done flag */

/* Loock only in plain, Shift, Control and Alt map */
/*   Ref : man page for keytables(5) for the value of the modifier key */

static int keyboard_map [] = {
	K_NORMTAB,      /* plain key map : (0) */
	K_SHIFTTAB,     /* Shift modified map : (0x01) */
	0x04,           /* Control modified map (0x04) */
	-1              /* Alt modified map (0x08) */
	};
#define nb_kbmap (sizeof (keyboard_map) / sizeof (keyboard_map[0]))
#define shifted 1   /* map index of shift modifier */

static char *key_shift_msg [nb_kbmap] = { "", "Shift ", "Ctrl ", "Alt " };


typedef struct _Key_Assign {
    short lkcode;           /* linux keycode (as displayed by "showkey -k" XLATE mode) */
    short Xkcode;           /* X11 keycode (use xev or showkey progs to display value) */
    char *klabel;           /* label for listing and display */
    int kcode;              /* keycode linux console or X11 */
    int ktfunc [nb_kbmap];  /* key function (retuned by linux key mapping) */
    } Key_Assign;

/* PC-101 style keyboard assignement description array */
/* --------------------------------------------------- */

static Key_Assign pc_keyboard_desc [] = {
			/* lkcode  Xkcode label */
    /* === Function Keys === */                         /* keycode for linux X server */
    /*  F1          */      {  59, XK_F1 , "F1"  },             /*   67 */
    /*  F2          */      {  60, XK_F2 , "F2"  },             /*   68 */
    /*  F3          */      {  61, XK_F3 , "F3"  },             /*   69 */
    /*  F4          */      {  62, XK_F4 , "F4"  },             /*   70 */
    /*  F5          */      {  63, XK_F5 , "F5"  },             /*   71 */
    /*  F6          */      {  64, XK_F6 , "F6"  },             /*   72 */
    /*  F7          */      {  65, XK_F7 , "F7"  },             /*   73 */
    /*  F8          */      {  66, XK_F8 , "F8"  },             /*   74 */
    /*  F9          */      {  67, XK_F9 , "F9"  },             /*   75 */
    /*  F10         */      {  68, XK_F10, "F10" },             /*   76 */
    /*  F11         */      {  87, XK_F11, "F11" },             /*   95 */
    /*  F12         */      {  88, XK_F12, "F12" },             /*   96 */

    /* === Cursor Key === */
    /*  UP ARROW    */      { 103, XK_Up   , "UP"    },         /*   98 */
    /*  LEFT ARROW  */      { 105, XK_Left , "LEFT"  },         /*  100 */
    /*  DOWN ARROW  */      { 108, XK_Down , "DOWN"  },         /*  104 */
    /*  RIGHT ARROW */      { 106, XK_Right, "RIGHT" },         /*  102 */

    /* === Extended Key Pad === */
    /*  INSERT      */      { 110, XK_Insert , "INSERT"  },     /*  106 */
    /*  HOME        */      { 102, XK_Home   , "HOME"    },     /*   97 */
    /*  PAGE UP     */      { 104, XK_Page_Up, "PAGE-UP" },     /*   99 */

    /*  DELETE      */      { 111, XK_Delete   , "DELETE"   },  /*  107 */
    /*  END         */      { 107, XK_End      , "END"      },  /*  103 */
    /*  PAGE DOWN   */      { 109, XK_Page_Down, "PAGE-DOWN"},  /*  105 */

    /* === Numeric Key Pad === */
    /*  KP 0        */      {  82, XK_KP_0 , "KP-0" },          /*   90 */
    /*  KP 1        */      {  79, XK_KP_1 , "KP-1" },          /*   87 */
    /*  KP 2        */      {  80, XK_KP_2 , "KP-2" },          /*   88 */
    /*  KP 3        */      {  81, XK_KP_3 , "KP-3" },          /*   89 */
    /*  KP 4        */      {  75, XK_KP_4 , "KP-4" },          /*   83 */
    /*  KP 5        */      {  76, XK_KP_5 , "KP-5" },          /*   84 */
    /*  KP 6        */      {  77, XK_KP_6 , "KP-6" },          /*   85 */
    /*  KP 7        */      {  71, XK_KP_7 , "KP-7" },          /*   79 */
    /*  KP 8        */      {  72, XK_KP_8 , "KP-8" },          /*   80 */
    /*  KP 9        */      {  73, XK_KP_9 , "KP-9" },          /*   81 */

    /*  KP +        */      {  78, XK_KP_Add    ,  "KP-PLUS" }, /*   86 */
    /*  KP ENTER    */      {  96, XK_KP_Enter  , "KP-ENTER" }, /*  108 */
    /*  KP .        */      {  83, XK_KP_Separator, "KP-DOT" }, /*   91 */

    /*  KP NUM LOCK */      {  69, XK_Num_Lock   , "KP-NLOCK" }, /*   77 */
    /*  KP /        */      {  98, XK_KP_Divide  , "KP-DIV"   }, /*  112 */
    /*  KP *        */      {  55, XK_KP_Multiply, "KP-MUL"   }, /*   63 */
    /*  KP -        */      {  74, XK_KP_Subtract, "KP-MINUS" }, /*   82 */

    /* === Miscellaneous keys */
    /*  BACK SPACE  */      {  14, XK_BackSpace, "BKSP" },      /*    22 */
    /*  TAB         */      {  15, XK_Tab      , "TAB"  }       /*    23 */

    };

#define pc_keyboard_desc_nb (sizeof (pc_keyboard_desc) / sizeof (pc_keyboard_desc[0]))
static int nb_key = pc_keyboard_desc_nb;

#if 0
/* -------------------------------------------------------------
    Usefull reading : "The Linux Keyboard and Console HOWTO"
	in Linux doc. (file : /usr/doc/HOWTO/Keyboard-and-Console-HOWTO)
	man pages : console_codes(5), keytables(5)

    ----- Extract from this "Keyboard and Console HOWTO" :

	3.  Keyboard generalities

	You press a key, and the keyboard controller sends scancodes to the
	kernel keyboard driver. Some keyboards can be programmed, but usually
	the scancodes corresponding to your keys are fixed.  The kernel
	keyboard driver just transmits whatever it receives to the application
	program when it is in scancode mode, like when X is running.
	Otherwise, it parses the stream of scancodes into keycodes,
	corresponding to key press or key release events.  (A single key press
	can generate up to 6 scancodes.)  These keycodes are transmitted to
	the application program when it is in keycode mode (as used, for
	example, by showkey).  Otherwise, these keycodes are looked up in the
	keymap, and the character or string found there is transmitted to the
	application, or the action described there is performed.  (For
	example, if one presses and releases the a key, then the keyboard
	produces scancodes 0x1e and 0x9e, this is converted to keycodes 30 and
	158, and then transmitted as 0141, the ASCII or latin-1 code for `a';
	if one presses and releases Delete, then the keyboard produces
	scancodes 0xe0 0x53 0xe0 0xd3, these are converted to keycodes 111 and
	239, and then transmitted as the 4-symbol sequence ESC [ 3 ~, all
	assuming a US keyboard and a default keymap. An example of a key
	combination to which an action is assigned is Ctrl-Alt-Del.)

	The translation between unusual scancodes and keycodes can be set
	using the utility setkeycodes - only very few people will need it.
	The translation between keycodes and characters or strings or actions,
	that is, the keymap, is set using the utilities loadkeys and
	setmetamode.  For details, see getkeycodes(8), setkeycodes(8),
	dumpkeys(1), loadkeys(1), setmetamode(1). The format of the files
	output by dumpkeys and read by loadkeys is described in keytables(5).

	Where it says `transmitted to the application' in the above
	description, this really means `transmitted to the terminal driver'.
	That is, further processing is just like that of text that comes in
	over a serial line.  The details of this processing are set by the
	program stty.

    -----

    Linux Keyboard Driver analysis :
    --------------------------------

    WARNING : scan code translation (modified by setkeycodes)
	      and keymap (modified by loadkeys or setmetamode)
	      can change the behaviour of the keyboard.

	In the following analysis it is assumed that the standard
	translation and mapping is not modified.
	The best way to add functionality to the keyboard is to
	define a mapping into a function (or a user string) for
	un-mapped keycode. Changing scan code translation is
	not recommanded.

    Linux keyboard driver (/usr/src/linux/drivers/char/keyboard.c)
    build in string generation for keypad and cursor keys.

	The value (and order) of Kxxx functions is defined by the
	include file : /usr/include/linux/keyboard.h

	To know the binding between a function defined in the dumpkeys
	listing and a actual result by the keyboard handler, you must
	found the binding between the loadkeys / dumpkeys function name
	and Kxxx symbol of keyboard.h include file.
	This can be done by :
	    1 - loock into the output of "dumpkeys --long-info" to
		found the function value of a given function name
		using synonyms if needed.
	    2 - found in /usr/include/linux/keyboard.h to found
		the Kxxx symbol for the function value.

    The string generated for the numerical key pad and the cursor keys
    is build in the keyboard driver and cannot be redefined by "loadkeys"
    It is only possible to assigne the key to another function.

    The build in string depend upon the keyboard mode :
	Cursor mode (normal or alternate)
    and Key Pad mode (numeric or application).

    General terminal reset : "\033c" (numeric mode and normal cursor mode)
    Alternate cursor mode is set by :             "\033[?1h" (DECCKM)
	reset to the default (normal cursor) by : "\033[?1l"
    Appliction mode is set by :           "\033="  (DECPAM)
	reset to default (Numerical) by : "\033>"  (DECPNM)
	ref : man console_codes(4)

    In alternate cursor mode the string generated are :
	<Esc>Ox in stead of <Esc>[x (where x is a char).


    ---- analysis of linux keyboard driver (/usr/src/linux/drivers/char/keyboard.c)
    The string generated by a given Numerical Key Pad key is build
    by the routine "do_pad" according to the arrays :
	pad_chars (Num Lock mode)
	app_map (application mode)
    the index in this arrays is the low byte of the K_Pxxx key function
    code (defined in /usr/include/linux/keyboard.h)

	static const char *pad_chars = "0123456789+-*/\015,.?";
	static const char *app_map   = "pqrstuvwxylSRQMnn?";

    The Numerical Key Pad (K_P0 .... K_PPLUSMINUS) can produce :
	numbers (corresponding to the Major" key label) according to "pad_chars"
	application strings, according to "app_map"
	remapped into cursor and service (corresponding to alternate key label)

    Build in logic with depend upon mode (applic or cursor) and Num Lock state is :
	if (applic)  if (no Shift) applic string
		     else if (Num Lock) number (= major key label)
			  else remaped into cursor (with application string)
						   (= alternate key label)
	else (= cursor mode)
	    if (Num Lock) number (= major key label)
	    else remaped into cursor (with cursor string)
				     (= alternate key label)

    The remapping is defined by :
	K_P0     -> K_INSERT
	K_P1     -> K_SELECT
	K_P2     -> K_DOWN
	K_P3     -> K_PGDN
	K_P4     -> K_LEFT
	K_P5     -> <Esc>[G (numerical mode), <Esc>OG (application)
	K_P6     -> K_RIGHT
	K_P7     -> K_FIND
	K_P8     -> K_UP
	K_P9     -> K_PGUP
	K_PCOMMA -> K_REMOVE
	K_PDOT   -> K_REMOVE

	K_PPLUS, K_PMINUS, K_PSTAR, K_PSLASH, K_PENTER, K_PPLUSMINUS
	are mapped into the numerical value (according to pad_chars array)

    Special case of the Num Lock key (upper left key of the Numerical Key Pad)
	Function value : K_NUM
	By default is is handled by the "num" routine :
	    which builds <esc>OP in application mode
	    or which flips the Num Lock state (and keyboard led), "bare_num" routine.
	If the key is bind to K_BARENUMLOCK it will always
	    flips the Num Lock state (and keyboard led), "bare_num" routine.


    The case of the Cursor Keys is handle by "do_cur" routine.
    The string is buils according to the "cur_chars" array
	and cursor / applic mode.
	static const char *cur_chars = "BDCA";
	(order : K_DOWN, K_LEFT, K_RIGHT, K_UP)

  -------------------------------------------------------------- */
#endif

/* structure to describe the escape sequences */

struct KTdesc {
    int ktfunc;             /* key function (kt or keysym, retuned by linux key mapping) */
    char *tcap;             /* termcap / terminfo ident or label for mapping */
    char *strg;             /* string currently generated by this key function */
    char *norstrg;          /* string for normal (or numeric) mode, NULL : not sensitive */
    char *altstrg;          /* string for alternate (or applic) mode, NULL : not sensitive */
    };

struct _all_ktdesc {
    int kt_type;            /* KT type for linux / console, not used for xterm */
    char *type_name;        /* label for the group */
    struct KTdesc **ktdesc_array;   /* pointer to the descriptor array */
    int *ktdesc_nb;         /* numberof element in the array */
    };

/* Terminal related global variables */
/* --------------------------------- */

static char *terminal_name = NULL;  /* pointer to TERM variable */
static Flag Xterminal_flg = NO;     /* X terminal family */
static Flag Xterm_flg = YES;        /* special xterm processing : to be set by option para */

static Flag keypad_appl_mode = NO; /* application (Yes) or numeric (NO) */
static Flag cursor_alt_mode  = NO; /* alternate (Yes) or normal (NO) */

static Flag assess_cursor_flg, assess_keypad_flg;   /* terminal mode to be assessed */
static int kt_void;             /* not use entry in kt description array */

static struct _all_ktdesc *inuse_ktdesc = NULL; /* all_ktdesc or all_Xktdesc */
static int inuse_ktdesc_nb;     /* nb of elements in inuse_ktdesc [] */

static char *init_msg = NULL;   /* string used to initialize the terminal */

/* -------------------------------------------------------------- */

/* Terminal control setting escape sequence (for initialisation) */
    /* only a copy of this structure must be used for re-ordering */
static struct Init_Sequence {
    int type;
    int ini;    /* index of the string or -1 : used dynamicaly */
    char *ref;
    char *info;
    } init_seq [] = {
	{ 0, -1, "\033c"   , "Terminal and Keyboard reset" },
	{ 1, -1, "\033>"   , "Numeric Key Pad mode       " },   /* DECPNM */
	{ 2, -1, "\033="   , "Application Key Pad mode   " },   /* DECPAM */
	{ 3, -1, "\033[?1l", "Normal Cursor mode         " },   /* DECCKM */
	{ 4, -1, "\033[?1h", "Alternate Cursor mode      " }    /* DECCKM */
    };
#define init_seq_nb (sizeof (init_seq) / sizeof (init_seq[0]))
#define key_pad_num 1
#define cursor_norm 3

/* Description of escape squences generated by keyboard */
/* ---------------------------------------------------- */

/* ref to terminfo man page for the termcap / terminfo ident string */

/* linux (or console) terminal type */
/* -------------------------------- */

/* Cursor Key Pad */
/* build in string, sensitive to cursor / applic mode */
/*  warning : in terminfo the cursor value seem to be defined */

/* cursor normal mode */
static struct KTdesc kt_desc_cur_norm [] = {
	{ K_DOWN  , "kcud1", NULL, "\033[B", "\033OB" },
	{ K_LEFT  , "kcub1", NULL, "\033[D", "\033OD" },
	{ K_RIGHT , "kcuf1", NULL, "\033[C", "\033OC" },
	{ K_UP    , "kcuu1", NULL, "\033[A", "\033OA" },
    };
#define KT_CUR_nb (sizeof (kt_desc_cur_norm) / sizeof (struct KTdesc))

/* cursor alternated mode */
static struct KTdesc kt_desc_cur_alt [] = {
	{ K_DOWN  , "kcud1", "\033OB" },
	{ K_LEFT  , "kcub1", "\033OD" },
	{ K_RIGHT , "kcuf1", "\033OC" },
	{ K_UP    , "kcuu1", "\033OA" },
    };

/* Numeric Key Pad */
/* build in string, sensitive to cursor / applic mode */
/*  warning : in terminfo the numeric key pad seems to be not defined */

static struct KTdesc kt_desc_pad [] = {
	{ K_P0      ,NULL,  "\033Op" },
	{ K_P1      ,NULL,  "\033Oq" },
	{ K_P2      ,NULL,  "\033Or" },
	{ K_P3      ,NULL,  "\033Os" },
	{ K_P4      ,NULL,  "\033Ot" },
	{ K_P5      ,NULL,  "\033Ou" },
	{ K_P6      ,NULL,  "\033Ov" },
	{ K_P7      ,NULL,  "\033Ow" },
	{ K_P8      ,NULL,  "\033Ox" },
	{ K_P9      ,NULL,  "\033Oy" },
	{ K_PPLUS   ,NULL,  "\033Ol" }, /* key-pad plus */
	{ K_PMINUS  ,NULL,  "\033OS" }, /* key-pad minus */
	{ K_PSTAR   ,NULL,  "\033OR" }, /* key-pad asterisk (star) */
	{ K_PSLASH  ,NULL,  "\033OQ" }, /* key-pad slash */
	{ K_PENTER  ,NULL,  "\033OM" }, /* key-pad enter */
	{ K_PCOMMA  ,NULL,  "\033On" }, /* key-pad comma: kludge... */
	{ K_PDOT    ,NULL,  "\033On" }, /* key-pad dot (period): kludge... */
	{ K_PPLUSMINUS,NULL,"\033O?" }, /* key-pad plus/minus */
    };
#define KT_PAD_nb (sizeof (kt_desc_pad) / sizeof (struct KTdesc))

/* translation table for Key Pad */

/* special case for K_P5 */
static char kp5_trans_num_strg[]  = "\033[G";   /* numeric mode */
static char kp5_trans_appl_strg[] = "\033OG";   /* application mode */
static char kp5_trans_appl1_strg[]= "\033Ou";   /* application mode */

static short kp_translate [] = {
	K_P0     , K_INSERT ,
	K_P1     , K_SELECT ,
	K_P2     , K_DOWN   ,
	K_P3     , K_PGDN   ,
	K_P4     , K_LEFT   ,
	K_P6     , K_RIGHT  ,
	K_P7     , K_FIND   ,
	K_P8     , K_UP     ,
	K_P9     , K_PGUP   ,
	K_PPLUS  , '+'      ,
	K_PMINUS , '-'      ,
	K_PSTAR  , '*'      ,
	K_PSLASH , '/'      ,
	K_PENTER , '\r'     ,
	K_PCOMMA , K_REMOVE ,
	K_PDOT   , K_REMOVE ,
	K_PPLUSMINUS, '?'
    };
#define kp_translate_nb (sizeof (kp_translate) / sizeof (kp_translate[0]))

/* Special case keys */

static struct KTdesc kt_desc_spec [] = {
	/* special case keys */
	{ K_NUM     ,NULL,  "\033OP" }, /* key-pad Num Lock */
    };
#define KT_SPEC_nb (sizeof (kt_desc_spec) / sizeof (struct KTdesc))

	/* end of linux build in string generation zone */

/* KT_FN descriptor */

static struct KTdesc kt_desc_fn [] = {
	/* key pad service keys */
	{ K_FIND   , "khome", "\033[1~" }, /* home key label */
	{ K_INSERT , "kich1", "\033[2~" },
	{ K_REMOVE , "kdch1", "\033[3~" },
	{ K_SELECT , "kend" , "\033[4~" }, /* end key label */
	{ K_PGUP   , "kpp"  , "\033[5~" }, /* PGUP is a synonym for PRIOR */
	{ K_PGDN   , "knp"  , "\033[6~" }, /* PGDN is a synonym for NEXT */
	{ K_MACRO  ,  NULL  , "\033[M~" },
	{ K_HELP   ,  NULL  ,  NULL     },
	{ K_DO     ,  NULL  ,  NULL     },
	{ K_PAUSE  ,  NULL  , "\033[P~" },

	/* function keys */
	{ K_F1,  "kf1",  "\033[A" },
	{ K_F2,  "kf2",  "\033[B" },
	{ K_F3,  "kf3",  "\033[C" },
	{ K_F4,  "kf4",  "\033[D" },
	{ K_F5,  "kf5",  "\033[E" },
	{ K_F6,  "kf6",  "\033[17~" },
	{ K_F7,  "kf7",  "\033[18~" },
	{ K_F8,  "kf8",  "\033[19~" },
	{ K_F9,  "kf9",  "\033[20~" },
	{ K_F10, "kf10", "\033[21~" },

	{ K_F11, "kf11", "\033[23~" },
	{ K_F12, "kf12", "\033[24~" },
	{ K_F13, "kf13", "\033[25~" },
	{ K_F14, "kf14", "\033[26~" },
	{ K_F15, "kf15", "\033[28~" },
	{ K_F16, "kf16", "\033[29~" },
	{ K_F17, "kf17", "\033[31~" },
	{ K_F18, "kf18", "\033[32~" },
	{ K_F19, "kf19", "\033[33~" },
	{ K_F20, "kf20", "\033[34~" },

	{ K_F21, "kf21",  NULL },
	{ K_F22, "kf22",  NULL },
	{ K_F23, "kf23",  NULL },
	{ K_F24, "kf24",  NULL },
	{ K_F25, "kf25",  NULL },
	{ K_F26, "kf26",  NULL },
	{ K_F27, "kf27",  NULL },
	{ K_F28, "kf28",  NULL },
	{ K_F29, "kf29",  NULL },
	{ K_F30, "kf30",  NULL },

	/* empty slot for 10 additional function keys */
	{ 0    ,  NULL ,  NULL },
	{ 0    ,  NULL ,  NULL },
	{ 0    ,  NULL ,  NULL },
	{ 0    ,  NULL ,  NULL },
	{ 0    ,  NULL ,  NULL },
	{ 0    ,  NULL ,  NULL },
	{ 0    ,  NULL ,  NULL },
	{ 0    ,  NULL ,  NULL },
	{ 0    ,  NULL ,  NULL },
	{ 0    ,  NULL ,  NULL },
    };
#define KT_FN_nb (sizeof (kt_desc_fn) / sizeof (struct KTdesc))


/* KT_LATIN descriptor (latin characters) */

static struct KTdesc kt_desc_latin [] = {
	{ '\t'  ,  NULL ,  "\t"   },  /* horizontal tab */
	{ '\177',  NULL ,  "\177" },  /* back space */
    };
#define KT_LATIN_nb (sizeof (kt_desc_latin) / sizeof (struct KTdesc))


static struct KTdesc *ktcur_desc_pt = kt_desc_cur_norm;
static int ktcur_desc_nb = KT_CUR_nb;

static struct KTdesc *ktpad_desc_pt = kt_desc_pad;
static int ktpad_desc_nb = KT_PAD_nb;

static struct KTdesc *ktspec_desc_pt = kt_desc_spec;
static int ktspec_desc_nb = KT_SPEC_nb;

static struct KTdesc *ktfn_desc_pt = kt_desc_fn;
static int ktfn_desc_nb = KT_FN_nb;

static struct KTdesc *ktlatin_desc_pt = kt_desc_latin;
static int ktlatin_desc_nb = KT_LATIN_nb;

static struct _all_ktdesc all_ktdesc [] = {
    { KT_CUR,  "Cursor key",          &ktcur_desc_pt,   &ktcur_desc_nb  },
    { KT_PAD,  "Num key pad",         &ktpad_desc_pt,   &ktpad_desc_nb  },
    { KT_SPEC, "Special key",         &ktspec_desc_pt,  &ktspec_desc_nb },
    { KT_FN,   "Function key",        &ktfn_desc_pt,    &ktfn_desc_nb   },
    { KT_LATIN,"Latin character key", &ktlatin_desc_pt, &ktlatin_desc_nb}
};
#define all_ktdesc_nb (sizeof (all_ktdesc) / sizeof (all_ktdesc[0]))

/* ------------------------------------------------------------------------ */

/* xterm terminal family */
/* --------------------- */

/* It is assumed thet the VT100 emulation is in use, the VT52 mode is not
	supported and the 3270 is not handle.

    In the old version of xterm (or in nxterm ...) the string generated
	for XK_Home, XK_KP_Home, XK_End, XK_KP_End, XK_Begin, XK_KP_Begin
	are not properly defined. In this case it is necessary to
	override the translation table for XTerm  with :


    Ref :
	"/usr/include/X11/keysymdef.h"  for symbol definition.
	XFree xterm source : "input.c" file for the way use by xtrem
	    terminal emulator to generate the escape sequence.
*/

/*
------------------------------------------------------------------------------
It is assumed that this translation and keymapping is done on Linux X server
#-----------------------------------------------------------------------------
# Linux specific case :
# ---------------------
# The following translation override is assumed to be done for nxterm
#   application in "/usr/X11R6/lib/X11/app-defaults/NXTerm"

# ! The Home and End are not defined in XTerm by default
# !   Overriding the translation for the key in the numerical key pad
# !       KP_Home, KP_End, KP_Delete, KP_Begin
# !       (the right side key pad) disturbe the normal behaviour of these
# !       keys as defined by the shift modifier.
# !   The new translation must be applied only to shift and lock_shift
# !       case (see the ':' in the translation defintion).
#
# *VT100.Translations: #override \n\
#         <Key>Home:             string(0x1b) string("[H")\n\
#        :<Key>KP_Home:          string(0x1b) string("[H")\n\
#        :<Key>KP_End:           string(0x1b) string("[F")\n\
#         <Key>End:              string(0x1b) string("[F")\n\
#         <Key>Begin:            string(0x1b) string("[E")\n\
#        :<Key>KP_Begin:         string(0x1b) string("[E")\n\
#         <Key>Delete:           string(0x1b) string("[3~")

# Expected keyboard X mapping :
#
# ! /usr/local/X11R6/lib/X11/xinit/Xmodmap.sf :
# !   105 keys US keymaping. by Fabien.Perriollat@cern.ch Dec 98
# !   This is an good compromise setting fo the rand editor.
# !   A link to this file is expected to be defined in
# !       "/usr/X11R6/lib/X11/xinit/.Xmodmap"
#
# ! keycode  22 = BackSpace
# ! keycode  23 = Tab KP_Tab
#
# keycode  67 = F1  F11
# keycode  68 = F2  F12
# keycode  69 = F3  F13
# keycode  70 = F4  F14
# keycode  71 = F5  F15
# keycode  72 = F6  F16
# keycode  73 = F7  F17
# keycode  74 = F8  F18
# keycode  75 = F9  F19
# keycode  76 = F10 F20
#
# ! keycode  77 = Num_Lock F17 Num_Lock
# ! keycode  78 = Multi_key
#
# keycode  79 = KP_7 KP_Home KP_7 KP_Home
# keycode  80 = KP_8 Up KP_8 Up
# keycode  81 = KP_9 Prior KP_9 Prior
# keycode  82 = KP_Subtract
# keycode  83 = KP_4 Left KP_4 Left
# keycode  84 = KP_5 Begin KP_5 Begin
# keycode  85 = KP_6 Right KP_6 Right
# keycode  86 = KP_Separator KP_Add KP_Separator KP_Add
# keycode  87 = KP_1 KP_End KP_1 KP_End
# keycode  88 = KP_2 Down KP_2 Down
# keycode  89 = KP_3 Next KP_3 Next
# keycode  90 = KP_0 Insert KP_0 Insert
# keycode  91 = KP_Decimal KP_Delete KP_Decimal KP_Delete
#
# ! keycode  92 = 0x1007ff00
#
# ! keycode 107 = Delete

#-----------------------------------------------------------------------------
*/


/* Cursor control & motion */
/*      sensitive to cursor mode (DECCKM) : normal or alternate */
/*      The XK_KP_Home ... XK_KP_Begin are remapped on XK_Home ... XK_Begin */

static struct KTdesc Xkt_desc_cursor [] = {
					    /* normal      alternate */
    { XK_Insert   , "Insert"   , "\033[2~",   NULL     ,   NULL     },
    { XK_Delete   , "Delete"   , "\033[3~",   NULL     ,   NULL     },

    { XK_Home     , "Home"     ,  NULL     , "\033[H"  ,  "\033OH"  },
    { XK_Left     , "Left"     ,  NULL     , "\033[D"  ,  "\033OD"  },   /* Move left, left arrow */
    { XK_Up       , "Up"       ,  NULL     , "\033[A"  ,  "\033OA"  },   /* Move up, up arrow */
    { XK_Right    , "Right"    ,  NULL     , "\033[C"  ,  "\033OC"  },   /* Move right, right arrow */
    { XK_Down     , "Down"     ,  NULL     , "\033[B"  ,  "\033OB"  },   /* Move down, down arrow */
    { XK_Prior    , "Prior"    , "\033[5~" ,  NULL     ,   NULL     },   /* Prior, previous */
    { XK_Page_Up  , "Page_Up"  , "\033[5~" ,  NULL     ,   NULL     },
    { XK_Next     , "Next"     , "\033[6~" ,  NULL     ,   NULL     },   /* Next */
    { XK_Page_Down, "Page_Down", "\033[6~" ,  NULL     ,   NULL     },
    { XK_End      , "End"      ,  NULL     , "\033[F"  ,  "\033OF"  },   /* EOL */
    { XK_Begin    , "Begin"    ,  NULL     , "\033[E"  ,  "\033OE"  },   /* BOL */

	/* XK_KP_Home ... XK_KP_Begin are mapped into XK_Home ... XK_Begin by xterm routines */
						 /* normal      alternate */
    { XK_KP_Home     , "KP_Home"     ,  NULL     , "\033[H"  ,  "\033OH"  },
    { XK_KP_Left     , "KP_Left"     ,  NULL     , "\033[D"  ,  "\033OD"  },   /* Move left, left arrow */
    { XK_KP_Up       , "KP_Up"       ,  NULL     , "\033[A"  ,  "\033OA"  },   /* Move up, up arrow */
    { XK_KP_Right    , "KP_Right"    ,  NULL     , "\033[C"  ,  "\033OC"  },   /* Move right, right arrow */
    { XK_KP_Down     , "KP_Down"     ,  NULL     , "\033[B"  ,  "\033OB"  },   /* Move down, down arrow */
    { XK_KP_Prior    , "KP_Prior"    , "\033[5~" ,  NULL     ,   NULL     },   /* Prior, previous */
    { XK_KP_Page_Up  , "KP_Page_Up"  , "\033[5~" ,  NULL     ,   NULL     },
    { XK_KP_Next     , "KP_Next"     , "\033[6~" ,  NULL     ,   NULL     },   /* Next */
    { XK_KP_Page_Down, "KP_Page_Down", "\033[6~" ,  NULL     ,   NULL     },
    { XK_KP_End      , "KP_End"      ,  NULL     , "\033[F"  ,  "\033OF"  },   /* EOL */
    { XK_KP_Begin    , "KP_Begin"    ,  NULL     , "\033[E"  ,  "\033OE"  },   /* BOL */
};
#define XKT_CUR_nb (sizeof (Xkt_desc_cursor) / sizeof (struct KTdesc))


/* Keypad Functions, keypad numbers cleverly chosen to map to ascii */
/*      sensitive to key pad mode (DECKPAM) : numeric or application */

static struct KTdesc Xkt_desc_pad [] = {
    { XK_KP_F1,     "KP_F1" ,    "\033OP" , NULL , NULL },  /* PF1, KP_A, ... */
    { XK_KP_F2,     "KP_F2" ,    "\033OQ" , NULL , NULL },
    { XK_KP_F3,     "KP_F3" ,    "\033OR" , NULL , NULL },
    { XK_KP_F4,     "KP_F4" ,    "\033OS" , NULL , NULL },

    { XK_KP_Insert, "KP_Insert", "\033[2~", NULL , NULL },
    { XK_KP_Delete, "KP_Delete", "\033[3~", NULL , NULL },

					   /* num    applic */
    { XK_KP_0        , "KP_0"        , NULL , "1" , "\033Op" },
    { XK_KP_1        , "KP_1"        , NULL , "2" , "\033Oq" },
    { XK_KP_2        , "KP_2"        , NULL , "3" , "\033Or" },
    { XK_KP_3        , "KP_3"        , NULL , "4" , "\033Os" },
    { XK_KP_4        , "KP_4"        , NULL , "5" , "\033Ot" },
    { XK_KP_5        , "KP_5"        , NULL , "6" , "\033Ou" },
    { XK_KP_6        , "KP_6"        , NULL , "7" , "\033Ov" },
    { XK_KP_7        , "KP_7"        , NULL , "8" , "\033Ow" },
    { XK_KP_8        , "KP_8"        , NULL , "9" , "\033Ox" },
    { XK_KP_9        , "KP_9"        , NULL , "0" , "\033Oy" },

    { XK_KP_Tab      , "KP_Tab"      , NULL , "\t", "\033OI" },
    { XK_KP_Enter    , "KP_Enter"    , NULL , "\r", "\033OM" },  /* enter */
    { XK_KP_Equal    , "KP_Equal"    , NULL , "=" , "\033OX" },  /* equals */
    { XK_KP_Multiply , "KP_Multiply" , NULL , "*" , "\033Oj" },
    { XK_KP_Add      , "KP_Add"      , NULL , "+" , "\033Ok" },
    { XK_KP_Separator, "KP_Separator", NULL , "," , "\033Ol" },  /* separator, often comma */
    { XK_KP_Subtract , "KP_Subtract" , NULL , "-" , "\033Om" },
    { XK_KP_Decimal  , "KP_Decimal"  , NULL , "." , "\033On" },
    { XK_KP_Divide   , "KP_Divide"   , NULL , "/" , "\033Oo" },
};
#define XKT_PAD_nb (sizeof (Xkt_desc_pad) / sizeof (struct KTdesc))


/* Auxilliary Functions */

static struct KTdesc Xkt_desc_fn [] = {
    /* function keys */
    { XK_F1,  "F1" , "\033[11~" },
    { XK_F2,  "F2" , "\033[12~" },
    { XK_F3,  "F3" , "\033[13~" },
    { XK_F4,  "F4" , "\033[14~" },
    { XK_F5,  "F5" , "\033[15~" },
    { XK_F6,  "F6" , "\033[17~" },
    { XK_F7,  "F7" , "\033[18~" },
    { XK_F8,  "F8" , "\033[19~" },
    { XK_F9,  "F9" , "\033[20~" },
    { XK_F10, "F10", "\033[21~" },

    { XK_F11, "F11", "\033[23~" },
    { XK_F12, "F12", "\033[24~" },
    { XK_F13, "F13", "\033[25~" },
    { XK_F14, "F14", "\033[26~" },
    { XK_F15, "F15", "\033[28~" },
    { XK_F16, "F16", "\033[29~" },
    { XK_F17, "F17", "\033[31~" },
    { XK_F18, "F18", "\033[32~" },
    { XK_F19, "F19", "\033[33~" },
    { XK_F20, "F20", "\033[34~" },
};
#define XKT_FN_nb (sizeof (Xkt_desc_fn) / sizeof (struct KTdesc))


/* KT_LATIN descriptor (latin characters) */

static struct KTdesc Xkt_desc_latin [] = {
    { XK_Tab,       "Tab",        "\t"  ,  NULL, NULL },
    { XK_BackSpace, "BackSpace",  "\177",  NULL, NULL },
};
#define XKT_LATIN_nb (sizeof (Xkt_desc_latin) / sizeof (struct KTdesc))


static struct KTdesc *Xktcur_desc_pt = Xkt_desc_cursor;
static int Xktcur_desc_nb = XKT_CUR_nb;

static struct KTdesc *Xktpad_desc_pt = Xkt_desc_pad;
static int Xktpad_desc_nb = XKT_PAD_nb;

static struct KTdesc *Xktfn_desc_pt = Xkt_desc_fn;
static int Xktfn_desc_nb = XKT_FN_nb;

static struct KTdesc *Xktlatin_desc_pt = Xkt_desc_latin;
static int Xktlatin_desc_nb = XKT_LATIN_nb;

static struct _all_ktdesc all_Xktdesc [] = {
    { 0, "Cursor key",          &Xktcur_desc_pt,   &Xktcur_desc_nb },
    { 0, "Key Pad",             &Xktpad_desc_pt,   &Xktpad_desc_nb },
    { 0, "Function key",        &Xktfn_desc_pt,    &Xktfn_desc_nb  },
    { 0, "Latin character key", &Xktlatin_desc_pt, &Xktlatin_desc_nb }
};
#define all_Xktdesc_nb (sizeof (all_Xktdesc) / sizeof (all_Xktdesc[0]))

static char *get_kt_strg (int, char **);

/* array to be used for quick search */
static struct KTdesc *(*Xkt_desc_pt) [] = NULL;
static int Xkt_desc_pt_nb;

/* special case of badly handle key in old xterm version and in nxterm */
/*  The handling for these key is correctly done in new version of xterm,
	we assume that a "#VT100.Translation: #override" is done
	in app_defaults ("/usr/X11R6/lib/X11/app_defaults/NXTerm")
	to provide the escape sequence of the new xterm. But this
	cannot provide the adequate handling of the cursor mode.
	A special case in "string_to_key" routine is done to go
	arround this problem.
*/

static struct _nxterm_special {
    int ktfunc;
    struct KTdesc *Xkt_desc_pt;
    } nxterm_special [] = {
	{ XK_Home    , NULL },
	{ XK_End     , NULL },
	{ XK_Begin   , NULL },
	{ XK_KP_Home , NULL },
	{ XK_KP_End  , NULL },
	{ XK_KP_Begin, NULL }
    };
#define nxterm_special_nb (sizeof (nxterm_special) / sizeof (nxterm_special[0]))

/* In Linux xterm, which is compiled with the OPT_VT52_MODE keys F1-F4
    are interpreted as PF1-PF4 (see input.c in Input routine.
    A back translation as to be done for a correct handling of F1-F4
*/

static struct _xterm_F1F4_keys {
    char *PFstring;
    char  *Fstring;
    } xterm_F1F4_keys [] = {
	{ "\033OP", "\033[11~" },
	{ "\033OQ", "\033[12~" },
	{ "\033OR", "\033[13~" },
	{ "\033OS", "\033[14~" }
    };
#define xterm_F1F4_keys_nb (sizeof (xterm_F1F4_keys) / sizeof (xterm_F1F4_keys[0]))

/* ------------------------------------------------------------------------ */

/* Routines specific for the linux / console terminal */
/* -------------------------------------------------- */

static void kbf_linux_string (int idx, char *strg)
{
    char *cp;

    cp = (char *) malloc ( strlen(strg)+1);
    if ( cp ) {
	strcpy (cp,strg);
	ktfn_desc_pt[idx].strg = cp;
    }
}

/* build_linux_ktfn_string : build the descriptors of string function (KT_FN type) */
static int build_linux_ktfn_string (int fn, int *nbpt, int *szpt,
			       struct KTdesc *ktfn_pt, char *ktfnstrg_pt)
{
    int i, cc;
    int nb, sz;
    char *sp;
    struct kbsentry iokbstr;

    nb = sz = 0;
    for ( i = 0 ; i <= 255 ; i++ ) {
	iokbstr.kb_func = i;
	iokbstr.kb_string[0] = '\0';
	cc = ioctl (fn, KDGKBSENT, &iokbstr); /* read the string */
	if ( cc < 0 ) continue; /* some error */
	if ( iokbstr.kb_string[0] == '\0' ) continue; /* not assigned */
	if ( ktfn_pt && ktfnstrg_pt ) {
	    sp = &ktfnstrg_pt[sz];
	    strcpy (sp, iokbstr.kb_string);
	    ktfn_pt[nb].ktfunc = K (KT_FN, i);
	    ktfn_pt[nb].strg = sp;
	}
	nb++;
	sz += strlen (iokbstr.kb_string) +1;
    }
    if ( ktfn_pt && ktfnstrg_pt ) {
	if ( (nb != *nbpt) || (sz != *szpt) )
	    return (-1);   /* error */
    }
    else {
	*nbpt = nb;
	*szpt = sz;
    }
    return (nb);
}

/* get_linux_kbmap_string : get the defined function strings and build descriptors */
static void get_linux_kbmap_string (int fn)
{
    int i, cc;
    int nb, sz;
    struct kbsentry *kbstr;
    struct KTdesc *ktds;
    char *ktstrg_buf;

    ktfn_desc_pt = (struct KTdesc *) NULL;
    ktfn_desc_nb = 0;

    ktds = (struct KTdesc *) NULL;
    kbstr = (struct kbsentry *) NULL;
    nb = sz = 0;
    cc = build_linux_ktfn_string (fn, &nb, &sz, NULL, NULL); /* get size */
    if ( (cc <= 0) || (nb == 0) || (sz == 0) ) return;   /* error or nothing */

    ktds = (struct KTdesc *) calloc (nb, sizeof (struct KTdesc));
    if ( ktds ) {
	ktstrg_buf = (char *) calloc (sz, sizeof (char));
	if ( ktstrg_buf ) {
	    cc = build_linux_ktfn_string (fn, &nb, &sz, ktds, ktstrg_buf); /* build */
	    if ( cc > 0 ) {
		ktfn_desc_pt = ktds;
		ktfn_desc_nb = nb;
		return;
	    }
	}
    }
    if ( ktds ) free (ktds);
    if ( ktstrg_buf ) free (ktstrg_buf);
    return;
}

static void get_linux_map (char * dummy)
{
    int i, j;
    short val;
    int fn, cc;
    struct kbentry kbetr;
    struct kbsentry kbstr;

    fn = 0;     /* ioctl on console input */
    /* get the function string */
    get_linux_kbmap_string (fn);

    /* get the map */
    for ( j = 0 ; j < nb_kbmap ; j++ ) {
	val = kbetr.kb_table = keyboard_map[j];
	if ( val < 0 ) continue;    /* do not read */
	for ( i = 0 ; i < nb_key ; i++ ) {
	    kbetr.kb_index = pc_keyboard_desc[i].kcode;
	    kbetr.kb_value = 0;
	    cc = ioctl (fn, KDGKBENT, &kbetr);  /* read map entry */
	    if ( cc < 0 ) continue;     /* some error */
	    val = kbetr.kb_value;
	    if ( val == K_NOSUCHMAP) break; /* no such map */
	    if ( val == K_HOLE ) continue;  /* nothing mapped */
	    pc_keyboard_desc[i].ktfunc[j] = val;
	}
    }
}

/* get_linux_ktpad_strg : special case to get the string for Key Pad */

static char * get_linux_ktpad_strg (int *ktcodept, int shift, char **tcap_strg)
{
    char *strg;
    int i;

    strg = get_kt_strg (*ktcodept, NULL);
    if ( !keypad_appl_mode || (shift == shifted) ) {
	/* translate */
	if ( *ktcodept == K_P5 ) {
	    strg = ( keypad_appl_mode ) ? kp5_trans_appl_strg : kp5_trans_num_strg;
	} else {
	    for ( i = 0 ; i < kp_translate_nb ; i +=2 ) {
		if ( kp_translate[i] == *ktcodept ) break;
	    }
	    if ( i < kp_translate_nb ) {
		*ktcodept = kp_translate[i+1];
		strg = get_kt_strg (*ktcodept, NULL);
	    }
	}
    }
    return (strg);
}

/* get_linux_ktspec_strg : special case to get the string for special key */

static char * get_linux_ktspec_strg (int ktcode, char **tcap_strg)
{
    char *strg;

    switch (ktcode) {
	case K_NUM :
	    if ( ! keypad_appl_mode ) return (NULL);
	    break;
	default :
	}
    strg = get_kt_strg (ktcode, NULL);
    return (strg);
}

/* get_linux_kt_escseq : get the escape sequence for a given kt code */

static char *get_linux_kt_escseq (int ktcode, char **tcap_strg)
{
    int i, aridx, nb;
    int idx, kttype;
    struct KTdesc *ktpt;

    kttype = KTYP (ktcode);
    for ( idx = 0 ; idx < inuse_ktdesc_nb ; idx++ ) {
	if ( kttype == inuse_ktdesc [idx].kt_type ) break;
    }
    if ( idx >= inuse_ktdesc_nb ) return (NULL);
    if ( ! inuse_ktdesc [idx].ktdesc_array ) return (NULL);

    nb = *(inuse_ktdesc [idx].ktdesc_nb);
    for ( i = 0 ; i < nb ; i++ ) {
	ktpt = *(inuse_ktdesc [idx].ktdesc_array) + i;
	if ( ktpt->ktfunc != ktcode ) continue;
	if ( tcap_strg ) *tcap_strg = ktpt->tcap;
	return (ktpt->strg );
    }
    return (NULL);
}

static char *get_linux_kt_strg (int *ktcode_pt, unsigned int shift)
{
    static char latin[2];
    char *strg;
    int ktcode;

    ktcode = *ktcode_pt;
    switch ( KTYP (ktcode) ) {
	case KT_FN :
	case KT_CUR :
	    strg = get_linux_kt_escseq (ktcode, NULL);
	    break;

	case KT_PAD :
	    /* can modify ktcode according to the console state */
	    strg = get_linux_ktpad_strg (&ktcode, shift, NULL);
	    break;

	case KT_SPEC :
	    strg = get_linux_ktspec_strg (ktcode, NULL);
	    break;

	case KT_LATIN :
	    strg = get_linux_kt_escseq (ktcode, NULL);
	    if ( ! strg ) {
		latin[0] = KVAL (ktcode);
		latin[1] = '\0';
		strg = latin;
	    }
	    break;

	default :
	    strg = NULL;
	}
    *ktcode_pt = ktcode;
    return (strg);
}

/* ------------------------------------------------------------------------ */

/* Routines specific for the xterm family terminal */
/* ----------------------------------------------- */

static int compare_keyboard_desc (Key_Assign **kdesc1, Key_Assign **kdesc2)
{
    return ((*kdesc1)->Xkcode - (*kdesc2)->Xkcode );
}

static int compare_Xkt_desc_tcap (struct KTdesc **Xkt_desc1, struct KTdesc **Xkt_desc2)
{
    return (strcasecmp ((*Xkt_desc1)->tcap, (*Xkt_desc2)->tcap) );
}

static int compare_Xkt_desc_keysym (struct KTdesc **Xkt_desc1, struct KTdesc **Xkt_desc2)
{
    return ((*Xkt_desc1)->ktfunc - (*Xkt_desc2)->ktfunc);
}

static get_xterm_map (char * xmodmap_filename)
{
    Key_Assign *keyboard_desc_pt [pc_keyboard_desc_nb];
    Key_Assign **kbdesc_ptpt, *kbdesc_pt, *kbdesc_val_pt, kbdesc;
    struct KTdesc **Xktdesc_ptpt, *Xktdesc_pt, Xktdesc;
    int i, j, idx, nb, val, sz;
    FILE *xmodmap_file;
    char line [256], *sp, *sp1;
    char * ksym_strg [nb_kbmap];

    /* build arrays for quick search */
    /* array for keyboard description */
    for ( i = 0 ; i < pc_keyboard_desc_nb ; i++ )
	keyboard_desc_pt [i] = &pc_keyboard_desc [i];
    qsort ((void *)keyboard_desc_pt , pc_keyboard_desc_nb,
	   sizeof (Key_Assign *), (int (*)()) compare_keyboard_desc);

    /* array for keysymb descrition */
    for ( i = Xkt_desc_pt_nb = 0 ; i < all_Xktdesc_nb ; i++ )
	Xkt_desc_pt_nb += *(all_Xktdesc [i].ktdesc_nb);
    Xkt_desc_pt = (struct KTdesc *(*)[]) calloc (Xkt_desc_pt_nb, sizeof (struct KTdesc **));
    if ( ! Xkt_desc_pt ) return;    /* nothing can be done */

    for ( i = idx = 0 ; i < all_Xktdesc_nb ; i++ ) {
	nb = *(all_Xktdesc [i].ktdesc_nb);
	for ( j = 0 ; j < nb ; j++ ) {
	    (*Xkt_desc_pt) [idx++] = &((*(all_Xktdesc [i].ktdesc_array))[j]);
	}
    }
    qsort ((void *)Xkt_desc_pt , Xkt_desc_pt_nb,
	   sizeof (struct KTdesc *), (int (*)()) compare_Xkt_desc_tcap);

    /* parse the output of "xmodmap -pke" to get the mapping */
    xmodmap_file = fopen (xmodmap_filename, "r");
    if ( xmodmap_file == NULL ) return;

    while ( fgets (line, sizeof line, xmodmap_file) != NULL ) {
	if ( strncmp ("keycode ", line, 8) != 0 ) continue;
	sp = strchr (line, '=');
	if ( ! sp ) continue;

	kbdesc_val_pt = &kbdesc;

	/* get the keycode value */
	*(sp++) = '\0';
	val = atoi (&line[8]);
	kbdesc_val_pt->kcode = val;

	sz = strlen (sp);
	if ( sz <= 1 ) continue;    /* nothing assigned */

	/* get the keysym strings */
	memset (ksym_strg, 0, sizeof (ksym_strg));
	sp [sz-1] = '\0';   /* remove the trailling '\n' */
	sp1 = sp;
	for ( j = 0 ; j < nb_kbmap ; j ++ ) {
	    for ( ; *sp1 == ' ' ; sp1++ ) ; /* skip over headding space */
	    if ( *sp1 == '\0' ) break;  /* nothing more */
	    ksym_strg [j] = sp1;
	    sp1 = strchr (ksym_strg [j], ' ');
	    if ( ! sp1 ) break;
	    *(sp1++) = '\0';    /* remove trailling space */
	}

	/* setup the keyboard descriptor */
	kbdesc_pt = NULL;
	for ( j = 0 ; j < nb_kbmap ; j ++ ) {
	    if ( ! ksym_strg [j] ) continue;    /* nothing assigned */

	    Xktdesc.tcap = ksym_strg [j];
	    Xktdesc_pt = &Xktdesc;
	    Xktdesc_ptpt = (struct KTdesc **) bsearch (&Xktdesc_pt,
			    *Xkt_desc_pt, Xkt_desc_pt_nb,
			    sizeof (struct KTdesc *),
			    (int (*)()) compare_Xkt_desc_tcap);
	    if ( ! Xktdesc_ptpt ) continue;

	    Xktdesc_pt = *Xktdesc_ptpt;
	    if ( j == 0 ) {
		/* the unshifted assignement is supposed to be 'regular' */
		/* get the key for this value */
		kbdesc_val_pt->Xkcode = Xktdesc_pt->ktfunc;     /* keysymb value */
		kbdesc_ptpt = (Key_Assign **) bsearch (&kbdesc_val_pt, keyboard_desc_pt,
				pc_keyboard_desc_nb, sizeof (Key_Assign *),
				(int (*)()) compare_keyboard_desc);
		if ( kbdesc_ptpt ) {
		    kbdesc_pt = *kbdesc_ptpt;
		    kbdesc_pt->kcode = kbdesc_val_pt->kcode;
		}
	    }
	    if ( kbdesc_pt ) {
		kbdesc_pt->ktfunc [j] = Xktdesc_pt->ktfunc;
	    }
	}
    }

    fclose (xmodmap_file);
    /* re-order the array to be used by "get_xterm_kt_escseq" */
    qsort ((void *)Xkt_desc_pt, Xkt_desc_pt_nb,
	   sizeof (struct KTdesc *), (int (*)()) compare_Xkt_desc_keysym);

    /* build the special case array of cursor key for nxterm */
    for ( i = 0 ; i < nxterm_special_nb ; i++ ) {
	struct KTdesc Xktdesc, *Xktdesc_pt, **Xktdesc_ptpt;
	Xktdesc.ktfunc = nxterm_special [i].ktfunc;
	Xktdesc_pt = &Xktdesc;
	Xktdesc_ptpt = (struct KTdesc **) bsearch (&Xktdesc_pt,
			*Xkt_desc_pt, Xkt_desc_pt_nb,
			sizeof (struct KTdesc *),
			(int (*)()) compare_Xkt_desc_keysym);
	if ( ! Xktdesc_ptpt ) continue;
	nxterm_special [i].Xkt_desc_pt = *Xktdesc_ptpt;
    }
}

/* get_xterm_kt_escseq : get the escape sequence for a given kt (keysym) code */

static char *get_xterm_kt_escseq (int keysym, char **tcap_strg)
{
    static char latin[2];
    struct KTdesc Xktdesc, *Xktdesc_pt, **Xktdesc_ptpt;

    if ( tcap_strg ) *tcap_strg = NULL;

    if ( (keysym >= XK_space) && (keysym <= XK_asciitilde) ) {
	latin [0] = (keysym - XK_space) + ' ';
	latin [1] = '\0';
	return (latin);
    }

    /* --------------
    if ( (keysym >= XK_KP_Home) && (keysym <= XK_KP_Begin) ) {
	keysym += XK_Home - XK_KP_Home;
    }
    */

    Xktdesc.ktfunc = keysym;
    Xktdesc_pt = &Xktdesc;
    Xktdesc_ptpt = (struct KTdesc **) bsearch (&Xktdesc_pt,
		    *Xkt_desc_pt, Xkt_desc_pt_nb,
		    sizeof (struct KTdesc *),
		    (int (*)()) compare_Xkt_desc_keysym);
    if ( ! Xktdesc_ptpt ) return (NULL);

    Xktdesc_pt = *Xktdesc_ptpt;
    if ( tcap_strg ) *tcap_strg = Xktdesc_pt->tcap;
    return (Xktdesc_pt->strg);
}

/* get_xterm_kt_strg : get the string for a given kt (keysym) code */

static char *get_xterm_kt_strg (int *keysym_pt, unsigned int shift)
{
    static char latin[2];
    char *strg;
    int keysym;

    keysym = *keysym_pt;
    if ( (keysym >= XK_space) && (keysym <= XK_asciitilde) ) {
	latin [0] = (keysym - XK_space) + ' ';
	latin [1] = '\0';
	return (latin);
    }

    /* -------------
    if ( (keysym >= XK_KP_Home) && (keysym <= XK_KP_Begin) ) {
	keysym += XK_Home - XK_KP_Home;
	*keysym_pt = keysym;
    }
    */
    strg = get_xterm_kt_escseq (keysym, NULL);
    return (strg);
}

/* ------------------------------------------------------------------------ */

/* Try to assert the terminal mode (according to the initialisation string) */
/* ------------------------------------------------------------------------ */

static char * get_last_occurence (char *cs, char *ct)
{
    char *str, *lstr;

    lstr = NULL;
    for ( str = cs ; str ; str++ ) {
	str = strstr (str, ct);
	if ( ! str ) break;
	lstr = str;
    }
    return (lstr);
}

/* analyse the initialisation string */

static int compare_init_seq (struct Init_Sequence *sq1, struct Init_Sequence *sq2)
{
    return ( sq1->ini - sq2->ini );
}

static void get_terminal_mode (char *init_strg, Flag *app_mode, Flag *alt_cursor_mode)
{
    int i;
    char *sp;
    struct Init_Sequence initseq [init_seq_nb];

    if ( ! init_strg ) return;

    memcpy (initseq, init_seq, sizeof (init_seq));
    for ( i = 0 ; i < init_seq_nb ; i++ ) {
	sp = get_last_occurence (init_strg, initseq[i].ref);
	initseq[i].ini = ( sp ) ? sp - init_strg : -1;
    }
    qsort ((void *)initseq, init_seq_nb, sizeof (initseq[0]), (int (*)()) compare_init_seq);
    /* build the status */
    for ( i = 0 ; i < init_seq_nb ; i++ ) {
	if ( initseq[i].ini < 0 ) continue;
	switch (initseq[i].type) {
	    case 0 :        /* reset (normal cursor and numerical */
		*app_mode = *alt_cursor_mode = NO;
		break;
	    case 1 :        /* numeric key pad mode */
		*app_mode = NO;
		break;
	    case 2 :        /* application key pad mode */
		*app_mode = YES;
		break;
	    case 3 :        /* normal cursor mode */
		*alt_cursor_mode = NO;
		break;
	    case 4 :        /* alternate cursor mode */
		*alt_cursor_mode = YES;
		break;
	}
    }
}

/* set_term_mode : set terminal mode (cursor and numerical key pad) */
/* ---------------------------------------------------------------- */

static void switch_mode (struct KTdesc *ktdesc_pt, int nb, Flag mode_flg)
{
    int i;
    char *sp;

    for ( i = 0 ; i < nb ; i++ ) {
	sp = ( mode_flg ) ? ktdesc_pt [i].altstrg : ktdesc_pt [i].norstrg ;
	if ( sp ) ktdesc_pt [i].strg = sp;
    }
}

static void set_appl_mode ()
{
    if ( ! Xterminal_flg ) return;  /* nothing for linux terminal */

    switch_mode (Xktpad_desc_pt, Xktpad_desc_nb, keypad_appl_mode);
}


static void set_cursor_mode ()
{
    switch_mode ( ( Xterminal_flg ) ? Xktcur_desc_pt : ktcur_desc_pt,
		  ( Xterminal_flg ) ? Xktcur_desc_nb : ktcur_desc_nb,
		  cursor_alt_mode);
}

static void set_term_mode (Flag keypad_mode, Flag cursor_mode, Flag set)
{
    int idx;

    keypad_appl_mode = keypad_mode;
    cursor_alt_mode  = cursor_mode;
    set_appl_mode ();
    set_cursor_mode ();
    if ( !set ) return;

    idx = ( keypad_appl_mode ) ? 2 : 1;
    fputs (init_seq[idx].ref, stdout);
    idx = ( cursor_alt_mode ) ? 4 : 3;
    fputs (init_seq[idx].ref, stdout);
}

/* get_kb_map : get the current active keyboard mapping */
/* ---------------------------------------------------- */

static void get_kb_map (char *terminal, char *init_strg,
			Flag *app_mode,
			Flag *alt_cursor_mode)
{
    static char * terminal_type (char *);
    int i, j;
    short val;
    int fn, cc;
    struct kbentry kbetr;
    struct kbsentry kbstr;
    char * xmodmap_filename;

    if ( get_map_flg ) return;  /* init already done */

    /* check for terminal type and build xmodmap file if needed */
    terminal_name = terminal;
    xmodmap_filename = terminal_type (terminal);

    /* init according to the terminal type */
    kt_void         = ( Xterminal_flg ) ? XK_VoidSymbol  : K_HOLE;
    inuse_ktdesc    = ( Xterminal_flg ) ? all_Xktdesc    : all_ktdesc;
    inuse_ktdesc_nb = ( Xterminal_flg ) ? all_Xktdesc_nb : all_ktdesc_nb;

    /* reset the keyboard description according to the terminal class */
    for ( i = 0 ; i < nb_key ; i++ ) {
	pc_keyboard_desc[i].kcode = ( Xterminal_flg ) ? XK_VoidSymbol : pc_keyboard_desc[i].lkcode;
	for ( j = 0 ; j < nb_kbmap ; j++ ) {
	    /* reset to no action mapped */
	    pc_keyboard_desc[i].ktfunc[j] = kt_void;
	}
    }

    if ( init_strg ) get_terminal_mode (init_strg, app_mode, alt_cursor_mode);

    if ( ! xmodmap_filename ) Xterminal_flg = NO;
    if ( Xterminal_flg ) get_xterm_map (xmodmap_filename);
    else                 get_linux_map (xmodmap_filename);
    if ( xmodmap_filename ) (void) unlink (xmodmap_filename);

    /* initialisation done */
    get_map_flg = YES;
}

/* terminal_type : test for X terminal case and build xmodmap file */
/* --------------------------------------------------------------- */
/*  If the terminal name is neither "linux" nor "console" (standard names for linux console)
	use a call to "xmodmap -pke" to produce the file which will be
	parsed to get the keyboard mapping.
	If the call to xmodmap return an error, it assumes that the user
	terminal is not an X terminal.
*/

static char * terminal_type (char *terminal)
{
    static char strg [PATH_MAX + 64];
    static char *tfname = NULL;
    char tnm [64];
    int cc;

    Xterminal_flg = NO;     /* default */
    if ( ! terminal ) return;
    if (   (strcasecmp (terminal, "linux") == 0)
	|| (strcasecmp (terminal, "console") == 0) ) return (NULL);

    /* can be a X terminal emulator */
    memset (strg, 0, sizeof (strg));
    strcpy (strg, "xmodmap -pke > ");
    tfname = tmpnam (strg + strlen (strg));
    if ( ! tfname ) return;

    cc = system (strg);
    if ( cc != 0 ) {
	unlink (tfname);
	return (NULL);
    }
    Xterminal_flg = YES;
    return (tfname);
}

/* get_keyboard_map : load the key map according to the init escape sequence */
/* ------------------------------------------------------------------------- */

Flag get_keyboard_map (char *terminal, int strg_nb,
		       char *strg1, char *strg2, char *strg3,
		       char *strg4, char *strg5, char *strg6)
{
    int i, sz;
    Flag app_mode, alt_cursor_mode;
    char *strg[6];

    if ( strg_nb > (sizeof (strg) / sizeof (strg[0])) )
	strg_nb = (sizeof (strg) / sizeof (strg[0]));
    strg [0] = strg1;
    strg [1] = strg2;
    strg [2] = strg3;
    strg [3] = strg4;
    strg [4] = strg5;
    strg [5] = strg6;

    init_msg = NULL;
    for ( sz = i = 0 ; i < strg_nb ; i++ )
	if ( strg[i] ) sz += strlen (strg[i]);
    if ( sz ) {
	init_msg = malloc (sz);
	memset (init_msg, 0, sz);
    }
    if ( init_msg ) {
	for ( i = 0 ; i < strg_nb ; i++ )
	    if ( strg[i] ) strcat (init_msg, strg[i]);
    } else init_msg = "Not defined";

    app_mode = alt_cursor_mode = NO;    /* assume default mode */
    get_kb_map (terminal, init_msg, &app_mode, &alt_cursor_mode);
    set_term_mode (app_mode, alt_cursor_mode, NO);
    return (Xterminal_flg);
}

/* get_keyboard_mode_strg : return the current keyboard mode */
/* --------------------------------------------------------- */

static void get_kbmode_strgs (char **appl_strg, char **curs_strg)
{
    *appl_strg = ( keypad_appl_mode ) ? "Application" : "Numeric";
    *curs_strg = ( cursor_alt_mode )  ? "Alternate"   : "Normal";
}

char * get_keyboard_mode_strg ()
{
    static char kbmode_strg [80];
    char *appl_strg, *curs_strg;

    get_kbmode_strgs (&appl_strg, &curs_strg);
    sprintf (kbmode_strg, "%s Key Pad, %s Cursor", appl_strg, curs_strg);
    return (kbmode_strg);
}

static char * get_key_bycode (int ktfunc)
{
    static char strg[128];
    int i, j;
    short val;

    memset (strg, 0, sizeof (strg));
    if ( ktfunc == kt_void ) return (strg);

    for ( i = 0 ; i < nb_key ; i++ ) {
	for ( j = 0 ; j < nb_kbmap ; j++ ) {
	    val = pc_keyboard_desc[i].ktfunc[j];
	    if ( val != ktfunc ) continue;
	    sprintf (&strg[strlen (strg)], "%s%s, ",
		     key_shift_msg[j], pc_keyboard_desc[i].klabel);
	}
    }
    return (strg);
}

/* get_ktfunc_by_string : while *i and *j >= 0 : can be more entry */
/*  return kt_void : string is existing but no KT function attached */

static int get_ktfunc_by_string (char *strg, int *i, int *j)
{
    char *sp;
    int ktfunc;
    struct KTdesc *ktpt;

    ktfunc = kt_void;
    if ( (*i >= 0) && (*j >= 0) ) {
	for (  ; *i < inuse_ktdesc_nb ; (*i)++ ) {
	    if ( ! inuse_ktdesc [*i].ktdesc_array ) continue;
	    for ( ; *j < *(inuse_ktdesc [*i].ktdesc_nb) ; (*j)++ ) {
		ktpt = *(inuse_ktdesc [*i].ktdesc_array) + (*j);
		if ( ktpt->ktfunc == 0 ) continue;
		if ( ktpt->ktfunc == kt_void ) continue;
		sp = ktpt->strg;
		if ( !sp ) continue;
		if ( strcmp (sp, strg) == 0 ) {
		    ktfunc = ktpt->ktfunc;
		    return (ktfunc);
		}
	    }
	}
    }
    *i = *j = -1;
    return (ktfunc);
}


/* get_kt_strg : get the string for a given kt (or keysym) code */
/* ------------------------------------------------------------ */

static char *get_kt_strg (int ktcode, char **tcap_strg)
{
    char *sp;

    if ( tcap_strg ) *tcap_strg = NULL;
    if ( ktcode == kt_void ) return (NULL);
    sp = ( Xterminal_flg ) ? get_xterm_kt_escseq (ktcode, tcap_strg)
			   : get_linux_kt_escseq (ktcode, tcap_strg);
    return (sp);
}

/* ------------------------------------------------------------------------
  In Linux xterm, which is compiled with the OPT_VT52_MODE keys F1-F4
    the string generated for the keys F1-F4 are translated into
    the string for the keys PF1-PF4
    Ref to input.c file of xterm, routine : Input
  -------------------------------------------------------------------------
*/

/* xterm_terminal : check for xterm terminal type */
/* ---------------------------------------------- */

static Flag xterm_terminal ()
{
    extern char *tname;

    if ( !Xterm_flg ) return (NO);
    if ( strcasecmp (tname, "xterm") != 0 ) return (NO);

    return (YES);
}


/* overwrite_PF1PF4 : overwrite the cmd for PF1-PF4 with the cmd for F1-F4 */
/* ----------------------------------------------------------------------- */

void overwrite_PF1PF4 ()
{
    extern int itoverwrite ();
    extern struct itable *ithead;

    int i, cc, nb;
    char **strg;
    unsigned char cmd;

    if ( ! xterm_terminal () ) return;

    for ( i = xterm_F1F4_keys_nb -1 ; i >= 0 ; i-- ) {
	cc = itoverwrite (xterm_F1F4_keys[i].Fstring, xterm_F1F4_keys[i].PFstring, ithead);
    }
}


/* reverse_xterm_F1F4 : reverse the F1-F4 to PF1-PF4 interpretation for xterm */
/* -------------------------------------------------------------------------- */

static void reverse_xterm_F1F4 (char *escp)
{
    int i;

    if ( ! xterm_terminal () ) return;

    for ( i = xterm_F1F4_keys_nb -1 ; i >= 0 ; i-- ) {
	if ( strcmp (xterm_F1F4_keys[i].PFstring, escp) != 0 ) continue;
	strcpy (escp, xterm_F1F4_keys[i].Fstring);
	return;
    }
}

/* string_to_key : get the key and modifier for a given string */
/* ----------------------------------------------------------- */
/*  To start a new scan ; *key = -1
    Return key label and modifier string,
	  or NULL if nothing or no more key
*/

static Flag nxterm_special_case (int ktf, char *strg)
{
    int i;
    char *strg1;

    if ( ! Xterminal_flg ) return (NO);

    /* try for special cursor case (in case of translation overrride) */
    for ( i = nxterm_special_nb ; i >= 0 ; i-- ) {
	if ( nxterm_special [i].ktfunc != ktf ) continue;
	if ( ! nxterm_special [i].Xkt_desc_pt ) continue;
	strg1 = ( cursor_alt_mode )
		? nxterm_special [i].Xkt_desc_pt->norstrg
		: nxterm_special [i].Xkt_desc_pt->altstrg;
	if ( strcmp (strg, strg1) == 0 ) return (YES);
    }
    return (NO);
}


static char * string_to_key (char *strg, int *key, int *idx, int *shift, char **modstrg)
{
    static char * kcode_to_string (int, unsigned int, Flag, Flag, int *, char **, char **);

    int i, j;
    int ktf;
    char *ktstrg;
    char *klb, *shf;


    if ( *key < 0 ) {
	*key = pc_keyboard_desc [0].kcode;
	*shift = -1;
    }
    for ( i = 0 ; i < nb_key ; i++ ) {
	if ( *key == pc_keyboard_desc [i].kcode ) break;
    }
    j = *shift +1;
    if ( j >= nb_kbmap ) {
	i++ ; j = 0;
    }
    for ( ; i < nb_key ; i++ ) {
	for (  ; j < nb_kbmap ; j++ ) {
	    if ( keyboard_map[j] < 0 ) continue;
	    ktf = pc_keyboard_desc [i].ktfunc[j];
	    if ( ktf == kt_void ) continue;
	    ktstrg = kcode_to_string (pc_keyboard_desc [i].kcode, (unsigned int) j,
		     keypad_appl_mode, cursor_alt_mode,
		     &ktf, &klb, &shf);
	    if ( ! ktstrg ) continue;
	    if ( strcmp (strg, ktstrg) != 0 ) {
		if ( !Xterminal_flg || !nxterm_special_case (ktf, strg) ) continue;
	    }
	    if ( modstrg ) *modstrg = key_shift_msg [j];
	    *key = pc_keyboard_desc [i].kcode;
	    *shift = j;
	    *idx = i;
	    return (pc_keyboard_desc [i].klabel);
	}
	j = 0;
    }
    if ( i >= nb_key ) *key = -1;
    return (NULL);
}

/* all_string_to_key : all keys label for a given escape sequence */
/* -------------------------------------------------------------- */
/*  The buffer (buf) must be large enough to be split into "nbkbmap"
	(the value "nbkbmap" provided by the caller can be reduce
	to the best value for the usage of the buffer),
	substrings which will receive the key labels for the
	corresponding modifier.
    On return keys is init with the pointer to label string of the modifiers,
	if no keys, the string is empty.

    Can be call more that one time without re-init of keys_pt to add
	keys label for various escape sequences. Between 2 of these calls
	nbkbmap, buf and buf_sz must not be modified.
	The first call is flag by NULL value in the keys (char *) array.
*/

void all_string_to_key (char *escp_seq,
	char *(*keys_pt) [],    /* pointer to array of nb_kbmap pointer to char */
	int  *keys_nb,          /* nb of members, return usable nb */
	char *buf, int buf_sz)  /* buffer for key label and its size */
{
    int i, idx, nb, sz;
    int nbkbmap, label_sz;
    int key_code, shift;
    Flag kfnd_flg [pc_keyboard_desc_nb][nb_kbmap];

    char *key_label, *modf_label;
    char **keys;

    if ( !get_map_flg ) return;
    if ( *keys_nb <= 0 ) return;    /* nothing can be done */

    keys = (char **) *keys_pt;
    if ( ! *keys ) {
	/* keys is NULL : initialisation to be done */
	memset (*keys_pt, 0, *keys_nb * sizeof((*keys_pt)[0]));
	memset (buf, 0, buf_sz);
	memset (kfnd_flg, 0, sizeof (kfnd_flg));

	/* get the best value for the number of usefull key maps */
	nbkbmap = *keys_nb;
	if ( nbkbmap > nb_kbmap ) nbkbmap = nb_kbmap;
	for ( i = nb = 0 ; i < nbkbmap ; i++ ) {
	    if ( keyboard_map [i] < 0 ) continue;
	    nb++;
	}
	if ( nb < nbkbmap ) nbkbmap = nb;     /* usefull number of map */
	*keys_nb = nbkbmap;

	label_sz = buf_sz / nbkbmap;
	if ( label_sz < 16 ) return;    /* too small buffer */
	/* init the array of pointer to key label strings */
	for ( i = 0 ; i < nbkbmap ; i++ ) {
	    keys [i] = &buf [i*label_sz];
	}
    } else {
	nbkbmap = *keys_nb;
	label_sz = buf_sz / nbkbmap;
    }

    for ( key_code = shift = -1 ; ; ) {
	key_label = string_to_key (escp_seq, &key_code, &idx, &shift, &modf_label);
	if ( ! key_label ) break;
	if ( shift >= nbkbmap ) continue;
	if ( ! *key_label ) continue;
	if ( !keys[shift][0] && modf_label && *modf_label ) {
	    strcpy (keys[shift], modf_label);
	    sz = strlen (modf_label);
	    if ( modf_label [sz -1] == ' ' ) sz--;
	    strcpy (&keys[shift][sz], ": ");
	}

	if ( kfnd_flg [idx][shift] ) continue;  /* already catched */

	/* leave space at the end for an extra separator character */
	if ( (strlen (key_label) + strlen (keys[shift]) +2) < (label_sz -1) )
	    sprintf (&keys[shift][strlen (&keys[shift][0])], "%s ", key_label);
	kfnd_flg [idx][shift] = YES;
    }
}

/* kcode_to_string : get the string for a given key and console state */
/* ------------------------------------------------------------------ */

static char * kcode_to_string (int kcode, unsigned int shift,
		     Flag applic_mode, Flag curs_mode,
		     int *ktcodept,
		     char **shift_strg, char **key_label)
{
    static char latin[2];
    int i;
    int ktcode;
    char *strg;

    *shift_strg = *key_label = NULL;
    *ktcodept = kt_void;
    if ( (shift >= nb_kbmap) && (keyboard_map [shift] < 0) )
	return (NULL);
    *shift_strg = key_shift_msg [shift];
    for ( i = nb_key-1 ; i >= 0 ; i-- ) {
	if ( pc_keyboard_desc [i].kcode == kcode ) break;
    }
    if ( i < 0 ) return (NULL);     /* no key descriptor */

    *key_label = pc_keyboard_desc [i].klabel;
    ktcode = pc_keyboard_desc [i].ktfunc[shift];
    if ( ktcode == kt_void ) return (NULL);  /* nothing assigned to this key */

    strg = ( Xterminal_flg ) ? get_xterm_kt_strg (&ktcode, shift)
			     : get_linux_kt_strg (&ktcode, shift);
    *ktcodept = ktcode;
    return (strg);
}

/* checkkeyb : interactive check fo keyboard state */
/* ----------------------------------------------- */

static void checkkeyb (Flag echo)
{
    static char * escseqstrg ();
    static Flag catch_escape_seq ();

    static char msg[] = "type \"Ctrl C\" exit, \"Ctrl A\" switch App mode, \"Ctrl B\" switch cusor mode\n";
    Flag app_mode, alt_cursor_mode;
    char ch, escp[16];
    int idx;
    char *st, *st1;
    int ktcode, tmode;
    Flag nl;

    fputs (msg, stdout); nl = YES;
    for ( idx = 0 ; ch != '\003' ; ) {  /* exit on <Ctrl>C */
	ch = getchar ();
	if ( catch_escape_seq (echo, ch, &idx, &nl) ) continue;
	if ( ch == '\033' ) continue;
	if ( (ch == '\r') || (ch == '\n') ) {
	    if ( echo && !nl ) putchar ('\n');
	    fputs (msg, stdout); nl = YES;
	    continue;
	}
	if ( echo ) {
	    if ( ch < ' ' ) {
		putchar ('^'); putchar (ch + '@');
		}
	    else if ( ch == '\177' ) putchar ('\137');
	    else putchar (ch);
	    nl = NO;
	}
	if ( ch < ' ' ) {
	    tmode = -1;
	    if ( ch == '\001' ) {
		app_mode = ! keypad_appl_mode;
		tmode = ( app_mode ) ? 2 : 1;
	    }
	    else if ( ch == '\002' ) {
		alt_cursor_mode = ! cursor_alt_mode;
		tmode = ( alt_cursor_mode ) ? 4 : 3;
	    }
	    if ( tmode > 0 ) {
		set_term_mode (app_mode, alt_cursor_mode, YES);
		get_kbmode_strgs (&st, &st1);
		printf ("%s = New terminal mode, send : \"%s\"\n--- new terminal mode : %s key pad, %s cursor ----\n",
			init_seq[tmode].ref,
			escseqstrg (init_seq[tmode].ref),
			st, st1);
		nl = YES;
	    }
	}
    }
}

#ifndef TEST_PROGRAM
/* check_keyboard : interactive check fo keyboard state */
/* ---------------------------------------------------- */

void check_keyboard ()
{
    static char * escseqstrg ();
    char *st, *st1;
    int applmod, cursmod;

    /* save the initial terminal mode */
    applmod = keypad_appl_mode;
    cursmod = cursor_alt_mode;

    set_term_mode (applmod, cursmod, NO);
    get_kbmode_strgs (&st, &st1);
    printf ("\n--- terminal mode : %s key pad, %s cursor ----\n", st, st1);
    printf ("    Initialisation string : %s\n", escseqstrg (init_msg));
    printf ("    <kbinit> (from kbfile) : %s\n\n", escseqstrg (kbinistr));

    if ( Xterminal_flg ) {
	printf ("    If the mapping seem to be strange, check the xmodemap mapping\n");
	printf ("    and you can use xev to see if the mapping is 'regular' :\n");
	printf ("        F1 mapped to F1 key ...\n\n");

    }
    if ( xterm_terminal () ) {
	printf ("    Warning : for xterm terminal type PF1 .. PF4 are mapped to F1 .. F4\n\n");
    }

    checkkeyb (YES);
    /* restaure terminal mode */
    set_term_mode (applmod, cursmod, YES);
}
#endif /* - TEST_PROGRAM */

static char * escseqstrg (char *escst)
{
    static char st[128];
    char *sp, ch;

    if ( !escst || !escst[0] ) return;
    memset (st, 0, sizeof (st));
    for ( sp = escst; (ch = *sp) ; sp++ ) {
	if ( ch == '\033' ) strcat (st, "<Esc>");
	else if ( ch == '\t' ) strcat (st, "<Tab>");
	else if ( ch == '\177' ) strcat (st, "<Bksp>");
	else if ( ch < ' ' ) {
	    st[strlen(st)] = '^';
	    st[strlen(st)] = '@' + ch;
	}
	else st[strlen(st)] = ch;
    }
    return (st);
}


static void print_keys (char *escp, Flag *nl_pt)
{
    int i, sz;
    char label_buf [nb_kbmap * 128];
    char *labels_strgs [nb_kbmap];
    int labels_strgs_nb;

    if ( ! *nl_pt ) putchar ('\n');
    printf ("\"%s\" by: ", escseqstrg (escp));

    labels_strgs_nb = sizeof (labels_strgs) / sizeof (labels_strgs [0]);
    memset (labels_strgs, 0, sizeof (labels_strgs));
    all_string_to_key (escp, &labels_strgs, &labels_strgs_nb,
		       label_buf, sizeof (label_buf));
    for ( i = 0 ; i < labels_strgs_nb ; i++ ) {
	if ( labels_strgs [i] &&  labels_strgs [i][0] ) {
	    sz = strlen (labels_strgs [i]);
	    if ( labels_strgs [i][sz -1] == ' ' ) {
		labels_strgs [i][sz -1] = ';';
		labels_strgs [i][sz] = ' ';
	    }
	    fputs (labels_strgs [i], stdout);
	}
    }
    putchar ('\n');
    *nl_pt = YES;
}

static void assess_term_mode (char *escp)
{
    int i;
    char *st, *st1;
    Flag cur_flg;

    if ( ! (assess_cursor_flg || assess_keypad_flg) ) return;

    if ( assess_cursor_flg ) {
	/* try to found cursor mode */
	cur_flg = NO;
	for ( i = 0 ; i < KT_CUR_nb ; i++ ) {
	    if ( strcmp (escp, kt_desc_cur_norm [i].strg) == 0 ) {
		assess_cursor_flg = NO;
		break;
	    }
	}
	if ( assess_cursor_flg ) {
	    cur_flg = YES;
	    for ( i = 0 ; i < KT_CUR_nb ; i++ ) {
		if ( strcmp (escp, kt_desc_cur_alt [i].strg) == 0 ) {
		    assess_cursor_flg = NO;
		    break;
		}
	    }
	}
	if ( ! assess_cursor_flg ) cursor_alt_mode = cur_flg;
    }
    if ( assess_keypad_flg ) {
	/* try to found key pad mode (using KP-5 key) */
	assess_keypad_flg = NO;     /* assume it will be found */
	if ( strcmp (escp, kp5_trans_num_strg) == 0 ) keypad_appl_mode = NO;
	else if ( strcmp (escp, kp5_trans_appl_strg) == 0 ) keypad_appl_mode = YES;
	else if ( strcmp (escp, kp5_trans_appl1_strg) == 0 ) keypad_appl_mode = YES;
	else     assess_keypad_flg = YES;
    }
    if ( ! (assess_cursor_flg || assess_keypad_flg) ) {
	set_term_mode (keypad_appl_mode, cursor_alt_mode, NO);

	get_kbmode_strgs (&st, &st1);
	printf ("\n--- assessed terminal mode : %s key pad, %s cursor ----\n\n", st, st1);
    }
}

static Flag catch_escape_seq (Flag echo, char ch, int *idx, Flag *nl_pt)
{
    static char escp [16];
    char small_strg [2];

    if ( !*idx && (ch != '\033') ) {
	if ( (ch == '\t') || (ch == '\177') ) {
	    small_strg [0] = ch;
	    small_strg [1] = '\0';
	    print_keys (small_strg, nl_pt);
	    return (YES);
	}
	return (NO);
    }

    if ( ch == '\033' ) {
	if (echo) fputs (escseqstrg (escp), stdout);
	if ( *idx ) *nl_pt = NO;
	*idx = 0;
	memset (escp, 0, sizeof (escp));
	escp [(*idx)++] = ch;
	return  (YES);
    }
    if ( ch < ' ' ) {
	if (echo) fputs (escseqstrg (escp), stdout);
	if ( *idx ) *nl_pt = NO;
	memset (escp, 0, sizeof (escp));
	*idx = 0;
	return (NO);
    }
    if ( *idx == 1 ) {
	if ( (ch == '[') || (ch == 'O') ) {
	    escp [(*idx)++] = ch;
	    return (YES);
	} else {
	    if (echo) fputs (escseqstrg (escp), stdout);
	    if ( *idx ) *nl_pt = NO;
	    memset (escp, 0, sizeof (escp));
	    *idx = 0;
	    return (NO);
	}
    }
    escp [(*idx)++] = ch;
    if ( (ch != '~') && !isalpha (ch) )
	return (YES);

    if ( assess_cursor_flg || assess_keypad_flg )
	assess_term_mode (escp);

    reverse_xterm_F1F4 (escp);
    print_keys (escp, nl_pt);
    *idx = 0;
    memset (escp, 0, sizeof (escp));
    return (YES);
}


#ifndef TEST_PROGRAM

static int sort_looktb (S_looktbl *obj1, S_looktbl *obj2)
{
    char *p1, *p2;
    int cmp;

    p1 = ( isalpha (obj1->str[0]) ) ? obj1->str : &obj1->str[1];
    p2 = ( isalpha (obj2->str[0]) ) ? obj2->str : &obj2->str[1];
    cmp = strcmp (p1, p2);
    if ( cmp == 0 ) cmp = strcmp (obj1->str, obj2->str);
    return (cmp);
}

static void looktbl_print (S_looktbl *tbl, char *line_pt, int idx)
{
    (void) sprintf (line_pt + 2, "%3d %-9s ", tbl[idx].val, tbl[idx].str);
}

static void ctrlkey_print (unsigned char *ctrl_asg, char *line_pt, int idx)
{
    extern char * itsyms_by_val (short);
    int i;

    i = idx * 3;
    (void) sprintf (line_pt, "<Ctrl %c> %s %s", ctrl_asg [i],
		itsyms_by_val (ctrl_asg [i+1]),
		( ctrl_asg [i+2] != CCUNAS1 ) ? itsyms_by_val (ctrl_asg [i+2]) : "");
}

/* print_sorted : routine to print in columns a list of strings */

static void print_sorted (tbl, print_func, item_nb, nb_clm, nb_chr, nb_spc)
void *tbl;
void (*print_func)();
int item_nb;    /* number of items in sorted array */
int nb_clm;     /* number of columns */
int nb_chr;     /* nb of charcaters printed for each item */
int nb_spc;     /* number of spaces between each item */

{
    char line[81];
    int lnb, lastln;
    int itemidx[16];
    int i, j, k, l, idx;
    int psiz;

    psiz = nb_chr + nb_spc;
    if ( (nb_spc < 1) || (nb_clm > 16)
	 || ((nb_clm * psiz) >= sizeof (line)) )
	return;

    lnb = item_nb / nb_clm;
    lastln = item_nb % nb_clm;
    for ( i = 0, k = 0 ; i < nb_clm ; i++ ) {
	itemidx[i] = k;
	k += ( i < lastln ) ? lnb +1 : lnb;
	}
    for ( i = 0 ; i <= lnb ; i ++ ) {
	memset (line, ' ', sizeof (line));
	l = (i == lnb) ? lastln : nb_clm;
	if ( l == 0 ) continue;
	for ( j = 0 ; j < l ; j ++ ) {
	    (* print_func) ((char *) tbl, &line[psiz*j], itemidx[j]);
	    itemidx[j] +=1;
	    }
	for ( idx = 0, k = sizeof (line) -1 ; k >= 0 ; k-- ) {
	    if ( line[k] == '\0' ) line[k] = ' ';
	    if ( (idx == 0) && (line[k] != ' ') ) idx = k+1;
	    }
	if ( idx == 0 ) idx = sizeof (line) -2;
	line[idx++] = '\n'; line[idx] = '\0';
	fputs (line, stdout);
	}
    return;
}

/* print the Control characters assignement */

static void display_ctrl ()
{
    extern char * itsyms_by_val (short);
    unsigned char ctrl_asg [128];    /* maxi is 32 control assigment */
    char ch, cmd[256];
    int i, idx, lnb, nb;

    /* get the assignement of control characters */
    memset (ctrl_asg, CCUNAS1, sizeof (ctrl_asg));
    idx = i = 0;
    for ( ch = 'A' ; ch <= 'Z' ; ch++ ) {
	cmd[0] = (ch & '\037'); cmd[1] = '\0';
	lnb = 1;
	nb = (*kbd.kb_inlex) (cmd, &lnb);
	if ( nb <= 0 ) continue;
	ctrl_asg [i++] = ch;
	ctrl_asg [i++] = cmd[0];
	if ( nb > 1 ) ctrl_asg [i] = cmd[1];
	i++;
	idx++;
    }
    printf ("\nControl Key assignement \n");
    print_sorted ((void *) ctrl_asg, ctrlkey_print, idx, 4, 19, 1);
    putchar ('\n');
}

/* linux_keymap : display the current key map */
/* ------------------------------------------ */

void display_keymap (Flag verbose)
{
    /* verbose flag must be used only for "-help -verbose" option case */
    extern S_term term;
    extern S_looktbl itsyms[];
    extern struct itable *ithead;
    int i, j, k, idx, nbnl;
    Key_Assign *kap;
    struct KTdesc *ktpt;
    int ktcode, cc, nb, cmd;
    int dispsz;
    char *shift_strg, *klabel;
    char *strg, strg1 [256], *cmdstrg, *cmdstrg0, *sp;

    dispsz = term.tt_height;
    if ( verbose ) dispsz = 100000;     /* very very large : do not hold the screen */
    if ( pc101_keyboard_struct [1].idx == 0 ) {
	for ( idx = k = 0 ; k < pc101_keyboard_struct_nb ; k++ ) {
	    pc101_keyboard_struct [k].idx = idx;
	    idx += pc101_keyboard_struct [k].nb;
	}
    }
    if ( ! verbose ) fputs ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", stdout);
    printf ("\n-- \"%s\" PC-101 style keyboard map, (%s) --", terminal_name, get_keyboard_mode_strg ());
    nbnl = 1;
    if ( Xterminal_flg ) {
	fputs ("\n    X terminal which use the output of \"xmodmap -pke\" to get the key mapping", stdout);
	fputs ("\n    The mapping of the plain keys is assumed to be 'regular' :", stdout);
	fputs ("\n        F1 mapped to F1 key ...", stdout);
	nbnl += 3;
    }
    if ( verbose ) {
	if ( Xterminal_flg ) {
	    fputs ("\n    X terminal which use the output of \"xmodmap -pke\" to get the key mapping", stdout);
	    fputs ("\n  Keyboard mapping is normaly defined by a call to \"xmodmap\" during X11 startup", stdout);
	    fputs ("\n    see \"xinitrc\" and \".Xmodmap\" in \"/etc/X11/xinit\".", stdout);
	    nbnl += 3;
	} else {
	    fputs ("\n  Keyboard mapping is normaly defined by a call to \"loadkeys\" during startup.", stdout);
	    nbnl += 1;
	}
	fputs ("\n  Ref \"The Linux Keyboard and Console HOWTO\" for more info.", stdout);
	nbnl += 1;

	printf ("\n    Initialisation string : %s", escseqstrg (init_msg));
	printf ("\n    For each key : key code (ref /usr/include/linux/keyboard.h),\n                  Escape Sequence, Rand key function");
	nbnl += 2;
    }
    for ( i = 0 ; i < nb_key ; i++ ) {
	for ( k = 0 ; k < pc101_keyboard_struct_nb ; k++ ) {
	    if ( pc101_keyboard_struct [k].idx != i ) continue;
	    if ( ! verbose ) {
		if ( (nbnl + pc101_keyboard_struct [k].nb) +4 > dispsz ) {
		    wait_keyboard ("\nPush a key to continue");
		    fputs ("\r                              ", stdout);
		    nbnl = 0;
		}
	    }
	    printf ("\n\n%-20s", pc101_keyboard_struct [k].label);
	    nbnl++;
	    for ( j = 0 ; j < nb_kbmap ; j++ ) {
		if ( keyboard_map [j] < 0 ) continue;
		if ( verbose ) printf ("                %-7s         ", key_shift_msg [j]);
		else printf ("%-7s      ", key_shift_msg [j]);
	    }
	    break;
	}
	kap = &pc_keyboard_desc[i];
	for ( j = 0 ; j < nb_kbmap ; j++ ) {
	    if ( keyboard_map [j] < 0 ) continue;

	    cmdstrg = NULL;
	    strg = kcode_to_string (kap->kcode, j,
				keypad_appl_mode, cursor_alt_mode,
				&ktcode, &shift_strg, &klabel);
	    if ( j == 0 ) {
		if ( Xterminal_flg ) {
		    if ( kap->kcode != kt_void )
			printf ("\n%+10s %3d :", klabel, kap->kcode);
		    else
			printf ("\n%+10s     :", klabel);
		}
		else printf ("\n%+14s :", klabel);
		nbnl++;
	    }
	    if ( verbose ) {
		if ( Xterminal_flg ) {
		    char strg1[24];
		    (void) get_xterm_kt_escseq (ktcode, &sp);
		    sprintf (strg1, "0x%4X %s", ktcode, (sp) ? sp : "");
		    printf (" ((%-19s) -> %-9s)", strg1, (strg) ? escseqstrg (strg) : "null");
		} else
		    printf (" ((%d,%3d) -> %-9s)", (ktcode/256), (ktcode%256), (strg) ? escseqstrg (strg) : "null");
	    }
	    if ( strg ) {
		nb = strlen (strg);
		cc = itget (&strg, &nb, ithead, strg1);
		if ( cc > 0  ) {
		    cmd = (unsigned char) strg1[0];
		    for ( k = 0 ; itsyms [k].str ; k++ ) {
			if ( itsyms [k].val != cmd ) continue;
			cmdstrg = itsyms [k].str;
			break;
		    }
		}
	    }
	    if ( j == 0 ) cmdstrg0 = cmdstrg;   /* default value for shift ... */
	    if ( ! cmdstrg0 ) cmdstrg0 = "";
	    if ( verbose ) printf ( " %-9s, ", (cmdstrg) ? cmdstrg : "");
	    else printf ( "%+10s, ", (cmdstrg) ? cmdstrg : cmdstrg0);
	}
    }
    putchar ('\n');

    if ( ! verbose ) {
	wait_keyboard ("Push a key to continue with Control keys");
	fputs ("\r                                              ", stdout);
	nbnl = 0;
    }
    display_ctrl ();
    if ( ! verbose )
	wait_keyboard ("Push a key to return to edition session");

    if ( verbose ) {
	/* display the defined Key Functions */
	S_looktbl *keyftable;
	int sz;
	for ( sz = 0 ; itsyms [sz].str ; sz++ ) ;
	keyftable = (S_looktbl *) calloc (sz, sizeof (keyftable[0]));
	if ( keyftable ) {
	    memcpy (keyftable, itsyms, sizeof (keyftable[0]) * sz);
	    qsort (keyftable, sz, sizeof (keyftable[0]),
		   (int (*)(const void *,const void *)) sort_looktb);
	    printf ("Defined (%d) Rand Key Functions\n", sz);
	    print_sorted (keyftable, looktbl_print, sz, 5, 14, 1);
	    putchar ('\n');
	    free (keyftable);
	}
    }

}
#endif /* - TEST_PROGRAM */


#ifdef TEST_PROGRAM
/* -------------------------------------------------------------------- */

/* Global variables for the autonomus keyboard test program */
/* -------------------------------------------------------- */

    Flag listing_flg, help_flg, acursor_flg, pkeypad_flg;
    Flag noinit_flg, mode_flg;

    static char def_init [] = "\033c";  /* reset */
    /* ------- for debuging of the init string parsing -----
    static char def_init [] = "\033c\033=\033[?1hTrikTrak\033c\033>\033[?1l";
    static char def_init [] = "\033=\033[2l";
    */

static char * init_strg = def_init;

static struct termios termiobf;


void prog_help (char *pname)
{
    int i;

    printf ("\
Synopsis : %s [option ...]\n\
    options :\n\
	-h   --help    : this on-line help\n\
	-l   --listing : listing of the keyboard mapping on standard output\n\
	-num --numkey  : set key pad in numeric mode\n\
	-app --appkey  : set key pad in application mode\n\
	-nor --norcur  : set in normal cursor mode\n\
	-alt --altcur  : set in alternate cursor mode\n\
	-n   --noinit  : do not set the terminal mode, use the current on\n\
	-init <init_string> : string to be used for terminal and keyboard init\n\
	    major init string :\n\
",
	    pname);

    for ( i = 0 ; i < init_seq_nb ; i++ )
	printf ("\
		%s : %s\n", init_seq[i].info, escseqstrg (init_seq[i].ref));
    printf ("\
\n\
Program to assess the keyboard mapping of Function and Key Pad keys\n\
    %s\n\n",
	    version);
    exit (1);
}

static void reset_term ()
{
    /* reset keyboard input mode */
    (void) tcsetattr (0, TCSANOW, &termiobf);

    /* reset the terminal in default mode */
    fputs (init_seq [key_pad_num].ref, stdout);
    fputs (init_seq [cursor_norm].ref, stdout);
    (void) fflush (stdout);
}

static void intr_handler (int signb)
{
    if ( signb != SIGINT ) printf ("Ended by signal %d\n", signb);
    reset_term ();
    exit (0);
}

static void program_option (int argc, char *argv[])
{
    static char user_init [128];
    int i, j, sz, v;
    char ch, *strg;

    listing_flg = help_flg = acursor_flg = pkeypad_flg = noinit_flg = mode_flg = NO;
    assess_cursor_flg = assess_keypad_flg = NO;
    if ( argc <= 1 ) return;

    for ( i = 1 ; i < argc ; i++ ) {
	strg = argv [i];
	if (   (strcasecmp ("-h", strg) == 0)
	    || (strcasecmp ("--help", strg) == 0) ) {
	    help_flg = YES;
	} else if ( (strcasecmp ("-l", strg) == 0)
	    || (strcasecmp ("--listing", strg) == 0) ) {
	    listing_flg = YES;
	} else if ( (strcasecmp ("-alt", strg) == 0)
	    || (strcasecmp ("--altcur", strg) == 0) ) {
	    mode_flg = acursor_flg = YES;
	} else if ( (strcasecmp ("-nor", strg) == 0)
	    || (strcasecmp ("--norcur", strg) == 0) ) {
	    mode_flg = YES;
	    acursor_flg = NO;
	} else if ( (strcasecmp ("-num", strg) == 0)
	    || (strcasecmp ("--numkey", strg) == 0) ) {
	    mode_flg = YES;
	    pkeypad_flg = NO;
	} else if ( (strcasecmp ("-app", strg) == 0)
	    || (strcasecmp ("--appkey", strg) == 0) ) {
	    mode_flg = pkeypad_flg = YES;
	} else if ( (strcasecmp ("-n", strg) == 0)
	    || (strcasecmp ("--noinit", strg) == 0) ) {
	    mode_flg = noinit_flg = YES;
	} else if ( (strcasecmp ("-init", strg) == 0)
	    || (strcasecmp ("--init", strg) == 0) ) {
	    mode_flg = noinit_flg = NO;
	    if ( (i+1) < argc ) {
		memset (user_init, 0, sizeof (user_init));
		strg = argv[++i];
		for ( sz = 0 ; ch = *strg ; strg++ ) {
		    if ( ch == '\\' ) {
			ch = *(++strg);
			if ( ! ch ) {
			    printf ("%s : invalid init string\n", argv[i]);
			    prog_help (argv[0]);
			}
			if ( isdigit (ch) ) {
			    /* the check is not righ enough : '8' and '9' and taken ! */
			    sscanf (strg, "%o%", &v);
			    ch = v & 0177;
			    for ( ; isdigit (*strg) ; strg++ ) ;
			    strg--;
			}
			/*
			else if ( ch == 'a' ) ch = '\a';
			else if ( ch == 'b' ) ch = '\b';
			else if ( ch == 'f' ) ch = '\f';
			else if ( ch == 'n' ) ch = '\n';
			else if ( ch == 'r' ) ch = '\r';
			else if ( ch == 't' ) ch = '\t';
			else if ( ch == 'v' ) ch = '\v';
			else if ( ch == '\\' ) ch = '\\';
			*/
			else switch (ch) {
			    case 'a' :
				ch = '\a';
				break;
			    case 'b' :
				ch = '\b';
				break;
			    case 'f' :
				ch = '\f';
				break;
			    case 'n' :
				ch = '\n';
				break;
			    case 'r' :
				ch = '\r';
				break;
			    case 't' :
				ch = '\t';
				break;
			    case 'v' :
				ch = '\v';
				break;
			    case '\\' :
				ch = '\\';
				break;
			}
		    }
		    user_init [sz++] = ch;
		}
		init_strg = user_init;
	    } else {
		printf ("%s : missing initialisation string\n", strg);
		prog_help (argv[0]);
	    }
	} else {
	    printf ("Unknow option : %s\n", strg);
	    prog_help (argv[0]);
	}
    }
}

main (int argc , char *argv[])
{
    static user_init [64];

    int i, j;
    Key_Assign *kap;
    char *st, *st1;
    struct KTdesc *ktpt;
    int ktcode, tmode;
    Flag app_mode, alt_cursor_mode;
    char *shift_strg, *klabel, *terminal;
    char *strg, *terminal;
    int lflag;

    /* read the console driver state */
    (void) tcgetattr (0, &termiobf);
    lflag = termiobf.c_lflag;
    termiobf.c_lflag &= ~(ICANON);
    (void) tcsetattr (0, TCSANOW, &termiobf);
    termiobf.c_lflag = lflag;

    program_option (argc, argv);
    if ( help_flg ) prog_help (argv[0]);

    signal (SIGINT, intr_handler);

    if ( ! mode_flg ) {
	strg = init_strg;
	app_mode = alt_cursor_mode = NO;
    } else {
	strg = NULL;
	app_mode = pkeypad_flg;
	alt_cursor_mode = acursor_flg;
    }
    if ( strg ) {
	printf ("%s\nInit string: %s\n", strg, escseqstrg (strg));
	}
    terminal = getenv ("TERM");
    get_kb_map (terminal, strg, &app_mode, &alt_cursor_mode);

    if ( mode_flg && noinit_flg ) {
	st = st1 = "?????";
	fputs ("\nTerminal mode is not set : use the current unknow mode.\n", stdout);
	fputs ("Push a \"Cursor Key\" and \"Key-Pad 5\" key to try to assess the current mode.\n", stdout);
	assess_cursor_flg = assess_keypad_flg = YES;
    } else {
	set_term_mode (app_mode, alt_cursor_mode, mode_flg);
	get_kbmode_strgs (&st, &st1);
    }
    printf ("\n--- %s terminal mode : %s key pad, %s cursor ----\n\n",
	    terminal, st, st1);

    if ( listing_flg ) {
	printf ("\n---- keyboard map ----\n");
	for ( i = 0 ; i < nb_key ; i++ ) {
	    kap = &pc_keyboard_desc[i];
	    for ( j = 0 ; j < nb_kbmap ; j++ ) {
		strg = kcode_to_string (kap->kcode, j, 1, 1, &ktcode, &shift_strg, &klabel);
		if ( j == 0 ) printf ("\nkcode %3d %+12s :", kap->kcode, klabel);
		if ( !strg ) continue;
		printf ( " %s 0x%03x, ",  shift_strg, ktcode);
		/*
		if ( kap->ktfunc[j] == -1 ) continue;
		printf ( " %s 0x%03x, ",  key_shift_msg[j], kap->ktfunc[j]);
		*/
	    }
	}

	printf ("\n\n---- Function strings ----\n");
	for ( i = 0 ; i < inuse_ktdesc_nb ; i++ ) {
	    printf ("  -- %d %s --\n", *(inuse_ktdesc [i].ktdesc_nb), inuse_ktdesc [i].type_name);
	    for ( j = 0 ; j < *(inuse_ktdesc [i].ktdesc_nb) ; j++ ) {
		ktpt = *(inuse_ktdesc [i].ktdesc_array) + j;
		if ( ktpt->ktfunc == 0 ) continue;
		if ( !ktpt->strg ) continue;
		st = escseqstrg (ktpt->strg);
		st1 = get_key_bycode (ktpt->ktfunc);
		printf ("%#05x %+10s = %-10s : %s\n",
			ktpt->ktfunc, (ktpt->tcap) ? ktpt->tcap : "", st, st1);
	    }
	}
	printf ("\n--------\n");
    }

    else {      /* interractive usage */
	checkkeyb (NO);
    }
    reset_term ();
}
#endif /* TEST_PROGRAM */
/* -------------------------------------------------------------------- */
