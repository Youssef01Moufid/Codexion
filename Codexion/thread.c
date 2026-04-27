#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

// int main(int argc, char **argv)
// {
//     int pid = fork();
//     if (pid == -1)
//     {
//         return 1;
//     }
//     printf("Hello from processes\n");
//     if (pid != 0)
//     {
//         wait(NULL);
//     }
//     return 0;
// }

int x = 0;
pthread_mutex_t mutex;
void* routine2()
{
    int i = 0;
    while(x < 10000)
    {
        pthread_mutex_lock(&mutex);
        x++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&t1, NULL, &routine2, NULL);
    pthread_create(&t2, NULL, &routine2, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    printf(" x is : %d\n", x);
    return 0;
}