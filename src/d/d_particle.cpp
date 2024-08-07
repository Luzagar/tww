//
// Generated by dtk
// Translation Unit: d_particle.cpp
//

#include "d/d_kankyo_wether.h"
#include "d/d_particle.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAEmitterManager.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "SSystem/SComponent/c_malloc.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_resorce.h"
#include "m_Do/m_Do_dvd_thread.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_lib.h"
#include "stdio.h"

static f32 dummy_2100[3] = {1.0f, 1.0f, 1.0f};
static f32 dummy_2080[3] = {1.0f, 1.0f, 1.0f};

static Vec dummy_3569;

/* 8007A4D8-8007A514       .text __ct__18dPa_modelEmitter_cFv */
dPa_modelEmitter_c::dPa_modelEmitter_c() {
    cNd_ForcedClear(this);
}

/* 8007A514-8007A804       .text __ct__21dPa_J3DmodelEmitter_cFP14JPABaseEmitterP12J3DModelDataR12dKy_tevstr_cP16J3DAnmTexPatternUsi */
dPa_J3DmodelEmitter_c::dPa_J3DmodelEmitter_c(JPABaseEmitter* param_1, J3DModelData* param_2, dKy_tevstr_c& param_3, J3DAnmTexPattern* param_4, u16 param_5, int param_6) {
    mpBaseEmitter = param_1;
    modelData = param_2;
    field_0x26 = param_6;
    tevStr = param_3;
    field_0x1c = param_4;
    if (!field_0x1c) {
        mpHeap = NULL;
        field_0x20 = NULL;
    } else {
        u16 r29 = field_0x1c->getUpdateMaterialNum();
        mpHeap = mDoExt_createSolidHeapToCurrent(r29 * 12 + 16, NULL, 0);
        if (mpHeap) {
            field_0x20 = new J3DTexNoAnm[r29];
            if (field_0x20) {
                field_0x1c->searchUpdateMaterialID(modelData);
                J3DTexNoAnm* tex = field_0x20;
                for (u16 i = 0; i < r29; i++) {
                    tex->setAnmIndex(i);
                    tex->setAnmTexPattern(field_0x1c);
                    tex++;
                }
                field_0x24 = param_5;
            }
            mDoExt_restoreCurrentHeap();
            mDoExt_adjustSolidHeap(mpHeap);
        }
    }
    param_1->becomeImmortalEmitter();
}

/* 8007A84C-8007A8C8       .text __dt__21dPa_J3DmodelEmitter_cFv */
dPa_J3DmodelEmitter_c::~dPa_J3DmodelEmitter_c()  {
    if (mpHeap) {
        mDoExt_destroySolidHeap(mpHeap);
    }
}

/* 8007A8C8-8007ADC4       .text draw__21dPa_J3DmodelEmitter_cFv */
void dPa_J3DmodelEmitter_c::draw() {
    /* Nonmatching */
    mpBaseEmitter->quitImmortalEmitter();

    JSUList<JPABaseParticle>* list = mpBaseEmitter->getParticleList();
    if (list != NULL) {
        Mtx mtx1, mtx2;
        mDoMtx_identity(mtx1);
        mDoMtx_identity(mtx2);
        for (JSULink<JPABaseParticle>* link = list->getFirst(); link != NULL; link = link->getNext()) {
            JPABaseParticle* ptcl = link->getObject();
            J3DModel* model = dPa_modelControl_c::newModel(modelData);
            if (model == NULL)
                return;

            mtx1[0][1] = ptcl->mVelocity.x;
            mtx1[1][1] = ptcl->mVelocity.y;
            mtx1[2][1] = ptcl->mVelocity.z;
            f32 sq = mtx1[0][1]*mtx1[0][1] + mtx1[1][1]*mtx1[1][1] + mtx1[2][1]*mtx1[2][1];
            if (!cM3d_IsZero(sq)) {
                if (!cM3d_IsZero(sq)) {
                    f32 mag = sqrtf(sq);
                    mtx1[0][1] *= mag;
                    mtx1[1][1] *= mag;
                    mtx1[2][1] *= mag;
                }
            } else {
                mtx1[0][1] = 0.0f;
                mtx1[1][1] = 1.0f;
                mtx1[2][1] = 0.0f;
            }

            // TODO: all the math
            mtx1[0][3] = ptcl->mPosition.x;
            mtx1[1][3] = ptcl->mPosition.y;
            mtx1[2][3] = ptcl->mPosition.z;
            g_env_light.setLightTevColorType(model, &tevStr);

            if (field_0x1c != NULL && field_0x20 != NULL) {
                field_0x1c->setFrame(field_0x24);
                modelData->getMaterialTable().setTexNoAnimator(field_0x1c, field_0x20);
            }

            cXyz scale;
            scale.x = mpBaseEmitter->mGlobalParticleScale.x * ptcl->mScaleX;
            scale.y = mpBaseEmitter->mGlobalParticleScale.y * ptcl->mScaleY;
            scale.z = mpBaseEmitter->mGlobalParticleScale.z * ptcl->mScaleX;

            model->setBaseScale(scale);
            model->setBaseTRMtx(mtx1);
            mDoExt_modelUpdate(model);
            if (field_0x1c != NULL) {
                modelData->getMaterialTable().removeTexNoAnimator(field_0x1c);
            }
        }
    }

    mpBaseEmitter->stopDrawParticle();
}

/* 8007ADC4-8007AECC       .text __ct__18dPa_modelControl_cFP12J3DModelData */
dPa_modelControl_c::dPa_modelControl_c(J3DModelData* modelData) {
    mModel = new dPa_J3Dmodel_c[0x80];
    JUT_ASSERT(0x10f, mModel != NULL);

    dPa_J3Dmodel_c * model = mModel;
    for (s32 i = 0; i < 0x80; i++, model++) {
        model->mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x37221223);
        JUT_ASSERT(0x120, model->mModel != NULL);
    }

    cLs_Create(this);
}

/* 8007AECC-8007AED8       .text __ct__14dPa_J3Dmodel_cFv */
dPa_J3Dmodel_c::dPa_J3Dmodel_c() {
    mInit = 0;
}

/* 8007AED8-8007AF64       .text __dt__18dPa_modelControl_cFv */
dPa_modelControl_c::~dPa_modelControl_c() {
    /* this cast is a guess */
    dPa_modelEmitter_c* node = (dPa_modelEmitter_c*)mpHead;
    while (node) {
        dPa_modelEmitter_c* nextNode = (dPa_modelEmitter_c*)node->mpNextNode;
        cLs_SingleCut(node);
        delete node;
        node = nextNode;
    }
}

dPa_J3Dmodel_c * dPa_modelControl_c::mModel;

/* 8007AF64-8007B074       .text newModel__18dPa_modelControl_cFP12J3DModelData */
J3DModel * dPa_modelControl_c::newModel(J3DModelData* modelData) {
    dPa_J3Dmodel_c * model = mModel;
    for (s32 i = 0; i < 0x80; i++, model++) {
        if (model->mInit == 0) {
            model->mInit = 1;
            J3DModel * j3dmodel = model->mModel;
            j3dmodel->mModelData = modelData;

            J3DShapePacket * shapePacket = j3dmodel->getShapePacket(0);
            for (u16 j = 0; j < modelData->getShapeNum(); shapePacket++, j++) {
                shapePacket->setShape(modelData->getShapeNodePointer(j));
            }

            J3DMatPacket * matPacket = j3dmodel->getMatPacket(0);
            for (u16 j = 0; j < modelData->getMaterialNum(); matPacket++, j++) {
                J3DMaterial* mat = modelData->getMaterialNodePointer(j);
                matPacket->setMaterial(mat);
                J3DShapePacket * shapePacket = j3dmodel->getShapePacket(mat->getShape()->getIndex());
                matPacket->setInitShapePacket(shapePacket);
                matPacket->addShapePacket(shapePacket);
                matPacket->setTexture(modelData->getTexture());
                matPacket->setDisplayListObj(mat->getSharedDisplayListObj());
            }

            j3dmodel->mVertexBuffer.setVertexData(&modelData->getVertexData());
            return model->mModel;
        }
    }

    return NULL;
}

/* 8007B074-8007B158       .text draw__18dPa_modelControl_cFv */
void dPa_modelControl_c::draw() {
    /* Nonmatching */
    for (u32 i = 0; i < 0x80; i++)
        mModel[i].mInit = 0;

    dKy_GxFog_set();

    for (node_class * nd = LIST_GET_HEAD(this); nd != NULL;) {
        node_class* pNext = NODE_GET_NEXT(nd);

        // TODO

        nd = pNext;
    }
}

/* 8007B158-8007B194       .text __ct__19dPa_followEcallBackFUcUc */
dPa_followEcallBack::dPa_followEcallBack(u8 param_1, u8 param_2) {
    mRateOff = param_1;
    field_0x12 = param_2;
    field_0x13 = 0;
    mpEmitter = NULL;
}

/* 8007B194-8007B288       .text execute__19dPa_followEcallBackFP14JPABaseEmitter */
void dPa_followEcallBack::execute(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007B288-8007B2C8       .text draw__19dPa_followEcallBackFP14JPABaseEmitter */
void dPa_followEcallBack::draw(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007B2C8-8007B360       .text setup__19dPa_followEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_followEcallBack::setup(JPABaseEmitter*, const cXyz*, const csXyz*, s8) {
    /* Nonmatching */
}

/* 8007B360-8007B3B0       .text end__19dPa_followEcallBackFv */
void dPa_followEcallBack::end() {
    /* Nonmatching */
}

/* 8007B3B0-8007B444       .text __ct__18dPa_smokeEcallBackFUc */
dPa_smokeEcallBack::dPa_smokeEcallBack(u8 param_1) {
    /* Nonmatching */
    GXColor local_18 = {0xa0, 0xa0, 0x80, 0xff};
    field_0x16 = local_18;
    mRateOff = param_1;
    field_0x12 = 0;
    mTevstr = NULL;
    field_0x14 = -1;
    field_0x15 = 0;
}

/* 8007B4B0-8007B558       .text __ct__18dPa_smokeEcallBackFUcUcUcUc */
dPa_smokeEcallBack::dPa_smokeEcallBack(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    /* Nonmatching */
    GXColor local_28 = {0xa0, 0xa0, 0x80, 0xff};
    field_0x16 = local_28;
    mRateOff = param_1;
    field_0x12 = param_2;
    field_0x13 = param_4;
    mTevstr = NULL;
    field_0x14 = -1;
    mFlag = param_3;
    field_0x15 = 0;

}

/* 8007B558-8007B5E8       .text __ct__18dPa_smokeEcallBackFRC8_GXColorP12dKy_tevstr_cUc */
dPa_smokeEcallBack::dPa_smokeEcallBack(const _GXColor& param_1, dKy_tevstr_c* param_2, u8 param_3) {
    field_0x16 = param_1;
    mTevstr = param_2;
    mRateOff = param_3;
    field_0x12 = 0;
    field_0x14 = -1;
}

/* 8007B5E8-8007B698       .text setup__18dPa_smokeEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_smokeEcallBack::setup(JPABaseEmitter* param_1, const cXyz* param_2, const csXyz* param_3, s8 param_4) {
    static dPa_smokePcallBack l_smokePcallBack;
    dPa_followEcallBack::setup(param_1, param_2, param_3, param_4);
    field_0x14 = param_4;
    param_1->mpParticleCallBack = &dPa_control_c::mSmokePcallback;
    param_1->mUserData = field_0x15;
}

/* 8007B73C-8007B804       .text initiateLighting__FR11_GXColorS10R8_GXColorR8_GXColor */
void initiateLighting(GXColorS10& dif, GXColor& amb, GXColor& k1) {
    GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, 1, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanAmbColor(GX_COLOR0A0, g_blackColor);
    GXSetTevColorS10(GX_TEVREG0, dif);
    GXSetTevKColor(GX_KCOLOR0, amb);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKColor(GX_KCOLOR1, k1);
    GXSetTevKColorSel(GX_TEVSTAGE2, GX_TEV_KCSEL_K1);
}

/* 8007B804-8007BB44       .text smokeEcallBack__FP14JPABaseEmitterP12dKy_tevstr_cSc8_GXColor */
void smokeEcallBack(JPABaseEmitter* emtr, dKy_tevstr_c* tevStr, s8, GXColor color) {
    GXFlush();
    GXLoadNrmMtxImm(JPADraw::cb.mDrawMtxPtr, GX_PNMTX0);
    GXInvalidateVtxCache();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    if (!(emtr->mDraw.field_0xc2 & 1)) {
        GXSetNumChans(1);
        if (tevStr == NULL) {
            GXColorS10 dif;
            dif.r = g_env_light.mActorC0.r;
            dif.g = g_env_light.mActorC0.g;
            dif.b = g_env_light.mActorC0.b;
            dif.a = 0xFF;
            GXColor amb;
            amb.r = g_env_light.mActorK0.r;
            amb.g = g_env_light.mActorK0.g;
            amb.b = g_env_light.mActorK0.b;
            amb.a = 0xFF;
            initiateLighting(dif, amb, color);
        } else {
            initiateLighting(tevStr->mColorC0, tevStr->mColorK0, color);
        }
        color.a = emtr->mGlobalPrmColor.a;
        GXSetNumTexGens(2);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_SRTG, GX_TG_COLOR0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        emtr->mDraw.loadTexture(1, GX_TEXMAP1);
        GXSetNumTevStages(3);                             
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    }
}

/* 8007BB44-8007BB80       .text draw__18dPa_smokeEcallBackFP14JPABaseEmitter */
void dPa_smokeEcallBack::draw(JPABaseEmitter* emtr) {
    smokeEcallBack(emtr, mTevstr, field_0x14, field_0x16);
}

/* 8007BB80-8007BBD8       .text end__18dPa_smokeEcallBackFv */
void dPa_smokeEcallBack::end() {
    if (mpEmitter == NULL)
        return;

    mpEmitter->becomeInvalidEmitter();
    mpEmitter->quitImmortalEmitter();
    mpEmitter->setEmitterCallBackPtr(&dPa_control_c::mSmokeEcallback);
    mFlag |= 1;
    mpEmitter = NULL;
}

/* 8007BBD8-8007BC84       .text dPa_setWindPower__FP15JPABaseParticle */
void dPa_setWindPower(JPABaseParticle* ptcl) {
    /* Nonmatching */
    cXyz pos(ptcl->mGlobalPosition);
    cXyz wind;
    f32 pow;
    dKyw_get_AllWind_vec(&pos, &wind, &pow);
    ptcl->mGlobalPosition.add(pos, wind * (pow * 8.0f));
}

/* 8007BC84-8007BCB4       .text execute__18dPa_smokePcallBackFP14JPABaseEmitterP15JPABaseParticle */
void dPa_smokePcallBack::execute(JPABaseEmitter* emtr, JPABaseParticle* ptcl) {
    if (emtr->mUserData == 0)
        dPa_setWindPower(ptcl);
}

/* 8007BCB4-8007C380       .text draw__18dPa_smokePcallBackFP14JPABaseEmitterP15JPABaseParticle */
void dPa_smokePcallBack::draw(JPABaseEmitter* emtr, JPABaseParticle* ptcl) {
    /* Nonmatching */
}

/* 8007C380-8007C3B0       .text draw__22dPa_selectTexEcallBackFP14JPABaseEmitter */
void dPa_selectTexEcallBack::draw(JPABaseEmitter* emtr) {
    emtr->mDraw.loadTexture(mTexNo, GX_TEXMAP0);
}

/* 8007C3B0-8007C420       .text __ct__19dPa_simpleEcallBackFv */
dPa_simpleEcallBack::dPa_simpleEcallBack() {
    mpBaseEmitter = NULL;
    mCount = 0;
}

/* 8007C45C-8007C460       .text __ct__16dPa_simpleData_cFv */
dPa_simpleData_c::dPa_simpleData_c() {}

/* 8007C460-8007C618       .text executeAfter__19dPa_simpleEcallBackFP14JPABaseEmitter */
void dPa_simpleEcallBack::executeAfter(JPABaseEmitter* param_1) {
    /* Nonmatching */
    s32 r28 = JPABaseEmitter::emtrInfo.mVolumeEmitCount;
    if (r28 <= 0) {
        mCount = 0;
    } else {
        dPa_simpleData_c* simpleData = mSimpleData;
        param_1->playCreateParticle();
        while (mCount != 0) {
            Vec local_38 = simpleData->mPos;
            if (mDoLib_clipper::mClipper.clip(j3dSys.getViewMtx(), local_38, 200.0f) == 0) {
                param_1->setGlobalTranslation(simpleData->mPos.x, simpleData->mPos.y, simpleData->mPos.z);
                param_1->setGlobalPrmColor(simpleData->mPrmColor.r, simpleData->mPrmColor.g, simpleData->mPrmColor.b);
                param_1->setGlobalAlpha(simpleData->mPrmColor.a);
                param_1->setGlobalEnvColor(simpleData->mEnvColor[0], simpleData->mEnvColor[1], simpleData->mEnvColor[2]);
                for (int i = 0; i < r28; i++) {
                    JPABaseParticle* particle = param_1->createParticle();
                    if (particle == NULL) {
                        break;
                    }
                    particle->setOffsetPosition(simpleData->mPos.x, simpleData->mPos.y, simpleData->mPos.z);
                    if (simpleData->mbAffectedByWind) {
                        static dPa_windPcallBack l_windPcallBack;
                        particle->setCallBackPtr(&l_windPcallBack);
                    }
                }
            }
            mCount--;
            simpleData++;
        }
    }
    param_1->stopCreateParticle();
}

/* 8007C674-8007C6EC       .text draw__19dPa_simpleEcallBackFP14JPABaseEmitter */
void dPa_simpleEcallBack::draw(JPABaseEmitter* emtr) {
    /* Nonmatching */
    if (mbIsSmoke)
        smokeEcallBack(emtr, NULL, -1, (GXColor){ 0xA0, 0xA0, 0x80, 0xFF });

    if (emtr->mGroupID == 4)
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
}

/* 8007C6EC-8007C774       .text create__19dPa_simpleEcallBackFP17JPAEmitterManagerUsUc */
JPABaseEmitter* dPa_simpleEcallBack::create(JPAEmitterManager* manager, u16 param_2, u8 param_3) {
    mpBaseEmitter = NULL;
    mGrpID = param_3;
    mResID = param_2;
    if (createEmitter(manager)) {
        if (param_2 == 0xa06a || param_2 == 0xa410) {
            mbIsSmoke = true;
            mpBaseEmitter->mpParticleCallBack = &dPa_control_c::mSmokePcallback;
        } else {
            mbIsSmoke = false;
        }
    }
    return mpBaseEmitter;
}

/* 8007C774-8007C840       .text createEmitter__19dPa_simpleEcallBackFP17JPAEmitterManager */
JPABaseEmitter* dPa_simpleEcallBack::createEmitter(JPAEmitterManager* manager) {
    if (!mpBaseEmitter) {
        static JGeometry::TVec3<f32> pos(0.0f, 0.0f, 0.0f);
        mpBaseEmitter = manager->createSimpleEmitterID(pos, mResID, mGrpID, dPa_control_c::getRM_ID(mResID), NULL, NULL);
        if (!mpBaseEmitter) {
            return mpBaseEmitter;
        }
        mpBaseEmitter->mpEmitterCallBack = this;
        mpBaseEmitter->mMaxFrame = 0;
        mpBaseEmitter->stopCreateParticle();
    }
    return mpBaseEmitter;
}

/* 8007C840-8007C8C4       .text set__19dPa_simpleEcallBackFPC4cXyzUcRC8_GXColorRC8_GXColori */
bool dPa_simpleEcallBack::set(const cXyz*, u8, const _GXColor&, const _GXColor&, int) {
    /* Nonmatching */
}

/* 8007C8C4-8007C8E8       .text execute__17dPa_windPcallBackFP14JPABaseEmitterP15JPABaseParticle */
void dPa_windPcallBack::execute(JPABaseEmitter* emtr, JPABaseParticle* ptcl) {
    dPa_setWindPower(ptcl);
}

GXColor l_lifeBallColor[] = {
    0xeb, 0x20, 0x78, 0xff,
    0x20, 0xf1, 0x9b, 0xff,
    0xeb, 0xd7, 0x2f, 0xff
};

dPa_selectTexEcallBack dPa_control_c::mTsubo[] = {
    dPa_selectTexEcallBack(0),
    dPa_selectTexEcallBack(1),
    dPa_selectTexEcallBack(2),
    dPa_selectTexEcallBack(3),
};

dPa_setColorEcallBack dPa_control_c::mLifeBall[] = {
    dPa_setColorEcallBack(l_lifeBallColor[0]),
    dPa_setColorEcallBack(l_lifeBallColor[1]),
    dPa_setColorEcallBack(l_lifeBallColor[2]),
};

JPAEmitterManager* dPa_control_c::mEmitterMng;
dPa_stripesEcallBack dPa_control_c::mStripes;
dPa_kageroEcallBack dPa_control_c::mKagero;
dPa_smokeEcallBack dPa_control_c::mSmokeEcallback(0, 1, 1, 1);
dPa_smokePcallBack dPa_control_c::mSmokePcallback;
dPa_singleRippleEcallBack dPa_control_c::mSingleRippleEcallBack;
dPa_ripplePcallBack dPa_control_c::mRipplePcallBack;
dPa_bombSmokeEcallBack dPa_control_c::mBombSmokeEcallBack;
u8 dPa_control_c::mStatus;

/* 8007C8E8-8007C9A4       .text __ct__13dPa_control_cFv */
dPa_control_c::dPa_control_c() {
    mHeap = mDoExt_createSolidHeapFromSystem(0x16e800, 0);
    JUT_ASSERT(0x4d8, mHeap != NULL);
    mSceneNo = 0xFF;
    mCount = 0;
    mEmitterMng = NULL;
    mNumSimple = 0;
    mCommonResMng = NULL;
    mSceneResMng = NULL;
}

/* 8007CA28-8007CA30       .text getRM_ID__13dPa_control_cFUs */
u8 dPa_control_c::getRM_ID(u16 uid) {
    return (uid >> 15) & 0x01;
}

/* 8007CA30-8007CA98       .text swapFrameBufferTexture__13dPa_control_cFv */
void dPa_control_c::swapFrameBufferTexture() {
    if (mCommonResMng) {
        mCommonResMng->swapTexture(mDoGph_gInf_c::mFrameBufferTimg, "AK_kagerouSwap00");
    }
    if (mSceneResMng) {
        mSceneResMng->swapTexture(mDoGph_gInf_c::mFrameBufferTimg, "AK_kagerouSwap00");
    }
}

/* 8007CA98-8007CCC8       .text createCommon__13dPa_control_cFPCv */
void dPa_control_c::createCommon(const void* param_1) {
    /* Nonmatching */
    mCommonResMng = new (mHeap, 0) JPAResourceManager(param_1, mHeap);
    JUT_ASSERT(1313, mCommonResMng != NULL);
    mCommonResMng->swapTexture(mDoGph_gInf_c::mFrameBufferTimg, "AK_kagerouSwap00");
    mEmitterMng = new(mHeap, 0) JPAEmitterManager(mCommonResMng, 3000, 150, 200, mHeap);
    JUT_ASSERT(1324, mEmitterMng != NULL);
    JKRHeap* oldHeap = mDoExt_setCurrentHeap(mHeap);
    mModelControl = new dPa_modelControl_c((J3DModelData*)dComIfG_getObjectRes("Always", ALWAYS_BDL_MPODA));
    JUT_ASSERT(1332, mModelControl != NULL);
    for (u16 i = 0; i < 8; i++) {
        u16 var1 = dPa_name::j_o_id[i];
        if (mCommonResMng->pEmtrRes->checkUserIndexDuplication(var1)) {
            newSimple(var1, var1 & 0x4000 ? 4 : 0);
        }
    }
    mNumSimpleCommon = mNumSimple;
    mDoExt_setCurrentHeap(oldHeap);
    mDoExt_adjustSolidHeap(mHeap);
}

/* 8007CCC8-8007CE50       .text createRoomScene__13dPa_control_cFPCv */
void dPa_control_c::createRoomScene(const void* param_1) {
    /* Nonmatching */
    mSceneHeap = mDoExt_createSolidHeapFromGame(0, 0);
    JUT_ASSERT(1369, mSceneHeap != NULL);
    mpData = param_1;
    mSceneResMng = new (mSceneHeap, 0) JPAResourceManager(mpData, mSceneHeap);
    JUT_ASSERT(1373, mSceneResMng != NULL);
    mSceneResMng->swapTexture(mDoGph_gInf_c::mFrameBufferTimg, "AK_kagerouSwap00");
    mEmitterMng->pResMgrArray[1] = mSceneResMng;
    mDoExt_adjustSolidHeap(mSceneHeap);
    for (u16 i = 0; i < 34; i++) {
        u16 var1 = dPa_name::s_o_id[i];
        if (mSceneResMng->pEmtrRes->checkUserIndexDuplication(var1)) {
            u32 var2;
            if (var1 & 0x4000) {
                var2 = 4;
            } else if (var1 & 0x2000) {
                var2 = 2;
            } else {
                var2 = 0;
            }
            newSimple(var1, var2);
        }
    }
}

/* 8007CE50-8007CF20       .text readScene__13dPa_control_cFUcPP21mDoDvdThd_toMainRam_c */
bool dPa_control_c::readScene(u8 i_no, mDoDvdThd_toMainRam_c** param_2) {
    if (i_no == 0xff) {
        return false;
    }
    if (mCount++) {
        JUT_ASSERT(1415, i_no != mSceneNo);
        return true;
    }
    mSceneNo = i_no;
    static char jpcName[32];
    sprintf(jpcName, "/res/Particle/Pscene%03d.jpc", i_no);
    *param_2 = mDoDvdThd_toMainRam_c::create(jpcName, 0, NULL);
    return true;
}

/* 8007CF20-8007CF98       .text createScene__13dPa_control_cFPCv */
void dPa_control_c::createScene(const void* data) {
    /* Nonmatching */
}

/* 8007CF98-8007D028       .text removeRoomScene__13dPa_control_cFv */
bool dPa_control_c::removeRoomScene() {
    if (mSceneHeap != NULL && mCount != 0 && --mCount == 0) {
        mEmitterMng->clearResourceManager(1);
        mDoExt_destroySolidHeap(mSceneHeap);
        mSceneHeap = NULL;
        mSceneResMng = NULL;
        JKRFree((void*)mpData);
        mNumSimple = mNumSimpleCommon;
        return true;
    } else {
        return false;
    }
}

/* 8007D028-8007D094       .text removeScene__13dPa_control_cFv */
void dPa_control_c::removeScene() {
    removeRoomScene();
    for (s32 i = 0; i < mNumSimpleCommon; i++)
        mSimpleCallbacks[i].mpBaseEmitter = NULL;
    mEmitterMng->forceDeleteAllEmitter();
    mModelControl->~dPa_modelControl_c();
}

/* 8007D094-8007D0DC       .text calc3D__13dPa_control_cFv */
void dPa_control_c::calc3D() {
    /* Nonmatching */
}

/* 8007D0DC-8007D124       .text calc2D__13dPa_control_cFv */
void dPa_control_c::calc2D() {
    /* Nonmatching */
}

/* 8007D124-8007D16C       .text calcMenu__13dPa_control_cFv */
void dPa_control_c::calcMenu() {
    /* Nonmatching */
}

/* 8007D16C-8007D1DC       .text draw__13dPa_control_cFP11JPADrawInfoUc */
void dPa_control_c::draw(JPADrawInfo* param_1, u8 param_2) {
    if (!mEmitterMng) {
        return;
    }
    j3dSys.reinitGX();
    dKy_setLight_again();
    mEmitterMng->draw(param_1, param_2);
    GXSetAlphaUpdate(GX_DISABLE);
    GXSetNumIndStages(0);
}

/* 8007D1DC-8007D378       .text set__13dPa_control_cFUcUsPC4cXyzPC5csXyzPC4cXyzUcP18dPa_levelEcallBackScPC8_GXColorPC8_GXColorPC4cXyz */
JPABaseEmitter* dPa_control_c::set(u8 groupID, u16 userID, const cXyz* pos, const csXyz* angle, const cXyz* scale, u8 alpha, dPa_levelEcallBack* pCallBack, s8 setupInfo, const GXColor* prm, const GXColor* env, const cXyz* globalScale) {
    /* Nonmatching */
    JPABaseEmitter* emtr = mEmitterMng->createSimpleEmitterID(*pos, userID, groupID, getRM_ID(userID), NULL, NULL);
    if (emtr == NULL)
        return NULL;

    if (angle != NULL)
        emtr->setGlobalRotation(*(JGeometry::TVec3<s16>*)angle);

    if (scale != NULL)
        emtr->setGlobalScale(*scale);

    emtr->mGlobalPrmColor.a = alpha;
    if (pCallBack != NULL) {
        emtr->setEmitterCallBackPtr(pCallBack);
        pCallBack->setup(emtr, pos, angle, setupInfo);
    } else if (!!(userID & 0x4000)) {
        emtr->setEmitterCallBackPtr(&mKagero);
    }

    if (prm != NULL)
        emtr->setGlobalPrmColor(prm->r, prm->g, prm->b);
    if (env != NULL)
        emtr->setGlobalEnvColor(env->r, env->g, env->b);
    if (globalScale != NULL)
        emtr->setGlobalParticleScale(*globalScale);
    return emtr;
}

/* 8007D378-8007D414       .text setBombSmoke__13dPa_control_cFUsPC4cXyzPC5csXyzPC4cXyzUc */
JPABaseEmitter* dPa_control_c::setBombSmoke(u16, const cXyz*, const csXyz*, const cXyz*, u8) {
    /* Nonmatching */
}

/* 8007D414-8007D998       .text setSimpleLand__13dPa_control_cFiPC4cXyzPC5csXyzfffP12dKy_tevstr_cPii */
void dPa_control_c::setSimpleLand(int type, const cXyz* pos, const csXyz* angle, f32, f32, f32, dKy_tevstr_c*, int*, int) {
    /* Nonmatching */
}

/* 8007D998-8007DA58       .text setSimpleLand__13dPa_control_cFR13cBgS_PolyInfoPC4cXyzPC5csXyzfffP12dKy_tevstr_cPii */
void dPa_control_c::setSimpleLand(cBgS_PolyInfo&, const cXyz*, const csXyz*, f32, f32, f32, dKy_tevstr_c*, int*, int) {
    /* Nonmatching */
}

/* 8007DA58-8007DAA8       .text checkAtrCodeEffect__13dPa_control_cFi */
void dPa_control_c::checkAtrCodeEffect(int) {
    /* Nonmatching */
}

/* 8007DAA8-8007DB34       .text setNormalStripes__13dPa_control_cFUsPC4cXyzPC5csXyzPC4cXyzUcUs */
void dPa_control_c::setNormalStripes(u16, const cXyz*, const csXyz*, const cXyz*, u8, u16) {
    /* Nonmatching */
}

/* 8007DB34-8007DBC4       .text newSimple__13dPa_control_cFUsUc */
bool dPa_control_c::newSimple(u16 param_1, u8 param_2) {
    /* Nonmatching */
    if (mNumSimple >= 25) {
        OSReport("\x1b[43;30m１エミッター登録数オーバー！！\n");
        return false;
    }
    JPABaseEmitter* emitter = mSimpleCallbacks[mNumSimple].create(mEmitterMng, param_1, param_2);
    if (!emitter) {
        return false;
    }
    mNumSimple++;
    return true;
}

/* 8007DBC4-8007DC30       .text setSimple__13dPa_control_cFUsPC4cXyzUcRC8_GXColorRC8_GXColori */
bool dPa_control_c::setSimple(u16 param_1, const cXyz* param_2, u8 param_3, const _GXColor& param_4, const _GXColor& param_5, int param_6) {
    dPa_simpleEcallBack* simple = getSimple(param_1);
    if (simple == NULL) {
        return false;
    }
    return simple->set(param_2, param_3, param_4, param_5, param_6);
}

/* 8007DC30-8007DC6C       .text getSimple__13dPa_control_cFUs */
dPa_simpleEcallBack* dPa_control_c::getSimple(u16 param_1) {
    /* Nonmatching */
    dPa_simpleEcallBack* simple = mSimpleCallbacks;
    for (int i = 0; i < mNumSimple; i++, simple++) {
        if (simple->mResID == param_1) {
            return simple;
        }
    }
    return NULL;
}

/* 8007DC6C-8007DC94       .text setup__19dPa_rippleEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_rippleEcallBack::setup(JPABaseEmitter* param_1, const cXyz* param_2, const csXyz* param_3, s8 param_4) {
    /* Nonmatching */
    param_1->setParticleCallBackPtr(&dPa_control_c::mRipplePcallBack);
    mPos = param_2;
    mRate = 1.0f;
    mFlags |= 1;
    mpBaseEmitter = param_1;
}

/* 8007DC94-8007DCE4       .text end__19dPa_rippleEcallBackFv */
void dPa_rippleEcallBack::end() {
    if (!mpBaseEmitter) {
        return;
    }
    mpBaseEmitter->becomeInvalidEmitter();
    mpBaseEmitter->quitImmortalEmitter();
    mpBaseEmitter->setEmitterCallBackPtr(NULL);
    mFlags |= 2;
    mpBaseEmitter = NULL;
}

/* 8007DCE4-8007DD70       .text execute__19dPa_rippleEcallBackFP14JPABaseEmitter */
void dPa_rippleEcallBack::execute(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007DD70-8007DDA8       .text draw__19dPa_rippleEcallBackFP14JPABaseEmitter */
void dPa_rippleEcallBack::draw(JPABaseEmitter*) {
    if (dPa_control_c::isStatus(1)) {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    }
}

/* 8007DDA8-8007DDB4       .text setup__25dPa_singleRippleEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_singleRippleEcallBack::setup(JPABaseEmitter* param_1, const cXyz*, const csXyz*, s8) {
    param_1->setParticleCallBackPtr(&dPa_control_c::mRipplePcallBack);
}

/* 8007DDB4-8007DE00       .text execute__25dPa_singleRippleEcallBackFP14JPABaseEmitter */
void dPa_singleRippleEcallBack::execute(JPABaseEmitter* param_1) {
    GXColor c1, c2;
    dKy_get_seacolor(&c1, &c2);
    param_1->setGlobalPrmColor(c1.r, c1.g, c1.b);
}

/* 8007DE00-8007DE38       .text draw__25dPa_singleRippleEcallBackFP14JPABaseEmitter */
void dPa_singleRippleEcallBack::draw(JPABaseEmitter*) {
    if (dPa_control_c::isStatus(1)) {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    }
}

/* 8007DE38-8007DE94       .text execute__19dPa_ripplePcallBackFP14JPABaseEmitterP15JPABaseParticle */
void dPa_ripplePcallBack::execute(JPABaseEmitter* param_1, JPABaseParticle* param_2) {
    /* Nonmatching */
    f32 tmp2;
    cXyz tmp1(param_2->mPosition.x, param_2->mPosition.y, param_2->mPosition.z);
    if (fopAcM_getWaterY(&tmp1, &tmp2)) {
        param_2->mGlobalPosition.y = tmp2;
    }
}

/* 8007DE94-8007E254       .text draw__19dPa_ripplePcallBackFP14JPABaseEmitterP15JPABaseParticle */
void dPa_ripplePcallBack::draw(JPABaseEmitter*, JPABaseParticle*) {
    /* Nonmatching */
}

/* 8007E254-8007E288       .text setup__17dPa_waveEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_waveEcallBack::setup(JPABaseEmitter* param_1, const cXyz* param_2, const csXyz* param_3, s8 param_4) {
    /* Nonmatching */
    mpPos = param_2;
    mpRot = param_3;
    mVel = 0.0f;
    mVelFade2 = 1.0f;
    mpBaseEmitter = param_1;
    mMaxParticleVelocity = 10000.0f;
    mState = 0;
    mFadeTimer = 0;
}

/* 8007E288-8007E2BC       .text remove__17dPa_waveEcallBackFv */
void dPa_waveEcallBack::remove() {
    if (!mpBaseEmitter) {
        return;
    }
    mpBaseEmitter->setEmitterCallBackPtr(NULL);
    mpBaseEmitter->becomeInvalidEmitter();
    mpBaseEmitter = NULL;
}

/* 8007E2BC-8007E484       .text executeAfter__17dPa_waveEcallBackFP14JPABaseEmitter */
void dPa_waveEcallBack::executeAfter(JPABaseEmitter* emitter) {
    /* Nonmatching - how to make TVec3 only exist in registers without being stored to the stack? */
    
    emitter->getEmitterAxis(mRotMtx[0], mRotMtx[1], mRotMtx[2]);
    
    if (mState != 0) {
        emitter->setDirectionalSpeed(0.0f);
        if (mFadeTimer > 0) {
            mFadeTimer--;
        } else {
            remove();
        }
        return;
    }
    
    JGeometry::TVec3<f32> delta;
    emitter->getGlobalTranslation(delta); // TODO
    emitter->setGlobalTranslation(*mpPos);
    delta.sub(*mpPos); // TODO
    f32 vel = delta.length();
    if (vel >= mMaxParticleVelocity) {
        vel = mMaxParticleVelocity;
    }
    f32 speed = mVelFade1 * vel * mVelFade2;
    
    JGeometry::TVec3<s16> rot;
    rot.x = 0;
    rot.y = mpRot->y;
    rot.z = 0;
    emitter->setGlobalRotation(rot);
    
    if (std::fabsf(speed - mVel) > mVelSpeed) {
        if (speed - mVel > 0.0f) {
            speed = mVel + mVelSpeed;
        } else {
            speed = mVel - mVelSpeed;
        }
    }
    
    emitter->setDirectionalSpeed(speed);
    mVel = speed;
}

/* 8007E484-8007E804       .text draw__17dPa_waveEcallBackFP14JPABaseEmitter */
void dPa_waveEcallBack::draw(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007E804-8007E81C       .text setup__19dPa_splashEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_splashEcallBack::setup(JPABaseEmitter* param_1, const cXyz* param_2, const csXyz* param_3, s8 param_4) {
    mpPos = param_2;
    mpRot = param_3;
    mpBaseEmitter = param_1;
    mState = 0;
}

/* 8007E81C-8007E850       .text remove__19dPa_splashEcallBackFv */
void dPa_splashEcallBack::remove() {
    if (!mpBaseEmitter) {
        return;
    }
    mpBaseEmitter->setEmitterCallBackPtr(NULL);
    mpBaseEmitter->becomeInvalidEmitter();
    mpBaseEmitter = NULL;
}

/* 8007E850-8007E9B8       .text execute__19dPa_splashEcallBackFP14JPABaseEmitter */
void dPa_splashEcallBack::execute(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007E9B8-8007E9D4       .text setup__22dPa_cutTurnEcallBack_cFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_cutTurnEcallBack_c::setup(JPABaseEmitter* param_1, const cXyz* param_2, const csXyz* param_3, s8 param_4) {
    mpBaseEmitter = param_1;
    field_0x6 = 0;
    field_0x5 = 0;
    field_0x4 = 0xFF;
}

/* 8007E9D4-8007EAC4       .text executeAfter__22dPa_cutTurnEcallBack_cFP14JPABaseEmitter */
void dPa_cutTurnEcallBack_c::executeAfter(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007EAC4-8007EB00       .text end__22dPa_cutTurnEcallBack_cFv */
void dPa_cutTurnEcallBack_c::end() {
    if (!mpBaseEmitter) {
        return;
    }
    mpBaseEmitter->setEmitterCallBackPtr(NULL);
    mpBaseEmitter->becomeInvalidEmitter();
    mpBaseEmitter = NULL;
    field_0x5 = 1;
}

/* 8007EB00-8007F028       .text draw__20dPa_stripesEcallBackFP14JPABaseEmitter */
void dPa_stripesEcallBack::draw(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007F028-8007F05C       .text draw__19dPa_kageroEcallBackFP14JPABaseEmitter */
void dPa_kageroEcallBack::draw(JPABaseEmitter*) {
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
}

/* 8007F05C-8007F108       .text execute__22dPa_bombSmokeEcallBackFP14JPABaseEmitter */
void dPa_bombSmokeEcallBack::execute(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007F108-8007F1A8       .text getMaxWaterY__18dPa_trackEcallBackFPQ29JGeometry8TVec3<f> */
void dPa_trackEcallBack::getMaxWaterY(JGeometry::TVec3<f32>*) {
    /* Nonmatching */
}

/* 8007F1A8-8007F1C0       .text setup__18dPa_trackEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_trackEcallBack::setup(JPABaseEmitter* param_1, const cXyz* param_2, const csXyz* param_3, s8 param_4) {
    mpPos = param_2;
    mState = 0;
    mpRot = param_3;
    mpBaseEmitter = param_1;
}

/* 8007F1C0-8007F1F4       .text remove__18dPa_trackEcallBackFv */
void dPa_trackEcallBack::remove() {
    if (!mpBaseEmitter) {
        return;
    }
    mpBaseEmitter->setEmitterCallBackPtr(NULL);
    mpBaseEmitter->becomeInvalidEmitter();
    mpBaseEmitter = NULL;
}

/* 8007F1F4-8007F3BC       .text execute__18dPa_trackEcallBackFP14JPABaseEmitter */
void dPa_trackEcallBack::execute(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007F3BC-8007F62C       .text draw__18dPa_trackEcallBackFP14JPABaseEmitter */
void dPa_trackEcallBack::draw(JPABaseEmitter*) {
    /* Nonmatching */
}

/* 8007F704-8007F708       .text setup__22dPa_bombSmokeEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_bombSmokeEcallBack::setup(JPABaseEmitter*, const cXyz*, const csXyz*, s8) {
}

/* 8007F7D0-8007F7D4       .text setup__20dPa_stripesEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_stripesEcallBack::setup(JPABaseEmitter*, const cXyz*, const csXyz*, s8) {
}

/* 8007FAE8-8007FAEC       .text setup__22dPa_selectTexEcallBackFP14JPABaseEmitterPC4cXyzPC5csXyzSc */
void dPa_selectTexEcallBack::setup(JPABaseEmitter*, const cXyz*, const csXyz*, s8) {
}
