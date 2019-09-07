#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct avl_node *avl_ptr;

struct avl_node
{
    int elem;
    avl_ptr left;
    avl_ptr right;
    int height;
};

typedef avl_ptr AVL;

int max(int a, int b)
{
    return a > b ? a : b;
}
int height(AVL T)
{
    if (T == NULL)
        return -1;
    else
        return T->height;
}

avl_ptr single_rotate_right(AVL T)
{
    avl_ptr hook;
    hook = T->right;
    T->right = hook->left;
    hook->left = T;
    T->height--;
    return hook;
}

avl_ptr single_rotate_left(AVL T)
{
    avl_ptr hook;
    hook = T->left;
    T->left = hook->right;
    hook->right = T;
    T->height--;
    return hook;
}

avl_ptr double_rotate_right(AVL T)
{
    avl_ptr hook;
    hook = T->right->left;
    T->right->left = hook->right;
    hook->right = T->right;
    T->right = hook->left;
    hook->left = T;
    T->height--;
    hook->height++;
    hook->right->height--;
    return hook;
}

avl_ptr double_rotate_left(AVL T)
{
    avl_ptr hook;
    hook = T->left->right;
    T->left->right = hook->left;
    hook->left = T->left;
    T->left = hook->right;
    hook->right = T;
    T->height--;
    hook->height++;
    hook->left->height--;
    return hook;
}

avl_ptr find_min(AVL T)
{
    avl_ptr hook = T;
    if (T == NULL)
        return NULL;
    else
    {
        while (hook->left != NULL)
            hook = hook->left;
        return hook;
    }
}

avl_ptr InsertTree(int x, AVL T)
{
    if (T == NULL)
    {
        T = (avl_ptr)malloc(sizeof(struct avl_node));
        T->elem = x;
        T->left = T->right = NULL;
        T->height = 0;
        return T;
    }
    else if (x == T->elem)
    {
        printf("Element existed\n");
        return T;
    }
    else if (x < T->elem)
    {
        T->left = InsertTree(x, T->left);
        if (height(T->left) > height(T->right) + 1)
        {
            if (height(T->left->left) > height(T->left->right))
                return single_rotate_left(T);
            else if (height(T->left->left) < height(T->left->right))
                return double_rotate_left(T);
            else
            {
                printf("Proof Error");
                return T;
            }
        }
        else
        {
            T->height = max(height(T->left), height(T->right)) + 1;
            return T;
        }
    }
    else if (x > T->elem)
    {
        T->right = InsertTree(x, T->right);
        if (height(T->right) > height(T->left) + 1)
        {
            if (height(T->right->right) > height(T->right->left))
                return single_rotate_right(T);
            else if (height(T->right->right) < height(T->right->left))
                return double_rotate_right(T);
            else
            {
                printf("Proof Error");
                return T;
            }
        }
        else
        {
            T->height = max(height(T->left), height(T->right)) + 1;
            return T;
        }
    }
}

avl_ptr DeleteTree(int x, AVL T)
{
    avl_ptr hook;
    if (T == NULL)
    {
        printf("\nElement does not exist");
        return T;
    }
    else if (x < T->elem)
    {
        T->left = DeleteTree(x, T->left);
        if (height(T->right) > height(T->left) + 1)
        {
            if (height(T->right->right) > height(T->right->left))
            {
                T->height--;
                return single_rotate_right(T);
            }
            else if (height(T->right->right) == height(T->right->left))
                return single_rotate_right(T);
            else if (height(T->right->right) < height(T->right->left))
            {
                T->height--;
                return double_rotate_right(T);
            }
            else
            {
                printf("\nProof Error");
                return T;
            }
        }
        else
        {
            T->height = max(height(T->left), height(T->right)) + 1;
            return T;
        }
    }
    else if (x > T->elem)
    {
        T->right = DeleteTree(x, T->right);
        if (height(T->left) > height(T->right) + 1)
        {
            if (height(T->left->left) > height(T->left->right))
            {
                T->height--;
                return single_rotate_left(T);
            }
            else if (height(T->left->left) == height(T->left->right))
                return single_rotate_left(T);
            else if (height(T->left->left) < height(T->left->right))
            {
                T->height--;
                return double_rotate_left(T);
            }
            else
            {
                printf("\nProof Error");
                return T;
            }
        }
        else
        {
            T->height = max(height(T->left), height(T->right)) + 1;
            return T;
        }
    }
    else
    {
        if (T->right == NULL)
        {
            hook = T->left;
            free(T);
            return hook;
        }
        else if (T->right->left == NULL)
        {
            hook = T->right;
            hook->left = T->left;
            free(T);
            hook->height = max(height(hook->left), height(hook->right)) + 1;
            return hook;
        }
        else
        {
            return DeleteTree(find_min(T)->elem, T);
        }
    }
}

avl_ptr SearchTree(int x, AVL T)
{
    avl_ptr hook = T;
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

void PrintTree(AVL T)
{
    if (T == NULL)
        return;
    else
    {
        PrintTree(T->left);
        printf("\t%d", T->elem);
        PrintTree(T->right);
    }
}

void main()
{
    AVL T = NULL;
    int i;
    for (i = 0; i < 20; i++)
    {
        T = InsertTree(rand() % 20, T);
    }
    PrintTree(T);
    printf("\n");
    for (i = 0; i < 10; i++)
    {    
        T = DeleteTree(rand() % 20, T);
    }
    PrintTree(T);
}
