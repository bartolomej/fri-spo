#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>

int main(const int argc, char* argv[]) {
    bool append = false;
    char* output_file_path;
    if (argc == 3) {
        if (strcmp(argv[1], "-a") == 0) {
            append = true;
            output_file_path = argv[2];
        } else {
            printf("Unknown flag: %s", argv[1]);
            exit(1);
        }
    } else if (argc == 2) {
        output_file_path = argv[1];
    } else {
        printf("Usage: ./tee [-a] <output_file>\n");
        exit(1);
    }

    char *mode;
    if (append) {
        mode = "a";
    } else {
        mode = "w";
    }
    FILE* output_file = fopen(output_file_path, mode);
    if (output_file == NULL) {
        printf("Error opening output file: %s\n", output_file_path);
        exit(1);
    }

    const int input_buffer_size = 10;
    char input_buffer[input_buffer_size];
    ssize_t n_bytes_read = 0;
    while ((n_bytes_read = read(STDIN_FILENO, input_buffer, input_buffer_size)) > 0) {
        fwrite(input_buffer, 1, n_bytes_read, output_file);
    }

    fclose(output_file);

    if (errno > 0) {
        printf("Error reading from stdin: %s", strerror(errno));
    }

}
