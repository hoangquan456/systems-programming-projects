
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "io_helpers.h"

ssize_t write_all(int fd, const void *buf, size_t count) {
    size_t total = 0;
    const char *ptr = buf;

    while (total < count) {
        ssize_t n = write(fd, ptr + total, count - total);
        if (n == -1) {
            if (errno == EINTR) continue;  // Retry if interrupted
            perror("write failed");
            return -1;
        }
        total += n;
    }

    return total;
}

char* read_all(int fd) {
    char *buffer = malloc(128 * sizeof(char));

    int total = 0;
    while (1) {
        ssize_t n = read(fd, buffer + total, 128);
        if (n == -1) {
            if (errno == EINTR) continue;  // Retry on interrupt
            perror("read failed");
            free(buffer);
            return NULL;
        }

        if (n == 0) {
            free(buffer);
            return NULL; //write-end close the pipe
        }

        total += n;
        if (buffer[total - 1] == '\0') break; 
    }

    return buffer;
}