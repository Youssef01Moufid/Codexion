#include "../includes/codexion.h"

bool    pq_init(t_pqueue *pq, int capacity)
{
    pq->data = malloc(capacity * sizeof(t_waiter));
    if (!pq->data)
    {
        fprintf(stderr, "Error in allocation data of queue");
        return (false);
    }
    pq->capacity = capacity;
    pq->size = 0;
    return (true);
}
void    pq_push(t_pqueue *pq, int coder_id, long priority)
{
    int i;
    int parent;
    t_waiter    tmp;

    pq->data[pq->size].coder_id = coder_id;
    pq->data[pq->size].priority = priority;
    pq->size += 1;
    
    i = pq->size - 1;
    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (pq->data[i].priority < pq->data[parent].priority)
        {
            tmp = pq->data[i];
            pq->data[i] = pq->data[parent];
            pq->data[parent] = tmp;
            i = parent;
        }
        else
            return;
    }
}
t_waiter    pq_pop(t_pqueue *pq)
{
    t_waiter    result;
    t_waiter    tmp;
    int         i;
    int         smallest;
    int         left;
    int         right;

    result = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size -= 1;
    
    i = 0;
    while (1)
    {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;
        
        if (left < pq->size && pq->data[left].priority < pq->data[smallest].priority)
            smallest = left;
        if (right < pq->size && pq->data[right].priority < pq->data[smallest].priority)
            smallest = right;
        if (smallest == i)
            break;
        tmp = pq->data[i];
        pq->data[i] = pq->data[smallest];
        pq->data[smallest] = tmp;
        i = smallest;
    }
    return (result);
}
void    pq_free(t_pqueue *pq)
{
    free(pq->data);
    pq->data = NULL;
    pq->size = 0;
}
