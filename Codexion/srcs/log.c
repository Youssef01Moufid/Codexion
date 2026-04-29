#include "../includes/codexion.h"

void    log_state(t_sim *sim, int coder_id, char *mssg)
{
    long    time;

    time = get_time_ms() - sim->start_time;
    pthread_mutex_lock(&sim->print_mutex);
    printf("%ld %d %s\n", time, coder_id, mssg);
    pthread_mutex_unlock(&sim->print_mutex);
}