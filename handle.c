#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "net.h"
#include "handle.h"
#include "config.h"

enum {WAITING, REGISTERED} state = WAITING;

void handle(int fd, char *line)
{
    char *hostmask, *command, *param, *args;
    wlog(MSG, "==> %s", line);
    if ( state == WAITING )
    {
        reply(fd, "USER %s 8 * :IRCat", nick);
        reply(fd, "NICK %s", nick);
        #ifdef PASS
            reply(fd, "PASS %s", pass);
        #endif
        state = REGISTERED;
        return;
    }

    hostmask = line; // w/ :
    command = strchr(hostmask, ' ');
    if ( strcmp("ERROR", hostmask) == 0 )
    {
        wlog(DEBUG, "Server closed the connection");
        exit(0);
    }
    else if ( command )
    {
        *command = '\0';
        param = strchr(++command, ' ');
        if ( strcmp("PING", hostmask) == 0 )
            reply(fd, "PONG %s", command);
        else if ( param )
        {
            *param = '\0';
            args = strchr(++param, ' ');
            if ( strcmp("001", command) == 0 )
                reply(fd, "JOIN %s", chan);
        }
    }
}
