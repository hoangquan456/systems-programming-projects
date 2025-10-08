#ifndef IO_HELPERS_H
#define IO_HELPERS_H

#include <stdlib.h>
#include <unistd.h>

ssize_t write_all(int fd, const void *buf, size_t count);
char* read_all(int fd);

#endif