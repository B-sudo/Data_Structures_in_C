#include <stdio.h>
#include <stdlib.h>

typedef struct node *node_ptr;
struct node
{
    int element;
    node_ptr next;
};
typedef node_ptr LIST;
typedef node_ptr position;

void create_list(LIST L)
{
    position p = L;
    int num;
    printf("\nplease input a natural number with 0 to quit");
    scanf("%d", &num);
    while (num != 0)
    {
        p->next = (position)malloc(sizeof(struct node));
        p->next->element = num;
        p->next->next = NULL;
        p = p->next;
        printf("\nplease input a natural number and use 0 to quit");
        scanf("%d", &num);
    }
}

void print_list(LIST L)
{
    if (L->next == NULL)
        printf("\nno node in this list");
    position p = L->next;
    while (p != NULL)
    {
        printf("%d\t", p->element);
        p = p->next;
    }
}

void swap_list(LIST L, int a)
{
    position p = L, q = NULL;
    int i;
    if (a <= 0 || L->next == NULL || L->next->next == NULL)
    {
        printf("\nwrong");
        return;
    }
    for (i = 0; i < a - 1; i++)
    {
        p = p->next;
        if (p->next->next == NULL)
        {
            printf("\noverflow");
            return;
        }
    }
    q = p->next;
    p->next = p->next->next;
    q->next = p->next->next;
    p->next->next = q;
}
int main()
{
    int a;
    LIST L = (position)malloc(sizeof(struct node));
    L->element = 0;
    L->next = NULL;
    create_list(L);
    print_list(L);
    printf("\nplease input a number to determine the position you want to swap,-1 to exit");
    scanf("%d", &a);
    while (a != -1)
    {
        swap_list(L, a);
        print_list(L);
        printf("\nplease input a number to determine the position you want to swap,-1 to exit");
        scanf("%d", &a);
    }
}