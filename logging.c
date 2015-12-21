#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "logging.h"

const char *log_s[] = {"D", "I", "W", "E", "F", "M"};

void wlog(loglevel level, const char *s, ...)
{
    char fmt[strlen(s)+5];
    FILE *stream;
    va_list args;
    va_start(args, s);
    if ( level == MSG )
    {
        snprintf(fmt, sizeof(fmt), " %s\n", s);
        stream = stderr;
    }
    else
    {
        snprintf(fmt, sizeof(fmt), "%s: %s\n", log_s[level], s);
        stream = stderr;
    }
    vfprintf(stream, fmt, args);
    va_end(args);
    if ( level == FATAL ) exit(1);
}
