


struct string {
    char name;
    char *sbuf;
 };

struct strings {
    struct string s_header;
    struct string s_trailer;
    struct string s_res1;
    struct string s_enq;
    struct string s_da;
    struct string s_null;
 };

extern struct strings ini_strings;

struct keys {
    struct string k_send     ;
    struct string k_reset    ;
    struct string k_setup    ;
    struct string k_break    ;
    struct string k_shbreak  ;
    struct string k_pause    ;
 /* struct string k_shpause  ; */
    struct string k_return   ;
    struct string k_mvup     ;
    struct string k_shmvup   ;
    struct string k_mvdown   ;
    struct string k_shmvdown ;
    struct string k_zero     ;
    struct string k_one      ;
    struct string k_two      ;
    struct string k_three    ;
    struct string k_four     ;
    struct string k_five     ;
    struct string k_six      ;
    struct string k_seven    ;
    struct string k_eight    ;
    struct string k_nine     ;
    struct string k_period   ;
    struct string k_tab      ;
    struct string k_enter    ;
    struct string k_shtab    ;
    struct string k_erase    ;
    struct string k_sherase  ;
    struct string k_edit     ;
    struct string k_delete   ;
    struct string k_shdelete ;
    struct string k_insert   ;
    struct string k_shinsert ;
 /* struct string k_print    ; */
    struct string k_shprint  ;
    struct string k_ctlsh7   ;
    struct string k_pf1      ;
    struct string k_pf2      ;
    struct string k_pf3      ;
    struct string k_pf4      ;
    struct string k_pf5      ;
    struct string k_pf6      ;
    struct string k_pf7      ;
    struct string k_pf8      ;
    struct string k_pf9      ;
    struct string k_pf10     ;
    struct string k_pf11     ;
    struct string k_pf12     ;
    struct string k_shpf1    ;
    struct string k_shpf2    ;
    struct string k_shpf3    ;
    struct string k_shpf4    ;
    struct string k_shpf5    ;
    struct string k_shpf6    ;
    struct string k_shpf7    ;
    struct string k_shpf8    ;
    struct string k_shpf9    ;
    struct string k_shpf10   ;
    struct string k_shpf11   ;
    struct string k_shpf12   ;
    struct string k_null     ;
 };

extern struct keys ini_keys;
