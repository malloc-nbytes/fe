#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "term.h"
#include "global.h"

void
enable_raw(void)
{
        struct termios raw;

        if (tcgetattr(STDIN_FILENO, &g_config.term.win) == -1) {
                perror("tcgetattr");
                exit(1);
        }

        raw = g_config.term.win;

        raw.c_lflag &= ~(ECHO | ICANON);
        raw.c_iflag &= ~IXON;

        if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1) {
                perror("tcsetattr");
                exit(1);
        }

        get_terminal_xy(&g_config.term.c, &g_config.term.r);
}

void
disable_raw(void)
{
        struct termios current;

        if (tcgetattr(STDIN_FILENO, &current) == -1) {
                perror("tcgetattr");
                exit(1);
        }

        if (current.c_lflag & ECHO && current.c_lflag & ICANON) {
                return;
        }

        if (tcsetattr(STDIN_FILENO, TCSANOW, &g_config.term.win) == -1) {
                perror("tcsetattr");
                exit(1);
        }
}

int
get_terminal_xy(size_t *win_width,
                size_t *win_height)
{
        if (!win_width && !win_height) return 0;

        if (win_width)  *win_width = 0;
        if (win_height) *win_height = 0;

        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
                if (win_width)  *win_width = w.ws_col-1;
                if (win_height) *win_height = w.ws_row-1;
        } else {
                return 0;
        }

        return 1;
}

void
clear_term(void)
{
        printf("\033[2J");
        printf("\033[H");
        fflush(stdout);
}
