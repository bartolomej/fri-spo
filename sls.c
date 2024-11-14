#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char format_file_type(int st_mode);
void format_permissions(int st_mode, char *perm_str);

int main() {
    DIR *d = opendir(".");

    if (d == NULL) {
        perror("opendir failed");
        exit(EXIT_FAILURE);
    }

    struct dirent *dir;
    struct stat st;
    while ((dir = readdir(d)) != NULL) {
        const char* path = dir->d_name;
        const int status = stat(path, &st);

        if (status != 0) {
            perror("stat failed");
            exit(EXIT_FAILURE);
        }

        char permissions[11];
        format_permissions(st.st_mode, permissions);

        printf("%c%s %s\n", format_file_type(st.st_mode), permissions, path);
    }
    closedir(d);

    return 0;
}

char format_file_type(const int st_mode) {
    if (S_ISREG(st_mode)) {
        return '-';
    }
    if (S_ISDIR(st_mode)) {
        return 'd';
    }
    if (S_ISCHR(st_mode)) {
        return 'c';
    }
    if (S_ISBLK(st_mode)) {
        return 'b';
    }
    if (S_ISFIFO(st_mode)) {
        return 'p';
    }
    if (S_ISLNK(st_mode)) {
        return 'l';
    }
    if (S_ISSOCK(st_mode)) {
        return 's';
    }
    // Unreachable
    exit(EXIT_FAILURE);
}

void format_permissions(const int st_mode, char *perm_str) {
    perm_str[0] = (st_mode & S_IRUSR) ? 'r' : '-';
    perm_str[1] = (st_mode & S_IWUSR) ? 'w' : '-';
    perm_str[2] = (st_mode & S_IXUSR) ? ((st_mode & S_ISUID) ? 's' : 'x') : ((st_mode & S_ISUID) ? 'S' : '-');
    perm_str[3] = (st_mode & S_IRGRP) ? 'r' : '-';
    perm_str[4] = (st_mode & S_IWGRP) ? 'w' : '-';
    perm_str[5] = (st_mode & S_IXGRP) ? ((st_mode & S_ISGID) ? 's' : 'x') : ((st_mode & S_ISGID) ? 'S' : '-');
    perm_str[6] = (st_mode & S_IROTH) ? 'r' : '-';
    perm_str[7] = (st_mode & S_IWOTH) ? 'w' : '-';
    perm_str[8] = (st_mode & S_IXOTH) ? ((st_mode & S_ISVTX) ? 't' : 'x') : ((st_mode & S_ISVTX) ? 'T' : '-');
    perm_str[9] = '\0';
}