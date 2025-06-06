/**
 * d_a_obj_hbrf1.cpp
 * Object - Wind Temple - Elevator (giant fan room)
 */

#include "d/actor/d_a_obj_hbrf1.h"
#include "d/res/res_hbrf1.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_com_inf_game.h"

Mtx daObjHbrf1::Act_c::M_tmp_mtx;

const char daObjHbrf1::Act_c::M_arcname[] = "Hbrf1";
const char daObjHbrf1::Act_c::M_evname[] = "ami_cam";

/* 00000078-0000012C       .text CreateHeap__Q210daObjHbrf15Act_cFv */
BOOL daObjHbrf1::Act_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(M_arcname, HBRF1_BDL_HBRF1);
    JUT_ASSERT(0x5E, model_data != NULL);

    mpModel = mDoExt_J3DModel__create(model_data, 0, 0x11020203);

    return mpModel != NULL;
}

/* 0000012C-000002B4       .text Create__Q210daObjHbrf15Act_cFv */
BOOL daObjHbrf1::Act_c::Create() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

    if (prm_get_Type() == 0 && !fopAcM_isSwitch(this, prm_get_swSave())) {
        current.pos.y += 750.0f;
        mMode = Mode_UP_STOP_e;
    } else if (prm_get_Type() == 1 && fopAcM_isSwitch(this, prm_get_swSave())) {
        current.pos.y += 750.0f;
        mMode = Mode_UP_STOP_e;
    } else {
        mMode = Mode_DOWN_STOP_e;
    }

    init_mtx();
    fopAcM_setCullSizeBox(this, -800.0f, -1000.0f, -800.0f, 800.0f, 1000.0f, 800.0f);
    mYOffset = 10.0f;
    mEventIdx = dComIfGp_evmng_getEventIdx("LiftMove");
    m2E4 = 0;
    eyePos = home.pos;

    return TRUE;
}

/* 000002B4-000003B0       .text Mthd_Create__Q210daObjHbrf15Act_cFv */
cPhs_State daObjHbrf1::Act_c::Mthd_Create() {
    fopAcM_SetupActor(this, daObjHbrf1::Act_c);

    cPhs_State phase_state = dComIfG_resLoad(&mPhs, M_arcname);

    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(M_arcname, HBRF1_DZB_HBRF1, dBgS_MoveBGProc_Trans, 0);
        JUT_ASSERT(0x94, (phase_state == cPhs_COMPLEATE_e) || (phase_state == cPhs_ERROR_e));
    }

    return phase_state;
}

/* 000003B0-000003B8       .text Delete__Q210daObjHbrf15Act_cFv */
BOOL daObjHbrf1::Act_c::Delete() {
    return TRUE;
}

/* 000003B8-00000404       .text Mthd_Delete__Q210daObjHbrf15Act_cFv */
BOOL daObjHbrf1::Act_c::Mthd_Delete() {
    BOOL ret = MoveBGDelete();

    dComIfG_resDeleteDemo(&mPhs, M_arcname);

    return ret;
}

/* 00000404-00000484       .text set_mtx__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    cMtx_copy(mDoMtx_stack_c::get(), M_tmp_mtx);
}

/* 00000484-000004C0       .text init_mtx__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::init_mtx() {
    mpModel->setBaseScale(scale);
    set_mtx();
}

/* 000004C0-0000066C       .text daObjHbrf1_down_stop__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::daObjHbrf1_down_stop() {
    if ((prm_get_Type() == 0 && !fopAcM_isSwitch(this, prm_get_swSave())) || (prm_get_Type() != 0 && fopAcM_isSwitch(this, prm_get_swSave()))) {
        if (prm_get_Event() == 0 && !dComIfGs_isEventBit(0x1540)) {
            dComIfGs_onEventBit(0x1540);
            fopAcM_orderOtherEventId(this, mEventIdx);
            m2E4 = 1;
            mMode = Mode_UP_DEMO_WAIT_e;
        } else if (prm_get_Event() == 1 && !dComIfGs_isEventBit(0x1510)) {
            dComIfGs_onEventBit(0x1510);
            fopAcM_orderOtherEventId(this, mEventIdx);
            m2E4 = 1;
            mMode = Mode_UP_DEMO_WAIT_e;
        } else {
            mMode = Mode_UP_DEMO_e;
        }
    }
}

/* 0000066C-0000068C       .text daObjHbrf1_up_demo_wait__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::daObjHbrf1_up_demo_wait() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        mTimer = 30;
        mMode = Mode_UP_DEMO_TIMER_e;
    }
}

/* 0000068C-000006B0       .text daObjHbrf1_up_demo_timer__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::daObjHbrf1_up_demo_timer() {
    mTimer -= 1;

    if (mTimer == 0) {
        mMode = Mode_UP_DEMO_e;
    }
}

/* 000006B0-000007FC       .text daObjHbrf1_up_demo__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::daObjHbrf1_up_demo() {
    current.pos.y += mYOffset;

    if (current.pos.y >= home.pos.y + 750.0f) {
        current.pos.y = home.pos.y + 750.0f;

        if (m2E4 == 1) {
            dComIfGp_event_reset();
            m2E4 = 0;
        }

        mMode = Mode_UP_STOP_e;
        fopAcM_seStart(this, JA_SE_OBJ_B_LIFT_STOP, 0);
        dComIfGp_getVibration().StartShock(4, -0x21, cXyz(0.0f, 1.0f, 0.0f));
        return;
    }

    fopAcM_seStart(this, JA_SE_OBJ_B_LIFT_MOVE, 0);
}

/* 000007FC-000009B0       .text daObjHbrf1_up_stop__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::daObjHbrf1_up_stop() {
    if ((prm_get_Type() == 0 && fopAcM_isSwitch(this, prm_get_swSave())) || (prm_get_Type() != 0 && !fopAcM_isSwitch(this, prm_get_swSave()))) {
        if (prm_get_Event() == 0 && !dComIfGs_isEventBit(0x1520)) {
            dComIfGs_onEventBit(0x1520);
            fopAcM_orderOtherEventId(this, mEventIdx);
            m2E4 = 1;
            mMode = Mode_DOWN_DEMO_WAIT_e;
        } else if (prm_get_Event() == 1 && !dComIfGs_isEventBit(0x1508)) {
            dComIfGs_onEventBit(0x1508);
            fopAcM_orderOtherEventId(this, mEventIdx);
            m2E4 = 1;
            mMode = Mode_DOWN_DEMO_WAIT_e;
        } else {
            mMode = Mode_DOWN_DEMO_e;
        }
    }
}

/* 000009B0-00000A08       .text daObjHbrf1_down_demo_wait__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::daObjHbrf1_down_demo_wait() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        mTimer = 30;
        mMode = Mode_DOWN_DEMO_TIMER_e;
        return;
    }

    fopAcM_orderOtherEventId(this, mEventIdx);
}

/* 00000A08-00000A2C       .text daObjHbrf1_down_demo_timer__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::daObjHbrf1_down_demo_timer() {
    mTimer -= 1;

    if (mTimer == 0) {
        mMode = Mode_DOWN_DEMO_e;
    }
}

/* 00000A2C-00000B70       .text daObjHbrf1_down_demo__Q210daObjHbrf15Act_cFv */
void daObjHbrf1::Act_c::daObjHbrf1_down_demo() {
    current.pos.y -= mYOffset;

    if (current.pos.y <= home.pos.y) {
        current.pos.y = home.pos.y;

        if (m2E4 == 1) {
            dComIfGp_event_reset();
            m2E4 = 0;
        }

        mMode = Mode_DOWN_STOP_e;
        fopAcM_seStart(this, JA_SE_OBJ_B_LIFT_STOP, 0);
        dComIfGp_getVibration().StartShock(4, -0x21, cXyz(0.0f, 1.0f, 0.0f));
        return;
    }

    fopAcM_seStart(this, JA_SE_OBJ_B_LIFT_MOVE, 0);
}

/* 00000B70-00000C1C       .text Execute__Q210daObjHbrf15Act_cFPPA3_A4_f */
BOOL daObjHbrf1::Act_c::Execute(Mtx** mtx) {
    switch (mMode) {
    case Mode_DOWN_STOP_e:
        daObjHbrf1_down_stop();
        break;
    case Mode_UP_DEMO_WAIT_e:
        daObjHbrf1_up_demo_wait();
        break;
    case Mode_UP_DEMO_TIMER_e:
        daObjHbrf1_up_demo_timer();
        break;
    case Mode_UP_DEMO_e:
        daObjHbrf1_up_demo();
        break;
    case Mode_UP_STOP_e:
        daObjHbrf1_up_stop();
        break;
    case Mode_DOWN_DEMO_WAIT_e:
        daObjHbrf1_down_demo_wait();
        break;
    case Mode_DOWN_DEMO_TIMER_e:
        daObjHbrf1_down_demo_timer();
        break;
    case Mode_DOWN_DEMO_e:
        daObjHbrf1_down_demo();
        break;
    }

    set_mtx();

    *mtx = &M_tmp_mtx;
    return TRUE;
}

/* 00000C1C-00000CBC       .text Draw__Q210daObjHbrf15Act_cFv */
BOOL daObjHbrf1::Act_c::Draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    // Debug polygons get drawn here?

    return TRUE;
}

namespace daObjHbrf1 {
namespace {
/* 00000CBC-00000CDC       .text Mthd_Create__Q210daObjHbrf127@unnamed@d_a_obj_hbrf1_cpp@FPv */
cPhs_State Mthd_Create(void* i_this) {
    return static_cast<daObjHbrf1::Act_c*>(i_this)->Mthd_Create();
}

/* 00000CDC-00000CFC       .text Mthd_Delete__Q210daObjHbrf127@unnamed@d_a_obj_hbrf1_cpp@FPv */
BOOL Mthd_Delete(void* i_this) {
    return static_cast<daObjHbrf1::Act_c*>(i_this)->Mthd_Delete();
}

/* 00000CFC-00000D1C       .text Mthd_Execute__Q210daObjHbrf127@unnamed@d_a_obj_hbrf1_cpp@FPv */
BOOL Mthd_Execute(void* i_this) {
    return static_cast<daObjHbrf1::Act_c*>(i_this)->MoveBGExecute();
}

/* 00000D1C-00000D48       .text Mthd_Draw__Q210daObjHbrf127@unnamed@d_a_obj_hbrf1_cpp@FPv */
BOOL Mthd_Draw(void* i_this) {
    return static_cast<daObjHbrf1::Act_c*>(i_this)->MoveBGDraw();
}

/* 00000D48-00000D74       .text Mthd_IsDelete__Q210daObjHbrf127@unnamed@d_a_obj_hbrf1_cpp@FPv */
BOOL Mthd_IsDelete(void* i_this) {
    return static_cast<daObjHbrf1::Act_c*>(i_this)->MoveBGIsDelete();
}

static actor_method_class Mthd_Hbrf1 = {
    (process_method_func)Mthd_Create,
    (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute,
    (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
}; // namespace
}; // namespace daObjHbrf1

actor_process_profile_definition g_profile_Obj_Hbrf1 = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Hbrf1,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjHbrf1::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Hbrf1,
    /* Actor SubMtd */ &daObjHbrf1::Mthd_Hbrf1,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
