#include "utils.h"
#include "term.h"
#include "io.h"

#include <forge/colors.h>
#include <forge/array.h>

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

struct {
        struct {
                struct termios win;
                size_t r;
                size_t c;
        } term;
} g_config = {
        .term = {
                .win = {0},
                .r = 0,
                .c = 0,
        },
};

void
display(void)
{
        clear_term();

        char *dir = cwd();

        size_t row = 0;

        int_array marked = dyn_array_empty(int_array);

        while (1) {
                size_t files_n;
                char **files = ls(dir, &files_n);

                for (size_t i = 0; i < files_n; ++i) {
                        if (!strcmp(files[i], "..")) {
                                char *tmp = files[1];
                                files[1] = files[i];
                                files[i] = tmp;
                        } else if (!strcmp(files[i], ".")) {
                                char *tmp = files[0];
                                files[0] = files[i];
                                files[i] = tmp;
                        }
                }

                printf(YELLOW "  %s" RESET "\n", dir);

                for (size_t i = 0; i < files_n && i < g_config.term.r; ++i) {
                        int is_marked = 0;
                        for (size_t j = 0; j < marked.len; ++j) {
                                if (marked.data[j] == i) {
                                        is_marked = 1;
                                        printf(YELLOW);
                                        break;
                                }
                        }

                        if (row == i) printf(INVERT);
                        printf("%s\n", files[i]);
                        if (row == i) printf(RESET);

                        if (is_marked) printf(RESET);
                }

                char ch;
                input_type ty = get_input(&ch);

                switch (ty) {
                case INPUT_TYPE_NORMAL: {
                        if      (ch == 'j') ++row;
                        else if (ch == 'k') --row;
                        else if (ch == 'q') goto done;
                        else if (ch == 'd') rm_file(files[row]);
                        else if (ch == 'm') dyn_array_append(marked, row);
                        else if (ch == '\n') {
                                char *realpath = resolve_path(files[row]);
                                free(dir);
                                cd(realpath);
                                dir = realpath;
                        }
                } break;
                case INPUT_TYPE_ARROW: {
                        if      (ch == UP_ARROW)   --row;
                        else if (ch == DOWN_ARROW) ++row;
                } break;
                }

                clear_term();
        }

 done:
}

int
main(void)
{
        enable_raw();
        display();
        disable_raw();

        return 0;
}
