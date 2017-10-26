/*****************************************************************************
**
**  Name:           app_manager_main.c
**
**  Description:    Bluetooth Manager menu application
**
**  Copyright (c) 2010-2014, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>

#include "gki.h"
#include "uipc.h"

#include "bsa_api.h"

#include "app_xml_utils.h"

#include "app_disc.h"
#include "app_utils.h"
#include "app_dm.h"

#include "app_services.h"
#include "app_mgt.h"

#include "app_manager.h"
#include "app_avk.h"
#include "app_hs.h"



/*
 * Extern variables
 */
extern BD_ADDR                 app_sec_db_addr;    /* BdAddr of peer device requesting SP */
extern tAPP_MGR_CB app_mgr_cb;
extern tAPP_XML_CONFIG         app_xml_config;

/*
 * Local functions
 */

/*******************************************************************************
 **
 ** Function         app_mgt_callback
 **
 ** Description      This callback function is called in case of server
 **                  disconnection (e.g. server crashes)
 **
 ** Parameters
 **
 ** Returns          void
 **
 *******************************************************************************/
static BOOLEAN app_mgt_callback(tBSA_MGT_EVT event, tBSA_MGT_MSG *p_data)
{
    switch (event)
    {
    case BSA_MGT_STATUS_EVT:
        APP_DEBUG0("BSA_MGT_STATUS_EVT");
        if (p_data->status.enable)
        {
            APP_DEBUG0("Bluetooth restarted => re-initialize the application");
            app_mgr_config();
            app_avk_init(NULL);
            app_hs_init();
        }
        break;

    case BSA_MGT_DISCONNECT_EVT:
        APP_DEBUG1("BSA_MGT_DISCONNECT_EVT reason:%d", p_data->disconnect.reason);
        exit(-1);
        break;

    default:
        break;
    }
    return FALSE;
}

/*******************************************************************************
 **
 ** Function         signal_handler
 **
 ** Description      an hanlder to interrupt and termination signals
 **
 **
 ** Parameters
 **
 ** Returns          void
 **
 *******************************************************************************/

static void signal_handler(int sig)
{
    APP_ERROR0("signal_handler enter");
    app_hs_stop();
#ifdef PCM_ALSA_DSPC
    libdspc_stop();
#endif
    app_avk_end();
    app_mgt_close();
    signal(sig, SIG_DFL);
	exit(0);
}

/*******************************************************************************
 **
 ** Function         main
 **
 ** Description      This is the main function
 **
 ** Parameters
 **
 ** Returns          void
 **
 *******************************************************************************/
int main(int argc, char **argv)
{
    int mode;

    /*app_manager init*/
    app_mgt_init();
    if (app_mgt_open(NULL, app_mgt_callback) < 0)
    {
        APP_ERROR0("Unable to connect to server");
        return -1;
    }

    /* Init XML state machine */
    app_xml_init();

    if (app_mgr_config())
    {
        APP_ERROR0("Couldn't configure successfully, exiting");
        return -1;
    }

    /* Display FW versions */
    app_mgr_read_version();

    /* Get the current Stack mode */
    mode = app_dm_get_dual_stack_mode();
    if (mode < 0)
    {
        APP_ERROR0("app_dm_get_dual_stack_mode failed");
        return -1;
    }
    else
    {
        /* Save the current DualStack mode */
        app_mgr_cb.dual_stack_mode = mode;
        APP_INFO1("Current DualStack mode:%s", app_mgr_get_dual_stack_mode_desc());
    }

    /* Init Ad2p Sink Application */
    app_avk_init(NULL);
    app_avk_register();
    /* Init Headset Application */
    app_hs_init();
    app_hs_start(NULL);
    /* Init libdspc Application */
#ifdef PCM_ALSA_DSPC
    libdspc_init();
#endif
    /*suppose the program is only terminated by interrupt or termintion singal*/
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    while (1) {
        sleep(10);
    }

    return 0;
}
