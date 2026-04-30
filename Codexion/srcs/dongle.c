#include "../includes/codexion.h"

void    dongle_request(t_dongle *dongle, t_coder *coder)
{
    long priority;

    pthread_mutex_lock(&dongle->mutex);
    if (coder->sim->scheduler == FIFO)
        priority = get_time_ms();
    if (coder->sim->scheduler == EDF)
        priority = coder->last_compile_start + coder->sim->time_to_burnout;
    pq_push(&dongle->queue, coder->id, priority);
    while (dongle->available == 0 || get_time_ms() < dongle->available_at || pq_peek(&dongle->queue).coder_id != coder->id)
    {
        if (dongle->sim->sim_stopped)
        {
            pthread_mutex_unlock(&dongle->mutex);
            return ;
        }
        pthread_cond_wait(&dongle->cond, &dongle->mutex);
    }
    dongle->available = 0;
    pq_pop(&dongle->queue);
    pthread_mutex_unlock(&dongle->mutex);
    log_state(dongle->sim, coder->id, "has taken a dongle");
}
void    dongle_release(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->available = 1;
    dongle->available_at = get_time_ms() + dongle->sim->dongle_cooldown;
    pthread_cond_broadcast(&dongle->cond);
    pthread_mutex_unlock(&dongle->mutex);
}