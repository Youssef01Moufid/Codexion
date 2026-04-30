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
}
void    dongle_release(t_dongle *dongle)
{

}