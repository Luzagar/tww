/**
 * d_a_bwds.cpp
 * Enemy - Molgera Larva
 */

#include "d/actor/d_a_bwds.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

/* 000000EC-00000144       .text __ct__12daBwds_HIO_cFv */
daBwds_HIO_c::daBwds_HIO_c() {
    /* Nonmatching */
}

/* 00000144-0000026C       .text anm_init__FP10bwds_classifUcfi */
void anm_init(bwds_class*, int, float, unsigned char, float, int) {
    /* Nonmatching */
}

/* 0000026C-000002E8       .text body_draw__FP10bwds_class */
void body_draw(bwds_class*) {
    /* Nonmatching */
}

/* 000002E8-000003E4       .text daBwds_Draw__FP10bwds_class */
static BOOL daBwds_Draw(bwds_class*) {
    /* Nonmatching */
}

/* 00000420-00000B5C       .text body_control__FP10bwds_class */
void body_control(bwds_class*) {
    /* Nonmatching */
}

/* 00000EDC-00001004       .text easy_bg_check2__FP10bwds_class */
void easy_bg_check2(bwds_class*) {
    /* Nonmatching */
}

/* 00001004-00001198       .text pos_move__FP10bwds_classs */
void pos_move(bwds_class*, short) {
    /* Nonmatching */
}

/* 00001198-000019CC       .text ug_move__FP10bwds_class */
void ug_move(bwds_class*) {
    /* Nonmatching */
}

/* 000019CC-00001B38       .text hook_on__FP10bwds_class */
void hook_on(bwds_class*) {
    /* Nonmatching */
}

/* 00001B38-00001DCC       .text hook_chance__FP10bwds_class */
void hook_chance(bwds_class*) {
    /* Nonmatching */
}

/* 00001DCC-00002178       .text fail__FP10bwds_class */
void fail(bwds_class*) {
    /* Nonmatching */
}

/* 000021B4-0000245C       .text damage_check__FP10bwds_class */
void damage_check(bwds_class*) {
    /* Nonmatching */
}

/* 0000245C-00002590       .text move__FP10bwds_class */
void move(bwds_class*) {
    /* Nonmatching */
}

/* 00002590-000031B4       .text daBwds_Execute__FP10bwds_class */
static BOOL daBwds_Execute(bwds_class*) {
    /* Nonmatching */
}

/* 000031B4-000031BC       .text daBwds_IsDelete__FP10bwds_class */
static BOOL daBwds_IsDelete(bwds_class*) {
    /* Nonmatching */
}

/* 000031BC-00003278       .text daBwds_Delete__FP10bwds_class */
static BOOL daBwds_Delete(bwds_class*) {
    /* Nonmatching */
}

/* 00003278-00003634       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 0000367C-000039A4       .text daBwds_Create__FP10fopAc_ac_c */
static cPhs_State daBwds_Create(fopAc_ac_c*) {
    /* Nonmatching */
}

static actor_method_class l_daBwds_Method = {
    (process_method_func)daBwds_Create,
    (process_method_func)daBwds_Delete,
    (process_method_func)daBwds_Execute,
    (process_method_func)daBwds_IsDelete,
    (process_method_func)daBwds_Draw,
};

actor_process_profile_definition g_profile_BWDS = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_BWDS,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(bwds_class),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_BWDS,
    /* Actor SubMtd */ &l_daBwds_Method,
    /* Status       */ fopAcStts_UNK40000_e | fopAcStts_UNK80000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
