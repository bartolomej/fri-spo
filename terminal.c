#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void parse(char *input, char* args[]);
void exec(char *args[]);

int main() {
    int line_counter = 0;
    while (true) {
        char input[1024];
        const char *input_result = fgets(input, 64, stdin);
        if (input_result == NULL) {
            perror("failed reading from stdin");
            continue;
        }

        // Remove the newline character
        input[strcspn(input, "\n")] = '\0';

        char *args[64];
        parse(input, args);
        exec(args);

        line_counter++;
    }
}

void exec(char *args[]) {
    const pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
    } else if (pid == 0) {
        const int result = execvp(args[0], args);
        if (result == -1) {
            perror("execvp failed");
        }
    } else {
        // TODO: Handle parent context
    }
}

void parse(char *input, char* args[]) {
    const char delim = ' ';
    const char replacer = '\0';

    char *current = input;
    while (true) {
        if (*current == delim) {
            current++;
        } else {
            break;
        }
    }

    int args_index = 0;
    args[args_index++] = current;


    while (true) {
        if (*current == '\0') {
            break;
        }
        const char* previous = current - 1;
        if (current != input && *previous == replacer && *current != delim) {
            args[args_index++] = current;
        }
        if (*current == delim) {
            *current = replacer;
        }
        current++;
    }

    args[args_index++] = NULL;
}