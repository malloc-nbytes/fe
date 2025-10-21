#include "utils.h"

#include <forge/cstr.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>

char *
cwd(void)
{
        size_t size = 1024;
        char *buf = NULL;

        while (1) {
                char *tmp = (char *)realloc(buf, size);
                if (!tmp) {
                        free(buf);
                        return NULL;
                }
                buf = tmp;

                if (getcwd(buf, size) != NULL) {
                        return buf;
                }

                if (errno == ERANGE) {
                        size *= 2;
                        continue;
                }

                free(buf);
                return NULL;
        }

        return NULL; // unreachable
}

char **
ls(const char *dir, size_t *out_len)
{
        DIR *dp = opendir(dir);
        if (!dp) {
                return NULL;
        }

        // Count files
        struct dirent *entry;
        int count = 0;
        while ((entry = readdir(dp))) {
                count++;
        }
        rewinddir(dp);

        char **files = malloc((count + 1) * sizeof(char *));
        if (!files) {
                closedir(dp);
                return NULL;
        }

        int i = 0;
        while ((entry = readdir(dp))) {
                files[i] = strdup(entry->d_name);
                if (!files[i]) {
                        for (int j = 0; j < i; j++) {
                                free(files[j]);
                        }
                        free(files);
                        closedir(dp);
                        return NULL;
                }
                i++;
        }
        files[count] = NULL;

        closedir(dp);
        *out_len = count;
        return files;
}

char *
resolve_path(const char *fp)
{
        char *result = NULL;

        if (!fp || !*fp) { return NULL; }

        if (fp[0] == '~') {
                struct passwd *pw = getpwuid(getuid());
                if (!pw) {
                        return NULL;
                }

                size_t home_len = strlen(pw->pw_dir);
                size_t fp_len = strlen(fp);
                result = malloc(home_len + fp_len);
                if (!result) { return NULL; }

                strcpy(result, pw->pw_dir);
                strcat(result, fp + 1); // Skip the ~
        } else {
                result = strdup(fp);
                if (!result) {
                        return NULL;
                }
        }

        char *absolute = realpath(result, NULL);
        free(result);

        if (!absolute) {
                return NULL;
        }

        return absolute;
}

void
cd(const char *fp)
{
        if (chdir(fp) != 0) {
                perror("chdir");
                exit(1);
        }
}

int
is_dir(const char *path)
{
        struct stat path_stat;
        if (stat(path, &path_stat) != 0) {
                return 0;
        }
        return S_ISDIR(path_stat.st_mode);
}

void
rm_dir(const char *fp)
{
        size_t files_n = 0;
        char **files = ls(fp, &files_n);

        for (size_t i = 0; i < files_n; ++i) {
                if (!strcmp(files[i], "..")) continue;
                if (!strcmp(files[i], "."))  continue;
                char *path = forge_cstr_builder(fp, "/", files[i], NULL);
                if (is_dir(path)) {
                        rm_dir(path);
                } else {
                        rm_file(path);
                }
                free(path);
                free(files[i]);
        }

        if (remove(fp) != 0) {
                perror("remove");
                exit(1);
        }
}

void
rm_file(const char *fp)
{
        if (is_dir(fp)) {
                rm_dir(fp);
        } else {
                if (remove(fp) != 0) {
                        perror("remove");
                        exit(1);
                }
        }
}
