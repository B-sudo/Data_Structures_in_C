#include <stdio.h>
#include <stdlib.h>

typedef struct node *node_ptr;

struct node
{
    int elem;
    node_ptr next;
};

struct queue_record
{
    unsigned int queue_size;
    node_ptr queue_front;
    node_ptr queue_rear;
};

typedef struct queue_record *QUEUE;

QUEUE CreateQueue()
{
    QUEUE Q;
    Q = (QUEUE)malloc(sizeof(struct queue_record));
    Q->queue_front = Q->queue_rear = NULL;
    Q->queue_size = 0;
    return Q;
}

int is_empty(QUEUE Q)
{
    if (Q == NULL)
    {
        printf("\nError");
        return 0;
    }
    else
        return !Q->queue_size;
}

void Enqueue(int x, QUEUE Q)
{
    if (is_empty(Q))
    {
        Q->queue_front = (node_ptr)malloc(sizeof(struct node));
        Q->queue_rear = Q->queue_front;
        Q->queue_front->elem = x;
        Q->queue_front->next = NULL;
        Q->queue_size++;
    }
    else
    {
        Q->queue_rear = Q->queue_rear->next = (node_ptr)malloc(sizeof(struct node));
        Q->queue_rear->elem = x;
        Q->queue_rear->next = NULL;
        Q->queue_size++;
    }
}

int is_single(QUEUE Q)
{
    if (Q == NULL)
    {
        printf("\nError");
        return 0;
    }
    return (Q->queue_front == Q->queue_rear) && (Q->queue_front != NULL);
}

int Dequeue(QUEUE Q)
{
    int num;
    node_ptr hook;
    if (is_empty(Q))
    {
        printf("\nEmpty queue");
        return 0;
    }
    else if (is_single(Q))
    {
        num = Q->queue_front->elem;
        free(Q->queue_front);
        Q->queue_front = Q->queue_rear = NULL;
        Q->queue_size--;
    }
    else
    {
        hook = Q->queue_front;
        Q->queue_front = Q->queue_front->next;
        Q->queue_size--;
        num = hook->elem;
        free(hook);
        return num;
    }
}

void PrintQueue(QUEUE Q)
{
    node_ptr hook;
    if (Q == NULL)
    {
        printf("\nError");
        return;
    }
    else if (is_empty(Q))
    {
        printf("\nEmpty queue");
        return;
    }
    else
    {
        hook = Q->queue_front;
        printf("\n%d",hook->elem);
        while(hook != Q->queue_rear)
        {
            hook = hook->next;
            printf("\t%d",hook->elem);
        }
    }
}

void main()
{
    QUEUE Q;
    Q = CreateQueue();
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
