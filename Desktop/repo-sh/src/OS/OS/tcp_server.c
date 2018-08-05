// file:socket_server_write_getaddrinfo.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>// for memset
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#define MY_PORT "17171"
#define LINE "hello world"
#define MAX_ADDR_LEN 100
// size of str that holds IP
//--------------------------
int main()
{
    int rc;
    // return code
    int main_socket;
    int serving_socket;
    struct addrinfo con_kind,*addr_info_res ;
    memset(&con_kind, 0, sizeof con_kind) ;
    con_kind.ai_family = AF_UNSPEC ;
    con_kind.ai_socktype = SOCK_STREAM ;
    con_kind.ai_flags = AI_PASSIVE ;
    // system will fill my IP

     /* this funcation returns  one  or more addrinfo structures, each of which
       contains an Internet address that can be specified in a call to bind(2)
       or  connect(2) */
    if ((rc = getaddrinfo(NULL, MY_PORT, // NULL = you set IP addr
        &con_kind,  &addr_info_res)) != 0) {
        fprintf(stderr, "(getaddrinfo() failed %s\n", gai_strerror(rc)) ;
        exit(EXIT_FAILURE);
    }
    
    main_socket = socket(addr_info_res -> ai_family, addr_info_res -> ai_socktype,
        addr_info_res -> ai_protocol);
    
    if (main_socket < 0) {
        perror("socket: allocation failed");
        exit(EXIT_FAILURE) ;
    }

    /*  assigns the address  specified  by  addr  to  the  socket  referred  to by the file
        descriptor sockfd.*/
    rc = bind(main_socket, addr_info_res-> ai_addr, addr_info_res-> ai_addrlen);
    
    if (rc) {
        perror("bind failed");
        exit(EXIT_FAILURE) ;
    }

    rc = listen(main_socket, 5);
    
    if (rc) {
        perror("listen failed");
        exit(EXIT_FAILURE) ;
    }
    while (1) {
        struct sockaddr_storage client_info ;
        socklen_t client_info_size = sizeof client_info;
        char client_ip_addr[MAX_ADDR_LEN] ;
        struct sockaddr_in *clientAddr ;
        serving_socket = accept(main_socket,
        (struct sockaddr *)&client_info,
        &client_info_size);
        if (serving_socket < 0)
        continue;
        //-----------------------------------
        clientAddr = (struct sockaddr_in *)
        &(((struct sockaddr_in6*)(&client_info))->sin6_addr) ;
        inet_ntop(client_info.ss_family, clientAddr, client_ip_addr,
        sizeof client_ip_addr);
        printf("Got connection from %s\n", client_ip_addr);
        //-----------------------------------
        write(serving_socket, LINE, sizeof(LINE));
        close(serving_socket);
    }
    freeaddrinfo(addr_info_res) ;
    return(EXIT_SUCCESS) ;
}