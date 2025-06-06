/**
 * d_a_obj_monument.cpp
 * Object - Wind's Requiem stone monument
 */

#include "d/actor/d_a_obj_monument.h"
#include "d/res/res_esekh.h"
#include "f_op/f_op_actor_mng.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"

namespace daObjMonument {
    namespace {
        static Attr_c L_attr[] = {
            { ESEKH_BDL_ESEKH,  ESEKH_DZB_ESEKH },
            { ESEKH_BDL_ESEKH2, ESEKH_DZB_ESEKH2 },
        };

        inline const Attr_c & attr(Type_e type) { return L_attr[type]; }
    }
}

const char daObjMonument::Act_c::M_arcname[6] = "Esekh";

/* 00000078-0000009C       .text solidHeapCB__Q213daObjMonument5Act_cFP10fopAc_ac_c */
BOOL daObjMonument::Act_c::solidHeapCB(fopAc_ac_c* i_this) {
    return ((Act_c*)i_this)->create_heap();
}

/* 0000009C-00000238       .text create_heap__Q213daObjMonument5Act_cFv */
bool daObjMonument::Act_c::create_heap() {
    bool ret = true;

    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(M_arcname, attr(mType).mModelId));
    JUT_ASSERT(0x81, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x00, 0x11020203);
    if (!mpModel)
        return false;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    MTXCopy(mDoMtx_stack_c::get(), mtx);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    mpBgW = new dBgW();
    if (mpBgW && mpBgW->Set((cBgD_t*)dComIfG_getObjectRes(M_arcname, attr(mType).mBgWId), cBgW::MOVE_BG_e, &mtx))
        return false;

    return true;
}

/* 00000238-00000318       .text _create__Q213daObjMonument5Act_cFv */
cPhs_State daObjMonument::Act_c::_create() {
    fopAcM_SetupActor(this, Act_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, M_arcname);

    mpBgW = NULL;
    if (ret == cPhs_COMPLEATE_e) {
        mType = prm_get_type();

        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0xd20)) {
            fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
            dComIfG_Bgsp()->Regist(mpBgW, this);
            init_mtx();
        } else {
            ret = cPhs_ERROR_e;
        }
    }

    return ret;
}

/* 00000318-00000398       .text _delete__Q213daObjMonument5Act_cFv */
bool daObjMonument::Act_c::_delete() {
    if (mpBgW != NULL && mpBgW->ChkUsed())
        dComIfG_Bgsp()->Release(mpBgW);
    dComIfG_resDelete(&mPhs, M_arcname);
    return true;
}

/* 00000398-00000404       .text set_mtx__Q213daObjMonument5Act_cFv */
void daObjMonument::Act_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000404-00000440       .text init_mtx__Q213daObjMonument5Act_cFv */
void daObjMonument::Act_c::init_mtx() {
    mpModel->setBaseScale(scale);
    set_mtx();
}

/* 00000440-00000464       .text _execute__Q213daObjMonument5Act_cFv */
bool daObjMonument::Act_c::_execute() {
    set_mtx();
    return true;
}

/* 00000464-00000538       .text _draw__Q213daObjMonument5Act_cFv */
bool daObjMonument::Act_c::_draw() {
    u32 swSave = prm_get_swSave();
    if (dComIfGs_isSwitch(swSave, fopAcM_GetHomeRoomNo(this)))
        return true;

    g_env_light.settingTevStruct(TEV_TYPE_BG1, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return true;
}

namespace daObjMonument {
    namespace {
        /* 00000538-00000558       .text Mthd_Create__Q213daObjMonument30@unnamed@d_a_obj_monument_cpp@FPv */
        cPhs_State Mthd_Create(void* i_this) {
            return ((Act_c*)i_this)->_create();
        }

        /* 00000558-0000057C       .text Mthd_Delete__Q213daObjMonument30@unnamed@d_a_obj_monument_cpp@FPv */
        BOOL Mthd_Delete(void* i_this) {
            return ((Act_c*)i_this)->_delete();
        }

        /* 0000057C-000005A0       .text Mthd_Execute__Q213daObjMonument30@unnamed@d_a_obj_monument_cpp@FPv */
        BOOL Mthd_Execute(void* i_this) {
            return ((Act_c*)i_this)->_execute();
        }

        /* 000005A0-000005C4       .text Mthd_Draw__Q213daObjMonument30@unnamed@d_a_obj_monument_cpp@FPv */
        BOOL Mthd_Draw(void* i_this) {
            return ((Act_c*)i_this)->_draw();
        }

        /* 000005C4-000005CC       .text Mthd_IsDelete__Q213daObjMonument30@unnamed@d_a_obj_monument_cpp@FPv */
        BOOL Mthd_IsDelete(void* i_this) {
            return TRUE;
        }

        static actor_method_class Mthd_Table = {
            (process_method_func)daObjMonument::Mthd_Create,
            (process_method_func)daObjMonument::Mthd_Delete,
            (process_method_func)daObjMonument::Mthd_Execute,
            (process_method_func)daObjMonument::Mthd_IsDelete,
            (process_method_func)daObjMonument::Mthd_Draw,
        };
    }
}

actor_process_profile_definition g_profile_Obj_Monument = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Monument,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjMonument::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Monument,
    /* Actor SubMtd */ &daObjMonument::Mthd_Table,
    /* Status       */ 0x18 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
