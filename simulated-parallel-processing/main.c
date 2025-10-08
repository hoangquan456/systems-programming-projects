
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "io_helpers.h"

#define CORE_NUM 3

void assign_task(int task_id, int max_bits, int core, int main_to_core[CORE_NUM][2]) {
    int n = rand() % (max_bits + 1); //random integer between 0 and max_bits
    
    char task[128]; 
    snprintf(task, sizeof(task), "%d_%d", task_id, n);
    
    printf("\033[1;34m[MAIN]\033[0m Assign \033[1;35m%d_%d\033[0m to \033[1;36mCore %d\033[0m\n", task_id, n, core + 1);
    
    if (write_all(main_to_core[core][1], task, strlen(task) + 1) == -1) {
        fprintf(stderr, "Assigning failed!");
        exit(EXIT_FAILURE);
    }
}

volatile sig_atomic_t msg_num_core[CORE_NUM] = {0};
void handler(int sig) {
    msg_num_core[sig - SIGRTMIN]++;
}

void signal_handling_setup() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));       
    sa.sa_handler = handler;         
    sigemptyset(&sa.sa_mask);         
    sa.sa_flags = 0;                 

    for(int i = 0; i < CORE_NUM; ++i) {
        if (sigaction(SIGRTMIN + i, &sa, NULL) == -1) {
            fprintf(stderr, "Failed to register signal handler for SIGRTMIN + %d: ", i);
            perror("");
            exit(EXIT_FAILURE);
        }
    }
}

void parse_args(int argc, char *argv[], int *num_tasks, int *max_bits) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_tasks> <max_bits>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *endptr1, *endptr2;
    *num_tasks = strtol(argv[1], &endptr1, 10);
    *max_bits = strtol(argv[2], &endptr2, 10);

    if (*endptr1 != '\0' || *endptr2 != '\0' || *num_tasks <= 0 || *max_bits <= 0) {
        fprintf(stderr, "Both arguments must be valid positive integers.\n");
        exit(EXIT_FAILURE);
    }
}

void create_core(int i, int main_to_core[CORE_NUM][2], int core_to_main[CORE_NUM][2]) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid) {
        printf("\033[1;34m[MAIN]\033[0m \033[1;36mCore %d\033[0m created\n", i + 1);
        if (close(main_to_core[i][0]) == -1) {  
            perror("close main_to_core[i][0]");
            exit(EXIT_FAILURE);
        }
        if (close(core_to_main[i][1]) == -1) { 
            perror("close core_to_main[i][1]");
            exit(EXIT_FAILURE);
        }
    } else {
        for (int j = 0; j <= i; ++j) {   //Close all unrelated pipes
            if (close(main_to_core[j][1]) == -1) {
                perror("close main_to_core[j][1]");
                exit(EXIT_FAILURE);
            }
            if (close(core_to_main[j][0]) == -1) {
                perror("close core_to_main[j][0]");
                exit(EXIT_FAILURE);
            }
        }

        //prepare id, write_fd, read_fd to send to child 
        char id_str[12], fd_read_str[12], fd_write_str[12];
        snprintf(id_str, sizeof(id_str), "%d", i);
        snprintf(fd_read_str, sizeof(fd_read_str), "%d", main_to_core[i][0]);
        snprintf(fd_write_str, sizeof(fd_write_str), "%d", core_to_main[i][1]);

        char *argv[] = { "./process", id_str, fd_read_str, fd_write_str, NULL };
        execvp("./process", argv);
        perror("exec failed");
        exit(EXIT_FAILURE);
    }
}

void initialize_cores(int idle[], int main_to_core[CORE_NUM][2], int core_to_main[CORE_NUM][2]) {
    for (int i = 0; i < CORE_NUM; ++i) {
        if (pipe(main_to_core[i]) == -1) {
            perror("pipe (main_to_core)");
            exit(EXIT_FAILURE);
        }
        
        if (pipe(core_to_main[i]) == -1) {
            perror("pipe (core_to_main)");
            exit(EXIT_FAILURE);
        }
        
        create_core(i, main_to_core, core_to_main);
        idle[i] = 1;  //core is idle after created
    }
}

void schedule_tasks(int num_tasks, int max_bits, int idle[], int result[][8192], int result_count[],
                    int main_to_core[CORE_NUM][2], int core_to_main[CORE_NUM][2]) {
    int task_id = 0, completed_task = 0;

    while (completed_task < num_tasks) {   //keep looping until all tasks are finished
        for (int core = 0; core < CORE_NUM; ++core) {
            if (msg_num_core[core] > 0) {  //there is result from core
                --msg_num_core[core];

                int value;
                char *msg = read_all(core_to_main[core][0]);  //read result from read-end of pipe
                sscanf(msg, "%d", &value);
                free(msg);
                result[core][result_count[core]++] = value;  //store to result array
                ++completed_task;
                printf("\033[1;34m[MAIN]\033[0m Received result from \033[1;36mCore %d\033[0m: \033[1;33m%d\033[0m\n", core + 1, value);

                idle[core] = 1; //set core back to idle state
            }

            if (task_id < num_tasks && idle[core]) {
                idle[core] = 0;  //core is no longer idle.
                assign_task(task_id++, max_bits, core, main_to_core);
            }
        }
    }
}

void finalize(int result[][8192], int result_count[], int main_to_core[CORE_NUM][2], int core_to_main[CORE_NUM][2]) {
    for (int core = 0; core < CORE_NUM; ++core) {
        if (close(main_to_core[core][1]) == -1) {
            perror("close main_to_core write-end failed");
            exit(EXIT_FAILURE);
        }

        if (close(core_to_main[core][0]) == -1) {
            perror("close core_to_main read-end failed");
            exit(EXIT_FAILURE);
        }

        if (wait(NULL) == -1) {
            perror("wait failed");
            exit(EXIT_FAILURE);
        }

        printf("\033[1;34m[MAIN]\033[0m Reaped and closed pipes for \033[1;36mCore %d\033[0m\n", core + 1);
    }

    for (int core = 0; core < CORE_NUM; ++core) {
        printf("\033[1;34m[MAIN]\033[0m \033[1;36mCore %d\033[0m: ", core + 1); 
    
        for (int i = 0; i < result_count[core]; ++i) {
            printf("\033[1;33m%d\033[0m", result[core][i]);  
            if (i != result_count[core] - 1)
                printf(", ");
        }
    
        printf("\n");
    }    
}

int main(int argc, char *argv[]) {
    int num_tasks, max_bits;
    int idle[CORE_NUM] = {0};   //idle status of each core. 0 means NOT idle, 1 means idle.
    int result[CORE_NUM][8192];  
    int result_count[CORE_NUM] = {0};
    int main_to_core[CORE_NUM][2];
    int core_to_main[CORE_NUM][2];

    parse_args(argc, argv, &num_tasks, &max_bits);
    signal_handling_setup();
    srand(time(NULL));
    initialize_cores(idle, main_to_core, core_to_main);
    schedule_tasks(num_tasks, max_bits, idle, result, result_count, main_to_core, core_to_main);
    finalize(result, result_count, main_to_core, core_to_main);
}