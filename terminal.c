#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void parse(char *input, char* args[]);
void parse_with_strtok(char *input, char *args[]);
void exec_cmd(char *args[]);
void sig_handler(int signo);
void print_line_cursor();

int main() {
    // Install signal handlers
    signal(SIGINT, sig_handler);
    signal(SIGUSR1, sig_handler);

    int line_counter = 1;
    while (true) {
        print_line_cursor();
        fflush(stdout);

        char input[1024];
        const char *input_result = fgets(input, 1024, stdin);
        if (input_result == NULL) {
            if (feof(stdin)) {
                break;
            } else {
                perror("failed reading from stdin");
                continue;
            }
        }

        // Remove the newline character
        input[strcspn(input, "\n")] = '\0';

        char *args[64];
        if (line_counter % 2 == 1) {
            parse(input, args);
        } else {
            parse_with_strtok(input, args);
        }

        if (args[0] == NULL || strlen(args[0]) == 0) {
            line_counter++;
            continue;
        }

        exec_cmd(args);

        line_counter++;
    }
}

static void safe_print(const char *msg) {
    write(STDOUT_FILENO, msg, strlen(msg));
}


void sig_handler(int signo) {
    if (signo == SIGINT) {
        safe_print("Received SIGINT\n");
    } else if (signo == SIGUSR1) {
        safe_print("Received SIGUSR1\n");
    }
    print_line_cursor();
}

void print_line_cursor() {
    const char* out = "➜ ";
    safe_print("➜ ");
}

void exec_cmd(char *args[]) {
    const pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
    } else if (pid == 0) {
        const int result = execvp(args[0], args);
        if (result == -1) {
            perror("execvp failed");
            _exit(1); // Exit child process
        }
    } else {
        // Parent process: wait for child to finish to prevent zombie
        int status;
        waitpid(pid, &status, 0);
    }
}

void parse(char *input, char* args[]) {
    const char delim = ' ';
    const char replacer = '\0';
    char *current = input;

    while (*current == delim) {
        current++;
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
}

void parse_with_strtok(char *input, char *args[]) {
    int args_index = 0;
    char *token = strtok(input, " ");
    while (token != NULL && args_index < 64 - 1) {
        args[args_index++] = token;
        token = strtok(NULL, " ");
    }
    args[args_index] = NULL;
}