#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED

#include <stddef.h>

void enable_raw(void);
void disable_raw(void);
int get_terminal_xy(size_t *win_width, size_t *win_height);
void clear_term(void);

#endif // TERM_H_INCLUDED
