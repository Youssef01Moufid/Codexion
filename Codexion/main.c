#include "includes/codexion.h"

int main(int argc, char **argv)
{
    t_sim sim;

    if (!parsing(argc, argv, &sim))
        return 1;

    printf("number of coders : %d\n", sim.number_of_coders);
    printf("time to burnout : %d\n", sim.time_to_burnout);
    printf("time to compile : %d\n", sim.time_to_compile);
    printf("time to debug : %d\n", sim.time_to_debug);
    printf("time to refactor : %d\n", sim.time_to_refactor);
    printf("number of compiles required : %d\n", sim.number_of_compiles_required);
    printf("dongle cooldown : %d\n", sim.dongle_cooldown);
    printf("scheduler : %s\n", sim.scheduler == FIFO ? "fifo" : "edf");
}