#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 2147483647
#define INT_MIN -2147483648

typedef enum s_scheduler
{
    FIFO,
    EDF
}    t_scheduler;

typedef struct s_dongles
{
    int id;
    int available;
    long available_at;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    t_pqueue queue;
    t_sim *sime;

}   t_dongles;

typedef struct s_sim
{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    t_scheduler scheduler;
}    t_sim;
bool    parsing(int argc, char **argv, t_sim *sim);
#endif // CODEXION_H