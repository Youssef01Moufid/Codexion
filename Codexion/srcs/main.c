#include "../includes/codexion.h"

int main(int argc, char **argv)
{
    t_sim   sim;
    int         i;

    i = 0;
    if(!parsing(argc, argv, &sim))
        return (1);
    if (!init_sim(&sim))
        return (1);
    pthread_create(&sim.monitor, NULL, &monitor_routine, &sim);
    while (i < sim.number_of_coders)
    {
        pthread_create(&sim.coders[i].thread, NULL, coder_routine, &sim.coders[i]);
        i++;
    }
    pthread_join(sim.monitor, NULL);
    i = 0;
    while (i < sim.number_of_coders)
    {
        pthread_join(sim.coders[i].thread, NULL);
        i++;
    }
    cleanup(&sim);
    return (0);
}