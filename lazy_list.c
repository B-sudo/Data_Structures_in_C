#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ORGNUM 32

typedef struct node *node_ptr;

struct node
{
    int elem;
    int bool;
    node_ptr next;
};

typedef node_ptr LIST;
typedef node_ptr position;

LIST CreateList()
{
    LIST LazyList = NULL;
    position hook = LazyList;
    int count;
    LazyList = (LIST)malloc(sizeof(struct node));
    LazyList->elem = 0;
    LazyList->bool = 0;
    LazyList->next = NULL;
    LazyList->next = (node_ptr)malloc(sizeof(struct node));
    for (count = 0; count < ORGNUM; count++)
    {
        hook->next = (node_ptr)malloc(sizeof(struct node));
        hook = hook->next;
        hook->elem = rand() % 20;
        hook->bool = 1;
        hook->next = NULL;
    }
    return LazyList;
}

unsigned int is_empty(LIST List)
{
    if (List == NULL)
    {
        printf("\nInvalid LIST");
        return 1;
    }
    else if (List->next != NULL)
    {
        printf("\nNot empty");
        return 0;
    }
    else
    {
        printf("\nEmpty list");
        return 1;
    }
}
unsigned int is_single(LIST List)
{
    if (List == NULL)
    {
        printf("\nInvalid LIST");
        return 0;
    }
    else if (List->next == NULL)
    {
        printf("\nEmpty list");
        return 0;
    }
    else if (List->next->next != NULL)
    {
        printf("\nNot single node one");
        return 0;
    }
    else
    {
        printf("\nSingle node");
        return 0;
    }
}

unsigned int is_equal(LIST List)
{
    int plus = 0, minus = 0;
    position hook = List;
    if (is_empty(List) == 1)
    {
        printf("\nError");
        return 0;
    }
    while (hook->next != NULL)
    {
        hook = hook->next;
        if (hook->bool == 0)
            minus++;
        else
            plus++;
    }
    if (plus == minus)
        return 1;
    else
        return 0;
}

void ShrinkList(LIST LazyList)
{
    position hook = LazyList, trash = NULL;
    if (is_empty(LazyList) == 1)
    {
        printf("\nError");
        exit;
    }
    while (hook->next != NULL)
    {
        if (hook->next->bool == 1)
        {
            hook = hook->next;
        }
        else
        {
            trash = hook->next;
            hook->next = trash->next;
            free(trash);
        }
    }
    printf("\nSrunk");
}

void DeleteList(LIST LazyList, int x)
{
    position hook = LazyList;
    if (is_empty(LazyList) == 1)
    {
        printf("\nError");
        exit;
    }
    while ((hook->bool == 0 || hook->elem != x) && hook->next != NULL)
    {
        hook = hook->next;
    }
    if (hook->bool == 0 || hook->elem != x)
    {
        printf("\nDoes not exist");
        exit;
    }
    else
    {
        hook->bool = 0;
        printf("\nDeleted successfully");
    }
    if (is_equal(LazyList) == 1)
        ShrinkList(LazyList);
}

void PrintList(LIST LazyList)
{
    position hook = LazyList;
    if (is_empty(LazyList) == 1)
    {
        printf("\nEmpty List");
        exit;
    }
    while (hook->next != NULL)
    {
        hook = hook->next;
        printf("\t%d", hook->elem);
    }
    printf("\nPrinted successfully");
}

int main()
{
    LIST LazyList; 
    LazyList = CreateList();
    printf("\nCreated");
    PrintList(LazyList);
}