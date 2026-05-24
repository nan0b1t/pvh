#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);

    srand(time(NULL));

    char* buf = malloc(512);
    if (buf == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }
    int reads = 0;

    int totalBytes = 0;

    if (argc < 3) {
        printf("Usage: pvh [speed] [accuracy] [file]\n");
        free(buf);
        exit(1);
    }

    int speed = atoi(argv[1]);
    if (speed <= 0) {
        free(buf);
        printf("enter a valid speed\n");
        exit(1);
    }

    double accuracy = atof(argv[2]);
    if (accuracy <= 0.0) {
        free(buf);
        printf("enter a valid accuracy\n");
        exit(1);
    }

    if (argc > 3) {
        FILE *file;
        file = fopen(argv[3], "r");
        if (file == NULL) {
            perror("Error opening file");
            exit(1);
        }

        for (;;) {
            char* alloced = realloc(buf, 512 * (reads + 1));
            if (alloced == NULL) {
                perror("Memory allocation failed");
                exit(1);
            }
            buf = alloced;

            int fresult = fread(buf + (reads * 512), 1, 512, file);
            totalBytes += fresult;
            if (fresult != 512) {
                break;
            }

            reads++;
        }
    }

    _Bool error = 0;
    for (int i = 0; i < totalBytes; i++) {
        if (error) {
            printf("\x1b[1D");
        }

        if (rand() % 100 < accuracy) {
            printf("%c", buf[i]);
            error = 0;
        } else {
            if (buf[i] != '\n') {
                printf("%c", ' ' + (rand() % 94));
                i--;
                error = 1;
            } else {
                printf("\n");
            }
        }

        long nsPerChar = (long)(1e9 / speed);
        struct timespec sleep;
        sleep.tv_sec = nsPerChar / 1000000000;
        sleep.tv_nsec = nsPerChar % 1000000000;
        nanosleep(&sleep, NULL);
    }

    free(buf);
    return 0;
}
