#include "../includes/codexion.h"

int is_stopped(t_sim *sim)
{
    int check;
    pthread_mutex_lock(&sim->stop_mutex);
    check = sim->sim_stopped;
    pthread_mutex_unlock(&sim->stop_mutex);
    return (check);
}
void    *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while (1)
    {
        if (coder->id % 2)
        {
            dongle_request(coder->left_dongle, coder);
            if (is_stopped(coder->sim))
                return (NULL);
            dongle_request(coder->right_dongle, coder);
            if (is_stopped(coder->sim))
                return (NULL);
        }
        else
        {
        dongle_request(coder->right_dongle, coder);
            if (is_stopped(coder->sim))
                return (NULL);
        dongle_request(coder->left_dongle, coder);
            if (is_stopped(coder->sim))
                return (NULL);
        }
        pthread_mutex_lock(&coder->sim->stop_mutex);
        coder->last_compile_start = get_time_ms();
        pthread_mutex_unlock(&coder->sim->stop_mutex);
        log_state(coder->sim, coder->id, "is compiling");
        usleep(coder->sim->time_to_compile * 1000);
        pthread_mutex_lock(&coder->sim->stop_mutex);
        coder->compile_count++;
        pthread_mutex_unlock(&coder->sim->stop_mutex);
        dongle_release(coder->left_dongle);
        dongle_release(coder->right_dongle);
        if (is_stopped(coder->sim))
            return (NULL);
        log_state(coder->sim, coder->id, "is debugging");
        usleep(coder->sim->time_to_debug * 1000);
        if (is_stopped(coder->sim))
            return (NULL);
        log_state(coder->sim, coder->id, "is refactoring");
        usleep(coder->sim->time_to_refactor * 1000);
    }
}