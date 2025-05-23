//
// Generated by dtk
// Translation Unit: JPAParticle.cpp
//

#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPASweepShape.h"
#include "dolphin/mtx/mtxvec.h"

/* 8025DEF4-8025E62C       .text initParticle__15JPABaseParticleFv */
void JPABaseParticle::initParticle() {
    JPAEmitterInfo & emtrInfo = JPABaseEmitter::emtrInfo;
    JPABaseEmitter * emtr = emtrInfo.mpCurEmitter;

    JGeometry::TVec3<f32> velOmni;
    JGeometry::TVec3<f32> velAxis;
    JGeometry::TVec3<f32> velDir;
    JGeometry::TVec3<f32> velRndm;

    initStatus(JPAPtclStts_FirstFrame);
    mFieldAccel.zero();
    mFieldDrag = 1.0f;
    mDrag = 1.0f;
    MTXMultVec(emtrInfo.mEmitterGlobalSR, emtrInfo.mVolumePos, mLocalPosition);
    if ((emtr->mDataFlag & 0x08) != 0)
        setStatus(0x20);

    mOffsetPosition.set(emtrInfo.mEmitterGlobalCenter);

    if (emtr->mInitialVelOmni)
        velOmni.setLength(emtrInfo.mVelOmni, emtr->mInitialVelOmni);
    else
        velOmni.zero();

    if (emtr->mInitialVelAxis)
        velAxis.setLength(emtrInfo.mVelAxis, emtr->mInitialVelAxis);
    else
        velAxis.zero();

    if (emtr->mInitialVelDir) {
        Mtx mtx;
        JPAGetYZRotateMtx(
            emtr->getRandomRF() * 0x8000 * emtr->mSpread,
            emtr->getRandomSS(),
            mtx
        );
        MTXConcat(emtrInfo.mEmitterDirMtx, mtx, mtx);
        velDir.set(mtx[0][2], mtx[1][2], mtx[2][2]);
        velDir.scale(emtr->mInitialVelDir);
    } else {
        velDir.zero();
    }

    if (emtr->mInitialVelRndm) {
        velRndm.set(
            emtr->mInitialVelRndm * emtr->getRandomSF(),
            emtr->mInitialVelRndm * emtr->getRandomSF(),
            emtr->mInitialVelRndm * emtr->getRandomSF()
        );
    } else {
        velRndm.zero();
    }

    f32 velRatio = 1.0f + emtr->getRandomRF() * emtr->mInitialVelRatio;
    mBaseVel.set(
        velRatio * (velOmni.x + velAxis.x + velDir.x + velRndm.x),
        velRatio * (velOmni.y + velAxis.y + velDir.y + velRndm.y),
        velRatio * (velOmni.z + velAxis.z + velDir.z + velRndm.z)
    );

    if (emtr->checkEmDataFlag(0x04))
        mBaseVel.mul(emtr->mEmitterScale);

    MTXMultVec(emtrInfo.mEmitterGlobalRot, mBaseVel, mBaseVel);

    f32 accel = emtr->mAccel * (1.0f + emtr->getRandomRF() * emtr->mAccelRndm);
    mAccel.setLength(mBaseVel, accel);

    mAirResist = emtr->mAirResist + emtr->mAirResistRndm * emtr->getRandomSF();
    if (mAirResist > 1.0f)
        mAirResist = 1.0f;

    mMoment = emtr->mMoment * (1.0f - emtr->mMomentRndm * emtr->getRandomF());
    mCurFrame = -1.0f;
    mLifeTime = emtr->mLifeTime * (1.0f - emtr->mLifeTimeRndm * emtr->getRandomF());
    mCurNormTime = 0.0f;

    mGlobalPosition.set(
        mOffsetPosition.x + mLocalPosition.x * emtrInfo.mPublicScale.x,
        mOffsetPosition.y + mLocalPosition.y * emtrInfo.mPublicScale.y,
        mOffsetPosition.z + mLocalPosition.z * emtrInfo.mPublicScale.z
    );

    setCallBackPtr(emtr->mpParticleCallBack);
    emtr->mFieldManager.init(this);
    field_0xd0 = 0;
}

/* 8025E62C-8025EB28       .text initChild__15JPABaseParticleFP15JPABaseParticle */
void JPABaseParticle::initChild(JPABaseParticle* parent) {
    JPAEmitterInfo & emtrInfo = JPABaseEmitter::emtrInfo;
    JPABaseEmitter * emtr = emtrInfo.mpCurEmitter;
    JPASweepShape * sweep = emtr->mpDataLinkInfo->getSweepShape();

    initStatus(JPAPtclStts_FirstFrame | JPAPtlcStts_Child);

    if (!sweep->isEnableField()) {
        setStatus(0x40);
        mFieldDrag = 1.0f;
        mDrag = 1.0f;
    } else {
        mDrag = parent->mDrag;
        mFieldDrag = parent->mFieldDrag;
    }

    f32 baseVel = sweep->getBaseVel() * (1.0f + sweep->getBaseVelRndm() * emtr->getRandomRF());
    JGeometry::TVec3<f32> rndmVel(emtr->getRandomRF(), emtr->getRandomRF(), emtr->getRandomRF());
    rndmVel.setLength(baseVel);

    f32 baseVelInf = sweep->getVelInfRate();
    mBaseVel.x = rndmVel.x + parent->mBaseVel.x * baseVelInf;
    mBaseVel.y = rndmVel.y + parent->mBaseVel.y * baseVelInf;
    mBaseVel.z = rndmVel.z + parent->mBaseVel.z * baseVelInf;

    mFieldAccel.scale(sweep->getVelInfRate(), parent->mFieldVel);
    mAccel.set(0.0f, -sweep->getGravity(), 0.0f);
    mAirResist = parent->mAirResist;
    mMoment = parent->mMoment;
    mCurFrame = -1.0f;
    mLifeTime = sweep->getLife();
    mCurNormTime = 0.0f;
    mFieldVel.set(mFieldAccel);

    f32 velScale = mMoment * mDrag;
    JGeometry::TVec3<f32> vel;
    vel.x = (mBaseVel.x + mFieldVel.x) * velScale;
    vel.y = (mBaseVel.y + mFieldVel.y) * velScale;
    vel.z = (mBaseVel.z + mFieldVel.z) * velScale;
    mVelocity = vel;

    if (emtr->checkEmDataFlag(0x10))
        setStatus(0x20);

    mOffsetPosition.set(parent->mOffsetPosition);
    mLocalPosition.set(parent->mLocalPosition);

    f32 posRndm = sweep->getPosRndm();
    if (posRndm != 0.0f) {
        JGeometry::TVec3<f32> unit;
        JPAGetUnitVec(emtr->getRandomSS(), emtr->getRandomSS(), unit);
        unit.scale(posRndm * emtr->getRandomF());
        mLocalPosition.add(unit);
    }

    setCallBackPtr(emtr->mpParticleCallBack);
    field_0xd0 = parent->field_0xd0;
}

/* 8025EB28-8025EB90       .text incFrame__15JPABaseParticleFv */
void JPABaseParticle::incFrame() {
    mCurFrame += 1.0f;
    if (mCurFrame < 0.0f)
        mCurFrame = 0.0f;

    clearStatus(JPAPtclStts_FirstFrame);

    if (mCurFrame >= mLifeTime) {
        mCurNormTime = 1.0f;
        setStatus(0x02); // setDeleteParticleFlag?
    } else {
        mCurNormTime = mCurFrame / mLifeTime;
    }
}

/* 8025EB90-8025ECE8       .text calcVelocity__15JPABaseParticleFv */
void JPABaseParticle::calcVelocity() {
    mFieldVel.zero();
    if (checkStatus(0x20))
        mOffsetPosition.set(JPABaseEmitter::emtrInfo.mEmitterGlobalCenter);
    mBaseVel.add(mAccel);
    if (!checkStatus(0x40))
        JPABaseEmitter::emtrInfo.mpCurEmitter->mFieldManager.calc(this);
    mFieldVel.add(mFieldAccel);

    mBaseVel.scale(mAirResist);

    f32 velScale = mMoment * mDrag;
    JGeometry::TVec3<f32> vel;
    vel.x = (mBaseVel.x + mFieldVel.x) * velScale;
    vel.y = (mBaseVel.y + mFieldVel.y) * velScale;
    vel.z = (mBaseVel.z + mFieldVel.z) * velScale;

    mVelocity = vel;
}

/* 8025ECE8-8025ED6C       .text calcPosition__15JPABaseParticleFv */
void JPABaseParticle::calcPosition() {
    mLocalPosition.add(mVelocity);

    mGlobalPosition.set(
        mLocalPosition.x * JPABaseEmitter::emtrInfo.mPublicScale.x + mOffsetPosition.x,
        mLocalPosition.y * JPABaseEmitter::emtrInfo.mPublicScale.y + mOffsetPosition.y,
        mLocalPosition.z * JPABaseEmitter::emtrInfo.mPublicScale.z + mOffsetPosition.z
    );
}

/* 8025ED6C-8025EE44       .text checkCreateChild__15JPABaseParticleFv */
bool JPABaseParticle::checkCreateChild() {
    JPASweepShape * ssp = JPABaseEmitter::emtrInfo.mpCurEmitter->mpDataLinkInfo->getSweepShape();
    bool ret = false;
    f32 time = mLifeTime > 1.0f ? (mCurFrame / (mLifeTime - 1.0f)) : 1.0f;
    if (time >= ssp->getTiming()) {
        f32 frame = mCurFrame;
        s32 step = (ssp->getStep() & 0xFF) + 1;
        if (((s32)frame % step) == 0)
            ret = true;
    }
    return ret;
}
