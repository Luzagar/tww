/**
 * d_a_bwd.cpp
 * Boss - Molgera
 */

#include "d/actor/d_a_bwd.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

/* 000000EC-0000019C       .text __ct__11daBwd_HIO_cFv */
daBwd_HIO_c::daBwd_HIO_c() {
    /* Nonmatching */
}

/* 0000019C-000001B4       .text g_eff_on__FP9bwd_class */
void g_eff_on(bwd_class*) {
    /* Nonmatching */
}

/* 000001B4-000001CC       .text g_eff_off__FP9bwd_class */
void g_eff_off(bwd_class*) {
    /* Nonmatching */
}

/* 000001CC-00000250       .text ko_s_sub__FPvPv */
void ko_s_sub(void*, void*) {
    /* Nonmatching */
}

/* 00000250-0000029C       .text ko_delete_sub__FPvPv */
void ko_delete_sub(void*, void*) {
    /* Nonmatching */
}

/* 0000029C-000003C4       .text anm_init__FP9bwd_classifUcfi */
void anm_init(bwd_class*, int, float, unsigned char, float, int) {
    /* Nonmatching */
}

/* 000003C4-00000480       .text gr_draw__FP9bwd_class */
void gr_draw(bwd_class*) {
    /* Nonmatching */
}

/* 000004BC-00000564       .text suna_draw__FP9bwd_class */
void suna_draw(bwd_class*) {
    /* Nonmatching */
}

/* 00000564-00000760       .text daBwd_Draw__FP9bwd_class */
static BOOL daBwd_Draw(bwd_class*) {
    /* Nonmatching */
}

/* 00000760-00000FBC       .text fly_pos_move__FP9bwd_classss */
void fly_pos_move(bwd_class*, short, short) {
    /* Nonmatching */
}

/* 000013F4-00001640       .text damage_check__FP9bwd_class */
void damage_check(bwd_class*) {
    /* Nonmatching */
}

/* 0000167C-000018E8       .text start__FP9bwd_class */
void start(bwd_class*) {
    /* Nonmatching */
}

/* 000018E8-00001AEC       .text wait__FP9bwd_class */
void wait(bwd_class*) {
    /* Nonmatching */
}

/* 00001AEC-00001CBC       .text reset__FP9bwd_class */
void reset(bwd_class*) {
    /* Nonmatching */
}

/* 00001CBC-000026A4       .text sita_hit__FP9bwd_class */
void sita_hit(bwd_class*) {
    /* Nonmatching */
}

/* 000026A4-00002F30       .text eat_attack__FP9bwd_class */
void eat_attack(bwd_class*) {
    /* Nonmatching */
}

/* 00002F30-00003694       .text fly__FP9bwd_class */
void fly(bwd_class*) {
    /* Nonmatching */
}

/* 00003694-00003D3C       .text s_fly__FP9bwd_class */
void s_fly(bwd_class*) {
    /* Nonmatching */
}

/* 00003D3C-00004ABC       .text end__FP9bwd_class */
void end(bwd_class*) {
    /* Nonmatching */
}

/* 00004C54-0000527C       .text control1__FP9bwd_class */
void control1(bwd_class*) {
    /* Nonmatching */
}

/* 0000527C-00005438       .text control2__FP9bwd_class */
void control2(bwd_class*) {
    /* Nonmatching */
}

/* 00005438-00005D78       .text sita_move__FP9bwd_class */
void sita_move(bwd_class*) {
    /* Nonmatching */
}

/* 00005D78-00005DFC       .text move__FP9bwd_class */
void move(bwd_class*) {
    /* Nonmatching */
}

/* 00005DFC-00007320       .text demo_camera__FP9bwd_class */
void demo_camera(bwd_class*) {
    /* Nonmatching */
}

/* 00007320-000074A4       .text bwd_kankyo__FP9bwd_class */
void bwd_kankyo(bwd_class*) {
    /* Nonmatching */
}

/* 000074A4-00008C24       .text daBwd_Execute__FP9bwd_class */
static BOOL daBwd_Execute(bwd_class*) {
    /* Nonmatching */
}

/* 00008C24-00008C2C       .text daBwd_IsDelete__FP9bwd_class */
static BOOL daBwd_IsDelete(bwd_class*) {
    /* Nonmatching */
}

/* 00008C2C-00008E0C       .text daBwd_Delete__FP9bwd_class */
static BOOL daBwd_Delete(bwd_class*) {
    /* Nonmatching */
}

/* 00008E0C-000096D8       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 00009720-00009C7C       .text daBwd_Create__FP10fopAc_ac_c */
static cPhs_State daBwd_Create(fopAc_ac_c*) {
    /* Nonmatching */
}

static actor_method_class l_daBwd_Method = {
    (process_method_func)daBwd_Create,
    (process_method_func)daBwd_Delete,
    (process_method_func)daBwd_Execute,
    (process_method_func)daBwd_IsDelete,
    (process_method_func)daBwd_Draw,
};

actor_process_profile_definition g_profile_BWD = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_BWD,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(bwd_class),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_BWD,
    /* Actor SubMtd */ &l_daBwd_Method,
    /* Status       */ fopAcStts_UNK4000_e | fopAcStts_UNK40000_e | fopAcStts_BOSS_e,
    /* Group        */ fopAc_ENEMY_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
