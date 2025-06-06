/**
 * d_a_obj_vfan.cpp
 * Object - Ganon's Tower - Phantom Ganon door
 */

#include "d/actor/d_a_obj_vfan.h"
#include "d/res/res_vfan.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "m_Do/m_Do_mtx.h"

namespace daObjVfan {
s16 m_evid;
}

Mtx daObjVfan::Act_c::M_tmp_mtx;
const char daObjVfan::Act_c::M_arcname[] = "Vfan";

static dCcD_SrcCyl cyl_check_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ AT_TYPE_PGANON_SWORD,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ 0,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ 0,
        /* SrcGObjAt Spl     */ 0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ 0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ 0,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCylS
    {
        /* Center */ 0.0f, 0.0f, 0.0f,
        /* Radius */ 100.0f,
        /* Height */ 300.0f,
    }
};

/* 00000078-00000134       .text CreateHeap__Q29daObjVfan5Act_cFv */
BOOL daObjVfan::Act_c::CreateHeap() {
    J3DModelData* model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes(M_arcname, VFAN_BDL_V_FAN_00));

    JUT_ASSERT(0x8c, model_data != NULL);

    mpModel = mDoExt_J3DModel__create(model_data, 0, 0x11020203);

    if (mpModel == NULL) {
        return false;
    }
    return true;
}

/* 00000134-00000214       .text Create__Q29daObjVfan5Act_cFv */
BOOL daObjVfan::Act_c::Create() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

    init_mtx();

    fopAcM_setCullSizeBox(this, -4000.0f, -500.0f, -4000.0f, 4000.0f, 500.0f, 4000.0f);

    mStts.Init(0xff, 0xff, this);
    mCyl.Set(cyl_check_src);
    mCyl.SetC(current.pos);
    mCyl.SetStts(&mStts);

    mIsAlive = true;
    mBreakTimer = 0;
    mState = 0;

    m_evid = dComIfGp_evmng_getEventIdx("Vfan");

    return TRUE;
}

/* 00000214-000003D0       .text Mthd_Create__Q29daObjVfan5Act_cFv */
cPhs_State daObjVfan::Act_c::Mthd_Create() {
    fopAcM_SetupActor(this, daObjVfan::Act_c);

    cPhs_State phase_state;
    if (fopAcM_isSwitch(this, prm_get_swSave())) {
        return cPhs_STOP_e;
    } else {
        phase_state = dComIfG_resLoad(&mPhs, M_arcname);
        if (phase_state == cPhs_COMPLEATE_e) {
            phase_state = MoveBGCreate(M_arcname, VFAN_DZB_V_FAN_00, NULL, 0xa60);

            JUT_ASSERT(0xc6, (phase_state == cPhs_COMPLEATE_e) || (phase_state == cPhs_ERROR_e));
        }
    }

    return phase_state;
}

/* 00000588-00000590       .text Delete__Q29daObjVfan5Act_cFv */
BOOL daObjVfan::Act_c::Delete() {
    return TRUE;
}

/* 00000590-000005E8       .text Mthd_Delete__Q29daObjVfan5Act_cFv */
BOOL daObjVfan::Act_c::Mthd_Delete() {
    int res = MoveBGDelete();
    if (fpcM_CreateResult(this) != cPhs_STOP_e) {
        dComIfG_resDeleteDemo(&mPhs, M_arcname);
    }
    return res;
}

/* 000005E8-00000680       .text set_mtx__Q29daObjVfan5Act_cFv */
void daObjVfan::Act_c::set_mtx() {
    shape_angle = current.angle;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), M_tmp_mtx);
}

/* 00000680-000006BC       .text init_mtx__Q29daObjVfan5Act_cFv */
void daObjVfan::Act_c::init_mtx() {
    mpModel->setBaseScale(scale);

    set_mtx();
}

/* 000006BC-000009B4       .text ParticleSet__Q29daObjVfan5Act_cFv */
void daObjVfan::Act_c::ParticleSet() {
    dComIfGp_particle_set(dPa_name::ID_SCENE_83CD, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83CE, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83CF, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83D0, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83D1, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83D2, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83D3, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83D4, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83D5, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83D6, &current.pos, &current.angle);
    dComIfGp_particle_set(dPa_name::ID_SCENE_83D7, &current.pos, &current.angle);
}

/* 000009B4-00000C74       .text Execute__Q29daObjVfan5Act_cFPPA3_A4_f */
BOOL daObjVfan::Act_c::Execute(Mtx** mtx) {
    switch (mState) {
    case 0:
        dComIfG_Ccsp()->Set(&mCyl);
        if (mCyl.ChkTgHit()) {
            if (mpBgW != NULL) {
                if (mpBgW->ChkUsed()) {
                    dComIfG_Bgsp()->Release(mpBgW);
                }
            }
            fopAcM_orderOtherEventId(this, m_evid);
            mState = 1;
        }

        break;

    case 1:
        if (eventInfo.checkCommandDemoAccrpt()) {
            mDoAud_seStart(JA_SE_READ_RIDDLE_1);
            fopAcM_seStartCurrent(this, JA_SE_OBJ_GN_SW_DR_LIGHT, 0);

            ParticleSet();
            mBreakTimer = 0;
            mState = 2;
        } else {
            fopAcM_orderOtherEventId(this, m_evid);
        }

        break;

    case 2:
        if (mBreakTimer == 0x96) {
            mIsAlive = false;
            fopAcM_seStartCurrent(this, JA_SE_OBJ_GN_SW_DR_BREAK, 0);
        }

        mBreakTimer++;

        if (dComIfGp_evmng_endCheck(m_evid)) {
            fopAcM_onSwitch(this, prm_get_swSave());
            dComIfGs_onEventBit(0x3a08);
            dComIfGp_event_reset();
            fopAcM_delete(this);
        }
        break;
    }

    set_mtx();
    *mtx = &M_tmp_mtx;
    return TRUE;
}

/* 00000C74-00000D20       .text Draw__Q29daObjVfan5Act_cFv */
BOOL daObjVfan::Act_c::Draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);

    dComIfGd_setListBG();
    // This comparison is necessary to generate "cmplwi 0x1; bne" instead of "cmplwi 0x0; beq"
    if (mIsAlive == true) {
        mDoExt_modelUpdateDL(mpModel);
    }
    dComIfGd_setList();

    return TRUE;
}

namespace daObjVfan {
namespace {
static cPhs_State Mthd_Create(void* i_this) {
    return static_cast<daObjVfan::Act_c*>(i_this)->Mthd_Create();
}
static BOOL Mthd_Delete(void* i_this) {
    return static_cast<daObjVfan::Act_c*>(i_this)->Mthd_Delete();
}
static BOOL Mthd_Execute(void* i_this) {
    return static_cast<daObjVfan::Act_c*>(i_this)->MoveBGExecute();
}
static BOOL Mthd_Draw(void* i_this) {
    return static_cast<daObjVfan::Act_c*>(i_this)->MoveBGDraw();
}
static BOOL Mthd_IsDelete(void* i_this) {
    return static_cast<daObjVfan::Act_c*>(i_this)->MoveBGIsDelete();
}

static actor_method_class Mthd_Vfan = {
    (process_method_func)Mthd_Create,
    (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute,
    (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
}; // namespace
}; // namespace daObjVfan

actor_process_profile_definition g_profile_Obj_Vfan = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Vfan,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjVfan::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Vfan,
    /* Actor SubMtd */ &daObjVfan::Mthd_Vfan,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
