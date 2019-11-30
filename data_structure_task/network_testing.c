#include<stdio.h>
#include<stdlib.h>

typedef struct node_info
{
    char active;
    int dist;
}node_info;

typedef struct arc *arc_ptr;
typedef struct arc
{
    int weight;
    int node;
    arc_ptr next;
}arc;

typedef struct Network
{
    int node_num;
    int arc_num;
    node_info node[9971];
    arc_ptr weight[9971];
} Network;

Network G = 
{
    0,
    0,
    {{0, 0}},
};
//FILE *fin = NULL, *fout = NULL;
int char2int()
{
    int num = 0;
    char c;
    c = getchar();
    while (c < 58 && c > 47)
    {
        num = num * 10 + c - 48;
        c = getchar();
    }
    return num;
}

void Init()
{
    int count, start, weight, end;
    arc_ptr hook;
    G.node_num = char2int();
    G.arc_num = char2int();
    for (count = 0; count < G.node_num; count++)
        G.weight[count] = NULL;
    for (count = 0; count < G.arc_num; count++)
    {
        start = char2int();
        end = char2int();
        weight = char2int();
        hook = (arc_ptr)malloc(sizeof(struct arc));
        hook->weight = weight;
        hook->node = end;
        hook->next = G.weight[start];
        G.weight[start] = hook;


        hook = (arc_ptr)malloc(sizeof(struct arc));
        hook->weight = weight;
        hook->node = start;
        hook->next = G.weight[end];
        G.weight[end] = hook;
    }
}

int getMin()
{
    int count, Min=-1, num=-1;
    for (count = 0; count < G.node_num; count++)
    {
        if (Min == -1 && G.node[count].active == 0)
        {
            num = count;
            Min = G.node[count].dist;
        }
        else if (G.node[count].dist != -1 && G.node[count].dist < Min && G.node[count].active == 0)
        {
            num = count;
            Min = G.node[count].dist;
        }
    }
    return num;
}

void Process(int order)
{
    arc_ptr hook;
    G.node[order].active = 1;
    hook = G.weight[order];
    while (hook != NULL)
    {
        if (G.node[hook->node].active == 0)
        {
            if (hook->weight + G.node[order].dist < G.node[hook->node].dist || G.node[hook->node].dist == -1)
                G.node[hook->node].dist = hook->weight + G.node[order].dist;
        }
        hook = hook->next;
    }
}

int main()
{
    int count, order, num=0, MAX=0;
    arc_ptr hook;
    //fin = fopen("polyn.in", "r");
    //fout = fopen("polyn.out", "w"); //TOBE changed
    Init();
    for (count = 0; count < G.node_num; count++)
        G.node[count].dist = -1;
    G.node[0].active = 1;
    hook = G.weight[0];
    while (hook != NULL)
    {
        G.node[hook->node].dist = hook->weight;
        hook = hook->next;
    }
    G.node[0].dist = 0;
    for (count = 0; count < G.node_num - 1; count++)
    {
        order = getMin();
        Process(order);
    }
    for (count = 0; count < G.node_num; count++)
    {
        if (G.node[count].dist > MAX)
        {
            num = count;
            MAX = G.node[count].dist;
        }
    }
    printf("%d\n%d", num, MAX);
    //fclose(fin);
    //fclose(fout);
}