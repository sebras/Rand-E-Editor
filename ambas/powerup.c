
#include "ambas.h"

struct strings ini_strings = {
    {' ', "" },                 /* s_header  Header string   */
    {'!', "" },                 /* s_trailer Trailer string  */
    {'"', "" },                 /* s_res1    [Reserved]      */
    {'#', "" },                 /* s_enq     ENQ string      */
    {'$', "~[[>01;07c" },       /* s_da      DA string       */
    {'\0' }
 };

struct keys ini_keys = {
    {'%', "y~[S"     },          /* k_send      Send                    */
    {'&', ""         },          /* k_reset     Reset ???               */
    {'\'',"y~[{D}"   },          /* k_setup     Setup                   */
    {'(', "y~[{E}"   },          /* k_break     Break                   */
    {')', "y~[{F}"   },          /* k_shbreak   Break Shift             */
    {'*', "y~[{G}"   },          /* k_pause     Pause                   */
 /* {???, "y~[{H}"   },          /* k_shpause   Pause Shift ???         */
    {'+', "~M"       },          /* k_return    Return                  */
    {',', "xy~[[T"   },          /* k_mvup      Move Up         SD      */
    {'-', "y~[{I}"   },          /* k_shmvup    Move Up Shift           */
    {'.', "xy~[[S"   },          /* k_mvdown    Move Down       SU      */
    {'/', "y~[{J}"   },          /* k_shmvdown  Move Down Shift         */
    {'0', ""         },          /* k_zero      0 Shift                 */
    {'1', "~[F"      },          /* k_one       1 Shift         SSA     */
    {'2', "x~[[B"    },          /* k_two       2 Shift                 */
    {'3', "~[G"      },          /* k_three     3 Shift                 */
    {'4', "x~[[D"    },          /* k_four      4 Shift                 */
    {'5', "~[[H"     },          /* k_five      5 Shift                 */
    {'6', "x~[[C"    },          /* k_six       6 Shift                 */
    {'7', "~[[{G}"   },          /* k_seven     7 Shift                 */
    {'8', "x~[[A"    },          /* k_eight     8 Shift                 */
    {'9', "~[H"      },          /* k_nine      9 Shift                 */
    {':', "."        },          /* k_period    Period                  */
    {';', "~I"       },          /* k_tab       Tab                     */
    {'<', "~M"       },          /* k_enter     Enter                   */
    {'=', "~[[Z"     },          /* k_shtab     Tab Shift               */
    {'>', "~[[K"     },          /* k_erase     Erase                   */
    {'?', "~[[J"     },          /* k_sherase   Erase Shift             */
    {'@', "~[6"      },          /* k_edit      Edit                    */
    {'A', "~[[P"     },          /* k_delete    Delete                  */
    {'B', "~[[M"     },          /* k_shdelete  Delete Shift            */
    {'C', "~[[@"     },          /* k_insert    Insert                  */
    {'D', "~[[L"     },          /* k_shinsert  Insert Shift            */
 /* {'E', ""         }, /* ???   /* k_print     Print                   */
    {'F', "y~[{K}"   },          /* k_shprint   Print Shift             */
    {'G', "~[3{G}"   },          /* k_ctlsh7    7 Ctrl Shift            */
    {'H', ""         },          /* k_pf1       PF1             (null)  */
    {'I', ""         },          /* k_pf2       PF2             (null)  */
    {'J', ""         },          /* k_pf3       PF3             (null)  */
    {'K', ""         },          /* k_pf4       PF4             (null)  */
    {'L', ""         },          /* k_pf5       PF5             (null)  */
    {'M', ""         },          /* k_pf6       PF6             (null)  */
    {'N', ""         },          /* k_pf7       PF7             (null)  */
    {'O', ""         },          /* k_pf8       PF8             (null)  */
    {'P', ""         },          /* k_pf9       PF9             (null)  */
    {'Q', ""         },          /* k_pf10      PF10            (null)  */
    {'R', ""         },          /* k_pf11      PF11            (null)  */
    {'S', ""         },          /* k_pf12      PF12            (null)  */
    {'T', ""         },          /* k_shpf1     PF1   Shift     (null)  */
    {'U', ""         },          /* k_shpf2     PF2   Shift     (null)  */
    {'V', ""         },          /* k_shpf3     PF3   Shift     (null)  */
    {'W', ""         },          /* k_shpf4     PF4   Shift     (null)  */
    {'X', ""         },          /* k_shpf5     PF5   Shift     (null)  */
    {'Y', ""         },          /* k_shpf6     PF6   Shift     (null)  */
    {'Z', ""         },          /* k_shpf7     PF7   Shift     (null)  */
    {'[', ""         },          /* k_shpf8     PF8   Shift     (null)  */
    {'\\',""         },          /* k_shpf9     PF9   Shift     (null)  */
    {']', ""         },          /* k_shpf10    PF10  Shift     (null)  */
    {'^', ""         },          /* k_shpf11    PF11  Shift     (null)  */
    {'_', ""         },          /* k_shpf12    PF12  Shift     (null)  */
    {'\0',           }
 };
