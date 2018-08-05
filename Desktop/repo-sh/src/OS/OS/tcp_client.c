#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for read/write/close
#include <sys/types.h>  /* standard system types*/
#include <netinet/in.h> /* Internet address structures */
#include <arpa/inet.h>
#include <sys/socket.h> /* socket interface functions */
#include <netdb.h>/* host to IP resolution */

#define BUFLEN 1024
#define DEST_PORT "17171"
int main(int argc, char *argv[])
{
    int rc;                 /* the value that return from OS about sys calls */
    int my_socket;          /* the file number in the files descriptor */
    char buf[BUFLEN+1];     /* buffer read/write */
    char*pc;                /* moving pointer on the buffer */
    struct addrinfo con_kind,*addr_info_res;   /*   is returned by getaddrinfo(), and contains, on success, 
                                                a linked list of such structs for a specified hostname and/or service. */

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <host name>\n", argv[0]) ;
        exit(EXIT_FAILURE) ;
    }
    
    memset(&con_kind, 0, sizeof(con_kind));
    con_kind.ai_family = AF_UNSPEC;
   
    con_kind.ai_socktype = SOCK_STREAM; /* STREAM = TCP */

    /* this funcation returns  one  or more addrinfo structures, each of which
       contains an Internet address that can be specified in a call to bind(2)
       or  connect(2) */
    if ((rc = getaddrinfo(argv[1], DEST_PORT, &con_kind, &addr_info_res) != 0)) {
        fprintf(stderr, "(getaddrinfo() failed %s\n", gai_strerror(rc)) ;
        exit(EXIT_FAILURE);
    }
    
    my_socket = socket(addr_info_res->ai_family, addr_info_res->ai_socktype, addr_info_res->ai_protocol);
    // 0 = u decide which protocal to use
    if (my_socket < 0) {
        perror("socket: allocation failed");
        exit(EXIT_FAILURE);
    }
    rc = connect(my_socket, addr_info_res ->ai_addr, // holds: IP+Port of server
                addr_info_res ->ai_addrlen) ;
    if (rc) {
        perror("connect");
        exit(EXIT_FAILURE) ;
    }
    
    pc = buf;
    while ((rc = read(my_socket, pc, BUFLEN - (pc-buf)))) {
        pc += rc;
    }
    close(my_socket);
    *pc = '\0';
    
    printf("Got from server: %s\n", buf);
    freeaddrinfo(addr_info_res) ;
    
    return EXIT_SUCCESS;
}