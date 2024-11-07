#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void write_benchmark(char* label, const char* file_path, char* buf, int buf_size);

int main() {
    char buf1[10];
    write_benchmark("buf size 10", "./test1", buf1, 10);
    char buf2[100];
    write_benchmark("buf size 100", "./test2", buf2, 100);
}

void write_benchmark(char* label, const char* file_path, char* buf, const int buf_size) {
    FILE *file = fopen(file_path, "ab+");
    if (setvbuf(file, buf, _IOFBF, buf_size) != 0) {
        perror("setvbuf failed");
        exit(EXIT_FAILURE);
    }

    const clock_t start = clock();
    for (int i = 0; i < 100000; i++) {
        if (fputs("Hello World\n", file) == -1) {
            perror("fputs failed");
            exit(EXIT_FAILURE);
        }
    }
    const clock_t end = clock();
    if (fclose(file) != 0) {
        perror("fclose failed");
        exit(EXIT_FAILURE);
    }

    const double write_duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Benchmark %s took %f seconds\n", label, write_duration);
}