#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "io_helpers.h"

int extract_leftmost_bits(const char *msg) {
    int task_id, n;

    //parse "task_id_n"
    if (sscanf(msg, "%d_%d", &task_id, &n) != 2) {
        fprintf(stderr, "Invalid task message format: %s\n", msg);
        return -1;
    }

    //get total number of bits in task_id
    int total_bits = 0;
    for (int tmp = task_id; tmp > 0; tmp >>= 1)
        total_bits++;

    //shift right to keep the leftmost n bits
    int bits_to_discard = total_bits - n;
    if (bits_to_discard < 0) bits_to_discard = 0;

    return task_id >> bits_to_discard;
}

void no_interrupt_sleep(int sec)
{
    // * advanced sleep which will not be interfered by signals
    struct timespec req, rem;

    req.tv_sec = sec;  // The time to sleep in seconds
    req.tv_nsec = 0; // Additional time to sleep in nanoseconds

    while(nanosleep(&req, &rem) == -1)
        if(errno == EINTR)
            req = rem;
}

void parse_args(int argc, char *argv[], int *id, int *read_fd, int *write_fd) {
    if (argc != 4) {
        fprintf(stderr, "Missing arguments");
        exit(EXIT_FAILURE);
    }
    sscanf(argv[1], "%d", id);
    sscanf(argv[2], "%d", read_fd);
    sscanf(argv[3], "%d", write_fd);
}

void handle_task(int id, const char *msg, int write_fd, int parent_pid) {
    printf("\033[1;32m[CORE %d]\033[0m Received task \033[1;35m%s\033[0m\n", id + 1, msg);
    no_interrupt_sleep(1);

    int extracted = extract_leftmost_bits(msg);

    char result_str[32];
    snprintf(result_str, sizeof(result_str), "%d", extracted);

    printf("\033[1;32m[CORE %d]\033[0m Finished with result: \033[1;33m%s\033[0m\n", id + 1, result_str);
    write_all(write_fd, result_str, strlen(result_str) + 1);  //include NULL terminator
    if (kill(parent_pid, SIGRTMIN + id) == -1) {
        perror("kill failed");
        exit(EXIT_FAILURE);
    }
}

void core_loop(int id, int read_fd, int write_fd) {
    int parent_pid = getppid();

    while (1) {  //core loop, waiting for tasks from main
        char *msg = read_all(read_fd);
        if (msg == NULL) break;  //main closed the write-end. core stop the loop

        handle_task(id, msg, write_fd, parent_pid);
        free(msg);
    }
}

void cleanup(int id, int read_fd, int write_fd) {
    if (close(read_fd) == -1) {
        perror("close read_fd failed");
        exit(EXIT_FAILURE);
    }
    
    if (close(write_fd) == -1) {
        perror("close write_fd failed");
        exit(EXIT_FAILURE);
    }

    printf("\033[1;32m[CORE %d]\033[0m Closed pipes and exiting\n", id + 1);
}

int main(int argc, char *argv[]) {
    int id, read_fd, write_fd;

    parse_args(argc, argv, &id, &read_fd, &write_fd);
    core_loop(id, read_fd, write_fd);
    cleanup(id, read_fd, write_fd);

    exit(0);    
}