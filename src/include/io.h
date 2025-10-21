#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#define CTRL_A 1
#define CTRL_B 2
#define CTRL_C 3
#define CTRL_D 4
#define CTRL_E 5
#define CTRL_F 6
#define CTRL_G 7
#define CTRL_H 8
#define CTRL_I 9
#define CTRL_J 10
#define CTRL_K 11
#define CTRL_L 12
#define CTRL_M 13
#define CTRL_N 14
#define CTRL_O 15
#define CTRL_P 16
#define CTRL_Q 17
#define CTRL_R 18
#define CTRL_S 19
#define CTRL_T 20
#define CTRL_U 21
#define CTRL_V 22
#define CTRL_W 23
#define CTRL_X 24
#define CTRL_Y 25
#define CTRL_Z 26

#define UP_ARROW    'A'
#define DOWN_ARROW  'B'
#define RIGHT_ARROW 'C'
#define LEFT_ARROW  'D'

#define ENTER(ch) ((ch) == '\n')
#define BACKSPACE(ch) ((ch) == 8 || (ch) == 127)
#define TAB(ch)       ((ch) == '\t')
#define ESCSEQ(ch)    ((ch) == 27)
#define CSI(ch)       ((ch) == '[')

#define CURSOR_LEFT(n)  printf("\033[%dD", n);
#define CURSOR_RIGHT(n) printf("\033[%dC", n);
#define CURSOR_UP(n)    printf("\033[%dA", n);
#define CURSOR_DOWN(n)  printf("\033[%dB", n);

typedef enum {
        INPUT_TYPE_NORMAL,
        INPUT_TYPE_ARROW,
        INPUT_TYPE_SHIFT_ARROW,
        INPUT_TYPE_CTRL,
        INPUT_TYPE_ALT,
        INPUT_TYPE_UNKNOWN,
} input_type;

input_type get_input(char *c);

#endif // IO_H_INCLUDED
