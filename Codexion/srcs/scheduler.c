/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoufid <ymoufid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 00:29:07 by ymoufid           #+#    #+#             */
/*   Updated: 2026/05/08 23:06:40 by ymoufid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static int	less(t_waiter a, t_waiter b)
{
	if (a.key != b.key)
		return (a.key < b.key);
	if (a.seq != b.seq)
		return (a.seq < b.seq);
	return (a.coder_id < b.coder_id);
}

static void	swap(t_waiter *a, t_waiter *b)
{
	t_waiter	t;

	t = *a;
	*a = *b;
	*b = t;
}

bool	pq_init(t_pqueue *pq, int capacity)
{
	pq->data = malloc(capacity * sizeof(t_waiter));
	if (!pq->data)
		return (fprintf(stderr, "Error: malloc pqueue\n"), false);
	pq->size = 0;
	pq->capacity = capacity;
	return (true);
}

void	pq_push(t_pqueue *pq, t_waiter w)
{
	int	i;
	int	parent;

	if (pq->size >= pq->capacity)
		return ;
	pq->data[pq->size] = w;
	i = pq->size;
	pq->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (less(pq->data[i], pq->data[parent]))
		{
			swap(&pq->data[i], &pq->data[parent]);
			i = parent;
		}
		else
			break ;
	}
}

t_waiter	pq_pop(t_pqueue *pq)
{
	t_waiter	res;
	int			(i), (l), (r), (s);

	res = pq->data[0];
	pq->data[0] = pq->data[pq->size - 1];
	pq->size--;
	i = 0;
	while (1)
	{
		l = 2 * i + 1;
		r = 2 * i + 2;
		s = i;

		if (l < pq->size && less(pq->data[l], pq->data[s]))
			s = l;
		if (r < pq->size && less(pq->data[r], pq->data[s]))
			s = r;
		if (s == i)
			break ;
		swap(&pq->data[i], &pq->data[s]);
		i = s;
	}
	return (res);
}

t_waiter	pq_peek(t_pqueue *pq)
{
	return (pq->data[0]);
}

void	pq_free(t_pqueue *pq)
{
	free(pq->data);
	pq->data = NULL;
	pq->size = 0;
	pq->capacity = 0;
}

void	pq_drop_invalid_top(t_pqueue *pq, t_dongle *d)
{
	t_waiter	top;
	t_coder		*c;
	int			current_gen;

	while (pq->size > 0)
	{
		top = pq->data[0];
		c = &d->sim->coders[top.coder_id - 1];
		if (d == c->left_dongle)
			current_gen = c->gen_left;
		else
			current_gen = c->gen_right;
		if (top.gen == current_gen)
			return ;
		(void)pq_pop(pq);
	}
}
