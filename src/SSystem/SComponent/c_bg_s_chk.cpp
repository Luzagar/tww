//
// Generated by dtk
// Translation Unit: c_bg_s_chk.cpp
//

#include "SSystem/SComponent/c_bg_s_chk.h"
#include "f_pc/f_pc_manager.h"

/* 80247304-8024734C       .text __dt__8cBgS_ChkFv */
cBgS_Chk::~cBgS_Chk() {
}

/* 8024734C-8024738C       .text ChkSameActorPid__8cBgS_ChkCFUi */
bool cBgS_Chk::ChkSameActorPid(fpc_ProcID pid) const {
    if (mActorPid == fpcM_ERROR_PROCESS_ID_e || pid == fpcM_ERROR_PROCESS_ID_e || mSameActorChk == 0) {
        return FALSE;
    } else {
        return (mActorPid == pid) ? TRUE : FALSE;
    }
}
