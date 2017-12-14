#include "app_ble_socket_client.h"

/********************CLIENT API***************************/
int app_ble_socket_client_create(void)
{
    struct sockaddr_un address;
    int sockfd,  len;
    char app_ble_socket_name[]="/etc/bsa/config/app_ble_socket";
    
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            printf("%s: can not creat socket\n", __func__);
            return -1;
    }

    address.sun_family = AF_UNIX;
    strcpy (address.sun_path, app_ble_socket_name);
    len = sizeof (address);

    if (connect (sockfd, (struct sockaddr *)&address, len) == -1) {
        printf("%s: please ensure the server is up\n", __func__);
        printf("%s: can not connect to socket\n", __func__);
        return -1;
    }

    return sockfd;
}
int app_ble_socket_send(int sockfd, char *msg, int len)
{
    int bytes;
    if (sockfd < 0) {
        printf("%s: invalid sockfd\n",__func__);
        return -1;
    }
    if ((bytes = send(sockfd, msg, len, 0)) < 0) {
        perror ("send");
    }
    return bytes;
}

int app_ble_socket_recv(int sockfd, char *msg, int len)
{
    int bytes;
    if (sockfd < 0) {
        printf("%s: invalid sockfd\n",__func__);
        return -1;
    }
    if ((bytes = recv(sockfd, msg, len, 0)) < 0) {
        perror ("send");
    }
    return bytes;
}
void app_ble_socket_teardown(int sockfd)
{
    close(sockfd);
}

/********************COMMON API***************************/
int socket_send(int sockfd, char *msg, int len)
{
    int bytes;
    if (sockfd < 0) {
        printf("%s: invalid sockfd\n",__func__);
        return -1;
    }
    if ((bytes = send(sockfd, msg, len, 0)) < 0) {
        perror ("send");
    }
    return bytes;
}

int socket_recieve(int sockfd, char *msg, int len)
{
    int bytes;

    if (sockfd < 0) {
        printf("%s: invalid sockfd\n",__func__);
        return -1;
    }

    if ((bytes = recv(sockfd, msg, len, 0)) < 0)
    {
        perror ("recv");
    }
    return bytes;

}
