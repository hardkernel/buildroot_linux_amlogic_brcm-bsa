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
#ifndef APP_BLE_SOCKET_H
#define APP_BLE_SOCKET_H


#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <sys/stat.h>


typedef struct{
    int server_sockfd;
    int client_sockfd;
    int server_len;
    int client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;
    char sock_path[512];
} tAPP_SOCKET;

tAPP_SOCKET *app_ble_socket;
/********************CLIENT API***************************/
int app_ble_socket_client_create(void);
int app_ble_socket_send(int sockfd, char *msg, int len);
void app_ble_socket_teardown(int sockfd);

/********************SERVER API***************************/
int setup_socket_server(tAPP_SOCKET *app_socket);
int accpet_client(tAPP_SOCKET *app_socket);
void teardown_socket_server(tAPP_SOCKET *app_socket);
/********************COMMON API***************************/
int socket_send(int sockfd, char *msg, int len);
int socket_recieve(int sockfd, char *msg, int len);

#endif
