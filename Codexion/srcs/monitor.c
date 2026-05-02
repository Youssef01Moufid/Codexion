#include "../includes/codexion.h"

void    *monitor_routine(void *arg)
{
    t_sim   *sim;
    int     i;
    int     j;
    int     count;

    sim = (t_sim *)arg;
    while (1)
    {
        usleep(1000);
        i = 0;
        count = 0;
        while (i < sim->number_of_coders)
        {
            // printf("DEBUG: coder %d | last_compile_start=%ld | now=%ld | diff=%ld | burnout=%ld\n",
            //             sim->coders[i].id,
            //             sim->coders[i].last_compile_start,
            //             get_time_ms(),
            //             get_time_ms() - sim->coders[i].last_compile_start,
            //             sim->time_to_burnout);
            if (get_time_ms() - sim->coders[i].last_compile_start > sim->time_to_burnout)
            {
                log_state(sim, sim->coders[i].id, "burned out");
                pthread_mutex_lock(&sim->stop_mutex);
                sim->sim_stopped = 1;
                pthread_mutex_unlock(&sim->stop_mutex);
                j = 0;
                while  (j < sim->number_of_coders)
                {
                    pthread_mutex_lock(&sim->dongles[j].mutex);
                    pthread_cond_broadcast(&sim->dongles[j].cond);
                    pthread_mutex_unlock(&sim->dongles[j].mutex);
                    j++;
                }
                return (NULL);
            }
            if (sim->coders[i].compile_count >= sim->number_of_compiles_required)
                count++;        
            i++;
        }
        if (count == sim->number_of_coders)
        {
            pthread_mutex_lock(&sim->stop_mutex);
            sim->sim_stopped = 1;
            pthread_mutex_unlock(&sim->stop_mutex);
            j = 0;
            while  (j < sim->number_of_coders)
            {
                pthread_mutex_lock(&sim->dongles[j].mutex);
                pthread_cond_broadcast(&sim->dongles[j].cond);
                pthread_mutex_unlock(&sim->dongles[j].mutex);
                j++;
            }
            return (NULL);
        }
    }
}
