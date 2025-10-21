#include "io.h"

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

char
getch(void)
{
        char ch;
        int _ = read(STDIN_FILENO, &ch, 1);
        (void)_;
        return ch;
}

input_type
get_input(char *c)
{
        assert(c);
        while (1) {
                *c = getch();
                if (ESCSEQ(*c)) {
                        int next0 = getch();
                        if (CSI(next0)) {
                                int next1 = getch();
                                if (next1 >= '0' && next1 <= '9') { // Modifier key detected
                                        int semicolon = getch();
                                        if (semicolon == ';') {
                                                int modifier = getch();
                                                int arrow_key = getch();
                                                if (modifier == '2') { // Shift modifier
                                                        switch (arrow_key) {
                                                        case 'A': *c = UP_ARROW;    return INPUT_TYPE_SHIFT_ARROW;
                                                        case 'B': *c = DOWN_ARROW;  return INPUT_TYPE_SHIFT_ARROW;
                                                        case 'C': *c = RIGHT_ARROW; return INPUT_TYPE_SHIFT_ARROW;
                                                        case 'D': *c = LEFT_ARROW;  return INPUT_TYPE_SHIFT_ARROW;
                                                        default: return INPUT_TYPE_UNKNOWN;
                                                        }
                                                }
                                        }
                                        return INPUT_TYPE_UNKNOWN;
                                } else { // Regular arrow key
                                        switch (next1) {
                                        case DOWN_ARROW:
                                        case RIGHT_ARROW:
                                        case LEFT_ARROW:
                                        case UP_ARROW:
                                                *c = next1;
                                                return INPUT_TYPE_ARROW;
                                        default:
                                                return INPUT_TYPE_UNKNOWN;
                                        }
                                }
                        } else { // [ALT] key
                                *c = next0;
                                return INPUT_TYPE_ALT;
                        }
                }
                else if (*c >= CTRL_A && *c <= CTRL_Z && *c != CTRL_J) {
                        return INPUT_TYPE_CTRL;
                }
                else return INPUT_TYPE_NORMAL;
        }
        return INPUT_TYPE_UNKNOWN;
}
