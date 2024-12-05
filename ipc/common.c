#include <sys/sem.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define SHM_SIZE 256
#define MSG_KEY 91011

void sem_lock(const int sem_id) {
    struct sembuf op = {0, -1, 0};
    semop(sem_id, &op, 1);
}

void sem_unlock (const int sem_id) {
    struct sembuf op = {0, 1, 0};
    semop(sem_id, &op, 1);
}