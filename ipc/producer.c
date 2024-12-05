#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "common.c"

#define N_MESSAGES 5

int main() {
    // Allocate shared memory segment
    const int shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("[producer] shmget failed");
        exit(1);
    }
    // Attach to the shared memory
    char *shm_addr = shmat(shm_id, NULL, 0);
    if (shm_addr == (char *)-1) {
        perror("[producer] shmat");
        exit(1);
    }

    // Create a semaphore
    const int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("[producer] semget");
        exit(1);
    }
    // Initialize the semaphore (1 because there is only 1 producer)
    semctl(sem_id, 0, SETVAL, 1);

    for (int i = 1; i <= N_MESSAGES; i++) {
        sleep(2);
        sem_lock(sem_id);

        printf("[producer]: produced message '%s'\n", shm_addr);

        if (i < N_MESSAGES) {
            snprintf(shm_addr, SHM_SIZE, "ID %d: To je sporocilo.", i);
        } else {
            shm_addr[0] = '\0';
        }

        sleep(2);
        sem_unlock(sem_id);
    }

    shmdt(shm_addr);
    return 0;
}