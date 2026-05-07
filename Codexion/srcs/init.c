#include "../includes/codexion.h"

static bool	init_dongles(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->number_of_coders;
	sim->dongles = malloc(n * sizeof(t_dongle));
	if (!sim->dongles)
		return (fprintf(stderr, "Error: malloc dongles\n"), false);
	i = 0;
	while (i < n)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].available = 1;
		sim->dongles[i].available_at = 0;
		sim->dongles[i].next_seq = 1;
		sim->dongles[i].sim = sim;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (false);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
			return (false);
		if (!pq_init(&sim->dongles[i].queue, n * 8))
			return (false);
		i++;
	}
	return (true);
}

static bool	init_coders(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->number_of_coders;
	sim->coders = malloc(n * sizeof(t_coder));
	if (!sim->coders)
		return (fprintf(stderr, "Error: malloc coders\n"), false);
	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_start = sim->start_time;
		sim->coders[i].gen_left = 0;
		sim->coders[i].gen_right = 0;
		sim->coders[i].sim = sim;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % n];
		i++;
	}
	return (true);
}

bool	init_sim(t_sim *sim)
{
	sim->dongles = NULL;
	sim->coders = NULL;
	sim->sim_stopped = 0;
	sim->start_time = get_time_ms();
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (false);
	if (!init_dongles(sim))
		return (false);
	if (!init_coders(sim))
		return (false);
	return (true);
}

void	cleanup(t_sim *sim)
{
	int	i;

	if (sim->coders)
		free(sim->coders);
	if (sim->dongles)
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			pthread_cond_destroy(&sim->dongles[i].cond);
			pq_free(&sim->dongles[i].queue);
			i++;
		}
		free(sim->dongles);
	}
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
}