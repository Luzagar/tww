/**
 * d_a_ep.cpp
 * Object - Torches
 */

#include "d/actor/d_a_ep.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

/* 00000078-00000158       .text ga_draw__FP8ep_class */
void ga_draw(ep_class*) {
    /* Nonmatching */
}

/* 00000158-000001F8       .text ep_draw__FP8ep_class */
void ep_draw(ep_class*) {
    /* Nonmatching */
}

/* 000001F8-000002E0       .text daEp_Draw__FP8ep_class */
static BOOL daEp_Draw(ep_class*) {
    /* Nonmatching */
}

/* 000002E0-00000344       .text ep_switch_event_end__FP8ep_class */
void ep_switch_event_end(ep_class*) {
    /* Nonmatching */
}

/* 00000344-000003E8       .text ep_switch_event_begin__FP8ep_class */
void ep_switch_event_begin(ep_class*) {
    /* Nonmatching */
}

/* 000003E8-000004C4       .text ep_switch_event_move__FP8ep_class */
void ep_switch_event_move(ep_class*) {
    /* Nonmatching */
}

/* 000004C4-00000B48       .text ga_move__FP8ep_class */
void ga_move(ep_class*) {
    /* Nonmatching */
}

/* 00000F80-0000173C       .text ep_move__FP8ep_class */
void ep_move(ep_class*) {
    /* Nonmatching */
}

/* 0000173C-00001834       .text daEp_set_mtx__FP8ep_class */
void daEp_set_mtx(ep_class*) {
    /* Nonmatching */
}

/* 00001834-00001E30       .text daEp_Execute__FP8ep_class */
static BOOL daEp_Execute(ep_class*) {
    /* Nonmatching */
}

/* 00002270-00002278       .text daEp_IsDelete__FP8ep_class */
static BOOL daEp_IsDelete(ep_class*) {
    /* Nonmatching */
}

/* 00002278-000022D4       .text daEp_Delete__FP8ep_class */
static BOOL daEp_Delete(ep_class*) {
    /* Nonmatching */
}

/* 000022D4-00002518       .text daEp_CreateHeap__FP10fopAc_ac_c */
static BOOL daEp_CreateHeap(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 00002518-00002608       .text daEp_CreateInit__FP10fopAc_ac_c */
void daEp_CreateInit(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 00002608-00002840       .text daEp_Create__FP10fopAc_ac_c */
static cPhs_State daEp_Create(fopAc_ac_c*) {
    /* Nonmatching */
}

static actor_method_class l_daEp_Method = {
    (process_method_func)daEp_Create,
    (process_method_func)daEp_Delete,
    (process_method_func)daEp_Execute,
    (process_method_func)daEp_IsDelete,
    (process_method_func)daEp_Draw,
};

actor_process_profile_definition g_profile_EP = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_EP,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(ep_class),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_EP,
    /* Actor SubMtd */ &l_daEp_Method,
    /* Status       */ fopAcStts_UNK4000_e | fopAcStts_UNK40000_e | fopAcStts_UNK200000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
