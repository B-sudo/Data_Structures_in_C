#include <stdio.h>
#include <stdlib.h>

typedef struct tree_node *tree_ptr;

struct tree_node
{
    int elem;
    tree_ptr left;
    tree_ptr right;
};

typedef tree_ptr TREE;

struct queue_record
{
    unsigned int queue_max_size;
    int queue_front;
    int queue_rear;
    unsigned int queue_size;
    tree_ptr * queue_array;
};

typedef struct queue_record *QUEUE;

QUEUE Q;
char input[100000];
int count = 0;
int depth[10010] = {0};

int max(int a, int b)
{
    return a > b ? a : b;
}

int is_empty(QUEUE Q)
{
    if (Q == NULL)
    {
        return 0;
    }
    return Q->queue_size == 0;
}

QUEUE CreateQueue(unsigned int max_size)
{
    QUEUE Q;
    Q = (QUEUE)malloc(sizeof(struct queue_record));
    Q->queue_max_size = max_size;
    Q->queue_front = Q->queue_rear = -1;
    Q->queue_size = 0;
    Q->queue_array = (tree_ptr *)malloc(sizeof(tree_ptr) * max_size);
    return Q;
}

int succ(int num, QUEUE Q)
{
    return num % Q->queue_max_size;
}

void Enqueue(tree_ptr T, QUEUE Q)
{
    if (Q == NULL)
    {
        return;
    }
    else if (T == NULL)
        return ;
    else if (is_empty(Q))
    {
        Q->queue_front = Q->queue_rear = 0;
        Q->queue_array[Q->queue_front] = T;
        Q->queue_size++;
    }
    else
    {
        Q->queue_rear = succ(++Q->queue_rear, Q);
        Q->queue_array[Q->queue_rear] = T;
        Q->queue_size++;
    }
}

tree_ptr Dequeue(QUEUE Q)
{
    tree_ptr hook;
    if (Q == NULL)
    {
        return NULL;
    }
    if (is_empty(Q))
    {
        return NULL;
    }
    else
    {
        hook = Q->queue_array[Q->queue_front];
        Q->queue_front = succ(++Q->queue_front, Q);
        Q->queue_size--;
        return hook;
    }
}


int char2int(char c)
{
    if (c > 57 || c < 48)
        return -1;
    else
        return (int)(c - 48); 
}

TREE MakeNode()
{
    TREE T;
    T = (TREE)malloc(sizeof(struct tree_node));
    T->elem = 0;
    T->left = T->right = NULL;
    return T;
}

TREE Create()
{
    TREE T;
    int num;
    char c;
    c = *(input + ++count);
    while (c != 0)
    {
        if (c == '{')
        {
            T = MakeNode();
            T->left = Create();
        } 
        else if (c == ',') ;
        else if (c > 47 && c < 58)
        {
            num = char2int(c);
            c = *(input + ++count);
            while (c != ',')
            {    
                num = num * 10 + char2int(c);
                c = *(input + ++count);
            }
            T->elem = num;
            count++;
            T->right = Create();
            count++;
            return T;
        }
        else if (c == '}')
            return NULL;
        c = *(input + ++count);
    }
    return NULL;
}

void PreOrder_Traverse(TREE T)
{
    if (T == NULL)
        return ;
    else
    {
        if (count == 0)
            printf("%d", T->elem);
        else 
            printf(",%d", T->elem);
        count++;
        PreOrder_Traverse(T->left);
        PreOrder_Traverse(T->right);
    }
}

void Traverse(TREE T)
{
    if (T == NULL)
        return ;
    else
    {
        if (count == 0)
            printf("%d", T->elem);
        else 
            printf(",%d", T->elem);
        count++;
        Enqueue(T->left, Q);
        Enqueue(T->right, Q);
    }
}

int Height(TREE T, int level)
{
    int height;
    if (T == NULL)
        return 0;
    else
    {
        height = max(Height(T->left, level + 1), Height(T->right, level + 1)) + 1;
        depth[level]++;
        return height;
    }
}

int main()
{
    int height = 0, i, M_depth = 0, hook = 0;
    TREE T;
    scanf("%s", input);
    T = Create();
    count = 0;
    PreOrder_Traverse(T);
    printf("\n");
    count = 0;
    Q = CreateQueue(20000);
    Traverse(T);
    while (!is_empty(Q))
        Traverse(Dequeue(Q));
    printf("\n");
    height = Height(T, 1);
    for (i = height; i > 0; i--)
    {
        if (depth[i] > M_depth)
        {
            M_depth = depth[i];
            hook = i;
        }
    }
    printf("%d\n%d\n%d", hook, M_depth, height);
}

