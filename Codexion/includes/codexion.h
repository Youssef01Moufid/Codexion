#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <limits.h>


typedef struct s_sim t_sim;

typedef enum s_scheduler
{
    FIFO,
    EDF
}    t_scheduler;


typedef struct s_waiter
{
    int     coder_id;
    long    priority;
}   t_waiter;

typedef struct s_pqueue
{
    t_waiter    *data;
    int         size;
    int         capacity;
}   t_pqueue;

typedef struct s_dongle
{
    int id;
    int available;
    long available_at;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    t_pqueue queue;
    t_sim *sim;
    
}   t_dongle;

typedef struct s_coder
{
    int     id;
    pthread_t   thread;
    int compile_count;
    long    last_compile_start;
    t_dongle   *left_dongle;
    t_dongle   *right_dongle;
    t_sim       *sim;
}   t_coder;

typedef struct s_sim
{
    int number_of_coders;
    long time_to_burnout;
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    int number_of_compiles_required;
    long dongle_cooldown;
    t_scheduler scheduler;
/*------------------------------------*/
    t_dongle   *dongles;
    t_coder     *coders;
    pthread_t   monitor;
    int         sim_stopped;
    pthread_mutex_t   stop_mutex;
    pthread_mutex_t   print_mutex;
    long        start_time;
}    t_sim;


bool    parsing(int argc, char **argv, t_sim *sim);
long    get_time_ms(void);
bool    init_sim(t_sim *sim);
void    cleanup(t_sim *sim);
void    log_state(t_sim *sim, int coder_id, char *mssg);
bool    pq_init(t_pqueue *pq, int capacity);
void    pq_push(t_pqueue *pq, int coder_id, long priority);
t_waiter    pq_pop(t_pqueue *pq);
void    pq_free(t_pqueue *pq);
t_waiter    pq_peek(t_pqueue *pq);
void    dongle_request(t_dongle *dongle, t_coder *coder);
void    dongle_release(t_dongle *dongle);
#endif // CODEXION_H