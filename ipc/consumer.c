#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "common.c"

struct message {
    long msg_type;
    char msg_text[SHM_SIZE];
};

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "[consumer] Usage: %s <out_file_path>\n", argv[0]);
        exit(1);
    }

    FILE *file = fopen(argv[1], "w");
    if (!file) {
        perror("[consumer] fopen failed");
        exit(1);
    }

    int msgid = msgget(MSG_KEY, 0666);
    if (msgid == -1) {
        perror("[consumer] msgget failed");
        exit(1);
    }

    struct message msg;
    while (1) {
        msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0);
        fprintf(file, "%s\n", msg.msg_text);
        printf("[consumer] written '%s'\n", msg.msg_text);

        if (msg.msg_text[0] == '\0') {
            break;
        }
    }

    fclose(file);
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
