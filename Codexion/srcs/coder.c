#include "../includes/codexion.h"

static int	acquire_two(t_coder *c)
{
	t_dongle	*a;
	t_dongle	*b;
	int			a_is_left;
	int			b_is_left;

	a = c->left_dongle;
	b = c->right_dongle;
	a_is_left = 1;
	b_is_left = 0;
	if (a->id > b->id)
	{
		t_dongle	*tmp = a;
		int			t = a_is_left;

		a = b;
		b = tmp;
		a_is_left = b_is_left;
		b_is_left = t;
	}
	while (!is_stopped(c->sim))
	{
		dongle_request(a, c, a_is_left);
		if (is_stopped(c->sim))
			return (0);
		if (dongle_request_until(b, c, b_is_left, 20))
			return (1);
		dongle_release(a);
		usleep(200);
	}
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*c;

	c = (t_coder *)arg;
	if (c->sim->number_of_coders == 1)
	{
		while (!is_stopped(c->sim))
			usleep(1000);
		return (NULL);
	}
	while (!is_stopped(c->sim))
	{
		if (!acquire_two(c))
			return (NULL);

		pthread_mutex_lock(&c->sim->stop_mutex);
		c->last_compile_start = get_time_ms();
		pthread_mutex_unlock(&c->sim->stop_mutex);

		log_state(c->sim, c->id, "has taken a dongle");
		log_state(c->sim, c->id, "has taken a dongle");
		log_state(c->sim, c->id, "is compiling");
		precise_sleep(c->sim->time_to_compile);

		pthread_mutex_lock(&c->sim->stop_mutex);
		c->compile_count++;
		pthread_mutex_unlock(&c->sim->stop_mutex);

		dongle_release(c->left_dongle);
		dongle_release(c->right_dongle);

		log_state(c->sim, c->id, "is debugging");
		precise_sleep(c->sim->time_to_debug);

		log_state(c->sim, c->id, "is refactoring");
		precise_sleep(c->sim->time_to_refactor);
	}
	return (NULL);
}