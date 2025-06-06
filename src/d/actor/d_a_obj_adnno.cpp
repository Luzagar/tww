/**
 * d_a_obj_adnno.cpp
 * Object - Unused - Triforce prayer mural
 */

#include "d/actor/d_a_obj_adnno.h"
#include "d/res/res_adnno.h"
#include "f_op/f_op_actor_mng.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"

static const u32 daObjAdnno_bmt_table[16] = {
    ADNNO_BMT_ADNNO_00,
    ADNNO_BMT_ADNNO_01,
    ADNNO_BMT_ADNNO_02,
    ADNNO_BMT_ADNNO_03,
    ADNNO_BMT_ADNNO_04,
    ADNNO_BMT_ADNNO_05,
    ADNNO_BMT_ADNNO_06,
    ADNNO_BMT_ADNNO_07,
    ADNNO_BMT_ADNNO_08,
    ADNNO_BMT_ADNNO_09,
    ADNNO_BMT_ADNNO_10,
    ADNNO_BMT_ADNNO_11,
    ADNNO_BMT_ADNNO_12,
    ADNNO_BMT_ADNNO_13,
    ADNNO_BMT_ADNNO_14,
    ADNNO_BMT_ADNNO_15,
};

/* 00000078-00000098       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daObjAdnno_c*)i_this)->CreateHeap();
}

/* 00000098-00000178       .text CreateHeap__12daObjAdnno_cFv */
BOOL daObjAdnno_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)(dComIfG_getObjectRes("Adnno", ADNNO_BDL_ADNNO));
    JUT_ASSERT(0x5c, modelData != NULL);
    for (s32 i = 0; i < 16; i++) {
        mpModel[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x37441422);
        if (!mpModel[i])
            return FALSE;
    }
    return TRUE;
}

/* 00000178-000001E8       .text CreateInit__12daObjAdnno_cFv */
void daObjAdnno_c::CreateInit() {
    fopAcM_SetMtx(this, mpModel[0]->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -600.0f, -0.0f, -600.0f, 600.0f, 900.0f, 600.0f);
    fopAcM_setCullSizeFar(this, 1.0f);
    set_mtx();
}

/* 000001E8-00000330       .text set_mtx__12daObjAdnno_cFv */
void daObjAdnno_c::set_mtx() {
    for (int i = 0; i < 16; i++) {
        J3DModel * model = mpModel[i];
        model->setBaseScale(scale);

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(current.angle.y);
        int x = i % 4;
        int y = i / 4;
        mDoMtx_stack_c::transM(
            x * 40.0f - 60.0f,
            60.0f - y * 40.0f,
            0.0f
        );
        mpModel[i]->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

cPhs_State daObjAdnno_c::_create() {
    fopAcM_SetupActor(this, daObjAdnno_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, "Adnno");

    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x9C00) == 0) {
            ret = cPhs_ERROR_e;
        } else {
            CreateInit();
        }
    }

    return ret;
}

bool daObjAdnno_c::_delete() {
    dComIfG_resDelete(&mPhs, "Adnno");
    return true;
}

bool daObjAdnno_c::_execute() {
    set_mtx();
    return true;
}

static const u16 daObjAdnno_event_bit_table[16] = {
    0x3508, 0x3504, 0x3502, 0x3501,
    0x3680, 0x3640, 0x3620, 0x3610,
    0x3608, 0x3604, 0x3602, 0x3601,
    0x3780, 0x3740, 0x3720, 0x3710,
};

bool daObjAdnno_c::_draw() {
    dComIfGd_setListBG();
    for (s32 i = 0; i < 16; i++) {
        if (dComIfGs_isEventBit(daObjAdnno_event_bit_table[i])) {
            J3DMaterialTable* pBmt = (J3DMaterialTable*)dComIfG_getObjectRes("Adnno", daObjAdnno_bmt_table[i]);
            mpModel[i]->getModelData()->setMaterialTable(pBmt, J3DMatCopyFlag_All);
            mDoExt_modelUpdateDL(mpModel[i]);
        }
    }
    dComIfGd_setList();
    return true;
}

/* 00000330-000003D4       .text daObjAdnno_Create__FPv */
static cPhs_State daObjAdnno_Create(void* i_this) {
    return ((daObjAdnno_c*)i_this)->_create();
}

/* 000003D4-00000404       .text daObjAdnno_Delete__FPv */
static BOOL daObjAdnno_Delete(void* i_this) {
    return ((daObjAdnno_c*)i_this)->_delete();
}

/* 00000404-000004F8       .text daObjAdnno_Draw__FPv */
static BOOL daObjAdnno_Draw(void* i_this) {
    return ((daObjAdnno_c*)i_this)->_draw();
}

/* 000004F8-0000051C       .text daObjAdnno_Execute__FPv */
static BOOL daObjAdnno_Execute(void* i_this) {
    return ((daObjAdnno_c*)i_this)->_execute();
}

/* 0000051C-00000524       .text daObjAdnno_IsDelete__FPv */
static BOOL daObjAdnno_IsDelete(void* i_this) {
    return TRUE;
}

static actor_method_class daObj_AdnnoMethodTable = {
    (process_method_func)daObjAdnno_Create,
    (process_method_func)daObjAdnno_Delete,
    (process_method_func)daObjAdnno_Execute,
    (process_method_func)daObjAdnno_IsDelete,
    (process_method_func)daObjAdnno_Draw,
};

actor_process_profile_definition g_profile_Obj_Adnno = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Adnno,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjAdnno_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Adnno,
    /* Actor SubMtd */ &daObj_AdnnoMethodTable,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
