// Symmetry: Bracket counting IRC daemon
//  LICENSE: MPL2

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "handle.h"
#include "logging.h"
#include "net.h"
#include "config.h"

#define MAXBUF 4096

char *host = "localhost";
char *port = "6667";

char *pass = "ougi:oshino";

char *nick = "ircat";
char *chan = "#spam";

int main(int argc, char* argv[])
{
    int fd;
    ssize_t len;
    char buffer[MAXBUF];
    
    switch ( argc )
    {
    default:
    case 3:
        port = argv[2];
    case 2:
        host = argv[1];
        break;
    case 1:
    case 0:
        break;
    }

    len = 1;
    buffer[0] = buffer[1] = '\0';
    while ( len > 0 )
    {
        char *end, *str = buffer;
        while ( str )
        {
            end = strchr(str, '\n');
            if ( end ) *(end-1) = '\0';
            handle(fd, str);
            str = end ? ( *(end+1) == '\0' ? NULL : end+1 ) : NULL;
        }
        len = recv(fd, buffer, MAXBUF-1, 0);
        buffer[len] = '\0';
    }
    if ( len == -1 )
        die("Error reading from socket");
    close(fd);
    return 0;
}
