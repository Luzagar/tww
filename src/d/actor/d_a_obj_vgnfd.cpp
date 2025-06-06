/**
 * d_a_obj_vgnfd.cpp
 * Object - Ganon's Tower - Trials door
 */

#include "d/actor/d_a_obj_vgnfd.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_com_inf_game.h"
#include "d/res/res_vgnfd.h"
#include "JSystem/JUtility/JUTAssert.h"

#include "weak_data_1811.h" // IWYU pragma: keep

const s32 daObjVgnfd_c::M_bdl_table[] = {
    VGNFD_BDL_VGNFD0,
    VGNFD_BDL_VGNFD1,
    VGNFD_BDL_VGNFD2,
    VGNFD_BDL_VGNFD3,
    VGNFD_BDL_VGNFD4,
};
const s32 daObjVgnfd_c::M_brk_table[] = {
    VGNFD_BRK_VGNFD0,
    VGNFD_BRK_VGNFD1,
    VGNFD_BRK_VGNFD2,
    VGNFD_BRK_VGNFD3,
    VGNFD_BRK_VGNFD4,
};
const s32 daObjVgnfd_c::M_door_bdl_table[] = {
    VGNFD_BDL_VGNFD5,
    VGNFD_BDL_YGCBD00,
};
const u16 daObjVgnfd_c::M_door_ev_table[] = {
    0x3904, 0x3902, 0x3901, 0x3A80,
};
const u16 daObjVgnfd_c::M_boss_ev_table[] = {
    0x3240, 0x3220, 0x3210, 0x3208,
};
const char daObjVgnfd_c::M_arcname[6] = "VgnFD";

static const char* l_daObjVgnfd_demo_name[] = {
    "4_door_dn",
    "4_door_mr",
    "4_door_dc",
    "4_door_kz",
};

static char* l_daObjVgnfd_act_name[] = {
    "wait0",
    "wait1",
};

static char* l_daObjVgnfd_break_act_name[] = {
    "wait0",
    "gannon",
    "vib0",
    "vib1",
    "break",
    "burst",
};

static u16 l_daObjVgnfd_pt_table[] = {
    dPa_name::ID_SCENE_833E,
    dPa_name::ID_SCENE_833F,
    dPa_name::ID_SCENE_8340,
    dPa_name::ID_SCENE_8341,
    dPa_name::ID_SCENE_8342,
    dPa_name::ID_SCENE_8343,
};

enum {
    STATE_WAIT,
    STATE_DEMO,
    STATE_DEMO_FIN,
};

/* 00000078-00000098       .text solidHeapCB__12daObjVgnfd_cFP10fopAc_ac_c */
BOOL daObjVgnfd_c::solidHeapCB(fopAc_ac_c* i_ac) {
    return ((daObjVgnfd_c*)i_ac)->create_heap();
}

/* 00000098-0000022C       .text create_bdl_brk__12daObjVgnfd_cFi */
BOOL daObjVgnfd_c::create_bdl_brk(int i) {
    BOOL ret = FALSE;
    J3DModelData* mdl_data = static_cast<J3DModelData*>(dComIfG_getObjectRes(M_arcname, M_bdl_table[i]));
    JUT_ASSERT(0xfe, mdl_data != NULL);
    if (mdl_data != NULL) {
        mModel[i] = mDoExt_J3DModel__create(mdl_data, 0, 0x11020203);
        if (mModel[i] != NULL) {
            if (M_brk_table[i] != -1) {
                J3DAnmTevRegKey* brk_p = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(M_arcname, M_brk_table[i]));
                JUT_ASSERT(0x105, brk_p != NULL);
                if (brk_p != NULL) {
                    if (mBrkAnm[i].init(mdl_data, brk_p, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE))
                        ret = TRUE;
                }
            } else {
                ret = TRUE;
            }
        }
    }
    return ret;
}

/* 0000022C-000004A4       .text create_heap__12daObjVgnfd_cFv */
BOOL daObjVgnfd_c::create_heap() {
    BOOL ret = TRUE;
    s32 i;
    for (i = 0; i < (s32)ARRAY_SIZE(mModel); i++) {
        if (!create_bdl_brk(i)) {
            ret = FALSE;
            break;
        }
    }

    if (ret) {
        for (i = 0; i < (s32)ARRAY_SIZE(mModel2); i++) {
            J3DModelData* mdl_data = static_cast<J3DModelData*>(dComIfG_getObjectRes(M_arcname, M_door_bdl_table[i]));
            JUT_ASSERT(0x133, mdl_data != NULL);

            if (mdl_data != NULL) {
                mModel2[i] = mDoExt_J3DModel__create(mdl_data, 0, 0x11020203);
                if (mModel2[i] == NULL) {
                    ret = FALSE;
                    break;
                }
            } else {
                ret = FALSE;
                break;
            }
        }
    }

    if (ret) {
        J3DAnmTextureSRTKey* btk_data = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(M_arcname, VGNFD_BTK_YGCBD00));
        JUT_ASSERT(0x144, btk_data != NULL);

        if (btk_data == NULL || !mBtkAnm.init(mModel2[1]->getModelData(), btk_data, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE)) {
            ret = FALSE;
        }
    }

    if (ret) {
        Mtx* mtx = &mModel[0]->getBaseTRMtx();
        cBgD_t* dzb_data = (cBgD_t*)dComIfG_getObjectRes(M_arcname, VGNFD_DZB_VGNFD);
        M_bgw = dBgW_NewSet(dzb_data, dBgW::MOVE_BG_e, mtx);
        JUT_ASSERT(0x151, M_bgw != NULL);

        if (M_bgw == NULL)
            ret = FALSE;
    }

    return ret;
}

/* 000004A4-00000704       .text _create__12daObjVgnfd_cFv */
cPhs_State daObjVgnfd_c::_create() {
    cPhs_State ret = cPhs_ERROR_e;

    fopAcM_SetupActor(this, daObjVgnfd_c);

    if (!check_fin()) {
        ret = dComIfG_resLoad(&mPhs, M_arcname);
        if (ret == cPhs_COMPLEATE_e) {
            ret = cPhs_ERROR_e;
            if (fopAcM_entrySolidHeap(this, solidHeapCB, 0)) {
                fopAcM_SetMtx(this, mModel[0]->getBaseTRMtx());
                init_mtx();
                mBrkAnm[0].setPlaySpeed(0.0f);

                for (s32 i = 1; i < 5; i++) {
                    mBrkAnm[i].setPlaySpeed(0.0f);
                    if (check_ev_init(i - 1)) {
                        mBrkAnm[i].setFrame(mBrkAnm[i].getEndFrame());
                    }
                }

                mBtkAnm.setPlaySpeed(0.0f);
                M_demo_idx = get_start_demo_idx();
                if (M_demo_idx != -1) {
                    mState = STATE_DEMO;
                    mEventIdx = dComIfGp_evmng_getEventIdx(l_daObjVgnfd_demo_name[M_demo_idx]);
                }

                fopAcM_setCullSizeBox(this, -260.0f, -10.0f, -50.0f, 260.0f, 510.0f, 100.0f);
                mSmoke.setTevStr(&tevStr);
                mSmoke.setRateOff(0);
                mSmoke.onWindOff();
                mSmoke.setFollowOff();

                dComIfG_Bgsp()->Regist(M_bgw, this);
                M_bgw->Move();
                mInit = true;
                ret = cPhs_COMPLEATE_e;
            }
        }
    }

    return ret;
}

/* 00000830-000008D8       .text _delete__12daObjVgnfd_cFv */
bool daObjVgnfd_c::_delete() {
    if (heap != NULL) {
        if (M_bgw != NULL && M_bgw->ChkUsed()) {
            dComIfG_Bgsp()->Release(M_bgw);
            M_bgw = NULL;
        }
    }

    mSmoke.remove();
    dComIfG_resDelete(&mPhs, M_arcname);
    return true;    
}

/* 000008D8-00000914       .text check_ev_init__12daObjVgnfd_cFi */
BOOL daObjVgnfd_c::check_ev_init(int i) {
    return dComIfGs_isEventBit(M_door_ev_table[i]);
}

void daObjVgnfd_c::on_door_ev(int i) {
    dComIfGs_onEventBit(M_door_ev_table[i]);
}

BOOL daObjVgnfd_c::check_boss(int i) {
    return dComIfGs_isEventBit(M_boss_ev_table[i]);
}

/* 00000914-000009AC       .text get_start_demo_idx__12daObjVgnfd_cFv */
s32 daObjVgnfd_c::get_start_demo_idx() {
    s32 ret = -1;
    
    for (s32 i = 0; i < (s32)ARRAY_SIZE(M_door_ev_table); i++) {
        if (!check_ev_init(i) && check_boss(i)) {
            ret = i;
            break;
        }
    }

    return ret;
}

/* 000009AC-00000A28       .text check_fin__12daObjVgnfd_cFv */
BOOL daObjVgnfd_c::check_fin() {
    s32 i;
    BOOL ret = TRUE;

    for (i = 0; i < (s32)ARRAY_SIZE(M_door_ev_table); i++) {
        if (!check_ev_init(i)) {
            ret = FALSE;
            break;
        }
    }

    return ret;
}

/* 00000A28-00000A58       .text on_fin__12daObjVgnfd_cFv */
void daObjVgnfd_c::on_fin() {
    dComIfGs_onEventBit(0x3204);
}

/* 00000A58-00000B3C       .text init_mtx__12daObjVgnfd_cFv */
void daObjVgnfd_c::init_mtx() {
    mModel[0]->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    for (s32 i = 0; i < (s32)ARRAY_SIZE(mModel); i++)
        mModel[i]->setBaseTRMtx(mDoMtx_stack_c::get());
    for (s32 i = 0; i < (s32)ARRAY_SIZE(mModel2); i++)
        mModel2[i]->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000B3C-00000BA0       .text set_timer__12daObjVgnfd_cFv */
void daObjVgnfd_c::set_timer() {
    int* int_p = dComIfGp_evmng_getMyIntegerP(mStaffId, "Timer");
    mTimer = 0;
    if (int_p != NULL)
        mTimer = *int_p;
}

/* 00000BA0-00001298       .text _execute__12daObjVgnfd_cFv */
bool daObjVgnfd_c::_execute() {
    BOOL done = FALSE;

    switch (mState) {
    case STATE_WAIT:
        break;
    case STATE_DEMO:
        JUT_ASSERT(0x253, M_demo_idx != -1);
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderOtherEventId(this, mEventIdx);
            eventInfo.onCondition(dEvtCnd_UNK2_e);
        } else {
            if (dComIfGp_evmng_endCheck(mEventIdx)) {
                dComIfGp_event_reset();
                if (check_fin()) {
                    mEventIdx = dComIfGp_evmng_getEventIdx("4_door_fin");
                    fopAcM_orderOtherEventId(this, mEventIdx);
                    eventInfo.onCondition(dEvtCnd_UNK2_e);
                    mState = STATE_DEMO_FIN;
                } else {
                    mState = STATE_WAIT;
                }
            } else {
                mStaffId = dComIfGp_evmng_getMyStaffId(M_arcname);
                if (mStaffId != -1) {
                    enum {
                        ACT_WAIT0,
                        ACT_WAIT1,
                    };

                    s32 actIdx = dComIfGp_evmng_getMyActIdx(mStaffId, l_daObjVgnfd_act_name, ARRAY_SIZE(l_daObjVgnfd_act_name), FALSE, 0);
                    if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
                        switch (actIdx) {
                        case ACT_WAIT0:
                            on_door_ev(M_demo_idx);
                            set_timer();
                            break;
                        case ACT_WAIT1:
                            break;
                        }
                    }

                    switch (actIdx) {
                    case ACT_WAIT0:
                        mTimer--;
                        if (mTimer <= 0) {
                            dComIfGp_evmng_cutEnd(mStaffId);
                            mBrkAnm[M_demo_idx + 1].setPlaySpeed(1.0f);
                            mDoAud_seStart(JA_SE_OBJ_B_BOSS_DR_LT_1);
                            if (check_fin()) {
                                mDoAud_seStart(JA_SE_READ_RIDDLE_1);
                            }
                        }
                        break;
                    case ACT_WAIT1:
                        break;
                    }
                }
            }
        }
        break;
    case STATE_DEMO_FIN:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderOtherEventId(this, mEventIdx);
            eventInfo.onCondition(dEvtCnd_UNK2_e);
        } else {
            if (dComIfGp_evmng_endCheck(mEventIdx)) {
                dComIfGp_event_reset();
                on_fin();
                done = TRUE;
                mState = STATE_WAIT;
            } else {
                mStaffId = dComIfGp_evmng_getMyStaffId(M_arcname);
                if (mStaffId != -1) {
                    enum {
                        ACT_WAIT0,
                        ACT_GANNON,
                        ACT_VIB0,
                        ACT_VIB1,
                        ACT_BREAK,
                        ACT_BURST,
                    };

                    s32 actIdx = dComIfGp_evmng_getMyActIdx(mStaffId, l_daObjVgnfd_break_act_name, ARRAY_SIZE(l_daObjVgnfd_break_act_name), FALSE, 0);
                    if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
                        set_timer();
                        switch (actIdx) {
                        case ACT_WAIT0:
                            break;
                        case ACT_GANNON:
                            mBrkAnm[0].setPlaySpeed(1.0f);
                            for (s32 i = 1; i < ARRAY_SIZE(mBrkAnm); i++)
                                mBrkAnm[i].setPlaySpeed(-1.0f);
                            break;
                        case ACT_VIB0:
                            dComIfGp_getVibration().StartShock(8, 23, cXyz(0.0f, 1.0f, 0.0f));
                            fopAcM_seStartCurrent(this, JA_SE_OBJ_B_BOSS_DR_BRK_1, 0);
                            break;
                        case ACT_VIB1:
                            dComIfGp_getVibration().StartQuake(2, 11, cXyz(0.0f, 1.0f, 0.0f));
                            break;
                        case ACT_BREAK:
                            mCurModel = 1;
                            mBtkAnm.setPlaySpeed(1.0f);
                            for (s32 i = 0; i < (s32)ARRAY_SIZE(l_daObjVgnfd_pt_table); i++) {
                                JPABaseEmitter* emtr = dComIfGp_particle_set(l_daObjVgnfd_pt_table[i], &current.pos, &current.angle, &scale);
                                if (emtr != NULL) {
                                    emtr->setGlobalPrmColor(tevStr.mColorK0.r, tevStr.mColorK0.g, tevStr.mColorK0.b);
                                }
                            }
                            dComIfGp_particle_setToon(dPa_name::ID_SCENE_A344, &current.pos, &current.angle, &scale, 0xA0, &mSmoke);
                            dComIfGp_getVibration().StartQuake(4, 11, cXyz(0.0f, 1.0f, 0.0f));
                            fopAcM_seStartCurrent(this, JA_SE_OBJ_B_BOSS_DR_BRK_2, 0);
                            break;
                        case ACT_BURST:
                            dComIfGp_getVibration().StopQuake(-1);
                            dComIfGp_getVibration().StartShock(8, 27, cXyz(0.0f, 1.0f, 0.0f));
                            mCurModel = 2;
                            fopAcM_seStartCurrent(this, JA_SE_OBJ_B_BOSS_DR_BRK_3, 0);
                        }
                    }

                    if (mTimer > 0)
                        mTimer--;

                    switch (actIdx) {
                    case ACT_BURST:
                        break;
                    case ACT_WAIT0:
                    case ACT_GANNON:
                    case ACT_VIB0:
                    case ACT_VIB1:
                    case ACT_BREAK:
                        if (mTimer <= 0)
                            dComIfGp_evmng_cutEnd(mStaffId);
                        break;
                    }
                }
            }
        }
        break;
    }

    for (s32 i = 0; i < (s32)ARRAY_SIZE(mBrkAnm); i++)
        mBrkAnm[i].play();
    mBtkAnm.play();
    if (done)
        fopAcM_delete(this);
    return TRUE;
}

/* 00001298-000013F0       .text _draw__12daObjVgnfd_cFv */
bool daObjVgnfd_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    if (mCurModel == 0) {
        g_env_light.setLightTevColorType(mModel2[mCurModel], &tevStr);
        mDoExt_modelUpdateDL(mModel2[mCurModel]);
        for (s32 i = 4; i >= 0; i--) {
            g_env_light.setLightTevColorType(mModel[i], &tevStr);
            if (M_brk_table[i] != -1)
                mBrkAnm[i].entry(mModel[i]->getModelData());
            mDoExt_modelUpdateDL(mModel[i]);
        }
    } else if (mCurModel == 1) {
        g_env_light.setLightTevColorType(mModel2[mCurModel], &tevStr);
        mBtkAnm.entry(mModel2[mCurModel]->getModelData());
        mDoExt_modelUpdateDL(mModel2[mCurModel]);
    }
    return true;
}

namespace {
/* 000013F0-00001410       .text Mthd_Create__27@unnamed@d_a_obj_vgnfd_cpp@FPv */
cPhs_State Mthd_Create(void* i_ac) {
    return ((daObjVgnfd_c*)i_ac)->_create();
}

/* 00001410-00001434       .text Mthd_Delete__27@unnamed@d_a_obj_vgnfd_cpp@FPv */
BOOL Mthd_Delete(void* i_ac) {
    return ((daObjVgnfd_c*)i_ac)->_delete();
}

/* 00001434-00001458       .text Mthd_Execute__27@unnamed@d_a_obj_vgnfd_cpp@FPv */
BOOL Mthd_Execute(void* i_ac) {
    return ((daObjVgnfd_c*)i_ac)->_execute();
}

/* 00001458-0000147C       .text Mthd_Draw__27@unnamed@d_a_obj_vgnfd_cpp@FPv */
BOOL Mthd_Draw(void* i_ac) {
    return ((daObjVgnfd_c*)i_ac)->_draw();
}

/* 0000147C-00001484       .text Mthd_IsDelete__27@unnamed@d_a_obj_vgnfd_cpp@FPv */
BOOL Mthd_IsDelete(void* i_ac) {
    return TRUE;
}

static actor_method_class Vgnfd_Mthd_Table = {
    (process_method_func)Mthd_Create,
    (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute,
    (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
}; // namespace

actor_process_profile_definition g_profile_Obj_Vgnfd = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Vgnfd,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjVgnfd_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Vgnfd,
    /* Actor SubMtd */ &Vgnfd_Mthd_Table,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
