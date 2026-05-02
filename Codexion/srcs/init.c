#include "../includes/codexion.h"

bool    init_dongles(t_sim *sim)
{
    int i;
    int N;

    sim->dongles = malloc(sim->number_of_coders * sizeof(t_dongle));
    if (!sim->dongles)
    {
        fprintf(stderr, "Error in allocation array of dongles !\n");
        return (false);
    }
    i = 0;
    N = sim->number_of_coders;
    while (i < N )
    {
        sim->dongles[i].id = i;
        sim->dongles[i].available = 1;
        sim->dongles[i].available_at = 0;
        sim->dongles[i].sim = sim;
        if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
        {
            free(sim->dongles);
            return (false);
        }
        if (pthread_cond_init(&sim->dongles[i].cond, NULL))
        {
            free(sim->dongles);
            return (false);
        }
        if (!pq_init(&sim->dongles[i].queue, N))
        {
            free(sim->dongles);
            return (false);
        }
        i++;
    }
    return (true);
}
bool    init_coders(t_sim *sim)
{
    int i;
    int N;

    sim->coders = malloc (sim->number_of_coders * sizeof(t_coder));
    if (!sim->coders)
    {
        fprintf(stderr, "Error in allocation array of coders !\n");
        return (false);
    }
    i = 0;
    N = sim->number_of_coders;
    while (i < N)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].compile_count = 0;
        sim->coders[i].last_compile_start = get_time_ms();
        sim->coders[i].sim = sim;
        sim->coders[i].left_dongle = &sim->dongles[i];
        sim->coders[i].right_dongle = &sim->dongles[(i + 1) % N];
        i++;
    }
    return (true);
}

bool    init_sim(t_sim *sim)
{
    sim->dongles = NULL;
    sim->coders = NULL;
    if (pthread_mutex_init(&sim->print_mutex, NULL))
        return (false);
    if (pthread_mutex_init(&sim->stop_mutex, NULL))
        return (false);
    if (!init_dongles(sim))
        return (false);
    if (!init_coders(sim))
        return (false);
    sim->sim_stopped = 0;
    sim->start_time = get_time_ms();
    return (true);
}


