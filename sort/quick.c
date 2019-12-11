#include<stdio.h>
#include<time.h>
#define N 500

int table[N+1];

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

void Init(int * table)
{
    int count;
    for (count = 1; count < N + 1; count++ )
        table[count] = char2int();
}

void change(int * table, int a, int b)
{
    table[0] = table[a];
    table[a] = table[b];
    table[b] = table[0];
}

void QuickSort(int * table, int left, int right)
{
    int left_temp=left, right_temp=right, temp = left;
    if (right <= left)
        return ;
    while (left_temp != right_temp)
    {
        if (temp == left_temp)      //move the right
        {
            while (table[right_temp] >= table[temp] && right_temp != temp)
                right_temp--;
            change(table, right_temp, left_temp);
            temp = right_temp;
        }
        else
        {
            while (table[left_temp] <= table[temp] && left_temp != temp)
                left_temp++;
            change(table, right_temp, left_temp);
            temp = left_temp;
        }
    }
    QuickSort(table, left, temp - 1);
    QuickSort(table, temp + 1, right);
}

int main()
{
    int count;
    Init(table);
    QuickSort(table, 1, N);
    for (count = 1; count < N; count++)
        printf("%d,", table[count]);
    printf("%d", table[N]);
}