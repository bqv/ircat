#ifndef LOGGING_H
#define LOGGING_H

typedef enum {DEBUG, INFO, WARN, ERROR, FATAL, MSG} loglevel;

#define die(...) wlog(FATAL, __VA_ARGS__)

void wlog(loglevel level, const char *s, ...);

#endif /* LOGGING_H */
