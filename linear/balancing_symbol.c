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

void DisposeStack(STACK S)
{
    if (S != NULL)
    {
        free(S->stack_array);
        free(S);
    }
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

void CleanStack(STACK S)
{
    if (S != NULL)
    {
        S->top_of_stack = EMPTY_TOS;
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

void main()
{
    STACK S = CreateStack(30);
    char c, s[30];
    int count = 0;
    printf("Less than 30 chars\n");
    gets(s);
    c = *s;
    while (c != 0)
    {
        if (c == '(' || c == '[' || c == '{')
        {
            PushStack(c, S);
        }
        else if (c == ')')
        {
            if (PopStack(S) != '(')
            {
                printf("\n()match failed");
            }
        }
        else if (c == ']')
        {
            if (PopStack(S) != '[')
            {
                printf("\n[]match failed");
            }
        }
        else if (c == '}')
        {
            if (PopStack(S) != '{')
            {
                printf("\n{}match failed");
            }
        }
        else if (c == '/')
        {
            c = *(s + ++count);
            if (c == 0)
                break;
            else if (c == '*')
                PushStack(c, S);
            else
                count--;
        }
        else if (c == '*')
        {
            c = *(s + ++count);
            if (c == 0)
                break;
            else if (c == '/')
            {
                if (PopStack(S) != '*')
                    printf("\n/**/match failed");
            }
            else
                count--;
        }
        c = *(s + ++count);
    }
    if (!is_empty(S))
        printf("\nmatch failed");
}
