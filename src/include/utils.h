#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stddef.h>

char *cwd(void);
char **ls(const char *dir, size_t *out_len);
char *resolve_path(const char *fp);
void cd(const char *fp);
void rm_file(const char *fp);
int is_dir(const char *path);
void rm_dir(const char *fp);

#endif // UTILS_H_INCLUDED
