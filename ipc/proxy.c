#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include "common.c"

struct message {
    long msg_type;
    char msg_text[SHM_SIZE];
};

int main() {
    const int shm_id = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shm_id == -1) {
        perror("[proxy] shmget failed");
        exit(1);
    }
    char *shm_addr = shmat(shm_id, NULL, 0);
    if (shm_addr == (char *)-1) {
        perror("[proxy] shmat failed");
        exit(1);
    }

    const int sem_id = semget(SEM_KEY, 1, 0666);
    if (sem_id == -1) {
        perror("[proxy] semget failed");
        exit(1);
    }

    const int msg_id = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msg_id == -1) {
        perror("[proxy] msgget failed");
        exit(1);
    }

    char last_message[SHM_SIZE] = "";
    while (1) {
        sleep(1);
        sem_lock(sem_id);

        printf("[proxy] message '%s'\n", shm_addr);

        if (strcmp(shm_addr, last_message) != 0) {
            struct message msg;
            msg.msg_type = 1;
            strncpy(msg.msg_text, shm_addr, SHM_SIZE);

            msgsnd(msg_id, &msg, sizeof(msg.msg_text), 0);
            printf("[proxy] forwarded: '%s'\n", shm_addr);

            strcpy(last_message, shm_addr);
        }

        sem_unlock(sem_id);

        if (shm_addr[0] == '\0') {
            printf("[proxy] stopping\n");
            break;
        }
    }

    shmdt(shm_addr);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    return 0;
}