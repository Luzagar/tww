/**
 * d_a_tornado.cpp
 * Ballad Of Gales Tornado
 */

#include "d/actor/d_a_tornado.h"
#include "d/res/res_trnd.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_ship.h"
#include "d/d_kankyo_wether.h"

#include "weak_bss_936_to_1036.h" // IWYU pragma: keep

static char l_arcName[] = "Trnd";

const float daTornado_HIO_c0::move_dis = 7500.0f;
const float daTornado_HIO_c0::start_dis = 10000.0f;

/* 000000EC-00000260       .text jointCallBack__11daTornado_cFi */
BOOL daTornado_c::jointCallBack(int jntNo) {
    int jntIdx = jntNo - 1;
    if ((jntIdx < 0) || (jntIdx >= 11)) {
        return TRUE;
    }

    mDoMtx_stack_c::transS(mJointX[jntIdx], 0.0f, mJointZ[jntIdx]);
    if (jntIdx != 10 && jntIdx != 0) {
        mDoMtx_stack_c::ZXYrotM(-mJointZ[jntIdx] * 3572.0f * 0.001f, 0.0f, mJointX[jntIdx] * 3572.0f * 0.001f);
    }

    if (fopAcM_GetParam(this) != 0) {
        float scale = mJointScale[jntIdx];
        mDoMtx_stack_c::scaleM(scale, scale, scale);
    }
    mDoMtx_stack_c::revConcat(mpModel->getAnmMtx(jntNo));
    mpModel->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    mDoMtx_stack_c::transS(mJointX[jntIdx], 0.0f, mJointZ[jntIdx]);
    mDoMtx_stack_c::revConcat(J3DSys::mCurrentMtx);
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return TRUE;
}

static const float l_joint_scale[11] = { 0.1f, 0.4f, 0.7f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
static const float joint_offset[11] = { 100.0f, 200.0f, 300.0f, 400.0f, 300.0f, 300.0f, 400.0f, 500.0f, 400.0f, 500.0f, 600.0f };

static daTornado_HIO_c0 l_HIO;

/* 00000260-000002A4       .text daTornado_jointCallBack__FP7J3DNodei */
static BOOL daTornado_jointCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        daTornado_c* i_this = reinterpret_cast<daTornado_c*>(model->getUserArea());
        i_this->jointCallBack(jntNo);
    }
    return TRUE;
}

/* 000002A4-00000464       .text draw__11daTornado_cFv */
BOOL daTornado_c::draw() {
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);

    mBck.entry(mpModel->getModelData());
    mBtk.entry(mpModel->getModelData(), mBtkFrame);
    mBrk.entry(mpModel->getModelData(), mBrkFrame);

    if (dComIfGs_isTmpBit(0x404)) {
        dComIfGd_setListMaskOff();
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();
    } else {
        mDoExt_modelUpdateDL(mpModel);
    }

    if (fopAcM_GetParam(this) == 0) {
        g_env_light.settingTevStruct(TEV_TYPE_BG1, &current.pos, &tevStr);
        g_env_light.setLightTevColorType(mpModelUnder, &tevStr);

        mBckUnder.entry(mpModelUnder->getModelData(), mBck.getFrame());
        mBtkUnder.entry(mpModelUnder->getModelData(), mBtkUnderFrame);
        mBrkUnder.entry(mpModelUnder->getModelData(), mBrkFrame);

        dComIfGd_setListMaskOff();
        mDoExt_modelUpdateDL(mpModelUnder);
        dComIfGd_setList();
    }

    return TRUE;
}

/* 00000464-00000484       .text daTornado_Draw__FP11daTornado_c */
static BOOL daTornado_Draw(daTornado_c* i_this) {
    return i_this->draw();
}

/* 00000484-00000C4C       .text execute__11daTornado_cFv */
BOOL daTornado_c::execute() {
    static cXyz wind_scale(20.0f,20.0f,20.0f);

    mBck.play();
    mBtkFrame += 1.0f;
    if (mBtkFrame >= mBtk.getBtkAnm()->getFrameMax()) {
        mBtkFrame -= mBtk.getBtkAnm()->getFrameMax();
    }

    mBtkUnderFrame += 1.0f;
    if (mBtkUnderFrame >= mBtkUnder.getBtkAnm()->getFrameMax()) {
        mBtkUnderFrame -= mBtkUnder.getBtkAnm()->getFrameMax();
    }
    mAngle2 += 250;
    mAngle1 += 500;
    m32c = 10000.0f;
    u32 param = fopAcM_GetParam(this);
    f32 fVar8;
    if (param == 1) {
        if (dComIfGp_getShipActor() != NULL) {
            fVar8 = (dComIfGp_getShipActor()->current.pos.y - home.pos.y) / 500.0f;
            if (fVar8 < 0.0f) {
                fVar8 = 0.0f;
            } else {
                if (fVar8 > 1.0f) {
                    fVar8 = 1.0f;
                }
            }
        } else {
            fVar8 = 1.0f;
        }
    } else if (param == 0 && dComIfGp_getShipActor() != NULL && dComIfGp_getShipActor()->getTornadoActor() != NULL) {
        fVar8 = 0.25f;
    } else {
        fVar8 = 1.0f;
    }

    for (int i = 0; i < 11; i++) {
        f32 sin = cM_ssin(mAngle1 - 0x1000 * i);
        f32 tmp1 = fVar8 * joint_offset[i];
        f32 fVar1 = tmp1 * (sin + 1.0f);
        s16 angle2 = mAngle2 - 0x1800 * i;
        mJointX[i] = cM_ssin(angle2) * fVar1 * scale.x;
        mJointZ[i] = cM_scos(angle2) * fVar1 * scale.x;
    }

    param = fopAcM_GetParam(this);
    if (param == 0) {
        if (!dComIfGp_event_runCheck()) {
            s16 target = fopAcM_searchActorAngleY(this, dComIfGp_getLinkPlayer());
            cLib_addCalcAngleS(&current.angle.y, target, 10, 0x1000, 0x100);
            cLib_chaseF(&speedF, 20.0f, 0.2f);
        }
        if (dComIfGp_checkPlayerStatus0(0, daPyStts0_SHIP_RIDE_e) && dComIfGp_getShipActor() != NULL) {
            daShip_c* ship = dComIfGp_getShipActor();
            cXyz diff = ship->current.pos - current.pos;
            if (diff.abs2XZ() < 10000.0f*10000.0f) {
                ship->onTornadoFlg(fopAcM_GetID(this));
                speedF = 0.0f;
            }
        }

        if (!dComIfGp_event_runCheck()) {
            fopAcM_posMoveF(this, NULL);
            cXyz diff = current.pos - home.pos;
            diff.y = 0;
            if (diff.abs2XZ() > 7500.0f*7500.0f) {
                diff.normalize();
                current.pos = home.pos + diff * 7500.0f;
            }
        }

        if (mPtclTimer != 0) {
            mPtclTimer -= 1;
        } else {
            mPtclTimer = 10;
            dComIfGp_particle_set(dPa_name::ID_SCENE_8213, &current.pos, NULL, (cXyz*)&wind_scale);
        }

        fopAcM_seStartCurrent(this, JA_SE_OBJ_TORNADE_SUS, 100);

        if (dComIfGs_isEventBit(0x2710)) {
            mPtclCb.remove();
            daShip_c* ship = dComIfGp_getShipActor();
            if (ship != NULL) {
                ship->offTornadoFlg();
            }
            mBrkFrame += 0.3f;
            if (mBrkFrame >= mBrk.getBrkAnm()->getFrameMax()) {
                fopAcM_delete(this);
            }
        } else {
            dKyw_tornado_Notice(&current.pos);
        }
    } else if (param == 1) {
        if (m31c != 0) {
            for (int i = 0; i < 11; i++) {
                cLib_chaseF(&mJointScale[i], l_joint_scale[i], (11-i)*0.007f);
                mJointX[i] *= mJointScale[i];
                mJointZ[i] *= mJointScale[i];
            }
        }
    } else if (m31c != 0) {
        for (int i = 0; i < 11; i++) {
            cLib_chaseF(&mJointScale[i], 0.0f, (i + 1) * 0.001f);
            mJointX[i] *= mJointScale[i];
            mJointZ[i] *= mJointScale[i];
        }
        if (mJointScale[10] < 1e-06f) {
            fopAcM_delete(this);
            return TRUE;
        }
    }
    mDoMtx_stack_c::transS(current.pos);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    attention_info.position = current.pos;
    eyePos = current.pos;
    mCenter.set(current.pos.x + mJointX[0], current.pos.y, current.pos.z + mJointZ[0]);
    if (mPtclCb.getEmitter() != NULL) {
        GXColor colorAmb, colorDif;
        dKy_get_seacolor(&colorAmb, &colorDif);
        mPtclCb.getEmitter()->setGlobalPrmColor(colorAmb.r, colorAmb.g, colorAmb.b);
    }
    mDoMtx_stack_c::transS(mCenter);
    mpModelUnder->setBaseTRMtx(mDoMtx_stack_c::get());

    return TRUE;
}

/* 00000C88-00000CA8       .text daTornado_Execute__FP11daTornado_c */
static BOOL daTornado_Execute(daTornado_c* i_this) {
    return i_this->execute();
}

/* 00000CA8-00000CB0       .text daTornado_IsDelete__FP11daTornado_c */
static BOOL daTornado_IsDelete(daTornado_c*) {
    return TRUE;
}

/* 00000CB0-00000D30       .text tornado_delete__11daTornado_cFv */
BOOL daTornado_c::tornado_delete() {
    mPtclCb.remove();
    if (fopAcM_GetParam(this) == 0 && dComIfGp_getShipActor() != NULL) {
        dComIfGp_getShipActor()->offTornadoFlg();
    }
    dComIfG_resDelete(&mPhs, l_arcName);
    return TRUE;
}

/* 00000D30-00000D54       .text daTornado_Delete__FP11daTornado_c */
static BOOL daTornado_Delete(daTornado_c* i_this) {
    i_this->tornado_delete();
    return TRUE;
}

/* 00000D54-0000109C       .text createHeap__11daTornado_cFv */
BOOL daTornado_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, TRND_BDL_YTRND00);
    JUT_ASSERT(0x1fe, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000202);
    if (!mpModel)
        return FALSE;
    if (!mBck.init(modelData, (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, TRND_BCK_YTRND00), true, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false))
        return FALSE;
    if (!mBtk.init(modelData, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, TRND_BTK_YTRND00), false, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0))
        return FALSE;
    if (!mBrk.init(modelData, (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, TRND_BRK_YTRND00), false, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0))
        return FALSE;

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, TRND_BDL_YWUWT00);
    JUT_ASSERT(0x226, modelData != NULL);
    mpModelUnder = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000202);
    if (!mpModelUnder)
        return FALSE;
    if (!mBckUnder.init(modelData, (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, TRND_BCK_YWUWT00), false, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false))
        return FALSE;
    if (!mBtkUnder.init(modelData, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, TRND_BTK_YWUWT00), false, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0))
        return FALSE;
    if (!mBrkUnder.init(modelData, (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, TRND_BRK_YWUWT00), false, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0))
        return FALSE;

    return TRUE;
}

/* 0000109C-000010BC       .text daTornado_createHeap__FP10fopAc_ac_c */
static BOOL daTornado_createHeap(fopAc_ac_c* i_this) {
    return ((daTornado_c*)i_this)->createHeap();
}

/* 000010BC-000014D0       .text create__11daTornado_cFv */
cPhs_State daTornado_c::create() {
    static cXyz small_scale(0.25f, 0.175f, 0.25f);
    static cXyz under_small_scale(0.251f, 0.25f, 0.251f);
    static cXyz under_scale(1.01f, 1.0f, 1.01f);

    cPhs_State rt = dComIfG_resLoad(&mPhs, l_arcName);
    fopAcM_SetupActor(this, daTornado_c);

    if (rt == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daTornado_createHeap, 0x8000)) {
            return cPhs_ERROR_e;
        }

        u32 param = fopAcM_GetParam(this);
        if (param == 1) {
            current.pos.y -= 2000.0f;
            mpModel->setBaseScale(small_scale);
            mpModelUnder->setBaseScale(under_small_scale);
        } else if (param == 2) {
            mpModel->setBaseScale(small_scale);
            mpModelUnder->setBaseScale(under_small_scale);
            for (int i=0; i<11; i++) {
                mJointScale[i] = l_joint_scale[i];
            }
        } else {
            fopAcM_SetParam(this, 0);
            if (dComIfGs_isEventBit(0x2710)) {
                return cPhs_ERROR_e;
            }
            dKyw_tornado_Notice(&current.pos);
            mpModelUnder->setBaseScale(under_scale);
            mCenter = current.pos;
            dComIfGp_particle_set(dPa_name::ID_SCENE_81BB, &mCenter, NULL, NULL, 0xFF, &mPtclCb);
            fopAcM_OnStatus(this, fopAcStts_SHOWMAP_e);
        }
        mDoMtx_stack_c::transS(current.pos);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mpModelUnder->setBaseTRMtx(mDoMtx_stack_c::get());
        J3DModelData* modelData = mpModel->getModelData();
        for (u16 i = 1; i < modelData->getJointNum(); i++) {
            modelData->getJointNodePointer(i)->setCallBack(daTornado_jointCallBack);
        }
        mpModel->setUserArea((u32) this);
    }
    return rt;
}

/* 0000162C-0000164C       .text daTornado_Create__FP10fopAc_ac_c */
static cPhs_State daTornado_Create(fopAc_ac_c* i_this) {
    return ((daTornado_c*)i_this)->create();
}

static actor_method_class l_daTornado_Method = {
    (process_method_func)daTornado_Create,
    (process_method_func)daTornado_Delete,
    (process_method_func)daTornado_Execute,
    (process_method_func)daTornado_IsDelete,
    (process_method_func)daTornado_Draw,
};

actor_process_profile_definition g_profile_TORNADO = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_TORNADO,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTornado_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_TORNADO,
    /* Actor SubMtd */ &l_daTornado_Method,
    /* Status       */ 0x06 | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
