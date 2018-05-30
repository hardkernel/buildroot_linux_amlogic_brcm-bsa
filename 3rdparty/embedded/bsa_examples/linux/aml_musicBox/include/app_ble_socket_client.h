/*****************************************************************************
 **
 **  Name:           app_ble_socket.h
 **
 **  Description:    Bluetooth Manager application
 **
 **  Copyright (c) 2010-2013, Broadcom Corp., All Rights Reserved.
 **  Broadcom Bluetooth Core. Proprietary and confidential.
 **
 *****************************************************************************/
/* idempotency */
#ifndef APP_BLE_SOCKET_CLIENT_H
#define APP_BLE_SOCKET_CLIENT_H


#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <sys/stat.h>


/********************CLIENT API***************************/
int app_ble_socket_client_create(void);
int app_ble_socket_send(int sockfd, char *msg, int len);
int app_ble_socket_recv(int sockfd, char *msg, int len);
void app_ble_socket_teardown(int sockfd);

/********************COMMON API***************************/
int socket_send(int sockfd, char *msg, int len);
int socket_recieve(int sockfd, char *msg, int len);

#endif
