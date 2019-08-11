#include <stdio.h>
#include <stdlib.h>

struct queue_record
{
    unsigned int queue_max_size;
    int queue_front;
    int queue_rear;
    unsigned int queue_size;
    int *queue_array;
};

typedef struct queue_record *QUEUE;

QUEUE CreateQueue(unsigned int max_size)
{
    QUEUE Q;
    Q = (QUEUE)malloc(sizeof(struct queue_record));
    Q->queue_max_size = max_size;
    Q->queue_front = Q->queue_rear = -1;
    Q->queue_size = 0;
    Q->queue_array = (int *)malloc(sizeof(int) * max_size);
    return Q;
}

int succ(int num, QUEUE Q)
{
    return num % Q->queue_max_size;
}

int is_empty(QUEUE Q)
{
    if (Q == NULL)
    {
        printf("\nError");
        return 0;
    }
    return Q->queue_size == 0;
}

int is_full(QUEUE Q)
{
    if (Q == NULL)
    {
        printf("\nError");
        return 0;
    }
    return Q->queue_size == Q->queue_max_size;
}

void Enqueue(int x, QUEUE Q)
{
    if (Q == NULL)
    {
        printf("\nError");
        return;
    }
    if (is_full(Q))
    {
        printf("\nFull queue");
        return;
    }
    else if (is_empty(Q))
    {
        Q->queue_front = Q->queue_rear = 0;
        Q->queue_array[Q->queue_front] = x;
        Q->queue_size++;
    }
    else
    {
        Q->queue_rear = succ(++Q->queue_rear, Q);
        Q->queue_array[Q->queue_rear] = x;
        Q->queue_size++;
    }
}

int Dequeue(QUEUE Q)
{
    if (Q == NULL)
    {
        printf("\nError");
        return 0;
    }
    if (is_empty(Q))
    {
        printf("\nEmpty queue");
        return 0;
    }
    else
    {
        Q->queue_front = succ(++Q->queue_front, Q);
        Q->queue_size--;
    }
}

void PrintQueue(QUEUE Q)
{
    int i;
    if (Q == NULL)
    {
        printf("\nError");
        return;
    }
    if (is_empty(Q))
    {
        printf("\nEmpty queue");
        return;
    }
    else 
    {
        i = Q->queue_front;
        printf("\n%d",Q->queue_array[i]);
        while (i != Q->queue_rear)
        {
            i = succ(++i, Q);
            printf("\t%d",Q->queue_array[i]);
        }
    }
}

void main()
{
    QUEUE Q;
    Q = CreateQueue(30);
    Enqueue(1,Q);
    Enqueue(3,Q);
    PrintQueue(Q);
    Enqueue(5,Q);
    Dequeue(Q);
    PrintQueue(Q);
    Dequeue(Q);
    Dequeue(Q);
    PrintQueue(Q);
}