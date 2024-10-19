#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_usage();

int main(const int argc, char* argv[]) {
    int lines_to_read = 5;
    const char *input_file_path = NULL;
    if (argc == 4) {
        if (strcmp(argv[1], "-n") == 0) {
            lines_to_read = atoi(argv[2]);
            input_file_path = argv[3];
        } else {
            printf("Unknown flag: %s", argv[1]);
            print_usage();
            exit(1);
        }
    } else if (argc == 2) {
        input_file_path = argv[1];
    } else {
        print_usage();
        exit(1);
    }

    FILE *input_file = fopen(input_file_path, "r");
    const int input_file_fd = fileno(input_file);
    off_t offset_from_end = -1;

    int seen_newlines = 0;
    while (1) {
        const off_t offset_from_start = lseek(input_file_fd, offset_from_end, SEEK_END);
        if (offset_from_start <= 0) {
            // We haven't seen the specified number of newlines,
            // but are already at the beginning of the file.
            // This also handles the case of empty files.
            break;
        }
        char input;
        if (read(input_file_fd, &input, 1) == -1) {
            printf("Error reading bytes from input file\n");
            exit(1);
        }
        // Ignore newline if it's the very last character of the file
        if (input == '\n' && offset_from_end != -1) {
            seen_newlines++;
        }
        if (seen_newlines == lines_to_read) {
            break;
        }
        // Step towards the start of the file
        offset_from_end -= 1;
    }

    const unsigned long output_buffer_size = (-offset_from_end) * sizeof(char);
    char *output_buffer = malloc(output_buffer_size);
    read(input_file_fd, output_buffer, output_buffer_size);
    write(STDOUT_FILENO, output_buffer, output_buffer_size);
    free(output_buffer);

    fclose(input_file);
}

void print_usage() {
    printf("Usage: ./rep [-n 3] <file>\n");
}