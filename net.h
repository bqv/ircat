#ifndef NET_H
#define NET_H

int irc_connect(char *host, char *port);

void reply(int fd, const char *fmt, ...);

#endif /* NET_H */
