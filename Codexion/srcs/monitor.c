#include "../includes/codexion.h"

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		int		done;
		long	now;

		done = 0;
		now = get_time_ms();
		i = 0;
		while (i < sim->number_of_coders)
		{
			long	last;
			int		count;

			pthread_mutex_lock(&sim->stop_mutex);
			last = sim->coders[i].last_compile_start;
			count = sim->coders[i].compile_count;
			pthread_mutex_unlock(&sim->stop_mutex);

			if (now - last > sim->time_to_burnout)
			{
				log_burnout(sim, sim->coders[i].id);
				stop_sim(sim);
				return (NULL);
			}
			if (count >= sim->number_of_compiles_required)
				done++;
			i++;
		}
		if (done == sim->number_of_coders)
		{
			stop_sim(sim);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
