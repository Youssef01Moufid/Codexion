#include "../includes/codexion.h"

void	log_state(t_sim *sim, int coder_id, const char *msg)
{
	long	t;

	if (is_stopped(sim))
		return ;
	t = get_time_ms() - sim->start_time;
	pthread_mutex_lock(&sim->print_mutex);
	printf("%ld %d %s\n", t, coder_id, msg);
	pthread_mutex_unlock(&sim->print_mutex);
}

void	log_burnout(t_sim *sim, int coder_id)
{
	long	t;

	t = get_time_ms() - sim->start_time;
	pthread_mutex_lock(&sim->print_mutex);
	printf("%ld %d burned out\n", t, coder_id);
	pthread_mutex_unlock(&sim->print_mutex);
}