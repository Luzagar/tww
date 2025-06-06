/**
 * d_a_obj_ferris.cpp
 * Object - Windfall Island - Ferris wheel
 */

#include "d/actor/d_a_obj_ferris.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_wether.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/res/res_skanran.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"

#include "weak_bss_936_to_1036.h" // IWYU pragma: keep

namespace daObjFerris {
    namespace {
        static const Attr_c L_attr = { 45, 0x01, 50.0f, 195.0f, { 0.0f, -100.0f, -135.0f }, 1820.0f, 0.0f, 30.0f };
        inline const Attr_c & attr() { return L_attr; }
    }

    const dCcD_SrcCyl M_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ ~(AT_TYPE_LIGHT | AT_TYPE_UNK400000 | AT_TYPE_WIND | AT_TYPE_UNK20000 | AT_TYPE_WATER),
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_GrpAll_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e,
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
            /* Height */ 200.0f,
        },
    };

    const dCcD_SrcSph M_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ 0,
            /* SrcObjTg  SPrm    */ 0,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e,
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
        // cM3dGSphS
        {
            /* Center */ 0.0f, 0.0f, 0.0f,
            /* Radius */ 35.0f,
        },
    };
};

const char daObjFerris::Act_c::M_arcname[8] = "Skanran";

/* 000000EC-00000110       .text solidHeapCB__Q211daObjFerris5Act_cFP10fopAc_ac_c */
BOOL daObjFerris::Act_c::solidHeapCB(fopAc_ac_c* i_this) {
    return ((Act_c*)i_this)->create_heap();
}

/* 00000110-0000048C       .text create_heap__Q211daObjFerris5Act_cFv */
bool daObjFerris::Act_c::create_heap() {
    s32 i;
    J3DModelData* mdl_data_gondola = static_cast<J3DModelData*>(dComIfG_getObjectRes(M_arcname, SKANRAN_BDL_SGONDOR));
    JUT_ASSERT(0x183, mdl_data_gondola != NULL);
    if (mdl_data_gondola != NULL) {
        for (i = 0; i < 5; i++)
            mpModel[i] = mDoExt_J3DModel__create(mdl_data_gondola, 0, 0x11020203);
    }

    J3DModelData* mdl_data_wheelbase = static_cast<J3DModelData*>(dComIfG_getObjectRes(M_arcname, SKANRAN_BDL_SKANRAN));
    JUT_ASSERT(0x18c, mdl_data_wheelbase != NULL);
    if (mdl_data_wheelbase != NULL) {
        mpModel[5] = mDoExt_J3DModel__create(mdl_data_wheelbase, 0, 0x11020203);
    }

    if (mdl_data_gondola != NULL && mdl_data_wheelbase != NULL)
        init_mtx();

    cBgD_t* bgw_data_gondola = static_cast<cBgD_t*>(dComIfG_getObjectRes(M_arcname, SKANRAN_DZB_SGONDOR));
    JUT_ASSERT(0x1a0, bgw_data_gondola != NULL);
    if (bgw_data_gondola != NULL) {
        for (i = 0; i < 5; i++) {
            mpBgW[i] = new dBgW();
            if (mpBgW[i] != NULL && mpBgW[i]->Set(bgw_data_gondola, dBgW::MOVE_BG_e, &mMtx[i]) == true)
                return false;
        }
    }

    cBgD_t* bgw_data_wheelbase = static_cast<cBgD_t*>(dComIfG_getObjectRes(M_arcname, SKANRAN_DZB_SKANRAN));
    JUT_ASSERT(0x1b0, bgw_data_wheelbase != NULL);
    if (bgw_data_wheelbase != NULL) {
        mpBgW[5] = new dBgW();
        if (mpBgW[5] != NULL && mpBgW[5]->Set(bgw_data_wheelbase, dBgW::MOVE_BG_e, &mMtx[5]) == true)
            return false;
    }

    return ((mdl_data_gondola != NULL && mpModel[0] != NULL && mpModel[1] != NULL && mpModel[2] != NULL && mpModel[3] != NULL && mpModel[4] != NULL) &&
            (mdl_data_wheelbase != NULL && mpModel[5] != NULL) &&
            (bgw_data_gondola != NULL && mpBgW[0] != NULL && mpBgW[1] != NULL && mpBgW[2] != NULL && mpBgW[3] != NULL && mpBgW[4] != NULL) &&
            (bgw_data_wheelbase != NULL && mpBgW[5] != NULL));
}

/* 0000048C-000004DC       .text ride_call_back__Q211daObjFerris5Act_cFP4dBgWP10fopAc_ac_cP10fopAc_ac_c */
void daObjFerris::Act_c::ride_call_back(dBgW* bgw, fopAc_ac_c* i_ac, fopAc_ac_c* i_pt) {
    Act_c* i_this = (Act_c*)i_ac;
    for (s32 i = 0; i < 5; i++) {
        if (i_this->mpBgW[i] == bgw) {
            i_this->mRidePos = i_pt->current.pos;
            i_this->mRideState[i] = 1;
            break;
        }
    }
}

/* 000004DC-00000898       .text _create__Q211daObjFerris5Act_cFv */
cPhs_State daObjFerris::Act_c::_create() {
    fopAcM_SetupActor(this, Act_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, M_arcname);

    s32 i;
    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0x11a00)) {
            fopAcM_SetMtx(this, mpModel[5]->getBaseTRMtx());
            for (i = 0; i < 6; i++) {
                dComIfG_Bgsp()->Regist(mpBgW[i], this);
                mpBgW[i]->SetCrrFunc(dBgS_MoveBGProc_Typical);
                if (i < 5)
                    mpBgW[i]->SetRideCallback(ride_call_back);
            }
            fopAcM_setCullSizeBox(this, -1400.0f, -1400.0f, -500.f, 1400.0f, 1400.0f, 800.0f);
            fopAcM_setCullSizeFar(this, 10.0f);
            if (is_switch() == 1) {
                mRotSpeed = 45;
                mRotState = 1;
            } else {
                mRotSpeed = 0;
                mRotState = 0;
            }

            mEventIdx_kanban_vive = dComIfGp_evmng_getEventIdx("kanran_vive");
            mEventIdx_kanban_start = dComIfGp_evmng_getEventIdx("kanran_start");
            mRotAngle = 0x1800;
            mGondolaWaveAngle = 0;

            for (i = 0; i < 5; i++) {
                field_0x02d4[i].Init(0xFF, 0xFF, this);
                field_0x0400[i].Set(M_cyl_src);
                field_0x0400[i].SetStts(&field_0x02d4[i]);
                field_0x0400[i].SetTgVec((cXyz&)cXyz::Zero);
                field_0x0400[i].OnTgNoHitMark();

                field_0x09f0[i].Init(0xFF, 0xFF, this);
                field_0x0b1c[i].Set(M_cyl_src);
                field_0x0b1c[i].SetStts(&field_0x09f0[i]);
                field_0x0b1c[i].SetTgVec((cXyz&)cXyz::Zero);
                field_0x0b1c[i].OnTgNoHitMark();

                field_0x110c[i].Init(0xFF, 0xFF, this);
                field_0x1238[i].Set(M_sph_src);
                field_0x1238[i].SetStts(&field_0x110c[i]);
                field_0x1238[i].SetTgVec((cXyz&)cXyz::Zero);
                field_0x1238[i].OnTgNoHitMark();
            }
        } else {
            ret = cPhs_ERROR_e;
        }
    }

    return ret;
}

/* 00000DE8-00000EA8       .text _delete__Q211daObjFerris5Act_cFv */
bool daObjFerris::Act_c::_delete() {
    if (heap != NULL) {
        for (s32 i = 0; i < 6; i++) {
            if (mpBgW[i] != NULL && mpBgW[i]->ChkUsed()) {
                mpBgW[i]->SetRideCallback(NULL);
                dComIfG_Bgsp()->Release(mpBgW[i]);
            }
        }
    }

    dComIfG_resDelete(&mPhs, M_arcname);
    return true;
}

/* 00000EA8-000011B8       .text set_mtx__Q211daObjFerris5Act_cFi */
void daObjFerris::Act_c::set_mtx(int idx) {
    static cXyz offset[6] = {
        cXyz(0.56f, 1078.13f, 162.68f),
        cXyz(1026.17f, 332.98f, 162.68f),
        cXyz(634.42f, -872.69f, 162.68f),
        cXyz(-633.3f, -872.69f, 162.68f),
        cXyz(-1025.04f, 332.98f, 162.68f),
        cXyz(0.0f, 0.0f, 0.0f),
    };

    if (idx < 5) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::ZrotM(mRotAngle);
        mDoMtx_stack_c::transM(offset[idx]);
        mDoMtx_stack_c::ZrotM(-mRotAngle);
        mDoMtx_stack_c::ZrotM(mGondolaWaveAngle);
        mpModel[idx]->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_copy(mDoMtx_stack_c::get(), mMtx[idx]);
    } else if (idx == 5) {
        cXyz pos = current.pos;
        VECAdd(&pos, &offset[idx], &pos);
        mDoMtx_stack_c::transS(pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::ZrotM(mRotAngle);
        mpModel[idx]->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_copy(mDoMtx_stack_c::get(), mMtx[idx]);
    }
}

/* 000011B8-00001240       .text init_mtx__Q211daObjFerris5Act_cFv */
void daObjFerris::Act_c::init_mtx() {
    for (s32 i = 0; i < 6; i++) {
        mpModel[i]->setBaseScale(scale);
        set_mtx(i);
        mpModel[i]->calc();
    }
}

/* 00001240-0000126C       .text now_event__Q211daObjFerris5Act_cFs */
bool daObjFerris::Act_c::now_event(s16 p1) {
    return mEventState != 0 && mEventIdx == p1;
}

/* 0000126C-00001294       .text set_event__Q211daObjFerris5Act_cFs */
BOOL daObjFerris::Act_c::set_event(s16 p1) {
    if (mEventState == 0) {
        mEventIdx = p1;
        mEventState = 1;
        return TRUE;
    }
    return FALSE;
}

/* 00001294-0000135C       .text exe_event__Q211daObjFerris5Act_cFv */
void daObjFerris::Act_c::exe_event() {
    switch (mEventState) {
    case 1:
        if (eventInfo.checkCommandDemoAccrpt()) {
            mEventState = 2;
        } else {
            fopAcM_orderOtherEventId(this, mEventIdx);
            eventInfo.onCondition(dEvtCnd_UNK2_e);
        }
        break;
    case 2:
        if (dComIfGp_evmng_endCheck(mEventIdx)) {
            dComIfGp_event_reset();
            mEventIdx = -1;
            mEventState = 0;
        }
        break;
    }
}

/* 0000135C-000013B4       .text angle_mng__Q211daObjFerris5Act_cFv */
void daObjFerris::Act_c::angle_mng() {
    mGondolaWaveTimer += 500;
    mGondolaWaveAngle = cM_ssin(mGondolaWaveTimer) * 380.0f;
}

/* 000013B4-000016C0       .text rot_mng__Q211daObjFerris5Act_cFv */
void daObjFerris::Act_c::rot_mng() {
    switch (mRotState) {
    case 0:
        mRotSpeed = 0;
        mRotAngle = 0x1800;
        if (is_switch()) {
            cXyz* wind = dKyw_get_wind_vec();
            s16 windAngle = cM_atan2s(wind->x, wind->z);
            if (windAngle == -0x8000) {
                if (set_event(mEventIdx_kanban_start)) {
                    mRotState = 6;
                    mRotSpeed = 0;
                    mRotTimer = 0;
                    mDoAud_seStart(JA_SE_READ_RIDDLE_1);
                }
            } else {
                if (set_event(mEventIdx_kanban_vive)) {
                    mRotState = 2;
                    mRotSpeed = 0;
                    mRotTimer = 0;
                    off_switch();
                }
            }
        }
        break;
    case 2:
    case 6:
        mRotTimer++;
        if (mRotTimer > 20) {
            mRotTimer = 0;
            mRotState++;
        }
        break;
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
        if (mRotTimer < 11) {
            mRotSpeed = 4;
        } else {
            switch (mRotState) {
            case 9:
            case 5:
                if (!(mFrameTimer & 3))
                    mRotSpeed--;
                break;
            default:
                mRotSpeed--;
                break;
            }
        }

        mRotTimer++;
        if (mRotSpeed < 0) {
            mRotSpeed = 0;
            mRotTimer = 0;

            switch (mRotState) {
            case 6:
                break;
            case 3:
            case 4:
            case 7:
            case 8:
                mRotState++;
                break;
            case 5:
                mRotState = 10;
                break;
            case 9:
                mRotState = 1;
                break;
            }
        }
        break;
    case 10:
        mRotSpeed = -5;
        if (mRotAngle < 0x1800 || !now_event(mEventIdx_kanban_vive)) {
            mRotState = 0;
            mRotSpeed = 0;
        }
        break;
    case 1:
        dComIfGs_onEventBit(0x2104);
        if (!(mFrameTimer & 3))
            mRotSpeed++;

        if (now_event(mEventIdx_kanban_start)) {
            if (mRotSpeed > 67)
                mRotSpeed = 67;
        } else {
            if (mRotSpeed > 45)
                mRotSpeed = 45;
        }
        angle_mng();
        break;
    }
}

/* 000016C0-00001A50       .text set_collision__Q211daObjFerris5Act_cFv */
void daObjFerris::Act_c::set_collision() {
    for (s32 i = 0; i < 5; i++) {
        static cXyz zero_offset(0.0f, 0.0f, 0.0f);
        cXyz pt;
        mDoMtx_multVec(mMtx[i], &zero_offset, &pt);
        if (pt.y <= 1820.0f) {
            field_0x0400[i].SetR(50.0f);
            field_0x0400[i].SetH(195.0f);
            cXyz offs0(0.0f, -100.0f, -135.0f);
            mDoMtx_multVec(mMtx[i], &offs0, &pt);
            field_0x0400[i].SetC(pt);
            dComIfG_Ccsp()->Set(&field_0x0400[i]);

            field_0x0b1c[i].SetR(50.0f);
            field_0x0b1c[i].SetH(148.0f);
            cXyz offs1(0.0f, -56.0f, 127.0f);
            mDoMtx_multVec(mMtx[i], &offs1, &pt);
            field_0x0b1c[i].SetC(pt);
            dComIfG_Ccsp()->Set(&field_0x0b1c[i]);
        }
    }

    cXyz offsets[5] = {
        cXyz(-993.54f, -325.78f, 37.63f),
        cXyz(-616.86f, 844.24f, 37.63f),
        cXyz(612.3f, 847.55f, 37.63f),
        cXyz(995.28f, -320.43f, 37.63f),
        cXyz(2.82f, -1045.59f, 37.63f),
    };

    for (s32 i = 0; i < 5; i++) {
        static cXyz zero_offset(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::ZrotM(mRotAngle);
        mDoMtx_stack_c::transM(offsets[i]);
        mDoMtx_stack_c::ZrotM(-mRotAngle);
        mDoMtx_stack_c::ZrotM(mGondolaWaveAngle + mRideWaveAngle[i]);

        Mtx now;
        mDoMtx_copy(mDoMtx_stack_c::get(), now);
        cXyz pt;
        mDoMtx_multVec(now, &zero_offset, &pt);

        if (pt.y <= 1820.0f) {
            field_0x1238[i].SetC(pt);
            field_0x1238[i].SetR(30.0f);
            dComIfG_Ccsp()->Set(&field_0x1238[i]);
        }
    }
}

/* 00001A50-00001C30       .text make_lean__Q211daObjFerris5Act_cFv */
void daObjFerris::Act_c::make_lean() {
    cXyz pt0;
    cXyz pt1;
    cXyz offs1(0.0f, -100.0f, -135.0f);
    cXyz offs0(0.0f, 0.0f, 0.0f);
    cXyz delta;
    cXyz delta2;

    Mtx* mtx;
    s32* pRideState; // Fakematch?
    for (s32 i = 0; i < 5; i++) {
        pRideState = &mRideState[i];
        if (*pRideState == 1) {
            mtx = &mMtx[i];
            mDoMtx_multVec(*mtx, &offs0, &pt0);
            mDoMtx_multVec(*mtx, &offs1, &pt1);

            delta.x = pt1.x - pt0.x;
            delta.y = 0.0f;
            delta.z = pt1.z - pt0.z;

            delta2.set(mRidePos.x - pt0.x, 0.0f, mRidePos.z - pt0.z);
            delta.normalizeRS();

            f32 z = -delta.z;
            f32 h = std::fabsf(-(z * pt0.x + delta.x * pt0.z) + (z * mRidePos.x + delta.x * mRidePos.z)) / 162.0f;
            if (delta.x * delta2.z - delta.z * delta2.x < 0.0f) {
                mRideWaveTarget[i] = h * 550.0f;
            } else {
                mRideWaveTarget[i] = h * -550.0f;
            }
        } else {
            mRideWaveTarget[i] = 0;
        }

        cLib_addCalcAngleS2(&mRideWaveAngle[i], mRideWaveTarget[i], 4, 0x1000);
        *pRideState = 0;
    }
}

/* 00001C30-00001D58       .text _execute__Q211daObjFerris5Act_cFv */
bool daObjFerris::Act_c::_execute() {
    mRotAngle += mRotSpeed;
    f32 volume = (mRotSpeed / 45.0f) * 100.0f;
    volume = volume > 100.0f ? 100.0f : volume;
    mDoAud_seStart(JA_SE_OBJ_FER_WHEEL_ROUND, &current.pos, volume);
    rot_mng();
    exe_event();
    make_lean();
    for (s32 i = 0; i < 6; i++) {
        set_mtx(i);
        if (i < 6)
            mpBgW[i]->Move();
    }
    set_collision();
    mFrameTimer++;
    return true;
}

/* 00001D58-00001E18       .text _draw__Q211daObjFerris5Act_cFv */
bool daObjFerris::Act_c::_draw() {
    dKy_getEnvlight().settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    dComIfGd_setListBG();
    for (s32 i = 0; i < 6; i++) {
        dKy_getEnvlight().setLightTevColorType(mpModel[i], &tevStr);
        mDoExt_modelUpdateDL(mpModel[i]);
    }
    dComIfGd_setList();
    return true;
}

namespace daObjFerris {
    namespace {
        cPhs_State Mthd_Create(void* i_this) {
            return ((Act_c*)i_this)->_create();
        }

        BOOL Mthd_Delete(void* i_this) {
            return ((Act_c*)i_this)->_delete();
        }

        BOOL Mthd_Execute(void* i_this) {
            return ((Act_c*)i_this)->_execute();
        }

        BOOL Mthd_Draw(void* i_this) {
            return ((Act_c*)i_this)->_draw();
        }

        BOOL Mthd_IsDelete(void* i_this) {
            return TRUE;
        }

        static actor_method_class Mthd_Table = {
            (process_method_func)daObjFerris::Mthd_Create,
            (process_method_func)daObjFerris::Mthd_Delete,
            (process_method_func)daObjFerris::Mthd_Execute,
            (process_method_func)daObjFerris::Mthd_IsDelete,
            (process_method_func)daObjFerris::Mthd_Draw,
        };
    }
}

actor_process_profile_definition g_profile_Obj_Ferris = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Ferris,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjFerris::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Ferris,
    /* Actor SubMtd */ &daObjFerris::Mthd_Table,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
