/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoufid <ymoufid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 03:00:39 by ymoufid           #+#    #+#             */
/*   Updated: 2026/05/15 21:24:49 by ymoufid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include <time.h>

static long	deadline_of(t_coder *c)
{
	return (c->last_compile_start + c->sim->time_to_burnout);
}

static int	*gen_ptr(t_coder *c, int is_left)
{
	if (is_left)
		return (&c->gen_left);
	return (&c->gen_right);
}

static long	compute_key(t_dongle *d, t_coder *c, long seq)
{
	if (d->sim->scheduler == FIFO)
		return (seq);
	return (deadline_of(c));
}

static void	abs_timespec_from_now_ms(long delta_ms, struct timespec *ts)
{
	struct timeval	tv;
	long			abs_ms;

	gettimeofday(&tv, NULL);
	abs_ms = (tv.tv_sec * 1000L) + (tv.tv_usec / 1000L) + delta_ms;
	ts->tv_sec = abs_ms / 1000L;
	ts->tv_nsec = (abs_ms % 1000L) * 1000000L;
}

int	is_stopped(t_sim *sim)
{
	int	s;

	pthread_mutex_lock(&sim->stop_mutex);
	s = sim->sim_stopped;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (s);
}

void	stop_sim(t_sim *sim)
{
	int	j;

	pthread_mutex_lock(&sim->stop_mutex);
	if (sim->sim_stopped)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		return ;
	}
	sim->sim_stopped = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	j = 0;
	while (j < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[j].mutex);
		pthread_cond_broadcast(&sim->dongles[j].cond);
		pthread_mutex_unlock(&sim->dongles[j].mutex);
		j++;
	}
}

static int	can_take_now(t_dongle *d, t_coder *c)
{
	long	now;

	pq_drop_invalid_top(&d->queue, d);
	now = get_time_ms();
	if (!d->available)
		return (0);
	if (now < d->available_at)
		return (0);
	if (d->queue.size <= 0)
		return (0);
	if (pq_peek(&d->queue).coder_id != c->id)
		return (0);
	return (1);
}

int	dongle_request_until(t_dongle *d, t_coder *c, int is_left, long wait_ms)
{
	t_waiter		w;
	int				*gen;
	struct timespec	ts;

	gen = gen_ptr(c, is_left);
	pthread_mutex_lock(&d->mutex);
	(*gen)++;
	w.coder_id = c->id;
	w.gen = *gen;
	w.seq = d->next_seq++;
	w.key = compute_key(d, c, w.seq);
	pq_push(&d->queue, w);
	pthread_cond_broadcast(&d->cond);
	abs_timespec_from_now_ms(wait_ms, &ts);
	while (!can_take_now(d, c))
	{
		if (is_stopped(c->sim))
		{
			(*gen)++;
			pthread_cond_broadcast(&d->cond);
			pthread_mutex_unlock(&d->mutex);
			return (0);
		}
		if (pthread_cond_timedwait(&d->cond, &d->mutex, &ts) != 0)
		{
			(*gen)++;
			pthread_cond_broadcast(&d->cond);
			pthread_mutex_unlock(&d->mutex);
			return (0);
		}
	}
	d->available = 0;
	(void)pq_pop(&d->queue);
	pthread_mutex_unlock(&d->mutex);
	return (1);
}

void	dongle_request(t_dongle *d, t_coder *c, int is_left)
{
	while (!is_stopped(c->sim))
	{
		if (dongle_request_until(d, c, is_left, 50))
			return ;
	}
}

void	dongle_release(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle->available_at = get_time_ms() + dongle->sim->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}