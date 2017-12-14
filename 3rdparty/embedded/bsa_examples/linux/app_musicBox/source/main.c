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
#include "app_ble.h"
#include "app_ble_server.h"

/*
 * Extern variables
 */
extern BD_ADDR                 app_sec_db_addr;    /* BdAddr of peer device requesting SP */
extern tAPP_MGR_CB app_mgr_cb;
extern tAPP_XML_CONFIG         app_xml_config;
extern int socket_fd;
int ble_mode = 0;
extern char socket_rev[];
extern int socket_rev_len;
/*
 * Local functions
 */

/*******************************************************************************
 **
 ** Function         app_ble_setup
 **
 ** Description      Handle the BLE menu
 **
 ** Returns          void
 **
 *******************************************************************************/
void app_ble_setup(void)
{
    int choice, type,i;
    UINT16 ble_scan_interval, ble_scan_window;
    tBSA_DM_BLE_CONN_PARAM conn_param;
    tBSA_DM_BLE_ADV_CONFIG adv_conf;
    tBSA_DM_BLE_ADV_PARAM adv_param;
    UINT16 number_of_services;
    UINT8 app_ble_adv_value[APP_BLE_ADV_VALUE_LEN] = {0x2b, 0x1a, 0xaa, 0xbb, 0xcc, 0xdd}; /*First 2 byte is Company Identifier Eg: 0x1a2b refers to Bluetooth ORG, followed by 4bytes of data*/

    APP_INFO0("app ble server register");
    app_ble_server_register(APP_BLE_MAIN_INVALID_UUID, NULL);
    APP_INFO0("app ble server create service");
    app_ble_server_create_service();
    sleep(1); //must
    APP_INFO0("app ble server add char");
    app_ble_server_add_char(0);
    APP_INFO0("app ble server add char");
    app_ble_server_add_char(1);
    APP_INFO0("app ble server start service");
    app_ble_server_start_service();
    APP_INFO0("app ble server display");
    app_ble_server_display();
    APP_INFO0("Configure BLE Advertisement data");
    /*******************Configure BLE Advertisement data begin*************/
    memset(&adv_conf, 0, sizeof(tBSA_DM_BLE_ADV_CONFIG));
    /* start advertising */
    adv_conf.len = APP_BLE_ADV_VALUE_LEN;
    adv_conf.flag = BSA_DM_BLE_ADV_FLAG_MASK;
    memcpy(adv_conf.p_val, app_ble_adv_value, APP_BLE_ADV_VALUE_LEN);
    /* All the masks/fields that are set will be advertised*/
    adv_conf.adv_data_mask = BSA_DM_BLE_AD_BIT_FLAGS|BSA_DM_BLE_AD_BIT_SERVICE|BSA_DM_BLE_AD_BIT_APPEARANCE|BSA_DM_BLE_AD_BIT_MANU;
    adv_conf.appearance_data = 0x1122;
    APP_INFO1("Enter appearance value Eg:0x1122:0x%x",adv_conf.appearance_data);
    number_of_services = 1;
    APP_INFO1("Enter number of services between <1-6> Eg:2: %d",number_of_services);
    adv_conf.num_service = number_of_services;
    for(i=0; i< adv_conf.num_service; i++)
    {
	    adv_conf.uuid_val[i]= 0x180A;
	    APP_INFO1("Enter service UUID :0x%x",adv_conf.uuid_val[i]);
    }
    adv_conf.is_scan_rsp = 0;
    APP_INFO1("Is this scan response? (0:FALSE, 1:TRUE): %d",adv_conf.is_scan_rsp);
    app_dm_set_ble_adv_data(&adv_conf);
    /*******************Configure BLE Advertisement data end*************/
}

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
            app_mgr_config(TRUE);/*TURE: BLE MODE, FALSE: NO BLE MODE*/
            app_avk_init(NULL);
            app_hs_init();
	    if(ble_mode == 1)
		app_ble_start();
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
    if(ble_mode == 1) {
	    /* Exit BLE mode */
	    app_ble_exit();
	    app_ble_socket_teardown(socket_fd);
    }
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
	int status;

	if (argc == 2) {
		if(!strncmp(argv[1],"ble_mode",8)) {
			ble_mode = 1;
			APP_INFO0("enter ble mode");
		} else {
			ble_mode = 0;
		}
	} else 
		ble_mode = 0;

	/*app_manager init*/
	app_mgt_init();
	if (app_mgt_open(NULL, app_mgt_callback) < 0)
	{
		APP_ERROR0("Unable to connect to server");
		return -1;
	}

	/* Init XML state machine */
	app_xml_init();

	if (app_mgr_config(TRUE))/*TURE: BLE MODE, FALSE: NO BLE MODE*/
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
	if(!ble_mode) { 
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
	}
	if(ble_mode == 1) {
		/***********set app ble begin*****************/
		/* Initialize BLE application */
		status = app_ble_init();
		if (status < 0)
		{
			APP_ERROR0("Couldn't Initialize BLE app, exiting");
			exit(-1);
		}

		/* Start BLE application */
		status = app_ble_start();
		if (status < 0)
		{
			APP_ERROR0("Couldn't Start BLE app, exiting");
			return -1;
		}

		/* register one application */
		app_ble_client_register(APP_BLE_MAIN_DEFAULT_APPL_UUID);

		/* The BLE setup */
		app_ble_setup();

		APP_INFO0("\tEnable Set BLE Visibiltity");
		app_dm_set_ble_visibility(TRUE, TRUE);
		app_dm_set_visibility(FALSE, FALSE);
		/*********************create socket client*********/
		socket_fd = app_ble_socket_client_create();
		/***********set app ble end*****************/
	}
	/*suppose the program is only terminated by interrupt or termintion singal*/
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	while (1) {
		if(ble_mode == 1) {
			socket_rev_len = app_ble_socket_recv(socket_fd,socket_rev, 1);
			if (socket_rev_len == 1)
				APP_INFO1("\tget ble return wifi status value: %s",socket_rev);
		} else {
			sleep(10);
		}
	}

	return 0;
}
