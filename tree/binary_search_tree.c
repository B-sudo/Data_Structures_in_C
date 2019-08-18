#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tree_node *tree_ptr;

struct tree_node
{
    int elem;
    tree_ptr left;
    tree_ptr right;
};

typedef tree_ptr TREE;

tree_ptr InsertTree(int x, TREE T)
{
    tree_ptr hook = T;
    if (T == NULL)
    {
        T = (tree_ptr)malloc(sizeof(struct tree_node));
        T->elem = x;
        T->left = T->right = NULL;
        return T;
    }
    else
    {
        while (1)
        {
            if (x < hook->elem)
            {
                if (hook->left == NULL)
                {
                    hook->left = (tree_ptr)malloc(sizeof(struct tree_node));
                    hook->left->elem = x;
                    hook->left->left = hook->left->right = NULL;
                    return T;
                }
                hook = hook->left;
            }
            else if (x > hook->elem)
            {
                if (hook->right == NULL)
                {
                    hook->right = (tree_ptr)malloc(sizeof(struct tree_node));
                    hook->right->elem = x;
                    hook->right->left = hook->right->right = NULL;
                    return T;
                }
                hook = hook->right;
            }
            else
            {
                printf("\nElement existed");
                return T;
            }
        }
    }
}

tree_ptr find_min(TREE T)
{
    tree_ptr hook = T;
    if (T == NULL)
    {
        printf("\nError");
        return T;
    }
    while (hook->left != NULL)
    {
        hook = hook->left;
    }
    return hook;
}

tree_ptr find_max(TREE T)
{
    tree_ptr hook = T;
    if (T == NULL)
    {
        printf("\nError");
        return T;
    }
    while (hook->right != NULL)
    {
        hook = hook->right;
    }
    return hook;
}

tree_ptr SearchTree(int x, TREE T)
{
    tree_ptr hook = T;
    if (T == NULL)
    {
        printf("\nError");
        return T;
    }
    while (hook != NULL)
    {
        if (x < hook->elem)
            hook = hook->left;
        else if (x > hook->elem)
            hook->right;
        else
            return hook;
    }
    printf("\nNot found");
    return NULL;
}

tree_ptr DeleteTree(int x, TREE T)
{
    tree_ptr tmp_node = T;
    if (T == NULL)
        ;
    else if (x < T->elem)
        T->left = DeleteTree(x, T->left);
    else if (x > T->elem)
        T->right = DeleteTree(x, T->right);
    else if (T->left == NULL)
    {
        T = T->right;
        free(tmp_node);
    }
    else if (T->right == NULL)
    {
        T = T->left;
        free(tmp_node);
    }
    else
    {
        T->elem = find_min(T->right)->elem;
        T->right = DeleteTree(T->elem, T->right);
    }
    return T;
}

void main()
{
    int count;
    TREE T = NULL;
    for (count = 0; count < 10; count++)
    {
        T = InsertTree(rand() % 10, T);
    }
    for (count = 0; count < 10; count++)
    {
        T = DeleteTree(rand() % 10, T);
    }
}
