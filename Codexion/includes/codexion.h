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

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_waiter
{
	int		coder_id;
	long	key;
	long	seq;
	int		gen;
}	t_waiter;

typedef struct s_pqueue
{
	t_waiter	*data;
	int			size;
	int			capacity;
}	t_pqueue;

typedef struct s_dongle
{
	int				id;
	int				available;
	long			available_at;
	long			next_seq;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_pqueue		queue;
	t_sim			*sim;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	int			compile_count;
	long		last_compile_start;
	int			gen_left;
	int			gen_right;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_sim		*sim;
}	t_coder;

typedef struct s_sim
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	t_scheduler		scheduler;

	t_dongle			*dongles;
	t_coder				*coders;
	pthread_t			monitor;

	int					sim_stopped;
	pthread_mutex_t		stop_mutex;
	pthread_mutex_t		print_mutex;
	long				start_time;
}	t_sim;

bool	parsing(int argc, char **argv, t_sim *sim);
bool	init_sim(t_sim *sim);
void	cleanup(t_sim *sim);

long	get_time_ms(void);
void	precise_sleep(long ms);

int		is_stopped(t_sim *sim);
void	stop_sim(t_sim *sim);

void	log_state(t_sim *sim, int coder_id, const char *msg);
void	log_burnout(t_sim *sim, int coder_id);

bool		pq_init(t_pqueue *pq, int capacity);
void		pq_push(t_pqueue *pq, t_waiter w);
t_waiter	pq_pop(t_pqueue *pq);
t_waiter	pq_peek(t_pqueue *pq);
void		pq_free(t_pqueue *pq);
void		pq_drop_invalid_top(t_pqueue *pq, t_dongle *d);

void    dongle_request(t_dongle *dongle, t_coder *coder, int is_left);
int     dongle_request_until(t_dongle *dongle, t_coder *coder, int is_left, long until_ms);
void    dongle_release(t_dongle *dongle);

void	*monitor_routine(void *arg);
void	*coder_routine(void *arg);

#endif