#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

long int set_perm(const char* perm_str, long int perm);

int main(const int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: mojChmod <perm> <pathname>\n");
        return 1;
    }
    const char* perm_str = argv[1];
    const char* path_str = argv[2];

    char* end_perm_str = NULL;
    long int perm = strtol(perm_str, &end_perm_str, 8);

    if (perm_str == end_perm_str) {
        struct stat st;
        const int status = stat(path_str, &st);
        if (status != 0) {
            perror("stat failed");
            exit(EXIT_FAILURE);
        }

        perm = set_perm(perm_str, st.st_mode);
    }

    const int status = chmod(path_str, perm);

    if (status == -1) {
        perror("chmod failed");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

long int set_perm(const char* perm_str, long int perm) {
    if (strcmp(perm_str, "u+s") == 0) {
        return perm | S_ISUID;
    }
    if (strcmp(perm_str, "u-s") == 0) {
        return perm & ~S_ISUID;
    }
    if (strcmp(perm_str, "g+s") == 0) {
        return perm | S_ISGID;
    }
    if (strcmp(perm_str, "g-s") == 0) {
        return perm & ~S_ISGID;
    }
    return 0;
}