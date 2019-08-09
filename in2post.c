#include <stdio.h>
#include <stdlib.h>
#define EMPTY_TOS -1
#define MIN_STACK_SIZE 20

struct stack_record
{
    unsigned int stack_size;
    int top_of_stack;
    char *stack_array;
};

typedef struct stack_record *STACK;

STACK CreateStack(unsigned int max_elements)
{
    STACK S;
    if (max_elements < MIN_STACK_SIZE)
    {
        printf("\nThe size is too small");
        return NULL;
    }
    S = (STACK)malloc(sizeof(struct stack_record));
    if (S == NULL)
    {
        printf("\nOut of space");
        return NULL;
    }
    S->stack_array = (char *)malloc(sizeof(char) * max_elements);
    if (S->stack_array == NULL)
    {
        printf("\nOut of space");
        return NULL;
    }
    S->stack_size = max_elements;
    S->top_of_stack = EMPTY_TOS;
    return S;
}

int is_valid(char *exp)
{
    int count = 0;
    char c;
    if (exp == NULL)
    {
        printf("\nNull Error");
        return 0;
    }
    c = *exp;
    while (c != 0)
    {
        if ((c > 57 || c < 48) && (c < 40 || c > 43) && c != 45 && c != 47)
        {
            printf("\ninvalid");
            return 0;
        }
        c = *(exp + ++count);
    }
    return 1;
}

int is_empty(STACK S)
{
    if (S == NULL)
    {
        printf("\nError");
        exit(0);
    }
    if (S->top_of_stack == EMPTY_TOS)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_full(STACK S)
{
    if (S != NULL)
    {
        if (S->top_of_stack == S->stack_size - 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

void PushStack(char x, STACK S)
{
    if (is_full(S) == 1)
    {
        printf("\nOut of space");
        exit(0);
    }
    S->stack_array[++S->top_of_stack] = x;
}

char PopStack(STACK S)
{
    if (is_empty(S) == 1)
    {
        printf("\nEmpty stack");
        return 0;
    }
    return S->stack_array[S->top_of_stack--];
}

char TopStack(STACK S)
{
    if (is_empty(S) == 1)
    {
        printf("\nEmpty stack");
        return 0;
    }
    return S->stack_array[S->top_of_stack];
}

void PrintStack(STACK S)
{
    printf("\n%s", S->stack_array);
}

void main()
{
    STACK S, E;
    char c, exp[30];
    int count = 0;
    S = CreateStack(30);
    E = CreateStack(30);
    gets(exp);
    if (is_valid(exp) == 1)
    {
        c = *exp;
        while (c != 0)
        {
            if (c < 58 && c > 47)
                PushStack(c, E);
            else if (c == 40) // (
                PushStack(c, S);
            else if (c == 45 || c == 43) // - and +
            {
                while (!is_empty(S) && TopStack(S) != 40)
                    PushStack(PopStack(S), E);
                PushStack(c, S);
            }
            else if (c == 47 || c == 42) // / and *
            {
                while(!is_empty(S) && (TopStack(S) == 47 || TopStack(S) == 42))
                    PushStack(PopStack(S), E);
                PushStack(c, S);
            }
            else if (c == 41) // )
            {
                while (TopStack(S) != 40)
                    PushStack(PopStack(S), E);
                PopStack(S);
            }
            c = *(exp + ++count);
        }
        while (!is_empty(S))
            PushStack(PopStack(S), E);
        PrintStack(E);
    }
}