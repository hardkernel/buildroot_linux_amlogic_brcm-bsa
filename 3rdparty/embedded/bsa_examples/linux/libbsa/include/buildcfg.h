#ifndef BUILDCFG_H
#define BUILDCFG_H

#define BSA_DM_INCLUDED   TRUE
#define BSA_SEC_INCLUDED   TRUE
#define BSA_DISC_INCLUDED   TRUE
#define BSA_TM_INCLUDED   TRUE
#define BSA_AV_INCLUDED   TRUE
#define BSA_AVK_INCLUDED   TRUE
#define BSA_AG_INCLUDED   TRUE
#define BSA_HS_INCLUDED   TRUE
#define BSA_PBS_INCLUDED   FALSE
#define BSA_AVK_AV_AUDIO_RELAY   TRUE
#define BSA_AVK_AAC_SUPPORTED    TRUE
#define BSA_AV_UIPC_BUFFER_SIZE   20001
#define APP_AV_BITRATE_CONTROL_BY_USER   FALSE
#define SBC_FOR_EMBEDDED_LINUX   TRUE
#define BNEP_MAX_PROT_FILTERS   10
#define BNEP_MAX_MULTI_FILTERS   10
#define BT_USE_TRACES   TRUE
#define BT_USE_TRACES_TIMESTAMP   TRUE
#define BT_USE_TRACES_SYSLOG   FALSE
#define BT_USE_TRACES_SYSLOG_MSG_SIZE   256
#define BSA_COLORED_TRACES   FALSE
#define APPL_INITIAL_TRACE_LEVEL   BT_TRACE_LEVEL_DEBUG
#define BRCM_GKI_INCLUDED   TRUE
#define BRCM_DRV_INCLUDED   TRUE
#define CBACK_TASK   0
#define GKI_MAX_TASKS   5
#define GKI_NUM_FIXED_BUF_POOLS   4
#define GKI_BUF3_SIZE   10240
#define HID_DEV_MTU_SIZE   800

#ifdef CONFIG_MEM_REDUCTION
#define BSA_FM_INCLUDED   FALSE
#define BSA_HH_INCLUDED   FALSE
#define BSA_HD_INCLUDED   FALSE
#define BSA_OPC_INCLUDED   FALSE
#define BSA_OPS_INCLUDED   FALSE
#define BSA_FTS_INCLUDED   FALSE
#define BSA_FTC_INCLUDED   FALSE
#define BSA_DG_INCLUDED   FALSE
#define BSA_PAN_INCLUDED   FALSE
#define BSA_PBC_INCLUDED   FALSE
#define BSA_MCE_INCLUDED   FALSE
#define BSA_HL_INCLUDED   FALSE
#define BSA_NSA_INCLUDED   FALSE
#define BSA_3DS_INCLUDED   FALSE
#define BSA_BAV_INCLUDED   FALSE
#define BSA_SC_INCLUDED   FALSE
#define BSA_SAC_INCLUDED   FALSE
#define BSA_CCE_INCLUDED   FALSE
#define BLE_INCLUDED   FALSE
#define SMP_INCLUDED   FALSE
#define SMP_LE_SC_INCLUDED   FALSE
#define BTA_GATT_INCLUDED   FALSE
#define BLE_AUDIO_INCLUDED   FALSE
#define HID_AUDIO_STREAMING   FALSE
#define APP_TASK   1
#else
#define BSA_FM_INCLUDED   TRUE
#define BSA_HH_INCLUDED   TRUE
#define BSA_HD_INCLUDED   TRUE
#define BSA_OPC_INCLUDED   TRUE
#define BSA_OPS_INCLUDED   TRUE
#define BSA_FTS_INCLUDED   TRUE
#define BSA_FTC_INCLUDED   TRUE
#define BSA_DG_INCLUDED   TRUE
#define BSA_PAN_INCLUDED   TRUE
#define BSA_PBC_INCLUDED   TRUE
#define BSA_MCE_INCLUDED   TRUE
#define BSA_HL_INCLUDED   TRUE
#define BSA_NSA_INCLUDED   TRUE
#define BSA_3DS_INCLUDED   TRUE
#define BSA_BAV_INCLUDED   TRUE
#define BSA_SC_INCLUDED   TRUE
#define BSA_SAC_INCLUDED   TRUE
#define BSA_CCE_INCLUDED   TRUE
#define BLE_INCLUDED   TRUE
#define SMP_INCLUDED   TRUE
#define SMP_LE_SC_INCLUDED   TRUE
#define BTA_GATT_INCLUDED   TRUE
#define BLE_AUDIO_INCLUDED   TRUE
#define HID_AUDIO_STREAMING   TRUE
#define NSA_CBACK_TASK   1
#define APP_TASK   2
#endif

#endif
