#include "../includes/codexion.h"

long    get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
void    cleanup(t_sim *sim)
{
    int i;
    int N;

    pthread_mutex_destroy(&sim->print_mutex);
    pthread_mutex_destroy(&sim->stop_mutex);
    if (sim->coders)
        free(sim->coders);
    i = 0;
    N = sim->number_of_coders;
    if (sim->dongles)
    {
        while (i < N)
        {
            pthread_mutex_destroy(&sim->dongles[i].mutex);
            pthread_cond_destroy(&sim->dongles[i].cond);
            pq_free(&sim->dongles[i].queue);
            i++;
        }
        free(sim->dongles);
    }

}