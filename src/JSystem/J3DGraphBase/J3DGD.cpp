//
// Generated by dtk
// Translation Unit: J3DGD.cpp
//

#include "JSystem/J3DGraphBase/J3DGD.h"
#include "dolphin/types.h"
#include "dolphin/os/OS.h"

/* 802D5F38-802D60B0       .text J3DGDSetGenMode__FUcUcUcUc11_GXCullMode */
void J3DGDSetGenMode(u8 texGenNum, u8 colorChanNum, u8 tevNum, u8 indTevNum, GXCullMode cull) {
    static u8 cm2hw[] = {
        /* GX_CULL_NONE  */ 0,
        /* GX_CULL_FRONT */ 2,
        /* GX_CULL_BACK  */ 1,
        /* GX_CULL_ALL   */ 3,
    };
    GDOverflowCheck(10);

    J3DGDWriteBPCmd(0x07FC3F | 0xFE << 24);
    J3DGDWriteBPCmd(texGenNum << 0 | colorChanNum << 4 | (tevNum - 1) << 10 | cm2hw[cull] << 14 | indTevNum << 16 | 0x00 << 24);
}

u8 J3DGDTexMode0Ids[] = {
    GX_BP_REG_SETMODE0_TEX0,
    GX_BP_REG_SETMODE0_TEX1,
    GX_BP_REG_SETMODE0_TEX2,
    GX_BP_REG_SETMODE0_TEX3,
    GX_BP_REG_SETMODE0_TEX4,
    GX_BP_REG_SETMODE0_TEX5,
    GX_BP_REG_SETMODE0_TEX6,
    GX_BP_REG_SETMODE0_TEX7,
};
u8 J3DGDTexMode1Ids[] = {
    GX_BP_REG_SETMODE1_TEX0,
    GX_BP_REG_SETMODE1_TEX1,
    GX_BP_REG_SETMODE1_TEX2,
    GX_BP_REG_SETMODE1_TEX3,
    GX_BP_REG_SETMODE1_TEX4,
    GX_BP_REG_SETMODE1_TEX5,
    GX_BP_REG_SETMODE1_TEX6,
    GX_BP_REG_SETMODE1_TEX7,
};
u8 J3DGDTexImage0Ids[] = {
    GX_BP_REG_SETIMAGE0_TEX0,
    GX_BP_REG_SETIMAGE0_TEX1,
    GX_BP_REG_SETIMAGE0_TEX2,
    GX_BP_REG_SETIMAGE0_TEX3,
    GX_BP_REG_SETIMAGE0_TEX4,
    GX_BP_REG_SETIMAGE0_TEX5,
    GX_BP_REG_SETIMAGE0_TEX6,
    GX_BP_REG_SETIMAGE0_TEX7,
};
u8 J3DGDTexImage3Ids[] = {
    GX_BP_REG_SETIMAGE3_TEX0,
    GX_BP_REG_SETIMAGE3_TEX1,
    GX_BP_REG_SETIMAGE3_TEX2,
    GX_BP_REG_SETIMAGE3_TEX3,
    GX_BP_REG_SETIMAGE3_TEX4,
    GX_BP_REG_SETIMAGE3_TEX5,
    GX_BP_REG_SETIMAGE3_TEX6,
    GX_BP_REG_SETIMAGE3_TEX7,
};
u8 J3DGDTexTlutIds[] = {
    GX_BP_REG_SETTLUT_TEX0,
    GX_BP_REG_SETTLUT_TEX1,
    GX_BP_REG_SETTLUT_TEX2,
    GX_BP_REG_SETTLUT_TEX3,
    GX_BP_REG_SETTLUT_TEX4,
    GX_BP_REG_SETTLUT_TEX5,
    GX_BP_REG_SETTLUT_TEX6,
    GX_BP_REG_SETTLUT_TEX7,
};
static u8 GX2HWFiltConv[] = {
    /* GX_NEAR          */ 0,
    /* GX_LINEAR        */ 4,
    /* GX_NEAR_MIP_NEAR */ 1,
    /* GX_LIN_MIP_NEAR  */ 5,
    /* GX_NEAR_MIP_LIN  */ 2,
    /* GX_LIN_MIP_LIN   */ 6,
};

/* 802D60B0-802D6204       .text J3DGDSetGenMode_3Param__FUcUcUc */
void J3DGDSetGenMode_3Param(u8 texGenNum, u8 tevNum, u8 indTevNum) {
    GDOverflowCheck(10);
    J3DGDWriteBPCmd(0x073C0F | 0xFE << 24);
    J3DGDWriteBPCmd(texGenNum | (tevNum - 1) << 10 | indTevNum << 16 | 0x00 << 24);
}

/* 802D6204-802D632C       .text J3DGDSetIndTexStageNum__FUl */
void J3DGDSetIndTexStageNum(u32 indTevNum) {
    GDOverflowCheck(10);
    J3DGDWriteBPCmd(0x070000 | 0xFE << 24);
    J3DGDWriteBPCmd(indTevNum << 16 | 0x00 << 24);
}

inline u16 __GDLightID2Index(GXLightID id) {
    u16 lightIdx = (31 - __cntlzw(id));
    if (lightIdx > 7)
        lightIdx = 0;
    return lightIdx;
}

inline u16 __GDLightID2Offset(GXLightID id) {
    return __GDLightID2Index(id) * 0x10;
}

/* 802D632C-802D6624       .text J3DGDSetLightAttn__F10_GXLightIDffffff */
void J3DGDSetLightAttn(GXLightID id, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2) {
    J3DGDWriteXFCmdHdr(0x0604 + __GDLightID2Offset(id), 6);
    J3DGDWrite_f32(a0);
    J3DGDWrite_f32(a1);
    J3DGDWrite_f32(a2);
    J3DGDWrite_f32(k0);
    J3DGDWrite_f32(k1);
    J3DGDWrite_f32(k2);
}

/* 802D6624-802D6734       .text J3DGDSetLightColor__F10_GXLightID8_GXColor */
void J3DGDSetLightColor(GXLightID id, GXColor color) {
    J3DGDWriteXFCmd(0x0603 + __GDLightID2Offset(id), (color.r << 24) | (color.g << 16) | (color.b << 8) | (color.a << 0));
}

/* 802D6734-802D6900       .text J3DGDSetLightPos__F10_GXLightIDfff */
void J3DGDSetLightPos(GXLightID id, f32 x, f32 y, f32 z) {
    J3DGDWriteXFCmdHdr(0x060A + __GDLightID2Offset(id), 3);
    J3DGDWrite_f32(x);
    J3DGDWrite_f32(y);
    J3DGDWrite_f32(z);
}

/* 802D6900-802D6ACC       .text J3DGDSetLightDir__F10_GXLightIDfff */
void J3DGDSetLightDir(GXLightID id, f32 x, f32 y, f32 z) {
    J3DGDWriteXFCmdHdr(0x060D + __GDLightID2Offset(id), 3);
    J3DGDWrite_f32(x);
    J3DGDWrite_f32(y);
    J3DGDWrite_f32(z);
}

/* 802D6ACC-802D702C       .text J3DGDSetVtxAttrFmtv__F9_GXVtxFmtP17_GXVtxAttrFmtListb */
void J3DGDSetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList* vtxAttr, bool forceNBT) {
    GXCompCnt posCompCnt = GX_POS_XYZ;
    GXCompType posCompType = GX_F32;
    u32 posCompShift = 0;

    GXCompCnt nrmCompCnt = GX_NRM_XYZ;
    GXCompType nrmCompType = GX_F32;
    bool nbt3 = false;

    GXCompCnt clr0CompCnt = GX_CLR_RGBA;
    GXCompType clr0CompType = GX_RGBA8;
    GXCompCnt clr1CompCnt = GX_CLR_RGBA;
    GXCompType clr1CompType = GX_RGBA8;

    GXCompCnt tex0CompCnt = GX_TEX_ST;
    GXCompType tex0CompType = GX_F32;
    u32 tex0CompShift = 0;
    GXCompCnt tex1CompCnt = GX_TEX_ST;
    GXCompType tex1CompType = GX_F32;
    u32 tex1CompShift = 0;
    GXCompCnt tex2CompCnt = GX_TEX_ST;
    GXCompType tex2CompType = GX_F32;
    u32 tex2CompShift = 0;
    GXCompCnt tex3CompCnt = GX_TEX_ST;
    GXCompType tex3CompType = GX_F32;
    u32 tex3CompShift = 0;
    GXCompCnt tex4CompCnt = GX_TEX_ST;
    GXCompType tex4CompType = GX_F32;
    u32 tex4CompShift = 0;
    GXCompCnt tex5CompCnt = GX_TEX_ST;
    GXCompType tex5CompType = GX_F32;
    u32 tex5CompShift = 0;
    GXCompCnt tex6CompCnt = GX_TEX_ST;
    GXCompType tex6CompType = GX_F32;
    u32 tex6CompShift = 0;
    GXCompCnt tex7CompCnt = GX_TEX_ST;
    GXCompType tex7CompType = GX_F32;
    u32 tex7CompShift = 0;

    for (; vtxAttr->mAttrib != GX_VA_NULL; vtxAttr++) {
        switch (vtxAttr->mAttrib) {
        case GX_VA_POS:
            posCompCnt = vtxAttr->mCompCnt;
            posCompType = vtxAttr->mCompType;
            posCompShift = vtxAttr->mCompShift;
            break;
        case GX_VA_NRM:
        case GX_VA_NBT:
            nrmCompType = vtxAttr->mCompType;
            if (vtxAttr->mCompCnt == GX_NRM_NBT3) {
                nrmCompCnt = GX_NRM_NBT;
                nbt3 = true;
            } else {
                // possible fakematch? need to cast vtxAttr->mCompCnt to int to put value in r0 temporarily
                // nrmCompCnt = forceNBT ? GX_NRM_NBT : vtxAttr->mCompCnt;
                nrmCompCnt = (GXCompCnt)(forceNBT ? GX_NRM_NBT : (int)(vtxAttr->mCompCnt));
                nbt3 = false;
            }
            break;
        case GX_VA_CLR0:
            clr0CompCnt = vtxAttr->mCompCnt;
            clr0CompType = vtxAttr->mCompType;
            break;
        case GX_VA_CLR1:
            clr1CompCnt = vtxAttr->mCompCnt;
            clr1CompType = vtxAttr->mCompType;
            break;
        case GX_VA_TEX0:
            tex0CompCnt = vtxAttr->mCompCnt;
            tex0CompType = vtxAttr->mCompType;
            tex0CompShift = vtxAttr->mCompShift;
            break;
        case GX_VA_TEX1:
            tex1CompCnt = vtxAttr->mCompCnt;
            tex1CompType = vtxAttr->mCompType;
            tex1CompShift = vtxAttr->mCompShift;
            break;
        case GX_VA_TEX2:
            tex2CompCnt = vtxAttr->mCompCnt;
            tex2CompType = vtxAttr->mCompType;
            tex2CompShift = vtxAttr->mCompShift;
            break;
        case GX_VA_TEX3:
            tex3CompCnt = vtxAttr->mCompCnt;
            tex3CompType = vtxAttr->mCompType;
            tex3CompShift = vtxAttr->mCompShift;
            break;
        case GX_VA_TEX4:
            tex4CompCnt = vtxAttr->mCompCnt;
            tex4CompType = vtxAttr->mCompType;
            tex4CompShift = vtxAttr->mCompShift;
            break;
        case GX_VA_TEX5:
            tex5CompCnt = vtxAttr->mCompCnt;
            tex5CompType = vtxAttr->mCompType;
            tex5CompShift = vtxAttr->mCompShift;
            break;
        case GX_VA_TEX6:
            tex6CompCnt = vtxAttr->mCompCnt;
            tex6CompType = vtxAttr->mCompType;
            tex6CompShift = vtxAttr->mCompShift;
            break;
        case GX_VA_TEX7:
            tex7CompCnt = vtxAttr->mCompCnt;
            tex7CompType = vtxAttr->mCompType;
            tex7CompShift = vtxAttr->mCompShift;
            break;
        default:
            break;
        }
    }

    GDOverflowCheck(18);

    J3DGDWriteCPCmd(0x70 + fmt,
        (posCompCnt << 0) | (posCompType << 1) | (posCompShift << 4) |
        (nrmCompCnt << 9) | (nrmCompType << 10) |
        (clr0CompCnt << 13) | (clr0CompType << 14) |
        (clr1CompCnt << 17) | (clr1CompType << 18) |
        (tex0CompCnt << 21) | (tex0CompType << 22) | (tex0CompShift << 25) |
        (1 << 30) | (nbt3 << 31));

    J3DGDWriteCPCmd(0x80 + fmt,
        (tex1CompCnt << 0) | (tex1CompType << 1) | (tex1CompShift << 4) |
        (tex2CompCnt << 9) | (tex2CompType << 10) | (tex2CompShift << 13) |
        (tex3CompCnt << 18) | (tex3CompType << 19) | (tex3CompShift << 22) |
        (tex4CompCnt << 27) | (tex4CompType << 28) |
        (1 << 31));

    J3DGDWriteCPCmd(0x90 + fmt,
        (tex4CompShift << 0) |
        (tex5CompCnt << 5) | (tex5CompType << 6) | (tex5CompShift << 9) |
        (tex6CompCnt << 14) | (tex6CompType << 15) | (tex6CompShift << 18) |
        (tex7CompCnt << 23) | (tex7CompType << 24) | (tex7CompShift << 27));
}

/* 802D702C-802D71FC       .text J3DGDSetTexCoordGen__F13_GXTexGenType12_GXTexGenSrc */
void J3DGDSetTexCoordGen(GXTexGenType type, GXTexGenSrc src) {
    u32 form = 0;
    u32 proj = 0;
    u32 row = 5;
    u32 embossSrc = 5;
    u32 embossLight = 0;

    switch (src) {
    case GX_TG_POS: row = 0; form = 1; break;
    case GX_TG_NRM: row = 1; form = 1; break;
    case GX_TG_BINRM: row = 3; form = 1; break;
    case GX_TG_TANGENT: row = 4; form = 1; break;
    case GX_TG_COLOR0: row = 2; break;
    case GX_TG_COLOR1: row = 2; break;
    case GX_TG_TEX0: row = 5; break;
    case GX_TG_TEX1: row = 6; break;
    case GX_TG_TEX2: row = 7; break;
    case GX_TG_TEX3: row = 8; break;
    case GX_TG_TEX4: row = 9; break;
    case GX_TG_TEX5: row = 10; break;
    case GX_TG_TEX6: row = 11; break;
    case GX_TG_TEX7: row = 12; break;
    case GX_TG_TEXCOORD0: embossSrc = 0; break;
    case GX_TG_TEXCOORD1: embossSrc = 1; break;
    case GX_TG_TEXCOORD2: embossSrc = 2; break;
    case GX_TG_TEXCOORD3: embossSrc = 3; break;
    case GX_TG_TEXCOORD4: embossSrc = 4; break;
    case GX_TG_TEXCOORD5: embossSrc = 5; break;
    case GX_TG_TEXCOORD6: embossSrc = 6; break;
    }

    switch (type) {
    case GX_TG_MTX2x4: src = (GXTexGenSrc) 0; break;
    case GX_TG_MTX3x4: src = (GXTexGenSrc) 0; proj = 1; break;
    case GX_TG_BUMP0:
    case GX_TG_BUMP1:
    case GX_TG_BUMP2:
    case GX_TG_BUMP3:
    case GX_TG_BUMP4:
    case GX_TG_BUMP5:
    case GX_TG_BUMP6:
    case GX_TG_BUMP7:
        src = (GXTexGenSrc) 1;
        embossLight = type - GX_TG_BUMP0;
        break;
    case GX_TG_SRTG:
        if (src == GX_TG_COLOR0)
            src = (GXTexGenSrc) 2;
        else
            src = (GXTexGenSrc) 3;
        break;
    }

    J3DGDWrite_u32((proj << 1) | (form << 2) | (src << 4) | (row << 7) | (embossSrc << 12) | (embossLight << 15));
}

/* 802D71FC-802D7400       .text J3DGDSetTexCoordScale2__F13_GXTexCoordIDUsUcUcUsUcUc */
void J3DGDSetTexCoordScale2(GXTexCoordID id, u16 scaleS, u8 biasS, u8 wrapS, u16 scaleT, u8 biasT, u8 wrapT) {
    GDOverflowCheck(15);
    J3DGDWriteBPCmd(0xFE03FFFF);
    // Bug: When id is GX_TEXCOORD_NULL (0xFF), the calculation below winds up producing the wrong
    // BP register ID. Instead of a SU_SSIZE and SU_TSIZE register, it effectively negatively
    // overflows to the previous registers: RAS1_TREF6 and RAS1_TREF7, potentially corrupting them.
    J3DGDWriteBPCmd((scaleS - 1) | (biasS << 16) | (wrapS << 17) | (0x30 + id * 2) << 24);
    J3DGDWriteBPCmd((scaleT - 1) | (biasT << 16) | (wrapT << 17) | (0x31 + id * 2) << 24);
}

/* 802D7400-802D759C       .text J3DGDSetTexLookupMode__F11_GXTexMapID14_GXTexWrapMode14_GXTexWrapMode12_GXTexFilter12_GXTexFilterfffUcUc13_GXAnisotropy */
void J3DGDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter minFilter, GXTexFilter magFilter, f32 minLOD, f32 maxLOD, f32 lodBias, u8 biasClamp, u8 edgeLOD, GXAnisotropy maxAniso) {
    J3DGDWriteBPCmd(
        (wrapS) << 0 |
        (wrapT) << 2 |
        (magFilter == GX_LINEAR) << 4 |
        (GX2HWFiltConv[minFilter]) << 5 |
        (!edgeLOD) << 8 |
        ((u8)(lodBias * 32.0f)) << 9 |
        (maxAniso) << 19 |
        (biasClamp) << 21 |
        (J3DGDTexMode0Ids[id] << 24));
    J3DGDWriteBPCmd(
        ((u8)(minLOD * 16.0f)) << 0 |
        ((u8)(maxLOD * 16.0f)) << 8 |
        (J3DGDTexMode1Ids[id] << 24));
}

/* 802D759C-802D7644       .text J3DGDSetTexImgAttr__F11_GXTexMapIDUsUs9_GXTexFmt */
void J3DGDSetTexImgAttr(GXTexMapID id, u16 w, u16 h, GXTexFmt fmt) {
    J3DGDWriteBPCmd((w - 1) | ((h - 1) << 10) | (fmt << 20) | (J3DGDTexImage0Ids[id] << 24));
}

/* 802D7644-802D76D4       .text J3DGDSetTexImgPtr__F11_GXTexMapIDPv */
void J3DGDSetTexImgPtr(GXTexMapID id, void* pTex) {
    J3DGDWriteBPCmd((OSCachedToPhysical(pTex) >> 5) | (J3DGDTexImage3Ids[id] << 24));
}

/* 802D76D4-802D77A8       .text J3DGDSetTexImgPtrRaw__F11_GXTexMapIDUl */
void J3DGDSetTexImgPtrRaw(GXTexMapID id, u32 addr) {
    GDOverflowCheck(5);
    J3DGDWriteBPCmd(addr | (J3DGDTexImage3Ids[id] << 24));
}

/* 802D77A8-802D7840       .text J3DGDSetTexTlut__F11_GXTexMapIDUl10_GXTlutFmt */
void J3DGDSetTexTlut(GXTexMapID id, u32 addr, GXTlutFmt fmt) {
    J3DGDWriteBPCmd(((addr - 0x80000) >> 9) | fmt << 10 | J3DGDTexTlutIds[id] << 24);
}

/* 802D7840-802D7AF8       .text J3DGDLoadTlut__FPvUl11_GXTlutSize */
void J3DGDLoadTlut(void* pTex, u32 addr, GXTlutSize size) {
    J3DGDWriteBPCmd(0xFFFF00 | 0xFE << 24);
    J3DGDWriteBPCmd(0x0F << 24);

    J3DGDWriteBPCmd((OSCachedToPhysical(pTex) >> 5) | 0x64 << 24);
    J3DGDWriteBPCmd(((addr - 0x80000) >> 9) | size << 10 | 0x65 << 24);

    J3DGDWriteBPCmd(0xFFFF00 | 0xFE << 24);
    J3DGDWriteBPCmd(0x0F << 24);
}

/* 802D7AF8-802D7DD8       .text J3DGDSetIndTexMtx__F14_GXIndTexMtxIDPA3_fSc */
void J3DGDSetIndTexMtx(GXIndTexMtxID id, Mtx23 mtx, s8 exp) {
    s32 mtx32[6];
    u32 idx;

    switch (id) {
    case GX_ITM_0:
    case GX_ITM_1:
    case GX_ITM_2:
        idx = (u32)(id - GX_ITM_0);
        break;
    case GX_ITM_S0:
    case GX_ITM_S1:
    case GX_ITM_S2:
        idx = (u32)(id - GX_ITM_S0);
        break;
    case GX_ITM_T0:
    case GX_ITM_T1:
    case GX_ITM_T2:
        idx = (u32)(id - GX_ITM_T0);
        break;
    default:
        idx = 0;
        break;
    }

    exp += 17;

    mtx32[0] = (s32)(mtx[0][0] * 1024.0f) & 0x7FF;
    mtx32[1] = (s32)(mtx[1][0] * 1024.0f) & 0x7FF;

    mtx32[2] = (s32)(mtx[0][1] * 1024.0f) & 0x7FF;
    mtx32[3] = (s32)(mtx[1][1] * 1024.0f) & 0x7FF;

    mtx32[4] = (s32)(mtx[0][2] * 1024.0f) & 0x7FF;
    mtx32[5] = (s32)(mtx[1][2] * 1024.0f) & 0x7FF;

    GDOverflowCheck(15);

    J3DGDWriteBPCmd((mtx32[0] << 0) | (mtx32[1] << 11) | (((exp >> 0) & 0x03) << 22) | (0x06 + idx * 3) << 24);
    J3DGDWriteBPCmd((mtx32[2] << 0) | (mtx32[3] << 11) | (((exp >> 2) & 0x03) << 22) | (0x07 + idx * 3) << 24);
    J3DGDWriteBPCmd((mtx32[4] << 0) | (mtx32[5] << 11) | (((exp >> 4) & 0x03) << 22) | (0x08 + idx * 3) << 24);
}

/* 802D7DD8-802D7ED0       .text J3DGDSetIndTexCoordScale__F16_GXIndTexStageID14_GXIndTexScale14_GXIndTexScale14_GXIndTexScale14_GXIndTexScale */
void J3DGDSetIndTexCoordScale(GXIndTexStageID id, GXIndTexScale s0, GXIndTexScale t0, GXIndTexScale s1, GXIndTexScale t1) {
    GDOverflowCheck(5);
    J3DGDWriteBPCmd((s0) | (t0 << 4) | (s1 << 8) | (t1 << 12) | ((0x25 + (id >> 1)) << 24));
}

/* 802D7ED0-802D80D0       .text J3DGDSetIndTexOrder__FUl13_GXTexCoordID11_GXTexMapID13_GXTexCoordID11_GXTexMapID13_GXTexCoordID11_GXTexMapID13_GXTexCoordID11_GXTexMapID */
void J3DGDSetIndTexOrder(u32 num, GXTexCoordID tc0, GXTexMapID tm0, GXTexCoordID tc1, GXTexMapID tm1, GXTexCoordID tc2, GXTexMapID tm2, GXTexCoordID tc3, GXTexMapID tm3) {
    GDOverflowCheck(10);
    J3DGDWriteBPCmd(((tm0 & 0x07) << 0) | ((tc0 & 0x07) << 3) | ((tm1 & 0x07) << 6) | ((tc1 & 0x07) << 9) | ((tm2 & 0x07) << 12) | ((tc2 & 0x07) << 15) | ((tm3 & 0x07) << 18) | ((tc3 & 0x07) << 21) | (0x27 << 24));

    u32 mask = 0;
    for (u32 i = 0; i < num; i++) {
        switch (i) {
        case 0: mask |= (1 << (tm0 & 0x07)); break;
        case 1: mask |= (1 << (tm1 & 0x07)); break;
        case 2: mask |= (1 << (tm2 & 0x07)); break;
        case 3: mask |= (1 << (tm3 & 0x07)); break;
        }
    }
    J3DGDWriteBPCmd((mask) | (0x0F << 24));
}

/* 802D80D0-802D825C       .text J3DGDSetTevOrder__F13_GXTevStageID13_GXTexCoordID11_GXTexMapID12_GXChannelID13_GXTexCoordID11_GXTexMapID12_GXChannelID */
void J3DGDSetTevOrder(GXTevStageID stage, GXTexCoordID coord1, GXTexMapID map1, GXChannelID color1, GXTexCoordID coord2, GXTexMapID map2, GXChannelID color2) {
    static u8 c2r[] = {
        /* GX_COLOR0      */ 0,
        /* GX_COLOR1      */ 1,
        /* GX_ALPHA0      */ 0,
        /* GX_ALPHA1      */ 1,
        /* GX_COLOR0A0    */ 0,
        /* GX_COLOR1A1    */ 1,
        /* GX_COLOR_ZERO  */ 7,
        /* GX_ALPHA_BUMP  */ 5,
        /* GX_ALPHA_BUMPN */ 6,
        0,
        0,
        0,
        0,
        0,
        0,
        /* GX_COLOR_NULL  */ 7,
    };

    GXTexCoordID r31 = coord1 >= GX_MAXCOORD ? GX_TEXCOORD0 : coord1;
    GXTexCoordID r30 = coord2 >= GX_MAXCOORD ? GX_TEXCOORD0 : coord2;
    GDOverflowCheck(5);
    J3DGDWriteBPCmd(
        ((map1 & 7) << 0) |
        (r31 << 3) |
        ((map1 != GX_TEXMAP_NULL && (map1 & GX_TEXMAP_DISABLE) == 0) << 6) |
        (c2r[color1 & 0xf] << 7) |
        ((map2 & 7) << 12) |
        (r30 << 15) |
        ((map2 != GX_TEXMAP_NULL && (map2 & GX_TEXMAP_DISABLE) == 0) << 18) |
        (c2r[color2 & 0xf] << 19) |
        ((stage / 2 + 0x28) << 24));
}

/* 802D825C-802D83C4       .text J3DGDSetTevKColor__F14_GXTevKColorID8_GXColor */
void J3DGDSetTevKColor(GXTevKColorID id, GXColor color) {
    u32 r0 =
        (color.r << 0) |
        (color.a << 12) |
        (1 << 23) |
        ((0xe0 + id * 2) << 24);
    u32 r1 =
        (color.b << 0) |
        (color.g << 12) |
        (1 << 23) |
        ((0xe1 + id * 2) << 24);

    GDOverflowCheck(10);
    J3DGDWriteBPCmd(r0);
    J3DGDWriteBPCmd(r1);
}

/* 802D83C4-802D85F8       .text J3DGDSetTevColorS10__F11_GXTevRegID11_GXColorS10 */
void J3DGDSetTevColorS10(GXTevRegID id, GXColorS10 color) {
    u32 r0 =
        ((color.r & 0x7FF) << 0) |
        ((color.a & 0x7FF) << 12) |
        ((0xe0 + id * 2) << 24);
    u32 r1 =
        ((color.b & 0x7FF) << 0) |
        ((color.g & 0x7FF) << 12) |
        ((0xe1 + id * 2) << 24);

    GDOverflowCheck(20);
    J3DGDWriteBPCmd(r0);
    J3DGDWriteBPCmd(r1);

    /* wtf? */
    J3DGDWriteBPCmd(r1);
    J3DGDWriteBPCmd(r1);
}

/* 802D85F8-802D895C       .text J3DGDSetFog__F10_GXFogTypeffff8_GXColor */
void J3DGDSetFog(GXFogType type, f32 startZ, f32 endZ, f32 nearZ, f32 farZ, GXColor color) {
    f32 A, B, C;
    if (farZ == nearZ || endZ == startZ) {
        A = 0.0f;
        B = 0.5f;
        C = 0.0f;
    } else {
        A = (farZ * nearZ) / ((farZ - nearZ) * (endZ - startZ));
        B = farZ / (farZ - nearZ);
        C = startZ / (endZ - startZ);
    }

    f32 mantissa = B;
    u32 exponent = 1;
    while (mantissa > 1.0) {
        mantissa /= 2.0f;
        exponent++;
    }

    while (mantissa > 0.0f && mantissa < 0.5) {
        mantissa *= 2.0f;
        exponent--;
    }

    f32 tA = A / (1 << exponent);
    u32 mantissa32 = (u32)(mantissa * 8388638.0f);

    u32 A32 = *(u32*)&tA;
    u32 C32 = *(u32*)&C;

    J3DGDWriteBPCmd((A32 >> 12) | (0xee << 24));
    J3DGDWriteBPCmd(mantissa32 | (0xef << 24));
    J3DGDWriteBPCmd(exponent | (0xf0 << 24));
    J3DGDWriteBPCmd((C32 >> 12) | (type << 21) | (0xf1 << 24));
    J3DGDWriteBPCmd(color.b << 0 | color.g << 8 | color.r << 16 | (0xf2 << 24));
}

/* 802D895C-802D8AA8       .text J3DGDSetFogRangeAdj__FUcUsP14_GXFogAdjTable */
void J3DGDSetFogRangeAdj(u8 enabled, u16 center, GXFogAdjTable* pFogAdjTable) {
    if (enabled) {
        for (s32 i = 0; i < 10; i += 2) {
            J3DGDWriteBPCmd((0xe9 + i / 2) << 24 | pFogAdjTable->fogVals[i + 1] << 12 | pFogAdjTable->fogVals[i] << 0);
        }
    }

    u32 reg = (0xe8 << 24) | (center + 342) << 0 | enabled << 10;
    J3DGDWriteBPCmd(reg);
}
