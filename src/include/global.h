#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <termios.h>

extern struct {
        struct {
                struct termios win;
                size_t r;
                size_t c;
        } term;
} g_config;

#endif // GLOBAL_H_INCLUDED
