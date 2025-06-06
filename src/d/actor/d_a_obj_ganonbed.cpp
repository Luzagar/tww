/**
 * d_a_obj_ganonbed.cpp
 * Object - Bed (Puppet Ganon intro cutscene)
 */

#include "d/actor/d_a_obj_ganonbed.h"
#include "d/res/res_gbed.h"
#include "f_op/f_op_actor_mng.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"

namespace {
    static const char l_arcname[] = "Gbed";
};

/* 00000078-00000100       .text init_mtx__11daObjGbed_cFv */
void daObjGbed_c::init_mtx() {
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::XYZrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000100-00000124       .text solidHeapCB__11daObjGbed_cFP10fopAc_ac_c */
BOOL daObjGbed_c::solidHeapCB(fopAc_ac_c* i_this) {
    return ((daObjGbed_c*)i_this)->create_heap();
}

/* 00000124-00000224       .text create_heap__11daObjGbed_cFv */
bool daObjGbed_c::create_heap() {
    bool ret = true;

    J3DModelData* pModelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcname, GBED_BDL_K_GBED));

    if (!pModelData) {
        JUT_ASSERT(0xb1, FALSE);
        ret = false;
    } else {
        mpModel = mDoExt_J3DModel__create(pModelData, 0x80000, 0x11000022);
        mpBgW = dBgW_NewSet((cBgD_t*)dComIfG_getObjectRes(l_arcname, GBED_DZB_K_GBED), cBgW::MOVE_BG_e, &mpModel->getBaseTRMtx());

        if (!mpModel || !mpBgW)
            ret = false;
    }

    return ret;
}

/* 00000224-000002F8       .text _create__11daObjGbed_cFv */
cPhs_State daObjGbed_c::_create() {
    fopAcM_SetupActor(this, daObjGbed_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, l_arcname);

    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0x13e0)) {
            if (dComIfG_Bgsp()->Regist(mpBgW, this)) {
                ret = cPhs_ERROR_e;
            } else {
                fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
                init_mtx();
            }
        } else {
            ret = cPhs_ERROR_e;
        }
    }

    return ret;
}

/* 000002F8-0000038C       .text _delete__11daObjGbed_cFv */
bool daObjGbed_c::_delete() {
    dComIfG_resDelete(&mPhs, l_arcname);

    if (heap != NULL && mpBgW != NULL) {
        if (mpBgW->ChkUsed()) {
            dComIfG_Bgsp()->Release(mpBgW);
        }

        mpBgW = NULL;
    }

    return true;
}

/* 0000038C-000003E4       .text _execute__11daObjGbed_cFv */
bool daObjGbed_c::_execute() {
    if (mpBgW != NULL && mpBgW->ChkUsed())
        mpBgW->Move();
    return true;
}

/* 000003E4-00000444       .text _draw__11daObjGbed_cFv */
bool daObjGbed_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    return true;
}

/* 00000444-00000464       .text daObjGbed_Create__FP10fopAc_ac_c */
static cPhs_State daObjGbed_Create(fopAc_ac_c* i_this) {
    return ((daObjGbed_c*)i_this)->_create();
}

/* 00000464-00000488       .text daObjGbed_Delete__FP11daObjGbed_c */
static BOOL daObjGbed_Delete(daObjGbed_c* i_this) {
    return i_this->_delete();
}

/* 00000488-000004AC       .text daObjGbed_Execute__FP11daObjGbed_c */
static BOOL daObjGbed_Execute(daObjGbed_c* i_this) {
    return i_this->_execute();
}

/* 000004AC-000004D0       .text daObjGbed_Draw__FP11daObjGbed_c */
static BOOL daObjGbed_Draw(daObjGbed_c* i_this) {
    return i_this->_draw();
}

/* 000004D0-000004D8       .text daObjGbed_IsDelete__FP11daObjGbed_c */
static BOOL daObjGbed_IsDelete(daObjGbed_c* i_this) {
    return TRUE;
}

static actor_method_class l_daObjGbed_Method = {
    (process_method_func)daObjGbed_Create,
    (process_method_func)daObjGbed_Delete,
    (process_method_func)daObjGbed_Execute,
    (process_method_func)daObjGbed_IsDelete,
    (process_method_func)daObjGbed_Draw,
};

actor_process_profile_definition g_profile_Obj_Gbed = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Gbed,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjGbed_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Gbed,
    /* Actor SubMtd */ &l_daObjGbed_Method,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
