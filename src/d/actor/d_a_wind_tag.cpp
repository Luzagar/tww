//
// Generated by dtk
// Translation Unit: d_a_wind_tag.cpp
//

#include "d/actor/d_a_wind_tag.h"
#include "d/res/res_yaflw00.h"
#include "d/res/res_ybgaf00.h"
#include "f_op/f_op_camera.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_level_se.h"
#include "d/d_path.h"
#include "d/d_procname.h"

namespace daWindTag {
    dCcD_SrcCps l_cps_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_WIND,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e | cCcD_AtSPrm_VsOther_e,
            /* SrcObjTg  Type    */ 0,
            /* SrcObjTg  SPrm    */ 0,
            /* SrcObjCo  SPrm    */ 0,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ 0,
            /* SrcGObjAt Spl     */ 0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ dCcG_AtSPrm_NoHitMark_e,
            /* SrcGObjTg Se      */ 0,
            /* SrcGObjTg HitMark */ 0,
            /* SrcGObjTg Spl     */ 0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoHitMark_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGCpsS
        {
            /* P0 */ 0.0f, 0.0f, 0.0f,
            /* P1 */ 0.0f, 0.0f, 0.0f,
            /* Height */ 200.0f,
        },
    };
}

const char *daWindTag::daWindTag_c::m_arcname[] = { "Yaflw00", "Ybgaf00" };
s16 daWindTag::daWindTag_c::m_bdlidx[] = { YAFLW00_BDL_YAFLW00, YBGAF00_BDL_YBGAF00 };
s16 daWindTag::daWindTag_c::m_heapsize[] = { 0x0C40, 0x0C40 };
s16 daWindTag::daWindTag_c::m_bckidx[] = { YAFLW00_BCK_YAFLW00, YBGAF00_BCK_YBGAF00 };
s16 daWindTag::daWindTag_c::m_btkidx[] = { YAFLW00_BTK_YAFLW00_01, YBGAF00_BTK_YBGAF00_01 };
s16 daWindTag::daWindTag_c::m_btkidx2[] = { YAFLW00_BTK_YAFLW00_02, YBGAF00_BTK_YBGAF00_02 };
f32 daWindTag::daWindTag_c::m_cullsize_far = 4.0f;
f32 daWindTag::daWindTag_c::m_ef_cullsize_far = 2.0f;

/* 00000078-000000F0       .text _delete__Q29daWindTag11daWindTag_cFv */
bool daWindTag::daWindTag_c::_delete() {
    if (mpEmitter != NULL) {
        mpEmitter->becomeInvalidEmitter();
        mpEmitter = NULL;
    }

    dComIfG_resDelete(&mPhs, m_arcname[mType]);
    mPointWind.set_pwind_delete();
    return true;
}

/* 000000F0-00000110       .text CheckCreateHeap__9daWindTagFP10fopAc_ac_c */
BOOL daWindTag::CheckCreateHeap(fopAc_ac_c* i_ac) {
    return ((daWindTag::daWindTag_c*)i_ac)->CreateHeap();
}

/* 00000110-000003E4       .text CreateHeap__Q29daWindTag11daWindTag_cFv */
BOOL daWindTag::daWindTag_c::CreateHeap() {
    /* Nonmatching */
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(m_arcname[mType], m_bdlidx[mType]);
    JUT_ASSERT(0x16A, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000222);
    if (mpModel == NULL)
        return FALSE;

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(m_arcname[mType], m_btkidx[mType]);
    JUT_ASSERT(0x17A, pbtk != NULL);
    if (!mBtkAnm0.init(modelData, pbtk, TRUE, J3DFrameCtrl::LOOP_ONCE_e, 1.0f, 0, -1, false, 0))
        return FALSE;

    pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(m_arcname[mType], m_btkidx2[mType]);
    JUT_ASSERT(0x17A, pbtk != NULL);
    if (!mBtkAnm1.init(modelData, pbtk, TRUE, J3DFrameCtrl::LOOP_ONCE_e, 1.0f, 0, -1, false, 0))
        return FALSE;

    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(m_arcname[mType], m_bckidx[mType]);
    JUT_ASSERT(0x192, pbck != NULL);
    if (!mBckAnm.init(modelData, pbck, TRUE, J3DFrameCtrl::LOOP_ONCE_e, 1.0f, 0, -1, false))
        return FALSE;

    return TRUE;
}

/* 000003E4-00000828       .text CreateInit__Q29daWindTag11daWindTag_cFv */
void daWindTag::daWindTag_c::CreateInit() {
    /* Nonmatching */
    mbDraw = true;
    if (((home.angle.z >> 4) & 0x0F) == 1) {
        mbDraw = false;
    }

    field_0x57f = false;
    if ((home.angle.z & 0x0F) == 1) {
        field_0x57f = true;
    }

    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    f32 radius = scale.x * mData[mType * 2];
    fopAcM_setCullSizeBox(this, -radius, 0.0f, -radius, radius, 0.0f, radius);
    fopAcM_setCullSizeFar(this, m_cullsize_far);

    mStts.Init(0xFF, 0xFF, this);
    mCps.Set(l_cps_src);
    mCps.SetStts(&mStts);
    field_0x49c = scale.y;
    mSwNo = fopAcM_GetParam(this) >> 24;
    field_0x57c = fopAcM_isSwitch(this, mSwNo);

    bool state;
    if (((fopAcM_GetParam(this) >> 23) & 1) == 0) {
        state = (mSwNo != 0xFF && !fopAcM_isSwitch(this, mSwNo));
    } else {
        state = (mSwNo != 0xFF && fopAcM_isSwitch(this, mSwNo));
    }

    if (mSwNo == 0xFF | state) { // nice bug
        mOffsY = field_0x49c * mData[mType * 2 + 1];
        mBtkAnm1.setFrame(mBtkAnm1.getEndFrame());
    } else {
        mOffsY = 0.0f;
        mBtkAnm1.setFrame(0.0f);
    }

    set_wind_angle();
    mPathId = (fopAcM_GetParam(this) >> 8) & 0xFF;
    if (mPathId != 0xFF) {
        mpPath = dPath_GetRoomPath(mPathId, fopAcM_GetRoomNo(this));
        if (mpPath != NULL) {
            mPathPointDir = 1;
            mCurPathPoint = 1;
            mTargetPos = mpPath->mpPnt[mCurPathPoint].mPos;
            current.pos = mpPath->mpPnt[0].mPos;
            speedF = 10.0f + ((fopAcM_GetParam(this) >> 16) & 0x1F);
        } else {
            mPathId = 0xFF;
        }
    }
    mPointWind.set_pwind_init(&mCpsS);
    mLevelSeID = fopKyM_create(PROC_LEVEL_SE, checkSizeSpecialBig() ? 0x701e : 0x701d, &eyePos);
    mpEmitter = NULL;
    cXyz efScale(scale.x, scale.x, scale.x);
    if (field_0x57f) {
        g_env_light.settingTevStruct(TEV_TYPE_BG1, &current.pos, &mEfTevStr);
        mEfColor.r = mEfTevStr.mColorC0.r;
        mEfColor.g = mEfTevStr.mColorC0.g;
        mEfColor.b = mEfTevStr.mColorC0.b;
        mEfColor.a = mEfTevStr.mColorC0.a;
        mpEmitter = dComIfGp_particle_set(0x8290, &current.pos, NULL, &scale, 0xFF, NULL, fopAcM_GetRoomNo(this), &mEfColor);
        if (mpEmitter != NULL)
            mpEmitter->setGlobalScale(efScale);
    }

    current.angle.z = 0;
    home.angle.z = 0;
    dKy_tevstr_init(&mEfTevStr, fopAcM_GetRoomNo(this), 0xFF);
}

/* 00000828-000008D4       .text set_wind_angle__Q29daWindTag11daWindTag_cFv */
void daWindTag::daWindTag_c::set_wind_angle() {
    mCpsS.mStart = current.pos;
    mCpsS.mRadius = scale.x * mData[mType * 2];
    set_mtx();
    mCps.cM3dGCps::Set(mCpsS);
    mCps.CalcAtVec();
}

/* 000008D4-0000099C       .text _create__Q29daWindTag11daWindTag_cFv */
s32 daWindTag::daWindTag_c::_create() {
    fopAcM_SetupActor(this, daWindTag_c);
    mType = (fopAcM_GetParam(this) >> 21) & 0x03;
    s32 rt = dComIfG_resLoad(&mPhs, m_arcname[mType]);
    if (rt == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, m_heapsize[mType]))
            return cPhs_ERROR_e;
        CreateInit();
    }
    return rt;
}

/* 00000E60-00000F74       .text set_mtx__Q29daWindTag11daWindTag_cFv */
void daWindTag::daWindTag_c::set_mtx() {
    f32 offsY = mOffsY;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(current.angle);
    mDoMtx_stack_c::transM(0.0f, offsY, 0.0f);
    mDoMtx_stack_c::multVec(&cXyz::Zero, &mCpsS.mEnd);
    if (current.angle.x == 0) {
        mCpsS.mEnd.x = current.pos.x;
        mCpsS.mEnd.z = current.pos.z;
    }
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(current.angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000F74-00000F88       .text checkSizeSpecialBig__Q29daWindTag11daWindTag_cFv */
bool daWindTag::daWindTag_c::checkSizeSpecialBig() {
    if (mType == 1)
        return true;
    return false;
}

/* 00000F88-00001044       .text set_wind_se_sub__Q29daWindTag11daWindTag_cFUlP4cXyz */
void daWindTag::daWindTag_c::set_wind_se_sub(u32, cXyz* pos) {
    if (mLevelSeID != fpcM_ERROR_PROCESS_ID_e) {
        dLevelSe_c* se = (dLevelSe_c*)fopKyM_SearchByID(mLevelSeID);
        if (se != NULL) {
            s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
            se->setReverb(100, reverb);
            se->mPos = *pos;
            if (mOffsY < 1.0f)
                se->mFlag |= 0x08;
            else
                se->mFlag &= ~0x08;
        }
    }
}

/* 00001044-000011CC       .text set_wind_se__Q29daWindTag11daWindTag_cFv */
void daWindTag::daWindTag_c::set_wind_se() {
    /* Nonmatching */
    f32 radius = scale.x * mData[mType * 2];
    camera_class* camera = dComIfGp_getCamera(0);

    cM3dGCps cps;
    cM3dGSph camSph;
    cps.SetR(radius);
    cps.SetStartEnd(mCpsS.mStart, mCpsS.mEnd);
    camSph.SetC(camera->mLookat.mEye);
    camSph.SetR(1.0f);
    Vec cross;
    if (cM3d_Cross_CpsSph(cps, camSph, &cross)) {
        mSePos = eyePos;
        set_wind_se_sub(0, &mSePos);
    } else {
        cXyz pos = eyePos;
        cps.NearPos(pos, &mSePos);
        set_wind_se_sub(0, &mSePos);
    }
}

/* 00001234-000015C8       .text _execute__Q29daWindTag11daWindTag_cFv */
bool daWindTag::daWindTag_c::_execute() {
    /* Nonmatching */
}

/* 000015C8-0000161C       .text path_move__Q29daWindTag11daWindTag_cFv */
void daWindTag::daWindTag_c::path_move() {
    if (mPathId != 0xFF && cLib_chasePos(&current.pos, mTargetPos, speedF))
        set_next_pnt();
}

/* 0000161C-00001700       .text set_next_pnt__Q29daWindTag11daWindTag_cFv */
void daWindTag::daWindTag_c::set_next_pnt() {
    /* Nonmatching */
    if (mPathId == 0xFF)
        return;

    mCurPathPoint += mPathPointDir;
    if (mpPath->mLoops & 1) {
        if (mCurPathPoint > mpPath->m_num - 1) {
            mCurPathPoint = 0;
        } else if (mCurPathPoint < 0) {
            mCurPathPoint = mpPath->m_num - 1;
        }
    } else {
        if (mCurPathPoint > mpPath->m_num - 1) {
            mPathPointDir = -1;
            mCurPathPoint = mpPath->m_num - 2;
        } else if (mCurPathPoint < 0) {
            mPathPointDir = 1;
            mCurPathPoint = 0;
        }
    }

    mTargetPos = mpPath->mpPnt[mCurPathPoint].mPos;
}

/* 00001700-00001814       .text _draw__Q29daWindTag11daWindTag_cFv */
bool daWindTag::daWindTag_c::_draw() {
    if (!mbDraw)
        return TRUE;

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    mBtkAnm0.entry(mpModel->getModelData());
    mBtkAnm1.entry(mpModel->getModelData());
    mBckAnm.entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);

    if (mpEmitter != NULL) {
        g_env_light.settingTevStruct(TEV_TYPE_BG1, &current.pos, &mEfTevStr);
        mEfColor.r = mEfTevStr.mColorC0.r;
        mEfColor.g = mEfTevStr.mColorC0.g;
        mEfColor.b = mEfTevStr.mColorC0.b;
        mpEmitter->setGlobalPrmColor(mEfColor.r, mEfColor.g, mEfColor.b);
    }

    return TRUE;
}

/* 00001814-00001AA4       .text MoveEmitter__Q29daWindTag11daWindTag_cFv */
void daWindTag::daWindTag_c::MoveEmitter() {
    /* Nonmatching */
}

/* 00001B00-00001B20       .text daWindTag_Create__FPv */
static s32 daWindTag_Create(void* i_ac) {
    return ((daWindTag::daWindTag_c*)i_ac)->_create();
}

/* 00001B20-00001B44       .text daWindTag_Delete__FPv */
static BOOL daWindTag_Delete(void* i_ac) {
    return ((daWindTag::daWindTag_c*)i_ac)->_delete();
}

/* 00001B44-00001B68       .text daWindTag_Draw__FPv */
static BOOL daWindTag_Draw(void* i_ac) {
    return ((daWindTag::daWindTag_c*)i_ac)->_draw();
}

/* 00001B68-00001B8C       .text daWindTag_Execute__FPv */
static BOOL daWindTag_Execute(void* i_ac) {
    return ((daWindTag::daWindTag_c*)i_ac)->_execute();
}

/* 00001B8C-00001B94       .text daWindTag_IsDelete__FPv */
static BOOL daWindTag_IsDelete(void*) {
    return TRUE;
}

static actor_method_class daWindTagMethodTable = {
    (process_method_func)daWindTag_Create,
    (process_method_func)daWindTag_Delete,
    (process_method_func)daWindTag_Execute,
    (process_method_func)daWindTag_IsDelete,
    (process_method_func)daWindTag_Draw,
};

actor_process_profile_definition g_profile_WindTag = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_WindTag,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daWindTag::daWindTag_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ 0x00A8,
    /* Actor SubMtd */ &daWindTagMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
