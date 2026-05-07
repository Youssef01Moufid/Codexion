#include "../includes/codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_sleep(long ms)
{
	long	start;
	long	now;
	long	remaining;

	if (ms <= 0)
		return ;
	start = get_time_ms();
	while (1)
	{
		now = get_time_ms();
		remaining = ms - (now - start);
		if (remaining <= 0)
			break ;
		if (remaining > 2)
			usleep((remaining - 1) * 1000);
		else
			usleep(200);
	}
}