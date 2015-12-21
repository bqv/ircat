#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "logging.h"
#include "net.h"
#include "config.h"

int irc_connect(char *host, char *port)
// Fill in addrinfo, and format ip to string
{
    int sockfd, retval;
    struct addrinfo hints;
    struct addrinfo *ai, *p;
    char ip[INET6_ADDRSTRLEN];

    wlog(DEBUG, "Selected host %s with port %s", host, port);
  
    memset(&hints, 0, sizeof hints);
    hints.ai_family = V4_ONLY ? AF_INET : AF_UNSPEC; // IPv4 or 6 (AF_INET or AF_INET6)
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // Autodetect local host
    if ( (retval = getaddrinfo(host, port, &hints, &ai)) != 0 )
        die("Failed to resolve host: %s (%s)", host, gai_strerror(retval));

    for ( p = ai; p != NULL; p = p->ai_next )
    {
        void *server; // sockaddr_in or sockaddr_in6
        if ( p->ai_family == AF_INET )
        { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            server = &(ipv4->sin_addr);
        }
        else
        { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            server = &(ipv6->sin6_addr);
        }
        inet_ntop(p->ai_family, server, ip, sizeof(ip));

        wlog(DEBUG, "Connecting to %s (%s)... ", host, ip);

        if ( (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0 )
        { 
            wlog(DEBUG, "Could not create socket");
            continue;
        }

        if ( (retval = connect(sockfd, p->ai_addr, p->ai_addrlen)) < 0 )
            die("Connection to %s failed [%d]", retval);
        else break;
    }
    if ( p == NULL ) 
        die("Could not connect to host: %s", host);
    else wlog(INFO, "Connected!");

    freeaddrinfo(ai);
        
    return sockfd;
}

void reply(int fd, const char *fmt, ...)
{
    size_t len;
    char line[512];
    va_list args;
    va_start(args, fmt);
    len = vsnprintf(line, 510, fmt, args);
    va_end(args);
    wlog(MSG, "<== %s", line);
    strncat(line, "\r\n", 2);
    send(fd, line, len+2, 0);
}
