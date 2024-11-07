#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void append_stdin(FILE *file);

void delete_line(FILE *in_file, const char *in_file_name, const int delete_line_num);

void insert_line(FILE *in_file, const char *in_file_name, const int insert_at_line_num);

int main(const int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  ./kamned <filename> -a\n");
        fprintf(stderr, "  ./kamned <filename> -d n\n");
        fprintf(stderr, "  ./kamned <filename> -i n\n");
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];

    if (strcmp(argv[2], "-a") == 0) {
        FILE *file = fopen(filename, "a");
        if (file == NULL) {
            perror("fopen failed");
            exit(1);
        }
        append_stdin(file);
    } else if (strcmp(argv[2], "-d") == 0) {
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("fopen failed");
            exit(1);
        }
        const int line_num = atoi(argv[3]);
        delete_line(file, filename, line_num);
    } else if (strcmp(argv[2], "-i") == 0) {
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("fopen failed");
            exit(1);
        }
        const int line_num = atoi(argv[3]);
        insert_line(file, filename, line_num);
    } else {
        fprintf(stderr, "Unknown flag %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
}

void append_stdin(FILE *file) {
    char buffer[10];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (fputs(buffer, file) == EOF) {
            perror("fputs failed");
            exit(EXIT_FAILURE);
        }
    }
    if (fclose(file) == EOF) {
        perror("fclose failed");
        exit(EXIT_FAILURE);
    }
}

void delete_line(FILE *in_file, const char *in_file_name, const int delete_line_num) {
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("fopen failed");
        fclose(temp_file);
        exit(EXIT_FAILURE);
    }
    char rewrite_buf[100];
    int line_num = 1;
    while (fgets(rewrite_buf, sizeof(rewrite_buf), in_file) != NULL) {
        if (line_num != delete_line_num) {
            if (fputs(rewrite_buf, temp_file) == EOF) {
                perror("fputs failed");
                exit(EXIT_FAILURE);
            }
        }
        line_num++;
    }
    fclose(in_file);
    fclose(temp_file);
    unlink(in_file_name);
    rename("temp.txt", in_file_name);
}

void insert_line(FILE *in_file, const char *in_file_name, const int insert_at_line_num) {
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("fopen failed");
        fclose(temp_file);
        exit(EXIT_FAILURE);
    }
    char rewrite_buf[100];
    int line_num = 1;
    while (fgets(rewrite_buf, sizeof(rewrite_buf), in_file) != NULL) {
        if (line_num == insert_at_line_num) {
            char in_buf[100];
            if (fgets(in_buf, sizeof(in_buf), stdin) != NULL) {
                fputs(in_buf, temp_file);
            }
        }
        fputs(rewrite_buf, temp_file);
        line_num++;
    }
    fclose(in_file);
    fclose(temp_file);
    unlink(in_file_name);
    rename("temp.txt", in_file_name);
}
