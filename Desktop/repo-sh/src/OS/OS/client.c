#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Write TEXT to the socket given by file descriptor SOCKET_FD.
*/
void write_text (int socket_fd, const char* text)
{
    /* Write the number of bytes in the string, including
    NUL-termination. */
    int length = strlen (text) + 1;
    write (socket_fd, &length, sizeof (length));
    /* Write the string. */
    write (socket_fd, text, length);
}

int main (void)
{
    const char* const socket_name = "/tmp/sock";
    const char* const message = "ping";
    int socket_fd;
    struct sockaddr_un name;
    
    /* Create the socket. */
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    
    /* Store the server’s name in the socket address. */
    name.sun_family = AF_LOCAL;
    strcpy (name.sun_path, socket_name);
    
    /* Connect the socket. */
    connect (socket_fd, &name, SUN_LEN (&name));
    
    /* Write the text on the command line to the socket. */
    write_text (socket_fd, message);
    
    close (socket_fd);
    unlink(sock
    return 0;
}